// FileWatcher.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "vld/vld.h"
//#include "logger_define.h"
//#include "fmt/format.h"

//import FiboLogger;
import FiboMinidump;
import FiboStringUtils;
import std.core;

int test()
{
    fibo::StringUtils::convert("hello");
    std::cout << "hello world\n";
    return 0;
}

int wmain(int argc, wchar_t* argv[])
{
    //return test();
    fibo::MiniDump::monitoring("");
    using namespace std::string_literals;
    using namespace std::string_view_literals;
    try
    {
        //fibo::Logger::createLogger();
        //SPDLOG_INFO("{}", fibo::StringUtils::convert(L"hello world from wc2mb"));
        //SPDLOG_INFO(fibo::StringUtils::randAlphabet(32));

        // split
        auto vt = fibo::StringUtils::split("hello,world,ok", ",");
        //for(const auto& e : vt) SPDLOG_INFO(e);

        //// equal
        auto eq = fibo::StringUtils::equal("hello"s, "hello", true);
        //SPDLOG_INFO("equal: {}", eq);
    }
    catch (std::exception const& e)
    {
        std::cout << "Exception: " << e.what() << std::endl;
    }
    return 0;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
