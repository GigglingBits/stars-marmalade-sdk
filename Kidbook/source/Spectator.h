//--------------------------------------------------------------------------
// Viewer that can move around on the PageMap
//--------------------------------------------------------------------------
#ifndef _SPECTATOR_H_ 
#define _SPECTATOR_H_ 

#include "Page.h"
#include "PageMap.h"
#include "Directions.h"
#include "BaseBrick.h"

class Spectator : public BaseBrick {
private:
	int m_iColCursor;
	int m_iRowCursor;

	PageMap& m_rxMap;
	Page* m_apxPages[DIR_TYPE_MAXVAL+1];

	Direction m_eLastMoveDir;

public:
	Spectator(PageMap& map);
	void Move(Direction dir);
	Page* GetPage(Direction dir);

private:
	bool OnUpdate(bool first);
	void OnRender(const CIwSVec2& screensize, const CIwRect& itemregion);

	inline bool UpdateSingle(Direction dir, bool first);
	inline void RenderSingle(Direction dir, const CIwSVec2& screensize, const CIwRect& itemregion);

	void ActivateAll();
	inline bool SetActive(Direction dir, bool active);
	void UpdatePageReferences();
};

#endif
