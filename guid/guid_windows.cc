#include <objbase.h>

#include "guid/guid.h"

namespace pack::guid {

void Guid::initialize(Guid* guid) {
  GUID new_id;
  CoCreateGuid(&new_id);

  guid->bytes_ = {(unsigned char)((new_id.Data1 >> 24) & 0xff),
                  (unsigned char)((new_id.Data1 >> 16) & 0xff),
                  (unsigned char)((new_id.Data1 >> 8) & 0xff),
                  (unsigned char)((new_id.Data1) & 0xff),

                  (unsigned char)((new_id.Data2 >> 8) & 0xff),
                  (unsigned char)((new_id.Data2) & 0xff),

                  (unsigned char)((new_id.Data3 >> 8) & 0xff),
                  (unsigned char)((new_id.Data3) & 0xff),

                  (unsigned char)new_id.Data4[0],
                  (unsigned char)new_id.Data4[1],
                  (unsigned char)new_id.Data4[2],
                  (unsigned char)new_id.Data4[3],
                  (unsigned char)new_id.Data4[4],
                  (unsigned char)new_id.Data4[5],
                  (unsigned char)new_id.Data4[6],
                  (unsigned char)new_id.Data4[7]};
}

}  // namespace pack::guid
