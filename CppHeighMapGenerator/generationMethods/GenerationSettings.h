class GenerationSettings
{
public:
	enum class GenerationMethodType
	{
		PerlinNoise
	};

	GenerationSettings(GenerationMethodType methodType, int octaves, float scale, float weight, float persistance, float smoothing, int chunkSize);
	~GenerationSettings();

	bool isActive() { return active; }
	void isActive(bool isActive) { this->active = isActive; }

	bool isFirstOctaveAsMask() { return useFirstOctaveAsMask; };
	void isFirstOctaveAsMask(bool useFirstOctaveAsMask) { this->useFirstOctaveAsMask = useFirstOctaveAsMask; };

	bool isFirstHeightMapAsMask() { return useFirstHeightMapsAsMask; };
	void isFirstHeightMapAsMask(bool useFirstHeightMapsAsMask) { this->useFirstHeightMapsAsMask = useFirstHeightMapsAsMask; };

	bool isInvertFirstHeightMapMask() { return invertFirstHeightMapMask; };
	void isInvertFirstHeightMapMask(bool invertFirstHeightMapMask) { this->invertFirstHeightMapMask = invertFirstHeightMapMask; };

	bool isSubtractFromMap() { return subtractFromMap; };
	void isSubtractFromMap(bool subtractFromMap) { this->subtractFromMap = subtractFromMap; };

	GenerationMethodType getMethodType() { return methodType; };
	void setGenerationMethodType(GenerationMethodType methodType) { this->methodType = methodType; };

	int		getOctaves() { return octaves; };
	void	setOctaves(int octaves) { this->octaves = octaves; };

	float	getScale() { return scale; };
	void	setScale(float scale) { this->scale = scale; };

	float	getWeight() { return weight; };
	void	setWeight(float weight) { this->weight = weight; };

	float	getPersistance() { return persistance; };
	void	setPersistance(float persistance) { this->persistance = persistance; };

	float	getSmoothing() { return smoothing; };
	void	setSmoothing(float smoothing) { this->smoothing = smoothing; };

	int		getChunkSize() { return chunkSize; };
	void	setChunkSize(int chunkSize) { this->chunkSize = chunkSize; };

private:
	bool active = true;
	bool useFirstOctaveAsMask = false;
	bool useFirstHeightMapsAsMask = false;
	bool invertFirstHeightMapMask = false;
	bool subtractFromMap = false;

	GenerationMethodType methodType = GenerationMethodType::PerlinNoise;

	int octaves = 1;
	float scale = 1.0f;
	float weight = 1.0f;
	float persistance = 1.0f;
	float smoothing = 0.5f;

	int chunkSize = 241;
};
