// Copyright 2016, University of Freiburg,
// Chair of Algorithms and Data Structures.
// Authors: Patrick Brosi <brosi@informatik.uni-freiburg.de>

#ifndef GTFS2GRAPH_CONFIG_GTFS2GEOCONFIG_H_
#define GTFS2GRAPH_CONFIG_GTFS2GEOCONFIG_H_

#include <string>
#include "ad/cppgtfs/gtfs/flat/Route.h"
#include "3rdparty/json.hpp"
#include "shared/config/JsonConfigHelper.h"
#include "util/String.h"

namespace gtfs2graph {
namespace config {

struct Config {
  std::string inputFeedPath;

  double pruneThreshold;

  std::set<ad::cppgtfs::gtfs::flat::Route::TYPE> useMots;
};

// JSON -> Config mapper
inline void jsonToConfig(const nlohmann::json& jsonObj, Config* cfg) {
  using shared::config::assignIfContains;
  using shared::config::assignIfContainsBool;

  assignIfContains<std::string>(jsonObj, "inputFeedPath", [&](const std::string& v){ cfg->inputFeedPath = v; });
  assignIfContains<double>(jsonObj, "prune-threshold", [&](double v){ cfg->pruneThreshold = v; });
  if (jsonObj.contains("mots")) {
    std::string motStr;
    if (jsonObj["mots"].is_string()) motStr = jsonObj["mots"].get<std::string>();
    else if (jsonObj["mots"].is_array()) {
      bool first = true;
      for (auto &el : jsonObj["mots"]) {
        if (!first) motStr += ",";
        motStr += el.get<std::string>();
        first = false;
      }
    }
    for (auto sMotStr : util::split(motStr, ',')) {
      for (auto mot : ad::cppgtfs::gtfs::flat::Route::getTypesFromString(sMotStr)) {
        cfg->useMots.insert(mot);
      }
    }
  }
}

}  // namespace config
}  // namespace gtfs2graph

#endif  // GTFS2GRAPH_CONFIG_GTFS2TOPOCONFIG_H_
