// Chair of Algorithms and Data Structures.
// Authors: Patrick Brosi <brosi@informatik.uni-freiburg.de>

#include <float.h>
#include <getopt.h>
#include <exception>
#include <iostream>
#include <string>
#include <3rdparty/json.hpp>
#include <sstream>
#include "loom/_config.h"
#include "loom/config/ConfigReader.h"
#include "loom/config/LoomConfig.h"
#include "util/log/Log.h"

using loom::config::ConfigReader;

using std::exception;
using std::string;
using std::vector;
using nlohmann::json;

// _____________________________________________________________________________
ConfigReader::ConfigReader() {}

// _____________________________________________________________________________
void ConfigReader::read(Config* cfg, std::stringstream *jsonStream) const {
  // Set default values for the configuration
  cfg->untangleGraph = false;
  cfg->outputStats = false;
  cfg->pruneGraph = false;
  cfg->crossPenMultiSameSeg = 4.0;
  cfg->crossPenMultiDiffSeg = 1.0;
  cfg->separationPenWeight = 3.0;
  cfg->stationCrossWeightSameSeg = 12.0;
  cfg->stationCrossWeightDiffSeg = 3.0;
  cfg->stationSeparationWeight = 9.0;
  cfg->ilpNumThreads = 0;
  cfg->ilpTimeLimit = -1.0;
  cfg->ilpSolver = "gurobi";
  cfg->optimMethod = "comb-no-ilp";
  cfg->optimRuns = 1;
  cfg->dbgPath = ".";
  cfg->outOptGraph = false;
  cfg->writeStats = false;
  cfg->fromDot = false;

  // Parse the JSON from the stringstream
  nlohmann::json optionsJson;
  try {
     (*jsonStream) >> optionsJson;
  } catch (const std::exception& e) {
    std::cerr << "Error parsing JSON: " << e.what() << std::endl;
    exit(1);
  }

  // Update the config if the key is found in the JSON
  if (optionsJson.contains("no-untangle")) {
    cfg->untangleGraph = optionsJson["no-untangle"].get<bool>();
  }

  if (optionsJson.contains("output-stats")) {
    cfg->outputStats = optionsJson["output-stats"].get<bool>();
  }

  if (optionsJson.contains("no-prune")) {
    cfg->pruneGraph = optionsJson["no-prune"].get<bool>();
  }

  if (optionsJson.contains("same-seg-cross-pen")) {
    cfg->crossPenMultiSameSeg = optionsJson["same-seg-cross-pen"].get<double>();
  }

  if (optionsJson.contains("diff-seg-cross-pen")) {
    cfg->crossPenMultiDiffSeg = optionsJson["diff-seg-cross-pen"].get<double>();
  }

  if (optionsJson.contains("sep-pen")) {
    cfg->separationPenWeight = optionsJson["sep-pen"].get<double>();
  }

  if (optionsJson.contains("in-stat-cross-pen-same-seg")) {
    cfg->stationCrossWeightSameSeg = optionsJson["in-stat-cross-pen-same-seg"].get<double>();
  }

  if (optionsJson.contains("in-stat-cross-pen-diff-seg")) {
    cfg->stationCrossWeightDiffSeg = optionsJson["in-stat-cross-pen-diff-seg"].get<double>();
  }

  if (optionsJson.contains("in-stat-sep-pen")) {
    cfg->stationSeparationWeight = optionsJson["in-stat-sep-pen"].get<double>();
  }

  if (optionsJson.contains("ilp-num-threads")) {
    cfg->ilpNumThreads = optionsJson["ilp-num-threads"].get<int>();
  }

  if (optionsJson.contains("ilp-time-limit")) {
    cfg->ilpTimeLimit = optionsJson["ilp-time-limit"].get<double>();
  }

  if (optionsJson.contains("ilp-solver")) {
    cfg->ilpSolver = optionsJson["ilp-solver"].get<std::string>();
  }

  if (optionsJson.contains("optim-method")) {
    cfg->optimMethod = optionsJson["optim-method"].get<std::string>();
  }

  if (optionsJson.contains("optim-runs")) {
    cfg->optimRuns = optionsJson["optim-runs"].get<int>();
  }

  if (optionsJson.contains("dbg-output-path")) {
    cfg->dbgPath = optionsJson["dbg-output-path"].get<std::string>();
  }

  if (optionsJson.contains("output-optgraph")) {
    cfg->outOptGraph = optionsJson["output-optgraph"].get<bool>();
  }

  if (optionsJson.contains("write-stats")) {
    cfg->writeStats = optionsJson["write-stats"].get<bool>();
  }

  if (optionsJson.contains("from-dot")) {
    cfg->fromDot = optionsJson["from-dot"].get<bool>();
  }
}
