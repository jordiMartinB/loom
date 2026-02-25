#ifndef SHARED_CONFIG_JSONCONFIGHELPER_H_
#define SHARED_CONFIG_JSONCONFIGHELPER_H_

#include "3rdparty/json.hpp"
#include <sstream>
#include <iostream>
#include <map>

namespace shared {
namespace config {

// Parse JSON from stream. Returns false if stream is null (no JSON provided).
inline bool parseJsonStream(std::stringstream* ss, nlohmann::json& out) {
  if (!ss) return false;
  try {
    (*ss) >> out;
    return true;
  } catch (const nlohmann::json::parse_error& e) {
    std::cerr << "Error parsing JSON: " << e.what() << std::endl;
    exit(1);
  }
}

// Assign JSON value to cfg via setter if key exists and is convertible to T.
template <typename T, typename Setter>
inline void assignIfContains(const nlohmann::json& j, const std::string& key, Setter setter) {
  if (j.contains(key)) {
    try {
      setter(j.at(key).get<T>());
    } catch (...) {
      // ignore conversion errors here; callers can validate after
    }
  }
}

// Convenience for boolean flags where presence toggles a field.
template <typename Setter>
inline void assignIfContainsBool(const nlohmann::json& j, const std::string& key, Setter setter) {
  if (j.contains(key)) {
    try {
      bool v = j.at(key).get<bool>();
      setter(v);
    } catch (...) {}
  }
}

// Map string values to enum and call setter; exits on unknown value.
template <typename Enum, typename Setter>
inline void assignIfContainsEnum(const nlohmann::json& j, const std::string& key,
                                 const std::map<std::string, Enum>& map, Setter setter) {
  if (j.contains(key)) {
    std::string s;
    try { s = j.at(key).get<std::string>(); } catch (...) { return; }
    auto it = map.find(s);
    if (it != map.end()) {
      setter(it->second);
    } else {
      std::cerr << "Unknown value for " << key << ": " << s << std::endl;
      exit(1);
    }
  }
}

}  // namespace config
}  // namespace shared

#endif  // SHARED_CONFIG_JSONCONFIGHELPER_H_