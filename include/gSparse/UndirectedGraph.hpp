// Copyright (C) 2018 Thanaphon Chavengsaksongkram <as12production@gmail.com>, He Sun <he.sun@ed.ac.uk>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#ifndef GSPARSE_UNDIRECTEDGRAPH_HPP
#define GSPARSE_UNDIRECTEDGRAPH_HPP

#include <Eigen/Sparse>  
#include <Eigen/Dense>

#include <exception>  // For exception handling
#include <sstream>    // For exception messages
#include <string>     
#include <vector>
#include <map>
#include <cstddef>   // size_t definition
#include <cmath>     
#include <memory>    // Shared_ptr

#include "Config.hpp"
#include "Interface/Graph.hpp"

/* TODO:
    // Support move operator. Review Eigen documentation.
*/


namespace gSparse
{
    //! An Undirected Graph class
    /*!
        This class provides a multiple representation of an Undirected, Simple graph. 
    */
	class UndirectedGraph : public IGraph
	{
	public:
		//! A copy constructor
		UndirectedGraph(const UndirectedGraph & graph):    
        {
            // Call assignment operator
            *this = graph; 
        }
        //! An assignment operator to copy every elements of the class
		UndirectedGraph & operator=(const UndirectedGraph & rhs)
        {
            if (this != &rhs)
            {            
                // Perform expensive copy on these matrix
                this->_adjMatrix = graph._adjMatrix;
                this->_degMatrix = graph._degMatrix;
                this->_incidentMatrix = graph._incidentMatrix;
                this->_weightMatrix = graph._weightMatrix;
                this->_laplacianMatrix = graph._laplacianMatrix;
                this->_edges = graph._edges;
                this->_weight = graph._weights;
                this->_edgeCount = graph._edgeCount;
                this->_nodeCount = graph._nodeCount;
            }
            return *this;
        }

		//! A constructor to initialize graph based on GraphReader
        /*!
        \param DataReader: A subclass of IGraphReader which provides an interface to read external data.
        */
		UndirectedGraph(const gSparse::GraphReader & DataReader)
		{
			if (DataReader == nullptr)
			{
				throw std::invalid_argument("UndirectedGraph: DataReader must not be NULL");
			}
			DataReader->Read(_edges, _weights);

			//Initialize the graph system. 
			_initializeSystem();
		}

		//! A constructor to initialize graph based from Edge data. Weight sets to one.
        /*!
        \param Edges: An Eigen Matrix containing Edge List.
        */
		UndirectedGraph(const gSparse::EdgeMatrix & Edges) :_edges(Edges)
		{
			_weights = gSparse::PrecisionMatrix::Ones(Edges.rows(), 1);
			_initializeSystem();
		}
        //! A constructor to initialize graph based from Edge data. Weight sets to one.
        /*!
        \param Edges: An Eigen Matrix containing Edge List.
        \param Weights: An Eigen Matrix containing associated Weights.
        */
		UndirectedGraph(const gSparse::EdgeMatrix & Edges,
			const gSparse::PrecisionMatrix & Weights) :
			_edges(Edges),
			_weights(Weights)
		{
			_initializeSystem();
		}
        //! Return Graph's Adjancency Matrix
		virtual inline const gSparse::SparsePrecisionMatrix & GetAdjacentMatrix() const { return _adjMatrix; }
		//! Return Graph's Incident Matrix
        virtual inline const gSparse::SparsePrecisionMatrix & GetIncidentMatrix() const { return _incidentMatrix; }
		//! Return Graph's Degree Matrix
        virtual inline const gSparse::SparsePrecisionMatrix & GetDegreeMatrix() const { return _degMatrix; }
		//! Return Graph's Weight Matrix
        virtual inline const gSparse::SparsePrecisionMatrix & GetWeightMatrix() const { return _weightMatrix; }
		//! Return Graph's Laplacian Matrix
        virtual inline const gSparse::SparsePrecisionMatrix &  GetLaplacianMatrix() const { return _laplacianMatrix; }
		//! Return Graph's Edge List Matrix
        virtual inline const gSparse::EdgeMatrix & GetEdgeList() const { return _edges; }
		//! Return Graph's Weight List Matrix
        virtual inline const gSparse::PrecisionMatrix & GetWeightList() const { return _weights; }
        //! Return the number of Edges in the Graph
		virtual inline std::size_t GetEdgeCount() const { return _edgeCount; }
		//! Return the number of Nodes in the Graph
        virtual inline std::size_t GetNodeCount() const { return _nodeCount; }
        // A destructor
		virtual ~UndirectedGraph() = default;
	protected:
		gSparse::SparsePrecisionMatrix _adjMatrix;        //!< adjacency matrix representation
		gSparse::SparsePrecisionMatrix _degMatrix;        //!< degree matrix representation
		gSparse::SparsePrecisionMatrix _incidentMatrix;   //!< incident matrix representation
		gSparse::SparsePrecisionMatrix _weightMatrix;     //!< weight matrix representation
        gSparse::SparsePrecisionMatrix _laplacianMatrix;  //!< laplacian matrix representation

		gSparse::EdgeMatrix _edges;                        //!< edge list
		gSparse::PrecisionMatrix _weights;                 //!< weight list

		std::size_t _edgeCount;                            //!< count of edges
		std::size_t _nodeCount;                            //!< number of vertices
	private:
        //! Private function to perform validate and build graph representations
		virtual inline void _initializeSystem()
		{
			_validateInput();
			_initializeMatrixSystem();
		}
        //! validate preconditions
		void inline _validateInput()
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
		//! Private function to create graph representation from edge and weight list
		void inline _initializeMatrixSystem()
		{
			//Calculate counts
			_nodeCount = static_cast<std::size_t>(std::max(_edges.leftCols(1).maxCoeff(), _edges.rightCols(1).maxCoeff()) + 1);
			_edgeCount = _edges.rows();

			// Building Sparse Symmetric Adjacency Metric
			std::vector<Eigen::Triplet<gSparse::PRECISION>> adjacentList;
			std::vector<Eigen::Triplet<gSparse::PRECISION>> incidentList;
			std::vector<Eigen::Triplet<gSparse::PRECISION>> weightList;
			adjacentList.reserve(_edgeCount * 2);
			incidentList.reserve(_edgeCount * 2);
			weightList.reserve(_edgeCount);
			Eigen::VectorXd degVector = Eigen::VectorXd::Zero(_nodeCount);
            
			for (std::size_t i = 0; i != _edgeCount; ++i)
			{
				std::size_t r = static_cast<std::size_t>(_edges(i, 0));
				std::size_t c = static_cast<std::size_t>(_edges(i, 1));

				//adjacent matrix
				adjacentList.push_back(Eigen::Triplet<gSparse::PRECISION>(r, c, _weights(i, 0)));
				adjacentList.push_back(Eigen::Triplet<gSparse::PRECISION>(c, r, _weights(i, 0)));

				//degree matrix
				degVector(r) += _weights(i, 0);
				degVector(c) += _weights(i, 0);

				//incident matrix
				if (r != c)
				{
					incidentList.push_back(Eigen::Triplet<gSparse::PRECISION>(i, r, 1));
					incidentList.push_back(Eigen::Triplet<gSparse::PRECISION>(i, c, -1));
				}
				//Weight matrix
				weightList.push_back(Eigen::Triplet<gSparse::PRECISION>(i, i, _weights(i)));
			}

			// Create adj matrix
			_adjMatrix = gSparse::SparsePrecisionMatrix(_nodeCount, _nodeCount);
			_adjMatrix.setFromTriplets(adjacentList.begin(), adjacentList.end());

			//Create degree matrix
			_degMatrix = degVector.asDiagonal().toDenseMatrix().sparseView();

			//Create incident matrix
			_incidentMatrix = gSparse::SparsePrecisionMatrix(_edgeCount, _nodeCount);
			_incidentMatrix.setFromTriplets(incidentList.begin(), incidentList.end());

			//Create weight matrix
			_weightMatrix = gSparse::SparsePrecisionMatrix(_edgeCount, _edgeCount);
			_weightMatrix.setFromTriplets(weightList.begin(), weightList.end());

            //Create Laplacian matrix
            _laplacianMatrix = _degMatrix - _adjMatrix; 
		}
	};
}

#endif
