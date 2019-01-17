#pragma once

/****************************************************************************
* Author:	Luca Calacci													*
* Company:	Universita' degli studi di Roma - Tor Vergata					*
* Email:	luca.calacci@gmail.com											*
*																			*
****************************************************************************/

#include "../rgcommon.h"

#ifdef __cplusplus
extern "C" {
#endif

	/** Allocates a single precision reference generator.
	* @param alpha_rate1 growth rate of the external constrain multiplier.
	* @param r1 external attractive constrain radius.
	* @param alpha_rate2 growth rate of the internal constrain multiplier.
	* @param r2 internal repulsive constrain radius.
	* @param max_ni multipliers saturation.
	* @param alpha_slow dynamic friction coefficient.
	* @param d_gauss safe distance among agents.
	* @param min_alpha_gauss minimum value for the gauss repulsive distribution of the agent respect others.
	* @param max_var maximum distance of the new reference with respect to the actual position.
	* @see SPSA
	* @see Refgen
	*/
	RG_API void * __stdcall new_refgen_float(float alpha_rate1, float r1, float alpha_rate2, float r2 , float max_ni, float alpha_slow,
											 float d_gauss, float min_alpha_gauss, float max_var);


	/** Allocates a single precision reference generator specifing the SPSA algorithm parameters.
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
	* @see SPSA
	* @see Refgen
	*/
	RG_API void * __stdcall new_refgen_float_ext(float alpha_rate1, float r1, float alpha_rate2, float r2, float max_ni, float alpha_slow,
									float d_gauss, float min_alpha_gauss, float max_var,
									unsigned int max_iter, float max_delta, float a, float A, float alpha, float c, float gamma);

	/** Allocates a double precision reference generator.
	* @param alpha_rate1 growth rate of the external constrain multiplier.
	* @param r1 external attractive constrain radius.
	* @param alpha_rate2 growth rate of the internal constrain multiplier.
	* @param r2 internal repulsive constrain radius.
	* @param max_ni multipliers saturation.
	* @param alpha_slow dynamic friction coefficient.
	* @param d_gauss safe distance among agents.
	* @param min_alpha_gauss minimum value for the gauss repulsive distribution of the agent respect others.
	* @param max_var maximum distance of the new reference with respect to the actual position.
	* @see SPSA
	* @see Refgen
	*/
	RG_API void * __stdcall new_refgen_double(double alpha_rate1, double r1, double alpha_rate2, double r2, double max_ni, double alpha_slow,
											  double d_gauss, double min_alpha_gauss, double max_var);

	/** Allocates a single precision reference generator specifying the SPSA algorithm parameters.
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
	* @see SPSA
	* @see Refgen
	*/
	RG_API void * __stdcall new_refgen_double_ext(double alpha_rate1, double r1, double alpha_rate2, double r2, double max_ni, double alpha_slow,
												  double d_gauss, double min_alpha_gauss, double max_var,
												  unsigned int max_iter, double max_delta, double a, double A, double alpha, double c, double gamma);

	/** Destroy a single precision reference generator.
	* @param refgen pointer to a single precision reference generator to destroy.
	* @see Refgen
	*/
	RG_API void __stdcall delete_refgen_float(void *refgen);

	/** Destroy a double precision reference generator.
	* @param refgen pointer to a double precision reference generator to destroy.
	* @see Refgen
	*/
	RG_API void __stdcall delete_refgen_double(void *refgen);

	/** Computes the next reference using a single precision reference generator.
	* @param refgen pointer to a single precision reference generator.
	* @param data float pointer to data memory with the following properties:
	*	- rank: 2
	*	- shape: spaceSize x length
	*	- memory layout: row major (e.g. x1, x2, ..., xk, y1, y2, ..., yk, ...)
	*	- data meaning: [target agentActualPosition othersPosition...].
	* @param spaceSize space dimension (e.g planar -> 2)
	* @param length number of columns of the data memory (e.g. 2 + number of visible other agents).
	* @param ref a pointer to an already allocated memory of size equal to spaceSize in which store the new computed reference.
	*/
	RG_API float __stdcall refgen_float_computeref(void *refgen, float RG_IN *data, unsigned int spaceSize, unsigned int length, float RG_OUT *ref);

	/** Computes the next reference using a double precision reference generator.
	* @param refgen pointer to a double precision reference generator.
	* @param data double pointer to data memory with the following properties:
	*	- rank: 2
	*	- shape: spaceSize x length
	*	- memory layout: row major (e.g. x1, x2, ..., xk, y1, y2, ..., yk, ...)
	*	- data meaning: [target agentActualPosition othersPosition...].
	* @param spaceSize space dimension (e.g planar -> 2)
	* @param length number of columns of the data memory (e.g. 2 + number of visible other agents).
	* @param ref a pointer to an already allocated memory of size equal to spaceSize in which store the new computed reference.
	*/
	RG_API double __stdcall refgen_double_computeref(void *refgen, double RG_IN *data, unsigned int spaceSize, unsigned int length, double RG_OUT *ref);

#ifdef __cplusplus
}
#endif