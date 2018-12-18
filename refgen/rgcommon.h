#pragma once

/****************************************************************************
* Author:	Luca Calacci													*	
* Company:	Universita' degli studi di Roma - Tor Vergata					*
* Email:	luca.calacci@gmail.com											*
*																			*
****************************************************************************/


#define RG_IN
#define RG_OUT
#define RG_INOUT

#ifdef RG_EXPORTS
	#ifdef __GNUC__
		#define RG_API __attribute__((visibility("default")))
	#else
		#define RG_API __declspec(dllexport)
	#endif
#else
	#ifdef __GNUC__
		#define RG_API __attribute__((visibility("default")))
	#else
		#define RG_API __declspec(dllimport)
	#endif
#endif

#ifdef __GNUC__
#define __stdcall __attribute__((stdcall))
#endif



#ifdef WIN32 
#define THROW_EXCPT(msg) throw std::exception(msg);
#else	
#define THROW_EXCPT(msg) throw std::runtime_error(msg);
#endif


