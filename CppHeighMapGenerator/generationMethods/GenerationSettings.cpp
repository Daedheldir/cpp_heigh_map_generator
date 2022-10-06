#include "GenerationSettings.h"

GenerationSettings::GenerationSettings(GenerationMethodType methodType, int octaves, float scale, float weight, float persistance, float smoothing, int chunkSize)
{
	setGenerationMethodType(methodType);
	setOctaves(octaves);
	setScale(scale);
	setWeight(weight);
	setPersistance(persistance);
	setSmoothing(smoothing);
	setChunkSize(chunkSize);
}

GenerationSettings::~GenerationSettings()
{
}