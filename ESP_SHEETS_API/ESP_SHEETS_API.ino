#include "ESP8266WiFi.h"
#include <SoftwareSerial.h>

//3rd Party website information
const char WEBSITE[] = "api.pushingbox.com"; //pushingbox API server
const String devid = "v9CE1BCE48848641"; //device ID from Pushingbox 

//WiFi information
const char* MY_SSID = "";
const char* MY_PWD =  "";

//Variables to upload to googleSheets
String sheetID = "1cMlEN7ZF2fPk9kcJsDMf5f1v9CnJC_4UUIQ47ErUbcE"; //Google Sheets SheetID
String timestamp;
String setCO2;
String readCO2;

//Serially communicate with arduino
SoftwareSerial arduino(1, 2);

void setup()
{
  connectWiFi();
  String temp;
  if(arduino.available())
  {
//    parseData(String(arduino.read()));
//    int count = 0;
//    while(WiFi.status() != WL_CONNECTED)
//    {
//      connectWiFi();
//      count++;
//      if (count >= 1000)
//        exit;
//    }
    timestamp = String(arduino.read());
    setCO2 = "10";
    readCO2 = "20";
    uploadData();
  }
  else
  {
    timestamp = "30";
    setCO2 = "10";
    readCO2 = "20";
    uploadData();
  }
}

void loop()
{
  
}

void parseData(String data)
{
  int switcher = 0;
  for(int i = 0; i < data.length(); i++)
  {
    if(data[i] == '{')
    {
      i++;
      while(data[i] != '}')
      {
        switch(switcher)
        {
          case 0:
            timestamp += data[i];
        }
        
        i++;
      }
      switcher++;
      if(switcher == 3)
      {
        switcher = 0;
      }
    }
  }
}

void uploadData()
{
  //Print to Serial monitor or Terminal of your chocice at 115200 Baud
  Serial.print("Google Sheets ID: ");
  Serial.println(sheetID);
  Serial.print("Timestamp: ");
  Serial.println(timestamp);
  Serial.print("setCO2: ");
  Serial.print(setCO2);
  Serial.println("PPM");
  Serial.print("CO2 Reading: ");
  Serial.print(readCO2);
  Serial.println("PPM");
    
  WiFiClient client;  //Instantiate WiFi object

    //Start or API service using our WiFi Client through PushingBox
    if (client.connect(WEBSITE, 80))
    { 
        // Variables set to "N/A" are space savers to make
        //   it easier to add more variables in the future
        client.print("GET /pushingbox?devid=" + devid
                     + "&sheetID="      + (String) sheetID
                     + "&timestamp="    + (String) timestamp
                     + "&setCO2="       + (String) setCO2
                     + "&readCO2="      + (String) readCO2
                     + "&var1="         + "N/A"
                     + "&var2="         + "N/A"
                     + "&var3="         + "N/A");

        client.println(" HTTP/1.1");
        client.print("Host: ");
        client.println(WEBSITE);
        client.println("User-Agent: ESP8266/1.0");
        client.println("Connection: close");
        client.println();
        Serial.print("Success!");
    }
    else
    {
      Serial.println("ERROR: Can't connect!");
    }
}

void connectWiFi()
{
  
  Serial.begin(115200);
  Serial.print("Connecting to " + *MY_SSID);
  WiFi.begin(MY_SSID, MY_PWD);
  Serial.println("going into wl connect");

  while (WiFi.status() != WL_CONNECTED) //not connected,..waiting to connect
  {
      delay(1000);
      Serial.print(".");
  }
  Serial.println("wl connected");
  Serial.println("");
  Serial.println("Credentials accepted! Connected to wifi\n ");
  Serial.println("");
}
