#pragma once
#include "generationMethods/GenerationSettings.hpp"

namespace dh::Config {
	namespace Categories {
		constexpr std::string_view map{ "Map" };
		constexpr std::string_view generationMethods{ "GenerationMethods" };
		constexpr std::string_view methodType_perlin{ "Perlin" };
		constexpr std::string_view methodType_sine{ "Sine" };
		constexpr std::string_view methodType_cosine{ "Cosine" };
		constexpr std::string_view methodType_ridged{ "Ridged" };
		constexpr std::string_view methodType_billow{ "Billow" };
	}
	namespace Map {
		constexpr std::string_view mapSize{ "uMapSize" };
		constexpr std::string_view iterations{ "uIterations" };
		constexpr std::string_view randomizeMasks{ "bRandomizeMasks" };
		constexpr std::string_view randomizeActiveLayers{ "bRandomizeActiveLayers" };
		constexpr std::string_view octavesRandomizationFactor{ "uOctavesRandomizationFactor" };
		constexpr std::string_view scaleRandomizationFactor{ "fScaleRandomizationFactor" };
		constexpr std::string_view weightRandomizationFactor{ "fWeightRandomizationFactor" };
		constexpr std::string_view persistanceRandomizationFactor{ "fPersistanceRandomizationFactor" };
		constexpr std::string_view smoothingRandomizationFactor{ "fSmoothingRandomizationFactor" };
	}
	namespace Method {
		constexpr std::string_view useFirstOctaveAsMask{ "bUseFirstOctaveAsMask" };
		constexpr std::string_view useFirstHeightMapsAsMask{ "bUseFirstHeightMapsAsMask" };
		constexpr std::string_view invertFirstHeightMapMask{ "bInvertFirstHeightMapMask" };
		constexpr std::string_view subtractFromMap{ "bSubtractFromMap" };

		constexpr std::string_view octaves{ "uOctaves" };
		constexpr std::string_view scale{ "fScale" };
		constexpr std::string_view weight{ "fWeight" };
		constexpr std::string_view persistance{ "fPersistance" };
		constexpr std::string_view smoothing{ "fSmoothing" };
	}

	struct Configuration {
		size_t mapSize = 64;
		size_t iterations = 1;
		bool randomizeMasks = false;
		bool randomizeActiveLayers = false;
		int octavesRandomizationFactor = 0;
		float scaleRandomizationFactor = 0.0f;
		float weightRandomizationFactor = 0.0f;
		float persistanceRandomizationFactor = 0.0f;
		float smoothingRandomizationFactor = 0.0f;
		std::vector<GenerationSettings> generationSettingsArr;
	};
}