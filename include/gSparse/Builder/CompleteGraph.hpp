#ifndef GSPARSE_BUILDER_COMPLETEGRAPH_HPP
#define GSPARSE_BUILDER_COMPLETEGRAPH_HPP

#include "../Config.hpp"
#include "../UndirectedGraph.hpp"
#include "../Util/Sampling.hpp"

#include <random>

namespace gSparse
{
	namespace Builder
	{
		inline gSparse::Graph buildUnitCompleteGraph(std::size_t nodeCount)
		{
            std::size_t & n = nodeCount;

			gSparse::EdgeMatrix resultEdge = gSparse::EdgeMatrix((n * (n - 1)) / 2, 2);
			std::size_t row = 0;
			for (std::size_t i = 0; i != n; ++i)
			{
				for (std::size_t j = 0; j != n; ++j)
				{
					if (i < j)
					{
						resultEdge(row, 0) = i;
						resultEdge(row, 1) = j;
						++row;
					}
				}
			}
			return std::make_shared<gSparse::UndirectedGraph>(resultEdge);
		}

        inline gSparse::Graph buildRandomCompleteGraph(std::size_t nodeCount, double lower_weight, double upper_weight)
		{
            std::size_t & n = nodeCount;

            #ifndef NDEBUG
			assert (upper_weight >= lower_weight);
			#endif 
            
			gSparse::EdgeMatrix resultEdge = gSparse::EdgeMatrix((n * (n - 1)) / 2, 2);
            gSparse::PrecisionRowMatrix resultWeight = gSparse::PrecisionRowMatrix((n * (n - 1)) / 2, 1);
			std::size_t row = 0;
            std::uniform_real_distribution<double> dist(lower_weight, upper_weight);
			for (std::size_t i = 0; i != n; ++i)
			{
				for (std::size_t j = 0; j != n; ++j)
				{
					if (i < j)
					{
						resultEdge(row, 0) = i;
						resultEdge(row, 1) = j;
                        resultWeight(row) = gSparse::Util::sample(dist);
						++row;
					}
				}
			}
			return std::make_shared<gSparse::UndirectedGraph>(resultEdge,resultWeight);
		}

	}
}

#endif
