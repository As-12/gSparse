#ifndef GSPARSE_SPECTRAL_SPARSIFIER_HPP
#define GSPARSE_SPECTRAL_SPARSIFIER_HPP
#include <Eigen/Dense>
#include <Eigen/Sparse>
#include <memory>
#include "Interface.hpp"
#include "UndirectedGraph.hpp"
#include "Util/Random.hpp"
#include "Util/CompInfo.hpp"

namespace gSparse
{
	/* default spectral sparsifier */
	template <typename ScalarType>
	class SpeilmanSparsifier : public ISparsifier<ScalarType>
	{
	protected:
		double _c;
		double _eps;
		int _computeInfo;
		std::shared_ptr<IGraph<ScalarType>> _graph;
		Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor> _er;
	public:
		SpeilmanSparsifier(std::shared_ptr<IGraph<ScalarType>> graph, const double C = 4.0f, const double Epsilon = 0.3f)
		{
			if (C <= 0.0f || Epsilon <= 0.0f)
			{
				throw std::invalid_argument("SpeilmanSparsifier: hyper-parameters cannot be non-positive");
			}
			_c = C;
			_eps = Epsilon;
			_graph = graph;
			_computeInfo = gSparse::NOT_COMPUTED;
		}
		virtual int Compute()
		{
			//Calculate Effective Resistance
			_approximateER();
			return _computeInfo;
		}
		virtual int Compute(double solver_eps, double JLTol)
		{
			//Calculate Effective Resistance
			_approximateER(solver_eps, JLTol);
			return _computeInfo;
		}
		virtual std::shared_ptr<IGraph<ScalarType>> GetSparsifiedGraph()
		{
			if (_computeInfo == gSparse::NOT_COMPUTED)
			{
				throw std::logic_error("SpectralSparsifier: User must run Compute before GetSparsifiedGraph()");
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
			std::size_t samplingCount = static_cast<std::size_t>(std::ceil(_graph->GetNodeCount() * std::log(_graph->GetNodeCount()) / std::pow(_eps, 2)));

			// Sample edges from a distribution, then add to a hash table
			std::map<std::size_t, ScalarType> sparsifiedMap;
			for (std::size_t i = 0; i != samplingCount; ++i)
			{
				std::size_t edgeIndex = gSparse::Util::sampleDiscreteDistribution(samplingDistribution);
				sparsifiedMap[edgeIndex] += _graph->GetWeightList()(edgeIndex) / samplingWeights[edgeIndex];
			}

			// Build Graph object from sparsified information
			Eigen::Matrix<std::size_t, Eigen::Dynamic, Eigen::Dynamic> resultEdge(sparsifiedMap.size(), 2);
			Eigen::Matrix<ScalarType, Eigen::Dynamic, Eigen::Dynamic> resultWeight(sparsifiedMap.size(), 1);
			std::size_t row = 0;
			for (auto const& x : sparsifiedMap)
			{
				resultEdge(row, 0) = _graph->GetEdgeList()(x.first, 0);
				resultEdge(row, 1) = _graph->GetEdgeList()(x.first, 1);
				resultWeight(row, 0) = x.second;
				++row;
			}
			return std::make_shared<gSparse::UndirectedGraph<ScalarType>>(resultEdge, resultWeight);
		}
	protected:

		void _approximateER(double solver_eps = 1.0f, double JLTol = 0.5f)
		{
			_er = Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor>::Zero(_graph->GetEdgeCount(), 1);

			std::size_t scale = static_cast<size_t>(std::ceil(std::log2(static_cast<double>(_graph->GetIncidentMatrix().cols()) / solver_eps)));

			//TODO: Parallize this section and avoid data race conditions
			//#ifndef GSPARSE_OPENMP
			for (int i = 1; i != scale + 1; ++i)
			{
				Eigen::VectorXd x;
				Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic> Q =
					gSparse::Util::randomProjectionMatrix<ScalarType>(1, _graph->GetIncidentMatrix().rows(), static_cast<double>(scale), JLTol);
				Eigen::Matrix<ScalarType, Eigen::Dynamic, Eigen::Dynamic> Y = (Q * _graph->GetWeightMatrix().cwiseSqrt() * _graph->GetIncidentMatrix());
				//_solveLinearSystem(_graph->GetLaplacianMatrix(), x, Y.transpose());

				Eigen::ConjugateGradient<Eigen::SparseMatrix<ScalarType>, Eigen::Lower | Eigen::Upper  > cg;
				cg.setMaxIterations(300);
				x = cg.compute(_graph->GetLaplacianMatrix()).solve(Y.transpose());
				//std::cout << "CG Iterations: " << cg.iterations() << std::endl;
				
				if (cg.info() != Eigen::Success)
				{
					_computeInfo = gSparse::NOT_CONVERGING;
					break;
				}
				for (std::size_t j = 0; j != _graph->GetEdgeCount(); ++j)
				{
					_er(j) += pow(std::abs(x(_graph->GetEdgeList()(j, 0)) - x(_graph->GetEdgeList()(j, 1))), 2.0f);
				}
			}
			//#else
			//#endif
			_computeInfo = gSparse::SUCCESSFUL;
		}
	public:
		/* set and get */
		inline void SetC(double C)
		{
			if (C <= 0.0f)
			{
				throw std::invalid_argument("SpeilmanSparsifier: hyper-parameters cannot be non-positive");
			}
			_c = C;
		}
		inline void SetEpsilon(double Epsilon)
		{
			if (Epsilon <= 0.0f)
			{
				throw std::invalid_argument("SpeilmanSparsifier: hyper-parameters cannot be non-positive");
			}
			_eps = Epsilon;
		}
		inline double GetC() const { return _c; }
		inline double GetEpsilon() const { return _eps; }
		inline int GetInfo() const { return _computeInfo; }
		const Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor> & GetEffectiveResistance() const
		{
			return _er;
		}
	};
	using SpectralSparsifier = SpeilmanSparsifier<double>;

	template <typename ScalarType>
	class SpectralSparsifier_SampleEveryEdges : public SpeilmanSparsifier<ScalarType>
	{
	public:
		SpectralSparsifier_SampleEveryEdges(std::shared_ptr<IGraph<ScalarType>> graph, const double C = 4.0f, const double Epsilon = 0.3f)
			:SpectralSparsifier(graph, C, Epsilon)
		{}
		virtual std::shared_ptr<IGraph<ScalarType>> GetSparsifiedGraph()
		{
			if (SpectralSparsifier::_er.rows() == 0)
			{
				throw std::logic_error("SpectralSparsifier: User must run Fit before Sparsify()");
			}
			// Build probability distribution
			std::vector<double> samplingWeights;
			samplingWeights.reserve(SpectralSparsifier::_er.rows());

			std::map<std::size_t, ScalarType> sparsifiedMap;
			//Generate sampling weights (p)
			for (std::size_t i = 0; i != SpectralSparsifier::_er.rows(); ++i)
			{
				double temp = SpectralSparsifier::_er(i, 0) * SpectralSparsifier::_graph->GetWeightList()(i) * SpectralSparsifier::_c * std::log(SpectralSparsifier::_graph->GetNodeCount()) / std::pow(SpectralSparsifier::_eps, 2);
				double prob = 1.0f < temp ? 1.0f : temp;
				if (gSparse::Util::sampleUniform() < prob)
				{
					sparsifiedMap[i] += SpectralSparsifier::_graph->GetWeightList()(i) / prob;
				}
			}
			Eigen::Matrix<std::size_t, Eigen::Dynamic, Eigen::Dynamic> resultEdge(sparsifiedMap.size(), 2);
			Eigen::Matrix<ScalarType, Eigen::Dynamic, Eigen::Dynamic> resultWeight(sparsifiedMap.size(), 1);
			int row = 0;
			for (auto const& x : sparsifiedMap)
			{
				resultEdge(row, 0) = SpectralSparsifier::_graph->GetEdgeList()(x.first, 0);
				resultEdge(row, 1) = SpectralSparsifier::_graph->GetEdgeList()(x.first, 1);
				resultWeight(row, 0) = x.second;
				++row;
			}
			return std::make_shared<gSparse::UndirectedGraph<ScalarType>>(resultEdge, resultWeight);
		}
	};
}


#endif
