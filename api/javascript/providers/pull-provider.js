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

/*
	mockItems
*/
var _mock = function(componentId, num, max, loc){
	var num = num || 100;
	var min = min || 0;
	var max = max || 100;
  	var data = [];
  	var now = (new Date).getTime();
  	for (var i=num; i>0; i--){
   	var ts = now - (i * 1000); // 1 sec granularity 
    	var val = Math.floor((Math.random() * max) + 1); //1-100%
    	// TODO: Not sure why value has to be  a string. 
    	//       Seems to fail as numeric 
	   var item = {
	      "componentId": componentId,
	      "on": ts,
	   	"value": val.toString()
	   }
	   if (loc) item.loc = loc;
    	data.push(item);
  }
  return data;
}


var get = function(config){
	return _mock(config.component, 10, 100);
}

var sub = function(config){
	throw "Not implemented";
}

module.exports = {
    get: get,
    sub: sub
};

