#include "BillowNoiseMethod.hpp"

BillowNoiseMethod::BillowNoiseMethod(GenerationSettings settings, int seed, float scaleOverride) :
	GenerationMethodBase(settings, seed, scaleOverride)
{
}

BillowNoiseMethod::~BillowNoiseMethod()
{
}

float BillowNoiseMethod::EvaluateHeight(const std::pair<float, float>& point)
{
	float perlinVal = static_cast<float>(perlin.noise2D_01(point.first, point.second));
	return std::abs(perlinVal * 2.0f - 1.0f);
}