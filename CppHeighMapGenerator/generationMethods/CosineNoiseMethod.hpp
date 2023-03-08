#pragma once
#include "GenerationMethodBase.h"

class CosineNoiseMethod : public GenerationMethodBase
{
public:
	CosineNoiseMethod(GenerationSettings settings, int seed, float scaleOverride);
	virtual ~CosineNoiseMethod();

private:
	// Inherited via GenerationMethodBase
	virtual float EvaluateHeight(const std::pair<float, float>& point) override;

private:
};