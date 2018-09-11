/**
 * Example-1.cpp
 * 
 * This example calculate Eigen values of a specified graph using Spectra. s
 * 
 * */

/**
 * Example-1.cpp
 * 
 * This examples read graph from a CSV files, and perform Spectral Sparsification by Effective Weight resistance.
 * 
 * */

// Copyright (C) 2018 Thanaphon Chavengsaksongkram <as12production@gmail.com>, He Sun <he.sun@ed.ac.uk>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#include <gSparse/gSparse.hpp>
#include <iostream>


int main()
{
    auto graph = gSparse::Builder::buildUnitCompleteGraph(100);
    
    gSparse::SpectralSparsifier::ERSampling sparsifier(graph);

    // Approximate the Effective Weight Resistance.
    sparsifier.SetERPolicy(gSparse::SpectralSparsifier::APPROXIMATE_ER);
    sparsifier.SetC(4.0);
    sparsifier.SetEpsilon(0.3);
    sparsifier.Compute();

    std::cout<<sparsifier.GetSparsifiedGraph()->GetEdgeCount()<<std::endl;
    // Calculate exact Effective Weight Resistance. This can be extremely slow as graph size increases. 
    sparsifier.SetERPolicy(gSparse::SpectralSparsifier::EXACT_ER);
    sparsifier.Compute();
    std::cout<<sparsifier.GetSparsifiedGraph()->GetEdgeCount()<<std::endl;
    
    return EXIT_SUCCESS;
}


