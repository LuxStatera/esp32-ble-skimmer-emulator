# ESP32 BLE Card Skimmer Emulator

A simple BLE advertising tool for the ESP32 WROOM-32 that broadcasts known card skimmer device names over Bluetooth Low Energy. Built to test BLE skimmer detectors without needing an actual skimmer module.

Place the emulator on a desk, connect your skimmer detector to a powerbank, and walk toward it to verify proximity alerts, screen flashing, and LED color sweeps are all working correctly.

## What It Does

On boot the ESP32 begins advertising as `HC-05` — a Bluetooth module name commonly associated with card skimmers. Two buttons let you interact:

| Button | GPIO | Function |
|--------|------|----------|
| **Cycle Name** | D13 | Cycles through 8 known skimmer names |
| **Toggle Advert** | D32 | Turns BLE advertising on / off |

The built-in blue LED stays **on** while advertising and turns **off** when stopped.

### Advertised Names

Each press of the Cycle button steps through this list:

```
HC-05  ->  HC-06  ->  HC-08  ->  BT05  ->  JDY-31  ->  HM-10  ->  CC41A  ->  SPP-CA
```

These are common Bluetooth module names associated with card skimmers. Any BLE skimmer detector that flags these names will trigger when this emulator is in range.

## Hardware

- **ESP32 WROOM-32** dev board (any variant with BLE support)
- **2 momentary push buttons** (normally open)
- **2 jumper wires** (or direct soldering)

No resistors needed — the ESP32's internal pull-up resistors are enabled in firmware.

## Wiring

Each button has two legs. One leg connects to a GPIO pin, the other to GND. Both buttons share the same GND pin on the ESP32.

```
Button A (Cycle Name)          Button B (Toggle Advert)
┌─────────┐                    ┌─────────┐
│         │                    │         │
│  ┌───┐  │                    │  ┌───┐  │
│  │   │  │                    │  │   │  │
│  └─┬─┘  │                    │  └─┬─┘  │
│    │    │                    │    │    │
└──┬─┴──┘                     └──┬─┴──┘
   │   │                         │   │
   │   │                         │   │
  D13  GND                      D32  GND
```

### Pin connections

| Wire | From | To |
|------|------|----|
| 1 | Button A — leg 1 | **D13** on ESP32 |
| 2 | Button A — leg 2 | **GND** on ESP32 |
| 3 | Button B — leg 1 | **D32** on ESP32 |
| 4 | Button B — leg 2 | **GND** on ESP32 |

If both buttons are near the same GND pin, you can daisy-chain: run one wire from GND to Button A leg 2, then jump from there to Button B leg 2.

### Quick reference pinout

```
ESP32 WROOM-32

        ┌──────────┐
        │   USB    │
        ├──────────┤
   D13 ─┤          ├─
        │          │
        │          │
   D32 ─┤          ├─
        │          │
   GND ─┤          ├─
        │          │
        └──────────┘

D13 = Button A (cycle skimmer name)
D32 = Button B (toggle advertising on/off)
GND = Shared ground for both buttons
```

## Flashing

### PlatformIO (recommended)

1. Clone the repo
2. Open in VS Code with PlatformIO or use the CLI
3. Update `upload_port` and `monitor_port` in `platformio.ini` to match your board's serial port
4. Flash:

```bash
pio run --target upload
```

5. Open serial monitor to see status:

```bash
pio device monitor
```

### Finding your serial port

**macOS:**
```bash
ls /dev/cu.usb*
```

**Linux:**
```bash
ls /dev/ttyUSB*
```

**Windows:**
Check Device Manager under "Ports (COM & LPT)" for the COM port number.

## Serial Output

The serial monitor (115200 baud) shows the current state:

```
=== Skimmer BLE Simulator ===
D13 (WiFi button): cycle skimmer name
D32 (BLE button):  toggle advertising on/off

Advertising as: "HC-05"
Advertising as: "HC-06"       <- after pressing Cycle
Advertising stopped            <- after pressing Toggle
Advertising as: "HC-06"       <- after pressing Toggle again
```

## Customizing

### Change button pins

Edit the defines at the top of `src/main.cpp`:

```cpp
#define BTN_A  13  // change to your GPIO
#define BTN_B  32  // change to your GPIO
```

### Add or change skimmer names

Edit the `NAMES` array in `src/main.cpp`:

```cpp
static const char* NAMES[] = {
    "HC-05",
    "HC-06",
    // add your own names here
};
```

## Notes

- The ESP32's MAC address stays the same when you cycle names. Detectors that count unique devices by MAC will only count this as 1 skimmer regardless of name changes.
- BLE advertising range is typically 10-30 meters depending on environment, making it easy to test proximity-based alerts by walking closer or farther from the emulator.
- The blue LED on GPIO 2 is the onboard LED found on most ESP32 dev boards. If yours is on a different pin, change `#define LED 2` in the source.