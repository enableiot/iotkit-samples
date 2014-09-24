# enableiot-pump

Simple utility to push data to [enableiot.com](https://dashboard.us.enableiot.com). The [enableiot API](https://github.com/enableiot/iotkit-agent/wiki/Api-Home) is documented on GitHub.

## Installation

    npm install

## Usage

The `enableiot-pump` supports two usage patterns:

* Simple get data and submit results (single)
* Event-based subscription model (ongoing)

### Simple

Simply setup the cloud client, retreave results from observation provider and submit:

    // error handling and validation omitted for clarity 
    cloud.setup(config, function(err, state){
        var observations = provider.get(state);
        cloud.submitObservations(state, observations, err_handler);
    });

See [examples](https://github.com/enableiot/iotkit-samples/blob/master/api/javascript/examples/pull-example.js) for working example.

### Event-based

When ongoing submission is required you can use the event-based pattern with subscription to the `data` event: 

    // error handling and validation omitted for clarity 
    cloud.setup(config , function(err, state){
        provider.sub(state).on("data", function(observations){
          cloud.submitObservations(state, observations, err_handler);
        });
    });

See [examples](https://github.com/enableiot/iotkit-samples/blob/master/api/javascript/examples/event-example.js) for working example.

## Provider

Both of these usage models are implemented using provider pattern. You can easily implement your own provider from Database, Message Queue or simply by tailing a log by implementing `get` and `sub` functions.

### Get

    var get = function(config){
        //TODO: Implement your code here
    }

### Sub

    var sub = function(config){
        //TODO: Implement your code here
    }

Both of these functions must return an array of `observations`.

## Observation 

Observation is a simple construct representing an event in time:

    {
        "componentId": "my-registered-component-1",
        "on": 1406685882807,
        "value": "96.6"
    }

The above object represents only the required elements. See [enableiot documentation](https://github.com/enableiot/iotkit-agent/wiki/Data-API) for more details about the supported types.

## TODO

* Tests (yes please)
* DB, MQ, Log provider samples

## License

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.



