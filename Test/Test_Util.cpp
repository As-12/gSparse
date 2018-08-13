#include "catch.hpp"
#include "../gSparse/Util/Random.hpp"

TEST_CASE("Unit Test: Random Untilities", "[gSparse]")
{
	// Just make sure that they run without crashing.
	// TODO: Find a way to test these random generators
	std::discrete_distribution<int> distribution{ 1,2,3 };
	REQUIRE_NOTHROW(gSparse::Util::sampleDiscreteDistribution(distribution));
	REQUIRE_NOTHROW(gSparse::Util::sampleUniform());

	REQUIRE_NOTHROW(gSparse::Util::randomProjectionMatrix<double>(10, 10, 1));
}

