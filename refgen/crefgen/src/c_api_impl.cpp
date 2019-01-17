/****************************************************************************
* Author:	Luca Calacci													*
* Company:	Universita' degli studi di Roma - Tor Vergata					*
* Email:	luca.calacci@gmail.com											*
*																			*
****************************************************************************/

#include "rgcommon.h"
#include "crefgen/refgen.h"

#include "crefgen/c_api.h"



template<typename R>
inline void *new_refgen(R alpha_rate1, R r1, R alpha_rate2, R r2, R max_ni, R alpha_slow, R d_gauss, R min_alpha_gauss, R max_var, 
	size_t max_iter = 120, R max_delta = 0.3, R a = 0.4, R A = 1, R alpha = 0.602, R c = 0.1, R gamma = 0.1) {
	
	return new rg::Refgen<R>(alpha_rate1, r1, alpha_rate2, r2, alpha_slow, max_var, max_iter, max_delta, a, A, alpha, c, gamma);
}

template<typename R>
inline void delete_refgen(void *refgen) {
	rg::Refgen<R> *refgenR = (rg::Refgen<R> *)refgen;
	delete refgenR;
}

template<typename R>
inline R refgeg_computeref_impl(void *refgen, R RG_IN *data, size_t spaceSize, size_t length, R RG_OUT *ref) {
	rg::Refgen<R> *refgenR = (rg::Refgen<R> *)refgen;

	return refgenR->computeRef(data, spaceSize, length, ref);
}


void *new_refgen_float(float alpha_rate1, float r1, float alpha_rate2, float r2, float max_ni, float alpha_slow,
						float d_gauss, float min_alpha_gauss, float max_var) {
	return new_refgen<float>(alpha_rate1, r1, alpha_rate2, r2, max_ni, alpha_slow, d_gauss, min_alpha_gauss, max_var);
}

void *new_refgen_float_ext(float alpha_rate1, float r1, float alpha_rate2, float r2, float max_ni, float alpha_slow,
							float d_gauss, float min_alpha_gauss, float max_var,
							unsigned int max_iter, float max_delta, float a, float A, float alpha, float c, float gamma) {

	return new_refgen<float>(alpha_rate1, r1, alpha_rate2, r2, max_ni, alpha_slow, d_gauss, min_alpha_gauss, max_var,
							max_iter, max_delta, a, A, alpha, c, gamma);
}

void *new_refgen_double(double alpha_rate1, double r1, double alpha_rate2, double r2, double max_ni, double alpha_slow,
						double d_gauss, double min_alpha_gauss, double max_var) {
	return new_refgen<double>(alpha_rate1, r1, alpha_rate2, r2, max_ni, alpha_slow, d_gauss, min_alpha_gauss, max_var);
}

void *new_refgen_double_ext(double alpha_rate1, double r1, double alpha_rate2, double r2, double max_ni, double alpha_slow,
							double d_gauss, double min_alpha_gauss, double max_var,
							unsigned int max_iter, double max_delta, double a, double A, double alpha, double c, double gamma) {

	return new_refgen<double>(alpha_rate1, r1, alpha_rate2, r2, max_ni, alpha_slow, d_gauss, min_alpha_gauss, max_var,
							  max_iter, max_delta, a, A, alpha, c, gamma);
}

void delete_refgen_float(void *refgen) {
	delete_refgen<float>(refgen);
}

void delete_refgen_double(void *refgen) {
	delete_refgen<double>(refgen);
}

float refgen_float_computeref(void *refgen, float RG_IN *data, unsigned int spaceSize, unsigned int length, float RG_OUT *ref) {
	return refgeg_computeref_impl<float>(refgen, data, spaceSize, length, ref);
}

double refgen_double_computeref(void *refgen, double RG_IN *data, unsigned int spaceSize, unsigned int length, double RG_OUT *ref) {
	return refgeg_computeref_impl<double>(refgen, data, spaceSize, length, ref);
}
