#pragma once

/****************************************************************************
* Author:	Luca Calacci													*
* Company:	Universita' degli studi di Roma - Tor Vergata					*
* Email:	luca.calacci@gmail.com											*
*																			*
****************************************************************************/

#include "rgcommon.h"

#include <vector>
#include <array>
#include <xtl/xsequence.hpp>
#include <xtensor/xarray.hpp>
#include <xtensor/xrandom.hpp>
#include <xtensor/xview.hpp>
#include <xtensor/xnorm.hpp>
#include "c_api_comm.h"


namespace rg {

	/** Support structure for the cost function used.
	*/
	template<typename R>
	struct costParam {
		R			ni1, ni2;
		R			r1, r2;
		R			alpha_slow;
		raw_xarray	data_raw;
	};

	/** Cost function used by the reference generator.
	* @param theta xtensor expression or container.
	* @param parameters pointer to other data useful.
	* @see SPSA
	* @see Refgen
	*/
	template<class R, class E>
	R costfnc(E &&theta, void *parameters) {

		costParam<R> *params = (costParam<R> *) parameters;

		auto data = xtc::xarray_map_raw<R>(&(params->data_raw));

		
		//neighborhood repulsive factor
		auto neigh = xt::view(data, xt::all(), xt::range(2, xt::placeholders::_));
		auto relPos = theta - neigh;
		auto diff = xt::norm_l2(relPos, { 0 });
		auto expDiff = xt::exp(1 / diff);

		auto neighFactor = xt::sum(expDiff, 0);

		//target actractive factor
		auto target = xt::view(data, xt::all(), xt::range(0, 1));
		auto tarRelPos = target - theta;
		auto targetSqDist = xt::norm_sq(tarRelPos, { 0 });

		auto targetFactor = params->ni1 * xt::pow(targetSqDist - params->r1*params->r1, 2) +
			params->ni2 * xt::pow(targetSqDist - params->r2*params->r2, 2);

		//dynamic friction
		auto mylastPos = xt::view(data, xt::all(), xt::range(1, 2));
		auto mySqVar = xt::norm_sq(theta - mylastPos, { 0 });


		auto minDiff = xt::amin<R>(diff, { 0 });

		auto frictionFactor = params->alpha_slow / ( 1 + minDiff ) * mySqVar; //TODO :  to add exponential reduction

		auto total = neighFactor + targetFactor + frictionFactor;

		return (R) total(0, 0);

	}

}