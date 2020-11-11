#include "DummyCall.h"
#include "fibo_std.h"
#include "DummyCall.h"

//import FiboLogger; //++ TODO Can't use
import Fibo.Minidump;
import Fibo.StringUtils;

void DummyCall::print()
{
    try
    {
        using namespace std::string_literals;
        using namespace std::string_view_literals;

        //fibo::Logger::createLogger();
        auto wc2mb = fibo::StringUtils::convert(L"hello world from wc2mb");
        std::cout << wc2mb << '\n';

        auto mb2wc = fibo::StringUtils::convert("hello world from mb2wc");
        std::wcout << mb2wc << '\n';

        // split
        char const* nil = nullptr;
        auto vt = fibo::StringUtils::split("hello,world,ok", ",");
        for (const auto& e : vt) std::cout << e << '\n';

        // equal
        auto eq = fibo::StringUtils::equal("hello"s, "hello", true);
        std::cout << "equal: " << eq << '\n';
    }
    catch (std::exception const& e)
    {
        std::cout << "Exception: " << e.what() << std::endl;
    }
}