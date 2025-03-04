#pragma once

#include "Object/GameObject.h"


class Blob : public CGameObject
{


public:
	Blob();
	~Blob();

	virtual bool Init();
	virtual void Begin();
	virtual void Update(float fTime);
	virtual void Render(float fTime);

	void Start(float fTime, const Vector3& vPos);
	void OnBlock(class CColliderBase* pSrc, class CColliderBase* pDest, float fTime);
	class CColliderRect* GetBody() const;
	void SetDir(int dir) { m_iDir = dir; Flip(dir); }

	void SetNormalMonster();

private:
	class CStaticMeshComponent* m_pMesh;
	// class CAnimation2D* m_pAnimation;
	class CProjectileMovementComponent* m_pMovement;
	class CColliderRect* m_pBody;

	// class HitEffect* m_pHitEffect;

	bool m_bIsOn;
	Vector3 m_vPlayerPos;

	bool m_bJump = false;
	int m_iDir = 0;

	float m_fMoveSpeed = 0.f;

	float m_fScale;
};


