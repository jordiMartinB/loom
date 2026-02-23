// Copyright 2017, University of Freiburg,
// Chair of Algorithms and Data Structures.
// Authors: Patrick Brosi <brosi@informatik.uni-freiburg.de>

#ifndef OCTI_CONFIG_CONFIGREADER_H_
#define OCTI_CONFIG_CONFIGREADER_H_

#include <vector>
#include "octi/config/OctiConfig.h"

namespace octi {
namespace config {

class ConfigReader {
 public:
  ConfigReader();
  void read(Config* cfg, std::stringstream *jsonStream) const;
};
}
}
#endif  // OCTI_CONFIG_CONFIGREADER_H_
