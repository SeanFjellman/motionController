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
  //pinMode(4, INPUT_PULLUP);
  //pinMode(2, INPUT);
}

int16_t trials = 1;
int16_t sX, sY, sZ, nX, nY, nZ;



// enum ArmState { ARM_DOWN, ARM_UP };
// ArmState armState = ARM_DOWN;

//For Average

    int16_t ax, ay, az;
    int16_t gx, gy, gz;

    int32_t sumX = 0;
    int32_t sumY = 0;
    int32_t sumZ = 0;
    const int N = 80;


void loop() {


//   int16_t trials = 1;

//   int16_t sX, sY, sZ, nX, nY, nZ;


// int tiltState = digitalRead(4);


  if (trials == 1)
  {
    AverageFunction();
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

        //1500 90 up -1600 90 down
        if(abs(changeY) > abs(changeX))
        {
          if (changeY > 500 && changeY < 15000) 
          { 
            Serial.print("Backwards");
            //Serial.print(changeY);
            moved = true;
          }
          else if(changeY < -800 && changeY > -14000) 
          { 
            Serial.print("Forwards");
            //Serial.print(changeY);
            moved = true;
          }
          else if(changeY > 15000)
          {
            Serial.print("Move Up");
          }
          else if(changeY < -14000)
          {
            Serial.print("Move Down");
          }


        }
        //Serial.print(changeY);
        Serial.println();
        delay(50);

      Serial.println();

      
      if(digitalRead(2) == HIGH)
      {
        Serial.print("New ");
        AverageFunction();
      }


  }



  delay(200);
  //delay(6000);
}


void AverageFunction()
{
      Serial.println("Average:");


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
