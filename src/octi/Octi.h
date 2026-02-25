#ifndef OCTI_MAIN_H
#define OCTI_MAIN_H

#include <string>
#include <vector>
#include "shared/linegraph/LineGraph.h"
#include "octi/combgraph/CombGraph.h"
#include "util/json/Writer.h"
#include "octi/basegraph/BaseGraph.h"
#include "octi/config/OctiConfig.h"

struct TotalScore;

double avgStatDist(const shared::linegraph::LineGraph& g);
const octi::combgraph::CombNode* getCenterNd(const octi::combgraph::CombGraph* cg);
std::vector<util::geo::DPolygon> readObstacleFile(const std::string& path);
void drawComp(shared::linegraph::LineGraph& tg, double avgDist,
              std::vector<util::json::Val>& jsonScores,
              std::vector<shared::linegraph::LineGraph*>& resultGraphs,
              std::vector<octi::basegraph::BaseGraph*>& resultGridGraphs,
              TotalScore& totScore, const octi::config::Config& cfg);
std::string run_octi(const std::vector<std::string>& args);
#endif  // OCTI_MAIN_H