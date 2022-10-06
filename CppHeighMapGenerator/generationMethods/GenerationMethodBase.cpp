#include "GenerationMethodBase.h"

GenerationMethodBase::GenerationMethodBase(GenerationSettings settings, int seed, float scaleOverride) :
	settings(settings)
{
	randomOffsets.reserve(settings.getOctaves());
	for (int i = 0; i < settings.getOctaves(); ++i) {
		float offsetX = static_cast<float>((rand() % 200000) - 100000);
		float offsetY = static_cast<float>((rand() % 200000) - 100000);
		randomOffsets.emplace_back(std::make_pair(offsetX, offsetY));
	}
	this->scaleOverride = scaleOverride;
}

GenerationMethodBase::~GenerationMethodBase()
{
}

std::vector<std::vector<float>> GenerationMethodBase::CreateHeightMap(const std::pair<float, float>& generationOffset)
{
	std::vector<std::pair<float, float>> octaveOffsets;
	octaveOffsets.reserve(settings.getOctaves());

	for (int i = 0; i < settings.getOctaves(); ++i) {
		octaveOffsets.emplace_back(std::make_pair(
			randomOffsets[i].first + generationOffset.first,
			randomOffsets[i].second + generationOffset.second
		)
		);
	}

	std::vector<std::vector<float>> map;
	map.resize(settings.getChunkSize());
	for (auto& row : map)
		row.resize(settings.getChunkSize());

	std::vector<std::vector<float>> mask;
	mask.resize(settings.getChunkSize());
	for (auto& row : mask)
		row.resize(settings.getChunkSize());

	//creating first octave
	const size_t maskStartingIndex = 0;
	const size_t maskEndingIndex = 1;
	for (int y = 0; y < settings.getChunkSize(); ++y)
	{
		for (int x = 0; x < settings.getChunkSize(); ++x)
		{
			float value = EvaluateHeight(
				std::make_pair(static_cast<float>(x), static_cast<float>(y)),
				octaveOffsets,
				maskStartingIndex,
				maskEndingIndex
			);
			map[x][y] = mask[x][y] = value;
		}
	}

	//creating rest of octaves
	for (int y = 0; y < settings.getChunkSize(); ++y)
	{
		for (int x = 0; x < settings.getChunkSize(); ++x)
		{
			float value = EvaluateHeight(
				std::make_pair(static_cast<float>(x), static_cast<float>(y)),
				octaveOffsets,
				maskEndingIndex,
				octaveOffsets.size(),
				mask[x][y]);

			map[x][y] += value;
		}
	}

	//wait for access to resources
	return map;
}

std::pair<float, float> GenerationMethodBase::EvaluateSamplePoint(const std::pair<float, float>& point, const std::pair<float, float>& octaveOffset, float frequency)
{
	float x = point.first;
	float y = point.second;
	std::pair<float, float> sample;
	sample.second = ((y + octaveOffset.second - settings.getChunkSize() / 2.f) / (settings.getScale() * scaleOverride)) * frequency;
	sample.first = ((x + octaveOffset.first - settings.getChunkSize() / 2.f) / (settings.getScale() * scaleOverride)) * frequency;

	return sample;
}