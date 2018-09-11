
#ifndef GSPARSE_ER_POLICY_APROXERSLMJACOBICG_HPP
#define GSPARSE_ER_POLICY_APROXERSLMJACOBICG_HPP
// Adaptation from http://ccom.uprrp.edu/~ikoutis/SpectralAlgorithms.htm
#include "../../Config.hpp"
#include "../../Util/JL.hpp"  // Building Random Projection

#include <Eigen/Dense>
#include <Eigen/Sparse>
namespace gSparse 
{
    namespace ER 
    {
        namespace Policy
        {
            class AproxERSLMJacobiCG
            {
            protected:
                inline gSparse::COMPUTE_INFO _calculateER(
                    gSparse::PrecisionRowMatrix & er,
                    const gSparse::Graph & graph,
                    double eps = 1.0f,
                    double JLTol = 0.5,
                    int maxIter = 300
                    )
                {
                    er = gSparse::PrecisionRowMatrix::Zero(graph->GetEdgeCount(), 1);

                    std::size_t scale = static_cast<size_t>(
                                std::ceil(
                                std::log2(
                                static_cast<double>(graph->GetIncidentMatrix().cols()) / eps)));

                    for (int i = 1; i != scale + 1; ++i)
                    {
                        Eigen::VectorXd x;
                        gSparse::PrecisionMatrix Q =
                        gSparse::Util::randomProjectionMatrix(1, 
                                                                graph->GetIncidentMatrix().rows(), 
                                                                static_cast<double>(scale), 
                                                                JLTol);

                        gSparse::PrecisionMatrix Y = (Q * graph->GetWeightMatrix().cwiseSqrt() * graph->GetIncidentMatrix());

                        // solve Linear system with 300 max iteration
                        Eigen::ConjugateGradient<gSparse::SparsePrecisionMatrix, Eigen::Lower | Eigen::Upper  > cg;
                        cg.setMaxIterations(maxIter);
                        x = cg.compute(graph->GetLaplacianMatrix()).solve(Y.transpose());
                                    
                        if (cg.info() != Eigen::Success)
                        {
                            // Does not converge this iteration. Keeps going.
                            continue;
                        }
                        for (std::size_t j = 0; j != graph->GetEdgeCount(); ++j)
                        {
                            er(j) += pow(std::abs(x(graph->GetEdgeList()(j, 0)) - x(graph->GetEdgeList()(j, 1))), 2.0f);
                        }
                    }
                    if (er.rows() != graph->GetEdgeCount())
                        return gSparse::NOT_CONVERGING;
                    return gSparse::SUCCESSFUL;       
                }
            };
        }
    }
}
#endif

