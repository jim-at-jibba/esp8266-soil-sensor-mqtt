/**
 * Plant Moisture Sensor~ 
 */

 #include <ESP8266WiFi.h>
 #include <PubSubClient.h>
 #include "settings.h"


 /**
  * MQTT Settings
  */
const char* mqtt_server = "192.168.1.217";
const char* plantMoistureTopic = "office/windowPlant";

WiFiClient espClient;
PubSubClient client(espClient);

/**
 * Sensor variables
 */
int value ;
int sensor_pin = A0;
char stringValue[16];
char stringBattery[16];

int interval = 60000;

/**
 * Setup function
 */
void setup() {
  WiFi.forceSleepWake();
  delay(1);
  WiFi.mode(WIFI_STA);
  
  Serial.begin(115200);
  Serial.println();
  Serial.print("Setting up....");
  setupWifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);

  mqtt_connect();
  delay(100);
  value = analogRead(sensor_pin);
  Serial.print("Plant Mositure: ");
  Serial.println(value);
  value = map(value,1024,0,0,100);
  Serial.print("Plant Mositure: ");
  Serial.print(value);
  Serial.print("%");
  
  client.publish("officeplant/moisture", itoa(value, stringValue, 10));

  // When this is battery powered, I want to receive battery voltage
  Serial.print(ESP.getVcc());
  //client.publish("officeplant/battery", ESP.getVcc());

  delay(100);
  deepSleep();
}

/**
 * Setup wifi
 */
void setupWifi() {
  delay(10);

  // Start connecting to the wifi
  Serial.println();
  Serial.print("Connecting to....");
  Serial.print(ssid);

  WiFi.begin(ssid, password);

  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print("*");
  }

  delay(100);
  Serial.println();
  Serial.println("WiFi connected!!");
  Serial.println("IP Address: ");
  Serial.println(WiFi.localIP());
}

/**
 * Deep sleep
 */
void deepSleep() {
  Serial.println("Deep Sleeping...");
  WiFi.mode(WIFI_OFF);
  WiFi.forceSleepBegin();
  delay(1);
  ESP.deepSleep(interval, WAKE_RF_DEFAULT);
}


/**
 * Loop function
 */
void loop() {

}

/**
 * MQTT connect
 */
void mqtt_connect() {
  // loop until we connect
  while(!client.connected()) {
    Serial.println("Attempting MQTT connection...");

    // Attempt to connect
    if(client.connect("ESP8266Client")) {
      Serial.println("Connected!!!");
      
    } else {
      Serial.print("Failed rc=");
      Serial.print(client.state());
      Serial.println(" trying again in 5 seconds");
      delay(5000);
    }
  }
}
