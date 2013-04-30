//--------------------------------------------------------------------------
// Implements the automatic screen rotation (introduced because of bada platform)
// Thanks to: https://www.airplaysdk.com/node/1638
//--------------------------------------------------------------------------

#ifndef _AUTOROTATE_H_ 
#define _AUTOROTATE_H_ 

#include "IwGeom.h"

class AutoRotate {
public:
    AutoRotate();     
    ~AutoRotate();     

	CIwSVec2 RotateCoords(const CIwSVec2& pos);

private:
    void Update();     
    static int UpdateCallback(void* systemData, void* userData);
};

#endif
