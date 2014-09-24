#!/usr/bin/env node

"use strict";

// imports
var config = require("../configs/app"),
    provider = require("../providers/pull-provider"),
    logger = require("../lib/logger").init(),
    cloud = require("../index");

// global errors
process.on("uncaughtException", function(err) {
    logger.error("UncaughtException:", err.message);
    logger.error(err.stack);
    process.exit(1);
});


logger.info("Setup...");
cloud.setup(config, function(err, state){
  if (err) throw err;
  if (state){
    logger.debug("State: %s", state);

    logger.info("Mock some data...");
    var observations = provider.get(state);

    logger.info("Submit data...");
    cloud.submitObservations(state, observations, function(err){
      if (err) throw err;
      logger.info("Done");
    }); // observation
    
  } // data
}); // setup

