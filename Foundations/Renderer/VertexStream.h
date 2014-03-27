#ifndef __VERTEXSTREAM_H__
#define __VERTEXSTREAM_H__

#include "IwGeom.h"

template<class TVec>
class VertexStream {
private:
	TVec* m_axVerts;
	int m_iVertCount;

public:
	VertexStream() : m_axVerts(NULL), m_iVertCount(0) {
	}

	~VertexStream() {
		ClearVerts();
	}

public:
	TVec* GetVerts() {
		return m_axVerts;
	}
	
	int GetVertCount() {
		return m_iVertCount;
	}

	void SetVerts(TVec verts[], int vertcount, bool import = false) {
		if (import) {
			// take ownership of memory
			ClearVerts();
			m_axVerts = verts;
		} else {
			// copy into own memory
			AllocateVertices(vertcount);
			for (int i = 0; i < vertcount; i++) {
				m_axVerts[i] = verts[i];
			}
		}
		m_iVertCount = vertcount;
	}

	void MoveVerts(const TVec& offset) {
		for (int i = 0; i < m_iVertCount; i++) {
			m_axVerts[i] += offset;
		}
	}

	void ClosePolygon() {
		if (!m_axVerts || m_iVertCount < 2) {
			return;
		}
		
		TVec* verts = m_axVerts;
		m_axVerts = new TVec[m_iVertCount + 1];
		for (int i = 0; i < m_iVertCount; i++) {
			m_axVerts[i] = verts[i];
		}
		m_axVerts[m_iVertCount] = verts[0];
		m_iVertCount++;
		
		delete [] verts;
	}
	
	bool GetAABB(TVec& lowerleft, TVec& upperright) {
		if (m_iVertCount < 1) {
			return false;
		}
		
		lowerleft = m_axVerts[0];
		upperright = m_axVerts[0];
		
		for (int i = 0; i < m_iVertCount; i++) {
			TVec& v = m_axVerts[i];
			if (upperright.x < v.x) {
				upperright.x = v.x;
			}
			if (upperright.y < v.y) {
				upperright.y = v.y;
			}
			if (lowerleft.x > v.x) {
				lowerleft.x = v.x;
			}
			if (lowerleft.y > v.y) {
				lowerleft.y = v.y;
			}
		}
		
		return true;
	}
	
	VertexStream<TVec>& operator=(const VertexStream<TVec>& vertstream) {
		if (&vertstream == this) {
			return *this;
		}
		SetVerts(vertstream.m_axVerts, vertstream.m_iVertCount, false);
		return *this;
	}

protected:
	template<typename TScale>
	void SetRect(TScale x, TScale y, TScale w, TScale h) {
		TVec* verts = new TVec[4];
		verts[0].x = x;
		verts[0].y = y + h;
		verts[1].x = x + w;
		verts[1].y = y + h;
		verts[2].x = x + w;
		verts[2].y = y;
		verts[3].x = x;
		verts[3].y = y;
		SetVerts(verts, 4, true);
	}
	
private:
	void ClearVerts() {
		if (m_axVerts) {
			delete [] m_axVerts;
			m_axVerts = NULL;
		}
		m_iVertCount = 0;
	}

	void AllocateVertices(int count) {
		if (!m_axVerts || count != m_iVertCount) {
			ClearVerts();
			m_axVerts = new TVec[count];
			m_iVertCount = count;
		}
	}
};

#endif
