#ifndef __DEBUG_H__
#define __DEBUG_H__

#include "IwRuntime.h"

///////////////////////////////////////////////////////////////////////////////
// identify current function name
///////////////////////////////////////////////////////////////////////////////
#if defined(_MSC_VER) && (_MSC_VER >= 1200 )
	// Microsoft visual studio, version 6 and higher.
	#define ____func __FUNCSIG__
#elif defined(__GNUC__) && (__GNUC__ >= 3 )
	// GCC version 3 and higher.
	#define ____func __PRETTY_FUNCTION__
#else
	#define ____func "<unknown>"
#endif

///////////////////////////////////////////////////////////////////////////////
// callstack identification
///////////////////////////////////////////////////////////////////////////////
#if defined(IW_DEBUG) 
	#define IW_CALLSTACK_SELF IW_CALLSTACK(____func)
#else	
	#define IW_CALLSTACK_SELF
#endif

#endif
