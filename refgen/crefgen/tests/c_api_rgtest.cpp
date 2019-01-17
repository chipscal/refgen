/****************************************************************************
* Author:	Luca Calacci													*
* Company:	Universita' degli studi di Roma - Tor Vergata					*
* Email:	luca.calacci@gmail.com											*
*																			*
****************************************************************************/


#include "crefgen/c_api.h"

#include <cmath>
#include <chrono>
#include <iostream>



int main(void) {


	int num_episodes = 100;
	int episode_size = 200;

	for (int m = 0; m < num_episodes; m++) {
		void *refgen = new_refgen_float(0.01f, 1.414f, 1000.0f, 0.0001f, 500.0f, 6.0f, 1.5f, 30.0f, 0.3f);

		float magnitude = 20;
		float x = magnitude * ( static_cast <float> (rand()) / static_cast <float> (RAND_MAX) - 0.5f);
		float y = magnitude * ( static_cast <float> (rand()) / static_cast <float> (RAND_MAX) - 0.5f);

		std::cout << "start ref: " << x << ", " << y << std::endl;

		float *data = new float[8]{ 0, x, 1,  1,
									0, y, 1, -1 };  //row major {target, old_agent_pos, others...}


		float *outRef = new float[2];

		double tavg;


		for (int k = 0; k < episode_size; k++) {
			auto t1 = std::chrono::high_resolution_clock::now();
			refgen_float_computeref(refgen, data, 2, 4, outRef);
			auto t2 = std::chrono::high_resolution_clock::now();
			auto time_span = std::chrono::duration_cast<std::chrono::duration<double>>(t2 - t1);

			if (k == 0) {
				tavg = time_span.count();
			}
			else
			{
				tavg += 1 / k * (time_span.count() - tavg);
			}

			if (k == episode_size - 1)
				std::cout << "new ref: " << outRef[0] << ", " << outRef[1] << " avg time: " << tavg << std::endl;
			data[1] = outRef[0];
			data[5] = outRef[1];
		}

		delete[] data;
		delete[] outRef;

		delete_refgen_float(refgen);
	}
	int a;
	std::cin >> a;
}