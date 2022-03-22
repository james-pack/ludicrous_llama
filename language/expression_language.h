#pragma once

#include <cassert>
#include <functional>
#include <string_view>
#include <vector>

#include "language/parse_tree.h"

namespace pack::language {

class ExpressionLanguage final {
 public:
  static bool parse(std::string_view expression, ParseStack& stack);
};

}  // namespace pack::language
