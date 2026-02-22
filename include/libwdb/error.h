#ifndef WDB_ERROR_H
#define WDB_ERROR_H

#include <format>
#include <stdexcept>
#include <cstring>

namespace wdb {

class error : public std::runtime_error {
public:
    [[noreturn]]
    static void send(const std::string& what) { throw error(what); }

    [[noreturn]]
    static void send_errno(const std::string& prefix) {
        throw error(std::format("{}: ",prefix, std::strerror(errno)));
    }

private:
    error(const std::string& what) : std::runtime_error(what) {}
};

} // namespace wdb

#endif // WDB_ERROR_H