// Copyright 2017, University of Freiburg,
// Chair of Algorithms and Data Structures.
// Chair of Algorithms and Data Structures.
// Authors: Patrick Brosi <brosi@informatik.uni-freiburg.de>

#include <float.h>
#include <getopt.h>
#include <sstream>
#include "3rdparty/json.hpp"

#include <exception>
#include <iostream>
#include <string>

#include "octi/_config.h"
#include "octi/config/ConfigReader.h"
#include "util/log/Log.h"

using octi::config::ConfigReader;

using octi::basegraph::BaseGraphType;
using octi::config::OrderMethod;
using util::ERROR;

static const char* YEAR = &__DATE__[7];
static const char* COPY =
    "University of Freiburg - Chair of Algorithms and Data Structures";
static const char* AUTHORS = "Patrick Brosi <brosi@informatik.uni-freiburg.de>";

// _____________________________________________________________________________
ConfigReader::ConfigReader() {}

// _____________________________________________________________________________
void ConfigReader::read(Config* cfg, std::stringstream *jsonStream) const {
  nlohmann::json jsonObj;
  try {
    (*jsonStream) >> jsonObj;
  } catch (const nlohmann::json::parse_error& e) {
    std::cerr << "Error parsing JSON: " << e.what() << std::endl;
    exit(1);
  }

  if (jsonObj.contains("abortAfter")) {
    cfg->abortAfter = jsonObj["abortAfter"].get<int>();
  }
  if (jsonObj.contains("fromDot")) {
    cfg->fromDot = jsonObj["fromDot"].get<bool>();
  }
  if (jsonObj.contains("optimMode")) {
    cfg->optMode = jsonObj["optimMode"].get<std::string>();
  }
  if (jsonObj.contains("ilpNumThreads")) {
    cfg->ilpNumThreads = jsonObj["ilpNumThreads"].get<int>();
  }
  if (jsonObj.contains("hananIters")) {
    cfg->hananIters = jsonObj["hananIters"].get<int>();
  }
  if (jsonObj.contains("heurLocSearchIters")) {
    cfg->heurLocSearchIters = jsonObj["heurLocSearchIters"].get<int>();
  }
  if (jsonObj.contains("ilpCacheThreshold")) {
    cfg->ilpCacheThreshold = jsonObj["ilpCacheThreshold"].get<double>();
  }
  if (jsonObj.contains("ilpTimeLimit")) {
    cfg->ilpTimeLimit = jsonObj["ilpTimeLimit"].get<int>();
  }
  if (jsonObj.contains("ilpCacheDir")) {
    cfg->ilpCacheDir = jsonObj["ilpCacheDir"].get<std::string>();
  }
  if (jsonObj.contains("ilpSolver")) {
    cfg->ilpSolver = jsonObj["ilpSolver"].get<std::string>();
  }
  if (jsonObj.contains("writeStats")) {
    cfg->writeStats = jsonObj["writeStats"].get<bool>();
  }
  if (jsonObj.contains("obstaclePath")) {
    cfg->obstaclePath = jsonObj["obstaclePath"].get<std::string>();
  }
  if (jsonObj.contains("deg2Heur")) {
    cfg->deg2Heur = jsonObj["deg2Heur"].get<bool>();
  }
  if (jsonObj.contains("enfGeoPen")) {
    cfg->enfGeoPen = jsonObj["enfGeoPen"].get<double>();
  }
  if (jsonObj.contains("maxGrDist")) {
    cfg->maxGrDist = jsonObj["maxGrDist"].get<double>();
  }
  if (jsonObj.contains("restrLocSearch")) {
    cfg->restrLocSearch = jsonObj["restrLocSearch"].get<bool>();
  }
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
      cfg->baseGraphType = BaseGraphType::GRID;
    } else if (baseGraphStr == "octilinear") {
      cfg->baseGraphType = BaseGraphType::OCTIGRID;
    } else if (baseGraphStr == "hexalinear") {
      cfg->baseGraphType = BaseGraphType::HEXGRID;
    } else if (baseGraphStr == "chulloctilinear") {
      cfg->baseGraphType = BaseGraphType::CONVEXHULLOCTIGRID;
    } else if (baseGraphStr == "pseudoorthoradial") {
      cfg->baseGraphType = BaseGraphType::PSEUDOORTHORADIAL;
    } else if (baseGraphStr == "quadtree") {
      cfg->baseGraphType = BaseGraphType::OCTIQUADTREE;
    } else if (baseGraphStr == "octihanan") {
      cfg->baseGraphType = BaseGraphType::OCTIHANANGRID;
    } else {
      LOG(ERROR) << "Unknown base graph type " << baseGraphStr;
      exit(1);
    }
  }
  if (jsonObj.contains("pens")) {
    auto pens = jsonObj["pens"];
    if (pens.contains("densityPen")) {
      cfg->pens.densityPen = pens["densityPen"].get<double>();
    }
    if (pens.contains("verticalPen")) {
      cfg->pens.verticalPen = pens["verticalPen"].get<double>();
    }
    if (pens.contains("horizontalPen")) {
      cfg->pens.horizontalPen = pens["horizontalPen"].get<double>();
    }
    if (pens.contains("diagonalPen")) {
      cfg->pens.diagonalPen = pens["diagonalPen"].get<double>();
    }
    if (pens.contains("p_0")) {
      cfg->pens.p_0 = pens["p_0"].get<double>();
    }
    if (pens.contains("p_135")) {
      cfg->pens.p_135 = pens["p_135"].get<double>();
    }
    if (pens.contains("p_90")) {
      cfg->pens.p_90 = pens["p_90"].get<double>();
    }
    if (pens.contains("p_45")) {
      cfg->pens.p_45 = pens["p_45"].get<double>();
    }
    if (pens.contains("ndMovePen")) {
      cfg->pens.ndMovePen = pens["ndMovePen"].get<double>();
    }
  }
  if (jsonObj.contains("skipOnError")) {
    cfg->skipOnError = jsonObj["skipOnError"].get<bool>();
  }
  if (jsonObj.contains("retryOnError")) {
    cfg->retryOnError = jsonObj["retryOnError"].get<bool>();
  }
  if (jsonObj.contains("gridSize")) {
    cfg->gridSize = jsonObj["gridSize"].get<std::string>();
  }
}
