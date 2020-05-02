/**
 * MQTT Publish/Subscribe Demo Program
 * 
 */
#include <ESP8266WiFi.h>
#include <MQTTClient.h>

/* Wi-Fi SSID name and password */
const char *ssid = "";
const char *pass = "";

/* MQTT Config */
const char *mqttEndPoint = ""; /* MQTT endpoint */
const int mqttPort = 1883;  /* MQTT port number */
const char *mqttUsername = ""; /* MQTT username */
const char *mqttPassword = ""; /* MQTT password */
const char *publishTopic = ""; /* MQTT publish topic */
const char *subscribeTopic = ""; /* MQTT subscribe topic */
const int transmissionIntervalMS = 10000; /* 10 seconds interval */

/* MQTT Client */
MQTTClient mqttClient;

/* WiFi Client */
WiFiClient wifiClient;

/* Miscellaneous */
unsigned long lastMillis = 0;
char msg[20];
int count = 0;


void setup() {
    Serial.begin(115200);
    WiFi.begin(ssid, pass);

    mqttClient.begin(mqttEndPoint, mqttPort, wifiClient);
    mqttClient.onMessage(messageReceived);
    
    connect();
}


void connect() {
  Serial.println("\nChecking Wi-Fi...");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(1000);
  }

  Serial.println("Wi-Fi connected");
  Serial.println("Connecting to MQTT broker...");
  while (!mqttClient.connect("ESP32", mqttUsername, mqttPassword)) {
    Serial.print(".");
    delay(100);
  }

  Serial.println("Connected to MQTT broker!");

  if (subscribeTopic != "") {
    mqttClient.subscribe(subscribeTopic);
    Serial.println("Subscribed successfully");
  }
}


void loop() {
  if (!mqttClient.connected()) {
    connect();
  }
  mqttClient.loop();
  delay(100);

  if (millis() - lastMillis > transmissionIntervalMS) {   
    lastMillis = millis();
    ++count;
    snprintf (msg, 20, "Message #%d", count);
    Serial.print("Publish message: ");
    Serial.println(msg);
    mqttClient.publish(publishTopic, msg);
  }
}


void messageReceived(String &topic, String &payload) {
  Serial.println("Received message: " + topic + " - " + payload);

  // Note: Do not use the client in the callback to publish, subscribe or
  // unsubscribe as it may cause deadlocks when other things arrive while
  // sending and receiving acknowledgments. Instead, change a global variable,
  // or push to a queue and handle it in the loop after calling `client.loop()`.
}
