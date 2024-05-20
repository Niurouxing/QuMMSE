#pragma once
#include <chrono>
#include <cstddef>
#include <random>

struct GaussianGenerator
{

    std::mt19937 rng;
    // 标准高斯分布
    std::normal_distribution<double> distribution;

    GaussianGenerator(double mean = 0.0, double stddev = 1.0) : distribution(mean, stddev)
    {
        auto seed = 983;
        rng.seed(seed);
    }
    double operator()()
    {
        return distribution(rng);
    }
};

static GaussianGenerator gaussianGenerator;


inline double randomGaussian()
{
    return gaussianGenerator();
}

