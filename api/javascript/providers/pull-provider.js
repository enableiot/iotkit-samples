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

