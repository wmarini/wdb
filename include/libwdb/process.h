#ifndef WDB_PROCESS_H
#define WDB_PROCESS_H

#include <filesystem>
#include <memory>
#include <sys/types.h>

namespace wdb {

enum class process_state {
    stopped,
    running,
    exited,
    terminated
};

struct stop_reason {
    stop_reason(int wait_status);

    process_state reason;
    std::uint8_t info;
};

class process {
public:
    ~process();

    static std::unique_ptr<process> launch(
        std::filesystem::path path, bool debug = true);
    static std::unique_ptr<process> attach(pid_t pid);

    process_state state() const { return state_; }
    void resume();
    stop_reason wait_on_signal();
    pid_t pid() const { return pid_; }

    process() = delete;
    process(const process&) = delete;
    process& operator=(const process&) = delete;

private:
    explicit process(pid_t pid, bool terminate_on_end, bool is_attached)
    : pid_{pid}
    , terminate_on_end_{terminate_on_end}
    , is_attached_{is_attached} {}

private:
    pid_t pid_ = 0;
    bool terminate_on_end_ = true;
    process_state state_ = process_state::stopped;
    bool is_attached_ = true;
};

} // namespace wdb

#endif // WDB_PROCESS_H
