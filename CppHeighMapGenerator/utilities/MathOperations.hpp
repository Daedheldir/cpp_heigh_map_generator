#include <vector>
#include <algorithm>
#include <numeric>

namespace dh::Math {
    template<typename T>
    T CalculateMean(const std::vector<T>& vec) {
        const size_t sz = vec.size();
        return std::accumulate(vec.begin(), vec.end(), 0.0) / sz;
    }

    template<typename T>
    T CalculateVariance(const std::vector<T>& vec) {
        const size_t sz = vec.size();
        if (sz <= 1) {
            return 0.0;
        }

        const T mean = CalculateMean(vec);

        auto variance_func = [&mean, &sz](T accumulator, const T& val) {
            return accumulator + ((val - mean) * (val - mean) / (sz - 1));
        };

        return std::accumulate(vec.begin(), vec.end(), 0.0, variance_func);
    }

    template<typename T>
    T CalculateStandardDeviation(const std::vector<T>& vec) {
        
        T sum = std::reduce(std::begin(vec), std::end(vec));
        T mean = CalculateMean(vec);

        std::vector<T> diff(vec.size());
        std::transform(vec.begin(), vec.end(), diff.begin(), [mean](T x) { return x - mean; });
        T sq_sum = std::inner_product(diff.begin(), diff.end(), diff.begin(), 0.0);
       return std::sqrt(sq_sum / vec.size());
    }
}