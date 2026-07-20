if ((NOT CONFIG_ZMK_SPLIT) OR CONFIG_ZMK_SPLIT_ROLE_CENTRAL)
  target_sources_ifdef(CONFIG_ZMK_BEHAVIOR_TURBO_KEY app PRIVATE src/behaviors/behavior_turbo_key.c)
endif()
