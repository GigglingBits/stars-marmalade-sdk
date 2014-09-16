#ifndef __TYPECONVERTER_H__
#define __TYPECONVERTER_H__

#include "IwDebug.h"
#include "Debug.h"

#include <limits>

class TypeConverter {
public:
	template <class TFloat, class TInt>
	static TInt SafeFloatToInt(const TFloat &num) {
		// check if float fits into integer
		if (std::numeric_limits<TInt>::digits < std::numeric_limits<TFloat>::digits) {
			if (num > static_cast<TFloat>(std::numeric_limits<TInt>::max())) {
				IwAssertMsg(MYAPP, false, ("Unsafe conversion of value: %f. Value is too large.", num));
				return std::numeric_limits<TInt>::max();
			} else if (num < static_cast<TFloat>(std::numeric_limits<TInt>::min())) {
				IwAssertMsg(MYAPP, false, ("Unsafe conversion of value: %f. Value is too small.", num));
				return std::numeric_limits<TInt>::min();
			} else {
				return static_cast<TInt>(num); // it is safe to cast
			}
		} else {
			// it is safe to cast
			return static_cast<TInt>(num);
		}
	}

	template <class TFloat>
	static int SafeFloatToInt(const TFloat &num) {
		return SafeFloatToInt<TFloat, int>(num);
	}
};

#endif