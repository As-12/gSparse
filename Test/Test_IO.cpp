#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <memory>

#include <Eigen/Dense>
#include "../gSparse/IO/GraphCSV.hpp"

#include "catch.hpp"

//This function compares whether two files are the same
bool compareFiles(const std::string& p1, const std::string& p2)
{
	std::ifstream f1(p1, std::ifstream::binary | std::ifstream::ate);
	std::ifstream f2(p2, std::ifstream::binary | std::ifstream::ate);
	if (f1.fail() || f2.fail())
	{
		return false; //file problem
	}
	//seek back to beginning and use std::equal to compare contents
	f1.seekg(0, std::ifstream::beg);
	f2.seekg(0, std::ifstream::beg);
	return std::equal(std::istreambuf_iterator<char>(f1.rdbuf()),
		std::istreambuf_iterator<char>(),
		std::istreambuf_iterator<char>(f2.rdbuf()));
}


/* Dummy graph for integration testing */
class DummyGraph : public gSparse::IGraph<double>
{
private:
	Eigen::SparseMatrix<double> dummy;
	Eigen::Matrix<std::size_t, Eigen::Dynamic, Eigen::Dynamic> dummyEdge;
	Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic> dummyWeight;
public:
	DummyGraph()
	{
		dummyEdge = Eigen::Matrix<std::size_t, Eigen::Dynamic, Eigen::Dynamic>(3, 2);
		dummyWeight = Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic>(3, 1);

		dummyEdge(0, 0) = 1; dummyEdge(0, 1) = 2;
		dummyEdge(1, 0) = 2; dummyEdge(1, 1) = 3;
		dummyEdge(2, 0) = 3; dummyEdge(2, 1) = 1;
		dummyWeight << 1, 2, 3;
	}
	/* Get Interface*/
	const Eigen::SparseMatrix<double> & GetAdjacentMatrix() const
	{
		return dummy;
	}
	const Eigen::SparseMatrix<double> & GetIncidentMatrix() const
	{
		return dummy;
	}
	const Eigen::SparseMatrix<double> & GetDegreeMatrix() const 
	{
		return dummy;
	}
	Eigen::SparseMatrix<double>  GetLaplacianMatrix() const 
	{
		return dummy;
	}
	const Eigen::SparseMatrix<double> & GetWeightMatrix() const
	{
		return dummy;
	}
	const Eigen::Matrix<std::size_t, Eigen::Dynamic, Eigen::Dynamic> & GetEdgeList() const 
	{
		return dummyEdge;;
	}
	const Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic> & GetWeightList() const
	{
		return dummyWeight;
	}
	std::size_t GetEdgeCount() const {
		return 0;
	}
	std::size_t GetNodeCount() const {
		return 0;
	}
};


TEST_CASE("Unit Test: GraphCSVReader - Reading Data", "[gSparse]")
{
	/* Testing Initialization */
	gSparse::CSVReader csvReaderDouble("Test/test-edges.csv", "Test/test-weight.csv", " ");
	gSparse::GraphCSVReader<double> & csvReader = csvReaderDouble;
	
	//Loading Test Data
	Eigen::Matrix<std::size_t, Eigen::Dynamic, Eigen::Dynamic>  Edges;
	Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic>  Weight;
	csvReader.Read(Edges, Weight);
	
	//Building edges validation data
	Eigen::Matrix<std::size_t, Eigen::Dynamic, Eigen::Dynamic>  Edges_Validate(3,2);
	Edges_Validate(0, 0) = 1; Edges_Validate(0, 1) = 2;
	Edges_Validate(1, 0) = 2; Edges_Validate(1, 1) = 3;
	Edges_Validate(2, 0) = 3; Edges_Validate(2, 1) = 1;

	//Building weight validation data
	Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic>  Weight_Validate(3,1);
	Weight_Validate << 1, 2, 3; 

	REQUIRE((Weight - Weight_Validate).norm() == Approx(0.0).margin(1e-12));
	REQUIRE((Edges - Edges_Validate).norm() == Approx(0.0).margin(1e-12));

	/* Test fail file opening */
	gSparse::CSVReader csvReaderFail("ThisFileDoesNotExist.txt", "ThisFileDoesNotExist.txt", " ");
	REQUIRE_THROWS(csvReaderFail.Read(Edges, Weight));

}

TEST_CASE("Unit Test: GraphCSVWriter - Writing Data", "[gSparse]")
{
	/* Testing Initialization */

	gSparse::CSVWriter csvWriterDouble("Test/csvwriter-edges.csv", "Test/csvwriter-weight.csv", " ");
	gSparse::GraphCSVWriter<double> & csvWriter = csvWriterDouble;

	//Building Validation data
	Eigen::Matrix<std::size_t, Eigen::Dynamic, Eigen::Dynamic>  Edges(3, 2);
	Edges(0, 0) = 1; Edges(0, 1) = 2;
	Edges(1, 0) = 2; Edges(1, 1) = 3;
	Edges(2, 0) = 3; Edges(2, 1) = 1;

	//Building weight validation data
	Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic>  Weight(3, 1);
	Weight << 1, 2, 3;

	csvWriter.Write(Edges, Weight);

	REQUIRE(compareFiles("Test/csvwriter-edges.csv", "Test/test-edges.csv"));
	REQUIRE(compareFiles("Test/csvwriter-weight.csv", "Test/test-weight.csv"));

	/* Write file based on data from graph */
	std::shared_ptr<gSparse::Graph> dummy(new DummyGraph());
	csvWriter.Write(dummy);
	REQUIRE(compareFiles("Test/csvwriter-edges.csv", "Test/test-edges.csv"));
	REQUIRE(compareFiles("Test/csvwriter-weight.csv", "Test/test-weight.csv"));

	/* Test fail file when weight file was not specified, but write one anyway */
	gSparse::CSVWriter csvWriterFail("Test/test-write-something.txt", " ");
	REQUIRE_THROWS(csvWriterFail.Write(Edges, Weight));
}

