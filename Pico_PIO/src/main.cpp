#include <Arduino.h>

#include <Servo.h>

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
// #include <ESP32Servo.h>
#include <limits.h> 

Servo myServo;

SparkFun_VL53L5CX myImager;
VL53L5CX_ResultsData measurementData; // Result data class structure, 1356 byes of RAM

int imageResolution = 0; //Used to pretty print output
int imageWidth = 0; //Used to pretty print output

int map2d_to_1d_array(int x, int y, int width){
  return y*width + x;
}

void map8x8_to_3x4(const int16_t* data, int16_t grid[4][3]) {
  const int width = 8;
  for (int y = 0; y < 4; y++)
    for (int x = 0; x < 3; x++) grid[y][x] = INT16_MAX;

  for (int y = 0; y < width; y++) {
    for (int x = width - 1; x >= 0; x--) {
      int mappedX = 0;  // Ensure initialization
      int mappedY = y / 2;

      // Calculate the index and retrieve the value from the data array

      // Map x-coordinate to the grid
      switch (x) {
        case 0:
        case 1:
          mappedX = 2;
          break;
        case 2:
        case 3:
        case 4:
        case 5:
          mappedX = 1;
          break;
        case 6:
        case 7:
          mappedX = 0;
          break;
        default:
          // Handle unexpected cases (e.g., if x is outside the range)
          break;
      }

      int16_t dist = data[map2d_to_1d_array(x, y, width)];

      // Update the grid value using std::min
      grid[mappedY][mappedX] = min(dist, grid[mappedY][mappedX]);
    }
  }
}


int16_t grid[4][3];

const int LED_PIN = 27;
void setup()
{  
  Wire.setSDA(20);
  Wire.setSCL(21);

  // pinMode(D25, OUTPUT);
  pinMode(LED_PIN, OUTPUT_2MA);
  myServo.attach(D0);
  
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

bool ledState = 0;
int servoPos = 0;
bool dir = true;
void loop()
{
  static int lastSwitch = millis();
  //Poll sensor for new data
  
  if (myImager.isDataReady() == true)
  {
    if (myImager.getRangingData(&measurementData)) //Read distance data into array
    {
      // if(ledState) ledState = 0;
      // else ledState = 1;

      //The ST library returns the data transposed from zone mapping shown in datasheet
      //Pretty-print data with increasing y, decreasing x to reflect reality
      for (int y = 0 ; y <= imageWidth * (imageWidth - 1) ; y += imageWidth)
      {
        for (int x = imageWidth - 1 ; x >= 0 ; x--)
        {
          Serial.print("\t");
          Serial.print(measurementData.distance_mm[x + y]);
        }
        Serial.println();
      }
      Serial.println("grid");

      map8x8_to_3x4(measurementData.distance_mm, grid);
      for (int y = 0 ; y < 4; y++)
      {
        for (int x = 0; x < 3; x++)
        {
          Serial.print("\t");
          // Serial.print(measurementData.distance_mm[x + y]);
          Serial.print(grid[3-y][x]);
        }
        Serial.println();
      }
      Serial.println();

      // int index = map2d_to_1d_array(imageWidth/2, imageWidth/2, imageWidth);
      // int servoVal = constrain(map(measurementData.distance_mm[index],100, 1500, 0, 180), 0, 180);
      int servoVal = constrain(map(grid[1][2],100, 1500, 180, 0), 0, 180);

      myServo.write(servoVal);
      // Serial.print(measurementData.distance_mm[index]);
      Serial.print(grid[1][2]);
      Serial.print("\t Servo: ");
      Serial.println(servoVal);


    }
  }
  
  if(millis() - lastSwitch > 1000){
    ledState = !ledState;
    digitalWrite(LED_PIN, ledState);

    Serial.print("Millis: ");
    Serial.print(millis());
    Serial.print("lst: ");
    Serial.print(lastSwitch);
    Serial.print("state ");
    Serial.println(ledState);

    lastSwitch = millis();
  }
  delay(5); //Small delay between polling
  // if(dir){
  //   servoPos++;
  // }
  // else{
  //   servoPos--;
  // }
  // if(servoPos <= 0) dir = true;
  // if(servoPos >= 180) dir = false;

  // myServo.write(servoPos);

}



// #include <Arduino.h>
// #include "pico/stdlib.h"


// #include <Servo.h>

// /*
//   Read an 8x8 array of distances from the VL53L5CX
//   By: Nathan Seidle
//   SparkFun Electronics
//   Date: October 26, 2021
//   License: MIT. See license file for more information but you can
//   basically do whatever you want with this code.

//   This example shows how to read all 64 distance readings at once.

//   Feel like supporting our work? Buy a board from SparkFun!
//   https://www.sparkfun.com/products/18642

// */

// #include <Wire.h>

// #include <SparkFun_VL53L5CX_Library.h> //http://librarymanager/All#SparkFun_VL53L5CX
// // servo
// // #include <ESP32Servo.h>
// #include <limits.h> 

// void setup() {
//     const uint LED_PIN = 25;
//     gpio_init(LED_PIN);
//     gpio_set_dir(LED_PIN, GPIO_OUT);
//     while (true) {
//         gpio_put(LED_PIN, 1);
//         sleep_ms(250);
//         gpio_put(LED_PIN, 0);
//         sleep_ms(250);
//     }
// }
// void loop() {}