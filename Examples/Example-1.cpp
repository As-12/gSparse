/**
 * Example-1.cpp
 * 
 * This examples leverages gSparse to build a complete graph. Then save it to a file.
 * 
 * */

#include <gSparse/gSparse.hpp>
#include <iostream>

int main()
{
    auto graph = gSparse::Builder::buildRandomCompleteGraph(4, 1.0, 5.0);
    gSparse::GraphCSVWriter csvWriter("Examples/Example-1-edges.csv", "Examples/Example-1-weights.csv");
    csvWriter.Write(graph);

    return EXIT_SUCCESS;
}