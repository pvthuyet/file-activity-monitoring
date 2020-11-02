// FileWatcher.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "vld/vld.h"
#include "logger_define.h"
#include "fmt/format.h"
#include <exception>
#include <iostream>
#include <cassert>
#include <concepts>

import FiboLogger;
import FiboMinidump;
import FiboStringUtils;

int wmain(int argc, wchar_t* argv[])
{
    using namespace std::string_literals;
    using namespace std::string_view_literals;
    fibo::MiniDump::monitoring();
    try
    {
        fibo::Logger::createLogger();
        SPDLOG_INFO("{}", fibo::StringUtils::convert(L"hello world from wc2mb"));
        SPDLOG_INFO(L"{}", fibo::StringUtils::convert("hello world from mb2wc"));
        SPDLOG_INFO(fibo::StringUtils::randAlphabet(32));

        auto vt = fibo::StringUtils::split("hello,world,ok"sv, ","sv);
        for(const auto& e : vt) SPDLOG_INFO(e);
    }
    catch (std::exception const& e)
    {
        std::cout << "Exception: " << e.what() << std::endl;
        assert(false);
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
