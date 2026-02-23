#include <libwdb/process.h>
#include <libwdb/pipe.h>
#include <libwdb/error.h>
#include <sys/ptrace.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <format>
#include <unistd.h>

namespace wdb {

void exit_with_perror(pipe& channel, std::string const& prefix)
{
    auto message = std::format("{}: {}", prefix, std::strerror(errno));
    channel.write(
        reinterpret_cast<std::byte*>(message.data()), message.size());
    std::exit(-1);
}

stop_reason::stop_reason(int wait_status)
{
    if (WIFEXITED(wait_status)) {
        reason = process_state::exited;
        info = WEXITSTATUS(wait_status);
    } else if (WIFSIGNALED(wait_status)) {
        reason = process_state::terminated;
        info = WTERMSIG(wait_status);
    } else if (WIFSTOPPED(wait_status)) {
        reason = process_state::stopped;
        info = WSTOPSIG(wait_status);
    }
}

process::~process()
{
    if (pid_ != 0) {
        int status;
        if (is_attached_) {
            if (state_ == process_state::running) {
                kill(pid_, SIGSTOP);
                waitpid(pid_, &status, 0);
            }
            ptrace(PTRACE_DETACH, pid_, nullptr, nullptr);
            kill(pid_, SIGCONT);
        }
        if (terminate_on_end_) {
            kill(pid_, SIGKILL);
            waitpid(pid_, &status, 0);
        }
    }
}

std::unique_ptr<process> process::launch(
    std::filesystem::path path, bool debug)
{
    pipe channel(/*close_on_exec=*/true);

    pid_t pid;
    if ((pid = fork()) < 0) {
        error::send_errno("fork failed");
    }

    if (pid == 0) {
        channel.close_read();
        if (debug and ptrace(PTRACE_TRACEME, 0, nullptr, nullptr) < 0) {
            exit_with_perror(channel, "Tracing failed");
        }
        if (execlp(path.c_str(), path.c_str(), nullptr) < 0) {
            exit_with_perror(channel, "exec failed");
        }
    }

    channel.close_write();
    auto data = channel.read();
    channel.close_read();

    if (data.size() > 0) {
        waitpid(pid, nullptr, 0);
        auto chars = reinterpret_cast<char*>(data.data());
        error::send(std::string(chars, chars + data.size()));
    }

    std::unique_ptr<process> proc (new process(pid, /*terminated_on_end*/true, debug));

    if (debug) {
        proc->wait_on_signal();
    }

    return proc;
}

std::unique_ptr<process> process::attach(pid_t pid)
{
    if (pid == 0) {
        error::send("invalid PID");
    }
    if (ptrace(PTRACE_ATTACH, pid, nullptr, nullptr) < 0) {
        error::send_errno("Could not attach");
    }

    std::unique_ptr<process> proc(
        new process(pid, /*terminated_on_end*/false, /*attached=*/true));
    proc->wait_on_signal();

    return proc;
}

void process::resume()
{
    if (ptrace(PTRACE_CONT, pid_, nullptr, nullptr) < 0) {
        error::send_errno("Could not resume");
    }
    state_ = process_state::running;
}

stop_reason process::wait_on_signal()
{
    int wait_status;
    int options = 0;
    if (waitpid(pid_, &wait_status, options) < 0) {
        error::send_errno("waitpid failed");
    }
    stop_reason reason(wait_status);
    state_ = reason.reason;

    if (is_attached_ and state_ == process_state::stopped) {
        read_all_registers();
    }
    
    return reason;
}

void process::write_user_area(std::size_t offset, std::uint64_t data)
{
    if (ptrace(PTRACE_POKEUSER, pid_. offset, data) < 0) {
        error::send_errno("Could not write to user area");
    }
}

void process::read_all_registers()
{
    if (ptrace(PTRACE_GETREGS, pid, nullptr, &get_registers().data_.regs) < 0) {
        error::send_errno("Could not read GPR registers");
    }
    if (ptrace(PTRACE_GETFPREGS, pid_, nullptr, &get_gereisters().data_.i387) < 0) {
        error::send_errno("Could not read FPR registers");
    }
    for (int i = 0; i < 8; ++i) {
        auto id = static_cast<int>(register_id::dro) + i;
        auto info = register_info_by_id(static_cast<register_id>(id));

        errno = 0;
        std::int64_t data = ptrace(PTRACE_PEEKUSER, pid_, info.offset, nullptr);
        if (errno != 0) {
            error::send_errno("Could not read debug register");
        }

        get_register().data_.u_debugreg[i] = data;
    }
}


} // namespace wdb