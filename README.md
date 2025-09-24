# Lily58 Wireless ZMK Config with Nice!View

ZMK firmware configuration for Lily58 split keyboard with Nice!Nano controllers and Nice!View displays.

## Layout Visualization

### Base Layer (0)
```
┌─────┬─────┬─────┬─────┬─────┬─────┐                 ┌─────┬─────┬─────┬─────┬─────┬───────┐
│  `  │  1  │  2  │  3  │  4  │  5  │                 │  6  │  7  │  8  │  9  │  0  │ BKSPC │
├─────┼─────┼─────┼─────┼─────┼─────┤                 ├─────┼─────┼─────┼─────┼─────┼───────┤
│ TAB │  Q  │  W  │  E  │  R  │  T  │                 │  Y  │  U  │  I  │  O  │  P  │   -   │
├─────┼─────┼─────┼─────┼─────┼─────┤                 ├─────┼─────┼─────┼─────┼─────┼───────┤
│ ESC │  A  │  S  │  D  │  F  │  G  │                 │  H  │  J  │  K  │  L  │  ;  │   '   │
├─────┼─────┼─────┼─────┼─────┼─────┼─────┐     ┌─────┼─────┼─────┼─────┼─────┼─────┼───────┤
│SHIFT│  Z  │  X  │  C  │  V  │  B  │     │     │     │  N  │  M  │  ,  │  .  │  /  │ SHIFT │
└─────┴─────┴─────┼─────┼─────┼─────┼─────┤     ├─────┼─────┼─────┼─────┼─────┴─────┴───────┘
                  │CTRL │ GUI │ ALT │SPACE│     │ RET │ L1  │ L2  │ L3  │
                  └─────┴─────┴─────┴─────┘     └─────┴─────┴─────┴─────┘
```

### Extra Layer (1)
```
┌─────┬─────┬─────┬─────┬─────┬─────┐                 ┌─────┬─────┬─────┬─────┬─────┬───────┐
│     │     │     │     │     │     │                 │     │     │     │     │     │  DEL  │
├─────┼─────┼─────┼─────┼─────┼─────┤                 ├─────┼─────┼─────┼─────┼─────┼───────┤
│     │     │     │     │     │     │                 │     │     │     │     │PRTSC│       │
├─────┼─────┼─────┼─────┼─────┼─────┤                 ├─────┼─────┼─────┼─────┼─────┼───────┤
│     │     │     │     │     │     │                 │  -  │  =  │  {  │  }  │  |  │       │
├─────┼─────┼─────┼─────┼─────┼─────┼─────┐     ┌─────┼─────┼─────┼─────┼─────┼─────┼───────┤
│     │     │     │     │     │     │     │     │     │  _  │  +  │  [  │  ]  │  \  │       │
└─────┴─────┴─────┼─────┼─────┼─────┼─────┤     ├─────┼─────┼─────┼─────┼─────┴─────┴───────┘
                  │     │     │     │     │     │     │     │     │     │
                  └─────┴─────┴─────┴─────┘     └─────┴─────┴─────┴─────┘
```

### Arrows Layer (2)
```
┌─────┬─────┬─────┬─────┬─────┬─────┐                 ┌─────┬─────┬─────┬─────┬─────┬───────┐
│     │     │     │     │     │     │                 │     │     │     │     │     │       │
├─────┼─────┼─────┼─────┼─────┼─────┤                 ├─────┼─────┼─────┼─────┼─────┼───────┤
│     │     │     │     │     │     │                 │     │     │ INS │     │     │       │
├─────┼─────┼─────┼─────┼─────┼─────┤                 ├─────┼─────┼─────┼─────┼─────┼───────┤
│     │     │     │     │     │     │                 │  ←  │  ↓  │  ↑  │  →  │     │       │
├─────┼─────┼─────┼─────┼─────┼─────┼─────┐     ┌─────┼─────┼─────┼─────┼─────┼─────┼───────┤
│     │     │     │     │     │     │     │     │     │     │     │     │     │     │       │
└─────┴─────┴─────┼─────┼─────┼─────┼─────┤     ├─────┼─────┼─────┼─────┼─────┴─────┴───────┘
                  │     │     │     │     │     │     │     │     │     │
                  └─────┴─────┴─────┴─────┘     └─────┴─────┴─────┴─────┘
```

### Function Layer (3)
```
┌─────┬─────┬─────┬─────┬─────┬─────┐                 ┌─────┬─────┬─────┬─────┬─────┬───────┐
│ F1  │ F2  │ F3  │ F4  │ F5  │ F6  │                 │ F7  │ F8  │ F9  │ F10 │ F11 │  F12  │
├─────┼─────┼─────┼─────┼─────┼─────┤                 ├─────┼─────┼─────┼─────┼─────┼───────┤
│     │     │     │     │     │     │                 │     │     │     │     │     │       │
├─────┼─────┼─────┼─────┼─────┼─────┤                 ├─────┼─────┼─────┼─────┼─────┼───────┤
│BTCLR│     │     │     │     │     │                 │PREV │VOL- │VOL+ │NEXT │     │       │
├─────┼─────┼─────┼─────┼─────┼─────┼─────┐     ┌─────┼─────┼─────┼─────┼─────┼─────┼───────┤
│     │     │     │     │     │     │ BT0 │     │ BT1 │     │     │     │     │     │       │
└─────┴─────┴─────┼─────┼─────┼─────┼─────┤     ├─────┼─────┼─────┼─────┼─────┴─────┴───────┘
                  │     │     │     │     │     │PLAY │     │     │     │
                  └─────┴─────┴─────┴─────┘     └─────┴─────┴─────┴─────┘
```

## Hardware

- **Microcontroller**: Nice!Nano v2
- **Display**: Nice!View
- **Battery**: Compatible with 301230 Li-Po batteries
- **Features**:
  - Wireless connectivity via Bluetooth
  - OLED display support
  - Deep sleep mode for extended battery life
  - Hot-swappable switches (depending on PCB variant)

## Build Instructions

### Prerequisites

- Docker installed and running
- Make sure you have proper permissions to run Docker

### Quick Build

```bash
# Build both halves
make all

# Build individual halves
make left   # Build left half only
make right  # Build right half only

# Copy firmware to current directory
make copy   # Copies to lily58_left.uf2 and lily58_right.uf2
```

### Firmware Locations

After building, firmware files are located at:
- Left half: `build-left/zephyr/zmk.uf2`
- Right half: `build-right/zephyr/zmk.uf2`

### Flashing Instructions

1. Put your Nice!Nano into bootloader mode (double-tap reset button)
2. Copy the appropriate `.uf2` file to the USB drive that appears
3. The controller will automatically reboot with the new firmware

### Available Make Targets

- `make all` - Build both keyboard halves
- `make left` - Build left half only
- `make right` - Build right half only
- `make copy` - Build and copy firmware files to project root
- `make clean` - Remove build directories
- `make clean-all` - Remove all generated files (requires `make init` after)
- `make init` - Initialize ZMK workspace (run once after `clean-all`)
- `make help` - Show available commands

## Configuration

Key configuration is defined in `config/lily58.keymap`. The layout includes:

- **Base layer**: Standard QWERTY layout with modifiers
- **Extra layer**: Symbols and punctuation
- **Arrows layer**: Navigation keys and insert
- **Function layer**: Function keys, media controls, and Bluetooth management

### Bluetooth Pairing

- Clear all pairings: Function layer + BT CLR (bottom left)
- Select profile 0: Function layer + left thumb cluster encoder
- Select profile 1: Function layer + right thumb cluster encoder

## Customization

To modify the keymap:

1. Edit `config/lily58.keymap`
2. Run `make all` to build new firmware
3. Flash updated firmware to both halves

For advanced configuration options, refer to the [ZMK documentation](https://zmk.dev/docs).

## Troubleshooting

### Build Issues
- Ensure Docker is running and you have proper permissions
- Try `make clean-all && make init && make all` for a fresh build

### Bluetooth Issues
- Clear all Bluetooth pairings using the BT CLR key
- Re-pair both halves with your device
- Ensure both halves are charged

### Display Issues
- Verify Nice!View is properly seated in its socket
- Check that the display configuration matches your hardware

## License

This configuration is based on the ZMK firmware project and follows the MIT license.