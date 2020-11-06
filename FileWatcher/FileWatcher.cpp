// FileWatcher.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "vld/vld.h"
#include "logger_define.h"
//#include "fmt/format.h"
#include <exception>
#include <iostream>
#include <cassert>
#include <concepts>
#include <string>

import FiboLogger;
import FiboMinidump;
import FiboStringUtils;
import FiboConcept;

template<auto X, auto Y>
struct ValueIdentity {
    static constexpr auto value = X;
};

int test()
{
    static_assert(42 == ValueIdentity<42, 'x'>::value);
    static_assert(std::is_same_v<char,      fibo::primitive_string_t<char[12]>>, "Not the same type");
    static_assert(std::is_same_v<char,      fibo::primitive_string_t<char const[12]>>, "Not the same type");
    static_assert(std::is_same_v<char,      fibo::primitive_string_t<char const [12]>>, "Not the same type");
    static_assert(std::is_same_v<char,      fibo::primitive_string_t<char *>>, "Not the same type");
    static_assert(std::is_same_v<char,      fibo::primitive_string_t<char const *>>, "Not the same type");
    static_assert(std::is_same_v<char,      fibo::primitive_string_t<char * const>>, "Not the same type");
    static_assert(std::is_same_v<char,      fibo::primitive_string_t<std::string>>, "Not the same type");
    static_assert(std::is_same_v<char,      fibo::primitive_string_t<std::string_view>>, "Not the same type");
    static_assert(std::is_same_v<wchar_t,   fibo::primitive_string_t<std::wstring>>, "Not the same type");
    static_assert(std::is_same_v<wchar_t,   fibo::primitive_string_t<std::wstring_view>>, "Not the same type");
    static_assert(std::is_same_v<char8_t,   fibo::primitive_string_t<std::u8string>>, "Not the same type");
    static_assert(std::is_same_v<char8_t,   fibo::primitive_string_t<std::u8string_view>>, "Not the same type");
    static_assert(std::is_same_v<char16_t,  fibo::primitive_string_t<std::u16string>>, "Not the same type");
    static_assert(std::is_same_v<char16_t,  fibo::primitive_string_t<std::u16string_view>>, "Not the same type");
    static_assert(std::is_same_v<char32_t,  fibo::primitive_string_t<std::u32string>>, "Not the same type");
    static_assert(std::is_same_v<char32_t,  fibo::primitive_string_t<std::u32string_view>>, "Not the same type");
    //static_assert(std::is_same_v<char,      fibo::primitive_string_t<std::nullptr_t>>, "Not the same type");
    //static_assert(std::is_same_v<char,      fibo::primitive_string_t<void>>, "Not the same type");
    //static_assert(std::is_same_v<char, fibo::primitive_string_t<int>>, "Not the same type");
    //const char* c = nullptr;
    //static_assert(std::is_same_v<char, fibo::primitive_string_t<decltype(c)>>, "Not the same type");
    //auto s = fmt::format("hello world");
    //std::cout << s << '\n';
    return 0;
}

int wmain(int argc, wchar_t* argv[])
{
    return test();
    using namespace std::string_literals;
    using namespace std::string_view_literals;
    fibo::MiniDump::monitoring();
    try
    {
        fibo::Logger::createLogger();
        SPDLOG_INFO("{}", fibo::StringUtils::convert(L"hello world from wc2mb"));
        SPDLOG_INFO(fibo::StringUtils::randAlphabet(32));

        // split
        auto vt = fibo::StringUtils::split("hello,world,ok", ",");
        for(const auto& e : vt) SPDLOG_INFO(e);

        // equal
        char const* c = NULL;
        auto eq = fibo::StringUtils::equal("hello"s, c, true);
        SPDLOG_INFO("equal: {}", eq);
    }
    catch (std::exception const& e)
    {
        std::cout << "Exception: " << e.what() << std::endl;
        //assert(false);
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
