/*
 *	copy hgeVector 2008.12
 */

#ifndef DXVECTOR_H
#define DXVECTOR_H

#include <math.h>

/*
** Fast 1.0/sqrtf(float) routine
*/
float InvSqrt(float x);

class CDxVector
{
public:
	float	x,y;

	CDxVector(float _x, float _y)	{ x=_x; y=_y; }
	CDxVector()						{ x=0; y=0; }

	CDxVector	operator-  ()					const { return CDxVector(-x, -y);		}
	CDxVector	operator-  (const CDxVector &v) const { return CDxVector(x-v.x, y-v.y); }
	CDxVector	operator+  (const CDxVector &v) const { return CDxVector(x+v.x, y+v.y); }
	CDxVector&	operator-= (const CDxVector &v)		  { x-=v.x; y-=v.y; return *this;	}
	CDxVector&	operator+= (const CDxVector &v)		  { x+=v.x; y+=v.y; return *this;	}
	bool		operator== (const CDxVector &v)	const { return (x==v.x && y==v.y);		}
	bool		operator!= (const CDxVector &v)	const { return (x!=v.x || y!=v.y);		}

	CDxVector	operator/  (const float scalar)	const { return CDxVector(x/scalar, y/scalar); }
	CDxVector	operator*  (const float scalar) const { return CDxVector(x*scalar, y*scalar); }
	CDxVector&	operator*= (const float scalar)		  { x*=scalar; y*=scalar; return *this;   }

	float		Dot(const CDxVector *v) const { return x*v->x + y*v->y; }
	float		Length() const { return sqrtf(Dot(this)); }
	float		Angle(const CDxVector *v = 0) const;
	
	void		Clamp(const float max) { if(Length() > max)	{ Normalize(); x *= max; y *= max; } }
	CDxVector*	Normalize() { float rc=InvSqrt(Dot(this)); x*=rc; y*=rc; return this; }
	CDxVector*	Rotate(float a);
};

inline CDxVector operator* (const float s, const CDxVector &v)		{ return v*s; }
inline float	 operator^ (const CDxVector &v, const CDxVector &u) { return v.Angle(&u); }
inline float	 operator% (const CDxVector &v, const CDxVector &u) { return v.Dot(&u); }


#endif
