
#ifndef GSPARSE_ER_APPROXIMATEER_HPP
#define GSPARSE_ER_APPROXIMATEER_HPP
// Adaptation from http://ccom.uprrp.edu/~ikoutis/SpectralAlgorithms.htm
#include "../Interface/EffectiveResistance.hpp"
#include "../Config.hpp"
#include "../Util/JL.hpp"  // Building Random Projection

// Approximate ER Policies
#include "Policy/AproxERSLMJacobiCG.hpp"

#include <Eigen/Dense>
#include <Eigen/Sparse>
namespace gSparse 
{
    namespace ER 
    {
        template <typename Policy> 
        class _ApproximateER : public IEffectiveResistance, private Policy
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
        typedef _ApproximateER<Policy::AproxERSLMJacobiCG> ApproximateER;
    }
}
#endif

