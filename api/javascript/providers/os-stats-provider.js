/*
 Copyright (c) 2015, Intel Corporation

 Redistribution and use in source and binary forms, with or without modification,
 are permitted provided that the following conditions are met:

 * Redistributions of source code must retain the above copyright notice,
 this list of conditions and the following disclaimer.
 * Redistributions in binary form must reproduce the above copyright notice,
 this list of conditions and the following disclaimer in the documentation
 and/or other materials provided with the distribution.
 * Neither the name of Intel Corporation nor the names of its contributors
 may be used to endorse or promote products derived from this software
 without specific prior written permission.

 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
 ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
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

