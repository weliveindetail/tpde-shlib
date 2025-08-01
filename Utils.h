#pragma once

#include <algorithm>
#include <optional>
#include <string>

inline std::optional<std::string> getEnv(const std::string &Var) {
  const char *Val = std::getenv(Var.c_str());
  if (!Val)
    return std::nullopt;
  return std::string(Val);
}

inline bool getEnvBool(const std::string &VarName, bool Default = false) {
  if (auto ValOpt = getEnv(VarName)) {
    std::string V = *ValOpt;
    std::transform(V.begin(), V.end(), V.begin(), ::tolower);

    if (V == "1" || V == "true" || V == "yes" || V == "on")
      return true;
    if (V == "0" || V == "false" || V == "no" || V == "off")
      return false;
  }
  return Default;
}
