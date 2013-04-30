#include "AutoRotate.h"

#include "s3eConfig.h"
#include "s3eAccelerometer.h"
#include "IwGx.h"

AutoRotate::AutoRotate() {
    s3eAccelerometerStart();             
    s3eTimerSetTimer(500, UpdateCallback, this);
}
     
AutoRotate::~AutoRotate() {
    s3eAccelerometerStop();
    s3eTimerCancelTimer(UpdateCallback, this);
}
     
void AutoRotate::Update() {       
    // Get accelerometer values
    int x = s3eAccelerometerGetX();
    int y = s3eAccelerometerGetY();
    int z = s3eAccelerometerGetZ();

    // Ignore accelerometer if we're flat
    // Ignore accelerometer if we're at a angle
    if ((3 * MAX(abs(x), abs(y)) > abs(z)) &&
        (3 * abs(x) < abs(y) || 3 * abs(y) < abs(x))) {
        // Choose orient from biggest axis
        IwGxScreenOrient orient;
        if ( abs(x) > abs(y) ) {
            if( x > 0 ) {
                orient = IW_GX_ORIENT_270;
			} else {
                orient = IW_GX_ORIENT_90;
			}
        } else {
            if( y > 0 ) {
                orient = IW_GX_ORIENT_180;
			} else {
                orient = IW_GX_ORIENT_NONE;
			}
        }
             
        if (orient != IwGxGetScreenOrient()) {
            IwGxSetScreenOrient(orient);
        }
    }
         
    s3eTimerSetTimer(1000, UpdateCallback, this);
}

CIwSVec2 AutoRotate::RotateCoords(const CIwSVec2& pos) {
    switch (IwGxGetScreenOrient()) {
    default:
    case IW_GX_ORIENT_NONE:
        return pos;
    case IW_GX_ORIENT_90:
        return CIwSVec2(pos.y, IwGxGetScreenHeight() - pos.x);
    case IW_GX_ORIENT_180:
        return CIwSVec2(IwGxGetScreenWidth() - pos.x, IwGxGetScreenHeight() - pos.y);
    case IW_GX_ORIENT_270:
        return CIwSVec2(IwGxGetScreenWidth() - pos.y, pos.x);
    }
}

int AutoRotate::UpdateCallback(void* systemData, void* userData) {
    ((AutoRotate*)userData)->Update();
    return 0;
}
