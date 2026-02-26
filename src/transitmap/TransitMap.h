#ifndef TRANSITMAP_H
#define TRANSITMAP_H

#include <string>
#include <vector>

/// Run the transitmap stage.
///
/// Expects two arguments: graph JSON (or DOT) and config JSON (as strings).
/// Returns the rendered output (e.g. SVG) as a string.
std::string run_transitmap(const std::vector<std::string>& args);

#endif // TRANSITMAP_H