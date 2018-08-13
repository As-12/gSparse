#include "catch.hpp"

#include "../gSparse/SpectralSparsifier.hpp"
#include "../gSparse/Util/Builder.hpp"

#include <SymEigsSolver.h>
#include <MatOp/SparseGenMatProd.h>
#include <GenEigsSolver.h>


#include <iostream>

void calculateEigenValue(const Eigen::SparseMatrix<double> & matrix)
{
	// Construct matrix operation object using the wrapper class SparseGenMatProd
	Spectra::SparseGenMatProd<double> op(matrix);

	// Construct eigen solver object, requesting the largest three eigenvalues
	Spectra::GenEigsSolver< double, Spectra::LARGEST_MAGN, Spectra::SparseGenMatProd<double> > eigs(&op, 3, 6);

	eigs.init();
	int nconv = eigs.compute();

	// Retrieve results
	if (eigs.info() == Spectra::SUCCESSFUL)
	{
		const auto evalues = eigs.eigenvalues();
		std::cout.precision(std::numeric_limits<double>::digits10);
		std::cout << "Top 3 Eigenvalues:\n" << evalues << '\n';
	}

}


TEST_CASE("Unit Test: Speilman Sparsifier", "[gSparse]")
{
	std::cout << "------------------------------------------------------" << std::endl;
	std::cout << "------------------------------------------------------" << std::endl;
	std::cout << "------------------------------------------------------" << std::endl;
	std::shared_ptr<gSparse::Graph> testGraph = gSparse::Util::buildUnitCompletedGraph(1000);
	
	gSparse::SpectralSparsifier mySparsifier(testGraph, 1.5,0.5);
	mySparsifier.Compute();
	std::cout << "Testing Spectral Sparsifier on Completed graph of 1000x1000" << std::endl;
	for (int i = 0; i != 10; ++i)
	{
		std::shared_ptr<gSparse::Graph> testGraphTwo = mySparsifier.GetSparsifiedGraph();
		std::cout << "Attempt #" << i << std::endl;
		std::cout << " Top 3 Eigen Value " << std::endl;
		calculateEigenValue(testGraphTwo->GetLaplacianMatrix());

		std::cout << "Original Edges : " << testGraph->GetEdgeCount() << std::endl;
		std::cout << "Remaining Edges : " << testGraphTwo->GetEdgeCount() << std::endl;
	}
}

TEST_CASE("Unit Test: Speilman Sparsifier : All Edges", "[gSparse]")
{
	std::cout << "------------------------------------------------------" << std::endl;
	std::cout << "------------------------------------------------------" << std::endl;
	std::cout << "------------------------------------------------------" << std::endl;
	std::shared_ptr<gSparse::Graph> testGraph = gSparse::Util::buildUnitCompletedGraph(1000);

	gSparse::SpectralSparsifier_SampleEveryEdges<double> mySparsifier(testGraph, 4.0, 0.5);
	mySparsifier.Compute();
	std::cout << "Testing Spectral Sparsifier on Completed graph of 1000x1000" << std::endl;
	for (int i = 0; i != 10; ++i)
	{
		std::shared_ptr<gSparse::Graph> testGraphTwo = mySparsifier.GetSparsifiedGraph();
		std::cout << "Attempt #" << i << std::endl;
		std::cout << " Top 3 Eigen Value " << std::endl;
		calculateEigenValue(testGraphTwo->GetLaplacianMatrix());

		std::cout << "Original Edges : " << testGraph->GetEdgeCount() << std::endl;
		std::cout << "Remaining Edges : " << testGraphTwo->GetEdgeCount() << std::endl;
	}
}
