# IoT Kit Arduino Samples

Samples illustrating application development on the IoT Kit platform using Arduino. 
Before reading this, please read [parent README](../README.md "").

## Sample Sketch Descriptions

* **IoTKitActuationExample** - Sample sketch demonstrating how to listen for incoming actuation messages for local component, "power". Sketch should toggle pin 13 on/off - depending on the command in the actuation message. Actuation command can be sent from the "Control" menu on the IoT Analytics website.

* **IoTkitRegisterExample** - Sample sketch to demonstrate how to perform device setup and activation steps. The sketch sets the device ID, activates the device, *This sketch can be used to perform activation/setup on a new device. Be careful when running sketch if the device has already been activated* 

* **IotKitSendMetricsExample** - Sample sketch reads the cpu load, memory usage, voltage and uptime on Edison board and sends the data to the Intel IoTkit Cloud. It also sends 1 as power when agent is started.

* **IoTkitSimpleExample** - Sample sketch to read system temperature/voltage values and upload to the cloud. Temperature values are valid on **Galileo** devices by default. Modify the sketch to measure temperatures on **Edison** devices. 

