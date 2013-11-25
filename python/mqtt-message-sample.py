#!/usr/bin/env python
# 
# This script publish a random MQTT messages in specified cadence
#

import random
import time
import mosquitto

# Replace with environment variables 
cadence = int(6) 
broker = '127.0.0.1'
port = 1883
topic = 'data/mark/device-py1'
message = {
            "msg_type": "metrics_msg",
            "sender_id": "TODO_MAC_ADDRESS",
            "account_id": account_id,
            "timestamp": int(time.time()),
            "data_source": [
                {
                    "name": "gateway_data_source",
                    "metrics": []
                }
            ]
        }


print 'Messages are published on topic %s/#... -> CTRL + C to shutdown' \
    % topic

while True:
    client = mosquitto.Mosquitto("mqtt-py-test")
    # Add auth info
    client.connect(broker)
    client.publish(topic, message)
    time.sleep(cadence)
