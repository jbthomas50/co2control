#include <Arduino_FreeRTOS.h>
#include "fan.h"
#include "solenoid.h"

float CO2_target = 0;
float CO2_level = 0;

void changeCO2_target(void);
void writeToFile(void *pvParameters);
void displayLCD(void *pvParameters);
void readCO2_sensor(void *pvParameters);
void manageCO2_levels(void *pvParameters);

void setup() 
{
  //setting up Interrupts
  attachInterrupt(0, changeCO2_target, CHANGE);
  
  //priotities
  int manageCO2_priority = 1;
  int readCO2_priority = 2;
  int writeToFile_priority = 3;
  int displayLCD_priority = 4;


  xTaskCreate(writeToFile, "Writting to file", 128, 
              NULL, writeToFile_priority, NULL);
  xTaskCreate(displayLCD, "Displaying to LCD", 128,
              NULL, displayLCD_priority, NULL);
  xTaskCreate(readCO2_sensor, "Reading CO2 sensor", 128,
              NULL, readCO2_priority, NULL);
  xTaskCreate(manageCO2_levels, "Managing CO2 levels", 128,
              NULL, manageCO2_priority, NULL);
}

void loop() 
{
  // Empty. Things are done in Tasks.
}

/**
 * 
 */
void changeCO2_target(void)
{
  
}

/**
 * 
 */
void writeToFile(void *pvParameters)
{
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
 * 
 */
void displayLCD(void *pvParameters)
{
  //runs when function is called for the first time.
  int timeDelay = 1000; //in ms

  //runs forever
  for(;;)
  {
    
    // divide by "portTICK_PERIOD_MS" to convert to seconds
    vTaskDelay(timeDelay / portTICK_PERIOD_MS); 
  }
}

/**
 * 
 */
void readCO2_sensor(void *pvParameters)
{
  //runs when function is called for the first time.
  int timeDelay = 100; //in ms

  //runs forever
  for(;;)
  {
    
    vTaskDelay(timeDelay / portTICK_PERIOD_MS);
  }
}

/**
 * 
 */
void manageCO2_levels(void *pvParameters)
{
  //runs when function is called for the first time.
  Fan fan;
  Solenoid solenoid;
  int timeDelay = 10; //in ms

  //runs forever.
  for(;;)
  {

    vTaskDelay(timeDelay / portTICK_PERIOD_MS);
  }
}

