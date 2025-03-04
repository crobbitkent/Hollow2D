#include "BarrelParticle.h"


#include "EngineGlobals.h"

#include "Scene/Scene.h"
#include "Component/CameraComponent.h"
#include "Resource/Animation2DSequence.h"
#include "Component/Animation2D.h"



#include "Resource/ResourceManager.h"
#include "Resource/Material.h"
#include "Component/ColliderSphere2D.h"
#include "Component/ColliderRect.h"


#include "../RandomNumber.h"

int BarrelParticle::m_iStyle = 1;






BarrelParticle::BarrelParticle()
	: m_pMesh(nullptr)
	, m_pMovement(nullptr)
	, m_pRotPivot(nullptr)
	, m_pChildMesh(nullptr)
	, m_pBody(nullptr)
{


	m_fRot = (float)RandomNumber::GetRandomNumber(0, 360);
}

BarrelParticle::~BarrelParticle()
{
	SAFE_RELEASE(m_pMovement);
	SAFE_RELEASE(m_pRotPivot);
	SAFE_RELEASE(m_pMesh);
	SAFE_RELEASE(m_pChildMesh);
	SAFE_RELEASE(m_pBody);
}

bool BarrelParticle::Init()
{
	if (false == CGameObject::Init())
	{
		return false;
	}



	m_pMesh = CreateComponent<CStaticMeshComponent>("Mesh");
	CStaticMesh*	pMesh = (CStaticMesh*)GET_SINGLE(CResourceManager)->FindMesh("TexRect");
	m_pMesh->SetStaticMesh(pMesh);
	SetRoot(m_pMesh);



	m_pMovement = CGameObject::CreateComponent<CCharacterMovementComponent>("Movement");
	m_pMovement->SetUpdateComponent(m_pMesh);
	SetPhysics(true);

	// CHILD MESH
	m_pChildMesh = CreateComponent<CStaticMeshComponent>("Mesh");
	m_pChildMesh->SetStaticMesh(pMesh);
	SAFE_RELEASE(pMesh);

	// CHILD MATERIAL
	CMaterial* pMaterial = nullptr;

	switch (m_iStyle)
	{
	case 1:
		pMaterial = GET_SINGLE(CResourceManager)->FindMaterial("BarrelParticle0");
		m_pChildMesh->SetMaterial(pMaterial);
		break;

	case 2:
		pMaterial = GET_SINGLE(CResourceManager)->FindMaterial("BarrelParticle1");
		m_pChildMesh->SetMaterial(pMaterial);
		break;

	case 3:
		pMaterial = GET_SINGLE(CResourceManager)->FindMaterial("BarrelParticle2");
		m_pChildMesh->SetMaterial(pMaterial);
		break;

	case 4:
		pMaterial = GET_SINGLE(CResourceManager)->FindMaterial("BarrelParticle3");
		m_pChildMesh->SetMaterial(pMaterial);
		break;

	case 5:
		pMaterial = GET_SINGLE(CResourceManager)->FindMaterial("BarrelParticle4");
		m_pChildMesh->SetMaterial(pMaterial);
		break;

	case 6:
		pMaterial = GET_SINGLE(CResourceManager)->FindMaterial("BarrelParticle5");
		m_pChildMesh->SetMaterial(pMaterial);
		break;


	default:
		BOOM;
		break;
	}

	m_iStyle++;

	SAFE_RELEASE(pMaterial);



	// ROT PIVOT
	m_pRotPivot = CreateComponent<CSceneComponent>("RotPivot");


	m_pMesh->AddChild(m_pRotPivot, TR_POS | TR_ROT);
	m_pRotPivot->AddChild(m_pChildMesh, TR_POS | TR_ROT);

	m_pChildMesh->SetRelativeScale(100.f, 100.f, 0.f);
	m_pChildMesh->SetRelativePos(0.f, 0.f, 0.f);
	m_pChildMesh->SetPivot(0.5f, 0.5f, 0.f);

	/*if (0 == m_iStyle)
	{
		pMaterial = GET_SINGLE(CResourceManager)->FindMaterial("BarrelParticle0");
	}
	else if (1 == m_iStyle)
	{
		pMaterial = GET_SINGLE(CResourceManager)->FindMaterial("BarrelParticle1");
	}
	else
	{
		pMaterial = GET_SINGLE(CResourceManager)->FindMaterial("BarrelParticle2");
	}*/
	pMaterial = GET_SINGLE(CResourceManager)->FindMaterial("Empty");

	m_pMesh->SetMaterial(pMaterial);
	SAFE_RELEASE(pMaterial);

	m_pMesh->SetRelativeScale(1.f, 1.f, 1.f);
	m_pMesh->SetPivot(0.5f, 0.5f, 0.f);


	int speed = RandomNumber::GetRandomNumber(400, 600);

	m_pMovement->SetMoveSpeed((float)speed);

	m_fMoveSpeed = speed;


	// COLLIDER
	m_pBody = CGameObject::CreateComponent<CColliderRect>("ParticleBody");


	m_pBody->SetExtent(50.f, 50.f);
	m_pBody->SetPivot(0.5f, 0.5f, 0.f);

	m_pBody->SetCollisionProfile("Particle");

	m_pBody->AddBlockCallback<BarrelParticle>(this, &BarrelParticle::OnBlock);

	m_pMesh->AddChild(m_pBody, TR_POS);


	// �߷� ����
	CGameObject::SetPhysics(true);
	// SetForce(m_fCurrentForce * 1.2f);

	m_fGravitySpeed = 30.f;

	return true;
}

void BarrelParticle::Begin()
{
	CGameObject::Begin();
}

void BarrelParticle::Update(float fTime)
{
	CGameObject::Update(fTime);

	


	// �ð�
	m_fLifeTime += fTime;

	if (m_fLifeTime >= m_fDeathTime)
	{
		Kill();

		return;
	}




	if (true == m_bStop)
	{
		ClearGravity();
		return;
	}














	// ���� ������
	if (true == m_bOnLand)
	{
		ClearGravity();
		return;
	}

	// ����
	if (false == m_bJump)
	{
		ClearGravity();

		m_fCurrentForce = RandomNumber::GetRandomNumber(10, 300);

		m_fForce = m_fCurrentForce;

		m_bJump = true;
		m_bOnLand = false;
	}

	// m_fForce -= (m_fOriginForce * m_fOriginForce);

	// ������
	if (1 == m_iDir)
	{
		if (true == m_bNoRight)
		{
			m_pMovement->AddMovement(GetWorldAxis(AXIS_X) * -1.f);
			m_fMoveSpeed *= 0.6f;
			m_pMovement->SetMoveSpeed(m_fMoveSpeed);

			m_iDir = 2;
			m_bNoRight = false;
		}
		else
		{
			
			m_pMovement->AddMovement(GetWorldAxis(AXIS_X));
			m_pRotPivot->AddRelativeRotationZ(-180.f * fTime);
		}

	}
	// ����
	else
	{
		if (true == m_bNoLeft)
		{
			m_pMovement->AddMovement(GetWorldAxis(AXIS_X) * -1.f);
			m_fMoveSpeed *= 0.6f;
			m_pMovement->SetMoveSpeed(m_fMoveSpeed);

			m_iDir = 1;
			m_bNoLeft = false;
		}
		else
		{
			m_pMovement->AddMovement(GetWorldAxis(AXIS_X) * -1.f);
			m_pRotPivot->AddRelativeRotationZ(180.f * fTime);
		}
	}
}

void BarrelParticle::Render(float fTime)
{
	CGameObject::Render(fTime);
}

void BarrelParticle::MoveX()
{

}

void BarrelParticle::JumpEnd(float fTime)
{
	m_fForce = 0.f;
}

void BarrelParticle::OnBlock(CColliderBase * pSrc, CColliderBase * pDest, float fTime)
{
	if (nullptr == pDest)
	{
		return;
	}



	// if ("Stage" == pDest->GetCollisionProfile()->strName)
	if (true == pDest->IsStage())
	{
		int playerPos = (int)pSrc->GetIntersect().z;

		switch (playerPos)
		{
		case 1: // LEFT
			m_pMovement->AddMovement(Vector3(pSrc->GetIntersect().x * -2.f, 0.f, 0.f));
			m_bNoRight = true;

			break;
		case 2: // TOP
			m_pMovement->AddMovement(Vector3(0.f, pSrc->GetIntersect().y * 2.f, 0.f));
			ClearGravity();
			JumpEnd(fTime);
			m_bOnLand = true;
			m_bNoRight = false;
			m_bNoLeft = false;


			break;
		case 3: // RIGHT
			m_pMovement->AddMovement(Vector3(pSrc->GetIntersect().x * 2.f, 0.f, 0.f));
			m_bNoLeft = true;

			break;
		case 4: // BOTTOM
			m_pMovement->AddMovement(Vector3(0.f, pSrc->GetIntersect().y * -2.f, 0.f));

			// m_bCeiling = true;
			break;
		default:
			BOOM
				break;
		}

		// �ٿ
		if (true == m_bJump && true == m_bOnLand)
		{
			m_fCurrentForce = m_fCurrentForce * 0.33f;
			m_fMoveSpeed = m_fMoveSpeed * 0.33f;
			ClearGravity();

			if (m_fCurrentForce <= 10.f)
			{
				// ����, ��
				m_bJump = false;
				m_bStop = true;
			}
			// �ٽ� �ѹ� ƨ��
			else
			{
				float y = pDest->GetColliderSectionMax().y + pSrc->GetRelativeScale().y * 0.5f + 5.f;

				SetWorldPos(Vector3(GetWorldPos().x, y, GetWorldPos().z));

				SetForce(m_fCurrentForce);
				m_pMovement->SetMoveSpeed(m_fMoveSpeed);
				m_bJump = true;
				m_bOnLand = false;

				return;
			}
		}

		if (false == m_bNoLeft && false == m_bNoRight)
		{
			ClearGravity();
			JumpEnd(fTime);
		}


	}
}

void BarrelParticle::SetDirection(int dir)
{
	m_iDir = dir;
}

void BarrelParticle::ResetStyle()
{
	m_iStyle = 1;
}



















/////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////

//
//bool BarrelParticle::Init()
//{
//	if (false == CGameObject::Init())
//	{
//		return false;
//	}
//
//
//
//	m_pMesh = CreateComponent<CStaticMeshComponent>("Mesh");
//	CStaticMesh*	pMesh = (CStaticMesh*)GET_SINGLE(CResourceManager)->FindMesh("TexRect");
//	m_pMesh->SetStaticMesh(pMesh);
//	SetRoot(m_pMesh);
//
//
//
//	m_pMovement = CGameObject::CreateComponent<CCharacterMovementComponent>("Movement");
//	m_pMovement->SetUpdateComponent(m_pMesh);
//	SetPhysics(true);
//
//	// CHILD MESH
//	m_pChildMesh = CreateComponent<CStaticMeshComponent>("Mesh");
//	m_pChildMesh->SetStaticMesh(pMesh);
//	SAFE_RELEASE(pMesh);
//
//	// CHILD MATERIAL
//	
//	CMaterial* pMaterial = nullptr;
//
//	switch (m_iStyle)
//	{
//	case 1:
//		pMaterial = GET_SINGLE(CResourceManager)->FindMaterial("BarrelParticle0");
//		m_pChildMesh->SetMaterial(pMaterial);
//		break;
//
//	case 2:
//		pMaterial = GET_SINGLE(CResourceManager)->FindMaterial("BarrelParticle1");
//		m_pChildMesh->SetMaterial(pMaterial);
//		break;
//
//	case 3:
//		pMaterial = GET_SINGLE(CResourceManager)->FindMaterial("BarrelParticle2");
//		m_pChildMesh->SetMaterial(pMaterial);
//		break;
//
//	case 4:
//		pMaterial = GET_SINGLE(CResourceManager)->FindMaterial("BarrelParticle3");
//		m_pChildMesh->SetMaterial(pMaterial);
//		break;
//
//	case 5:
//		pMaterial = GET_SINGLE(CResourceManager)->FindMaterial("BarrelParticle4");
//		m_pChildMesh->SetMaterial(pMaterial);
//		break;
//
//	case 6:
//		pMaterial = GET_SINGLE(CResourceManager)->FindMaterial("BarrelParticle5");
//		m_pChildMesh->SetMaterial(pMaterial);
//		break;
//
//
//	default:
//		BOOM;
//		break;
//	}
//
//	m_iStyle++;
//
//	SAFE_RELEASE(pMaterial);
//
//
//
//	// ROT PIVOT
//	m_pRotPivot = CreateComponent<CSceneComponent>("RotPivot");
//
//
//	m_pMesh->AddChild(m_pRotPivot, TR_POS | TR_ROT);
//	m_pRotPivot->AddChild(m_pChildMesh, TR_POS | TR_ROT);
//
//	m_pChildMesh->SetRelativeScale(100.f, 100.f, 0.f);
//	m_pChildMesh->SetRelativePos(0.f, 0.f, 0.f);
//	m_pChildMesh->SetPivot(0.5f, 0.5f, 0.f);
//
//	/*if (0 == m_iStyle)
//	{
//		pMaterial = GET_SINGLE(CResourceManager)->FindMaterial("BarrelParticle0");
//	}
//	else if (1 == m_iStyle)
//	{
//		pMaterial = GET_SINGLE(CResourceManager)->FindMaterial("BarrelParticle1");
//	}
//	else
//	{
//		pMaterial = GET_SINGLE(CResourceManager)->FindMaterial("BarrelParticle2");
//	}*/
//	pMaterial = GET_SINGLE(CResourceManager)->FindMaterial("Empty");
//
//	m_pMesh->SetMaterial(pMaterial);
//	SAFE_RELEASE(pMaterial);
//
//	m_pMesh->SetRelativeScale(1.f, 1.f, 1.f);
//	m_pMesh->SetPivot(0.5f, 0.5f, 0.f);
//
//
//	int speed = RandomNumber::GetRandomNumber(10, 50);
//
//	m_pMovement->SetMoveSpeed((float)speed);
//
//
//	return true;
//}
//
//void BarrelParticle::Begin()
//{
//	CGameObject::Begin();
//}
//
//void BarrelParticle::Update(float fTime)
//{
//	CGameObject::Update(fTime);
//
//	if (false == m_bJumping)
//	{
//		// m_fOriginForce = 2.f;
//		m_fForce = 400;
//
//		m_pMovement->SetRotationSpeed(100.f);
//		m_fGravitySpeed = 1.f;
//
//		m_iDir = RandomNumber::GetRandomNumber(1, 2);
//		m_iRotDir = RandomNumber::GetRandomNumber(1, 2);
//
//		// m_pRotPivot->SetRotationSpeed
//	}
//
//	// �� �Ǵ� ��� �����̱�
//	MoveX();
//
//	// ���� ����
//	if (1 == m_iRotDir)
//	{
//		m_pRotPivot->AddRelativeRotationZ(180.f * fTime);
//	}
//	else
//	{
//		m_pRotPivot->AddRelativeRotationZ(-180.f * fTime);
//	}
//
//
//
//	if (m_fForce < 140)
//	{
//		m_fGravitySpeed -= 0.06f;
//
//		if (m_fGravitySpeed < 0.02f)
//		{
//			m_fGravitySpeed = 0.02f;
//		}
//	}
//
//
//
//
//	// �ϰ�
//	m_fJumpTime += fTime;
//	m_bJumping = true;
//
//
//
//
//
//	if (m_fJumpTime >= 2.f)
//	{
//		Kill();
//	}
//
//
//}
//
//void BarrelParticle::Render(float fTime)
//{
//	CGameObject::Render(fTime);
//}
//
//void BarrelParticle::MoveX()
//{
//	if (1 == m_iDir)
//	{
//		m_pMovement->AddMovement(GetWorldAxis(AXIS_X));
//	}
//	else
//	{
//		m_pMovement->AddMovement(GetWorldAxis(AXIS_X) * -1.f);
//	}
//}
//
//void BarrelParticle::ResetStyle()
//{
//	m_iStyle = 0;
//}
