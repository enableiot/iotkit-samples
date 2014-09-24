#!/usr/bin/env node

"use strict";

// imports
var config = require("../configs/app"),
    provider = require("../providers/os-stats-provider"),
    logger = require("../lib/logger").init(),
    cloud = require("../index"),
    os = require("os"),
    events = require("events");

// global errors
process.on("uncaughtException", function(err) {
    logger.error("UncaughtException:", err.message);
    logger.error(err.stack);
    process.exit(1);
});

// modify default configuration values
config.source.deviceId = "d-" + os.hostname();
config.source.gatewayId = config.source.deviceId;
config.source.name = os.hostname();
config.component.type = "SystemUtilization.v1.0";
config.component.name = "SystemUtilization";
config.component.cid = "c-" + config.source.deviceId;

logger.info("Setup...");
cloud.setup(config, function(err, state){
  if (err) throw err;
  if (state){
    logger.debug("State: %s", state);
    logger.info("Subscribe to events...");
    provider.sub(state).on("data", function(observations){
      logger.info("Submit data: ", observations);
      cloud.submitObservations(state, observations);
    }); // sub
  } // data
}); // setup

