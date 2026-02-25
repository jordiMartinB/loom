// Copyright 2016, University of Freiburg,
// Chair of Algorithms and Data Structures.
// Authors: Patrick Brosi <brosi@informatik.uni-freiburg.de>

#ifndef TOPO_CONFIG_TOPOCONFIG_H_
#define TOPO_CONFIG_TOPOCONFIG_H_

#include <string>
#include "3rdparty/json.hpp"
#include "shared/config/JsonConfigHelper.h"

namespace topo {
namespace config {

struct Config {
  double maxAggrDistance = 50;
  double maxLengthDev = 500;
  double maxTurnRestrCheckDist = 50;
  double turnInferFullTurnPen = 0;
  double fullTurnAngle = 0;
  double segmentLength = 5;
  bool outputStats = false;
  bool noInferRestrs = false;
  bool writeComponents = false;
  bool randomColors = false;
  bool aggregateStats = false;
  double connectedCompDist = 10000;
  double smooth = 0;
  std::string componentsPath = "";
};

// JSON -> TopoConfig mapper
inline void jsonToConfig(const nlohmann::json& jsonObj, Config* cfg) {
  using shared::config::assignIfContains;
  using shared::config::assignIfContainsBool;

  assignIfContains<double>(jsonObj, "max-aggr-dist", [&](double v){ cfg->maxAggrDistance = v; });
  assignIfContainsBool(jsonObj, "no-infer-restrs", [&](bool v){ cfg->noInferRestrs = v; });
  assignIfContainsBool(jsonObj, "write-stats", [&](bool v){ cfg->outputStats = v; });
  assignIfContains<double>(jsonObj, "infer-restr-max-dist", [&](double v){ cfg->maxTurnRestrCheckDist = v; });
  assignIfContainsBool(jsonObj, "write-components", [&](bool v){ cfg->writeComponents = v; });
  assignIfContains<std::string>(jsonObj, "write-components-path", [&](const std::string& v){ cfg->componentsPath = v; });
  assignIfContains<double>(jsonObj, "turn-restr-full-turn-pen", [&](double v){ cfg->turnInferFullTurnPen = v; });
  assignIfContainsBool(jsonObj, "random-colors", [&](bool v){ cfg->randomColors = v; });
  assignIfContains<double>(jsonObj, "sample-dist", [&](double v){ cfg->segmentLength = v; });
  assignIfContains<double>(jsonObj, "max-comp-dist", [&](double v){ cfg->connectedCompDist = v; });
  assignIfContains<double>(jsonObj, "smooth", [&](double v){ cfg->smooth = v; });
  assignIfContains<double>(jsonObj, "turn-restr-full-turn-angle", [&](double v){ cfg->fullTurnAngle = v; });
  assignIfContainsBool(jsonObj, "aggr-stats", [&](bool v){ cfg->aggregateStats = v; });
}

}  // namespace config
}  // namespace topo

#endif  // TOPO_CONFIG_TOPOCONFIG_H_
