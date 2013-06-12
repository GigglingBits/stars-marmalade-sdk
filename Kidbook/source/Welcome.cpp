#include <fstream>
#include "Welcome.h"
#include "Iw2D.h"
#include "Configuration.h"

Welcome::Welcome() {
	m_pxLabel = NULL;
	m_pxScrollView = NULL;

	SetupControls(ReadHelpText().c_str());
}

Welcome::~Welcome() {
	IwGetUIView()->DestroyElements();

	// RRR: restore font, so that text box can render again
	CIwGxFont* pFont = (CIwGxFont*)IwGetResManager()->GetResNamed("Font", "CIwGxFont");
    IwGxFontSetFont(pFont);
	IwGxFontSetAlignmentVer(IW_GX_FONT_ALIGN_MIDDLE);
}

void Welcome::ScrollDown() {
	if (m_pxScrollView) {
		m_pxScrollView->Scroll(CIwVec2(0, 5000));
	}
}

void Welcome::ScrollUp() {
	if (m_pxScrollView) {
		m_pxScrollView->Scroll(CIwVec2(0, -5000));
	}
}

std::string Welcome::ReadHelpText() {
	std::ifstream ifs(Configuration::GetInstance().HelpFile.c_str());
	return std::string((std::istreambuf_iterator<char>(ifs)), std::istreambuf_iterator<char>());
}

void Welcome::SetupControls(const char* text) {
	int margin = 50;
	int width = IwGxGetDisplayWidth() * 5 / 7;
	int height =IwGxGetDisplayHeight(); 

	// start with blank view
	IwGetUIView()->DestroyElements();

	//Create a label to say "Hello World"
	m_pxLabel = new CIwUILabel;

	CIwGxFont* pFont = (CIwGxFont*)IwGetResManager()->GetResNamed("Font", "CIwGxFont");
	m_pxLabel->SetFont(pFont);

	m_pxLabel->SetPosAbsolute(CIwVec2(0, 0));
	m_pxLabel->SetSize(CIwVec2(width - (2 * margin), 5000));
	//m_pxLabel->SetPosAbsolute(CIwVec2(0, 0));
	//m_pxLabel->SetSize(CIwVec2(150, 5000));

    //Set text colour to blue
    CIwColour* LabelColour = new CIwColour();
    LabelColour->Set (0, 0, 0);
	m_pxLabel->SetTextColour (*LabelColour);
    delete LabelColour;

    //Set the text to be displayed
	m_pxLabel->SetCaption(text);

	m_pxScrollView = new CIwUIScrollableView();
	m_pxScrollView->SetPosAbsolute(CIwVec2(margin, margin));
	m_pxScrollView->SetSize(CIwVec2(width - (2 * margin), height - (2 * margin)));
	//m_pxScrollView->SetPosAbsolute(CIwVec2(50, 50));
	//m_pxScrollView->SetSize(CIwVec2(150, 300));
	m_pxScrollView->AddChild(m_pxLabel);

    //Add the label to the UIView singleton
	IwGetUIView()->AddElement(m_pxScrollView);
}

bool Welcome::OnUpdate(bool first) {
	return false;
}

void Welcome::OnRender(const CIwSVec2& screensize, const CIwRect& itemregion) {
	uint32 orig = Iw2DGetColour().Get();
	Iw2DSetColour(0xbb000000);
	Iw2DFillRect(
		CIwFVec2(itemregion.x, itemregion.y),
		CIwFVec2(itemregion.w, itemregion.h));
	
	Iw2DSetColour(0xffffffff);
	Iw2DDrawRect(
		CIwFVec2(itemregion.x, itemregion.y),
		CIwFVec2(itemregion.w, itemregion.h));
	Iw2DSetColour(orig);
}
