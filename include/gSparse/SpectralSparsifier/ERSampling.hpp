#ifndef GSPARSE_SPECTRALSPARSIFIER_EFFECTIVERESISTANCESAMPLING_HPP
#define GSPARSE_SPECTRALSPARSIFIER_EFFECTIVERESISTANCESAMPLING_HPP

#include "../Config.hpp"
#include "../Interface/Sparsifier.hpp"
#include "../Util/Sampling.hpp"

// ER Policies
#include "../ER/ApproximateER.hpp"
#include "../ER/ExactER.hpp"


#include <random>            // distributions
#include <vector>            // Vector
#include <unordered_map>     // Hashtable
namespace gSparse
{
    namespace SpectralSparsifier 
    {
        enum ER_METHODS
        {
            APPROXIMATE_ER = 0,
            EXACT_ER = 1
        };
        //! A class which performs Spectral Sparsification by Effective Resistance sampling
        /*!
        Spectral Sparsifier aims to preserve the eigen values of the graph. This class
        approximate effective weight resistance of the graph, then generate a O(m log m) sparsifier
        with probability porportional to the effective weight resistance. 
        m represents the edge of the densed graph.

        Original paper <https://arxiv.org/pdf/0803.0929.pdf>

        */
        class ERSampling : public ISparsifier
        {
        protected:
            double _c;                                  //!< C-hyper parameter.
            double _eps;                                //!< epsilon hyper-parameter.
            gSparse::COMPUTE_INFO _computeInfo;         //!< Status of sparsifier
            gSparse::Graph _graph;                      //!< Graph to Sparsify
            gSparse::PrecisionRowMatrix _er;            //!< Effective Resistance
            gSparse::EffectiveResistance _erCalculator;     //!< EffectiveResistance Calculation Policy
                          
            gSparse::SpectralSparsifier::ER_METHODS _erPolicy;
        public:

            //! Write graph data to a CSV file specified in the constructor
            /*!
            \param Edges: An edge list to be written to a CSV file
            \param Weights: A weight list to be written to a CSV file
            */
            ERSampling(const gSparse::Graph & graph, 
            double C = 4.0f, 
            double Epsilon = 0.3f,
            gSparse::SpectralSparsifier::ER_METHODS ERPolicy = gSparse::SpectralSparsifier::APPROXIMATE_ER
            )
            {
                SetC(C);
                SetEpsilon(Epsilon);
                _graph = graph;
                _computeInfo = gSparse::NOT_COMPUTED;
                SetERPolicy(ERPolicy);
            }
            virtual inline gSparse::COMPUTE_INFO Compute()
            {
                //Calculate Effective Resistance
                _computeInfo = _erCalculator->CalculateER(_er, _graph);
                return _computeInfo;
            }
            virtual inline gSparse::Graph GetSparsifiedGraph()
            {
                if (_computeInfo == gSparse::NOT_COMPUTED)
                {
                    throw std::logic_error("SpectralSparsifier by ER: User must run Compute before GetSparsifiedGraph()");
                }
                // Build probability distribution
                std::vector<double> samplingWeights;
                samplingWeights.reserve(_er.rows());

                //Generate sampling weights (p) for each edges
                for (std::size_t i = 0; i != _er.rows(); ++i)
                {
                    double temp = _er(i, 0) * _graph->GetWeightList()(i) * _c * std::log(_graph->GetNodeCount()) / std::pow(_eps, 2);
                    double prob = 1.0f < temp ? 1.0f : temp;
                    samplingWeights.push_back(prob);
                }

                // Build a discrete distribution based on sampling weight
                std::discrete_distribution<> samplingDistribution(samplingWeights.begin(), samplingWeights.end());

                // The algorithm samples O(n log n / ep^2) times edges
                std::size_t samplingCount = static_cast<std::size_t>(
                    std::ceil(_graph->GetNodeCount() * std::log(_graph->GetNodeCount()) / std::pow(_eps, 2)));
                // Sample edges from a distribution, then add to a hash table
                std::unordered_map<std::size_t, gSparse::PRECISION> sparsifiedMap;
                for (std::size_t i = 0; i != samplingCount; ++i)
                {
                    
                    std::size_t edgeIndex = gSparse::Util::sample(samplingDistribution);
                    sparsifiedMap[edgeIndex] += _graph->GetWeightList()(edgeIndex) / samplingWeights[edgeIndex];
                }
                
                // Build Graph object from sparsified information
                gSparse::EdgeMatrix resultEdge(sparsifiedMap.size(), 2);
                gSparse::PrecisionRowMatrix resultWeight(sparsifiedMap.size(), 1);
                std::size_t row = 0;
                for (auto x = sparsifiedMap.begin(); x != sparsifiedMap.end(); ++x)
                {
                    resultEdge(row, 0) = _graph->GetEdgeList()(x->first, 0);
                    resultEdge(row, 1) = _graph->GetEdgeList()(x->first, 1);
                    resultWeight(row, 0) = x->second;
                    ++row;
                }
                //return nullptr;
                return std::make_shared<gSparse::UndirectedGraph>(resultEdge, resultWeight);
            }
        public:
            inline void SetERPolicy(ER_METHODS policy)
            {
                assert (policy == gSparse::SpectralSparsifier::APPROXIMATE_ER || 
                        policy == gSparse::SpectralSparsifier::EXACT_ER);
                
                switch (policy)
                {
                case gSparse::SpectralSparsifier::EXACT_ER:
                    _erCalculator = std::make_shared<gSparse::ER::ExactER>(); 
                    _erPolicy = gSparse::SpectralSparsifier::EXACT_ER;
                 break;
                case gSparse::SpectralSparsifier::APPROXIMATE_ER:
                default:
                    _erCalculator = std::make_shared<gSparse::ER::ApproximateER>();
                    _erPolicy = gSparse::SpectralSparsifier::APPROXIMATE_ER;
                }
            }
            /* set and get */
            inline void SetC(double C)
            {
                #ifndef NDEBUG
                    assert(C > 0.0f);
                #endif
                _c = C;
            }
            inline void SetEpsilon(double Epsilon)
            {
                #ifndef NDEBUG
                    assert(Epsilon > 0.0f);
                #endif
                _eps = Epsilon;
            }
            inline double GetC() const { return _c; }
            inline double GetEpsilon() const { return _eps; }
            inline gSparse::SpectralSparsifier::ER_METHODS GetERPolicy() const { return _erPolicy; }
            inline gSparse::COMPUTE_INFO GetInfo() const { return _computeInfo; }
            inline const gSparse::PrecisionRowMatrix & GetEffectiveResistance() const
            {
                return _er;
            }
        };

    }
}
#endif