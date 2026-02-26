// Copyright 2016, University of Freiburg,
// Chair of Algorithms and Data Structures.
// Authors: Patrick Brosi <brosi@informatik.uni-freiburg.de>

#ifndef SHARED_CONFIG_CONFIGREADER_H_
#define SHARED_CONFIG_CONFIGREADER_H_

#include <iostream>
#include <map>
#include <sstream>

#include "3rdparty/json.hpp"
#include "JsonConfigHelper.h"

namespace shared {
namespace config {

class ConfigReader {
 public:
  
  ConfigReader(){};

  template <typename ConfigT, typename DefaultsSetter, typename JsonMapper>
    void read(ConfigT* cfg, std::stringstream* jsonStream, DefaultsSetter defaults,
                       JsonMapper mapper);
  
};

#include "ConfigReader.tpp"

}
}


#endif  // SHARED_CONFIG_CONFIGREADER_H_