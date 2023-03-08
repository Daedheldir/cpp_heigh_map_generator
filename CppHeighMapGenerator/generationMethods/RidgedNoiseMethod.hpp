#pragma once
#include "GenerationMethodBase.h"

class RidgedNoiseMethod : public GenerationMethodBase
{
public:
	RidgedNoiseMethod(GenerationSettings settings, int seed, float scaleOverride);
	virtual ~RidgedNoiseMethod();

private:
	// Inherited via GenerationMethodBase
	virtual float EvaluateHeight(const std::pair<float, float>& point) override;

private:
};