// Copyright 2016
// University of Freiburg - Chair of Algorithms and Datastructures
// Author: Patrick Brosi

#include <stdio.h>

#ifndef _WIN32
  #include <unistd.h>
#endif

#include <fstream>
#include <iostream>
#include <set>
#include <string>

#include "shared/rendergraph/Penalties.h"
#include "shared/rendergraph/RenderGraph.h"
#include "transitmap/config/TransitMapConfig.h"
#include "transitmap/graph/GraphBuilder.h"
#include "transitmap/output/MvtRenderer.h"
#include "transitmap/output/SvgRenderer.h"
#include "transitmap/TransitMap.h"
#include "util/log/Log.h"
#include "shared/config/ConfigReader.h"

using shared::linegraph::LineGraph;
using shared::rendergraph::RenderGraph;
using transitmapper::graph::GraphBuilder;

using util::DEBUG;
using util::ERROR;

// _____________________________________________________________________________
std::string run_transitmap(const std::vector<std::string>& args) {

  if (args.size() != 2) {
    std::cerr << "Usage: module.run( [<graph_json_file>,<config_json_file>])"<< std::endl;
    return "";
  }

  std::stringstream graphStream(args[0]);
  std::stringstream configFile(args[1]);
  std::ostringstream ss;
  
  // initialize randomness
  srand(time(NULL) + rand());

  transitmapper::config::Config cfg;

  shared::config::ConfigReader cr;
  cr.read(&cfg, &configFile,
          [](transitmapper::config::Config* c) { /* leave defaults */ },
          transitmapper::config::jsonToConfig);

  T_START(TIMER);

  GraphBuilder b(&cfg);

  LOGTO(DEBUG, std::cerr) << "Reading graph...";

  if (cfg.renderMethod == "mvt") {
#ifdef PROTOBUF_FOUND
    LineGraph lg;
    if (cfg.fromDot)
      lg.readFromDot(&graphStream);
    else
      lg.readFromJson(&graphStream);

    if (cfg.randomColors) lg.fillMissingColors();

    // snap orphan stations
    lg.snapOrphanStations();

    for (size_t z : cfg.mvtZooms) {
      double lWidth = cfg.lineWidth;
      double lSpacing = cfg.lineSpacing;
      double lOutlineWidth = cfg.outlineWidth;

      lWidth *= 156543.0 / (1 << z);
      lSpacing *= 156543.0 / (1 << z);
      lOutlineWidth *= 156543.0 / (1 << z);

      RenderGraph g(lg, lWidth, lOutlineWidth, lSpacing);

      g.contractStrayNds();
      g.smooth(cfg.inputSmoothing);
      b.writeNodeFronts(&g);
      b.expandOverlappinFronts(&g);

      g.createMetaNodes();

      // avoid overlapping stations
      if (true) {
        b.dropOverlappingStations(&g);
        g.contractStrayNds();
        b.expandOverlappinFronts(&g);
        g.createMetaNodes();
      }

      LOGTO(DEBUG, std::cerr) << "Outputting to MVT ...";
      transitmapper::output::MvtRenderer mvtOut(&cfg, z);
      mvtOut.print(g);
    }
#else
    LOG(ERROR) << "transitmap was not compiled with protocol buffers support, "
                  "cannot use render method "
               << cfg.renderMethod;
    exit(1);
#endif
  } else if (cfg.renderMethod == "svg") {
    RenderGraph g(cfg.lineWidth, cfg.outlineWidth, cfg.lineSpacing);
    if (cfg.fromDot)
      g.readFromDot(&graphStream);
    else
      g.readFromJson(&graphStream);

    if (cfg.randomColors) g.fillMissingColors();

    // snap orphan stations
    g.snapOrphanStations();

    g.contractStrayNds();
    g.smooth(cfg.inputSmoothing);
    b.writeNodeFronts(&g);
    b.expandOverlappinFronts(&g);
    g.createMetaNodes();

    if (true) {
      b.dropOverlappingStations(&g);
      g.contractStrayNds();
      b.expandOverlappinFronts(&g);
      g.createMetaNodes();
    }

    LOGTO(DEBUG, std::cerr) << "Outputting to SVG ...";
    // to string
    transitmapper::output::SvgRenderer svgOut(&ss, &cfg);
    svgOut.print(g);
  } else {
    LOG(ERROR) << "Unknown render method " << cfg.renderMethod;
    exit(1);
  }

  double took = T_STOP(TIMER);

  return ss.str();
}
