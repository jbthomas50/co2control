//Arduino Libraries
#include <Arduino_FreeRTOS.h>
#include <semphr.h>
#include <LiquidCrystal_I2C.h>
#include <SoftwareSerial.h>

//Project Libraries
#include "fan.h"
#include "CO2_Sensor.h"

//SEMAPHORES
SemaphoreHandle_t xLevelMutex;
SemaphoreHandle_t xTargetMutex;
SemaphoreHandle_t xDisplayMutex;
SemaphoreHandle_t xManageSignal;
SemaphoreHandle_t xSaveCloudSignal;
SemaphoreHandle_t xSaveFileSignal;
//QueueHandle_t xDisplayQueue;

//GLOBAL VARIABLES
volatile unsigned long CO2_target = 100;
volatile unsigned long CO2_level = 100;
LiquidCrystal_I2C lcd(0x27,2,1,0,4,5,6,7);
SoftwareSerial sensorSerial(12, 13); // RX, TX pins on Ardunio
CO2_Sensor mySensor(&sensorSerial);   // Set up sensor with software serial object.
uint8_t numFans = 1;
Fans fans = Fans(8, 9, 10, 11);
bool refreshDisplay = true;
uint8_t solenoidPin = 7;
uint8_t screenLightPin = 3;

//Prototypes
void writeToFile(void *pvParameters);
void writeToCloud(void *pvParameters);
void readCO2_sensor(void *pvParameters);
void manageCO2_levels(void *pvParameters);
void setUpHardware();

void setup() 
{
  Serial.begin(9600);
  setUpHardware();

  //define Semaphores
  if(xLevelMutex == NULL)
  {
    xLevelMutex = xSemaphoreCreateMutex();
    if( (xLevelMutex) != NULL)
    {
      xSemaphoreGive((xLevelMutex));
    }
  }

  if(xTargetMutex == NULL)
  {
    xTargetMutex = xSemaphoreCreateMutex();
    if( (xTargetMutex) != NULL)
    {
      xSemaphoreGive((xTargetMutex));
    }
  }

  if(xDisplayMutex == NULL)
  {
    xDisplayMutex = xSemaphoreCreateMutex();
    if( (xDisplayMutex) != NULL)
    {
      xSemaphoreGive((xDisplayMutex));
    }
  }

  if(xManageSignal == NULL)
  {
    vSemaphoreCreateBinary(xManageSignal);
  }

  if(xSaveCloudSignal == NULL)
  {
    vSemaphoreCreateBinary(xSaveCloudSignal);
  }

  if(xSaveFileSignal == NULL)
  {
    vSemaphoreCreateBinary(xSaveFileSignal);
  }
  
  xTaskCreate(manageCO2_levels, "mngCO2", 
              100, NULL, 3, NULL);
  xTaskCreate(readCO2_sensor, "rdCO2", 
              100, NULL, 3, NULL);
//   xTaskCreate(writeToCloud, "wrt2cld", 
//               100, NULL, 1, NULL);
//   xTaskCreate(writeToFile, "wrt2FL", 
//               100, NULL, 1, NULL);
}

void loop() 
{
  //Tasks during down time, or delays.
  uint8_t tempFans = numFans;
  numFans = map(analogRead(A0), 0, 1024, 0, 5);
  if (numFans != tempFans)
  {
    fans.on(numFans);
    refreshDisplay = true;
  }

  if(refreshDisplay)
  {
    lcd.setCursor(4, 0);
    lcd.print(CO2_level);
  
    lcd.setCursor(4, 1);
    lcd.print(CO2_target);
  
    lcd.setCursor(15, 1);
    lcd.print(numFans);

    refreshDisplay = false;
  }
}

void setUpHardware()
{
  lcd.begin(16,2);
  lcd.clear();
  lcd.home();
  lcd.print(F("ACT: "));
  lcd.setCursor(12, 0);
  lcd.print(F("|FANS"));
  lcd.setCursor(0,1);
  lcd.print(F("SET: "));
  lcd.setCursor(12, 1);
  lcd.print(F("|   "));
    
  pinMode(A0, INPUT);
  pinMode(A1, INPUT);
}

/**
 * Writes information to the cloud (Should make this a time interupt)
 */
void writeToCloud(void *pvParameters)
{  
  //runs when function is called for the first time.

  //runs forever
  for(;;)
  {
    //TODO: Write to cloud here... 
     if(xSemaphoreTake(xSaveCloudSignal, portMAX_DELAY))
     {
      if(xSemaphoreTake(xDisplayMutex, 1000))
      {
        Serial.println(F("FILE"));
      }
     }
  }
}

/**
 * Writes to SD card (Should make this a time interupt)
 */
void writeToFile(void *pvParameters)
{
  //runs when function is called for the first time.

  //runs forever
  for(;;)
  {
    //TODO: Write to SD card here...
     if(xSemaphoreTake(xSaveFileSignal, portMAX_DELAY))
     {
      if(xSemaphoreTake(xDisplayMutex, 1000))
      {
        Serial.println(F("FILE"));
      }
     }
  }
}

/**
 * Reads CO2 sensor
 */
void readCO2_sensor(void *pvParameters)
{
  //runs when function is called for the first time.
  static unsigned long tempLevel = 0;
  static unsigned long tempTarget = 0;
  //runs forever
  for(;;)
  {
    // Read sensor here...
    mySensor.read();
    tempLevel = mySensor.getCO2();

    // READ USER INPUT
    tempTarget = (map(analogRead(A2), 0, 1023, 0, 101) * 100) + (map(analogRead(A1), 0, 1023, 1, 101) * 10000);
    
    if(tempTarget >= tempLevel)
    {
      xSemaphoreGive(xManageSignal);
    }

    if(xSemaphoreTake(xLevelMutex, 1000))
    {
      if(CO2_level != tempLevel)
        refreshDisplay = true;
        
      CO2_level = tempLevel;
      xSemaphoreGive(xLevelMutex);
    }

    if(xSemaphoreTake(xTargetMutex, 1000))
    {
      if(CO2_target != tempTarget)
        refreshDisplay = true;
        
      CO2_target = tempTarget;
      xSemaphoreGive(xTargetMutex);
    }

    if(xSemaphoreTake(xDisplayMutex, 500))
    {
      xSemaphoreGive(xDisplayMutex);
    }

    vTaskDelay(500/*in ms*/ / portTICK_PERIOD_MS);
  }
}

/**
 * Controls how much CO2 is in the tank, and tries to 
 * match it based on the CO2 Sensor reading
 */
void manageCO2_levels(void *pvParameters)
{
  //runs when function is called for the first time.
//  static Solenoid solenoid;
  
  //runs forever.
  for(;;)
  {
    if(xSemaphoreTake(xManageSignal, portMAX_DELAY))
    {
      //TODO: Turn on solenoid here...
      if(xSemaphoreTake(xDisplayMutex, 500))
      {
        if (CO2_target < CO2_level)
        {
          digitalWrite(solenoidPin, HIGH);
        }
        else 
        {
          digitalWrite(solenoidPin, LOW);
        }
        xSemaphoreGive(xDisplayMutex);
      }
    }
  }
}
