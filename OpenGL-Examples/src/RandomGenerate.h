#pragma once
#include <random>
class RandomGenerate
{
public:
	static float GetRandomFloat()
	{
		return  dis(gen);
	}
private:
	static std::mt19937 gen;
	static std::uniform_real_distribution<float> dis;
};

