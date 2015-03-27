#!/usr/bin/env node
/*
 Copyright (c) 2015, Intel Corporation

 Redistribution and use in source and binary forms, with or without modification,
 are permitted provided that the following conditions are met:

 * Redistributions of source code must retain the above copyright notice,
 this list of conditions and the following disclaimer.
 * Redistributions in binary form must reproduce the above copyright notice,
 this list of conditions and the following disclaimer in the documentation
 and/or other materials provided with the distribution.

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

