#include <libwdb/process.h>
#include <libwdb/error.h>
#include <editline/readline.h>
#include <unistd.h>
#include <sys/ptrace.h>
#include <sys/wait.h>
#include <string_view>
#include <string>
#include <vector>
#include <algorithm>
#include <sstream>
#include <cstdlib>
#include <iostream>
#include <cstdio>
#include <cerrno>
#include <cstring>

namespace {

void print_stop_reason(
    const wdb::process& process,
    wdb::stop_reason reason) {
    std::cout << "Process " << process.pid() << ' ';
    switch (reason.reason) {
    case wdb::process_state::exited:
        std::cout << "exited with status "
            << static_cast<int>(reason.info);
        break;
    case wdb::process_state::terminated:
        std::cout << "terminated with signal "
            << static_cast<int>(reason.info);
        break;
    case wdb::process_state::stopped:
        std::cout << "stopped with signal "
            << sigabbrev_np(reason.info);
        break;
    }
    std::cout << std::endl;
}

std::vector<std::string> split(std::string_view str, char delimiter)
{
    std::vector<std::string> out{};
    std::stringstream ss{std::string{str}};
    std::string item;

    while (std::getline(ss, item, delimiter)) {
        out.push_back(item);
    }
    return out;
}

bool is_prefix(std::string_view str, std::string_view of)
{
    if (str.size() > of.size()) return false;
    return std::equal(str.begin(), str.end(), of.begin()); // ranges?
}

void resume(pid_t pid)
{
    if (ptrace(PTRACE_CONT, pid, nullptr, nullptr) < 0) {
        std::cerr << "Couldn't continue\n";
        std::exit(EXIT_FAILURE);
    }
}

void wait_on_signal(pid_t pid)
{
    int wait_status;
    int options = 0;
    if (waitpid(pid, &wait_status, options) < 0) {
        std::perror("waitpid failed");
        std::exit(EXIT_FAILURE);
    }
}

void handle_command(
    std::unique_ptr<wdb::process>& process,
    std::string_view line)
{
    auto args = split(line, ' ');
    auto command = args[0];

    if (is_prefix(command, "continue")) {
        process->resume();
        auto reason = process->wait_on_signal();
        print_stop_reason(*process, reason);
    } else {
        std::cerr << "Unknown command\n";
    }
}

std::unique_ptr<wdb::process> attach(int argc, const char** argv){
    pid_t pid = 0;
    // Passing ID
    using namespace std::string_view_literals;
    if (argc == 3 and argv[1] == "-p"sv) {
        pid = std::atoi(argv[2]);
        return wdb::process::attach(pid);
    } else {
        const char* program_path = argv[1];
        return wdb::process::launch(program_path);
    }
}

void main_loop(std::unique_ptr<wdb::process>& process) {
    char* line = nullptr;
    using namespace std::string_view_literals;
    while ((line = readline("wdb> ")) != nullptr) {
        std::string line_str;
        if (line == ""sv) {
            free(line);
            if (history_length > 0) {
                line_str = history_list()[history_length - 1]->line;
            }
        } else {
            line_str = line;
            add_history(line);
            free(line);
        }

        if (!line_str.empty()) {
            try {
                handle_command(process, line_str);
            } catch (const wdb::error& err) {
                std::cout << err.what() << '\n';
            }
        }
    }
}

} // namespace

int main(int argc, const char** argv)
{
    if (argc == 1) {
        std::cerr << "No arguments give.\n";
        return EXIT_FAILURE;
    }

    try {
        auto process = attach(argc, argv);
        main_loop(process);
    } catch (const wdb::error& err) {
        std::cout << err.what() << '\n';
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}

