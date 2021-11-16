#pragma once

#include "NonCopyable.h"

template <class T>
struct Singleton : NonCopyable
{
	static T& GetInstance()
	{
		static T instance;
		return instance;
	}
};
