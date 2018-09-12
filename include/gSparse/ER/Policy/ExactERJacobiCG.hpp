#ifndef GSPARSE_ER_POLICY_EXACTERJACOBICG_HPP
#define GSPARSE_ER_POLICY_EXACTERJACOBICG_HPP
// Adaptation from http://ccom.uprrp.edu/~ikoutis/SpectralAlgorithms.htm
#include "../../Config.hpp"
#include <Eigen/Dense>
#include <Eigen/Sparse>

#include <iostream>

namespace gSparse 
{
    namespace ER 
    {
        namespace Policy
        {
            class ExactERJacobiCG
            {
            protected:
                inline gSparse::COMPUTE_INFO _calculateER(
                    gSparse::PrecisionRowMatrix & er,
                    const gSparse::Graph & graph,
                    int maxIter = 300
                    )
                {
                    er = gSparse::PrecisionRowMatrix::Zero(graph->GetEdgeCount(), 1);
                    for (int i = 0; i != graph->GetEdgeCount(); ++i)
                    {
                        Eigen::ConjugateGradient<gSparse::SparsePrecisionMatrix, Eigen::Lower | Eigen::Upper  > cg;
                        cg.setMaxIterations(maxIter);
                        auto b = graph->GetIncidentMatrix().row(i);
                        cg.compute(graph->GetLaplacianMatrix());
                        Eigen::VectorXd x = cg.solve(b.transpose());
                        b.toDense();
                        er(i) = b.toDense() * x;
                    }
                    
                    return gSparse::SUCCESSFUL;
                }
            };
        }
        
    }
}
#endif
