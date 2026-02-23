// Copyright 2016, University of Freiburg,
// Chair of Algorithms and Data Structures.
// Authors: Patrick Brosi <brosi@informatik.uni-freiburg.de>

#include <float.h>
#include <getopt.h>
#include <sstream>

#include <exception>
#include <iostream>
#include <string>


#include "topo/_config.h"
#include "topo/config/ConfigReader.h"
#include "util/log/Log.h"
#include "3rdparty/json.hpp"

using topo::config::ConfigReader;

using std::exception;
using std::string;
using std::vector;

// _____________________________________________________________________________
ConfigReader::ConfigReader() {}

// _____________________________________________________________________________
void ConfigReader::read(TopoConfig* cfg, std::stringstream *jsonStream) const {
  // Initialize cfg with default arguments from TopoConfig.h
  cfg->maxAggrDistance = 50;  // Example default value
  cfg->noInferRestrs = false;
  cfg->outputStats = false;
  cfg->maxTurnRestrCheckDist = 50;  // Default same as maxAggrDistance
  cfg->writeComponents = false;
  cfg->componentsPath = "";
  cfg->turnInferFullTurnPen = 0;
  cfg->randomColors = false;
  cfg->segmentLength = 5;
  cfg->connectedCompDist = 10000;
  cfg->smooth = 0;
  cfg->fullTurnAngle = 0;
  cfg->aggregateStats = false;

  nlohmann::json jsonObj;
  try {
    (*jsonStream) >> jsonObj;
  } catch (const nlohmann::json::parse_error& e) {
    std::cerr << "Error parsing JSON: " << e.what() << std::endl;
    exit(1);
  }

  // Update cfg based on JSON content
  if (jsonObj.contains("max-aggr-dist")) {
    cfg->maxAggrDistance = jsonObj["max-aggr-dist"].get<double>();
  }
  if (jsonObj.contains("no-infer-restrs")) {
    cfg->noInferRestrs = jsonObj["no-infer-restrs"].get<bool>();
  }
  if (jsonObj.contains("write-stats")) {
    cfg->outputStats = jsonObj["write-stats"].get<bool>();
  }
  if (jsonObj.contains("infer-restr-max-dist")) {
    cfg->maxTurnRestrCheckDist = jsonObj["infer-restr-max-dist"].get<double>();
  }
  if (jsonObj.contains("write-components")) {
    cfg->writeComponents = jsonObj["write-components"].get<bool>();
  }
  if (jsonObj.contains("write-components-path")) {
    cfg->componentsPath = jsonObj["write-components-path"].get<std::string>();
  }
  if (jsonObj.contains("turn-restr-full-turn-pen")) {
    cfg->turnInferFullTurnPen = jsonObj["turn-restr-full-turn-pen"].get<double>();
  }
  if (jsonObj.contains("random-colors")) {
    cfg->randomColors = jsonObj["random-colors"].get<bool>();
  }
  if (jsonObj.contains("sample-dist")) {
    cfg->segmentLength = jsonObj["sample-dist"].get<double>();
  }
  if (jsonObj.contains("max-comp-dist")) {
    cfg->connectedCompDist = jsonObj["max-comp-dist"].get<double>();
  }
  if (jsonObj.contains("smooth")) {
    cfg->smooth = jsonObj["smooth"].get<double>();
  }
  if (jsonObj.contains("turn-restr-full-turn-angle")) {
    cfg->fullTurnAngle = jsonObj["turn-restr-full-turn-angle"].get<double>();
  }
  if (jsonObj.contains("aggr-stats")) {
    cfg->aggregateStats = jsonObj["aggr-stats"].get<bool>();
  }
}
