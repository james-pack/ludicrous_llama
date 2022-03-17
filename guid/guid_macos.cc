#include <CoreFoundation/CFUUID.h>

#include "guid/guid.h"

namespace pack::guid {

void Guid::initialize(Guid* guid) {
  auto new_id = CFUUIDCreate(NULL);
  auto bytes = CFUUIDGetUUIDBytes(new_id);
  CFRelease(new_id);

  guid->bytes_ = {{bytes.byte0, bytes.byte1, bytes.byte2, bytes.byte3, bytes.byte4,  //
                   bytes.byte5, bytes.byte6, bytes.byte7, bytes.byte8,               //
                   bytes.byte9, bytes.byte10, bytes.byte11, bytes.byte12,            //
                   bytes.byte13, bytes.byte14, bytes.byte15}};
}

}  // namespace pack::guid
