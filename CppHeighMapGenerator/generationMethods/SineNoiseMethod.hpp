#pragma once
#include "GenerationMethodBase.h"

class SineNoiseMethod : public GenerationMethodBase
{
public:
	SineNoiseMethod(GenerationSettings generationSettings, int seed, float scaleOverride);
	virtual ~SineNoiseMethod();

	// Inherited via GenerationMethodBase
	virtual float EvaluateHeight(const std::pair<float, float>& point) override;

private:
};