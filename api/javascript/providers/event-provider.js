"use strict";

var util = require('util'),
	 logger = require("../lib/logger").init(),
	 EventEmitter = require('events').EventEmitter;


var MockedObservationEmitter = function(){

	var me = this;

	me.get = function(config){
		throw "Not Implemented";
	}

	me.sub = function(config){
		setTimeout(function () {
			me.emit("data", [{
		      "componentId": config.component,
		      "on": (new Date).getTime(),
		   	"value": Math.floor((Math.random() * 100) + 1).toString()
		   }]);
		   me.sub(config);
		}, 3000);
		return me;
	} // sub

} // object

util.inherits(MockedObservationEmitter, EventEmitter);

var observationEmitter = new MockedObservationEmitter();

module.exports = {
    get: observationEmitter.get,
    sub: observationEmitter.sub
};

