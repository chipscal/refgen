# Reference Generator

This library aims to implement a reference generator for UAV/UTV that could be used to reach a target destination or for flock control. 
The system generates references, results of optimization, in order to reach a target or maintain a flock structure while avoiding collision
with obstacles and other agents. 
In combination with a positions broadcast system and some kind of target domain moving policy it is possible to control a group/flock of agents. 

This implementation uses the xtensor library that provides advanced mathematical tool, lazy evaluation, broadcasting of tensor array and SIMD
vectorization.


## Authors

* **Luca Calacci** - University of Rome "Tor Vergata" - luca.calacci@gmail.com - *Design and Implementation*
    
* **Giovanni de Carolis** - University of Rome "Tor Vergata" - decarolisgio@gmail.com - *Design* 

## Getting Started

These instructions will get you a copy of the project up and running on your local machine for development and testing purposes.

### Prerequisites

* **Building**
    * Compiler with C++14 support 
        * On Windows Visual C++ 2015 Update 2, or more recent
        * On Linux GCC 4.9 or higher
    * Dependencies
        * [xtensor](https://github.com/QuantStack/xtensor)
        * [xsimd](https://github.com/QuantStack/xsimd)
        * [xtl](https://github.com/QuantStack/xtl)  
    
* **C API** 
    * None (using pre-compiled library)

### Installing

* From pre-compiled binaries
    * Linux: from a terminal move to the root of the extracted package and type: sudo make install
    * Windows: Copy the release folder in a PATH reached folder (e.g. C:/Program Files/)

* Linux from source:
```
cd SOURCE_PATH
mkdir build
cd build
cmake -g "Unix Makefiles" -DARCH_SPECIFIC=arch_specific ..
make
sudo make install
```
    *Note on Linux ARMv7a (e.g. Raspberry PI 2/3):
        * arch_specific = -march=armv7-a -mfpu=neon -mfloat-abi=hard -flax-vector-conversions
* Windows
```
open proper visual studio native tool command prompt with administrative privileges
mkdir build
cd build
cmake -g "NMake Makefiles" ..
nmake 
nmake install
```

## C API Reference Generator example:
The following example 
```C
    #include <refgen/crefgen/c_api.h>


    int main(void) {

        int episode_size = 200;

        // creates a single precision reference generator
        void *refgen = new_refgen_float(0.01f, 1.414f, 1000.0f, 0.0001f, 500.0f, 6.0f, 1.5f, 30.0f, 0.3f);
        if (refgen == NULL){
            return -1;
        }

        //set agent initial point
        float x_actual = -0.3f;
        float y_actual = 0.2f;
        
        float data[8] = {   0, x_axtual, 1,  1,
                            0, y_actual, 1, -1 };  //row major {target, old_agent_pos, others...}


          
        float outRef[2];

        // Computes references
        for (int k = 0; k < episode_size; k++) {
            refgen_float_computeref(refgen, &(data[0]), 2, 4, &(outRef[0]));
            
            data[1] = outRef[0];
            data[5] = outRef[1];
        }
        
        // Destroy generator
        delete_refgen_float(refgen);
        
        return 0;
    }
```

## How to
In order to use the C API it is sufficient to install the provided package and follow the previous example. Instead, if you want to use the C++
template library or extend it you may simply use cmake to catch the crefgenConfig.cmake file. You may simply use the following:
```
cmake_minimum_required(VERSION 3.1)
project(first_example)

find_package(crefgen REQUIRED)

add_executable(first_example src/example.cpp)

target_include_directories(first_example 
							PUBLIC crefgen_INCLUDE_DIRS)

target_link_libraries(first_example crefgen_LIBRARIES)
```

Note: if for some reason cmake cannot find the file you may define crefgen_DIRS=path_to_crefgenConfig_folder.

## Built With

* [CMake](https://cmake.org/) - CMake is an open-source, cross-platform family of tools designed to build, test and package software.


## License

This project is licensed under the MIT License - see the LICENSE.md file for details

## Acknowledgments

* SPSA Algorithm https://www.jhuapl.edu/SPSA/

