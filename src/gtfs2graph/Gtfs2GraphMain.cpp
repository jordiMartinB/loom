// Copyright 2017
// University of Freiburg - Chair of Algorithms and Datastructures
// Author: Patrick Brosi

#include <stdio.h>
#include <unistd.h>
#include <fstream>
#include <iostream>
#include <set>
#include <string>
#include "ad/cppgtfs/Parser.h"
#include "ad/cppgtfs/gtfs/Service.h"
#include "gtfs2graph/builder/Builder.h"
#include "gtfs2graph/config/GraphBuilderConfig.h"
#include "gtfs2graph/graph/BuildGraph.h"
#include "gtfs2graph/graph/EdgePL.h"
#include "gtfs2graph/graph/NodePL.h"
#include "util/geo/output/GeoGraphJsonOutput.h"
#include "util/log/Log.h"
#include "shared/config/ConfigReader.h"

using namespace gtfs2graph;
using util::ERROR;
using util::DEBUG;

// _____________________________________________________________________________
std::string run_gtfs2graph(const std::vector<std::string>& args) {

  if (args.size() != 2) {
    std::cerr << "Usage: module.run( [<graph_json_file>,<config_json_file>])"<< std::endl;
    return "";
  }

  std::stringstream graphStream(args[0]);
  std::stringstream configFile(args[1]);
  std::ostringstream ss;
  
  // initialize randomness
  srand(time(NULL) + rand());

  gtfs2graph::config::Config cfg;

  shared::config::ConfigReader cr;
  cr.read(&cfg, &configFile,
          [](gtfs2graph::config::Config* c) { /* leave defaults */ },
          gtfs2graph::config::jsonToConfig);

  // parse an example feed
  ad::cppgtfs::gtfs::Feed feed;

  if (!cfg.inputFeedPath.empty()) {
    try {
      ad::cppgtfs::Parser parser(cfg.inputFeedPath);
      parser.parse(&feed);
    } catch (const ad::cppgtfs::ParserException& ex) {
      LOG(ERROR) << "Could not parse input GTFS feed, reason was:";
      std::cerr << ex.what() << std::endl;
      exit(1);
    }
    gtfs2graph::graph::BuildGraph g;
    Builder b(&cfg);

    b.consume(feed, &g);

    b.simplify(&g);

    util::geo::output::GeoGraphJsonOutput out;
    out.printLatLng(g, ss);
  }

  return ss.str();
}
