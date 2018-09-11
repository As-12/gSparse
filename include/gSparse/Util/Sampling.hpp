// Copyright (C) 2018 Thanaphon Chavengsaksongkram <as12production@gmail.com>, He Sun <he.sun@ed.ac.uk>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#ifndef GSPARSE_UTIL_SAMPLING_HPP
#define GSPARSE_UTIL_SAMPLING_HPP

#include "../Config.hpp"
#include <random>

namespace gSparse
{
    namespace Util
    {
        /* 
		Sample a number based from given discrete distribution
		The pseudo random generateuses Mersenne Twister algorithm.
		*/
		inline int sampleDiscreteDistribution(std::discrete_distribution<> & distribution)
		{
			thread_local std::mt19937 engine(std::random_device{}());
			return distribution(engine);
		}


		inline double sampleUniform()
		{
			thread_local std::mt19937 engine;
			engine.seed(std::random_device()());
			std::uniform_real_distribution<> d(0, 1);
			return d(engine);
		}
    }
}

#endif