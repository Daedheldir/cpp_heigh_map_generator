#include <vector>
#include <algorithm>
#include <numeric>

namespace dh::Math {
    template<typename T>
    T CalculateMean(const std::vector<T>& vec) {
        const size_t size = vec.size();
        return std::accumulate(vec.begin(), vec.end(), 0.0) / size;
    }

    template<typename T>
    T CalculateVariance(const std::vector<T>& vec) {
        const size_t size = vec.size();
        if (size <= 1) 
            return 0.0;

        const T mean = CalculateMean(vec);

        auto variance_func = [&mean, &size](T accumulator, const T& val) {
            return accumulator + std::pow((val - mean),2);
        };

        return std::accumulate(vec.begin(), vec.end(), 0.0, variance_func) / size;
    }

    template<typename T>
    T CalculateStandardDeviation(const std::vector<T>& vec) {
       return std::sqrt(CalculateVariance(vec));
    }

}