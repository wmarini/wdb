#ifndef WDB_REGISTERS_H
#define WDB_REGISTERS_H

#include "libwdb/types.h"
#include "libwdb/register_info.h"
#include <sys/user.h>

namespace wdb {

class process;
class registers {
public:
    registers() = delete;
    registers(const registers&) = delete;
    registers& operator=(const registers&) = delete;

    using value = std::variant<
                        std::uint8_t, std::uint16_t, std::uint32_t, std::uint64_t,
                        std::int8_t, std::int16_t, std::int32_t, std::int64_t,
                        float, double, long double,
                        byte64, byte128>;

    value read(const register_info& info) const;
    void write(const register_info& info, value val);
    
    template<typename T>
    T read_by_id_as(registers_id id) const {
        return std::get<T>(read(register_info_by_id(id)));
    }
    void write_by_id(register_id id, value val) {
        write(register_info_by(id), val);
    }

private:
    friend process;
    registers(process& proc) : proc_(&proc) {}

    user data_
    process* proc_;
};

} // namespace wdb

#endif // WDB_REGISTERS_H
