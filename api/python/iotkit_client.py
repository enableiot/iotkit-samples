#!/usr/bin/env python 

###################################################################
# This program will:
#   * Authenticate a user using existing credentials
#   * Create a device
#   * Activate the device (but currently does not persist the token)
#   * Register 2 time series for the device - one for temperature and
#     one for humidity (The component types for these are already
#     defined in the account catalog)
#   * Send observations for both time series. As configured it will 
#     send one per hour for the last 25 hours for each time series.
#   * Read the observations back and display the count.
#
#  To use:
#   * On the web:
#       * Go to https://dashboard.us.enableiot.com
#       * Register - be sure to click the "Sign Up Here" link. Do not use
#       * any of the OAuth options, or you will not be able to use the API.#
#       * Verify your email address
#       * Enter a name for your account
#   * Below line 39 in this file:
#       * Update your username, password and account_name
#       * Update the proxy address if required
#       * Update the device id below. The device id MUST be unique or
#         the step to create the device will fail
#   * Install the python "requests" library. You can use Python 
#     virtual environments, or install it globally:
#       $ pip install requests
#   * Run the program
#       $ python iotkit_client.py
#

import sys
import requests
import json
import uuid
import time
import random

#####################################
# Set these values first
#####################################
host = "dashboard.us.enableiot.com"

proxies = {
    # "https": "http://proxy.example.com:8080"
}

username = "my_email_address@example.com"
password = "myPassword"
account_name = "myAccountName"

#this will create a device with this id - error if it already exists
device_id = "57798f4b-2b1c-4cea-84f1-ac45bf6ae9a2"

# this will create {observations_per_hour} observations per hour for {days_of_data} days
observations_per_hour = 1
days_of_data = 1

verify = True # whether to verify certs
#####################################

api_root = "/v1/api"
base_url = "https://{0}{1}".format(host, api_root)
device_name = "Device-{0}".format(device_id)

g_user_token = ""
g_device_token = ""


def main():
    global g_user_token, g_device_token

    # get an authentication token for use in the following API calls.
    # It will be put in every header by get_user_headers()
    g_user_token = get_token(username, password)

    # get my user_id (uid) within the Intel IoT Analytics Platform
    uid = get_user_id()
    print "UserId: {0}".format(uid)

    # for all the accounts I have access to, find the first account 
    # with the name {account_name} and return the account_id (aid)
    aid = get_account_id(uid, account_name)
    print "AccountId: {0}".format(aid)

    # create a new device within the account - error if a device with
    # device_id already exists in the system even if it's in another account
    create_device(aid, device_id, device_name)

    # refresh the activation code. It can be used any number of times
    # in the next 60 minutes to activate devices.
    ac = generate_activation_code(aid)
    print "Activation code: {0}".format(ac)

    # activate the device. This returns an authentication token that the device
    # can use to register time series and send observations. It will be put in
    # every header for device calls by get_device_headers(). You MUST persist
    # this is you want to send additional observations at a later time.
    g_device_token = activate(aid, device_id, ac)

    # this registers a time series for this device. The component will have a
    # Component Type of "temperature.v1.0" which defines the data type of the
    # value, the format, the unit of measure, etc. This way, we don't need to
    # specify all of that here.  
    # Within the scope of the device, this time series will be named "temp".
    # This call returns the component_id (cid) which is globally unique
    # within the Intel IoT Analytics platform.
    cid = create_component(aid, device_id, "temperature.v1.0", "temp")
    print "ComponentID (cid): {0}".format(cid)

    # create some random observations around 23 +/- 1 in the new {cid} time series.
    create_observations(aid, device_id, cid, 23, 1)

    # read back the observation we just created.
    o = get_observations(aid, device_id, cid)
    print_observation_counts(o)

    # create a second time series for humidity.
    cid2 = create_component(aid, device_id, "humidity.v1.0", "humidity")
    print "ComponentID (cid): {0}".format(cid2)

    # create some random observations around 21 +/- 1
    create_observations(aid, device_id, cid2, 21, 1)

    o2 = get_observations(aid, device_id, cid2)
    print_observation_counts(o2)


def get_user_headers():
    headers = {
        'Authorization': 'Bearer ' + g_user_token,
        'content-type': 'application/json'
    }
    #print "Headers = " + str(headers)
    return headers


def get_device_headers():
    headers = {
        'Authorization': 'Bearer ' + g_device_token,
        'content-type': 'application/json'
    }
    #print "Headers = " + str(headers)
    return headers


def check(resp, code):
    if resp.status_code != code:
        print "Expected {0}. Got {1} {2}".format(code, resp.status_code, resp.text)
        sys.exit(1)


# Given a username and password, get the user token
def get_token(username, password):
    url = "{0}/auth/token".format(base_url)
    headers = {'content-type': 'application/json'}
    payload = {"username": username, "password": password}
    data = json.dumps(payload)
    resp = requests.post(url, data=data, headers=headers, proxies=proxies, verify=verify)
    check(resp, 200)
    js = resp.json()
    token = js['token']
    return token


# given a user token, get the user_id
def get_user_id():
    url = "{0}/auth/tokenInfo".format(base_url)
    resp = requests.get(url, headers=get_user_headers(), proxies=proxies, verify=verify)
    check(resp, 200)
    js = resp.json()
    #print js
    user_id = js["payload"]["sub"]
    return user_id


# given a user_id, get the account_id of the associated account with account_name
# if there are multiple accounts with the same name, return one of them
def get_account_id(user_id, account_name):
    url = "{0}/users/{1}".format(base_url, user_id)
    resp = requests.get(url, headers=get_user_headers(), proxies=proxies, verify=verify)
    check(resp, 200)
    js = resp.json()
    if 'accounts' in js:
        accounts = js["accounts"]
        for k, v in accounts.iteritems():
            if 'name' in v and v["name"] == account_name:
                return k
    print "Account name {0} not found.".format(account_name)
    print "Available accounts are: {0}".format([v["name"] for k, v in accounts.iteritems()])
    return None


# create a device
def create_device(account, device_id, device_name):
    url = "{0}/accounts/{1}/devices".format(base_url, account)
    device = {
        "deviceId": str(device_id),
        "gatewayId": str(device_id),
        "name": device_name,
        "tags": ["US", "California", "San Francisco"],
        # if the device will be static, use this
        # to remember where you put it
        #"loc": [37.783944, -122.401289, 17],
        "attributes": {
            "vendor": "intel",
            "platform": "x86",
            "os": "linux"
        }
    }
    data = json.dumps(device)
    resp = requests.post(url, data=data, headers=get_user_headers(), proxies=proxies, verify=verify)
    check(resp, 201)
    return resp


# Generate an activation code and return it
# This activation code will be good for 60 minutes
def generate_activation_code(account_id):
    url = "{0}/accounts/{1}/activationcode/refresh".format(base_url, account_id)
    resp = requests.put(url, headers=get_user_headers(), proxies=proxies, verify=verify)
    check(resp, 200)
    js = resp.json()
    activation_code = js["activationCode"]
    return activation_code


# Activate a device using a valid activation code
def activate(account_id, device_id, activation_code):
    url = "{0}/accounts/{1}/devices/{2}/activation".format(base_url, account_id, device_id)
    activation = {
        "activationCode": activation_code
    }
    data = json.dumps(activation)
    resp = requests.put(url, data=data, headers=get_user_headers(), proxies=proxies, verify=verify)
    check(resp, 200)
    js = resp.json()
    if "deviceToken" in js:
        token = js["deviceToken"]
        return token
    else:
        print js
        sys.exit(1)


# Given an account_id and device_id, and a component type name and name - create a component and return the cid
def create_component(account_id, device_id, component_type_name, name):
    url = "{0}/accounts/{1}/devices/{2}/components".format(base_url, account_id, device_id)
    component = {
        "type": component_type_name,
        "name": name,
        "cid": str(uuid.uuid4())
    }
    data = json.dumps(component)
    resp = requests.post(url, data=data, headers=get_device_headers(), proxies=proxies, verify=verify)
    check(resp, 201)
    js = resp.json()
    return js["cid"]


# Create several observations and submit them
# Create {observations_per_hour} observations per hour for {days_of_data} days
def create_observations(account_id, device_id, cid, mid, rang):
    url = "{0}/data/{1}".format(base_url, device_id)
    now = int(time.time()) * 1000;
    start = now - days_of_data * 24 * 60 * 60 * 1000
    body = {
        "on": start,
        "accountId": account_id,
        "data": []
    }
    # n observation per hour per day
    for i in range(int(days_of_data * 24 * observations_per_hour) + 1):
        val = round(mid - rang + (random.random() * rang * 2), 1)  # random number from mid-range to mid+range
        #print "val={0}".format(val)
        o = {
            "componentId": cid,
            "on": start + i * (60 / observations_per_hour) * 60 * 1000,
            # if the device is mobile, you can record where it was when
            # this observation was captured
            #"loc": [ 45.5434085, -122.654422, 124.3 ],
            "value": str(val),
            "attributes": {
                "i": i
            }
        }
        body["data"].append(o)
    data = json.dumps(body)
    #print "data={0}".format(data)
    resp = requests.post(url, data=data, headers=get_device_headers(), proxies=proxies, verify=verify)
    check(resp, 201)


#get_observations
def get_observations(account_id, device_id, component_id):
    url = "{0}/accounts/{1}/data/search".format(base_url, account_id)
    search = {
        "from": 0,
        "targetFilter": {
            "deviceList": [device_id]
        },
        "metrics": [
            {
                "id": component_id
            }
        ]
        # This will include lat, lon and alt keys
        #,"queryMeasureLocation": True
    }
    data = json.dumps(search)
    resp = requests.post(url, data=data, headers=get_user_headers(), proxies=proxies, verify=verify)
    check(resp, 200)
    js = resp.json()
    return js


# print all of the device names and observation counts, sorted by device name
def print_observation_counts(js):  # js is result of /accounts/{account}/data/search
    if 'series' in js:
        series = js["series"]
        series = sorted(series, key=lambda v: v["deviceName"])
        for v in series:
            print "Device: {0} Count: {1}".format(v["deviceName"], len(v["points"]))


if __name__ == "__main__":
    main()