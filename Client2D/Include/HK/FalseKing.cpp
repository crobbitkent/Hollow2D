#include "FalseKing.h"

#include "EngineGlobals.h"

#include "Resource/Animation2DSequence.h"
#include "Component/Animation2D.h"

#include "Component/ColliderRect.h"
#include "Component/ColliderOBB2D.h"

#include "TimerManager.h"

#include "Sencer.h"

#include "../RandomNumber.h"
#include "HollowKnight.h"

#include "MonsterHitEffect.h"
#include "MightyEffect.h"
#include "PowerWave.h"
#include "FireBarrel.h"

#include "BloodDust.h"
#include "Blob.h"

#include "BabyFace.h"

#include "SoftEffect.h"
#include "WhiteBall.h"

FalseKing::FalseKing()
{
}


FalseKing::~FalseKing()
{
	SAFE_RELEASE(m_pPlayerLeftSencer);
	SAFE_RELEASE(m_pPlayerRightSencer);

}

bool FalseKing::Init()
{
	if (false == Bug::Init())
	{
		BOOM
			return false;
	}

	m_pMesh->SetRelativeScale(1200.f, 1200.f, 1.f);

	Bug::SetAnimation("FK");
	// m_pMesh->SetPivot(0.57f, 0.385f, 0.f);
	m_pMesh->SetPivot(0.55f, 0.46f, 0.f);

	// �÷��̾� ã������ ����
	m_pPlayerLeftSencer = m_pScene->SpawnObject<Sencer>();
	m_pSencerBody = m_pPlayerLeftSencer->GetBody();
	m_pMesh->AddChild(m_pSencerBody, TR_POS);
	SAFE_RELEASE(m_pSencerBody);


	m_pPlayerRightSencer = m_pScene->SpawnObject<Sencer>();
	m_pSencerBody = m_pPlayerRightSencer->GetBody();

	m_pMesh->AddChild(m_pSencerBody, TR_POS);
	SAFE_RELEASE(m_pSencerBody);

	m_fMoveSpeed = 400.f;

	// m_pPlayerLeftSencer->SetRelativePos(-300.f, 0.f, 0.f);
	// m_pPlayerRightSencer->SetRelativePos(300.f, 0.f, 0.f);

	m_fGravitySpeed = 25.f;

	m_bDieLand = false;

	m_pBody->AddBlockCallback<FalseKing>(this, &FalseKing::OnBlock);

	m_pBody->SetExtent(250.f, 280.f);

	m_pHK = (HollowKnight*)m_pScene->GetGameMode()->GetPlayer();

	m_iHP = 30;

	SetCurrentState(BS_WALK);

	m_pBody->SetMonster(true);

	m_eDir = DIR_LEFT;
	Flip(-1);

	return true;
}

void FalseKing::Begin()
{
	Bug::Begin();

	m_pMovement->SetMoveSpeed(m_fMoveSpeed);

	// Bug::TurnOff();
}

void FalseKing::Update(float fTime)
{
	if (false == m_pHK->BossOn())
	{
		return;
	}




	CGameObject::Update(fTime);

	m_vHK = m_pHK->GetWorldPos();

	if (DIR_LEFT == m_eDir)
	{
		m_pMesh->SetPivot(0.45f, 0.46f, 0.f);
	}
	else if (DIR_RIGHT == m_eDir)
	{
		m_pMesh->SetPivot(0.55f, 0.46f, 0.f);
	}

	if(true == m_bOnLand)
	{
		ClearGravity();
	}

	




	switch (m_eState)
	{
	case BS_WALK:
		Fall(fTime);
		break;
	case BS_TURN:
		break;
	case BS_DIE:
		Die(fTime);
		break;
	case BS_DEAD:
		Dead(fTime);
		break;
	case BS_DASH:
		break;
	case BS_BWALK:
		break;
	case BS_STAND:
		Stand(fTime);
		break;
	case BS_BDASH:
		break;
	case BS_BDIE:
		break;
	case BS_BJUMP:
		break;
	case BS_JUMP:
		// ���� ����
		Jump(fTime);
		break;
	case BS_JUMPB:
		// �׳� ����
		JumpB(fTime);
		break;
	case BS_LAND:
		break;
	case BS_DIELAND:
		break;
	case BS_ATTACK:
		Attack(fTime);
		break;
	case BS_ATTACKA:
		break;
	case BS_ATTACKB:
		AttackB(fTime);
		break;
	case BS_ATTACKC:
		AttackC(fTime);
		break;
	case BS_WAIT:

		break;
	case BS_AATTACK:
		break;
	case BS_BATTACK:
		break;
	case BS_BLOCK:
		break;
	case BS_BLOCKHIT:
		break;
	case BS_GETHIT:
		GetHit(fTime);
		break;
	case BS_GETHIT_LAND:
		Land(fTime);
		break;
	case BS_GETUP:
		GetUp(fTime);
		break;
	case BS_OVER:
		return;
		break;
	default:
		BOOM;
		break;
	}









	int pattern = RandomNumber::GetRandomNumber(1, 3);



}

void FalseKing::Render(float fTime)
{
	Bug::Render(fTime);
}

void FalseKing::MoveX(float fTime)
{
	m_pMovement->AddMovement(GetWorldAxis(AXIS_X) * m_eDir);
}

void FalseKing::Reverse()
{
	Bug::Reverse();
}

void FalseKing::CheckFront()
{
	Bug::CheckFront();
}

void FalseKing::MoveBack(float fTime)
{
	Bug::MoveBack(fTime);
}

void FalseKing::JumpBack(float fTime)
{
	Bug::JumpBack(fTime);
}

void FalseKing::JumpEnd(float fTime)
{
	Bug::JumpEnd(fTime);
}



void FalseKing::SetCurrentState(BUG_STATE eState)
{
	m_eState = eState;
}

void FalseKing::SetAnim(BUG_STATE eState)
{
	if (eState == m_eAnimState)
	{
		return;
	}

	// m_eState = eState;
	m_eAnimState = eState;


	std::string stateName = m_strAniName;
	stateName.append("_");
	stateName.append(m_vecStateName[eState]);

	m_pAnimation->ChangeAnimation(stateName);
}


void FalseKing::OnBlock(CColliderBase * pSrc, CColliderBase * pDest, float fTime)
{
	int a = 0;

	if (nullptr == pDest)
	{
		return;
	}

	if ("PlayerProjectile" == pDest->GetCollisionProfile()->strName)
	{
		MonsterHitEffect* attack = m_pScene->SpawnObject<MonsterHitEffect>(GetWorldPos());
		SAFE_RELEASE(attack);

		HollowKnight* player = (HollowKnight*)(m_pScene->GetGameMode()->GetPlayer());

		m_eMoveBackDir = player->GetDirection();



		int count = RandomNumber::GetRandomNumber(3, 6);
		for (size_t i = 0; i < count; ++i)
		{
			BloodDust* bd = m_pScene->SpawnObject<BloodDust>(GetWorldPos());
			bd->SetDir(m_eMoveBackDir);
			SAFE_RELEASE(bd);
		}
		
		if (true == m_bDead || 0 >= m_iHP)
		{
			return;
		}


		if (BS_GETHIT_IDLE == m_eAnimState && false == m_bDead)
		{
			SetAnim(BS_DAMAGED);
		}

		m_iHP -= 1;
		
		if (m_iHP < 10)
		{
			ResetState();

	
			

		
			if (true == m_bOneChange)
			{
				if (false == m_bLowHP)
				{
					m_bLowHP = true;
				}
				m_iHP = 30;
				SetCurrentState(BS_GETHIT);
				return;
			}

			// �̹����� �Ѿ�����.
			if (false == m_bNoGetHit)
			{
				SetCurrentState(BS_GETHIT);
				m_bNoGetHit = true;
				return;
			}

			if (0 >= m_iHP)
			{
				SetCurrentState(BS_GETHIT);
			}
		}


		if (true == m_bDead)
		{
			return;
		}

		//if (0 >= m_iHP)
		//{
		///*	m_bJump = true;
		//	m_fMoveBackTimeMax = 0.2f;
		//	m_fMoveSpeed = 300.f;
		//	m_pMovement->SetMoveSpeed(m_fMoveSpeed);
		//	m_bLandPhysics = true;
		//	m_bJumping = false;
		//	SetCurrentState(BS_DIE);

		//	m_pBody->SetMonster(false);

		//	m_pRightSencer->Kill();
		//	m_pLeftSencer->Kill();*/
		//	return;
		//}
		//else
		//{
		//	m_bMoveBack = true;
		//}
	}


	if (true == pDest->IsStage())
	{
		int playerPos = (int)pSrc->GetIntersect().z;

		float y;
		float margin = 0.f;

		switch (playerPos)
		{
		case 1: // LEFT
			// m_pMovement->AddMovement(Vector3(pSrc->GetIntersect().x * -2.f, 0.f, 0.f));
			m_bNoRight = true;
			SetWorldPos(GetWorldPos() + Vector3(pSrc->GetIntersect().x * -2.f, 0.f, 0.f));
			// m_bOnLand = false;
			break;
		case 2: // TOP
			y = pDest->GetColliderSectionMax().y + pSrc->GetRelativeScale().y * 0.5f - margin;

			SetWorldPos(Vector3(GetWorldPos().x, y, GetWorldPos().z));

			ClearGravity();
			JumpEnd(fTime);
			m_bOnLand = true;
			m_bNoRight = false;
			m_bNoLeft = false;
			m_pLeftSencer->ClearOverlap();
			m_pRightSencer->ClearOverlap();
			m_pHK->CameraShakeOn();
			break;
		case 3: // RIGHT
			//m_pMovement->AddMovement(Vector3(pSrc->GetIntersect().x * 2.f, 0.f, 0.f));
			m_bNoLeft = true;
			SetWorldPos(GetWorldPos() + Vector3(pSrc->GetIntersect().x * 2.f, 0.f, 0.f));
			// m_bOnLand = false;
			break;
		case 4: // BOTTOM
			m_pMovement->AddMovement(Vector3(0.f, pSrc->GetIntersect().y * -2.f, 0.f));
			// m_bOnLand = false;
			// m_bCeiling = true;
			break;
		default:
			BOOM
				break;
		}

		// �ٿ
		if (true == m_bJump && true == m_bOnLand)
		{
			m_fCurrentForce = m_fCurrentForce * 0.33;
			m_fMoveSpeed = m_fMoveSpeed * 0.33;

			if (m_fCurrentForce <= 10.f)
			{
				SetCurrentState(BS_DEAD);
				m_bJump = false;
				m_bMoveBack = false;
				m_pBody->SetCollisionProfile("Object");
			}
			// �ٽ� �ѹ� ƨ��
			else
			{
				ClearGravity();
				// m_pMovement->AddMovement(Vector3(0.f, pSrc->GetIntersect().y * 2.f, 0.f));

				y = pDest->GetColliderSectionMax().y + pSrc->GetRelativeScale().y * 0.5f + margin;

				SetWorldPos(Vector3(GetWorldPos().x, y, GetWorldPos().z));

				SetForce(m_fCurrentForce);
				m_bJump = true;
				m_bJumping = false;

				if (true == m_bDieLand)
				{
					SetCurrentState(BS_DIELAND);
				}

				return;
			}


		}

		ClearGravity();
		JumpEnd(fTime);


		// m_pMovement->AddMovement(Vector3(0.f, pSrc->GetIntersect().y * 2.f, 0.f));

		return;
	}
}

void FalseKing::ClearState()
{

}

void FalseKing::Stand(float fTime)
{	
	CheckDir();

	SetAnim(BS_STAND);

	m_fTimer += fTime;

	if (m_fTimer >= m_fStandTime)
	{
		m_fTimer = 0.f;

		// ������ ����
		// ������ ������ ���� �߿��� �ϳ�
		// SetCurrentState(BS_JUMPB);

		int pattern = RandomNumber::GetRandomNumber(1, 3);

		if (1 == pattern)
		{
			SetCurrentState(BS_ATTACK);
		}
		else if (2 == pattern)
		{
			SetCurrentState(BS_JUMPB);
		}
		else
		{
			SetCurrentState(BS_JUMP);
		}

		// SetCurrentState(BS_GETHIT);

	}
}

// ���� ����
void FalseKing::Jump(float fTime)
{
	if (false == m_bFKJump)
	{
		CheckDir();
		SetAnim(BS_JUMP);

		m_fForce = 1000.f;
		m_bFKJump = true;

		m_fMoveSpeed = m_fDis;

		// �Ÿ��� ������ ����� ��ȯ
		if (m_fMoveSpeed <= 0)
		{
			m_fMoveSpeed *= -1.f;
		}

		m_fMoveSpeed -= 250.f;

		m_pMovement->SetMoveSpeed(m_fMoveSpeed);

		m_bOnLand = false;
	}

	// �ϰ���
	if (m_fForce <= -1000)
	{
		SetAnim(BS_ATTACKC);

		
	}


	// ����
	if (true == m_bOnLand)
	{
		if (false == m_bEffect)
		{
			MightyEffect* hit = m_pScene->SpawnObject<MightyEffect>(GetWorldPos() + Vector3((250.f * 0.5f + 100.f) * m_eDir, -50.f, 0.f));
			SAFE_RELEASE(hit);

			m_bEffect = true;
		}

		// �����ϸ鼭 ����
		SetCurrentState(BS_STAND);
		m_bEffect = false;
		m_bFKJump = false;
	}
	// ����
	else
	{
		m_pMovement->AddMovement(GetWorldAxis(AXIS_X) * m_eDir);
	}
}

// �׳� ����
void FalseKing::JumpB(float fTime)
{
	if (false == m_bFKJump)
	{
		CheckDir();

		m_fForce = 1000.f;
		m_bFKJump = true;

		m_fMoveSpeed = m_fDis;

		// �Ÿ��� ������ ����� ��ȯ
		if (m_fMoveSpeed <= 0)
		{
			m_fMoveSpeed *= -1.f;
		}

		m_pMovement->SetMoveSpeed(m_fMoveSpeed);

		m_bOnLand = false;
	}



	// ����
	if (true == m_bOnLand)
	{
		SetAnim(BS_LAND);

		if (true == m_pAnimation->IsSequenceEnd())
		{
			SetCurrentState(BS_STAND);
			m_bFKJump = false;
		}
	}
	// ����
	else
	{

		SetAnim(BS_JUMPB);
		m_pMovement->AddMovement(GetWorldAxis(AXIS_X) * m_eDir);
	}
}

// �¾Ƽ� ���ư��� ���´�.
void FalseKing::GetHit(float fTime)
{
	// ������
	SetAnim(BS_GETHIT);

	if (false == m_bGetHit)
	{
		CheckDir();

		m_bGetHit = true;

		m_fForce = 400.f;

		m_fMoveSpeed = 500.f;

		m_bOnLand = false;
	}


	// ������
	if (true == m_pAnimation->IsSequenceEnd())
	{
		m_pMovement->SetMoveSpeed(0.f);
		SetCurrentState(BS_GETHIT_LAND);
		m_bGetHit = false;
		return;
	}

	m_fMoveSpeed -= 100.f * fTime;

	if (m_fMoveSpeed < 0.f)
	{
		m_fMoveSpeed = 0.f;
	}

	m_pMovement->SetMoveSpeed(m_fMoveSpeed);
	m_pMovement->AddMovement(GetWorldAxis(AXIS_X) * m_eDir * -1.f);
}

void FalseKing::Attack(float fTime)
{
	// �غ�
	if (m_iPrep >= 0 && m_iPrep <= 3)
	{
		SetAnim(BS_BATTACK);

		if (m_pAnimation->IsSequenceEnd())
		{
			++m_iPrep;
		}

		return;
	}

	// ������
	SetAnim(BS_ATTACK);
	// MightyEffect* hit = m_pScene->SpawnObject<MightyEffect>(GetWorldPos() + Vector3((250.f * 0.5f + 100.f) * m_eDir, -50.f, 0.f));
	// SAFE_RELEASE(hit);

	m_fTimer += fTime;

	if (0.2f <= m_fTimer)
	{
		
	}
	else
	{
		return;
	}


	if (false == m_bEffect)
	{
		m_pHK->CameraShakeOn();

		MightyEffect* hit = m_pScene->SpawnObject<MightyEffect>(GetWorldPos() + Vector3((250.f * 0.5f + 100.f) * m_eDir, -50.f, 0.f));
		SAFE_RELEASE(hit);

		MightyEffect* hit2 = m_pScene->SpawnObject<MightyEffect>(GetWorldPos() + Vector3((250.f * 0.5f + 100.f) * m_eDir, -50.f, 0.f));
		hit2->SetWorldScale(400.f * 2.5f, 178.f * 2.5f, 1.f);
		SAFE_RELEASE(hit2);

		for (size_t i = 0; i < 13; ++i)
		{
			PowerWave* pw = m_pScene->SpawnObject<PowerWave>(GetWorldPos() + Vector3((250.f * 0.5f + 30.f * i) * m_eDir, -80.f, 0.f));
			pw->SetDir(m_eDir);

			if (12 == i)
			{
				pw->ResetStyle();
			}

			SAFE_RELEASE(pw);
		}

		m_bEffect = true;
	}

	// ���ư���
	if (m_pAnimation->IsSequenceEnd())
	{


		SetCurrentState(BS_STAND);

		// �غ��ڼ��� �ʱ�ȭ
		m_iPrep = 0;
		m_bEffect = false;
		m_fTimer = 0.f;
	}




}

// �������� ��...
void FalseKing::AttackB(float fTime)
{
	if (false == m_bFKJump)
	{
		SetAnim(BS_JUMP);

		m_fForce = 1000.f;
		m_bFKJump = true;

		// �߰����� �̵�
		float fDis = GetWorldPos().x - m_fCenter;
		if (0 > fDis)
		{
			fDis *= -1.f;
		}
		m_fMoveSpeed = fDis;

		m_pMovement->SetMoveSpeed(m_fMoveSpeed);

		m_bOnLand = false;		
	}

	// ����
	if (true == m_bOnLand)
	{
		SetAnim(BS_LAND);

		
		if (true == m_pAnimation->IsSequenceEnd())
		{
			SetCurrentState(BS_ATTACKC);
			m_bFKJump = false;
		}
	}
	// ����
	else
	{
		SetAnim(BS_JUMPB);
		m_pMovement->AddMovement(GetWorldAxis(AXIS_X) * m_eDir);
	}
	
}

// ���
void FalseKing::AttackC(float fTime)
{
	// ������� 2��
	// �غ�
	if (m_iPrep >= 0 && m_iPrep <= 1)
	{
		SetAnim(BS_BATTACK);

		if (m_pAnimation->IsSequenceEnd())
		{
			++m_iPrep;

			
		}

		return;
	}

	// ��������
	// �¿� 4����
	// m_iPrep = 2�� ���̴�.
	int dir = m_iPrep % 2;

	// Ƚ���� ������ ���ư���
	if (10 < m_iPrep)
	{
		m_iPrep = 0;
		SetCurrentState(BS_STAND);
		return;
	}


	// ��ġ�� ȿ�� �߻�
	if (2 == m_pAnimation->GetAnimFrame().iFrame)
	{
		if (false == m_bEffect)
		{
			m_pHK->CameraShakeOn();

			MightyEffect* hit = m_pScene->SpawnObject<MightyEffect>(GetWorldPos() + Vector3((250.f * 0.5f + 100.f) * m_eDir, -50.f, 0.f));
			SAFE_RELEASE(hit);

			MightyEffect* hit2 = m_pScene->SpawnObject<MightyEffect>(GetWorldPos() + Vector3((250.f * 0.5f + 100.f) * m_eDir, -50.f, 0.f));
			hit2->SetWorldScale(400.f * 2.5f, 178.f * 2.5f, 1.f);
			SAFE_RELEASE(hit2);

			m_bEffect = true;

			SetBarrel();
		}
	}





	// ������
	if (0 == dir)
	{
		m_eDir = DIR_RIGHT;
		Flip(1);
		SetAnim(BS_ATTACKB);
		

		if (true == m_pAnimation->IsSequenceEnd())
		{
			++m_iPrep;
			m_bEffect = false;

			return;
		}
	}
	// ����
	else
	{
		Flip(-1);
		m_eDir = DIR_LEFT;
		SetAnim(BS_ATTACKB);

		if (true == m_pAnimation->IsSequenceEnd())
		{
			++m_iPrep;
			m_bEffect = false;
			return;
		}
	}



	// STAND�� ���ư���.
}

void FalseKing::Land(float fTime)
{
	if (false == m_bOpen)
	{
		SetAnim(BS_GETHIT_LAND);
		m_bOpen = true;
		return;
	}

	// �Ѿ����� �ִ� ������ ����
	if (BS_GETHIT_LAND == m_eAnimState && true == m_pAnimation->IsSequenceEnd())
	{
		SetAnim(BS_OPEN);
		return;
	}

	// ���� �������� IDLE
	if (true == m_pAnimation->IsSequenceEnd())
	{
		SetAnim(BS_GETHIT_IDLE);
	}


	m_fOpenTime += fTime;

	// �Ͼ�� ���ư���
	if (m_fOpenTime >= m_fOpenTotalTime)
	{
		if (0 >= m_iHP)
		{
			SetCurrentState(BS_DIE);
		}
		else
		{
			SetCurrentState(BS_GETUP);
		}



		m_fOpenTime = 0.f;
		m_bOpen = false;
		return;
	}



}

void FalseKing::GetUp(float fTime)
{
	SetAnim(BS_GETUP);



	// ���ư���
	if (m_pAnimation->IsSequenceEnd())
	{
		if (true == m_bLowHP)
		{
			SetCurrentState(BS_ATTACKB);
			m_bLowHP = false;

			if (true == m_bOneChange)
			{
				m_iHP = 30;
			}

			m_bOneChange = false;

			return;
		}


		SetCurrentState(BS_STAND);
	}
}

//// GETHIT���� GETUP�� �ϰų� DIE �ϰų�
void FalseKing::Die(float fTime)
{
	

	m_fTimer += fTime;
	m_fTimer2 += fTime;
	m_fTimer3 += fTime;

	int count = 0;
	int countBlob = 0;

	// �𸶹�
	if (m_fTimer3 >= 12.f)
	{
		SetCurrentState(BS_DEAD);
		m_bEffect = false;
		return;
	}
	// ��������
	else if (m_fTimer3 >= 10.f)
	{
		m_pHK->CameraShakeOn();

		SetAnim(BS_DAMAGED);

		if (false == m_bEffect)
		{
			

			for (size_t i = 0; i < 12; ++i)
			{
				int x = RandomNumber::GetRandomNumber(1, 200) - 100;
				int y = RandomNumber::GetRandomNumber(1, 200) - 100;

				BloodDust* bd = m_pScene->SpawnObject<BloodDust>(GetWorldPos() + Vector3((float)x, (float)y, 0.f));
				SAFE_RELEASE(bd);
			}

			for (size_t i = 0; i < 18; ++i)
			{
				int x = RandomNumber::GetRandomNumber(1, 300) - 150;
				int y = RandomNumber::GetRandomNumber(1, 300) - 150;

				Blob* bd = m_pScene->SpawnObject<Blob>(GetWorldPos() + Vector3((float)x, (float)y, 0.f));
				// 0���� ������ �ݴ�� ���ư���.
				if (x <= 0)
				{
					bd->SetDir(-1);
				}
				SAFE_RELEASE(bd);
			}

		}

		m_bEffect = true;
		
		return;
	}
	// ����
	else if (m_fTimer3 >= 8.f)
	{
		SetAnim(BS_LAID);
		return;
	}

	SetAnim(BS_DAMAGED);

	if (m_fTimer >= 0.8f)
	{
		count = RandomNumber::GetRandomNumber(1, 3);
	}

	if (m_fTimer2 >= 1.f)
	{
		// countBlob = RandomNumber::GetRandomNumber(1, 3);
		countBlob = 1;
	}
	

	for (size_t i = 0; i < count; ++i)
	{
		int x = RandomNumber::GetRandomNumber(1, 200) - 100;
		int y = RandomNumber::GetRandomNumber(1, 200) - 100;

		BloodDust* bd = m_pScene->SpawnObject<BloodDust>(GetWorldPos() + Vector3((float)x, (float)y, 0.f));

		SAFE_RELEASE(bd);
	}

	for (size_t i = 0; i < countBlob; ++i)
	{
		int x = RandomNumber::GetRandomNumber(1, 300) - 150;
		int y = RandomNumber::GetRandomNumber(1, 300) - 150;

		Blob* bd = m_pScene->SpawnObject<Blob>(GetWorldPos() + Vector3((float)x, (float)y, 0.f));
		// 150���� ������ �ݴ�� ���ư���.
		if (x <= 0)
		{
			bd->SetDir(-1);
		}
		SAFE_RELEASE(bd);
	}


}

void FalseKing::Dead(float fTime)
{
	if (BS_DIE == m_eAnimState && m_pAnimation->IsSequenceEnd())
	{
		SetAnim(BS_DEAD);

		m_bDead = true;

		SetCurrentState(BS_OVER);

		m_pBody->SetCollisionProfile("Object");
		m_pBody->SetMonster(false);

		return;
	}

	SetAnim(BS_DIE);

	if (false == m_bEffect)
	{
		BabyFace* bf = m_pScene->SpawnObject<BabyFace>(GetWorldPos() + Vector3(100.f * m_iDir, 0.f, 0.f));
		bf->SetDir(m_iDir);
		bf->Flip(m_iDir);
		SAFE_RELEASE(bf);
		// ������ ����
		for (size_t i = 0; i < 12; ++i)
		{
			int x = RandomNumber::GetRandomNumber(1, 300) - 150;
			int y = RandomNumber::GetRandomNumber(1, 300) - 150;

			BloodDust* bd = m_pScene->SpawnObject<BloodDust>(GetWorldPos() + Vector3((float)x, (float)y, 0.f));
			SAFE_RELEASE(bd);
		}

		for (size_t i = 0; i < 12; ++i)
		{
			int x = RandomNumber::GetRandomNumber(1, 300) - 150;
			int y = RandomNumber::GetRandomNumber(1, 300) - 150;

			Blob* bd = m_pScene->SpawnObject<Blob>(GetWorldPos() + Vector3((float)x, (float)y, 0.f));
			// 150���� ������ �ݴ�� ���ư���.
			if (x <= 0)
			{
				bd->SetDir(-1);
			}
			SAFE_RELEASE(bd);
		}

		for (size_t i = 0; i < 25; ++i)
		{
			WhiteBall* wb = m_pScene->SpawnObject<WhiteBall>(GetWorldPos() + Vector3(100.f * m_iDir, 0.f, 0.f));
			wb->SetLeft();
			SAFE_RELEASE(wb);
		}
		for (size_t i = 0; i < 25; ++i)
		{
			WhiteBall* wb = m_pScene->SpawnObject<WhiteBall>(GetWorldPos() + Vector3(100.f * m_iDir, 0.f, 0.f));
			wb->SetLeft();
			SAFE_RELEASE(wb);
		}



		m_bEffect = true;
	}
	


}

// �÷��̾� ������ ���� ����.
void FalseKing::CheckDir()
{
	float x = m_pHK->GetWorldPos().x - GetWorldPos().x;

	m_fDis = x;

	// x�� ������ ����(��)�� ������
	if (x <= 0)
	{
		// �׷��� ������ ���ؾ� �Ѵ�.
		Flip(-1);
		m_eDir = DIR_LEFT;
	}
	else
	{
		Flip(1);
		m_eDir = DIR_RIGHT;
	}

}

void FalseKing::CheckHP()
{
	if (20 >= m_iHP)
	{
		m_iStage1 = true;

	}
	else if (10 >= m_iHP)
	{
		m_iStage2 = true;
	}

}

void FalseKing::SetBarrel()
{
	// �ѹ濡 6����

	for (size_t i = 0; i < 6; ++i)
	{
		int x = RandomNumber::GetRandomNumber(1, 17) - 1;
		int y = RandomNumber::GetRandomNumber(1, 6) - 1;

		Vector3 pos = Vector3(40000.f + 27 * 50.f + 25.f + x * 100.f, -(25.f + y * 100.f), 0.f);

		FireBarrel* fb = m_pScene->SpawnObject<FireBarrel>(pos);
		SAFE_RELEASE(fb);
	}
}

void FalseKing::ResetState()
{

}

void FalseKing::Fall(float fTime)
{
	SetAnim(BS_JUMP);

	if (true == m_bOnLand)
	{
		SetCurrentState(BS_STAND);
	}
}






//// �ȴٰ� �Ǵ� ���ִٰ� �÷��̾ ����

//if (BS_WALK == m_eState)
//{
//	// ���� ���� �ִµ� ���ʿ� �ִ� ���
//	if (true == m_pPlayerLeftSencer->IsPlayer() && DIR_LEFT == m_eDir)
//	{
//		SetCurrentState(BS_BDASH);
//		m_fWalkTime = 0.f;
//		m_fDashTime = 0.f;
//		m_pMovement->SetMoveSpeed(0.f);
//		return;
//	}
//	// ���� ���� �ִµ� �����ʿ� �ִ� ���
//	else if (true == m_pPlayerRightSencer->IsPlayer() && DIR_LEFT == m_eDir)
//	{
//		SetCurrentState(BS_TURN);
//		// ���� �ൿ ����
//		m_fWalkTime = 0.f;
//		m_fDashTime = 0.f;
//		m_bWillDash = true;
//		Reverse();
//		m_eDir = DIR_RIGHT;
//		return;
//	}
//	// ������ ���� �ִµ� ���ʿ� �ִ� ���
//	else if (true == m_pPlayerLeftSencer->IsPlayer() && DIR_RIGHT == m_eDir)
//	{
//		SetCurrentState(BS_TURN);
//		m_fWalkTime = 0.f;
//		m_fDashTime = 0.f;
//		// ���� �ൿ ����
//		m_bWillDash = true;
//		Reverse();
//		m_eDir = DIR_LEFT;
//		return;
//	}
//	// ������ ���� �ִµ� �����ʿ� �ִ� ���
//	else if (true == m_pPlayerRightSencer->IsPlayer() && DIR_RIGHT == m_eDir)
//	{
//		SetCurrentState(BS_BDASH);
//		m_fWalkTime = 0.f;
//		m_fDashTime = 0.f;
//		m_pMovement->SetMoveSpeed(0.f);

//		return;
//	}
//}




//// �غ��� ������ �޸���
//if (BS_BWALK == m_eState && m_pAnimation->IsSequenceEnd())
//{
//	SetCurrentState(BS_DASH);
//	m_pMovement->SetMoveSpeed(500.f);
//	return;
//}

//// �뽬�ϰ� �ִٰ� ���ư���
//if (BS_DASH == m_eState)
//{
//	m_fDashTime += fTime;

//	SetCurrentState(BS_DASH);

//	if (m_fDashTime >= m_fDashTotalTime)
//	{
//		m_fDashTime = 0.f;
//		SetCurrentState(BS_WALK);
//		m_fWalkTime = 0.f;
//		return;
//	}
//}

//// �ȱ� ���ư���
//if (BS_WALK == m_eState)
//{
//	m_fWalkTime += fTime;
//	SetCurrentState(BS_WALK);
//	m_pMovement->SetMoveSpeed(m_fMoveSpeed);

//	// �ȿ����̱�
//	if (m_fWalkTime >= m_fWalkTotalTime)
//	{
//		SetCurrentState(BS_WALK);
//		m_pMovement->SetMoveSpeed(0.f);
//		m_fWalkTime = 0.f;
//		return;
//	}

//	return;
//}

// ���ִٰ� ���ư���
//if (BS_STAND == m_eState)
//{
//	m_fStandTime += fTime;

//	SetCurrentState(BS_STAND);
//	// �ȿ����̱�
//	m_pMovement->SetMoveSpeed(0.f);

//	if (m_fStandTime >= m_fStandTotalTime)
//	{
//		// �ٽ� �����̱�
//		m_fStandTime = 0.f;
//		SetCurrentState(BS_WALK);
//		m_pMovement->SetMoveSpeed(m_fMoveSpeed);
//		return;
//	}
//}

// �ȱⰡ �����ٸ�
// if(BS_WALK == m_eSate)