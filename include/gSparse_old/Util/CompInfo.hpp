#ifndef GSPARSE_UTIL_COMP_INFO_HPP
#define GSPARSE_UTIL_COMP_INFO_HPP

namespace gSparse
{
	/*! Enum class representing computation information from sparsifier */
	enum COMPUTE_INFO
	{
		SUCCESSFUL = 0,    ///< Computation was successful.

		NOT_COMPUTED,      ///< Used in eigen solvers, indicating that computation
						   ///< has not been conducted. Users should call
						   ///< the `compute()` member function of solvers.

						   NOT_CONVERGING,    ///< Used in eigen solvers, indicating that some eigenvalues
											  ///< did not converge. The `compute()`
											  ///< function returns the number of converged eigenvalues.

											  NUMERICAL_ISSUE    ///
	};


}

#endif