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
#include <xtensor/xnorm.hpp>
#include <xtensor/xnoalias.hpp>
#include "c_api_comm.h"



namespace rg {

	/** Simultaneous Perturbation Stochastic Approximation algorithm implementation.
	* An efficient and white noise robust optimization alghorihm.
	* @param loss function pointer to a loss function:
	*	- return: R
	*	- args: xtensor expression or container
	*	- args: pointer to other data useful.
	* @param theta an xarray or xtensor container reference used to pass intial hint to the solver and to get back the optimized solution.
	* @param max_iter SPSA number of iteration for each reference computation (use: 120).
	* @param max_delta SPSA maximal perturbation admitted (use: 0.3).
	* @param a SPSA initial step size (use: 0.4).
	* @param A SPSA stability factor (use: 1).
	* @param alpha SPSA step size decay rate (use: 0.602).
	* @param c SPSA initial perturbation coefficient (use: 0.1).
	* @param gamma SPSA perturbation coefficient decay rate (use: 0.1).
	* @param params a pointer to other parameters used from the loss function.
	* @see https://www.jhuapl.edu/SPSA/
	*/
	template<class R, class E, class _Ey>
	R SPSA(R (*loss)(E &&, void *), _Ey && RG_INOUT theta, size_t max_iter, R max_delta, R a, R A, R alpha, R c, R gamma, void *params = nullptr) {
		
		size_t size = theta.size();

		xt::xarray<R> thetaInternal(std::vector<size_t>{size, 1});
		//std::array<size_t, 2> shape = { size, 1 };
		//xt::xtensor<R, 2> thetaInternal(shape);   <--- wrong result on PI (????)
		thetaInternal = xt::eval(theta);

		//xt::xtensor<R, 2> delta(shape);

		R ak = a;
		R ck = c;


		for (size_t k = 1; k <= max_iter; k++) {

			ak = a / std::pow(k + A, alpha);
			ck = c / std::pow(k, gamma);

			auto perturbation = xt::random::rand<R>(thetaInternal.shape());

			xt::xarray<R> delta = 2 * xt::round(perturbation) - 1;
			//delta = 2 * xt::round(perturbation) - 1;

			auto thetaplus = thetaInternal + ck * delta;
			auto thetaminus = thetaInternal - ck * delta;

			R yplus = loss(thetaplus, params);
			R yminus = loss(thetaminus, params); 
			//std::cout << "plus " << yplus << " minus" << yminus << std::endl;

			auto ghat = (yplus - yminus) / (2 * ck * delta);

			auto varNorm = xt::norm_l2(ghat, { 0 });
			R varNorm_eval = (R)varNorm(0);

			R normalization = 1;

			if (varNorm_eval > max_delta) {
				normalization = max_delta / varNorm_eval;
			}
			
			thetaInternal += - ak * ghat * normalization;

			//std::cout << "x " << thetaInternal(0, 0) << " y" << thetaInternal(1, 0) << "shape " << thetaInternal.shape()[0] << " " << thetaInternal.shape()[1] << std::endl;

		}
		xt::noalias(theta) = xt::eval(thetaInternal);


		return loss(std::forward<E>(thetaInternal), params);
	}
}