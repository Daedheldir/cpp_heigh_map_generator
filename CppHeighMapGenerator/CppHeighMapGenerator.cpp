// CppHeighMapGenerator.cpp : Defines the entry point for the application.
//

#include "CppHeighMapGenerator.h"
#include "generationMethods/PerlinNoiseMethod.h"

int main()
{
	GenerationSettings settings(
		GenerationSettings::GenerationMethodType::PerlinNoise,
		1,
		1.0f,
		1.0f,
		0.5f,
		0.5f,
		5
	);

	GenerationMethodBase* generationBasePointer;
	generationBasePointer = new PerlinNoiseMethod(settings, 0, 1.0f);

	std::vector<std::vector<float>> map = generationBasePointer->CreateHeightMap(std::pair<float, float>(0.0f, 0.0f));

	for (auto& row : map) {
		for (auto& item : row) {
			std::cout << item << ' ';
		}
		std::cout << std::endl;
	}
	return 0;
}