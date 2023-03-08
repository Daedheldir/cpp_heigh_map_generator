#pragma once
#include <vector>
#include "GenerationSettings.h"
#include "../utilities/PerlinNoise.hpp"

class GenerationMethodBase
{
public:
	GenerationMethodBase(GenerationSettings settings, int seed, float scaleOverride);
	virtual ~GenerationMethodBase();

	virtual float EvaluateHeight(const std::pair<float, float>& point) = 0;
	virtual float EvaluateHeight(const std::pair<float, float>& point, const std::vector<std::pair<float, float>>& octaveOffsets, size_t startingIndex, size_t endingIndex, float maskValue = 0);
	virtual std::vector<std::vector<float>> CreateHeightMap(const std::pair<float, float>& generationOffset);
protected:
	std::pair<float, float> EvaluateSamplePoint(const std::pair<float, float>& point, const std::pair<float, float>& octaveOffset, float frequency);
protected:
	GenerationSettings settings;
	const siv::PerlinNoise perlin;
private:
	std::vector<std::pair<float, float>> randomOffsets;
	float scaleOverride;
};
