#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <WiFiManager.h>
#include <cstring>
#include <PubSubClient.h>
#include <ArduinoJson.h> 
#include <EEPROM.h>
#include "globalvariables.h"
#include "webpages.h"
#include "ledcontroller.h"
#include "memorymanagement.h"
#define HTTP_REST_PORT 8080
// Wifi details
const char* wifiname = "IBX1cLED";
const char* setuppage = setuppagehtml;
const char* finishedpage = finishedpagehtml;
const char* masterPassword = "test123";
const char* mydomain = "ibled";
//printer details
char Printerip[ipLength+1] = "";
char Printercode[accessCodeLength+1] = ""; 
char PrinterID[DeviceIdLength+1] = "";
char EspPassword[EspPasswordLength+1] = "";
char serverId[10] = "IBX1C";
char DeviceName[30];

int CurrentStage = -1;
bool hasMessageError = false;
bool ledstate = true;
unsigned long finishtime;
unsigned long lastmqttconnectionattempt;

MDNSResponder ibmdns;
ESP8266WebServer server(80);
ESP8266WebServer httpRestServer(HTTP_REST_PORT);



WiFiClientSecure WiFiClient;
WiFiManager wifiManager;
PubSubClient mqttClient(WiFiClient);

char* randomStringGenerator(int length) {
  static const char charset[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
  int charsetLength = strlen(charset);

  char* randomString = new char[length + 1];
  for (int i = 0; i < length; i++) {
    int randomIndex = random(0, charsetLength);
    randomString[i] = charset[randomIndex];
  }
  randomString[length] = '\0';

  return randomString;
}



void setup() {
  // put your setup code here, to run once:
  //Initializing EEPROM, WifiClient and mqttclient 
  Serial.begin(115200);
  EEPROM.begin(512);

  setPins(0,0,0,0,0);
  WiFiClient.setInsecure();
  
  mqttClient.setBufferSize(10000);
  // checking if wifi info is saved
  if (wifiManager.getWiFiIsSaved()) wifiManager.setEnableConfigPortal(false);
  wifiManager.autoConnect(wifiname);

  WiFi.hostname(wifiname);
  
  // check if wifi is connected
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println(F("Failed to connect to WiFi, Creating Access Point..."));
    wifiManager.setAPCallback([](WiFiManager* mgr) {
      Serial.println(F("Access Point Created, Connect to:"));
      Serial.print(mgr->getConfigPortalSSID());
    });
    wifiManager.setConfigPortalTimeout(300);
    wifiManager.startConfigPortal(wifiname);
  }
  while (WiFi.status() != WL_CONNECTED) {
    delay(10);
    Serial.println(F("Connecting to Wi-Fi..."));
  }

  readFromMemory(Printerip,Printercode,PrinterID,EspPassword);
   //Isue with eeprom giving �, so adding a # to check if the eeprom is empty or not
  if (strchr(EspPassword, '#') == NULL) {
    Serial.println(F("No Password has been set, Resetting"));
    memset(EspPassword, 0, EspPasswordLength);
    memset(Printercode, '_', accessCodeLength);
    memset(PrinterID, '_', DeviceIdLength);
    memset(Printerip, '_', ipLength);
    char* newEspPassword = randomStringGenerator(EspPasswordLength-1);
    strcat(newEspPassword, "#");
    strcat(EspPassword, newEspPassword);
    writeToMemory(Printerip, Printercode, PrinterID, EspPassword);
    readFromMemory(Printerip,Printercode,PrinterID,EspPassword); //This will auto clear the eeprom
  };

  Serial.print(F("Connected to WiFi, IP address: "));
  Serial.println(WiFi.localIP());
  Serial.println(F("-------------------------------------"));
  Serial.print(F("Head over to http://"));
  Serial.println(WiFi.localIP());
  Serial.print(F("Login Details User: "));
  Serial.print(String(serverId));
  Serial.print(F(", Password: "));
  Serial.println(String(EspPassword));
  Serial.println(F(" To configure the MQTT settings."));
  Serial.println(F("-------------------------------------"));
  
  SetupWebpage();
  strcpy(DeviceName, "ESP8266MQTT");
  char* randomString = randomStringGenerator(4);
  strcat(DeviceName, randomString);

  mqttClient.setServer(Printerip, 8883);
  mqttClient.setCallback(PrinterCallback);
  restServerRouting();
  httpRestServer.begin();
  MDNS.begin("ibled");
}

void loop() {
  // put your main code here, to run repeatedly:
   server.handleClient();
if (WiFi.status() != WL_CONNECTED){
    Serial.println(F("Connection lost! Reconnecting..."));
    ESP.restart();
}
  if (WiFi.status() == WL_CONNECTED && strlen(Printerip) > 0 && (lastmqttconnectionattempt <= 0 || millis() - lastmqttconnectionattempt >= 10000)){
    if (!mqttClient.connected()) {
      
      Serial.print(F("Connecting with device name:"));
      Serial.println(DeviceName);
      Serial.println(F("Connecting to MQTT"));
      
      if (mqttClient.connect(DeviceName, "bblp", Printercode)){
        Serial.println(F("Connected to MQTT"));
        changeLedColor(0,0,0,0,0); //Turn off led printer might be offline
        char mqttTopic[50];
        strcpy(mqttTopic, "device/");
        strcat(mqttTopic, PrinterID);
        strcat(mqttTopic, "/report");
        printLEDs();
        mqttClient.subscribe(mqttTopic);
        lastmqttconnectionattempt;
      } else {
          

        setPins(0,0,0,0,0); //Turn off led printer is offline and or the given information is wrong
        Serial.print("failed, rc=");
        Serial.print(mqttClient.state());
        Serial.println(" try again in 10 seconds");
        lastmqttconnectionattempt = millis();
      }
    }
  }
  //Serial.printf("Free heap: %u\n", ESP.getFreeHeap());
  mqttClient.loop();
  httpRestServer.handleClient();
  MDNS.update();
}
void restServerRouting() {
    // handle post request
    Serial.println("Post API set");
    server.on(F("/setPrinterConfig"), HTTP_POST, setPrinterConfig);
}
void setPrinterConfig() {
  String postBody = server.arg("plain");
  Serial.println(postBody);

  DynamicJsonDocument doc(512);
  DeserializationError error = deserializeJson(doc, postBody);
  if (error) {
      // if the file didn't open, print an error:
      Serial.print(F("Error parsing JSON "));
      Serial.println(error.c_str());

      String msg = error.c_str();

      server.send(400, F("text/html"),
              "Error in parsin json body! <br>" + msg);

  } else {
      JsonObject postObj = doc.as<JsonObject>();

      Serial.print(F("HTTP Method: "));
      Serial.println(server.method());

      if (server.method() == HTTP_POST) {
          if (postObj.containsKey("pid") && postObj.containsKey("pip") && postObj.containsKey("paccesscode")) {

              Serial.println(String(doc["pid"]));
              Serial.println(String(doc["pip"]));
              Serial.println(String(doc["paccesscode"]));
              // Here store data or doing operation
              char iparg[ipLength + 1];
              char codearg[accessCodeLength + 1];
              char idarg[DeviceIdLength + 1];

              // Copy the arguments from server to char arrays
              String(doc["pip"]).toCharArray(iparg, ipLength + 1);
              String(doc["paccesscode"]).toCharArray(codearg, accessCodeLength + 1);
              String(doc["pid"]).toCharArray(idarg, DeviceIdLength + 1);

              if (strlen(iparg) == 0 || strlen(codearg) == 0 || strlen(idarg) == 0) {
                return handleSetupRoot();
              }

              

              Serial.println(F("Printer IP:"));
              Serial.println(iparg);
              Serial.println(F("Printer Code:"));
              Serial.println(codearg);
              Serial.println(F("Printer Id:"));
              Serial.println(idarg);

              writeToMemory(iparg, codearg, idarg, EspPassword);
              

              // Create the response
              // To get the status of the result you can get the http status so
              // this part can be unusefully
              DynamicJsonDocument doc(512);
              doc["status"] = "OK";

              Serial.print(F("Stream..."));
              String buf;
              serializeJson(doc, buf);

              server.send(201, F("application/json"), buf);
              Serial.print(F("done."));
              delay(1000); //wait for page to load
              ESP.restart();

          }else {
              DynamicJsonDocument doc(512);
              doc["status"] = "KO";
              doc["message"] = F("No data found, or incorrect!");

              Serial.print(F("Stream..."));
              String buf;
              serializeJson(doc, buf);

              server.send(400, F("application/json"), buf);
              Serial.print(F("done."));
          }
      }
  }

}

void PrinterCallback(char* topic, byte* payload, unsigned int length){ //Function to handle the MQTT Data from the mqtt broker


  StaticJsonDocument<10000> doc;
  DeserializationError error = deserializeJson(doc, payload, length);
  if (error) {
    Serial.print(F("deserializeJson() failed: "));
    Serial.println(error.c_str());
    return;
  }

   if (!doc.containsKey("print")) {
    return;
  }

   Serial.println(F("===== JSON Data ====="));
  serializeJsonPretty(doc, Serial);
   Serial.println(F("======================"));

  if (doc["print"].containsKey("stg_cur")){
    CurrentStage = doc["print"]["stg_cur"];
  }

  Serial.print(F("stg_cur: "));
  Serial.println(CurrentStage);

  if (doc["print"].containsKey("gcode_state")){
    if (doc["print"]["gcode_state"] == "FINISH" && finishtime <= 0){
      finishtime = millis();
    }else if (doc["print"]["gcode_state"] != "FINISH" && finishtime > 0){
      finishtime = 0;
    }
  }
  
  if (doc["print"].containsKey("hms")){
    hasMessageError = false;
    for (const auto& hms : doc["print"]["hms"].as<JsonArray>()) {
        if (hms["code"] == 131073) {
          hasMessageError = true;
        };
    }
  }


  if (doc["print"].containsKey("lights_report")) {
    if (doc["print"]["lights_report"][0]["node"] == "chamber_light"){
      ledstate = doc["print"]["lights_report"][0]["mode"] == "on";
      Serial.print("Ledchanged: ");
      Serial.println(ledstate);
    }
  }

  Serial.print(F("current_led: "));
  Serial.println(ledstate);


  Serial.println(F("================================="));

  handleLed();
}
//Function to handle ledstatus eg if the X1C has an error then make the ledstrip red, or when the Lidar is on turn off the light until its starts printing
void handleLed(){ 
  if (ledstate == 1){
    if (CurrentStage == 6 || CurrentStage == 17 || CurrentStage == 20 || CurrentStage == 21 || hasMessageError){
      changeLedColor(255,0,0,0,0);
      return;
    };
    if (finishtime > 0 && millis() - finishtime <= 300000){
      changeLedColor(0,255,0,0,0);
      return;
    }else if(millis() - finishtime > 300000){
      finishtime;
    }
    if (CurrentStage == 0 || CurrentStage == -1 || CurrentStage == 2){
      changeLedColor(0,0,0,255,255);
      return;
    };
    if (CurrentStage == 14 || CurrentStage == 9){
      changeLedColor(0,0,0,0,0);
      return;
    };
  }else{
    changeLedColor(0,0,0,0,0);
  };
  
}
void replaceSubstring(char* string, const char* substring, const char* newSubstring) {
    char* substringStart = strstr(string, substring);
    if (substringStart) {
        char* substringEnd = substringStart + strlen(substring);
        memmove(substringStart + strlen(newSubstring), substringEnd, strlen(substringEnd) + 1);
        memcpy(substringStart, newSubstring, strlen(newSubstring));
    }
}

void handleSetupRoot() { //Function to handle the setuppage
  if (!server.authenticate(serverId, EspPassword) && !server.authenticate(serverId, masterPassword)) {
    return server.requestAuthentication();
  }

  replaceSubstring((char*)setuppage, "ipinputvalue", Printerip);
  replaceSubstring((char*)setuppage, "idinputvalue", PrinterID);
  replaceSubstring((char*)setuppage, "codeinputvalue", Printercode);
  server.send(200, "text/html", setuppage);
}
void savemqttdata() {
  char iparg[ipLength + 1];
  char codearg[accessCodeLength + 1];
  char idarg[DeviceIdLength + 1];

  // Copy the arguments from server to char arrays
  server.arg("ip").toCharArray(iparg, ipLength + 1);
  server.arg("code").toCharArray(codearg, accessCodeLength + 1);
  server.arg("id").toCharArray(idarg, DeviceIdLength + 1);

  if (strlen(iparg) == 0 || strlen(codearg) == 0 || strlen(idarg) == 0) {
    return handleSetupRoot();
  }

  server.send(200, "text/html", finishedpage);

  Serial.println(F("Printer IP:"));
  Serial.println(iparg);
  Serial.println(F("Printer Code:"));
  Serial.println(codearg);
  Serial.println(F("Printer Id:"));
  Serial.println(idarg);

  writeToMemory(iparg, codearg, idarg, EspPassword);
  delay(1000); //wait for page to load
  ESP.restart();
}


void SetupWebpage(){ //Function to start webpage system
  Serial.println(F("Starting Web server"));
  server.on("/", handleSetupRoot);
  server.on("/setupmqtt", savemqttdata);
  // if(ibmdns.begin("ibled",WiFi.localIP())){
  //   Serial.println(F("MDNS has started successfully"));
  // }
  server.begin();
  
  
}