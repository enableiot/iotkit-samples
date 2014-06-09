# IoT Kit Samples

Samples illustrating application development on the IoT Kit platform. 

## Usage

These samples assume you have already installed (or have access to) the [iotkit-agent](https://github.com/enableiot/iotkit-agent) which supports the following protocols: 

* UDP
* TCP

### Setup

In order to use `iotkit-agent` you have to create an account on iotkit dashboard [iotkit-dashboard](https://dashboard.enableiot.com). Once you verify the registered email address you will be able to add individual devices. The rest of this document assumes you have already registered your devices in the Cloud.

### Sensor Registration 

In order to submit data to the IoT Kit Cloud, the individual sensors have to be registered first. Regardless of the protocol used, the `iotkit-agent` expects the inbound sensor registration message to be in following simple format:

#### UDP Message Format
    { "n": "<sensor name>", "t": "<sensor type>" }

    Examples:
    { "n": "temperature sensor", "t": "temperature.v1.0"}
    { "n": "humidity sensor", "t": "humidity.v1.0"}

#### TCP Message Format
    <size>#{ "n": "<sensor name>", "t": "<sensor type>" }

    Examples:
    53#{ "n": "temperature sensor", "t": "temperature.v1.0"}
    47#{ "n": "humidity sensor", "t": "humidity.v1.0"}

Where:

* sensor name: the sensor name ("Temperature", "Humidity", "Weight", "Force", etc.)
* sensor type: is the sensor type of data this source generates (This should be one of the Component Type defined in your account Catalog available in the [iotkit-dashboard](https://dashboard.enableiot.com))
* size: is the message length

> The registration needs to be performed only once for each new sensor

### Data Submission 

Once the sensor has been registered, you can send your observations for that sensor to the cloud. Everything else will be provided by the agent before your message is relayed to the cloud. Regardless of the protocol used, the `iotkit-agent` expects the inbound message to be in following format:

#### UDP Message Format
    { "n": "<sensor name>", "v": "<value>", "on": <on> }

    Examples:
    { "n": "temp sensor", "v": "5", "on": 1401893417000}

#### TCP Message Format
    <size>#{ "n": "<sensor name>", "v": "<value>", "on": <on> }

    Examples:
    52#{ "n": "temp sensor", "v": "5", "on": 1401893417000}

Where:

* sensor name: is the sensor name which was previously registered
* value: is the value of this observation
* on: optional: the observation timestamp

## Protocol-specific API

Many development frameworks have their own implementation of each one of these protocols. The following command-line examples should give you an idea how to access `iotkit-agent` API:

#### UDP

You can use UDP to send data to the Cloud. Here is a command line example:

    echo -n '{ "n": "temp sensor", "v": "5", "on": 1401893417000}' | nc -4u -w1 127.0.0.1 41234
         
#### TCP

If assuring the message delivery to the `iotkit-agent` is important to you (yes, I'm talking about you UDP) you can use a simple TCP socket connection to send your data. Here is a command line example:

    echo -n '52#{ "n": "temp sensor", "v": "5", "on": 1401893417000}' | nc 127.0.0.1 7070
        
## How to

See our [wiki pages](https://github.com/enableiot/iotkit-samples/wiki) for additional information (FAQs, Arduino, Data access etc.)

[How to for Arduino](https://github.com/enableiot/iotkit-samples/wiki/How-to-for-Arduino)

## Feedback 

Yes please... use the [Issues Section](https://github.com/enableiot/iotkit-samples/issues) to report bugs, recommend enhancements, or simply ask questions. 
