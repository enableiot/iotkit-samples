#!/usr/bin/env python

# Copyright © 2013 enableiot.com
# All rights reserved under the copyright laws 
# of the United States and applicable international 
# laws, treaties, and conventions.
# You may freely redistribute and use this sample 
# code, with or without modification, provided you 
# include the original copyright notice and use 
# restrictions.
# 
# Disclaimer: THE SAMPLE CODE IS PROVIDED "AS IS," 
#             AND ANY EXPRESS OR IMPLIED WARRANTIES, 
#             INCLUDING THE IMPLIED WARRANTIES OF 
#             MERCHANTABILITY AND FITNESS FOR A 
#             PARTICULAR PURPOSE, ARE DISCLAIMED. 
#             IN NO EVENT SHALL ENABLEIOT OR 
#             CONTRIBUTORS BE LIABLE FOR ANY DIRECT, 
#             INDIRECT, INCIDENTAL, SPECIAL, 
#             EXEMPLARY, OR CONSEQUENTIAL DAMAGES 
#             (INCLUDING, BUT NOT LIMITED TO, 
#             PROCUREMENT OF SUBSTITUTE GOODS OR 
#             SERVICES; LOSS OF USE, DATA, OR 
#             PROFITS; OR BUSINESS INTERRUPTION) 
#             SUSTAINED BY YOU OR A THIRD PARTY, 
#             HOWEVER CAUSED AND ON ANY THEORY OF 
#             LIABILITY, WHETHER IN CONTRACT, 
#             STRICT LIABILITY, OR TORT, ARISING 
#             IN ANY WAY OUT OF THE USE OF THIS 
#             SAMPLE CODE, EVEN IF ADVISED OF THE 
#             POSSIBILITY OF SUCH DAMAGE.

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
