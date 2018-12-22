#include <Arduino_FreeRTOS.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include "fan.h"
#include "solenoid.h"
#include "CO2_Sensor.h"


volatile float CO2_target = 0;
volatile float CO2_level = 0;
uint32_t baud = 9600; //global baud-rate
Fan fans[4] = {Fan(0), Fan(1), Fan(2), Fan(3)};

void setFans(void);
void changeCO2_target(void);
void writeToFile(void *pvParameters);
void displayLCD(void *pvParameters);
void readCO2_sensor(void *pvParameters);
void manageCO2_levels(void *pvParameters);
void writeToCloud(void *pvParameters);

void setup() 
{
  //setting up Interrupts
//  attachInterrupt(0, changeCO2_target, CHANGE);
//  attachInterrupt(1, setFans, CHANGE);
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB, on LEONARDO, MICRO, YUN, and other 32u4 based boards.
  }
  
  //priotities
  int manageCO2_priority = 1;
  int readCO2_priority = 2;
  int writeToFile_priority = 3;
  int writeToCloud_priority = 4;
  int displayLCD_priority = 5;

  xTaskCreate(displayLCD, "Displaying to LCD", 128,
              NULL, displayLCD_priority, NULL);
//  xTaskCreate(saveData, "Writting to file", 128, 
//              NULL, writeToFile_priority, NULL);
  xTaskCreate(manageCO2_levels, "Managing CO2 levels", 128,
              NULL, manageCO2_priority, NULL);

//  xTaskCreate(readCO2_sensor, "Reading CO2 sensor", 128,
//              NULL, readCO2_priority, NULL);
//  xTaskCreate(writeToCloud, "Writting to Google Sheets", 128,
//              NULL, writeToCloud_priority, NULL);
}

void loop() 
{
//  Serial.println("Bad loop!!");
  // Empty. Things are done in Tasks.
}

/**
 * Changes targeted CO2 level based on pot reading
 */
void changeCO2_target(void)
{
  CO2_target = analogRead(A1);
}

/**
 * Turns fans on/off depending on pot measurment
 */
void setFans(void)
{
  int numFans = map(analogRead(A0), 0, 1023, 0, 4);
  for(int i = 0; i < 4; i++)
  {
    if (i < numFans)
      fans[i].on();
    else
      fans[i].off();
  }
}

/**
 * Writes information to the cloud
 */
void writeToCloud(void *pvParameters)
{
  (void) pvParameters;
  
  //runs when function is called for the first time.
  int timeDelay = 2000; //in ms
  //runs forever
  for(;;)
  {
    noInterrupts();
    //write to cloud here
    interrupts();
    vTaskDelay(timeDelay / portTICK_PERIOD_MS);
  }
}

/**
 * Writes to SD card
 */
void saveData(void *pvParameters)
{
  (void) pvParameters;
  //runs when function is called for the first time.
  int timeDelay = 1000; //in ms

  //runs forever
  for(;;)
  {
    noInterrupts();
    //write to file here
    interrupts();
    vTaskDelay(timeDelay / portTICK_PERIOD_MS); //one tick delay is about 15ms 
  }
}

/**
 * Displays information on LCD display
 */
void displayLCD(void *pvParameters)
{
  (void) pvParameters;
  #define bottomPot A0
  #define topPot A1

  pinMode(bottomPot, INPUT);
  pinMode(topPot, INPUT);
  //runs when function is called for the first time.
  int timeDelay = 500; //in ms
  LiquidCrystal_I2C lcd(0x27,2,1,0,4,5,6,7);
  lcd.begin(16, 2);
  lcd.print("ACT:             ");
  lcd.setCursor(0,1);
  lcd.print("SET:             ");

  //runs forever
  for(;;)
  {
   lcd.home ();
   lcd.clear();

   long temp = map(analogRead(bottomPot), 0, 1023, 0, 101);
   long temp1 = map(analogRead(topPot), 0, 1023, 1, 101);
   temp *= 100;
   temp1 *= 10000;

   lcd.print("ACT: ");
   lcd.print("XXXXXXX|fan");
   lcd.setCursor(0,1);
   lcd.print("SET: ");
//   lcd.print(2);
//   lcd.setCursor(5, 1);
//   String num = (String) (temp + temp1);
//   for(int i = 0; i < 6 - num.length(); i++)
//   {
//       lcd.print(" ");
//   }
   lcd.print(temp + temp1);
   lcd.print("|  2");
    
    // divide by "portTICK_PERIOD_MS" to convert to seconds
    vTaskDelay(timeDelay / portTICK_PERIOD_MS); 
  }
}

/**
 * Reads CO2 sensor
 */
void readCO2_sensor(void *pvParameters)
{
  (void) pvParameters;
  //runs when function is called for the first time.
  int timeDelay = 200; //in ms

  //runs forever
  for(;;)
  {
    vTaskDelay(timeDelay / portTICK_PERIOD_MS);
  }
}

/**
 * Controls how much CO2 is in the tank, and tries to 
 * match it based on the CO2 Sensor reading
 */
void manageCO2_levels(void *pvParameters)
{
  (void) pvParameters;
  //runs when function is called for the first time.
  Solenoid solenoid;
  int timeDelay = 300; //in ms

  //runs forever.
  for(;;)
  {
    setFans();
    vTaskDelay(timeDelay / portTICK_PERIOD_MS);
  }
}

