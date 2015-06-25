#!/usr/bin/env python
# Copyright (c) 2015, Intel Corporation
#
# Redistribution and use in source and binary forms, with or without modification,
# are permitted provided that the following conditions are met:
#
#    * Redistributions of source code must retain the above copyright notice,
#      this list of conditions and the following disclaimer.
#    * Redistributions in binary form must reproduce the above copyright notice,
#      this list of conditions and the following disclaimer in the documentation
#      and/or other materials provided with the distribution.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
# ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
# WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
# DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
# ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
# (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
# LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
# ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
# (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
# SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

# This script opens a secure connection directly to the IoT Analytics server
# using the device credentials and requests the server health status

import paho.mqtt.client as mqtt
import paho.mqtt.publish as publish
import ssl
import time
import json
import random

# Get device/account settings from the iotkit-agent configuration file 
# (data/device.json) and fill in the variables below
device_id = "<device ID>"
account_id = "<account ID>"
comp_id = "<component ID>"
token = "<device token>"

broker_host = "broker.us.enableiot.com"
broker_port = 8883

# Server-health topics 
ptopic = "server/devices/%s/health" % device_id
stopic = "device/%s/health" % device_id

# The callback for when a PUBLISH message is received from the server.
def on_message(client, userdata, msg):
    print(msg.topic+" "+str(msg.payload))
    client.disconnect()

# Prepare secure MQTT client using iotkit credentials
mqttc = mqtt.Client()
mqttc.tls_set(ca_certs="/usr/lib/node_modules/iotkit-agent/certs/AddTrust_External_Root.pem")
mqttc.tls_insecure_set(True)     # prevents error - ssl.SSLError: Certificate subject does not match remote hostname.
mqttc.username_pw_set(device_id, token)   # username, password
mqttc.on_message = on_message

# Connect client
mqttc.connect(broker_host, broker_port)

# Subscribe to server health responses
mqttc.subscribe(stopic)

# Publish server-health request
payload = { "detail": "mqtt" }
print "pub topic:", ptopic
mqttc.publish(ptopic, json.dumps(payload))
mqttc.loop_forever()
