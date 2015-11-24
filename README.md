# IoT Kit Samples

Samples illustrating application development on the IoT Kit platform. 

## Usage

These samples assume you have already installed (or have access to) the [iotkit-agent](https://github.com/enableiot/iotkit-agent) which supports the following protocols: 

* UDP
* TCP

### Setup

In order to use `iotkit-agent` you have to create an account on iotkit dashboard [iotkit-dashboard](https://dashboard.enableiot.com). Once you verify the registered email address you will be able to add individual devices. The rest of this document assumes you have already registered your devices in the Cloud.

### Sensor Registration 

In order to submit data to the IoT Kit Cloud, the individual sensors have to be registered first. 
The sensors type must be included in the Component Catalog associated with your [iotkit-dashboard](https://dashboard.enableiot.com) account.

> Obtain the sensors types by executing the command `catalog`. Check how to execute this command in the [iotkit-agent Readme](https://github.com/enableiot/iotkit-agent/blob/master/README.md#5-notes-about-admin-commands).

Regardless of the protocol used, the `iotkit-agent` expects the inbound sensor registration message to be in following simple format:

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

* _sensor name_: the sensor name ("Temperature", "Humidity", "Weight", "Force", etc.). 
* _sensor type_: is the sensor type of data this source generates (This should be one of the Component Type defined in your account Catalog available in the [iotkit-dashboard](https://dashboard.enableiot.com))
* _size_: is the message length

> The registration needs to be performed only once for each new sensor

##### Usage of `register` command
For **testing purposes**, you can use the command `iotkit-admin register <comp_name> <catalogid>`. The <catalogid> is obtained with the `catalog` command mentioned before.
> It's not required that the iotkit-agent be running to execute this command. The command will start the iotkit-agent, register the component and stop it.

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

##### Usage of `observation` command
You can also use the command `iotkit-admin observation <comp_name> <value>` (just for **testing purposes**) to submit data for a specific component.

As command `register`, this will start the iotkit-agent, send the observation for the component and then stop the iotkit-agent.

> For more information about `iotkit-admin` commands, go to section [Notes about "admin" commands](https://github.com/enableiot/iotkit-agent#5-notes-about-admin-commands) of iotkit-agent readme file.

## Protocol-specific API

Many development frameworks have their own implementation of each one of these protocols. The following command-line examples should give you an idea how to access `iotkit-agent` API:


#### TCP

If assuring the message delivery to the `iotkit-agent` is important you can use a simple TCP socket connection to send your data. Here is a command line example:

    echo -n '52#{ "n": "temp sensor", "v": "5", "on": 1401893417000}' | nc 127.0.0.1 7070

#### UDP

You use same way to test UDP connection:

    echo -n '{ "n": "temp sensor", "v": "5", "on": 1401893417000}' | nc -u 127.0.0.1 41234


## How to

See our [wiki pages](https://github.com/enableiot/iotkit-samples/wiki) for additional information (FAQs, Arduino, Data access etc.)

[How to for Arduino](https://github.com/enableiot/iotkit-samples/wiki/How-to-for-Arduino)

## Feedback 

Yes please... use the [Issues Section](https://github.com/enableiot/iotkit-samples/issues) to report bugs, recommend enhancements, or simply ask questions. 
