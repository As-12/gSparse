#ifndef GSPARSE_UTIL_BUILDER_HPP
#define GSPARSE_UTIL_BUILDER_HPP

#include "../UndirectedGraph.hpp"

namespace gSparse
{
	namespace Util
	{
		inline std::shared_ptr<gSparse::Graph> buildUnitCompletedGraph(std::size_t n)
		{
			Eigen::Matrix<std::size_t, Eigen::Dynamic, Eigen::Dynamic> resultEdge = Eigen::Matrix<std::size_t, Eigen::Dynamic, Eigen::Dynamic>((n * (n - 1)) / 2, 2);
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
			return std::make_shared<gSparse::UGraph>(resultEdge);
		}
	}
}

#endif
