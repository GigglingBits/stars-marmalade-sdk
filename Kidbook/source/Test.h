//--------------------------------------------------------------------------
// Automated test
//--------------------------------------------------------------------------

#ifndef _TEST_H_ 
#define _TEST_H_ 

#include <string>
#include "iw2d.h"

#define DBG_FILE NULL //"memdbg.txt"

class Test {
public:
	static void RunTest();

private:
	static void RunStringTest();
	static void RunJpegFileTest();
	static void RunPageTest();
	static void RunPageMapTest();
	static void RunSpectatorTest();
	//static void RunAssetTest();

private:
	static void Evaluate(uint32 systemcheckpoint, uint32 testcheckpoint);
	static bool CheckFileExists(std::string filename);
};

#endif
