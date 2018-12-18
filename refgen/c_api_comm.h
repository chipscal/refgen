#pragma once

/****************************************************************************
* Author:	Luca Calacci													*
* Company:	Universita' degli studi di Roma - Tor Vergata					*
* Email:	luca.calacci@gmail.com											*
*																			*
****************************************************************************/

#include "rgcommon.h"
#include <xtensor/xarray.hpp>
#include <xtensor/xadapt.hpp>

#include <iostream>



extern "C" {

	/** An xarray (xtensor) external C rappresentation used to exchange data from/to xtensor and C consumer.
	*/
	struct RG_API raw_xarray {

		/** Pointer to allocated memory */
		char *data = nullptr; 

		/** Pointer to shape array.
		* The dimension are stored from external to internal (e.g. in position 0 the most external dimension)
		*/
		size_t *shape = nullptr; 

		/** Number of dimension. */
		size_t rank; 
	};

	
}



/** @brief Utility functions used to make easy the C / C++ xtensor interaction.
*/
namespace xtc {

	/** Utility function to map, without a copy an external C array to an xtensor one.
	* This funtion may be used either in input (get data from outside) and output (write back a result to raw memory).
	* @param from a pointer to a raw_array structure used to provide information about the array itself.
	*/
	template<class T>
	auto xarray_map_raw(raw_xarray *from) {

		T *data = (T *)from->data;
		size_t *shape = from->shape;
		size_t rank = from->rank;

		std::size_t dim = 1;
		for (size_t k = 0; k < rank; k++) {
			dim *= shape[k];
		}

		std::vector<size_t> shapeVec(shape, shape + rank);

		auto toRet = xt::adapt(data, dim, xt::no_ownership(), shapeVec);
		return toRet;

	}

	/** Utility function to map, without a copy an external C array to an xtensor one.
	* This funtion may be used either in input (get data from outside) and output (write back a result to raw memory).
	* @param dataPtr a pointer to an allocated memory area of the right dimension.
	* @param shape an array of dimesion (from external to internal).
	* @param rank number of dimension (e.g. the size of the shape array).
	*/
	template<class T>
	auto xarray_map_raw(T *dataPtr, size_t *shape, size_t rank) {

		//T *data = (T *)dataPtr; 

		std::size_t dim = 1;
		for (size_t k = 0; k < rank; k++) {
			dim *= shape[k];
		}

		std::vector<size_t> shapeVec(shape, shape + rank);

		auto toRet = xt::adapt(dataPtr, dim, xt::no_ownership(), shapeVec);
		return toRet;

	}

	/** Copy the content of an xtensor, xarray or xexpression to an external C memory.
	* This function automatically allocate and free previous memory managed by the raw_xarray structure.
	* @param from an xarray, xtensor or xexpression from which copy data
	* @param to a pointer to a raw_xarray structure where to copy information.
	*/
	template<class T, class _Ty>
	void xarray_copy_raw(_Ty &&from, raw_xarray *to) {


		auto shape = from.shape();
		size_t rank = from.dimension();
		
		to->rank = rank;

		if (to->shape != nullptr) {
			delete[] to->shape;
		}
		to->shape = new size_t[rank];

		size_t dim = 1;
		for (size_t k = 0; k < rank; k++) {
			to->shape[k] = shape[k];
			dim *= shape[k];
		}

		if (to->data != nullptr) {
			delete[] to->data;
		}

		T *t = new T[dim];
		to->data = (char *)t;
		
		auto fromCast = xt::cast<T>(from);

		for (size_t k = 0; k < dim; k++) {
			t[k] = fromCast[k];
		}

		
	}

	/** Copy the content of an xtensor, xarray or xexpression to an preallocated external C memory.
	* This function automatically does NOT allocate or free previous memory.
	* @param from an xarray, xtensor or xexpression from which copy data
	* @param toPtr a pointer to big enough memory where to copy data.
	*/
	template<class T, class _Ty>
	void xarray_copy_raw(_Ty &&from, T *toPtr) {

		auto shape = from.shape();
		size_t rank = from.dimension();

		auto flatten = xt::flatten(from);


		size_t dim = 1;
		for (size_t k = 0; k < rank; k++) {
			dim *= shape[k];
		}

		auto fromCast = xt::cast<T>(flatten);

		for (size_t k = 0; k < dim; k++) {
			toPtr[k] = fromCast[k];
		}

	}

	/** Write the content of an xtensor, xarray or xexpression to an output stream.
	* @param stream a reference to an output stream where write to.
	* @param xarray an xarray, xtensor or xexpression to stream
	* @param pretty if true the the array is streamed with cool indentation according to its shape.
	* @return a reference to the input stream.
	*/
	template<class T>
	std::ostream & stream_xarray(std::ostream &stream, T && xarray, bool pretty = true) {

		auto flatten = xt::flatten(xarray);

		if (pretty) {
			size_t cnt = 0;

			auto shape = xarray.shape();
			size_t rank = xarray.dimension();

			std::string indent = "  ";
			size_t level;

			for (auto it = flatten.begin(); it != flatten.end(); ++it) {
				
				if (cnt % shape[rank - 1] == 0) {

					level = rank;
					size_t submatdim = shape[rank - 1];

					for (size_t k = 2; k <= rank; k++) {
						
						submatdim *= shape[rank - k];
						if (cnt % submatdim != 0) {
							break;
						}
						level--;
					}

					for (size_t l = level; l < rank; l++) {
						for (size_t i = 0; i < l - 1; i++) {
							stream << indent;
						}
						stream << "{\n";
						
					}

					for (size_t k = 0; k < rank - 1; k++) {
						stream << indent;
					}
					stream << "{";
				}

				stream << *it;

				if (cnt % shape[rank - 1] == shape[rank - 1] - 1) {

					stream << "}\n";

					level = rank;
					size_t submatdim = shape[rank - 1];

					for (size_t k = 2; k <= rank; k++) {

						submatdim *= shape[rank - k];
						if ((cnt + 1) % submatdim != 0) {
							break;
						}
						level--;
					}

					for (size_t l = rank-1; l >= level; l--) {
						
						for (size_t i = 0; i < l - 1; i++) {
							stream << indent;
						}
						stream << "}\n";

					}
				}
				else {
					stream << " ";
				}

				cnt++;
			}
		}
		else {
			stream << "(";
			for (auto it = flatten.begin(); it != flatten.end(); ++it) {
				stream << *it << " ";
			}
			stream << ")";
		}

		return stream;
	}

	/** Utility function used to stream an xarray, xtensor or an xexpression to standard output stream.
	* @param xarray an xarray, xtensor or xexpression to stream
	*@see stream_xarray
	*/
	template<class T>
	void print_xarray(T && xarray) {
		stream_xarray<T>(std::cout, xarray);
	}
	

}

