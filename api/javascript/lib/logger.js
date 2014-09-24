"use strict";

var winston = require('winston'),
    conf = process.env;

exports.init = function() {

  var logs = [
    new (winston.transports.Console)({ 
          level: conf.CONSOLE_LOG_LEVEL || 'debug',
          colorize: true,
          timestamp: true
    })
  ];

  if (conf.FILE_LOG){
    logs.push(new (winston.transports.File)({ 
          filename: conf.FILE_LOG || './app.log',
          level: conf.FILE_LOG_LEVEL || 'info',
          timestamp: true
    }))
  }

  return new (winston.Logger)({
    transports: logs,
    exitOnError: false
  });
  
};  