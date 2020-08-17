#include <ArduinoMqttClient.h>
#include <SPI.h>
#include <WiFiNINA.h>

int solenoidPin = 4;

const char ssid[] = "WiFiName";
const char pass[] = "WiFiPass";

WiFiClient wifiClient;
MqttClient mqttClient(wifiClient);

const char  broker[]  = "BrokerIP";
int         port      = 1883;
const char  topic[]   = "garden/spigot";
const char  sendTop[] = "garden/spigot/status";
bool opened = false;


void setup() {
  // Set pin outputs.
  pinMode(solenoidPin, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);

  // Turn on setup LED.
  digitalWrite(LED_BUILTIN, HIGH);

  // Wait for Serial connection to establish.
  Serial.begin(9600);
  while (!Serial) {
    ;
  }

  // Connect to the WiFi.
  Serial.print("Attempting to connect to WPA SSID: ");
  Serial.println(ssid);
  while (WiFi.begin(ssid, pass) != WL_CONNECTED) {
    Serial.print(".");
    delay(5000);
  }
  Serial.println();
  Serial.println("You're connected to the network.");
  Serial.println();

  // Set up the MQTT client information
  mqttClient.setId("ClientID");
  mqttClient.setUsernamePassword("MQTTUsername", "MQTTPassword");

  // Connect to the MQTT broker.
  Serial.print("Attempting to connect to the MQTT broker: ");
  Serial.println(broker);
  if (!mqttClient.connect(broker, port)) {
    Serial.print("MQTT connection failed! Error code = ");
    Serial.println(mqttClient.connectError());

    while (1);
  }
  Serial.println("You're connected to the MQTT broker.");
  Serial.println();

  // Set up a handler for messages.
  mqttClient.onMessage(onMqttMessage);

  // Subscribe to our topic.
  Serial.print("Subscribing to topic: ");
  Serial.println(topic);
  mqttClient.subscribe(topic);

  // Turn off setup LED.
  Serial.println("Setup done. Turning off LED.");
  digitalWrite(LED_BUILTIN, LOW);
}

// While looping, maintain a connection to the MQTT service by calling 'poll()'.
void loop() {
  mqttClient.poll();
}

// Helper function that can send messages to the status topic.
void sendStatusMessage(const char stat[]) {
    mqttClient.beginMessage(sendTop);
    mqttClient.print(stat);
    mqttClient.endMessage();
}

// This function is called when we receive a message that we subscribed to.
void onMqttMessage(int messageSize) {
    // Print some information.
    Serial.print("Received a message with topic '");
    Serial.print(mqttClient.messageTopic());
    Serial.print("', length ");
    Serial.print(messageSize);
    Serial.println(" bytes: ");

    // Copy the message into a string
    char msg[messageSize + 1];
    int c = 0;
    while (mqttClient.available()) {
      msg[c++] = (char) mqttClient.read();
    }
    msg[messageSize] = '\0';

    // Print that message.
    Serial.print("The message received was: ");
    Serial.println(msg);
    Serial.println();

    // Depending on the message ('enable' or 'disable'), open or close the solenoid.
    int ret = strcmp(msg, "enable");
    if (ret == 0) {
      if (opened == true) {
        Serial.println("Solenoid is already open.");
      } else {
        digitalWrite(solenoidPin, HIGH);
        opened = true;
        Serial.println("Opened solenoid. Water can flow.");
        sendStatusMessage("Valve has been opened to allow water to flow.");
      }
    } else {
      if (opened == true) {
        digitalWrite(solenoidPin, LOW);
        opened = false;
        Serial.println("Closed solenoid. No water is flowing.");
        sendStatusMessage("Valve has been closed.");
      } else {
        Serial.println("Solenoid is already closed."); 
      }
    }
}
