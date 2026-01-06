# motionController

Hand-motion controller using an **Arduino + MPU6050 IMU**.  
Right now it prints simple commands to the Serial Monitor:

- `Left`, `Right`
- `Forwards`, `Backwards`
- `Move Up`, `Move Down`

This is a temporary input method for a controller project (eventually intended to control something like a drone).

---

## What it does

### Startup / Calibration
- Initializes the MPU6050 over I2C
- Takes a **baseline “center”** reading by averaging `N = 80` accelerometer samples
- Prints the baseline as: `sX, sY, sZ`

### Main Loop
Every loop:
1. Reads accelerometer values `(nX, nY, nZ)`
2. Computes delta from baseline:
   - `changeX = nX - sX`
   - `changeY = nY - sY`
   - `changeZ = nZ - sZ` *(computed but not used right now)*
3. Determines direction based on thresholds:
   - **Left / Right** from `changeX`
   - **Forwards / Backwards** from `changeY` *(with a “hold” requirement)*
   - **Move Up / Move Down** from large `changeY` values

### Recenter Button
- Pressing the button on **D2** triggers a new baseline calibration.

---

## Hardware

- Arduino Uno or Nano
- MPU6050 module
- Push button (recenter)
- Jumper wires

---

## Wiring

### MPU6050 → Arduino (I2C)
**Uno / Nano**
- `VCC` → `5V` *(or `3.3V` if required by your breakout)*
- `GND` → `GND`
- `SDA` → `A4`
- `SCL` → `A5`

### Recenter Button → Arduino (D2)
Current code uses `pinMode(2, INPUT);`, so wire:
- One side of button → `5V`
- Other side of button → `D2`
- Add a **10kΩ pulldown resistor** from `D2` → `GND`

> Optional improvement: use `INPUT_PULLUP` in code and wire the button to `GND` instead.

---

## Software Setup

### Libraries
- `Wire.h` *(built-in)*
- `MPU6050.h` *(install via Arduino Library Manager)*

In Arduino IDE:
- **Tools → Manage Libraries…**
- Search for **MPU6050**
- Install a library that matches: `#include <MPU6050.h>`

### Upload / Serial Monitor
- Upload the sketch
- Open Serial Monitor at **9600 baud**
- Expected startup output:
  - `BOOT`
  - `MPU init done`
  - `Average:`
  - baseline values like `-2190, 668, 17766`

---

## How Movement Detection Works

### Baseline (Center)
On the first run, the code averages 80 samples and stores:
- `sX, sY, sZ`

### Left / Right (X axis)
The code checks X only when it dominates Y:
- If `abs(changeX) > abs(changeY)`:
  - `changeX > 4000` → `Left`
  - `changeX < -4000` → `Right`

### Forwards / Backwards (Y axis) with “Hold”
To prevent accidental forward/back while lifting:
- It requires the movement to persist for a few cycles before printing:
  - `trials > 3` before allowing Forward/Backward
- Thresholds:
  - `changeY > 500` and `< 15000` → `Backwards`
  - `changeY < -800` and `> -14000` → `Forwards`

After printing Forward/Backward, the code resets `trials = 1`.

### Up / Down (Large Y change)
These trigger on large Y values:
- `changeY > 15000` → `Move Up`
- `changeY < -14000` → `Move Down`

> Note: Up/Down are currently based on the same axis as Forward/Backward (Y), so depending on how the sensor is mounted you may need to swap axes or adjust thresholds.

---

## Known Limitations

- Uses **raw accelerometer values** (no filtering), so noise/jitter can happen.
- Up/Down vs Forward/Backward can still overlap depending on mounting orientation.
- Thresholds are setup-specific (mounting, hand position, sensor orientation).

---

## Next Steps (Planned)

- Add filtering (low-pass / moving average)
- Add wireless output (nRF24L01, ESP-NOW, Bluetooth, etc.)
- Map commands into real control channels (throttle / pitch / roll / yaw)
- Add safety (arming/disarming, failsafe)

---

## License
TBD
