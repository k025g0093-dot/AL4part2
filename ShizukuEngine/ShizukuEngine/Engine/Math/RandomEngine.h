#pragma once
#include <random>
class RandomEngine {

public:
	float GetRandomFloat(float min, float max);
	int32_t GetRandomInt(int32_t min, int32_t max);

private:
	static std::random_device seedGenerator_;
	static std::mt19937_64 randomEngine_;
};