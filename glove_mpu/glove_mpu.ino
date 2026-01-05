#include <Wire.h>
#include <MPU6050.h>

MPU6050 mpu;

void setup() {
  Serial.begin(9600);
  delay(500);
  Serial.println("BOOT");
  Wire.begin();

//   Serial.println("Initializing MPU6050...");
  mpu.initialize();

//   if (mpu.testConnection()) {
//     Serial.println("MPU6050 connection successful");
//   } else {
//     Serial.println("MPU6050 connection FAILED");
//     while (1); // stop here if it failed
//   }
  Serial.println("MPU init done");
  pinMode(2, INPUT);
}

int16_t trials = 1;
int16_t sX, sY, sZ, nX, nY, nZ;

void loop() {
    int16_t ax, ay, az;
    int16_t gx, gy, gz;

    int32_t sumX = 0;
    int32_t sumY = 0;
    int32_t sumZ = 0;
    const int N = 80;

//   int16_t trials = 1;

//   int16_t sX, sY, sZ, nX, nY, nZ;

  if (trials == 1)
  {



    Serial.println("Average");


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
        int16_t changeY = nY - sY;
        int16_t changeZ = nZ - sZ;

        bool moved = false;

        if(abs(changeX) > abs(changeY))
        {
          if (changeX > 4000) 
          {
            Serial.print("Left ");
            moved = true; 
         }
          else if (changeX < -4000)
          { 
            Serial.print("Right ");
            moved = true; 
          }
        }


        if(abs(changeY) > abs(changeX))
        {
          if (changeY > 500) 
          { 
            Serial.print("Backwards");
            moved = true;
        }
          else if(changeY < -800) 
          { 
            Serial.print("Forwards");
            moved = true;
          }
        }

      if (!moved) Serial.print("Center");

      Serial.println();

      
      if(digitalRead(2) == HIGH)
      {
        Serial.println("New Center");
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


      }

        // Serial.print("X:");
        // Serial.print(changeX);

        // Serial.print("Y:");
        // Serial.print(changeY);

        // Serial.print("Z:");
        // Serial.print(changeZ);

        // Serial.println("");


        // if(changeY > 500)
        // {
        //   Serial.println("Move Up");
        // }
        // else if(changeY < -1000)
        // {
        //   Serial.println("Move Down");
        // }

        // if(changeX > 1000)
        // {
        //     //worth moving left
        //     Serial.println("Move Left");

        // }
        // else if (changeX < -1000)
        // {
        //     // worth moving Left
        //     Serial.println("Move Right");
        // }
        // else if (changeY > 500)
        // {
        //     Serial.println("Move Up");
        // }
        // else if(changeY < -1000)
        // {
        //     Serial.println("Move Down");
        // }
        // else
        // {
        //     Serial.println("Center");
        // }


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
  //delay(6000);
}

