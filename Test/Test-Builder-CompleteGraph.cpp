// Copyright (C) 2018 Thanaphon Chavengsaksongkram <as12production@gmail.com>, He Sun <he.sun@ed.ac.uk>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#include <gtest/gtest.h>
#include <gSparse/Builder/CompleteGraph.hpp>

TEST(Builder, CompleteGraph)
{
	gSparse::PrecisionMatrix completeGraph(4, 4);
	completeGraph << 3, -1, -1, -1,
		-1, 3, -1, -1,
		-1, -1, 3, -1,
		-1, -1, -1, 3;
	EXPECT_EQ(completeGraph, gSparse::Builder::buildUnitCompletedGraph(4)->GetLaplacianMatrix().toDense());
}

