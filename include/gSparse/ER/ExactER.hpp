#ifndef GSPARSE_ER_EXACTER_HPP
#define GSPARSE_ER_EXACTER_HPP
// Adaptation from http://ccom.uprrp.edu/~ikoutis/SpectralAlgorithms.htm
#include "../Config.hpp"

#include "../Interface/EffectiveResistance.hpp"
#include <Eigen/Dense>
#include <Eigen/Sparse>

#include <iostream>
#include "Policy/ExactERJacobiCG.hpp"
namespace gSparse 
{
    namespace ER 
    {
        template <typename Policy> 
        class _ExactER : public IEffectiveResistance, private Policy
        {
        public:
            using Policy::_calculateER;
            inline gSparse::COMPUTE_INFO CalculateER( gSparse::PrecisionRowMatrix & er,
                const gSparse::Graph & graph)
            {
                return _calculateER(er, graph);
            }
        };
        // Set Policy here
        typedef _ExactER<Policy::ExactERJacobiCG> ExactER;
    }
}
#endif
