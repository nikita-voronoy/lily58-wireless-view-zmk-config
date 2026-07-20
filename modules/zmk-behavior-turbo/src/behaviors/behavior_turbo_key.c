/*
 * Copyright (c) 2026
 * SPDX-License-Identifier: MIT
 *
 * Turbo / auto-fire: while the key is held, send discrete press/release
 * cycles of the bound keycode. Stops immediately on physical release.
 * Tap length and inter-press wait can be fixed or random within a range.
 */

#define DT_DRV_COMPAT zmk_behavior_turbo_key

#include <zephyr/device.h>
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>

#include <drivers/behavior.h>
#include <zmk/behavior.h>
#include <zmk/event_manager.h>
#include <zmk/events/keycode_state_changed.h>

LOG_MODULE_DECLARE(zmk, CONFIG_ZMK_LOG_LEVEL);

#if DT_HAS_COMPAT_STATUS_OKAY(DT_DRV_COMPAT)

#define ZMK_BHV_TURBO_MAX_HELD CONFIG_ZMK_BEHAVIOR_TURBO_KEY_MAX_HELD

struct behavior_turbo_key_config {
    uint32_t tap_ms_min;
    uint32_t tap_ms_max;
    uint32_t wait_ms_min;
    uint32_t wait_ms_max;
};

struct active_turbo {
    bool active;
    bool key_down;
    bool timer_cancelled;
    uint32_t position;
    uint32_t param1;
#if IS_ENABLED(CONFIG_ZMK_SPLIT)
    uint8_t source;
#endif
    const struct behavior_turbo_key_config *config;
    struct k_work_delayable timer;
};

static struct active_turbo active_turbos[ZMK_BHV_TURBO_MAX_HELD];

/* Lightweight PRNG so we don't depend on CONFIG_ENTROPY / sys_rand32_get */
static uint32_t rng_state;

static uint32_t next_rand(void) {
    uint32_t x = rng_state;
    if (x == 0) {
        x = k_cycle_get_32();
        if (x == 0) {
            x = 0xA5A5A5A5u;
        }
    }
    /* xorshift32 */
    x ^= x << 13;
    x ^= x >> 17;
    x ^= x << 5;
    rng_state = x;
    return x;
}

static uint32_t rand_range_ms(uint32_t min_ms, uint32_t max_ms) {
    if (max_ms <= min_ms) {
        return min_ms;
    }
    uint32_t span = max_ms - min_ms + 1;
    return min_ms + (next_rand() % span);
}

static struct active_turbo *find_turbo(uint32_t position) {
    for (int i = 0; i < ZMK_BHV_TURBO_MAX_HELD; i++) {
        if (active_turbos[i].active && active_turbos[i].position == position &&
            !active_turbos[i].timer_cancelled) {
            return &active_turbos[i];
        }
    }
    return NULL;
}

static struct active_turbo *store_turbo(struct zmk_behavior_binding_event *event, uint32_t param1,
                                        const struct behavior_turbo_key_config *config) {
    for (int i = 0; i < ZMK_BHV_TURBO_MAX_HELD; i++) {
        struct active_turbo *t = &active_turbos[i];
        if (t->active || t->timer_cancelled) {
            continue;
        }
        t->active = true;
        t->key_down = false;
        t->timer_cancelled = false;
        t->position = event->position;
        t->param1 = param1;
        t->config = config;
#if IS_ENABLED(CONFIG_ZMK_SPLIT)
        t->source = event->source;
#endif
        return t;
    }
    return NULL;
}

static void clear_turbo(struct active_turbo *t) {
    t->active = false;
    t->key_down = false;
    t->position = 0;
    t->param1 = 0;
    t->config = NULL;
}

static int press_key(struct active_turbo *t, int64_t timestamp) {
    t->key_down = true;
    return raise_zmk_keycode_state_changed_from_encoded(t->param1, true, timestamp);
}

static int release_key(struct active_turbo *t, int64_t timestamp) {
    if (!t->key_down) {
        return 0;
    }
    t->key_down = false;
    return raise_zmk_keycode_state_changed_from_encoded(t->param1, false, timestamp);
}

static void schedule_next(struct active_turbo *t, uint32_t delay_ms) {
    k_work_schedule(&t->timer, K_MSEC(delay_ms));
}

static void turbo_timer_handler(struct k_work *item) {
    struct k_work_delayable *d_work = k_work_delayable_from_work(item);
    struct active_turbo *t = CONTAINER_OF(d_work, struct active_turbo, timer);

    if (t->timer_cancelled) {
        t->timer_cancelled = false;
        if (!t->active) {
            clear_turbo(t);
        }
        return;
    }

    if (!t->active) {
        return;
    }

    int64_t now = k_uptime_get();
    const struct behavior_turbo_key_config *cfg = t->config;

    if (t->key_down) {
        /* End of tap: release, then random wait before next press */
        release_key(t, now);
        schedule_next(t, rand_range_ms(cfg->wait_ms_min, cfg->wait_ms_max));
    } else {
        /* Start of next tap with random hold duration */
        press_key(t, now);
        schedule_next(t, rand_range_ms(cfg->tap_ms_min, cfg->tap_ms_max));
    }
}

static int stop_timer(struct active_turbo *t) {
    int result = k_work_cancel_delayable(&t->timer);
    if (result == -EINPROGRESS) {
        t->timer_cancelled = true;
    }
    return result;
}

static int on_turbo_binding_pressed(struct zmk_behavior_binding *binding,
                                    struct zmk_behavior_binding_event event) {
    const struct device *dev = zmk_behavior_get_binding(binding->behavior_dev);
    const struct behavior_turbo_key_config *cfg = dev->config;

    /* Reseed a bit on each press so sessions don't look identical */
    rng_state ^= (uint32_t)event.timestamp ^ k_cycle_get_32() ^ (event.position << 16);

    struct active_turbo *existing = find_turbo(event.position);
    if (existing != NULL) {
        stop_timer(existing);
        release_key(existing, event.timestamp);
        clear_turbo(existing);
    }

    struct active_turbo *t = store_turbo(&event, binding->param1, cfg);
    if (t == NULL) {
        LOG_ERR("Unable to store turbo key (max %d held)", ZMK_BHV_TURBO_MAX_HELD);
        return ZMK_BEHAVIOR_OPAQUE;
    }

    LOG_DBG("turbo press pos %d key 0x%02X", event.position, binding->param1);
    press_key(t, event.timestamp);
    schedule_next(t, rand_range_ms(cfg->tap_ms_min, cfg->tap_ms_max));

    return ZMK_BEHAVIOR_OPAQUE;
}

static int on_turbo_binding_released(struct zmk_behavior_binding *binding,
                                     struct zmk_behavior_binding_event event) {
    struct active_turbo *t = find_turbo(event.position);
    if (t == NULL) {
        LOG_DBG("turbo release with no active instance at pos %d", event.position);
        return ZMK_BEHAVIOR_OPAQUE;
    }

    LOG_DBG("turbo release pos %d", event.position);
    stop_timer(t);
    release_key(t, event.timestamp);

    if (t->timer_cancelled) {
        /* Timer handler will finish cleanup */
        t->active = false;
    } else {
        clear_turbo(t);
    }

    return ZMK_BEHAVIOR_OPAQUE;
}

#if IS_ENABLED(CONFIG_ZMK_BEHAVIOR_METADATA)

static const struct behavior_parameter_value_metadata param_values[] = {
    {
        .display_name = "Key",
        .type = BEHAVIOR_PARAMETER_VALUE_TYPE_HID_USAGE,
    },
};

static const struct behavior_parameter_metadata_set param_metadata_set[] = {{
    .param1_values = param_values,
    .param1_values_len = ARRAY_SIZE(param_values),
}};

static const struct behavior_parameter_metadata metadata = {
    .sets_len = ARRAY_SIZE(param_metadata_set),
    .sets = param_metadata_set,
};

#endif // IS_ENABLED(CONFIG_ZMK_BEHAVIOR_METADATA)

static const struct behavior_driver_api behavior_turbo_key_driver_api = {
    .binding_pressed = on_turbo_binding_pressed,
    .binding_released = on_turbo_binding_released,
#if IS_ENABLED(CONFIG_ZMK_BEHAVIOR_METADATA)
    .parameter_metadata = &metadata,
#endif
};

static int behavior_turbo_key_init(const struct device *dev) {
    static bool init_first_run = true;
    if (init_first_run) {
        rng_state = k_cycle_get_32();
        for (int i = 0; i < ZMK_BHV_TURBO_MAX_HELD; i++) {
            k_work_init_delayable(&active_turbos[i].timer, turbo_timer_handler);
            active_turbos[i].active = false;
            active_turbos[i].timer_cancelled = false;
        }
    }
    init_first_run = false;
    return 0;
}

#define TK_INST(n)                                                                                 \
    static const struct behavior_turbo_key_config behavior_turbo_key_config_##n = {                \
        .tap_ms_min = DT_INST_PROP(n, tap_ms_min),                                                 \
        .tap_ms_max = DT_INST_PROP(n, tap_ms_max),                                                 \
        .wait_ms_min = DT_INST_PROP(n, wait_ms_min),                                               \
        .wait_ms_max = DT_INST_PROP(n, wait_ms_max),                                               \
    };                                                                                             \
    BEHAVIOR_DT_INST_DEFINE(n, behavior_turbo_key_init, NULL, NULL,                                \
                            &behavior_turbo_key_config_##n, POST_KERNEL,                           \
                            CONFIG_KERNEL_INIT_PRIORITY_DEFAULT, &behavior_turbo_key_driver_api);

DT_INST_FOREACH_STATUS_OKAY(TK_INST)

#endif /* DT_HAS_COMPAT_STATUS_OKAY */
