// Copyright 2016, University of Freiburg,
// Chair of Algorithms and Data Structures.
// Authors: Patrick Brosi <brosi@informatik.uni-freiburg.de>

#ifndef OCTI_CONFIG_OCTICONFIG_H_
#define OCTI_CONFIG_OCTICONFIG_H_

#include <string>
#include "octi/basegraph/BaseGraph.h"
#include "octi/basegraph/GridGraph.h"
#include "util/geo/Geo.h"
#include "3rdparty/json.hpp"
#include "shared/config/JsonConfigHelper.h"
#include "util/log/Log.h"

using octi::basegraph::BaseGraphType;
using util::ERROR;

namespace octi {
namespace config {

enum OrderMethod {
  NUM_LINES = 0,
  LENGTH = 1,
  ADJ_ND_DEGREE = 2,
  ADJ_ND_LDEGREE = 3,
  GROWTH_DEG = 4,
  GROWTH_LDEG = 5,
  ALL = 99
};

struct Config {
  std::string gridSize = "100%";
  double borderRad = 45;

  std::string printMode = "linegraph";
  std::string optMode = "heur";
  std::string ilpPath;
  bool fromDot = false;
  bool deg2Heur = true;
  bool restrLocSearch = false;
  double enfGeoPen = 0;
  bool ilpNoSolve = false;
  int ilpTimeLimit = 60;
  int ilpNumThreads = 0;
  double ilpCacheThreshold = DBL_MAX;
  std::string ilpSolver = "gurobi";
  std::string ilpCacheDir = ".";

  bool skipOnError = false;
  bool retryOnError = false;

  double maxGrDist = 3;

  int heurLocSearchIters = 100;

  size_t abortAfter = -1;

  size_t hananIters = 1;
  bool writeStats = false;

  OrderMethod orderMethod;

  std::string obstaclePath;
  std::vector<util::geo::DPolygon> obstacles;

  octi::basegraph::BaseGraphType baseGraphType;

  octi::basegraph::Penalties pens;
};

// JSON -> Config mapper
inline void jsonToConfig(const nlohmann::json& jsonObj, Config* cfg) {
  using shared::config::assignIfContains;
  using shared::config::assignIfContainsBool;

  assignIfContains<int>(jsonObj, "abortAfter", [&](int v){ cfg->abortAfter = v; });
  assignIfContainsBool(jsonObj, "fromDot", [&](bool v){ cfg->fromDot = v; });
  assignIfContains<std::string>(jsonObj, "optimMode", [&](const std::string& v){ cfg->optMode = v; });
  assignIfContains<int>(jsonObj, "ilpNumThreads", [&](int v){ cfg->ilpNumThreads = v; });
  assignIfContains<int>(jsonObj, "hananIters", [&](int v){ cfg->hananIters = v; });
  assignIfContains<int>(jsonObj, "heurLocSearchIters", [&](int v){ cfg->heurLocSearchIters = v; });
  assignIfContains<double>(jsonObj, "ilpCacheThreshold", [&](double v){ cfg->ilpCacheThreshold = v; });
  assignIfContains<int>(jsonObj, "ilpTimeLimit", [&](int v){ cfg->ilpTimeLimit = v; });
  assignIfContains<std::string>(jsonObj, "ilpCacheDir", [&](const std::string& v){ cfg->ilpCacheDir = v; });
  assignIfContains<std::string>(jsonObj, "ilpSolver", [&](const std::string& v){ cfg->ilpSolver = v; });
  assignIfContainsBool(jsonObj, "writeStats", [&](bool v){ cfg->writeStats = v; });
  assignIfContains<std::string>(jsonObj, "obstaclePath", [&](const std::string& v){ cfg->obstaclePath = v; });
  assignIfContainsBool(jsonObj, "deg2Heur", [&](bool v){ cfg->deg2Heur = v; });
  assignIfContains<double>(jsonObj, "enfGeoPen", [&](double v){ cfg->enfGeoPen = v; });
  assignIfContains<double>(jsonObj, "maxGrDist", [&](double v){ cfg->maxGrDist = v; });
  assignIfContainsBool(jsonObj, "restrLocSearch", [&](bool v){ cfg->restrLocSearch = v; });

  if (jsonObj.contains("edgeOrderMethod")) {
    std::string edgeOrderMethod = jsonObj["edgeOrderMethod"].get<std::string>();
    if (edgeOrderMethod == "num-lines") {
      cfg->orderMethod = OrderMethod::NUM_LINES;
    } else if (edgeOrderMethod == "length") {
      cfg->orderMethod = OrderMethod::LENGTH;
    } else if (edgeOrderMethod == "adj-nd-deg") {
      cfg->orderMethod = OrderMethod::ADJ_ND_DEGREE;
    } else if (edgeOrderMethod == "adj-nd-ldeg") {
      cfg->orderMethod = OrderMethod::ADJ_ND_LDEGREE;
    } else if (edgeOrderMethod == "growth-deg") {
      cfg->orderMethod = OrderMethod::GROWTH_DEG;
    } else if (edgeOrderMethod == "growth-ldeg") {
      cfg->orderMethod = OrderMethod::GROWTH_LDEG;
    } else if (edgeOrderMethod == "all") {
      cfg->orderMethod = OrderMethod::ALL;
    } else {
      LOG(ERROR) << "Unknown order method " << edgeOrderMethod;
      exit(1);
    }
  }

  if (jsonObj.contains("baseGraphType")) {
    std::string baseGraphStr = jsonObj["baseGraphType"].get<std::string>();
    if (baseGraphStr == "ortholinear") {
      cfg->baseGraphType = octi::basegraph::BaseGraphType::GRID;
    } else if (baseGraphStr == "octilinear") {
      cfg->baseGraphType = octi::basegraph::BaseGraphType::OCTIGRID;
    } else if (baseGraphStr == "hexalinear") {
      cfg->baseGraphType = octi::basegraph::BaseGraphType::HEXGRID;
    } else if (baseGraphStr == "chulloctilinear") {
      cfg->baseGraphType = octi::basegraph::BaseGraphType::CONVEXHULLOCTIGRID;
    } else if (baseGraphStr == "pseudoorthoradial") {
      cfg->baseGraphType = octi::basegraph::BaseGraphType::PSEUDOORTHORADIAL;
    } else if (baseGraphStr == "quadtree") {
      cfg->baseGraphType = octi::basegraph::BaseGraphType::OCTIQUADTREE;
    } else if (baseGraphStr == "octihanan") {
      cfg->baseGraphType = octi::basegraph::BaseGraphType::OCTIHANANGRID;
    } else {
      LOG(ERROR) << "Unknown base graph type " << baseGraphStr;
      exit(1);
    }
  }

  if (jsonObj.contains("pens")) {
    auto pens = jsonObj["pens"];
    if (pens.contains("densityPen")) cfg->pens.densityPen = pens["densityPen"].get<double>();
    if (pens.contains("verticalPen")) cfg->pens.verticalPen = pens["verticalPen"].get<double>();
    if (pens.contains("horizontalPen")) cfg->pens.horizontalPen = pens["horizontalPen"].get<double>();
    if (pens.contains("diagonalPen")) cfg->pens.diagonalPen = pens["diagonalPen"].get<double>();
    if (pens.contains("p_0")) cfg->pens.p_0 = pens["p_0"].get<double>();
    if (pens.contains("p_135")) cfg->pens.p_135 = pens["p_135"].get<double>();
    if (pens.contains("p_90")) cfg->pens.p_90 = pens["p_90"].get<double>();
    if (pens.contains("p_45")) cfg->pens.p_45 = pens["p_45"].get<double>();
    if (pens.contains("ndMovePen")) cfg->pens.ndMovePen = pens["ndMovePen"].get<double>();
  }

  if (jsonObj.contains("skipOnError")) cfg->skipOnError = jsonObj["skipOnError"].get<bool>();
  if (jsonObj.contains("retryOnError")) cfg->retryOnError = jsonObj["retryOnError"].get<bool>();
  if (jsonObj.contains("gridSize")) cfg->gridSize = jsonObj["gridSize"].get<std::string>();
}

}  // namespace config
}  // namespace octi

#endif  // OCTI_CONFIG_OCTICONFIG_H_
