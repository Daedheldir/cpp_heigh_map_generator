#pragma once
#include "GenerationMethodBase.hpp"

class BillowNoiseMethod : public GenerationMethodBase
{
public:
	BillowNoiseMethod(GenerationSettings settings, int seed, float scaleOverride);
	virtual ~BillowNoiseMethod();

private:
	// Inherited via GenerationMethodBase
	virtual float EvaluateHeight(const std::pair<float, float>& point) override;

private:
};