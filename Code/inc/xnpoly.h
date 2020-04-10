#ifndef _POLYGONS_H_
#define _POLYGONS_H_

struct point {
	int	x, y;
};

struct tpoint {
	int	x, y;
	int	u, v;
};

void xnPolyFlat(point * vertexlist, int vertices, WORD color, int alpha = 255);
void xnPolyTexture(tpoint * tvtx, int vertices, xnBmp * texture, int alpha = 255);

#endif
