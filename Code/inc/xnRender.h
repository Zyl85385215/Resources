#ifndef _XN_RENDER_H_
#define	_XN_RENDER_H_

#include "xnSprite3D.h"
#include "xnTexture.h"

struct xnTexturePoly {
	Vertex vertex[3];
	xnTexture * pTexture;

	void Set(Vertex * v1, Vertex * v2, Vertex * v3, xnTexture * pTexture);
};

struct xnRender {
	xnList * pPolyList;
	DWORD polyBufferSize;
	xnTexturePoly * polyBuffer;

	static xnRender * Create(void);
	void Free(void);
	void Reset(void);
	void AddPoly(Vertex * v1, Vertex * v2, Vertex * v3, xnTexture * pTexture);
	void AddPolyList(xnList * pVertexList, xnTexture * pTexture);
	void Render(int x, int y);
	void EnabledZBuffer(BOOLEAN flag);
};

#endif