// Copyright (C) 2018 Thanaphon Chavengsaksongkram <as12production@gmail.com>, He Sun <he.sun@ed.ac.uk>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.


#ifndef GSPARSE_CONFIG_HPP
#define GSPARSE_CONFIG_HPP

#include <cstddef>
#include <Eigen/Dense>
#include <Eigen/Sparse>

namespace gSparse
{
    //! PRECISION Definition
    /*! PRECISION is a typedef of a data type used by gSparse. 
        This can be modified by CMake build option. It is default to double.
    */
    typedef double PRECISION;

    //! PrecisionMatrix Definition
    /*! PrecisionMatrix is a typedef of Eigen::Matrix<gSparse::PRECISION, -1, -1>
    *   This is to shorten the code for readability.
    */
    typedef Eigen::Matrix<gSparse::PRECISION, Eigen::Dynamic, Eigen::Dynamic> PrecisionMatrix;

    //! EdgeMatrix Definition
    /*! EdgeMatrix is a typedef of Eigen::Matrix<gSparse::PRECISION, -1, -1>
    *   This is to shorten the code for readability.
    */
    typedef Eigen::Matrix<std::size_t, Eigen::Dynamic, Eigen::Dynamic> EdgeMatrix;

    //! SparsePrecisionMatrix Definition
    /*! EdgeMatrix is a typedef of Eigen::SparseMatrix<gSparse::PRECISION>
    *   This is to shorten the code for readability.
    */
    typedef Eigen::SparseMatrix<gSparse::PRECISION> SparsePrecisionMatrix;
}

#endif