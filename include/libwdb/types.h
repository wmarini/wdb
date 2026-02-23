#ifndef WDB_TYPES_H
#define WDB_TYPES_H

#include <array>
#include <cstddef>

namespace wdb {

using byte64 = std::array<std::byte, 8>;
using byte128 = std::array<std::byte, 16>;

} // namespace wdb

#endif // WDB_TYPES_H
