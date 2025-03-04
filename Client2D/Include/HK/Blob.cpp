#include "Blob.h"

#include "EngineGlobals.h"

#include "Scene/Scene.h"

#include "Device.h"
#include "Resource/ResourceManager.h"
#include "Component/Animation2D.h"
#include "Resource/Animation2DSequence.h"
#include "Component/ColliderRect.h"

#include "HKMode.h"

#include "GetHit.h"

#include "../RandomNumber.h"

Blob::Blob()
	: m_bIsOn(false)
	, m_vPlayerPos(Vector3::Zero)
{
}

Blob::~Blob()
{
	SAFE_RELEASE(m_pMesh);
	SAFE_RELEASE(m_pMovement);
	SAFE_RELEASE(m_pBody);
}

bool Blob::Init()
{
	if (false == CGameObject::Init())
	{
		return false;
	}

	m_pMesh = CreateComponent <CStaticMeshComponent>("Mesh");

	m_pBody = CreateComponent<CColliderRect>("FireBody");


	CStaticMesh* pMesh = (CStaticMesh*)GET_SINGLE(CResourceManager)->FindMesh("TexRect");
	m_pMesh->SetStaticMesh(pMesh);


	CMaterial* pMaterial = GET_SINGLE(CResourceManager)->FindMaterial("Blob0");
	m_pMesh->SetMaterial(pMaterial);

	SetRoot(m_pMesh);

	m_pMovement = CGameObject::CreateComponent<CProjectileMovementComponent>("BlobMovement");
	m_pMovement->SetUpdateComponent(m_pMesh);

	///////////////////////////////////////////////////////////


	m_pBody->AddBlockCallback<Blob>(this, &Blob::OnBlock);
	m_pBody->SetCollisionProfile("Object");

	m_pMesh->AddChild(m_pBody, TR_POS);

	m_pBody->SetPivot(0.5f, 0.5f, 0.f);

	SAFE_RELEASE(pMesh);
	SAFE_RELEASE(pMaterial);

	m_fScale = (float)RandomNumber::GetRandomNumber(50, 80);

	m_pMesh->SetRelativeScale(m_fScale, m_fScale, 1.f);
	m_pBody->SetExtent(m_fScale, m_fScale);
	m_pMesh->SetPivot(0.5f, 0.5f, 0.f);

	////////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////////

	



	m_iDir = RandomNumber::GetRandomNumber(1, 2);

	if (2 == m_iDir)
	{
		m_iDir = -1;
	}

	SetPhysics(true);

	return true;
}

void Blob::Begin()
{
	CGameObject::Begin();
}

void Blob::Update(float fTime)
{
	CGameObject::Update(fTime);

	if (0 == m_iDir)
	{
		int a = 0;
	}

	// ����
	// ����
	if (false == m_bJump)
	{
		float m_fCurrentForce = RandomNumber::GetRandomNumber(100, 1000);

		m_fForce = m_fCurrentForce;

		m_bJump = true;
		m_bOnLand = false;


		m_fMoveSpeed = 1500.f - m_fCurrentForce;

		m_pMovement->SetMoveSpeed(m_fMoveSpeed);
	}
	


	m_pMovement->AddMovement(GetWorldAxis(AXIS_X) * m_iDir);
}

void Blob::Render(float fTime)
{
	CGameObject::Render(fTime);
}

void Blob::Start(float fTime, const Vector3 & vPos)
{
}

void Blob::OnBlock(CColliderBase * pSrc, CColliderBase * pDest, float fTime)
{
	if (nullptr == pDest)
	{
		return;
	}

	if (true == pDest->IsStage() || true == pDest->IsPlayer())
	{
		GetHit* gh = m_pScene->SpawnObject<GetHit>(GetWorldPos() + Vector3(0.f, 50.f, 0.f));
		gh->SetAnimation(1);
		gh->SetRelativeScale(m_fScale * 3, m_fScale * 3, 1.f);
		SAFE_RELEASE(gh);

		Kill();
	}








}

CColliderRect * Blob::GetBody() const
{
	return m_pBody;
}

void Blob::SetNormalMonster()
{
	m_fScale = (float)RandomNumber::GetRandomNumber(30, 50);

	m_pMesh->SetRelativeScale(m_fScale, m_fScale, 1.f);
	m_pBody->SetExtent(m_fScale, m_fScale);
}

