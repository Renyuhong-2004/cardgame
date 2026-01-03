#ifndef MATH_UTILS_H
#define MATH_UTILS_H

#include <random>
#include <vector>

// 数学工具（无状态、通用）
class MathUtils {
public:
    // 生成随机整数
    static int randomInt(int min, int max) {
        static std::random_device rd;
        static std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(min, max);
        return dis(gen);
    }

    // 洗牌（打乱数组）
    template <typename T>
    static void shuffleVector(std::vector<T>& vec) {
        std::random_shuffle(vec.begin(), vec.end());
    }
};

#endif // MATH_UTILS_H