#pragma once
#include "GenerationMethodBase.h"
#include "../utilities/PerlinNoise.hpp"

class PerlinNoiseMethod : public GenerationMethodBase
{
public:
	PerlinNoiseMethod(GenerationSettings settings, int seed, float scaleOverride);
	~PerlinNoiseMethod();

private:
	// Inherited via GenerationMethodBase
	virtual float EvaluateHeight(const std::pair<float, float>& point) override;
	virtual float EvaluateHeight(const std::pair<float, float>& point, const std::vector<std::pair<float, float>>& octaveOffsets, size_t startingIndex, size_t endingIndex, float maskValue) override;

private:
	const siv::PerlinNoise perlin;
};
