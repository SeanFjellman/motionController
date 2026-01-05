#include <Wire.h>
#include <MPU6050.h>

MPU6050 mpu;

void setup() {
  Serial.begin(9600);
  delay(500);
  Serial.println("BOOT");
  Wire.begin();

  mpu.initialize();
  Serial.println("MPU init done");
  pinMode(2, INPUT);
}

int16_t trials = 1;
int16_t sX, sY, sZ, nX, nY, nZ;

float gX = 0, gY = 0, gZ = 0;

float liftFilt = 0.0f;
enum LiftState { LIFT_DOWN, LIFT_UP };
LiftState liftState = LIFT_DOWN;

unsigned long lastLiftMs = 0;
const unsigned long LIFT_COOLDOWN = 400;

void loop() {
  int16_t ax, ay, az;
  int16_t gx, gy, gz;

  int32_t sumX = 0, sumY = 0, sumZ = 0;
  const int N = 80;

  if (trials == 1) {
    Serial.println("Average");

    for (int i = 0; i < N; i++) {
      mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
      sumX += ax; sumY += ay; sumZ += az;
      delay(5);
    }

    sX = sumX / N;
    sY = sumY / N;
    sZ = sumZ / N;

    Serial.print(sX); Serial.print(", ");
    Serial.print(sY); Serial.print(", ");
    Serial.println(sZ);

    gX = (float)sX;
    gY = (float)sY;
    gZ = (float)sZ;

    trials++;
    return;
  }

  mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
  nX = ax; nY = ay; nZ = az;

  int16_t changeX = nX - sX;
  int16_t changeY = nY - sY;
  int16_t changeZ = nZ - sZ;

  bool moved = false;

  if (abs(changeX) > abs(changeY)) {
    if (changeX > 4000) { Serial.print("Left "); moved = true; }
    else if (changeX < -4000) { Serial.print("Right "); moved = true; }
  }

  if (abs(changeY) > abs(changeX)) {
    if (changeY > 500) { Serial.print("Backwards"); moved = true; }
    else if (changeY < -800) { Serial.print("Forwards"); moved = true; }
  }

  if (!moved) {
    const float alphaG = 0.90f;
    gX = alphaG * gX + (1.0f - alphaG) * (float)nX;
    gY = alphaG * gY + (1.0f - alphaG) * (float)nY;
    gZ = alphaG * gZ + (1.0f - alphaG) * (float)nZ;

    float linX = (float)nX - gX;
    float linY = (float)nY - gY;
    float linZ = (float)nZ - gZ;

    float gMag = sqrt(gX*gX + gY*gY + gZ*gZ);
    if (gMag < 1.0f) gMag = 1.0f;
    float ux = gX / gMag, uy = gY / gMag, uz = gZ / gMag;

    float lift = linX*ux + linY*uy + linZ*uz;

    liftFilt = 0.7f * liftFilt + 0.3f * lift;

    Serial.print("  liftFilt=");
    Serial.print(liftFilt);

    const float UP_THR   = 900.0f;
    const float DOWN_THR = -900.0f;
    unsigned long now = millis();

    if (now - lastLiftMs > LIFT_COOLDOWN) {
      if (liftState == LIFT_DOWN && liftFilt > UP_THR) {
        liftState = LIFT_UP;
        Serial.print("  ARM_UP");
        lastLiftMs = now;
      } else if (liftState == LIFT_UP && liftFilt < DOWN_THR) {
        liftState = LIFT_DOWN;
        Serial.print("  ARM_DOWN");
        lastLiftMs = now;
      }
    }
  }

  if (!moved) Serial.print("Center");
  Serial.println();

  if (digitalRead(2) == HIGH) {
    Serial.println("New Center");
    sumX = 0; sumY = 0; sumZ = 0;

    for (int i = 0; i < N; i++) {
      mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
      sumX += ax; sumY += ay; sumZ += az;
      delay(5);
    }

    sX = sumX / N;
    sY = sumY / N;
    sZ = sumZ / N;

    Serial.print(sX); Serial.print(", ");
    Serial.print(sY); Serial.print(", ");
    Serial.println(sZ);

    gX = (float)sX;
    gY = (float)sY;
    gZ = (float)sZ;

    liftFilt = 0;
    liftState = LIFT_DOWN;
    lastLiftMs = 0;
  }

  delay(200);
}
