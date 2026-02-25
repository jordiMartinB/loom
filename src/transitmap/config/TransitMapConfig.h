// Copyright 2016, University of Freiburg,
// Chair of Algorithms and Data Structures.
// Authors: Patrick Brosi <brosi@informatik.uni-freiburg.de>

#ifndef TRANSITMAP_CONFIG_TRANSITMAPCONFIG_H_
#define TRANSITMAP_CONFIG_TRANSITMAPCONFIG_H_

#include <string>
#include <vector>
#include "3rdparty/json.hpp"
#include "shared/config/JsonConfigHelper.h"
#include "util/String.h"

namespace transitmapper {
namespace config {

struct Config {
  double lineWidth = 20;
  double lineSpacing = 10;

  double lineLabelSize = 40;
  double stationLabelSize = 60;

  std::string renderMethod = "svg";

  std::string mvtPath = ".";

  bool writeStats = false;

  double outputResolution = 0.1;
  double inputSmoothing = 1;
  double innerGeometryPrecision = 3;

  double outputPadding = -1;

  double outlineWidth = 1;
  std::string outlineColor;

  bool renderStations = true;
  bool renderNodeFronts = false;
  bool renderNodeCircles = false;
  bool renderEdges = true;
  bool renderLabels = false;
  bool dontLabelDeg2 = false;
  bool fromDot = false;

  bool randomColors = false;

  bool renderNodeConnections = true;
  bool tightStations = false;

  std::vector<size_t> mvtZooms;

  bool renderDirMarkers = false;
  std::string worldFilePath;
};

// JSON -> Config mapper moved here.
inline void jsonToConfig(const nlohmann::json& jsonObj, Config* cfg) {
  using shared::config::assignIfContains;
  using shared::config::assignIfContainsBool;

  assignIfContains<std::string>(jsonObj, "render-engine",
      [&](const std::string& v){ cfg->renderMethod = v; });
  assignIfContains<double>(jsonObj, "line-width",
      [&](double v){ cfg->lineWidth = v; });
  assignIfContains<double>(jsonObj, "line-spacing",
      [&](double v){ cfg->lineSpacing = v; });
  assignIfContains<double>(jsonObj, "outline-width",
      [&](double v){ cfg->outlineWidth = v; });
  assignIfContains<double>(jsonObj, "line-label-textsize",
      [&](double v){ cfg->lineLabelSize = v; });
  assignIfContains<double>(jsonObj, "station-label-textsize",
      [&](double v){ cfg->stationLabelSize = v; });

  assignIfContainsBool(jsonObj, "no-render-stations",
      [&](bool v){ if (v) cfg->renderStations = false; });
  assignIfContainsBool(jsonObj, "labels",
      [&](bool v){ if (v) cfg->renderLabels = true; });
  assignIfContainsBool(jsonObj, "tight-stations",
      [&](bool v){ if (v) cfg->tightStations = true; });
  assignIfContainsBool(jsonObj, "render-dir-markers",
      [&](bool v){ if (v) cfg->renderDirMarkers = true; });
  assignIfContainsBool(jsonObj, "no-render-node-connections",
      [&](bool v){ if (v) cfg->renderNodeConnections = !v; });

  assignIfContains<double>(jsonObj, "resolution",
      [&](double v){ cfg->outputResolution = v; });
  assignIfContains<double>(jsonObj, "padding",
      [&](double v){ cfg->outputPadding = v; });
  assignIfContains<double>(jsonObj, "smoothing",
      [&](double v){ cfg->inputSmoothing = v; });
  assignIfContainsBool(jsonObj, "render-node-fronts",
      [&](bool v){ if (v) cfg->renderNodeFronts = true; });
  assignIfContainsBool(jsonObj, "no-deg2-labels",
      [&](bool v){ if (v) cfg->dontLabelDeg2 = true; });
  assignIfContains<std::string>(jsonObj, "mvt-path",
      [&](const std::string& v){ cfg->mvtPath = v; });
  assignIfContainsBool(jsonObj, "random-colors",
      [&](bool v){ if (v) cfg->randomColors = true; });
  assignIfContainsBool(jsonObj, "print-stats",
      [&](bool v){ if (v) cfg->writeStats = true; });
  assignIfContainsBool(jsonObj, "from-dot",
      [&](bool v){ if (v) cfg->fromDot = v; });

  // Zoom handling: accept string, number, or array
  std::string zoom;
  if (jsonObj.contains("zoom")) {
    if (jsonObj["zoom"].is_string()) {
      zoom = jsonObj["zoom"].get<std::string>();
    } else if (jsonObj["zoom"].is_number_integer()) {
      zoom = std::to_string(jsonObj["zoom"].get<int>());
    } else if (jsonObj["zoom"].is_array()) {
      bool first = true;
      for (auto &el : jsonObj["zoom"]) {
        if (!first) zoom += ",";
        if (el.is_string()) zoom += el.get<std::string>();
        else if (el.is_number_integer()) zoom += std::to_string(el.get<int>());
        first = false;
      }
    }
  }

  // Parse zoom ranges / list into mvtZooms
  for (auto range : util::split(zoom, ',')) {
    util::replaceAll(range, " ", "");
    util::replaceAll(range, "=", "");
    auto parts = util::split(range, '-');
    if (parts.size() > 2) {
      std::cerr << "Error while parsing zoom range" << zoom << std::endl;
      exit(1);
    }

    int from = atoi(parts.front().c_str());
    int to = atoi(parts.back().c_str());

    if (from > to) {
      int a = from;
      from = to;
      to = a;
    }

    if (from < 0 || from > 25 || to < 0 || to > 25) {
      std::cerr << "Error while parsing zoom range" << zoom << std::endl;
      exit(1);
    }

    for (int z = from; z <= to; z++) {
      cfg->mvtZooms.push_back(z);
    }
  }

  // Default zoom if none provided
  if (cfg->mvtZooms.size() == 0) cfg->mvtZooms.push_back(14);

  // Validate numeric options
  if (cfg->lineWidth < 0) {
    std::cerr << "Error: line width " << cfg->lineWidth << " is negative!" << std::endl;
    exit(1);
  }

  if (cfg->outlineWidth < 0) {
    std::cerr << "Error: outline width " << cfg->outlineWidth << " is negative!" << std::endl;
    exit(1);
  }

  if (cfg->outputPadding < 0) {
    cfg->outputPadding = (cfg->lineWidth + cfg->lineSpacing);
  }
}

}  // namespace config
}  // namespace transitmapper

#endif  // TRANSITMAP_CONFIG_TRANSITMAPCONFIG_H_
