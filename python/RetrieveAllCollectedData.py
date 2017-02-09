# Copyright (c) 2017, Marek Tomczewski
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

import requests
import json
import datetime, time, calendar

###
#
# This script retrieves data from all devices registered in specified account,
# creating a dump file for each day with data.
# It does not support days with more data that can be retrieved using single request.
# It retrieves original data instead of aggregated.
# Files are created in current directory.
#
###

# Configuration section
API_ENDPOINT = "https://dashboard.us.enableiot.com/v1/api" # Official public EnableIOT instance
ACCOUNT_NAME = "<account_name>" # Put your account name here
ACCOUNT_ID = "<account_id>" # Can be found in Account section
TOKEN = "<jwt_token>" # Retrieve using instructions available https://github.com/enableiot/iotkit-api/wiki/Authorization#get-user-jwt-token
CSV_HEADER_LEN = 81 # No need to change


def print_json(text):
    print json.dumps(text, indent=4, sort_keys=True)

headers = {
    "Content-Type": "application/json",
    "Authorization": "Bearer " + TOKEN
}

def get_devices():
    print 'Retrieving list of devices'
    r = requests.get(API_ENDPOINT + "/accounts/" + ACCOUNT_ID + '/devices', headers=headers)
    if r.status_code != 200:
        raise Exception('Getting devices failed!')
    print_json(r.json())
    return r.json()

def get_device_filter(device):
    filter = {
        "targetFilter": {
            "deviceList": [
                device['name']
            ]
        },
        "metrics": [
            {
                "id": comp['cid'],
                "op": "none"
            }
            for comp in device['components']
            ]
    }
    return filter

def retrieve_borders(device):
    body = {
        "components": [comp['cid'] for comp in device['components']]
    }
    r = requests.post(API_ENDPOINT + '/accounts/' + ACCOUNT_ID + '/data/firstLastMeasurementTimestamp',
                      json=body, headers=headers)
    if r.status_code != 200:
        print 'Issue occured during advanced calculations... Assuming tomorrow as end of period.'
        return datetime.date.fromtimestamp(0), datetime.date.today()
    minimum = time.time() * 1000
    maximum = 0
    for comp in r.json()['componentsFirstLast']:
        if comp['firstPointTS'] < minimum:
            minimum = comp['firstPointTS']
        if comp['lastPointTS'] > maximum:
            maximum = comp['lastPointTS']
    if minimum > maximum:
        print 'Device', device['name'], 'was not collecting data at all'
    else:
        print 'Device', device['name'], \
            'was collecting data from', datetime.date.fromtimestamp(minimum / 1000).isoformat(), \
            'to', datetime.date.fromtimestamp(maximum / 1000).isoformat()
    return datetime.date.fromtimestamp(minimum / 1000), \
           datetime.date.fromtimestamp(maximum / 1000) + datetime.timedelta(days=1)

def retrieve_from_period(body, start_date, end_date):
    print 'Trying to get data for device ', device['name'], \
        'from', start_date.isoformat(), \
        'to', end_date.isoformat()
    body['from'] = int(calendar.timegm(start_date.timetuple()) * 1000)
    body['to'] = int(calendar.timegm(end_date.timetuple()) * 1000)
    r = requests.post(API_ENDPOINT + '/accounts/' + ACCOUNT_ID + '/data/search?output=csv', json=body, headers=headers)
    if r.status_code == 401:
        raise Exception('Token has expired! Status code: ' + str(r.status_code))
    if r.status_code >= 500:
        raise Exception('Getting data failed! Status code: ' + str(r.status_code))
    return r.status_code, r.text

def retrieve_data_collected(device):
    device_filter = get_device_filter(device)
    first, end = retrieve_borders(device)
    while True:
        start = end - datetime.timedelta(days=1)
        code, csv = retrieve_from_period(device_filter, start, end)
        if code == 413:
            raise Exception('You had more than 500k data points per day! '
                            'To retrieve that, period requested should be for shorter time than a day.')
        if code == 200 and len(csv) > CSV_HEADER_LEN:
            with open(ACCOUNT_NAME + '_' + device['name'] + '_' + start.isoformat() + '.csv', 'w') as f:
                 f.write(csv)
                 print "Created file " + f.name + ' of size ' + str(len(csv))
        end = start

        code, csv = retrieve_from_period(device_filter, first, end)
        if code == 200 and len(csv) == CSV_HEADER_LEN:
            print 'Completed'
            return

for device in get_devices():
    print 'Processing device', device['name']
    if 'components' in device:
        retrieve_data_collected(device)
    else:
        print 'Device does not have any component.'
