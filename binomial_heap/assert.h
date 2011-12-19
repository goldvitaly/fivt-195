#ifndef ASSERT_H
#define ASSERT_H

#include <iostream>
#include <sstream>
#include <cstdlib>

#ifndef DEBUG
	#define assert(condition,message)
#else
	#define assert(condition,message) \
		if (! (condition)) \
			{ \
				std::cerr << __FILE__ << ":" << __LINE__ <<  ": " << __PRETTY_FUNCTION__ << ": " << message << std::endl << "Aborted" << std::endl; \
				exit(1); \
			}
#endif

#ifdef DEBUG
	#define DEBUG_CODE(source_code) source_code;
#else
	#define DEBUG_CODE(source_code)
#endif

#endif
