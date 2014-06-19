//--------------------------------------------------------------------------
// Automated test
//--------------------------------------------------------------------------

#ifndef _TEST_H_ 
#define _TEST_H_ 

#include <string>

class Test {
public:
	static void RunTest();

private:
	static void RunResourceLoaderTest();
	static void RunWorldTest();
	static void RunInputControllerTest();
	static void RunFactoryConfigTest();
	static void RunVertexStreamTest();
	static void RunFactoryTest();
	static void RunSpriteTest();
	static void RunSpriteBaseTest();
	static void RunLevelTest();
	static void RunViewportTest();
	static void RunOscillatorTest();
	static void RunPinchGestureTest();
	static void RunParticleTest();

	static void Evaluate(uint32 systemcheckpoint, uint32 testcheckpoint);
};

#endif
