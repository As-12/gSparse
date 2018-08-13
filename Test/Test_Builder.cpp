#include "catch.hpp"
#include "../gSparse/Util/Builder.hpp"

TEST_CASE("Unit Test: Complete Graph Builder", "[gSparse]")
{
	Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic> completeGraph(4, 4);
	completeGraph << 3, -1, -1, -1,
		-1, 3, -1, -1,
		-1, -1, 3, -1,
		-1, -1, -1, 3;
	REQUIRE((completeGraph - gSparse::Util::buildUnitCompletedGraph(4)->GetLaplacianMatrix().toDense()).norm() == 0);
}

