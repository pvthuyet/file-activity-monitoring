#include "DummyCall.h"
#include <iostream>

import Fibo.StringUtils;

void DummyCall::print()
{
	auto s = fibo::StringUtils::convert(L"DummyCall::print()\n");
	std::cout << s;
}