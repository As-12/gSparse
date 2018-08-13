#define VIENNACL_WITH_EIGEN
#include "../gSparse/UndirectedGraph.hpp"

#include <iostream>
#include <Eigen/Dense>
#include <Eigen/Sparse>


//#define CATCH_CONFIG_RUNNER
#include "catch.hpp"

class DummyReader : public gSparse::GraphReader
{
public:
	DummyReader() {}
	void Read(Eigen::Matrix<std::size_t, Eigen::Dynamic, Eigen::Dynamic> & Edges,
		Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic> & Weights)
	{

		Edges = Eigen::Matrix<std::size_t, Eigen::Dynamic, Eigen::Dynamic>(3, 2);
		Weights = Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic>(3, 1);
		
		Edges(0, 0) = 0; Edges(0, 1) = 1;
		Edges(1, 0) = 1; Edges(1, 1) = 2;
		Edges(2, 0) = 2; Edges(2, 1) = 3;

		Weights << 1, 2, 3;
	}
};


TEST_CASE("Unit Test: Undirected Graph: Initializations", "[gSparse]")
{
	
	Eigen::Matrix<std::size_t, Eigen::Dynamic, Eigen::Dynamic> Edges(3, 2);
	Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic> Weights(3, 1);
	Edges(0, 0) = 0; Edges(0, 1) = 1;
	Edges(1, 0) = 1; Edges(1, 1) = 2;
	Edges(2, 0) = 2; Edges(2, 1) = 3;

	Weights << 1, 2, 3;

	std::shared_ptr<gSparse::GraphReader> dummyReader(new DummyReader);
	std::shared_ptr<gSparse::Graph> testGraphOne(new gSparse::UGraph(dummyReader));
	std::shared_ptr<gSparse::Graph> testGraphTwo(new gSparse::UGraph(Edges, Weights));

	// Test unit graph
	std::shared_ptr<gSparse::Graph> testGraphThree(new gSparse::UGraph(Edges));

	REQUIRE(testGraphOne->GetEdgeCount() == 3);
	REQUIRE(testGraphOne->GetNodeCount() == 4);
	REQUIRE(testGraphOne->GetEdgeList() == testGraphTwo->GetEdgeList());
	REQUIRE(testGraphOne->GetWeightList() == testGraphTwo->GetWeightList());
	REQUIRE(testGraphOne->GetEdgeList() == testGraphThree->GetEdgeList());
	REQUIRE(testGraphThree->GetWeightList() == Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic>::Ones(3,1));
}


TEST_CASE("Unit Test: Undirected Graph: Computation", "[gSparse]")
{
	Eigen::Matrix<std::size_t, Eigen::Dynamic, Eigen::Dynamic> Edges(3, 2);
	Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic> Weights(3, 1);
	Edges(0, 0) = 0; Edges(0, 1) = 1;
	Edges(1, 0) = 1; Edges(1, 1) = 2;
	Edges(2, 0) = 2; Edges(2, 1) = 3;
	Weights << 1, 2, 3;

	std::shared_ptr<gSparse::Graph> testGraph(new gSparse::UGraph(Edges, Weights));

	Eigen::MatrixXd ValidateWeightedLaplacian(4, 4);
	ValidateWeightedLaplacian << 1, -1, 0, 0,
		-1, 3, -2, 0,
		0, -2, 5, -3,
		0, 0, -3, 3;

	REQUIRE((testGraph->GetLaplacianMatrix().toDense() - ValidateWeightedLaplacian).norm() == Approx(0.0).margin(1e-12));

	Eigen::MatrixXd ValidateOrientedIncidentMatrix(3, 4);
	ValidateOrientedIncidentMatrix <<
		1, -1, 0, 0,
		0, 1, -1, 0,
		0, 0, 1, -1;

	REQUIRE((testGraph->GetIncidentMatrix().toDense() - ValidateOrientedIncidentMatrix).norm() == Approx(0.0).margin(1e-12));

	Eigen::MatrixXd ValidateWeightMatrix(3, 3);
	ValidateWeightMatrix <<
		1, 0, 0,
		0, 2, 0,
		0, 0, 3;
	REQUIRE((testGraph->GetWeightMatrix().toDense() - ValidateWeightMatrix).norm() == Approx(0.0).margin(1e-12));
}


/*
int main(int argc, char* argv[]) {
	// global setup...

	int result = Catch::Session().run(argc, argv);

	// global clean-up...
	system("pause");
	return result;
}*/

