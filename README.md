# IoT Kit Samples

Samples illustrating how to develop applications on the IoT Kit platform. 

![IoT Kit Overview](https://dl.dropboxusercontent.com/u/202873466/IotKit/iotkit-sample-splash.png)

While the list of applications one could develop on this platform is endless, the samples listed in this repo demonstrate how to collect data from connected sensors, format it and relay that data to the Cloud portion of IoT Kit for analytics.

## Usage

Currently the `iotkit-agent` supports the following protocols: 

* MQTT
* REST 
* UDP
* TCP

### Setup

In order to use `iotkit-agent` you have to create a [enableiot.com](http://enableiot.com) account. Once your account is verified you will be able to add individual devices. The rest of this document assumes you have already registered your devices in the Cloud.

### Sensor Configuration 

In order to submit data to the IoT Kit Cloud, the individual sensor have to be first registered. Regardless of the protocol used, the `iotkit-agent` expects the inbound sensor registration message to be in following simple format:

    { "s": "temp-sensor", "t": "float", "u": "Celsius" }
    
Where:

* s - is the data source (sensor)
* t - is the type of data this source generates
* u - is the unit of measure for this data type

> The device registration needs to be performed for each new sensor


### Data Submission 

Once the data source is configured, you can send to the Cloud your metric data. Everything else will be provided by the agent before your message is relayed to the cloud. Regardless of the protocol used, the `iotkit-agent` expect the inbound message to be in following format:

    { "s": "temp-sensor", "m": "air-temp", "v": 26.7 }

Where:

* s - is the source of this measurement
* m - is the name of this measurement
* v - is the value of this measurement

## Protocol-specific API

Many development frameworks have their own implementation of each one of these protocols. The following command-line examples should give you an idea how to access `iotkit-agent` API:

#### MQTT

Any development framework supporting MQTT client can use local agent. Here is a mosquitto_pub example `tests/mqtt-test.sh`:

    mosquitto_pub -t 'metric' \
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

If assuring the message delivery to the `iotkit-agent` is important to you (yes, I'm talking about you UDP) you can use a simple TCP socket connection to send your data. Here is a command line example `tests/tcp-test.sh`:

    echo -n '{"s": "temp-sensor", "m": "air-temp", "v": 26.7}' | \
         nc 127.0.0.1 7070
        
## How to

See our [wiki](https://github.com/enableiot/iotkit-samples/wiki) for additional information (FAQs, Arduino, Data access etc.)

