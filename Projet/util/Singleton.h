#pragma once

#include "NonCopyable.h"

template <class T>
class Singleton : public NonCopyable
{
public:
	static T& GetInstance()
	{
		static T instance;
		return instance;
	}

private:
	Singleton() = default;
};
