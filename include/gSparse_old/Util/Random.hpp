#ifndef GSPARSE_UTIL_RANDOM_HPP
#define GSPARSE_UTIL_RANDOM_HPP

#include <Eigen/Dense>   // Dense Matrix definition
#include <random>

namespace gSparse
{
	namespace Util
	{
		/* Build random projection matrix based on  The Johnson-Lindenstrauss lemma */
		template <typename ScalarType>
		inline Eigen::Matrix<ScalarType, Eigen::Dynamic, Eigen::Dynamic>
			randomProjectionMatrix(const std::size_t rows,
				const std::size_t cols,
				const ScalarType scale,
				const double tolProb = 0.5f)
		{
			Eigen::Matrix<ScalarType, Eigen::Dynamic, Eigen::Dynamic> result =
				(Eigen::Matrix<ScalarType, Eigen::Dynamic, Eigen::Dynamic>::Random(rows, cols).array() + 1.0) / 2.0;

			result = result.unaryExpr([=](ScalarType x)
			{
				return x > tolProb ? 1.0 / sqrt(scale) : -1.0 / std::sqrt(scale);
			});
			return result;
		}

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
