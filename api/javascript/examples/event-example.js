#!/usr/bin/env node

"use strict";

// imports
var config = require("../configs/app"),
    provider = require("../providers/event-provider"),
    logger = require("../lib/logger").init(),
    cloud = require("../index"),
    events = require("events");

// global errors
process.on("uncaughtException", function(err) {
    logger.error("UncaughtException:", err.message);
    logger.error(err.stack);
    process.exit(1);
});

var err_handler = function(err){
  if (err) throw err;
}


logger.info("Setup...");
cloud.setup(config, function(err, state){
  if (err) throw err;
  if (state){
    logger.debug("State: %s", state);

    logger.info("Subscribe to events...");
    provider.sub(state).on("data", function(observations){

      logger.info("Submit data: ", observations);
      cloud.submitObservations(state, observations, err_handler);

    }); // sub

  } // data

}); // setup

