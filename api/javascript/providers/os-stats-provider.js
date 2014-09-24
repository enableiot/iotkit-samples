"use strict";

var util = require('util'),
	 os = require("os"),
	 logger = require("../lib/logger").init(),
	 EventEmitter = require('events').EventEmitter;


var OsStatsObservationEmitter = function(){
	var me = this;
	me.sub = function(config){
		setTimeout(function () {
			me.emit("data", [{
		      "componentId": config.component,
		      "on": (new Date).getTime(),
		   	"value": os.loadavg()[0].toString(),
		   	"attributes": {
                "uptime": os.uptime().toString(),
                "freemem": os.freemem().toString()
            }
		   }]);
		   me.sub(config);
		}, 5000);
		return me;
	} // sub

} // object

util.inherits(OsStatsObservationEmitter, EventEmitter);

var observationEmitter = new OsStatsObservationEmitter();

module.exports = {
    get: observationEmitter.get,
    sub: observationEmitter.sub
};

