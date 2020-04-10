/*
 *	copy dxParticle 2008.12
 */

#ifndef DXPARTICLE_H
#define DXPARTICLE_H

#include "dxcommon.h"
#include "dxcolor.h"
#include "dxvector.h"
#include "dxsprite.h"
#include "dxrect.h"

#define MAX_PARTICLES	500
#define MAX_PSYSTEMS	100

struct dxParticle
{
	CDxVector	vecLocation;
	CDxVector	vecVelocity;

	float		fGravity;
	float		fRadialAccel;
	float		fTangentialAccel;

	float		fSpin;
	float		fSpinDelta;

	float		fSize;
	float		fSizeDelta;

	CDxColor	colColor;		// + alpha
	CDxColor	colColorDelta;

	float		fAge;
	float		fTerminalAge;
};

struct dxParticleSystemInfo
{
	CDxSprite *	sprite;    // texture + blend mode
	int			nEmission; // particles per sec
	float		fLifetime;

	float		fParticleLifeMin;
	float		fParticleLifeMax;

	float		fDirection;
	float		fSpread;
	bool		bRelative;

	float		fSpeedMin;
	float		fSpeedMax;

	float		fGravityMin;
	float		fGravityMax;

	float		fRadialAccelMin;
	float		fRadialAccelMax;

	float		fTangentialAccelMin;
	float		fTangentialAccelMax;

	float		fSizeStart;
	float		fSizeEnd;
	float		fSizeVar;

	float		fSpinStart;
	float		fSpinEnd;
	float		fSpinVar;

	CDxColor	colColorStart; // + alpha
	CDxColor	colColorEnd;
	float		fColorVar;
	float		fAlphaVar;
};

class CDxParticleSystem
{
public:
	dxParticleSystemInfo info;
	
	CDxParticleSystem(const char * filename, CDxSprite * sprite);
	CDxParticleSystem(dxParticleSystemInfo * psi);
	CDxParticleSystem(const CDxParticleSystem & ps);
	~CDxParticleSystem() { }

	CDxParticleSystem&	operator= (const CDxParticleSystem &ps);


	void				Render();
	void				FireAt(float x, float y);
	void				Fire();
	void				Stop(bool bKillParticles=false);
	void				Update(float fDeltaTime);
	void				MoveTo(float x, float y, bool bMoveParticles=false);
	void				Transpose(float x, float y) { fTx=x; fTy=y; }
	void				SetScale(float scale) { fScale = scale; }
	void				TrackBoundingBox(bool bTrack) { bUpdateBoundingBox=bTrack; }

	int					GetParticlesAlive() const { return nParticlesAlive; }
	float				GetAge() const { return fAge; }
	void				GetPosition(float *x, float *y) const { *x=vecLocation.x; *y=vecLocation.y; }
	void				GetTransposition(float *x, float *y) const { *x=fTx; *y=fTy; }
	float				GetScale() { return fScale; }
	CDxRect *			GetBoundingBox(CDxRect * rect) const;

private:
	CDxParticleSystem();

	float				fAge;
	float				fEmissionResidue;

	CDxVector			vecPrevLocation;
	CDxVector			vecLocation;
	float				fTx, fTy;
	float				fScale;

	int					nParticlesAlive;
	CDxRect				rectBoundingBox;
	bool				bUpdateBoundingBox;

	dxParticle			particles[MAX_PARTICLES];
};

class CDxParticleManager
{
public:
	CDxParticleManager();
	~CDxParticleManager();

	void				Update(float dt);
	void				Render();

	CDxParticleSystem*	SpawnPS(dxParticleSystemInfo *psi, float x, float y);
	bool				IsPSAlive(CDxParticleSystem *ps) const;
	void				Transpose(float x, float y);
	void				GetTransposition(float *dx, float *dy) const {*dx=tX; *dy=tY;}
	void				KillPS(CDxParticleSystem *ps);
	void				KillAll();

private:
	CDxParticleManager(const CDxParticleManager &);
	CDxParticleManager&	operator= (const CDxParticleManager &);

	int					nPS;
	float				tX;
	float				tY;
	CDxParticleSystem*	psList[MAX_PSYSTEMS];
};


#endif
