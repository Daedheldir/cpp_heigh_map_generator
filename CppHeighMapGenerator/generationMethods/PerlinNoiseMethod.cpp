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
	float val = static_cast<float>(perlin.normalizedOctave2D_01(point.first, point.second, 1));
	return val;
}

float PerlinNoiseMethod::EvaluateHeight(const std::pair<float, float>& point, const std::vector<std::pair<float, float>>& octaveOffsets, size_t startingIndex, size_t endingIndex, float maskValue)
{
	std::pair<float, float> sample;

	//if starting index is 0 use frequency of 1
	float amplitude = (startingIndex > 0) ? (1 * (settings.getPersistance() * startingIndex)) : 1;
	float frequency = (startingIndex > 0) ? (1 / (settings.getSmoothing() * startingIndex)) : 1;
	float noiseHeight = 0;

	for (int i = startingIndex; i < endingIndex; ++i)
	{
		sample = EvaluateSamplePoint(point, octaveOffsets[i], frequency);

		float perlinValue = EvaluateHeight(sample);

		//if its not the first octave and use it as mask
		if (i != 0 && settings.isFirstOctaveAsMask())
		{
			perlinValue *= maskValue;
		}

		noiseHeight += perlinValue * amplitude;

		amplitude *= settings.getPersistance();
		frequency /= settings.getSmoothing();
	}

	return noiseHeight;
}