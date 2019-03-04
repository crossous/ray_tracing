#pragma once
#include <random>

float rand_uniform() {
	static std::uniform_real_distribution<double> u_rand(0, 1);
	static std::default_random_engine rand_engine;

	return u_rand(rand_engine);
}