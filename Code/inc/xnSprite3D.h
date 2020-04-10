#ifndef _XN_SPRITE_3D_H_
#define	_XN_SPRITE_3D_H_

#include <windows.h>
#include "xnList.h"

#pragma pack (push)
#pragma pack (16)

struct Vertex {
	float	x, y, z, w;
	WORD	fu, fv;
};

#pragma pack (pop)

struct UVPoint {
	float	u, v;
};

struct Triangle {
	DWORD a, b, c;
};

struct xnUV_List {
	UVPoint * pUV_PointBuffer;
	DWORD uvPointBufferSize;
	DWORD uvCount;
	
	static xnUV_List * Create(DWORD bufferSize);
	void Free(void);
	void AddUV(UVPoint * uvPoint);
	UVPoint * GetUV(DWORD index);
};

struct xnFrame3D {
	DWORD maxCount;
	DWORD vertexCount;
	Vertex * vertices;

	static xnFrame3D * Create(DWORD vertexCount);
	void Free(void);
	void AddVertex(Vertex *vertex);
	Vertex * GetVertex(DWORD index);
	void SetPosition(Vertex * pPos);
	void SetScale(float scale);
	void RotateX(float angle);
	void RotateY(float angle);
	void RotateZ(float angle);
};

struct xnFrame3DList {
	xnList * pFrameList;
	xnFrame3D * pCurrFrame;

	static xnFrame3DList * Create();
	void Free(void);
	void AddFrame(xnFrame3D * pFrame);
	xnFrame3D * GetFrame(float index);
	void SetUV(xnUV_List * pUV_List, WORD textWidth, WORD textHeight);
};

struct xnTriangleList {
	Triangle * triangles;
	DWORD maxCount;
	DWORD triangleCount;

	static xnTriangleList * Create(DWORD triangleCount);
	void Free(void);
	void AddTriangle(Triangle * pTriangle);
	Triangle * GetTriangle(DWORD index);
};

struct xnSprite3D_Obj {
	char * name;
	xnTriangleList * pTriangleList;

	static xnSprite3D_Obj * Create(char * name, DWORD triangleCount);
	void Free(void);
	xnList * GetVertexList(xnFrame3D * pFrame);
};

struct xnSprite3D {
	xnFrame3DList * pFrameList;
	xnUV_List * pUVPointList;
	xnList * pObjectList;

	static xnSprite3D * LoadSprite(char * sprite3DName);
	void Free(void);

	void SetTextureSize(WORD w, WORD h);
	xnSprite3D_Obj * GetObject(char * objName);
};

#endif

