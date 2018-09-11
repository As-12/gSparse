/**
 * Example-1.cpp
 * 
 * This example calculate Eigen values of a specified graph using Spectra. s
 * 
 * */

// Copyright (C) 2018 Thanaphon Chavengsaksongkram <as12production@gmail.com>, He Sun <he.sun@ed.ac.uk>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#include <gSparse/gSparse.hpp>
#include <iostream>
#include <gSparse/Builder/CommunityGraph.hpp>

int main()
{
    
    gSparse::GraphReader reader = std::make_shared<gSparse::GraphCSVReader>("../Dataset/dumbell-30.csv","None"," ");
    gSparse::Graph graph = std::make_shared<gSparse::UndirectedGraph>(reader);
    gSparse::SpectralSparsifier::ERSampling sparsifier(graph);

    sparsifier.SetC(100);
    sparsifier.SetEpsilon(0.75);
    
    sparsifier.Compute();
    auto result = sparsifier.GetSparsifiedGraph();

    gSparse::GraphCSVWriter writer("sparsifier-dumbell-30-eps75.csv");
    writer.Write(result);
    return EXIT_SUCCESS;
}


