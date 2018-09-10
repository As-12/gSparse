#ifndef GSPARSE_INTERFACE_HPP
#define GSPARSE_INTERFACE_HPP

#include <Eigen/Dense>   // Dense Matrix definition
#include <Eigen/Sparse>  //Sparse Matrix definition
#include <cstddef> // size_t definition
#include <memory>

namespace gSparse
{
	
	template <typename ScalarType>
	class IGraph
	{
	public:
		/* Get Interface*/
		virtual const Eigen::SparseMatrix<ScalarType> & GetAdjacentMatrix() const = 0;
		virtual const Eigen::SparseMatrix<ScalarType> & GetIncidentMatrix() const = 0;
		virtual const Eigen::SparseMatrix<ScalarType> & GetDegreeMatrix() const = 0;
		virtual Eigen::SparseMatrix<ScalarType>  GetLaplacianMatrix() const = 0;
		virtual const Eigen::SparseMatrix<ScalarType> & GetWeightMatrix() const = 0;
		virtual const Eigen::Matrix<std::size_t, Eigen::Dynamic, Eigen::Dynamic> & GetEdgeList() const = 0;
		virtual const Eigen::Matrix<ScalarType, Eigen::Dynamic, Eigen::Dynamic> & GetWeightList() const = 0;
		virtual std::size_t GetEdgeCount() const = 0;
		virtual std::size_t GetNodeCount() const = 0;
		/* Destructor*/
		virtual ~IGraph() = default;
	protected:
		IGraph() = default;
	};
	using Graph = IGraph<double>;

	//TODO: Add Factory class to support multiple implementations
	//Since many sparisifer may have different type of hyper-parameters, Factory class is most appropriate.
	// Until then each sub class will need to provide their own method of setting hyper-parameters.
	template <typename ScalarType>
	class ISparsifier
	{
	public:
		virtual int Compute() = 0;
		virtual int GetInfo() const = 0;
		virtual std::shared_ptr<IGraph<ScalarType>> GetSparsifiedGraph() = 0;
		virtual ~ISparsifier() = default;
	protected:
		ISparsifier() = default;
	};
	using Sparsifier = ISparsifier<double>;

	template <typename ScalarType>
	class IGraphWriter
	{
	public:
		IGraphWriter() = default;
		virtual void Write(std::shared_ptr<IGraph<ScalarType>>) = 0;
		virtual void Write(const Eigen::Matrix<std::size_t, Eigen::Dynamic, Eigen::Dynamic> & Edges) = 0;
		virtual void Write(const Eigen::Matrix<std::size_t, Eigen::Dynamic, Eigen::Dynamic> & Edges,
			const Eigen::Matrix<ScalarType, Eigen::Dynamic, Eigen::Dynamic> & Weights) = 0;
		virtual ~IGraphWriter() = default;
	};
	using GraphWriter = std::shared_ptr<IGraphWriter<double>>;

	template <typename ScalarType>
	class IGraphReader
	{
	public:
		IGraphReader() = default;
		virtual void Read(Eigen::Matrix<std::size_t, Eigen::Dynamic, Eigen::Dynamic> & Edges,
			Eigen::Matrix<ScalarType, Eigen::Dynamic, Eigen::Dynamic> & Weights) = 0;
		virtual ~IGraphReader() = default;
	};
	using GraphReader = IGraphReader<double>;
}

#endif