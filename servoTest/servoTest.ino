/*
  Read an 8x8 array of distances from the VL53L5CX
  By: Nathan Seidle
  SparkFun Electronics
  Date: October 26, 2021
  License: MIT. See license file for more information but you can
  basically do whatever you want with this code.

  This example shows how to read all 64 distance readings at once.

  Feel like supporting our work? Buy a board from SparkFun!
  https://www.sparkfun.com/products/18642

*/

#include <Wire.h>

#include <SparkFun_VL53L5CX_Library.h> //http://librarymanager/All#SparkFun_VL53L5CX
// servo
#include <ESP32Servo.h>
#include <limits.h> 

Servo myServo;

SparkFun_VL53L5CX myImager;
VL53L5CX_ResultsData measurementData; // Result data class structure, 1356 byes of RAM

int imageResolution = 0; //Used to pretty print output
int imageWidth = 0; //Used to pretty print output

int map2d_to_1d_array(int x, int y, int width){
  return y*width + x;
}

void map8x8_to_3x4(int16_t* data, int16_t grid[4][3]){
  const int imageWidth = 8;
  for (int y = 0 ; y < imageWidth; y++)
  {
    for (int x = imageWidth - 1 ; x >= 0 ; x--)
    {
      // Serial.print("\t");
      // Serial.print(measurementData.distance_mm[x + y]);
      int mappedX, mappedY;
      grid[mappedY][mappedX] = INT16_MAX;;

      int16_t dist = measurementData.distance_mm[map2d_to_1d_array(x, y, 8)];
      mappedY = y/2;

      switch(x){
        case 0:
          mappedX = 2;
          break;
        case 1:
          mappedX = 2;
          break;
        case 2:
          mappedX = 1;
          break;
        case 3:
          mappedX = 1;
          break;
        case 4:
          mappedX = 1;
          break;
        case 5:
          mappedX = 1;
          break;
        case 6:
          mappedX = 0;
          break;
        case 7:
          mappedX = 0;
          break;        
      }

      grid[mappedY][mappedX] = min(dist, grid[mappedY][mappedX]);

    }
    // Serial.println();
  }
}

void mapZoomedIn(int16_t* data, int16_t grid[4][3]){
  const int imageWidth = 4;
  for (int y = 0 ; y < imageWidth; y++)
  {
    for (int x = imageWidth - 1 ; x >= 0 ; x--)
    {
      // Serial.print("\t");
      // Serial.print(measurementData.distance_mm[x + y]);
      int mappedX, mappedY;
      grid[mappedY][mappedX] = INT16_MAX;;

      int16_t dist = measurementData.distance_mm[map2d_to_1d_array(x, y, 8)];
      mappedY = y/2;

      switch(x){
        case 0:
          mappedX = 2;
          break;
        case 1:
          mappedX = 2;
          break;
        case 2:
          mappedX = 1;
          break;
        case 3:
          mappedX = 1;
          break;
        case 4:
          mappedX = 1;
          break;
        case 5:
          mappedX = 1;
          break;
        case 6:
          mappedX = 0;
          break;
        case 7:
          mappedX = 0;
          break;        
      }

      grid[mappedY][mappedX] = min(dist, grid[mappedY][mappedX]);

    }
    // Serial.println();
  }
}

int16_t grid[4][3];
int16_t data[8][8];

void setup()
{  
  myServo.attach(12);
  
  Serial.begin(115200);
  delay(1000);
  Serial.println("SparkFun VL53L5CX Imager Example");

  Wire.begin(); //This resets to 100kHz I2C
  Wire.setClock(400000); //Sensor has max I2C freq of 400kHz 
  
  Serial.println("Initializing sensor board. This can take up to 10s. Please wait.");
  if (myImager.begin() == false)
  {
    Serial.println(F("Sensor not found - check your wiring. Freezing"));
    while (1) ;
  }
  
  myImager.setResolution(8*8); //Enable all 64 pads
  
  imageResolution = myImager.getResolution(); //Query sensor for current resolution - either 4x4 or 8x8
  imageWidth = sqrt(imageResolution); //Calculate printing width

  myImager.startRanging();
}

void loop()
{
  //Poll sensor for new data
  if (myImager.isDataReady() == true)
  {
    if (myImager.getRangingData(&measurementData)) //Read distance data into array
    {
      //The ST library returns the data transposed from zone mapping shown in datasheet
      //Pretty-print data with increasing y, decreasing x to reflect reality
      // for (int y = 0 ; y <= imageWidth * (imageWidth - 1) ; y += imageWidth)
      // {
      //   for (int x = imageWidth - 1 ; x >= 0 ; x--)
      //   {
      //     Serial.print("\t");
      //     Serial.print(measurementData.distance_mm[x + y]);
      //   }
      //   Serial.println();
      // }
      // Serial.println();


      int index = map2d_to_1d_array(imageWidth/2, imageWidth/2, imageWidth);
      int servoVal = constrain(map(measurementData.distance_mm[index],100, 2500, 180, 0), 0, 180);

      myServo.write(servoVal);
      Serial.print(measurementData.distance_mm[index]);
      Serial.print("\t Servo: ");
      Serial.println(servoVal);


    }
  }

  delay(5); //Small delay between polling
}
