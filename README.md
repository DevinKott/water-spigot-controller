# water-spigot-controller
Code uploaded to an Arduino that is used to control a water spigot via WiFi/MQTT.

## Description
This code uses a WiFi-enabled Arduino board to communicate with an MQTT broker. It just so happens that a solenoid valve used for controlling water flow is attached as well.


## Build Materials

1. Any Arduino that has digital IO pins, 12V power passthrough via the Vin pin, and WiFi. I chose the [Arduino Uno WiFi Rev2](https://store.arduino.cc/usa/arduino-uno-wifi-rev2). Based on the title, the board has WiFi. If you look at the tech specs section of the product description, you can also verify that the board has an Input Voltage of 7-12V (via the barrel connector) and has Digital IO pins.
2. A 12V power source. I just found an old laptop charger and verified that it output 12V.
3. USB 2.0 Cable Type A/B. I had one on hand, but you can grab one on the [arduino store](https://store.arduino.cc/usa/usb-2-0-cable-type-a-b) for $3.95. This cable is used to upload code to the board.
4. Solenoid Valve. I used [this one](https://www.amazon.com/gp/product/B00EIB1BSC) from Adafruit.
5. Breadboard
6. [TIP120 Darling Transistor](https://www.amazon.com/gp/product/B06ZY654XN)
7. 1K Ohm Resistor
8. [1N4001 Diode](https://www.amazon.com/gp/product/B07BVKQ6WY/)
9. Jumper Wires


## Instructions

1. I followed [this](https://bc-robotics.com/tutorials/controlling-a-solenoid-valve-with-arduino/) tutorial to get the solenoid working with my Arduino.
2. I added MQTT capabilities into the program and tested using a generic linux mqtt broker Use any MQTT broker to control the solenoid. I just happened to use HA (see below).

### HomeAssistant

I run HomeAssistant (HA)/NodeRED and wanted a button/toggle in my UI to trigger the solenoid:

1. In your *configuration.yaml* file, add a new *input_boolean*. I called mine '*water_control*'.
2. Restart HA for the new entity to be loaded.
3. Configure your UI to add the entity as a switch/toggle/button/whatever. I just happened to use a Button card. Enter the entity ID that you configured in the configuration file (mine was *input_boolean.water_control*).
4. Head over to NodeRED. Put down a HA state_changed node and point it to the newly-created entity. Set the *If State* to '*on*'. Leave the rest as defaults.
5. Add a *change* node, and set *msg.payload* to '*enable*'. Hook the *true* state of the *state_changed* node to the input of this change node.
6. Copy the step 5 node, but set the payload to '*disable*' and hook up the *false* state to the input.
7. Create a MQTT output node, set the topic that you set in this program (again, mine was *garden/spigot*) and hook up the outputs of the change nodes to the input of thie MQTT out node.
8. Deploy your changes.


## Building

1. Clone this repository.
2. Create a new project in the Arduino IDE.
3. Copy/replace the file with the cloned file.
4. Add the **ArduinoMqttClient** and **WiFiNINA** libraries through the library manager.
5. Click verify in the Arduino IDE to make sure it can compile correctly.


## Replacing Values

1. On line 5, replace **4** with whatever pin you connected the solenoid to.
2. On lines 7 and 8, replace **WiFiName** and **WiFiPass** with your respective credentials for your own WiFi network.
3. On line 13, replace **BrokerIP** with the IP/URL of your MQTT service.
4. On line 14, replace **1883** with the port you set your MQTT service to run on. By default, it is **1883**.
5. On line 15 and 16, you can either keep or replace the topics.
6. On line 47, replace **MQTTUsername** and **MQTTPassword** with your respective credentials for your own MQTT service.
