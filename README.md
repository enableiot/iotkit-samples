# IoT Kit Samples

Samples illustrating how to develop applications on the IoT Kit platform. 

> Note: this repo, while currently protected, will be made public. Put here only things intended for public consumption. 

![IoT Kit Overview](https://dl.dropboxusercontent.com/u/202873466/IotKit/iotkit-sample-splash.png)

While the list of applications one could develop on this platform is endless, the samples listed in this repo demonstrate how to collect data from connected sensors, format it and relay that data to the Cloud portion of IoT Kit for analytics: MapReduce, SQL-based query or a simple download for off-line processing.

## Usage

Currently the `iotkit-agent` supports the following protocols: 

* MQTT
* REST 
* UDP
* TCP

### Message Format

When integrating the IoT Kit Agent you only have to provide the metric information, everything else will be provided by the agent before your message is relayed to the cloud. Regardless of the protocol used, the `iotkit-agent` expect the inbound message to be in following simple format:

    { "s": "temp-sensor", "m": "air-temp", "v": 26.7 }

Where in:

* s - is the source of this measurement
* m - is the name of this measurement
* v - is the value of this measurement

## Protocol-specific API

Many development frameworks have their own implementation of each one of these protocols. The following command-line examples should give you an idea how to access `iotkit-agent` API:

#### MQTT

Any development framework supporting MQTT client can use local agent. Here is a mosquitto_pub example `tests/mqtt-test.sh`:

    mosquitto_pub -t 'data' \
                  -m '{ "s": "temp-sensor", "m": "air-temp", "v": 26.7 }'
                  
> Note the -t [topic] is required but it can be anything

#### REST (HTTP)

Most development framework have an integrated Web Request object. Here is a curl example `tests/rest-test.sh`:

    curl -i -X PUT http://127.0.0.1:9090/ \
    	  -H 'Content-Type: application/json' \
         --data '{ "s": "temp-sensor", "m": "air-temp", "v": 26.7 }' 
         
#### UDP

Even if your development framework does not support MQTT client or Web Request, you can still use UDP to send data to the Cloud. Here is a command line example `tests/udp-test.sh`:

    echo -n '{ "s": "temp-sensor", "m": "air-temp", "v": 26.7 }' | \
         nc -4u -w1 127.0.0.1 41234
         
#### TCP

If assuring the message delivery to the `iotkit-agent` is important to you (yes, I'm talking about you UDP) you can use a simple TCP socket connection to send data to the Cloud. Here is a command line example `tests/tcp-test.sh`:

    echo -n '{"s": "temp-sensor", "m": "air-temp", "v": 26.7}' | \
         nc 127.0.0.1 7070
        
### Arduino

####  Steps
1. Ensure you have the latest SD card image
2. Ensure you have the Arduino IDE (for Galileo) installed, the correct Serial/USB driver, and the correct COM port set. You should be able to download a simple sketch such as the "Blink" example and see the LED blink.
3. Import the IoTkit Arduino library (instuctions below).
4. Open the IoTkitStoreCredentials example,change the username and password, and download it to the board. This should set the credentials. This will not need to be done again unless you replace or overwrite the SD card.
5. Reset the board by pressing the "REBOOT" button (or remove and reinsert the power connector). This will enable the agent to use the new username and password parameters.
5. Open the IoTkitSetDateTime example, modify the function parameters for the current UTC date and time and download it to the board. This will need to be done each time power is applied to the board unless a battery has been connected to the COIN header.
6. Open the SimpleIoTkitExample and download it to the board.
7. Confirm data is being sent to the broker. There are several ways to do this, including subscribung to the broker.

##### Steps to install the IoTkit Arduino library and examples

1. Download the IoTkit.zip file from the Arduino directory
2. From the Arduino IDE, select Sketch -> Import Library -> Add Library
3. Select the zip file, click "open"
4. You should see a message like: Library added to your libraries. Check "Import Library" menu.
4. Select File -> Examples -> IoTkit -> SimpleIoTkitExample
5. This should open a new sketch window with an example
6. Ensure the agent is running on the Galileo
7. Compile and Upload the sketch
8. You should see a new measurements for: source: "temp-Sensor" measurement: "temp" value: 26.42

