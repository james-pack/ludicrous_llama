#include <iostream>

#include "guid/guid.h"

int main() {
  using Guid = pack::guid::Guid;

  std::cout << Guid{}.as_string() << "\n";
}
