module Fibo.DummyManager;

import Fibo.KeyboardManager;

namespace fibo
{
	void DummyManager::print()
	{
		fibo::KeyboardManager{}.wait();
	}
}