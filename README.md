# motionController
# Hand-Motion Drone Controller (MPU6050 + Arduino)

This project reads hand movement using an **MPU6050 IMU** (accelerometer + gyroscope) and converts it into simple direction commands intended to control a drone.  
Right now, the output is **printed to the Serial Monitor** as: `Left`, `Right`, `Forwards`, `Backwards`, or `Center`.

## What it does

- On boot, the controller:
  - Initializes the MPU6050 over I2C
  - Takes an initial **“center” calibration** by averaging raw accelerometer readings
- In the main loop:
  - Continuously reads accelerometer values
  - Computes change from the calibrated center
  - Determines which direction you moved your hand based on thresholds
  - Prints the detected direction to Serial
- A **recenter button** (digital pin 2) can be pressed to take a new “center” average at any time.

## Hardware needed

- Arduino (Uno/Nano both work)
- MPU6050 module
- Push button (for recenter)
- Jumper wires
- (Optional) glove/strap/mount to attach MPU6050 to your hand

## Wiring

### MPU6050 → Arduino (I2C)
**Arduino Uno / Nano:**
- `VCC` → `5V` (or `3.3V` if your MPU6050 board requires it)
- `GND` → `GND`
- `SDA` → `A4`
- `SCL` → `A5`

> Many MPU6050 breakout boards have a regulator and can take 5V. If yours is a bare sensor board, use 3.3V only.

### Recenter Button → Arduino
- One side of button → `5V`
- Other side of button → `D2`
- Add a **10kΩ pulldown resistor** from `D2` → `GND`

**Alternative (simpler):** use the internal pullup  
If you change the code to `pinMode(2, INPUT_PULLUP);`, then wire:
- One side of button → `D2`
- Other side of button → `GND`
…and treat pressed as `LOW`.

## Software setup

### Libraries
This code uses:
- `Wire.h` (built-in)
- `MPU6050.h` (MPU6050 library)

Install the MPU6050 library in Arduino IDE:
- **Tools → Manage Libraries…**
- Search: **MPU6050**
- Install the library that matches your include (`#include <MPU6050.h>`)

### Upload
1. Open the `.ino` in Arduino IDE
2. Select your board (Uno/Nano) and correct port
3. Upload
4. Open Serial Monitor at **9600 baud**

You should see:
- `BOOT`
- `MPU init done`
- `Average`
- then direction outputs

## How direction detection works

On first run (trial 1), we compute a baseline:
- Collect `N = 80` accelerometer readings
- Average them → `(sX, sY, sZ)` which becomes your **center**

Each loop after that:
- Read the new accelerometer values `(nX, nY, nZ)`
- Compute changes:
  - `changeX = nX - sX`
  - `changeY = nY - sY`
  - `changeZ = nZ - sZ` (currently not used for direction)

Then we decide direction using thresholds:
- **Left / Right** determined mainly from **X**:
  - `changeX > 4000` → `Left`
  - `changeX < -4000` → `Right`
- **Forwards / Backwards** determined mainly from **Y**:
  - `changeY > 500` → `Backwards`
  - `changeY < -800` → `Forwards`
- If neither is strong enough → `Center`

To avoid mixing axes, it compares magnitudes and uses whichever axis dominates:
- If `abs(changeX) > abs(changeY)` → use X decision
- If `abs(changeY) > abs(changeX)` → use Y decision

## Re-centering (calibration button)

If `digitalRead(2) == HIGH`, the controller prints `New Center` and re-runs the averaging step to set a new `(sX, sY, sZ)` baseline.

This is useful if:
- Your hand position changes
- The sensor shifts on your glove
- You want a new neutral orientation

## Notes / Known limitations

- This version only prints direction to Serial — it does **not** send commands to a drone yet.
- It uses **raw accelerometer values** (no filtering), so fast jitter may cause noisy outputs.
- Thresholds are tuned to one setup; you may need to adjust them depending on:
  - sensor mounting
  - hand angle range
  - board orientation
- `changeZ` is computed but not used yet (could be used for throttle/up/down).

## Next steps (planned)

Ideas to turn this into an actual drone controller:
- Add **wireless transmitter** (nRF24L01, ESP-NOW, Bluetooth, etc.)
- Convert direction into control channels (throttle / yaw / pitch / roll)
- Add smoothing:
  - moving average / low-pass filter
  - deadzone tuning
- Add arming/disarming and failsafe behavior

## License

MEEEEEEEE
