/****************************************************************************
* Author:	Luca Calacci													*
* Company:	Universita' degli studi di Roma - Tor Vergata					*
* Email:	luca.calacci@gmail.com											*
*																			*
****************************************************************************/


#define XTENSOR_USE_XSIMD


#include "crefgen/costfnc.h"
#include "c_api_comm.h"

#include <xtensor/xarray.hpp>
#include <xtensor/xnoalias.hpp>
#include <xtensor/xnorm.hpp>
#include <chrono>
#include <iostream>




int main(void) {
	std::cout << "ciaooooo\n";

	float *neigh = new float[8];
	size_t *shape = new size_t[2]{ 2, 4 };

	auto xneigh = xtc::xarray_map_raw<float>(neigh, shape, 2);
	std::cout << "map\n";

	//xneigh = xt::zeros<double>({ 2, 4 });
	//std::cout << "zeros\n";

	xneigh(0, 0) = 0;
	xneigh(1, 0) = 0;
	xneigh(0, 1) = -1;
	xneigh(1, 1) = -1;
	xneigh(0, 2) = 1;
	xneigh(1, 2) = 1;
	xneigh(0, 3) = 5;
	xneigh(1, 3) = 2;

	std::cout << "init\n";
	for (int k = 0; k < 8; k++) {
		std::cout << neigh[k] << " "; 
	}
	rg::costParamV2<float> ciao;
	ciao.alpha_slow = 0.1f;
	ciao.ni1 = 0;
	ciao.ni2 = 0;
	ciao.r1 = 3;
	ciao.r2 = 0.2f;
	ciao.min_alpha_gauss = 30;
	ciao.D_gauss = 2.0f;
	ciao.data_raw.data =(char *) neigh;
	ciao.data_raw.shape = shape;
	ciao.data_raw.rank = 2;
	
	float val;
	auto t1 = std::chrono::high_resolution_clock::now();
	for (int k = 0; k < 200; k++)
		val = rg::costfncV2<float>(xt::zeros<float>({ 2, 1 }), &ciao);
	auto t2 = std::chrono::high_resolution_clock::now();
	std::cout << "val: " << val << std::endl;


	auto time_span = std::chrono::duration_cast<std::chrono::duration<double>>(t2 - t1);
	std::cout << "It took me " << time_span.count() << " seconds.\n";

	
	int fine;
	std::cin >> fine;
	
}