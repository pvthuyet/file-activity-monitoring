// FileWatcher.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "vld/vld.h"
#include "logger_define.h"
#include "fmt/format.h"
#include <exception>
#include <iostream>
#include <cassert>
#include <concepts>
#include <string>

import FiboLogger;
import FiboMinidump;
import FiboStringUtils;
import FiboConcept;

#include <iostream>
#include <type_traits>

template <typename T, typename U>
struct decay_equiv :
    std::is_same<std::remove_cvref_t<std::remove_pointer_t<std::decay_t<T>>>, U>::type
{};

void testDecay()
{
    std::cout << std::boolalpha
        << decay_equiv<char, char>::value << '\n'
        << decay_equiv<char&, char>::value << '\n'
        << decay_equiv<char&&, char>::value << '\n'
        << decay_equiv<const char&, char>::value << '\n'
        << decay_equiv<char[100], char>::value << '\n'
        << decay_equiv<char*, char>::value << '\n'
        << decay_equiv<const char*, char>::value << '\n'
        ;
}


template<typename SRC, typename TOKEN> requires fibo::StringablePair<SRC, TOKEN>
auto split2(const SRC& s, const TOKEN& token)
{
    //using demoType = fibo::TString_t<std::string>;
    std::cout << "is_string_v: " << fibo::is_string_v<SRC> << '\n';
    using typ = fibo::TString_t<SRC>;
    std::cout << typeid(typ).name() << '\n';
}

int test()
{
    split2(std::string("a"), std::string{"b"});
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
        //SPDLOG_INFO("{}", fibo::StringUtils::convert(L"hello world from wc2mb"));
        //SPDLOG_INFO(fibo::StringUtils::randAlphabet(32));

        // split
        //auto vt = fibo::StringUtils::split(std::string{ "hello,world,ok" }, std::string{ "," });
        //for(const auto& e : vt) SPDLOG_INFO(e);

        // equal
        //auto eq = fibo::StringUtils::equal("hello", "helLo"sv, true);
        //SPDLOG_INFO("equal: {}", eq);
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
