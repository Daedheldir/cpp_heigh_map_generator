#include "PerlinNoiseMethod.h"

PerlinNoiseMethod::PerlinNoiseMethod(GenerationSettings settings, int seed, float scaleOverride) :
	GenerationMethodBase(settings, seed, scaleOverride)
{
}

PerlinNoiseMethod::~PerlinNoiseMethod()
{
}

float PerlinNoiseMethod::EvaluateHeight(const std::pair<float, float>& point)
{
	float val = static_cast<float>(perlin.noise2D_01(point.first, point.second));
	return val;
}