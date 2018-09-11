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
    auto graph = gSparse::Builder::buildUnitCompleteGraph(5);
    
    gSparse::ER::ExactER exactER;
    gSparse::ER::ApproximateER approxER; 
    gSparse::PrecisionRowMatrix er;
    exactER.CalculateER(er, graph );
    std::cout<<er<<std::endl;
    approxER.CalculateER(er, graph);
    std::cout<<er<<std::endl;
    return EXIT_SUCCESS;
}


