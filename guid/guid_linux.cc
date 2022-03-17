#include <uuid/uuid.h>

#include "guid/guid.h"

namespace pack::guid {

void Guid::initialize(Guid* guid) {
  static_assert(std::is_same<unsigned char[16], uuid_t>::value, "Wrong type!");
  uuid_generate(guid->bytes_.data());
}

}  // namespace pack::guid
