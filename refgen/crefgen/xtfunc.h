#pragma once

/****************************************************************************
* Author:	Luca Calacci													*
* Company:	Universita' degli studi di Roma - Tor Vergata					*
* Email:	luca.calacci@gmail.com											*
*																			*
****************************************************************************/

#include "rgcommon.h"

#include <vector>
#include <xtl/xsequence.hpp>
#include <xtensor/xarray.hpp>
#include <xtensor/xnoalias.hpp>
#include "c_api_comm.h"



namespace xtc {

	/** Fill a pre-allocated raw_xarray with all ones.
	* @param toSet a ponter to a raw_array structure.
	*/
	template<class T>
	void ones(raw_xarray *toSet) {

		
		auto xarr = xarray_map_raw<T>(toSet);

		auto toRetExpr = xt::ones<T>(xarr.shape());

		xt::noalias(xarr) = toRetExpr;
	}


	/** Fill a pre-allocated raw_xarray with all zeros.
	* @param toSet a ponter to a raw_array structure.
	*/
	template<class T>
	void zeros(raw_xarray *toSet) {


		auto xarr = xarray_map_raw<T>(toSet);

		auto toRetExpr = xt::zeros<T>(xarr.shape());

		xt::noalias(xarr) = toRetExpr;
	}


	/** Fill a pre-allocated raw_xarray with ones on the diagonal and zeros elsewhere.
	* @param toSet a ponter to a raw_array structure.
	*/
	template<class T>
	void eye(raw_xarray *toSet) {


		auto xarr = xarray_map_raw<T>(toSet);

		auto toRetExpr = xt::eye<T>(xarr.shape());

		xt::noalias(xarr) = toRetExpr;
	}

}
