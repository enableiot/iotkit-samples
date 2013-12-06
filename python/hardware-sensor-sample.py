import os
import socket
import time
import struct
import mosquitto
import json

if os.name == 'posix':
    import fcntl

interface = 'eth0'

# Orchestration variables 
is_debug = False
send_freq = 18 if is_debug else 60 # seconds
keep_alive = 30

# Message endpoint variables
root_topic = 'data'
account_id = os.environ.get('IOT_ACCOUNT_ID', 'a-101-sample')
device_id = os.environ.get('IOT_DEVICE_ID', 'd-123456-sample')
broker_endpoint = os.environ.get('IOT_BROKER_URI', 'localhost') # broker.enableiot.com')
broker_port = 1883

# Endpoint configuration
broker_topic = '/' + root_topic + '/' + account_id + '/' + device_id

# Local functions 

def getClientId():
    return socket.gethostname()


def getMacAddress(ifname):
    # from http://code.activestate.com/recipes/439094-get-the-ip-address-associated-with-a-network-inter/
    if os.name == 'posix':
        s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
        info = fcntl.ioctl(s.fileno(), 0x8927, struct.pack('256s', ifname[:15]))
        return '-'.join(['%02X' % ord(char) for char in info[18:24]])
    else:
        return '00-00-00-00-00-00'


def getTimestamp():
    return time.time()


def getTemp():
    temp = 0
    temp_src = "/sys/class/thermal/thermal_zone0/temp"

    if os.path.isfile(temp_src):
        try:
            f = open(temp_src)
            t = f.read()
            # temp = ((t/1000).toPrecision(3));
            temp = int(float(t)) / 1000
        except IOError:
            pass

    return temp


def makeItem(name, value):
    item = {
        'name': name,
        'sample': [
            {
                'value': value,
                'timestamp': getTimestamp()
            }
        ]
    }

    return item


def on_connect(mosq, obj, rc):
    if rc == 0:
        print("Connected successfully.")


def on_disconnect(mosq, obj, rc):
    print("Disconnected successfully.")


def on_publish(mosq, obj, mid):
    print("Message " + str(mid) + " published.")

# MQTT Client

client = mosquitto.Mosquitto(getClientId())
client.connect(broker_endpoint, broker_port, keep_alive)

if is_debug:
    client.on_connect = on_connect
    client.on_disconnect = on_disconnect
    client.on_publish = on_publish


def getMetrics():
    msg = {
        "msg_type": "metrics_msg",
        "sender_id": getMacAddress(interface),
        "account_id": account_id,
        "timestamp": getTimestamp(),
        "data_source": [
            {
                "name": "gateway_data_source",
                "metrics": []
            }
        ]
    }

    metrics = []

    #         # info about the CPU
    #         var cpus = os.cpus();

    #         # CPU
    #         for (var i in cpus) {
    #             metrics.push(makeItem("cpu_" + i + "_model",cpus[i].model));
    #             metrics.push(makeItem("cpu_" + i + "_speed",cpus[i].speed));
    #             metrics.push(makeItem("cpu_" + i + "_time_user",cpus[i].times.user));
    #             metrics.push(makeItem("cpu_" + i + "_time_sys",cpus[i].times.sys));
    #             metrics.push(makeItem("cpu_" + i + "time_idle",cpus[i].times.idle));
    #         }

    # Temp
    metrics.append(makeItem("temp", getTemp()))

    #         # Memory Total
    #         metrics.push(makeItem("total_memory", os.totalmem()));

    #         # Memory Free
    #         metrics.push(makeItem("free_memory", os.freemem()));

    #         # Uptime
    #         metrics.push(makeItem("uptime", os.uptime()));


    #         # Load averages
    #         /*
    #             The load average is a very UNIX-y concept;
    #             On Windows platforms this function always returns [0, 0, 0]
    #         */
    #         var loadavgs = os.loadavg();

    #         # 1, 5, 15 load avg min
    #         metrics.push(makeItem("min1_load_avg", loadavgs[0]));
    #         metrics.push(makeItem("min5_load_avg", loadavgs[1]));
    #         metrics.push(makeItem("min15_load_avg", loadavgs[2]));


    #         # Add metrics to the message
    msg['data_source'][0]['metrics'] = metrics

    # If in debug than print to console, else send to broker
    if is_debug:
        print json.dumps(msg, indent=4)
    else:
        client.publish(broker_topic, json.dumps(msg))

next_send_time = 0
while True:
    t = time.time()
    if t > next_send_time:
        getMetrics()
        next_send_time = t + send_freq
    client.loop(1)


