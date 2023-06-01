#include "generationMethods/CosineNoiseMethod.hpp"

CosineNoiseMethod::CosineNoiseMethod(GenerationSettings generationSettings, int seed, float scaleOverride) :
	GenerationMethodBase{ generationSettings, seed, scaleOverride }
{
}

CosineNoiseMethod::~CosineNoiseMethod()
{
}

float CosineNoiseMethod::EvaluateHeight(const std::pair<float, float>& point)
{
	float perlinVal = static_cast<float>(perlin.noise2D_01(point.first, point.second));

	float CosinArg = 2 * perlinVal - 1.f;
	return 2 * (1 - std::cos(CosinArg));
}