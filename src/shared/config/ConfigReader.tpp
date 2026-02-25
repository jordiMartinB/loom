// Generic config reader: caller provides a defaults setter and a JSON->config mapper.
// If jsonStream is null or empty, defaults are left in place and mapper is not invoked.
template <typename ConfigT, typename DefaultsSetter, typename JsonMapper>
void ConfigReader::read(ConfigT* cfg, std::stringstream* jsonStream, DefaultsSetter defaults,
                       JsonMapper mapper) {
  // set module defaults
  defaults(cfg);

  // parse JSON and call mapper
  nlohmann::json jsonObj;
  if (!shared::config::parseJsonStream(jsonStream, jsonObj)) return;
  mapper(jsonObj, cfg);
}