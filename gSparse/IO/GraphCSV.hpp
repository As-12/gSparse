#ifndef GSPARSE_IO_GRAPH_CSV_HPP
#define GSPARSE_IO_GRAPH_CSV_HPP

#include <exception>  // Runtime_exception
#include <fstream>    // File IO
#include <sstream>    // Errror Message
#include <vector>     // load_csv
#include <string>     // Getline
#include <cstddef>    // size_t
#include "../Interface.hpp"  // Baseclass definitions

namespace gSparse
{
	template <typename ScalarType>
	class GraphCSVReader : public IGraphReader<typename ScalarType>
	{
	public:
		GraphCSVReader(const std::string & EdgeFileName, const std::string & WeightFileName = "None", const std::string & delimeter = ",") :
			_delim(delimeter[0]),
			_edgeFile(EdgeFileName),
			_weightFile(WeightFileName)
		{}
		virtual void Read(Eigen::Matrix<std::size_t, Eigen::Dynamic, Eigen::Dynamic> & Edges,
			Eigen::Matrix<ScalarType, Eigen::Dynamic, Eigen::Dynamic> & Weights)
		{
			Edges = load_csv<Eigen::Matrix<std::size_t, Eigen::Dynamic, Eigen::Dynamic>>(_edgeFile);
			if (_weightFile != "None")
				Weights = load_csv<Eigen::Matrix<ScalarType, Eigen::Dynamic, Eigen::Dynamic>>(_weightFile);
			else
				Weights = Eigen::Matrix<ScalarType, Eigen::Dynamic, Eigen::Dynamic>::Ones(Edges.rows(), 1);
		}
		~GraphCSVReader() = default;
	private:
		char _delim;
		std::string  _edgeFile;
		std::string  _weightFile;

		template<typename M>
		M load_csv(const std::string & path)
		{
			std::ifstream indata;
			indata.open(path);
			if (!indata.is_open())
			{
				std::stringstream ss;
				ss << "GraphCSVReader: File Not Found: " << path << std::endl;
				throw std::runtime_error(ss.str());
			}
			std::string line;
			std::vector<typename M::Scalar> values;
			std::size_t rows = 0;
			while (std::getline(indata, line))
			{
				std::stringstream lineStream(line);
				std::string cell;
				while (std::getline(lineStream, cell, _delim))
				{
					values.push_back(static_cast<typename M::Scalar>(std::stod(cell)));
				}
				++rows;
			}
			indata.close();
			return Eigen::Map<const Eigen::Matrix<typename M::Scalar, M::RowsAtCompileTime, M::ColsAtCompileTime, Eigen::RowMajor>>(values.data(), rows, values.size() / rows);
		}
	}; 
	using CSVReader = GraphCSVReader<double>;

	template <typename ScalarType>
	class GraphCSVWriter : public IGraphWriter<ScalarType>
	{
	public:
		GraphCSVWriter(const std::string & EdgeFileName, const std::string & WeightFileName = "None", std::string delimeter = ",") :
			_delim(delimeter),
			_edgeFile(EdgeFileName),
			_weightFile(WeightFileName)
		{}
		virtual void Write(std::shared_ptr<IGraph<ScalarType>> graph)
		{

			if (graph == nullptr)
			{
				throw std::invalid_argument("GraphCSVWriter: graph must not be NULL");
			}
			if (_weightFile == "None")
				throw std::invalid_argument("GraphCSVWriter: Unable to write weight without destination filename");
			
			write_csv<Eigen::Matrix<std::size_t, Eigen::Dynamic, Eigen::Dynamic>>(_edgeFile, graph->GetEdgeList());
			write_csv<Eigen::Matrix<ScalarType, Eigen::Dynamic, Eigen::Dynamic>>(_weightFile, graph->GetWeightList());
		}
		virtual void Write(const Eigen::Matrix<std::size_t, Eigen::Dynamic, Eigen::Dynamic> & Edges)
		{
			write_csv<Eigen::Matrix<std::size_t, Eigen::Dynamic, Eigen::Dynamic>>(_edgeFile, Edges);
		}
		virtual void Write(const Eigen::Matrix<std::size_t, Eigen::Dynamic, Eigen::Dynamic> & Edges,
			const Eigen::Matrix<ScalarType, Eigen::Dynamic, Eigen::Dynamic> & Weights)
		{
			if (_weightFile == "None")
				throw std::invalid_argument("GraphCSVWriter: Unable to write weight without destination filename");
			write_csv<Eigen::Matrix<std::size_t, Eigen::Dynamic, Eigen::Dynamic>>(_edgeFile, Edges);
			write_csv<Eigen::Matrix<ScalarType, Eigen::Dynamic, Eigen::Dynamic>>(_weightFile, Weights);
		}
		virtual ~GraphCSVWriter() = default;
	private:
		std::string _delim;
		std::string  _edgeFile;
		std::string  _weightFile;

		template <typename M>
		void write_csv(const std::string fileName, const M & matrix)
		{
			Eigen::IOFormat CSVFormat(Eigen::StreamPrecision, Eigen::DontAlignCols, _delim, "\n");
			std::ofstream file(fileName.c_str());
			if (!file.is_open())
			{
				std::stringstream ss;
				ss << "GraphCSVWriter: File Not Found: " << fileName << std::endl;
				throw std::runtime_error(ss.str());
			}
			file << matrix.format(CSVFormat);
			file.close();
		}
	};
	using CSVWriter = GraphCSVWriter<double>;
}
#endif
