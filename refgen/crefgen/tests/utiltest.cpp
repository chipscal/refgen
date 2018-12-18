/****************************************************************************
* Author:	Luca Calacci													*
* Company:	Universita' degli studi di Roma - Tor Vergata					*
* Email:	luca.calacci@gmail.com											*
*																			*
****************************************************************************/

#define XTENSOR_USE_XSIMD
#include <chrono>

#include "rgcommon.h"
#include "c_api_comm.h"
#include "crefgen/xtfunc.h"

#include <xtensor/xarray.hpp>
#include <xtensor/xtensor.hpp>
#include <xtensor/xrandom.hpp>
#include <xtensor/xnoalias.hpp>

#include "xtensor/xstrided_view.hpp"

#include <xtensor/xvectorize.hpp>

#include "xsimd/xsimd.hpp"

#include <iostream>

//
template<class E1, class E2, class R>
void func1(E1 &&a, E2 &&b, R &&res) {

	xt::noalias(res) = a + b + 2.0f;
	//res = a + b;
}

float prov(float a, float b) {
	return a + b;
}

int main() {


	raw_xarray v;
	
	size_t sh[4];
	sh[0] = 2;
	sh[1] = 1;
	sh[2] =	2;
	sh[3] =	45;
	v.shape = sh;
	v.rank = 4;


	size_t size = sh[0];
	for (size_t k = 1; k < v.rank; k++) {
		size *= sh[k];
	}
	float *mem = new float[size];
	v.data = (char *)mem;

	auto remapped = xtc::xarray_map_raw<float>(&v);


	

	xtc::ones<float>(&v);
	xtc::stream_xarray(std::cout, remapped);
	std::cout << std::endl;


	xtc::zeros<float>(&v);
	xtc::stream_xarray(std::cout, remapped);
	std::cout << std::endl;


	xtc::eye<float>(&v);
	xtc::stream_xarray(std::cout, remapped);
	std::cout << std::endl;


	std::array<size_t, 3> shape = { 3, 2, 4 };
	xt::xtensor<double, 3> ciao(shape);


	ciao = xt::ones<double>(shape) * 1.2;

	xtc::stream_xarray(std::cout, ciao);

	raw_xarray ciao_raw;
	xtc::xarray_copy_raw<float>(ciao, &ciao_raw);

	float *p = (float *)(ciao_raw.data);
	for (int k = 0; k < 24; k++) {
		std::cout << p[k] << " ";
	}
	std::cout << std::endl;

	int *tmp = new int[24];
	xtc::xarray_copy_raw(ciao, tmp);
	for (int k = 0; k < 24; k++) {
		std::cout << tmp[k] << " ";
	}

	int a;
	std::cin >> a;
	
}