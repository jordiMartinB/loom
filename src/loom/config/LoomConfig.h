// Copyright 2016, University of Freiburg,
// Chair of Algorithms and Data Structures.
// Authors: Patrick Brosi <brosi@informatik.uni-freiburg.de>

#ifndef LOOM_CONFIG_LOOMCONFIG_H_
#define LOOM_CONFIG_LOOMCONFIG_H_

#include <string>
#include "3rdparty/json.hpp"
#include "shared/config/JsonConfigHelper.h"

namespace loom {
namespace config {

struct Config {
  std::string name;
  std::string outputPath;
  std::string dbgPath;

  std::string optimMethod = "comb-no-ilp";
  std::string MPSOutputPath;

  size_t optimRuns = 1;

  bool outOptGraph = false;

  bool outputStats = false;
  bool writeStats = false;
  bool pruneGraph = true;

  bool untangleGraph = true;
  bool fromDot = false;

  int ilpTimeLimit = -1;
  int ilpNumThreads = 0;

  double crossPenMultiSameSeg = 4;
  double crossPenMultiDiffSeg = 1;
  double separationPenWeight = 3;
  double stationCrossWeightSameSeg = 12;
  double stationCrossWeightDiffSeg = 3;
  double stationSeparationWeight = 9;

  std::string worldFilePath;

  std::string ilpSolver;
};

// JSON -> Config mapper
inline void jsonToConfig(const nlohmann::json& jsonObj, Config* cfg) {
  using shared::config::assignIfContains;
  using shared::config::assignIfContainsBool;

  assignIfContainsBool(jsonObj, "no-untangle", [&](bool v){ cfg->untangleGraph = v; });
  assignIfContainsBool(jsonObj, "output-stats", [&](bool v){ cfg->outputStats = v; });
  assignIfContainsBool(jsonObj, "no-prune", [&](bool v){ cfg->pruneGraph = v; });
  assignIfContains<double>(jsonObj, "same-seg-cross-pen", [&](double v){ cfg->crossPenMultiSameSeg = v; });
  assignIfContains<double>(jsonObj, "diff-seg-cross-pen", [&](double v){ cfg->crossPenMultiDiffSeg = v; });
  assignIfContains<double>(jsonObj, "sep-pen", [&](double v){ cfg->separationPenWeight = v; });
  assignIfContains<double>(jsonObj, "in-stat-cross-pen-same-seg", [&](double v){ cfg->stationCrossWeightSameSeg = v; });
  assignIfContains<double>(jsonObj, "in-stat-cross-pen-diff-seg", [&](double v){ cfg->stationCrossWeightDiffSeg = v; });
  assignIfContains<double>(jsonObj, "in-stat-sep-pen", [&](double v){ cfg->stationSeparationWeight = v; });
  assignIfContains<int>(jsonObj, "ilp-num-threads", [&](int v){ cfg->ilpNumThreads = v; });
  assignIfContains<int>(jsonObj, "ilp-time-limit", [&](int v){ cfg->ilpTimeLimit = v; });
  assignIfContains<std::string>(jsonObj, "ilp-solver", [&](const std::string& v){ cfg->ilpSolver = v; });
  assignIfContains<std::string>(jsonObj, "optim-method", [&](const std::string& v){ cfg->optimMethod = v; });
  assignIfContains<int>(jsonObj, "optim-runs", [&](int v){ cfg->optimRuns = static_cast<size_t>(v); });
  assignIfContains<std::string>(jsonObj, "dbg-output-path", [&](const std::string& v){ cfg->dbgPath = v; });
  assignIfContainsBool(jsonObj, "output-optgraph", [&](bool v){ cfg->outOptGraph = v; });
  assignIfContainsBool(jsonObj, "write-stats", [&](bool v){ cfg->writeStats = v; });
  assignIfContainsBool(jsonObj, "from-dot", [&](bool v){ cfg->fromDot = v; });
}

}  // namespace config
}  // namespace loom

#endif  // LOOM_CONFIG_LOOMCONFIG_H_
