#include <Wire.h>
#include <MPU6050.h>

MPU6050 mpu;

void setup() {
  Serial.begin(115200);
  Wire.begin();

//   Serial.println("Initializing MPU6050...");
  mpu.initialize();

//   if (mpu.testConnection()) {
//     Serial.println("MPU6050 connection successful");
//   } else {
//     Serial.println("MPU6050 connection FAILED");
//     while (1); // stop here if it failed
//   }
}

int16_t trials = 1;
int16_t sX, sY, sZ, nX, nY, nZ;

void loop() {
  int16_t ax, ay, az;
  int16_t gx, gy, gz;

//   int16_t trials = 1;

//   int16_t sX, sY, sZ, nX, nY, nZ;

  if (trials == 1)
  {

    int32_t sumX = 0;
    int32_t sumY = 0;
    int32_t sumZ = 0;

    Serial.println("Average");

    const int N = 80;
    for (int i=0; i<N; i++) 
    {
        mpu.getMotion6(&ax,&ay,&az,&gx,&gy,&gz);
        sumX += ax; sumY += ay; sumZ += az;
        delay(5);
    }
        


        
        sX = sumX / N;
        sY = sumY / N;
        sZ = sumZ / N;

        Serial.print(sX);
        Serial.print(", ");
        Serial.print(sY);
        Serial.print(", ");
        Serial.println(sZ);

        trials++;
  }
  else
  {
        mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
        nX = ax;
        nY = ay;
        nZ = az;

        int16_t changeX = nX - sX;

        if(changeX > 600)
        {
            //worth moving Right
            Serial.println("Move Right");

        }
        else if (changeX < -600)
        {
            // worth moving Left
            Serial.println("Move Left");
        }
        else
        {
            Serial.println("Center");
        }

        // Serial.print(nX);
        // Serial.print(", ");
        // Serial.print(nY);
        // Serial.print(", ");
        // Serial.println(nZ);
  }

//   mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);

//   Serial.print("Accel (raw): ");
//   Serial.print(ax); Serial.print(", ");
//   Serial.print(ay); Serial.print(", ");
//   Serial.print(az);

  //Serial.println("Done");

//   Serial.print(" | Gyro (raw): ");
//   Serial.print(gx); Serial.print(", ");
//   Serial.print(gy); Serial.print(", ");
//   Serial.println(gz);

  delay(200);
}
