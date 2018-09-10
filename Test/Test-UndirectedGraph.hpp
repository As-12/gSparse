// AllTests.cpp
#include <gtest/gtest.h>
#include <gSparse/UndirectedGraph.hpp>
#include <Eigen/Dense>
#include <Eigen/Sparse>
#include <memory>
#include <string>
#include <fstream>

/*******************************************************
 * Set up and utility functions
 * ******************************************************/

class DummyReader : public gSparse::IGraphReader
{
public:
	DummyReader() {}
	void Read(gSparse::EdgeMatrix & Edges,
		gSparse::PrecisionMatrix & Weights)
	{

		Edges = gSparse::EdgeMatrix(3, 2);
		Weights = gSparse::PrecisionMatrix(3, 1);
		
		Edges(0, 0) = 0; Edges(0, 1) = 1;
		Edges(1, 0) = 1; Edges(1, 1) = 2;
		Edges(2, 0) = 2; Edges(2, 1) = 3;

		Weights << 1, 2, 3;
	}
};

/*******************************************************
 * Test Suite
 * ******************************************************/

// Test UndirectedGraph constructor works properly
TEST(UndirectedGraph, AssignmentOperator)
{
    // Generate data
    gSparse::EdgeMatrix Edges(3, 2);
	gSparse::PrecisionMatrix Weights(3, 1);
	Edges(0, 0) = 0; Edges(0, 1) = 1;
	Edges(1, 0) = 1; Edges(1, 1) = 2;
	Edges(2, 0) = 2; Edges(2, 1) = 3;
	Weights << 1, 2, 3;
    

    gSparse::EdgeMatrix Edges_2(3, 2);
	gSparse::PrecisionMatrix Weights_2(3, 1);
	Edges(0, 0) = 1; Edges(0, 1) = 1;
	Edges(1, 0) = 1; Edges(1, 1) = 1;
	Edges(2, 0) = 1; Edges(2, 1) = 1;
	Weights << 2, 2, 2;

    // Generate test objects
    gSparse::UndirectedGraph test_1(Edges, Weights);
    gSparse::UndirectedGraph test_2(Edges_2, Weights_2);

    // Test Assignment operator operator
    test_2 = test_1;
    EXPECT_EQ(test_2.GetWeightList(),test_1.GetWeightList());  
    EXPECT_EQ(test_2.GetEdgeList(),test_1.GetEdgeList());  
}
TEST(UndirectedGraph, Constructor)
{
    // Call constructors
    gSparse::EdgeMatrix Edges(3, 2);
	gSparse::PrecisionMatrix Weights(3, 1);
	Edges(0, 0) = 0; Edges(0, 1) = 1;
	Edges(1, 0) = 1; Edges(1, 1) = 2;
	Edges(2, 0) = 2; Edges(2, 1) = 3;
	Weights << 1, 2, 3;
    
    // Call constructors
    gSparse::UndirectedGraph test_1(Edges, Weights);
    gSparse::Graph test_2(new gSparse::UndirectedGraph(Edges, Weights));

    // Undirected Graph
    gSparse::UndirectedGraph test_3(*test_2);
    gSparse::UndirectedGraph test_4(test_1);

    // Test constructors
    EXPECT_EQ(test_1.GetEdgeList(),test_2->GetEdgeList());     
    EXPECT_EQ(test_3.GetEdgeList(),test_2->GetEdgeList());  
    EXPECT_EQ(test_3.GetEdgeList(),test_4.GetEdgeList());  
}

TEST(UndirectedGraph, BuildEdgeOnly)
{
    // When weight is not specified, it should defaults to one.

    // Build validation data
    gSparse::EdgeMatrix Edges(3, 2);
	gSparse::PrecisionMatrix Weights(3, 1);
	Edges(0, 0) = 0; Edges(0, 1) = 1;
	Edges(1, 0) = 1; Edges(1, 1) = 2;
	Edges(2, 0) = 2; Edges(2, 1) = 3;
    Weights_2 << 1, 1, 1;

    // Construct the graph
    gSparse::UndirectedGraph test(Edges);
    gSparse::UndirectedGraph validation(Edges,Weights_2);

    // Validates
    EXPECT_EQ(test.GetEdgeList(),validation.GetEdgeList());  
    EXPECT_EQ(test.GetWeightList(),validation.GetWeightList());  
}

// Test GraphReader object
TEST(UndirectedGraph, GraphReader)
{
    gSparse::EdgeMatrix Edges(3, 2);
	gSparse::PrecisionMatrix Weights(3, 1);
	Edges(0, 0) = 0; Edges(0, 1) = 1;
	Edges(1, 0) = 1; Edges(1, 1) = 2;
	Edges(2, 0) = 2; Edges(2, 1) = 3;
	Weights << 1, 2, 3;
	gSparse::GraphReader dummyReader(new DummyReader);
	gSparse::UndirectedGraph test_1(dummyReader);
	gSparse::UndirectedGraph test_2(Edges, Weights);
    EXPECT_EQ(test_1.GetEdgeList(),test_2.GetEdgeList()); 
    EXPECT_EQ(test_1.GetWeightList(),test_2.GetWeightList()); 
}

// Test UndirectedGraph constructor that takes in GraphReader
TEST(UndirectedGraph, GraphReader)
{
    gSparse::EdgeMatrix Edges(3, 2);
	gSparse::PrecisionMatrix Weights(3, 1);
	Edges(0, 0) = 0; Edges(0, 1) = 1;
	Edges(1, 0) = 1; Edges(1, 1) = 2;
	Edges(2, 0) = 2; Edges(2, 1) = 3;
	Weights << 1, 2, 3;
	gSparse::GraphReader dummyReader(new DummyReader);
	gSparse::Graph testGraphOne(new gSparse::UndirectedGraph(dummyReader));
	gSparse::Graph testGraphTwo(new gSparse::UndirectedGraph(Edges, Weights));
}


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


