#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include <iostream>

#ifdef _WIN32
#include <windows.h>
#include <tchar.h>

class ProcessRunner
{
public:
    ProcessRunner()
    {
        Initialize();
    }

    const ProcessRunner(const ProcessRunner& rhs) = delete;
    ProcessRunner& operator=(const ProcessRunner& rhs) = delete;

    void Run(TCHAR* command)
    {
        command_ = command;
        create_process();
        ReadOutput();
        Close();
    }

    std::string output() { return output_; }
private:
    void Initialize()
    {
        InitSecuityAttributes();
        create_pipe();
        InitProcessInfo();
        InitStartupInfo();
    }

    void InitSecuityAttributes()
    {
        sa_attr_.nLength = sizeof(SECURITY_ATTRIBUTES);
        sa_attr_.bInheritHandle = TRUE;
        sa_attr_.lpSecurityDescriptor = NULL;
    }

    void InitProcessInfo()
    {
        ZeroMemory(&pi_, sizeof(PROCESS_INFORMATION));
    }

    void InitStartupInfo()
    {
        ZeroMemory(&si_, sizeof(STARTUPINFO));
        si_.cb = sizeof(STARTUPINFO);
        si_.hStdOutput = h_child_output_write_;
        si_.hStdError = h_child_output_write_;
        si_.dwFlags |= STARTF_USESTDHANDLES;

        // Do not redirect the standard input
        si_.hStdInput = GetStdHandle(STD_INPUT_HANDLE);
    }

    void create_pipe()
    {
        if (!CreatePipe(&h_child_output_read_,
            &h_child_output_write_,
            &sa_attr_, 0)) {
            throw std::runtime_error("Failed to create pipe");
        }
    }

    void create_process()
    {
        // Create the child process
        if (!CreateProcess(NULL, const_cast<TCHAR*>(command_), NULL, 
            NULL, TRUE, 0, NULL, NULL, &si_, &pi_)) {
            CloseHandle(h_child_output_read_);
            CloseHandle(h_child_output_write_);
            throw std::runtime_error("CreateProcess failed");
        }

        CloseHandle(h_child_output_write_);
    }

    void ReadOutput()
    {
        CHAR buffer[4096];
        DWORD bytes_read;

        while (ReadFile(h_child_output_read_, buffer, sizeof(buffer) - 1,
            &bytes_read, NULL) && bytes_read > 0) {
            buffer[bytes_read] = '\0';
            output_ += buffer;
        }
    }
    
    void Close()
    {
        CloseHandle(h_child_output_read_);
        CloseHandle(pi_.hProcess);
        CloseHandle(pi_.hThread);
    }

    SECURITY_ATTRIBUTES sa_attr_;
    HANDLE h_child_output_read_;
    HANDLE h_child_output_write_;
    STARTUPINFO si_;
    PROCESS_INFORMATION pi_;
    TCHAR* command_ = nullptr;
    std::string output_;
};

bool run_process(TCHAR* command)
{
    ProcessRunner runner;

    try {
        runner.Run(command);
        std::cout << runner.output();
        return true;
    }
    catch (const std::exception& ex) {
        std::cerr << ex.what() << std::endl;
        return false;
    }
}
#else
TEST_CASE("run_process_stub") {
    std::cout << "Windows process running tests are disabled on non-Windows platforms.\n";
    CHECK(true);
}
#endif

