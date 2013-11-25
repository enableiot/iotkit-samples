# Metrics Data Flow
The content and volume of data collected on the edge may greatly vary across sensors and implementations. To enable maximum compatibility and to ensure that messages sent from the edge to the Cloud can be read and persisted a simple JSON message format is used for all Cloud submissions. This data flow message format assures portability without enforcing specific data types. This document outlines the specific message format and the MQTT topic to to which these messages need to be submitted. 
## Topic    data/{account_id}/{device_id}

Where:

* account_id - unique account id assigned by the a cloud provider
* device_id - globally unique id assigned by the sending device     
## Message

The content of the submission is defined by a simple schema:


#### root

    {
       "timestamp": <time of when this message was sent>,
       "account_id": <account id uniquely assigned by the cloud provider>,
       "sender_id": <id of the original provider>,
       "msg_type": <message type identifier>,
       "data_source": <array of data source metrics>
    }
    
#### data_source

    {
        "name": <name of the metric collection>,
        "metrics": <array of metric samples>
    }
    
#### metric

    {
        "name": "name of the sample",
        "sample": <array of sample values>
    }
    
#### sample

    {
        "name": "cpu_0_speed",
        "sample": <array of values>
    }
    
#### value

    {
        "value": <metric sample value>,
        "timestamp": <time when this sample value was collected>
    }

    
## Example   
Following is a sample of a simple message     {
       "timestamp": 1385381307414,
       "account_id": "mark",
       "sender_id": "02:36:64:eb:f6:2d",
       "msg_type": "metrics_msg",
       "data_source": [
          {
             "name": "gateway_data_source",
             "metrics": [
                {   
                   "name": "cpu_0_time_user",
                   "sample": [
                      {
                         "value": 4438100,
                         "timestamp": 1385381307414
                      }
                   ]
                },
                {                
                   "name": "cpu_0_time_sys",
                   "sample": [
                      {
                         "value": 8826700,
                         "timestamp": 1385381307414
                      }
                   ]
                },
                {
                   "name": "cpu_0time_idle",
                   "sample": [
                      {
                         "value": 3136586300,
                         "timestamp": 1385381307414
                      }
                   ]
                },
                {
                   "name": "total_memory",
                   "sample": [
                      {
                         "value": 3932626944,
                         "timestamp": 1385381307415
                      }
                   ]
                },                
                {
                   "name": "free_memory",
                   "sample": [
                      {
                         "value": 2488446976,
                         "timestamp": 1385381307415
                      }
                   ]
                },
                {
                   "name": "uptime",
                   "sample": [
                      {
                         "value": 315403.718087507,
                         "timestamp": 1385381307415
                      }
                   ]
                },
                {
                   "name": "min1_load_avg",
                   "sample": [
                      {
                         "value": 0.03271484375,
                         "timestamp": 1385381307415
                      }
                   ]
                },
                {
                   "name": "min5_load_avg",
                   "sample": [
                      {
                         "value": 0.03662109375,
                         "timestamp": 1385381307415
                      }
                   ]
                },
                {
                   "name": "min15_load_avg",
                   "sample": [
                      {
                         "value": 0.046875,
                         "timestamp": 1385381307415
                      }
                   ]
                }
             ]
          }
       ]
    }
    
    
