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
    
   /*gSparse::Builder::createRandomGraph(2,1,30,"dumbell-30.csv");
    gSparse::Builder::createRandomGraph(3,4,30,"3-Community-30.csv");
    gSparse::Builder::createRandomGraph(4,8,30,"4-Community-30.csv");
    gSparse::Builder::createRandomGraph(3,8,1000,"3-Community-1000.csv");
    gSparse::GraphCSVWriter writer("Complete-1000.csv","None"," ");
    gSparse::GraphCSVWriter writer2("Complete-30.csv","None"," ");
    gSparse::GraphCSVWriter writer3("Complete-2000.csv","None"," ");

    writer.Write(gSparse::Builder::buildUnitCompleteGraph(1000)->GetEdgeList());
    writer3.Write(gSparse::Builder::buildUnitCompleteGraph(2000)->GetEdgeList());
    writer2.Write(gSparse::Builder::buildUnitCompleteGraph(30)->GetEdgeList());*/

    //writer.Write(std::make_shared<gSparse::UndirectedGraph>(gSparse::Builder::buildUnitCompleteGraph(30)));
    //writer.Write(std::make_shared<gSparse::UndirectedGraph>(gSparse::Builder::buildUnitCompleteGraph(2000)));
    gSparse::GraphReader reader = std::make_shared<gSparse::GraphCSVReader>("../Dataset/dumbell-30.csv","None"," ");
    gSparse::Graph graph = std::make_shared<gSparse::UndirectedGraph>(reader);
    gSparse::SpectralSparsifier::ERSampling sparsifier(graph);

    sparsifier.SetC(100);
    sparsifier.SetEpsilon(0.75);
    std::cout<<"ER"<<std::endl;
    sparsifier.Compute();
    std::cout<<"Sparsifying"<<std::endl;
    auto result = sparsifier.GetSparsifiedGraph();

    gSparse::GraphCSVWriter writer("sparsifier-dumbell-eps75-edges.csv",
                                   "sparsifier-dumbell-eps75-weights.csv");
    writer.Write(result);
    return EXIT_SUCCESS;
    return EXIT_SUCCESS;
}


