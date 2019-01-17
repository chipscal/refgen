#pragma once

/****************************************************************************
* Author:	Luca Calacci													*
* Company:	Universita' degli studi di Roma - Tor Vergata					*
* Email:	luca.calacci@gmail.com											*
*																			*
****************************************************************************/

#include "rgcommon.h"
#include "c_api_comm.h"

#include <xtensor/xarray.hpp>
#include <xtensor/xnorm.hpp>
#include <xtensor/xmath.hpp>
#include <xtensor/xnoalias.hpp>
#include <cmath>

#include "spsa.h"
#include "costfnc.h"


/** @brief Reference generator namespace.
*/
namespace rg {

	/** Reference Generator system.
	* It stores data and multipliers and it offers a method used to dynamically compute intermedial reference to
	* reach the target domain while avoiding collisions with others.
	*/
	template<typename R>
	class Refgen {

	private:
		costParamV2<R> params;
		R _alpha_rate1, _alpha_rate2;
		R _max_var;
		R _max_ni;
		size_t _max_iter;
		R _max_delta, _a, _A, _alpha, _c, _gamma;
	public:

		/** Reference Generator object constructor.
		* @param alpha_rate1 growth rate of the external constrain multiplier.
		* @param r1 external attractive constrain radius.
		* @param alpha_rate2 growth rate of the internal constrain multiplier.
		* @param r2 internal repulsive constrain radius.
		* @param max_ni multipliers saturation.
		* @param alpha_slow dynamic friction coefficient.
		* @param d_gauss safe distance among agents.
		* @param min_alpha_gauss minimum value for the gauss repulsive distribution of the agent respect others.
		* @param max_var maximum distance of the new reference with respect to the actual position.
		* @param max_iter SPSA number of iteration for each reference computation (use: 120).
		* @param max_delta SPSA maximal perturbation admitted (use: 0.3).
		* @param a SPSA initial step size (use: 0.4).
		* @param A SPSA stability factor (use: 1).
		* @param alpha SPSA step size decay rate (use: 0.602).
		* @param c SPSA initial perturbation coefficient (use: 0.1).
		* @param gamma SPSA perturbation coefficient decay rate (use: 0.1).
		*/
		Refgen(R alpha_rate1, R r1, R alpha_rate2, R r2, R max_ni, R alpha_slow, R d_gauss, R min_alpha_gauss, R max_var, 
			size_t max_iter = 120, R max_delta = 0.3, R a = 0.4, R A = 1, R alpha = 0.602, R c = 0.1, R gamma = 0.1) : params(){
			
			_alpha_rate1 = alpha_rate1;
			_alpha_rate2 = alpha_rate2;
			
			_max_var = max_var;

			_max_iter = max_iter;
			_max_delta = max_delta;
			_max_ni = max_ni;
			_a = a;
			_A = A;
			_alpha = alpha;
			_c = c;
			_gamma = gamma;

			params.alpha_slow = alpha_slow;
			params.ni1 = 0;
			params.ni2 = 0;
			params.r1 = r1;
			params.r2 = r2;
			params.D_gauss = d_gauss;
			params.min_alpha_gauss = min_alpha_gauss;
			
		}

		/** Reference generator destructor.
		*/
		~Refgen() {};

		/** Computes the next reference.
		* @param data pointer to proper data memory with the following properties:
		*	- rank: 2
		*	- shape: spaceSize x length
		*	- memory layout: row mayor (e.g. x1, x2, ..., xk, y1, y2, ..., yk, ...)
		*	- data meaning: [target agentActualPosition othersPosition...].
		* @param spaceSize space dimention (e.g planar -> 2)
		* @param length number of columns of the data memory (e.g. 2 + number of visible other agents).
		* @param ref a pointer to an already allocated memory of size equal to spaceSize in which store the new computed reference.
		*/
		R computeRef(R RG_IN *data, size_t spaceSize, size_t length, R RG_OUT *ref) {

			size_t refshape[2];
			refshape[0] = spaceSize;
			refshape[1] = 1u;

			auto ref_map = xtc::xarray_map_raw<R>(ref, refshape, 2u);

			size_t datashape[2];
			datashape[0] = spaceSize;
			datashape[1] = length;

			params.data_raw.data = (char *)data;
			params.data_raw.shape = datashape;
			params.data_raw.rank = 2u;

			//multiplier growth
			auto data_map = xtc::xarray_map_raw<R>(&(params.data_raw));

			auto target = xt::view(data_map, xt::all(), xt::range(0, 1)); //TODO: check view what does!
			auto actualPos = xt::view(data_map, xt::all(), xt::range(1, 2));
			auto tarRelPos = target - actualPos;
			auto targetSqDist = xt::norm_sq(tarRelPos, { 0 });

			R targetSqDist_eval = (R) targetSqDist(0);


			R cstr1SqErr = std::pow(targetSqDist_eval - params.r1*params.r1, 2);

			R cstr2_err = targetSqDist_eval - params.r2*params.r2;
			

			params.ni1 = std::min(params.ni1 + _alpha_rate1 * cstr1SqErr, _max_ni);


			if (cstr2_err > 0) {
				params.ni2 = 0;
			}
			else {
				params.ni2 = params.ni2 + _alpha_rate2 * cstr2_err * cstr2_err;
			}


			// optimization step

			xt::xarray<R> theta(std::vector<size_t>{spaceSize, 1});
			xt::noalias(theta) = actualPos * 1; //this makes a copy!

			R toRet = SPSA<R, xt::xarray<R>>(costfncV2, theta, _max_iter, _max_delta, _a, _A, _alpha, _c, _gamma, &params);

			
			auto variation = xt::norm_l2(ref_map - actualPos, { 0 });
			R variation_eval = (R) variation(0);


			if (variation_eval > _max_var) {
				R normalization = _max_var / variation_eval;
				theta = actualPos + (theta - actualPos) * normalization;

				toRet = costfnc<R>(ref_map, &params);
			}

			xtc::xarray_copy_raw(theta, ref);

			return toRet;

		}

	};
}