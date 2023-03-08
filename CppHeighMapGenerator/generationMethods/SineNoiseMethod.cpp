#include "SineNoiseMethod.hpp"

SineNoiseMethod::SineNoiseMethod(GenerationSettings generationSettings, int seed, float scaleOverride) :
	GenerationMethodBase{ generationSettings, seed, scaleOverride }
{
}

SineNoiseMethod::~SineNoiseMethod()
{
}

float SineNoiseMethod::EvaluateHeight(const std::pair<float, float>& point)
{
	float perlinVal = static_cast<float>(perlin.noise2D_01(point.first, point.second));

	float sinArg = 1 - std::abs(perlinVal * 2.f - 1);
	float val = std::sin(sinArg);
	return val;
}