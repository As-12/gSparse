# gSparse

**gSparse** is a C++ library for Graph Sparsification. It is built on top of [Eigen](http://eigen.tuxfamily.org),
an open source linear algebra library.

## Sparsification Algorithm(s) 

**gSparse** currently supports the following algorithm(s). 
- [Spectral Sparsification by Effective Resistance](https://arxiv.org/abs/0803.0929)

## Header-only Library
**gSparse** is implemented as a header-only C++ library, whose only dependency,
**Eigen**, is also header-only.

## License

**gSparse** is an open source project licensed under MIT License.

## Examples-1

An example that demonstrates the gSparse to sparsify a graph from CSV files

```cpp
#include <gSparse/gSparse.hpp>
#include <iostream>

int main()
{
    // Create a CSV Reader and specify the file location
    gSparse::GraphReader csvReader = std::make_shared<gSparse::GraphCSVReader>("edgeList.csv", "weightList.csv",",");
    // Create an Undirected Graph. gSparse::Graph object is a shared_ptr.
    gSparse::Graph graph = std::make_shared<gSparse::UndirectedGraph>(csvReader);

    // Display original edge count
    std::cout<<"Original Edge Count: " << graph->GetEdgeCount() << std::endl;

    // Creating Sparsifier Object
    gSparse::SpectralSparsifier::ERSampling sparsifier(graph);
    // Set Hyper-parameters
    // Approximate the Effective Weight Resistance (Faster)
    // C = 4 and Epsilon = 0.5
    sparsifier.SetERPolicy(gSparse::SpectralSparsifier::APPROXIMATE_ER);
    sparsifier.SetC(4.0);
    sparsifier.SetEpsilon(0.5);
    // Compute Effective Weight Resistance
    sparsifier.Compute();
    // Perform Sparsification 
    auto sparseGraph1 = sparsifier.GetSparsifiedGraph();
    std::cout<<"Sparised Edge Count (ApproxER): " << sparseGraph1->GetEdgeCount() << std::endl;
    return EXIT_SUCCESS;
}
```


## Examples-2

An example that shows how to create a Graph object from CSV file and Eigen Matrix

```cpp
#include <gSparse/gSparse.hpp>
#include <iostream>
#include <memory>

int main()
{
    // Create a CSV Reader and specify the file location
    gSparse::GraphReader csvReader = std::make_shared<gSparse::GraphCSVReader>("Examples/Example-1-edges.csv", "Examples/Example-1-weights.csv");
    // Create an Undirected Graph. gSparse::Graph object is a shared_ptr.
    gSparse::Graph graph = std::make_shared<gSparse::UndirectedGraph>(csvReader);

    // Display the Edge list of the graph
    std::cout<<"Edge List of Graph"<<std::endl;
    std::cout<<graph->GetEdgeList()<<std::endl;
    // Display the weight list of the graph
    std::cout<<"Weight List of Graph"<<std::endl;
    std::cout<<graph->GetWeightList()<<std::endl;

    // Creating Edge and Weight Data Manually using Eigen Matrix
    gSparse::EdgeMatrix Edges(3, 2);
	gSparse::PrecisionMatrix Weights(3, 1);
	Edges(0, 0) = 0; Edges(0, 1) = 1;
	Edges(1, 0) = 1; Edges(1, 1) = 2;
	Edges(2, 0) = 2; Edges(2, 1) = 3;
    Weights << 1, 1, 1;

    // Construct the graph based on the data generated
    gSparse::UndirectedGraph test(Edges);
    gSparse::UndirectedGraph validation(Edges,Weights);

    return EXIT_SUCCESS;
}
```


## Examples-3

An example that shows how to calculate graph effective resistance.

```cpp
#include <gSparse/gSparse.hpp>
#include <iostream>

int main()
{

    // Create a Unit Complete Graph
    auto graph = gSparse::Builder::buildUnitCompleteGraph(5);
    
    // Creating object to calculate Exact ER (slow)
    gSparse::ER::ExactER exactER;
    // Object for calculate Approximate ER (faster)
    gSparse::ER::ApproximateER approxER; 
    
    // Object to hold EffectiveResistance data
    gSparse::PrecisionRowMatrix er;
    
    // Calculate Exact ER
    exactER.CalculateER(er, graph );
    std::cout<<"Exact ER Results"<<std::endl;
    std::cout<<"----------------"<<std::endl;
    std::cout<<er<<std::endl;
    
    // Calculate Approximate ER
    approxER.CalculateER(er, graph);
    std::cout<<"Approx ER Results"<<std::endl;
    std::cout<<"----------------"<<std::endl;
    std::cout<<er<<std::endl;
    return EXIT_SUCCESS;
}
```


## Examples-4

This example shows you how to perform Spectral Sparsification 
by Effective Weight Resistance over completed graph set.

```cpp

#include <gSparse/gSparse.hpp>
#include <iostream>

int main()
{
    // Generate 100x100 Complete Graph
    auto graph = gSparse::Builder::buildUnitCompleteGraph(100);
    
    // Display original edge count
    std::cout<<"Original Edge Count: " << graph->GetEdgeCount() << std::endl;

    // Creating Sparsifier Object
    gSparse::SpectralSparsifier::ERSampling sparsifier(graph);
    // Set Hyper-parameters
    // Approximate the Effective Weight Resistance (Faster)
    // C = 4 and Epsilon = 0.5
    sparsifier.SetERPolicy(gSparse::SpectralSparsifier::APPROXIMATE_ER);
    sparsifier.SetC(4.0);
    sparsifier.SetEpsilon(0.5);
    // Compute Effective Weight Resistance
    sparsifier.Compute();
    // Perform Sparsification 
    auto sparseGraph1 = sparsifier.GetSparsifiedGraph();
    std::cout<<"Sparised Edge Count (ApproxER): " << sparseGraph1->GetEdgeCount() << std::endl;

    // Set ER Policy to Exact Effective Resistance instead of Approximation
    // This is expected to perform much slower.
    sparsifier.SetERPolicy(gSparse::SpectralSparsifier::EXACT_ER);
    sparsifier.Compute();
    auto sparseGraph2 = sparsifier.GetSparsifiedGraph();
    std::cout<<"Sparised Edge Count (ApproxER): " << sparseGraph2->GetEdgeCount() << std::endl;
    return EXIT_SUCCESS;
}
```

## Examples-5

This example perform Spectral Sparsification, and use Spectra to calcualte Eigenvalue of Graph Laplacians

```cpp
#include <gSparse/gSparse.hpp>
#include <iostream>
#include <Eigen/Core>
#include <Eigen/SparseCore>
#include <Spectra/GenEigsSolver.h>
#include <Spectra/MatOp/SparseGenMatProd.h>

Eigen::VectorXcd GetTopEigenValue(const gSparse::SparsePrecisionMatrix & M)
{
    std::cout << "Calculating Eigen Value"<<std::endl;
    using namespace Spectra;
    SparseGenMatProd<double> op(M);

    // Construct eigen solver object, requesting the largest three eigenvalues
    GenEigsSolver< double, LARGEST_MAGN, SparseGenMatProd<double> > eigs(&op, 3, 6);

    // Initialize and compute
    eigs.init();
    int nconv = eigs.compute();

    // Retrieve results
    Eigen::VectorXcd evalues;
    if(eigs.info() == SUCCESSFUL)
        evalues = eigs.eigenvalues();

    std::cout << "Eigenvalues found:\n" << evalues << std::endl;
    return evalues;
}
int main()
{
    // Generate 100x100 Complete Graph
    auto graph = gSparse::Builder::buildUnitCompleteGraph(100);
    // Creating Sparsifier Object
    gSparse::SpectralSparsifier::ERSampling sparsifier(graph);
    // Set Hyper-parameters
    // Approximate the Effective Weight Resistance (Faster)
    // C = 100 and Epsilon = 0.5
    sparsifier.SetERPolicy(gSparse::SpectralSparsifier::APPROXIMATE_ER);
    sparsifier.SetC(100.0);
    sparsifier.SetEpsilon(0.5);
    // Compute Effective Weight Resistance using ApproxER
    sparsifier.Compute();
    // Get a sparsified graph
    auto sparseGraph1 = sparsifier.GetSparsifiedGraph();
    // Set to EXACT ER
    sparsifier.SetERPolicy(gSparse::SpectralSparsifier::EXACT_ER);
    // Re-calcuate ER using ExactER
    sparsifier.Compute();
    // Get a sparsified graph
    auto sparseGraph2 = sparsifier.GetSparsifiedGraph();

    // Use Spectra to calculate top Eigen values. 
    std::cout<<"Original Eigen Value " <<std::endl;
    GetTopEigenValue(graph->GetLaplacianMatrix());
    std::cout<<"---------------------------"<<std::endl;
    std::cout<<"Top Eigen Value for ApproxER "<<std::endl;
    GetTopEigenValue(sparseGraph1->GetLaplacianMatrix());
    std::cout<<"Top Eigen Value for ApproxER - Original"<<std::endl;
    GetTopEigenValue(sparseGraph1->GetLaplacianMatrix() - graph->GetLaplacianMatrix());
    std::cout<<"---------------------------"<<std::endl;
    std::cout<<"Top Eigen Value for ExactER "<<std::endl;
    GetTopEigenValue(sparseGraph2->GetLaplacianMatrix());
    std::cout<<"Top Eigen Value for ApproxER - ExactER"<<std::endl;
    GetTopEigenValue(sparseGraph1->GetLaplacianMatrix() - graph->GetLaplacianMatrix());
    return 0;
}
```