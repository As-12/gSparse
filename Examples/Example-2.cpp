/**
 * Example-1.cpp
 * 
 * This examples read graph from a CSV files, and calcualte exact and approximate effective weight resistance. 
 * 
 * */

#include <gSparse/gSparse.hpp>
#include <iostream>

int main()
{

    gSparse::GraphReader csvReader = std::make_shared<gSparse::GraphCSVReader>("Examples/Example-1-edges.csv", "Examples/Example-1-weights.csv");
    gSparse::UndirectedGraph graph(csvReader);
    
    std::cout<<" Edge List of Graph"<<std::endl;
    std::cout<<graph.GetEdgeList()<<std::endl;
    std::cout<<graph.GetWeightList()<<std::endl;


    gSparse::EdgeMatrix Edges(3, 2);
	gSparse::PrecisionMatrix Weights(3, 1);
	Edges(0, 0) = 0; Edges(0, 1) = 1;
	Edges(1, 0) = 1; Edges(1, 1) = 2;
	Edges(2, 0) = 2; Edges(2, 1) = 3;
    Weights << 1, 1, 1;

    // Construct the graph
    gSparse::UndirectedGraph test(Edges);
    gSparse::UndirectedGraph validation(Edges,Weights);

    return EXIT_SUCCESS;
}