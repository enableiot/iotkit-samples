# IoT Kit Samples

Samples illustrating application development on the IoT Kit platform. 

## Usage

These samples assume you have already installed (or have access to) the [iotkit-agent](https://github.com/enableiot/iotkit-agent) which supports the following protocols: 

* UDP
* TCP

### Setup

In order to use `iotkit-agent` you have to create an account on iotkit dashboard [iotkit-dashboard](https://dashboard.enableiot.com). Once you verify the registered email address you will be able to add individual devices. The rest of this document assumes you have already registered your devices in the Cloud.

### Component Registration 

In order to submit data to the IoT Kit Cloud, the individual components have to be registered first. Regardless of the protocol used, the `iotkit-agent` expects the inbound component registration message to be in following simple format:

    { "n": "temperature sensor", "t": "temperature.v1.0"}
    
OR

    { "n": "humidity sensor", "t": "humidity.v1.0"}
    
Where:

* n - the component name ("Temperature", "Humidity", "Weight", "Force", etc.)
* t - is the component type of data this source generates (This should be one of the Component Type defined in your account Catalog available in the [iotkit-dashboard](https://dashboard.enableiot.com))

> The registration needs to be performed only once for each new metric

### Data Submission 

Once the metric has been registered, you can send your observations for that metric to the cloud. Everything else will be provided by the agent before your message is relayed to the cloud. Regardless of the protocol used, the `iotkit-agent` expects the inbound message to be in following format:

    { "s": "Temperature", "v": 26.7 }

Where:

* s - is the metric name which was previous registered
* v - is the value of this measurement

## Protocol-specific API

Many development frameworks have their own implementation of each one of these protocols. The following command-line examples should give you an idea how to access `iotkit-agent` API:

#### MQTT

Any development framework supporting MQTT client can use the local agent. Here is a mosquitto_pub example `tests/mqtt-test.sh`:

    mosquitto_pub -t 'metric' -m '{ "s": "Temperature", "v": 26.7 }'
                  
> Note the -t [topic] is required but it can be anything

#### REST (HTTP)

Most development framework have an integrated Web Request object. Here is a curl example `tests/rest-test.sh`:

    curl -i -X PUT http://127.0.0.1:9090/ \
    	  -H 'Content-Type: application/json' \
         --data '{ "s": "Temperature", "v": 26.7 }' 
         
#### UDP

Even if your development framework does not support MQTT client or Web Request, you can still use UDP to send data to the Cloud. Here is a command line example `tests/udp-test.sh`:

    echo -n '{ "s": "Temperature", "v": 26.7 }' | nc -4u -w1 127.0.0.1 41234
         
#### TCP

If assuring the message delivery to the `iotkit-agent` is important to you (yes, I'm talking about you UDP) you can use a simple TCP socket connection to send your data. Here is a command line example `tests/tcp-test.sh`:

    echo -n '{ "s": "Temperature", "v": 26.7 }' | nc 127.0.0.1 7070
        
## How to

See our [wiki pages](https://github.com/enableiot/iotkit-samples/wiki) for additional information (FAQs, Arduino, Data access etc.)

[How to for Arduino](https://github.com/enableiot/iotkit-samples/wiki/How-to-for-Arduino)

## Feedback 

Yes please... use the [Issues Section](https://github.com/enableiot/iotkit-samples/issues) to report bugs, recommend enhancements, or simply ask questions. 
