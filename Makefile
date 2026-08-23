.PHONY: all left right clean clean-all help

# Docker image for ZMK builds
DOCKER_IMAGE := zmkfirmware/zmk-build-arm:stable

# Docker run command with volume mount and user
DOCKER_RUN := docker run --rm -v "$(shell pwd):/app" -w /app --user $(shell id -u):$(shell id -g) $(DOCKER_IMAGE)

# Build directories
BUILD_LEFT := build-left
BUILD_RIGHT := build-right

# Output files
LEFT_UF2 := $(BUILD_LEFT)/zephyr/zmk.uf2
RIGHT_UF2 := $(BUILD_RIGHT)/zephyr/zmk.uf2

all: left right

left:
	@echo "Building left half..."
	$(DOCKER_RUN) bash -c "west zephyr-export && west build -s zmk/app -d $(BUILD_LEFT) -b nice_nano_v2 -- -DSHIELD='lily58_left nice_view_adapter nice_view' -DZMK_CONFIG=/app/config"
	@echo "Left half built: $(LEFT_UF2)"

right:
	@echo "Building right half..."
	$(DOCKER_RUN) bash -c "west zephyr-export && west build -s zmk/app -d $(BUILD_RIGHT) -b nice_nano_v2 -- -DSHIELD='lily58_right nice_view_adapter nice_view' -DZMK_CONFIG=/app/config"
	@echo "Right half built: $(RIGHT_UF2)"

$(LEFT_UF2): .west/config
	@echo "Building left half..."
	$(DOCKER_RUN) bash -c "west zephyr-export && west build -s zmk/app -d $(BUILD_LEFT) -b nice_nano_v2 -- -DSHIELD='lily58_left nice_view_adapter nice_view' -DZMK_CONFIG=/app/config"
	@echo "Left half built: $(LEFT_UF2)"

$(RIGHT_UF2): .west/config
	@echo "Building right half..."
	$(DOCKER_RUN) bash -c "west zephyr-export && west build -s zmk/app -d $(BUILD_RIGHT) -b nice_nano_v2 -- -DSHIELD='lily58_right nice_view_adapter nice_view' -DZMK_CONFIG=/app/config"
	@echo "Right half built: $(RIGHT_UF2)"

.west/config:
	@echo "Initializing ZMK workspace..."
	@if [ ! -d .west ]; then \
		$(DOCKER_RUN) bash -c "west init -l config"; \
	fi
	@if [ ! -d zmk ] || [ ! -d zephyr ]; then \
		$(DOCKER_RUN) bash -c "west update --fetch-opt=--filter=tree:0"; \
	fi
	@touch .west/config
	@echo "ZMK workspace initialized"

init: .west/config

workspace: .west/config

clean:
	@echo "Cleaning build directories..."
	rm -rf $(BUILD_LEFT) $(BUILD_RIGHT)
	@echo "Build directories cleaned"

clean-all:
	@echo "Cleaning all generated files..."
	rm -rf build build-left build-right .west zmk zephyr modules lily58_left.uf2 lily58_right.uf2
	@echo "All generated files cleaned"

copy: all
	@echo "Copying firmware files..."
	@cp $(LEFT_UF2) lily58_left.uf2
	@cp $(RIGHT_UF2) lily58_right.uf2
	@echo "Firmware files copied:"
	@ls -lh lily58_left.uf2 lily58_right.uf2 | awk '{print "  " $$9 "  (" $$5 ")"}'

help:
	@echo "ZMK Lily58 Build System"
	@echo ""
	@echo "Available targets:"
	@echo "  all        - Build both left and right halves"
	@echo "  left       - Build left half only"
	@echo "  right      - Build right half only"
	@echo "  copy       - Build and copy firmware files to current directory"
	@echo "  init       - Initialize ZMK workspace (run once after clean-all)"
	@echo "  clean      - Clean build directories"
	@echo "  clean-all  - Clean all generated files (requires init after)"
	@echo "  help       - Show this help message"
	@echo ""
	@echo "Output files:"
	@echo "  Left:  $(LEFT_UF2)"
	@echo "  Right: $(RIGHT_UF2)"