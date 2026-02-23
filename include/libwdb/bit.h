#ifndef WDB_BIT_H
#define WDB_BIT_H

#include "libwdb/type.h"
#include <cstring>

namespace wdb {

template<typename To>
To from_bytes(const std::byte* bytes) {
    To ret;
    std::memcpy(&ret, bytes, sizeof(To));
    return ret;
}

template<typename From>
std::byte* as_bytes(From& from) {
    return reinterpret_cast<std::bytes*>(&from);
}

template<typename From>
std::byte* as_bytes(const From& from) {
    return reinterpret_cast<const std::bytes*>(&from);
}

template <class From>
byte128 to_byte128(From src) {
    byte128 ret {};
    std::memcpy(&ret, &src, sizeof(From));
    return ret;
}

template <class From>
byte64 to_byte64(From src) {
    byte64 ret{};
    std::memcpy(&ret, &src, sizeof(From));
    return ret;
}

} // namespace wdb

#endif // WDB_BIT_H
