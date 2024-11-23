#include "RandomGenerate.h"
std::mt19937 RandomGenerate::gen = std::mt19937(std::random_device()());
std::uniform_real_distribution<float> RandomGenerate::dis = std::uniform_real_distribution<float>(0.0, 1.0);