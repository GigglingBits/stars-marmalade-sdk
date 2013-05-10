
#include <string>
#include "s3e.h"

#include "Test.h"

#include "Level.h"
#include "LevelMenu.h"

#include "Debug.h"

#include "FactoryManager.h"
#include "VertexStream.h"
#include "Oscillator.h"
#include "FrameData.h"
#include "PinchGesture.h"
#include "Touchpad.h"

#define DBG_FILE NULL //"memdbg.txt"

void Test::RunTest() {
	IW_CALLSTACK_SELF;

//	IwMemBucketVisualDump("memdump_dbg_pre.html", IW_MEM_BUCKET_ID_SYSTEM_DEBUG);
//	IwMemBucketVisualDump("memdump_sys_pre.html", IW_MEM_BUCKET_ID_SYSTEM);

	IwMemBucketPush(IW_MEM_BUCKET_ID_SYSTEM);
	uint32 systemcheckpoint = IwMemBucketCheckpoint();

	IwMemBucketPush(IW_MEM_BUCKET_ID_SYSTEM_DEBUG);
	uint32 testcheckpoint = IwMemBucketCheckpoint();

	RunWorldTest();
	Evaluate(systemcheckpoint, testcheckpoint);
	s3eDeviceYield(0);

	RunInputControllerTest();
	Evaluate(systemcheckpoint, testcheckpoint);
	s3eDeviceYield(0);

	RunFactoryConfigTest();
	Evaluate(systemcheckpoint, testcheckpoint);
	s3eDeviceYield(0);

	RunVertexStreamTest();
	Evaluate(systemcheckpoint, testcheckpoint);
	s3eDeviceYield(0);

	RunViewportTest();
	Evaluate(systemcheckpoint, testcheckpoint);
	s3eDeviceYield(0);

	RunFactoryTest();
	Evaluate(systemcheckpoint, testcheckpoint);
	s3eDeviceYield(0);

	RunSpriteTest();
	Evaluate(systemcheckpoint, testcheckpoint);
	s3eDeviceYield(0);

	RunLevelTest();
	Evaluate(systemcheckpoint, testcheckpoint);
	s3eDeviceYield(0);

	RunOscillatorTest();
	Evaluate(systemcheckpoint, testcheckpoint);
	s3eDeviceYield(0);

	RunPinchGestureTest();
	Evaluate(systemcheckpoint, testcheckpoint);
	s3eDeviceYield(0);
	
	RunTouchpadTest();
	Evaluate(systemcheckpoint, testcheckpoint);
	s3eDeviceYield(0);
	
	IwMemBucketPop();
	IwMemBucketPop();
}

void Test::Evaluate(uint32 systemcheckpoint, uint32 testcheckpoint) {
	uint32 allocid;
	allocid = IwMemBucketDebugCheck(IW_MEM_BUCKET_ID_SYSTEM_DEBUG, testcheckpoint, DBG_FILE);
	if (allocid != 0) {
		IwAssertMsg(MYAPP, false, ("Test failed; memory leaked in debug bucket! Allocation ID: %i", allocid));
		IwMemBucketVisualDump("memdump_sys_post.html", IW_MEM_BUCKET_ID_SYSTEM_DEBUG);
	}
	allocid = IwMemBucketDebugCheck(IW_MEM_BUCKET_ID_SYSTEM, systemcheckpoint, DBG_FILE);
	if (allocid != 0) {
		IwAssertMsg(MYAPP, false, ("Test failed; memory leaked in system bucket! Allocation ID: %i", allocid));
		IwMemBucketVisualDump("memdump_dbg_post.html", IW_MEM_BUCKET_ID_SYSTEM);
	}
}

void Test::RunWorldTest() {
	World* p = new World();
	delete p;
}

void Test::RunInputControllerTest() {
	IW_CALLSTACK_SELF;

	// button spec
	InputManager::VirtualButton btn;
	btn.cmdid = eButtonCommandIdOpenNextLevel; // just arbitrary command
	btn.rect.Make(10, 10, 10, 10);

	// point to press
	CIwSVec2 pointinside(15, 15);
	CIwSVec2 pointoutside(15, 5);
	
	// perform hit tests
	IwAssertMsg(MYAPP, btn.cmdid == InputManager::GetInstance().HitTestButtonCommand(pointinside), ("Button registration corrupt. The button should be hit."));
	IwAssertMsg(MYAPP, eButtonCommandIdNone == InputManager::GetInstance().HitTestButtonCommand(pointoutside), ("Button registration corrupt. The button should not be hit."));
}

void Test::RunFactoryConfigTest() {
	IW_CALLSTACK_SELF;

	// empty file list
	std::list<TiXmlDocument> xmldoclist;

	// texture template
	TextureTemplate tt1;
	tt1.AddFrame("id", 12, "img", "pat", 0x00556677, 1, "next");
	TextureTemplate tt2 = tt1;
	TextureTemplate tt3(tt2);
	IwAssert(MYAPP, tt3.GetFrameCount() == 1);

	// texture factory
	TextureFactory tf;
	tf.Initialize(xmldoclist);
	IwAssert(MYAPP, !tf.ConfigExists("abc"));
	tf.SetConfig("abc", tt1);
	TextureTemplate tt4 = tf.GetConfig("abc");
	IwAssert(MYAPP, tt4.GetFrameCount() == 1);

	// shape template
	ShapeTemplate st1;
	st1.SetRectangle(2.0f, 2.0f);
	ShapeTemplate st2 = st1;
	ShapeTemplate st3(st2);
	b2PolygonShape* s = (b2PolygonShape*)st3.CreatePhysicsShapeDef();
	IwAssert(MYAPP, s->GetType() == b2Shape::e_polygon);
	IwAssert(MYAPP, s->GetVertexCount() == 4);
	delete s;

	// shape factory
	ShapeFactory sf;
	sf.Initialize(xmldoclist);
	IwAssert(MYAPP, !sf.ConfigExists("abc"));
	sf.SetConfig("abc", st1);
	ShapeTemplate st4 = sf.GetConfig("abc");
	s = (b2PolygonShape*)st4.CreatePhysicsShapeDef();
	IwAssert(MYAPP, s->GetType() == b2Shape::e_polygon);
	IwAssert(MYAPP, s->GetVertexCount() == 4);
	delete s;

	// body template
	BodyTemplate bt1;
	bt1.Set("id", "dynamic", "ball", "rubber", "none");
	BodyTemplate bt2 = bt1;
	BodyTemplate bt3(bt2);
	IwAssert(MYAPP, !bt3.GetType().compare("dynamic"));

	// body factory
	BodyFactory bf;
	bf.Initialize(xmldoclist);
	IwAssert(MYAPP, !bf.ConfigExists("abc"));
	bf.SetConfig("abc", bt1);
	BodyTemplate bt4 = bf.GetConfig("abc");
	IwAssert(MYAPP, !bt4.GetType().compare("dynamic"));

	// level template
	LevelTemplate lt1;
	lt1.SetName("woohoo");
	LevelTemplate lt2 = lt1;
	LevelTemplate lt3(lt2);
	IwAssert(MYAPP, !lt3.GetName().compare("woohoo"));

	// level factory
	LevelFactory lf;
	lf.Initialize(xmldoclist);
	IwAssert(MYAPP, !lf.ConfigExists("abc"));
	lf.SetConfig("abc", lt1);
	LevelTemplate lt4 = lf.GetConfig("abc");
	IwAssert(MYAPP, !lt4.GetName().compare("woohoo"));
}

void Test::RunVertexStreamTest() {
	IW_CALLSTACK_SELF;

	const int count = 10;
	CIwFVec2* verts;
	VertexStreamWorld* p;

	verts = new CIwFVec2[count];
	p = new VertexStreamWorld();
	p->SetVerts(verts, count, false);
	IwAssert(MYAPP, p->GetVertCount() == count);
	delete [] verts;
	delete p;

	verts = new CIwFVec2[count];
	p = new VertexStreamWorld();
	p->SetVerts(verts, count, true); // imports vertices (takes ownership)
	IwAssert(MYAPP, p->GetVertCount() == count);
	delete p;
}

void Test::RunViewportTest() {
	IW_CALLSTACK_SELF;

	Viewport vp;
	CIwFVec2 worldsize(48.0f, 32.0f);
	CIwSVec2 viewsize(480, 320);
	float w2s = 10.0f;

	// world / screen factor
	vp.SetGeometry(worldsize, viewsize);
	vp.SetWorldToScreenFactor(w2s);
	IwAssert(MYAPP, vp.GetWorldToScreenFactor() == w2s);

	// vector conversion wold/screen
	// note: this depends on w2s == 10.0f; note: screen coords are upside down
	CIwSVec2 vs(10, 10);
	CIwFVec2 vw(1.0f, 31.0f); // 31 is because y is upside down 
	IwAssert(MYAPP, vp.ScreenToWorld(vs) == vw);
	IwAssert(MYAPP, vp.WorldToScreen(vw) == vs);

	// screen/view offset
	// note: this depends on w2s == 10.0f; note: screen coords are upside down
	CIwFVec2 worldcenter = worldsize / 2;
	vp.SetCenterPosition(worldcenter); // center of world
	IwAssert(MYAPP, vp.GetCenterPosition() == worldcenter);
	IwAssert(MYAPP, vp.GetScreenViewOffset() == CIwSVec2(0, 0));

	// vector conversion screen/view
	// note: depends on screen view offset is 0/0
	IwAssert(MYAPP, vp.ScreenToView(vs) == vs);
	IwAssert(MYAPP, vp.ViewToScreen(vs) == vs);

	//CIwSVec2 vs(10, 10);
	//CIwFVec2 vw(1.0f, 31.0f); // 31 is because y is upside down 
	//IwAssert(MYAPP, vp.ScreenToWorld(vs) == vw);
	//IwAssert(MYAPP, vp.WorldToScreen(vw) == vs);

	// focus motion

	//delete vp;
}

void Test::RunFactoryTest() {
	IW_CALLSTACK_SELF;

	std::list<std::string> filenames;
	filenames.push_back("test.xml");
	FactoryManager::InitializeFromFiles(filenames);

	Texture* t = NULL;
	IwAssert(MYAPP, t = FactoryManager::GetTextureFactory().Create("mytexture"));
	delete t;
	Sprite* s = NULL;
	IwAssert(MYAPP, s = FactoryManager::GetBodyFactory().Create("mybody"));
	delete s;
	Level* l = NULL;
	IwAssert(MYAPP, l = FactoryManager::GetLevelFactory().Create("test"));
	delete l;

	FactoryManager::Terminate();
}

void Test::RunSpriteTest() {
	IW_CALLSTACK_SELF;

	std::list<std::string> filenames;
	filenames.push_back("test.xml");
	FactoryManager::InitializeFromFiles(filenames);
	
	Sprite* p;
	p = FactoryManager::GetBodyFactory().Create("mystar");
	delete p;
	p = FactoryManager::GetBodyFactory().Create("mybody");
	delete p;
	p = FactoryManager::GetEffectFactory().Create("myeffect");
	delete p;

	FactoryManager::Terminate();
}

void Test::RunLevelTest() {
	IW_CALLSTACK_SELF;

	std::list<std::string> filenames;
	filenames.push_back("test.xml");
	FactoryManager::InitializeFromFiles(filenames);

	Page* p = new Level(CIwFVec2(15.0f, 10.0f), "background_level_earth");
	CIwSVec2 pos(IwGxGetDeviceWidth(), IwGxGetDisplayHeight());
	p->Update(FrameData());

	// leaks because the renderer uses the GxCache, which is 
	// garbage collected after every rendering to screen.
	// This test does not render to screen.
	//Renderer renderer;
	//p->Render(renderer, pos);

	delete p;

	FactoryManager::Terminate();
}

void Test::RunOscillatorTest() {
	IW_CALLSTACK_SELF;

	Oscillator o(0.5, 100, 200);
	IwAssert(MYAPP, o.GetValue() == 150);
	o.Update(500);
	IwAssert(MYAPP, o.GetValue() == 200);
	o.Update(500);
	IwAssert(MYAPP, o.GetValue() == 150);
	o.Update(500);
	IwAssert(MYAPP, o.GetValue() == 100);
	o.Update(500);
	IwAssert(MYAPP, o.GetValue() == 150);
}

void Test::RunPinchGestureTest() {
	IW_CALLSTACK_SELF;

	PinchGesture pinch;
	IwAssert(MYAPP, pinch.IsPinching() == false);
	IwAssert(MYAPP, pinch.GetPinchInfo().movement == CIwFVec2::g_Zero);

	CIwSVec2 start0(17, 23);
	CIwSVec2 translation0(7, 3);

	CIwFVec2 expectedtranslation;
	CIwFVec2 expectedcenter;
	PinchGesture::PinchInfo info;

	{
		// single touch / translation / release
		pinch.AddTouch(0, start0);
		expectedtranslation = CIwFVec2::g_Zero;
		expectedcenter = CIwFVec2((float)start0.x, (float)start0.y);
		info = pinch.GetPinchInfo();
		IwAssert(MYAPP, pinch.IsPinching() == true);
		IwAssert(MYAPP, info.movement == expectedtranslation);
		IwAssert(MYAPP, info.currentcenter == expectedcenter);

		pinch.MoveTouch(0, start0 + translation0);
		expectedtranslation = CIwFVec2((float)translation0.x, (float)translation0.y);
		expectedcenter = CIwFVec2((float)start0.x, (float)start0.y) + CIwFVec2((float)translation0.x, (float)translation0.y);
		info = pinch.GetPinchInfo();
		IwAssert(MYAPP, pinch.IsPinching() == true);
		IwAssert(MYAPP, info.movement == expectedtranslation);
		IwAssert(MYAPP, info.currentcenter == expectedcenter);

		pinch.RemoveTouch(0);
		expectedtranslation = CIwFVec2::g_Zero;
		expectedcenter = CIwFVec2::g_Zero;
		info = pinch.GetPinchInfo();
		IwAssert(MYAPP, pinch.IsPinching() == false);
		IwAssert(MYAPP, info.movement == expectedtranslation);
		IwAssert(MYAPP, info.currentcenter == expectedcenter);
	}

	CIwSVec2 start1(37, 43);
	CIwSVec2 translation1(27, 23);

	CIwSVec2 start2(27, 33);
	CIwSVec2 translation2(36, 45);

	{
		// triple touch / translation / release
		pinch.AddTouch(0, start0);
		expectedtranslation = CIwFVec2::g_Zero;
		expectedcenter = CIwFVec2((float)start0.x, (float)start0.y);
		info = pinch.GetPinchInfo();
		IwAssert(MYAPP, pinch.IsPinching() == true);
		IwAssert(MYAPP, info.movement == expectedtranslation);
		IwAssert(MYAPP, info.currentcenter == expectedcenter);

		pinch.AddTouch(1, start1);
		info = pinch.GetPinchInfo();
		expectedcenter = (CIwFVec2((float)start0.x, (float)start0.y) + CIwFVec2((float)start1.x, (float)start1.y)) / 2.0f;
		IwAssert(MYAPP, pinch.IsPinching() == true);
		IwAssert(MYAPP, info.movement == expectedtranslation);
		IwAssert(MYAPP, info.currentcenter == expectedcenter);

		pinch.MoveTouch(1, start1 + translation1);
		CIwFVec2 expectedtranslation = CIwFVec2((float)translation1.x, (float)translation1.y) / 2.0f;
		expectedcenter = (CIwFVec2((float)start0.x, (float)start0.y) + CIwFVec2((float)start1.x, (float)start1.y) + CIwFVec2((float)translation1.x, (float)translation1.y)) / 2.0f;
		info = pinch.GetPinchInfo();
		IwAssert(MYAPP, pinch.IsPinching() == true);
		IwAssert(MYAPP, info.movement == expectedtranslation);
		IwAssert(MYAPP, info.currentcenter == expectedcenter);

		pinch.AddTouch(2, start2);
		info = pinch.GetPinchInfo();
		IwAssert(MYAPP, pinch.IsPinching() == true);
		IwAssert(MYAPP, info.movement == expectedtranslation);

		pinch.MoveTouch(2, start2 + translation2);
		expectedtranslation += CIwFVec2((float)translation2.x, (float)translation2.y) / 3.0f;
		info = pinch.GetPinchInfo();
		IwAssert(MYAPP, pinch.IsPinching() == true);
		IwAssert(MYAPP, info.movement == expectedtranslation);

		pinch.RemoveTouch(2);
		info = pinch.GetPinchInfo();
		IwAssert(MYAPP, pinch.IsPinching() == true);
		IwAssert(MYAPP, info.movement == expectedtranslation);

		pinch.RemoveTouch(1);
		info = pinch.GetPinchInfo();
		IwAssert(MYAPP, pinch.IsPinching() == true);
		IwAssert(MYAPP, info.movement == expectedtranslation);

		pinch.RemoveTouch(0);
		expectedtranslation = CIwFVec2::g_Zero;
		info = pinch.GetPinchInfo();
		IwAssert(MYAPP, pinch.IsPinching() == false);
		IwAssert(MYAPP, info.movement == expectedtranslation);
	}
}

void Test::RunTouchpadTest() {
	IW_CALLSTACK_SELF;
	Touchpad t;
	
	CIwSVec2 touchpadsize(320, 240);
	t.SetSize(touchpadsize);

	CIwSVec2 touchpadpos(320, 240);
	t.SetPosition(touchpadpos);

	// center touch
	CIwSVec2 touchpos(320, 240);
	IwAssert(MYAPP, t.HitTest(touchpos));
	t.SetTouch(touchpos);
	IwAssert(MYAPP, t.GetTouchVectorNormalized() == CIwFVec2::g_Zero);

	// touch inbounds
	touchpos = CIwSVec2(160, 120);
	IwAssert(MYAPP, t.HitTest(touchpos));
	t.SetTouch(touchpos);
	IwAssert(MYAPP, t.GetTouchVectorNormalized() == CIwFVec2(-1.0f, -1.0f));

	touchpos = CIwSVec2(480, 120);
	IwAssert(MYAPP, t.HitTest(touchpos));
	t.SetTouch(touchpos);
	IwAssert(MYAPP, t.GetTouchVectorNormalized() == CIwFVec2(1.0f, -1.0f));

	touchpos = CIwSVec2(480, 360);
	IwAssert(MYAPP, t.HitTest(touchpos));
	t.SetTouch(touchpos);
	IwAssert(MYAPP, t.GetTouchVectorNormalized() == CIwFVec2(1.0f, 1.0f));

	touchpos = CIwSVec2(160, 360);
	IwAssert(MYAPP, t.HitTest(touchpos));
	t.SetTouch(touchpos);
	IwAssert(MYAPP, t.GetTouchVectorNormalized() == CIwFVec2(-1.0f, 1.0f));

	// touch inbounds with floating points
	touchpos = CIwSVec2(240, 180);
	IwAssert(MYAPP, t.HitTest(touchpos));
	t.SetTouch(touchpos);
	IwAssert(MYAPP, t.GetTouchVectorNormalized() == CIwFVec2(-0.5f, -0.5f));
	
	// touch outbounds
	IwAssert(MYAPP, !t.HitTest(CIwSVec2(159, 119)));
	IwAssert(MYAPP, !t.HitTest(CIwSVec2(481, 119)));
	IwAssert(MYAPP, !t.HitTest(CIwSVec2(481, 361)));
	IwAssert(MYAPP, !t.HitTest(CIwSVec2(159, 361)));
}


