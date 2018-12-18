/****************************************************************************
* Author:	Luca Calacci													*
* Company:	Universita' degli studi di Roma - Tor Vergata					*
* Email:	luca.calacci@gmail.com											*
*																			*
****************************************************************************/

#define XTENSOR_USE_XSIMD


#include "crefgen/spsa.h"
#include <xtensor/xarray.hpp>
#include <xtensor/xnoalias.hpp>
#include <xtensor/xnorm.hpp>
#include <chrono>
#include <iostream>


template<class R, class _Ey>
R myloss(_Ey &&param, void *other) {

	return std::pow(std::sin(param(0, 0)), 2) + std::pow(std::cos(param(1, 0)), 2) + std::pow(std::cos(param(2, 0)), 2) + std::pow(std::sin(param(3, 0)), 2);
}

int main(void) {

	std::vector<size_t> a_shape = { 4, 1 };
	//xt::xarray<double> a(a_shape);

	xt::xarray<double> theta(a_shape);
	theta(0, 0) = 3;
	theta(1, 0) = 1;
	theta(2, 0) = -1;
	theta(3, 0) = 1;

	auto amm = xt::norm_l2(theta);

	std::cout << "theta: " << theta(0, 0) << ", " << theta(1, 0) << ", " << theta(2, 0) << ", " << theta(3, 0) << " norm: " << amm[0] << std::endl;
	double val;
	auto t1 = std::chrono::high_resolution_clock::now();
	for (int k = 0; k < 100; k++)
	val = rg::SPSA<double, xt::xarray<double>>(myloss, theta,(size_t)200, 1, 0.4, 0.1, 0.602, 0.1, 0.1);
	auto t2 = std::chrono::high_resolution_clock::now();

	std::cout << "theta: " << theta(0, 0) << ", " << theta(1, 0) << ", " << theta(2, 0) << ", " << theta(3, 0) << " val: " << val << std::endl;
	
	auto time_span = std::chrono::duration_cast<std::chrono::duration<double>>(t2 - t1);
	std::cout << "It took me " << time_span.count() << " seconds.\n";

	int tmp;
	std::cin >> tmp;

}