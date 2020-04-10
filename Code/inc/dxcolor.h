/*
 *	copy hgeColor 2008.12
 */

#ifndef DXCOLOR_H
#define DXCOLOR_H

#include "dxcommon.h"

#define CDxColor CDxColorRGB

inline void ColorClamp(float &x) { if(x<0.0f) x=0.0f; if(x>1.0f) x=1.0f; }

class CDxColorRGB
{
public:
	float		r,g,b,a;

	CDxColorRGB(float _r, float _g, float _b, float _a) { r=_r; g=_g; b=_b; a=_a; }
	CDxColorRGB(DWORD col) { SetHWColor(col); }
	CDxColorRGB() { r=g=b=a=0; }

	CDxColorRGB		operator-  (const CDxColorRGB &c) const { return CDxColorRGB(r-c.r, g-c.g, b-c.b, a-c.a); }
	CDxColorRGB		operator+  (const CDxColorRGB &c) const { return CDxColorRGB(r+c.r, g+c.g, b+c.b, a+c.a); }
	CDxColorRGB		operator*  (const CDxColorRGB &c) const { return CDxColorRGB(r*c.r, g*c.g, b*c.b, a*c.a); }
	CDxColorRGB&	operator-= (const CDxColorRGB &c)		{ r-=c.r; g-=c.g; b-=c.b; a-=c.a; return *this;   }
	CDxColorRGB&	operator+= (const CDxColorRGB &c)		{ r+=c.r; g+=c.g; b+=c.b; a+=c.a; return *this;   }
	bool			operator== (const CDxColorRGB &c) const { return (r==c.r && g==c.g && b==c.b && a==c.a);  }
	bool			operator!= (const CDxColorRGB &c) const { return (r!=c.r || g!=c.g || b!=c.b || a!=c.a);  }

	CDxColorRGB		operator/  (const float scalar) const { return CDxColorRGB(r/scalar, g/scalar, b/scalar, a/scalar); }
	CDxColorRGB		operator*  (const float scalar) const { return CDxColorRGB(r*scalar, g*scalar, b*scalar, a*scalar); }
	CDxColorRGB&	operator*= (const float scalar)		  { r*=scalar; g*=scalar; b*=scalar; a*=scalar; return *this;   }

	void			Clamp() { ColorClamp(r); ColorClamp(g); ColorClamp(b); ColorClamp(a); }
	void			SetHWColor(DWORD col) {	a = (col>>24)/255.0f; r = ((col>>16) & 0xFF)/255.0f; g = ((col>>8) & 0xFF)/255.0f; b = (col & 0xFF)/255.0f;	}
	DWORD			GetHWColor() const { return (DWORD(a*255.0f)<<24) + (DWORD(r*255.0f)<<16) + (DWORD(g*255.0f)<<8) + DWORD(b*255.0f);	}
};

inline CDxColorRGB operator* (const float sc, const CDxColorRGB &c) { return c*sc; }


class CDxColorHSV
{
public:
	float		h,s,v,a;

	CDxColorHSV(float _h, float _s, float _v, float _a) { h=_h; s=_s; v=_v; a=_a; }
	CDxColorHSV(DWORD col) { SetHWColor(col); }
	CDxColorHSV() { h=s=v=a=0; }

	CDxColorHSV		operator-  (const CDxColorHSV &c) const { return CDxColorHSV(h-c.h, s-c.s, v-c.v, a-c.a); }
	CDxColorHSV		operator+  (const CDxColorHSV &c) const { return CDxColorHSV(h+c.h, s+c.s, v+c.v, a+c.a); }
	CDxColorHSV		operator*  (const CDxColorHSV &c) const { return CDxColorHSV(h*c.h, s*c.s, v*c.v, a*c.a); }
	CDxColorHSV&	operator-= (const CDxColorHSV &c)		{ h-=c.h; s-=c.s; v-=c.v; a-=c.a; return *this;   }
	CDxColorHSV&	operator+= (const CDxColorHSV &c)		{ h+=c.h; s+=c.s; v+=c.v; a+=c.a; return *this;   }
	bool			operator== (const CDxColorHSV &c) const { return (h==c.h && s==c.s && v==c.v && a==c.a);  }
	bool			operator!= (const CDxColorHSV &c) const { return (h!=c.h || s!=c.s || v!=c.v || a!=c.a);  }

	CDxColorHSV		operator/  (const float scalar) const { return CDxColorHSV(h/scalar, s/scalar, v/scalar, a/scalar); }
	CDxColorHSV		operator*  (const float scalar) const { return CDxColorHSV(h*scalar, s*scalar, v*scalar, a*scalar); }
	CDxColorHSV&	operator*= (const float scalar)		  { h*=scalar; s*=scalar; v*=scalar; a*=scalar; return *this;   }

	void			Clamp() { ColorClamp(h); ColorClamp(s); ColorClamp(v); ColorClamp(a); }
	void			SetHWColor(DWORD col);
	DWORD			GetHWColor() const;
};

inline CDxColorHSV operator* (const float sc, const CDxColorHSV &c) { return c*sc; }

#endif