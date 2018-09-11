// Copyright (C) 2018 Thanaphon Chavengsaksongkram <as12production@gmail.com>, He Sun <he.sun@ed.ac.uk>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.


#ifndef GSPARSE_UNDIRECTED_GRAPH_HPP
#define GSPARSE_UNDIRECTED_GRAPH_HPP


#include <Eigen/Sparse> 
#include <Eigen/Dense>

#include <exception>  // For exception handling
#include <sstream>    // For exception messages
#include <string>     
#include <vector>
#include <map>
#include <cstddef>   //size_t definition
#include <cmath>
#include <memory> // Shared_ptr
#include "Interface.hpp"

namespace gSparse
{
	template <typename ScalarType>
	class UndirectedGraph : public IGraph<ScalarType>
	{
	public:
		//C++11 default constructor and copies
		UndirectedGraph(const UndirectedGraph &) = default;
		UndirectedGraph & operator=(const UndirectedGraph &) = default;

		// Use GraphReader Object
		UndirectedGraph(std::shared_ptr<IGraphReader<ScalarType>> DataReader)
		{
			if (DataReader == nullptr)
			{
				throw std::invalid_argument("UndirectedGraph: DataReader must not be NULL");
			}
			DataReader->Read(_edges, _weights);

			//Initialize the graph system. 
			//TODO: Operation can be quite expensive. Is late initialization better?
			_initializeSystem();
		}
		// Edges only. Weight is assumed to be one.
		UndirectedGraph(const Eigen::Matrix<std::size_t, Eigen::Dynamic, Eigen::Dynamic> & Edges) :_edges(Edges)
		{
			_weights = Eigen::Matrix<ScalarType, Eigen::Dynamic, Eigen::Dynamic>::Ones(Edges.rows(), 1);
			//TODO: Operation can be quite expensive. Is late initialization better?
			_initializeSystem();
		}

		UndirectedGraph(const Eigen::Matrix<std::size_t, Eigen::Dynamic, Eigen::Dynamic> & Edges,
			const Eigen::Matrix<ScalarType, Eigen::Dynamic, Eigen::Dynamic> & Weights) :
			_edges(Edges),
			_weights(Weights)
		{
			//TODO: Operation can be quite expensive. Is late initialization better?
			_initializeSystem();
		}
		virtual inline const Eigen::SparseMatrix<ScalarType> & GetAdjacentMatrix() const { return _adjMatrix; }
		virtual inline const Eigen::SparseMatrix<ScalarType> & GetIncidentMatrix() const { return _incidentMatrix; }
		virtual inline const Eigen::SparseMatrix<ScalarType> & GetDegreeMatrix() const { return _degMatrix; }
		virtual inline const Eigen::SparseMatrix<ScalarType> & GetWeightMatrix() const { return _weightMatrix; }
		virtual inline Eigen::SparseMatrix<ScalarType>  GetLaplacianMatrix() const { return (_degMatrix - _adjMatrix); }

		virtual inline const Eigen::Matrix<std::size_t, Eigen::Dynamic, Eigen::Dynamic> & GetEdgeList() const { return _edges; }
		virtual inline const Eigen::Matrix<ScalarType, Eigen::Dynamic, Eigen::Dynamic> & GetWeightList() const { return _weights; }

		virtual inline std::size_t GetEdgeCount() const { return _edgeCount; }
		virtual inline std::size_t GetNodeCount() const { return _nodeCount; }

		virtual ~UndirectedGraph() = default;
	protected:
		Eigen::SparseMatrix<ScalarType> _adjMatrix;
		Eigen::SparseMatrix<ScalarType> _degMatrix;
		Eigen::SparseMatrix<ScalarType> _incidentMatrix;
		Eigen::SparseMatrix<ScalarType> _weightMatrix;

		Eigen::Matrix<std::size_t, Eigen::Dynamic, Eigen::Dynamic> _edges;
		Eigen::Matrix<ScalarType, Eigen::Dynamic, Eigen::Dynamic> _weights;

		std::size_t _edgeCount;
		std::size_t _nodeCount;
	private:

		virtual inline void _initializeSystem()
		{
			_validateInput();
			_initializeMatrixSystem();
		}

		void _validateInput()
		{
			//There are more edges than weights
			if (_edges.rows() != _weights.rows())
			{
				std::stringstream ss;
				ss << "UndirectedGraph: Edges.rows(): " << _edges.rows() << " =/= Weights.rows() " << _weights.rows() << std::endl;
				throw std::invalid_argument(ss.str());
			}
			if (_edges.cols() != 2)
			{
				std::stringstream ss;
				ss << "UndirectedGraph: Edges.cols(): must equal to two" << std::endl;
				throw std::invalid_argument(ss.str());
			}
			//Edge must start at zero
			if (_edges.minCoeff() != 0)
			{
				std::stringstream ss;
				ss << "UndirectedGraph: Vertices must start at zero" << std::endl;
				throw std::invalid_argument(ss.str());
			}
			//Weight must be non-zero non- start at zero
			if (_weights.minCoeff() <= 0)
			{
				std::stringstream ss;
				ss << "UndirectedGraph: Weights must be greater than zero" << std::endl;
				throw std::invalid_argument(ss.str());
			}
		}
		// This private function calculates adjacent and degree matrix from edges and data. This is used to generate a Laplacian.
		void _initializeMatrixSystem()
		{
			//Calculate counts
			_nodeCount = static_cast<std::size_t>(std::max(_edges.leftCols(1).maxCoeff(), _edges.rightCols(1).maxCoeff()) + 1);
			_edgeCount = _edges.rows();

			// Building Sparse Symmetric Adjacency Metric
			std::vector<Eigen::Triplet<ScalarType>> adjacentList;
			std::vector<Eigen::Triplet<ScalarType>> incidentList;
			std::vector<Eigen::Triplet<ScalarType>> weightList;
			adjacentList.reserve(_edgeCount * 2);
			incidentList.reserve(_edgeCount * 2);
			weightList.reserve(_edgeCount);
			Eigen::VectorXd degVector = Eigen::VectorXd::Zero(_nodeCount);

			for (std::size_t i = 0; i != _edgeCount; ++i)
			{
				std::size_t r = static_cast<std::size_t>(_edges(i, 0));
				std::size_t c = static_cast<std::size_t>(_edges(i, 1));

				//adjacent matrix
				adjacentList.push_back(Eigen::Triplet<ScalarType>(r, c, _weights(i, 0)));
				adjacentList.push_back(Eigen::Triplet<ScalarType>(c, r, _weights(i, 0)));

				//degree matrix
				degVector(r) += _weights(i, 0);
				degVector(c) += _weights(i, 0);

				//incident matrix
				if (r != c)
				{
					incidentList.push_back(Eigen::Triplet<double>(i, r, 1));
					incidentList.push_back(Eigen::Triplet<double>(i, c, -1));
				}
				//Weight matrix
				weightList.push_back(Eigen::Triplet<ScalarType>(i, i, _weights(i)));
			}

			// Create adj matrix
			_adjMatrix = Eigen::SparseMatrix<ScalarType>(_nodeCount, _nodeCount);
			_adjMatrix.setFromTriplets(adjacentList.begin(), adjacentList.end());

			//Create degree matrix
			_degMatrix = degVector.asDiagonal().toDenseMatrix().sparseView();

			//Create incident matrix
			_incidentMatrix = Eigen::SparseMatrix<ScalarType>(_edgeCount, _nodeCount);
			_incidentMatrix.setFromTriplets(incidentList.begin(), incidentList.end());

			//Create weight matrix
			_weightMatrix = Eigen::SparseMatrix<ScalarType>(_edgeCount, _edgeCount);
			_weightMatrix.setFromTriplets(weightList.begin(), weightList.end());
		}
	};
	using UGraph = UndirectedGraph<double>;
}

#endif
