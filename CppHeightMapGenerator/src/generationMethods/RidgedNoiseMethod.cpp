#include "generationMethods/RidgedNoiseMethod.hpp"

RidgedNoiseMethod::RidgedNoiseMethod(GenerationSettings settings, int seed, float scaleOverride) :
	GenerationMethodBase(settings, seed, scaleOverride)
{
}

RidgedNoiseMethod::~RidgedNoiseMethod()
{
}

float RidgedNoiseMethod::EvaluateHeight(const std::pair<float, float>& point)
{
	float perlinVal = 1.0f - std::abs(2.0f * static_cast<float>(perlin.noise2D_01(point.first, point.second)) - 1.0f);
	return std::pow(perlinVal, 2);
}