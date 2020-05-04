#include "JumpBug.h"

#include "EngineGlobals.h"

#include "Resource/Animation2DSequence.h"
#include "Component/Animation2D.h"

#include "Component/ColliderRect.h"
#include "Component/ColliderOBB2D.h"

#include "TimerManager.h"

#include "Sencer.h"
#include "MonsterHitEffect.h"
#include "HollowKnight.h"
#include "../RandomNumber.h"
#include "Coin.h"

#include "Blob.h"
#include "BloodDust.h"
#include "HitOrange.h"

JumpBug::JumpBug()
{
}


JumpBug::~JumpBug()
{
	SAFE_RELEASE(m_pPlayerLeftSencer);
	SAFE_RELEASE(m_pPlayerRightSencer);

}

bool JumpBug::Init()
{
	if (false == Bug::Init())
	{
		BOOM
			return false;
	}

	Bug::SetAnimation("JB");
	m_pMesh->SetPivot(0.5f, 0.42f, 0.f);

	// �÷��̾� ã������ ����
	m_pPlayerLeftSencer = m_pScene->SpawnObject<Sencer>();
	m_pSencerBody = m_pPlayerLeftSencer->GetBody();
	m_pMesh->AddChild(m_pSencerBody, TR_POS);
	SAFE_RELEASE(m_pSencerBody);


	m_pPlayerRightSencer = m_pScene->SpawnObject<Sencer>();
	m_pSencerBody = m_pPlayerRightSencer->GetBody();

	m_pMesh->AddChild(m_pSencerBody, TR_POS);
	SAFE_RELEASE(m_pSencerBody);

	m_fMoveSpeed = 100.f;

	// m_pPlayerLeftSencer->SetRelativePos(-300.f, 0.f, 0.f);
	// m_pPlayerRightSencer->SetRelativePos(300.f, 0.f, 0.f);

	m_fGravitySpeed = 16.f;

	m_bDieLand = true;

	m_pBody->AddBlockCallback<JumpBug>(this, &JumpBug::OnBlock);

	return true;
}

void JumpBug::Begin()
{
	Bug::Begin();

	m_pMovement->SetMoveSpeed(m_fMoveSpeed);

	Bug::TurnOff();
}

void JumpBug::Update(float fTime)
{
	CGameObject::Update(fTime);

	// char	strText[256] = {};
	// sprintf_s(strText, "LEFT = %d, RIGHT = %d\n", m_pLeftSencer->IsOverlap(), m_pRightSencer->IsOverlap());
	// OutputDebugStringA(strText);


	if (true == m_bOnLand)
	{
		ClearGravity();
	}
	else
	{

	}

	bool anim = m_pAnimation->IsSequenceEnd();

	if (true == m_bDead)
	{
		return;
	}


	if (BS_DEAD == m_eState && true == anim)
	{
		m_bDead = true;

		return;
	}

	////////////////////////////////////////////////////


	if (BS_TURN == m_eState && true == m_pAnimation->IsSequenceEnd())
	{
		// Reverse();
		m_bChildUpdate = true;

		SetCurrentState(BS_WALK);

		return;
	}




	





	// �׾�����
	if (true == m_bJump && 0 >= m_iHP)
	{
		if (false == m_bOnLand)
		{
			bool leftFree = m_pLeftSencer->IsOverlap();
			bool rightFree = m_pRightSencer->IsOverlap();

			// �����߿� �ٴڿ�
			if (false == leftFree && false == rightFree)
			{
				if (DIR_LEFT == m_eDir)
				{
					// �������� ���� �ִµ� ���� ������...
					if (true == leftFree || true == m_bNoLeft)
					{
						// Reverse();
						m_eDir = DIR_RIGHT;
						m_eMoveBackDir = DIR_RIGHT;
						m_bNoLeft = false;
						m_pLeftSencer->ClearOverlap();
						m_pRightSencer->ClearOverlap();


					}
				}
				else if (DIR_RIGHT == m_eDir)
				{
					// ���������� ���� �ִµ� ���� ������...
					if (true == rightFree || true == m_bNoRight)
					{
						// Reverse();
						m_eDir = DIR_LEFT;
						m_eMoveBackDir = DIR_LEFT;
						m_bNoRight = false;
						m_pLeftSencer->ClearOverlap();
						m_pRightSencer->ClearOverlap();

					}
				}
			}
			// �������ε� �������� �ɷȴ�.
			else if (true == leftFree && false == rightFree)
			{
				if (true == m_bNoRight)
				{
					// Reverse();
					m_eDir = DIR_LEFT;
					m_eMoveBackDir = DIR_LEFT;
					m_bNoRight = false;
					m_pLeftSencer->ClearOverlap();
					m_pRightSencer->ClearOverlap();

				}
			}
			else if (false == leftFree && true == rightFree)
			{
				if (true == m_bNoLeft)
				{
					// Reverse();
					m_eDir = DIR_RIGHT;
					m_eMoveBackDir = DIR_RIGHT;
					m_bNoLeft = false;
					m_pLeftSencer->ClearOverlap();
					m_pRightSencer->ClearOverlap();
				}
				else if (true == m_bNoRight)
				{
					// Reverse();
					m_eDir = DIR_LEFT;
					m_eMoveBackDir = DIR_LEFT;
					m_bNoRight = false;
					m_pLeftSencer->ClearOverlap();
					m_pRightSencer->ClearOverlap();
				}

			}
		}

		JumpBack(fTime);
	}
	// ���׾�����
	else if (BS_DEAD != m_eState)
	{
		if (true == m_bOnLand && false == m_bLandPhysics)
		{
			CheckFront();
		}
		else
		{
			// CheckCollision();
		}


		if (true == m_bMoveBack)
		{
			MoveBack(fTime);
		}
		else
		{
			MoveX(fTime);
		}
	}


	// m_pMovement->AddRotationZ(180.f * fTime * rotationNumber);


	if (true == m_bWillJump)
	{
		int a = 0;
	}

	// DEBUG
	// char	strText[256] = {};
	// sprintf_s(strText, (m_vecStateName[m_eState] + "\n").c_str());
	// OutputDebugStringA(strText);

	// ���ο��� ����
	m_bSlowAdjust = GET_SINGLE(CTimerManager)->IsSlowMotion();
	if (true == m_bSlowAdjust)
	{
		if (false == m_bSlowDone)
		{
			m_fMoveSpeed *= 2.f;
			m_bSlowDone = true;
		}		
	}
	else
	{
		if (true == m_bSlowDone)
		{
			m_fMoveSpeed *= 0.5f;
			m_bSlowDone = false;
		}
	}
	

	// ����
	if (true == m_bDead || 0 >= m_iHP)
	{
		m_pMesh->SetPivot(0.5f, 0.54f, 0.f);
		return;
	}


	if (BS_STAND == m_eState)
	{
		if (true == m_pAnimation->IsSequenceEnd())
		{
			SetCurrentState(BS_WALK);
			return;
		}
		else
		{
			return;
		}

	}

	// ���Ⱑ ������
	if (BS_TURN == m_eState && true == m_pAnimation->IsSequenceEnd())
	{
		// �����ϱ� ���� ���� �ƴ� ��츸
		if (false == m_bWillJump)
		{
			SetCurrentState(BS_WALK);
			m_bWillJump = false;
			return;
		}
		else
		{
			SetCurrentState(BS_BJUMP);
			RealJump(fTime);
			m_bWillJump = true;
			return;
		}

	}


	//////////////////////////////////////////////////////////////////////////////////////////

	// ����
	if (BS_BJUMP == m_eState && true == m_pAnimation->IsSequenceEnd())
	{
		SetCurrentState(BS_JUMP);
		return;
	}	

	// ������ �ȱ�
	if (BS_ATTACK == m_eState && true == m_pAnimation->IsSequenceEnd())
	{
		SetCurrentState(BS_STAND);
		return;
	}

	//// ���ư��� �������� �ƹ��͵� ���Ѵ�.
	if (false == m_bOnLand || BS_ATTACK == m_eState)
	{
		return;
	}

	// ����
	if (true == m_bIsJumping)
	{
		if (true == m_bOnLand)
		{
			m_bIsJumping = false;
			SetCurrentState(BS_ATTACK);
			m_fMoveSpeed = 100.f;
			m_pMovement->SetMoveSpeed(m_fMoveSpeed);
			m_bWillJump = false;

			return;
		}

		Bug::CheckCollision();

		if (true == m_bNoLeft && DIR_LEFT == m_eDir)
		{
			return;
		}
		else if (true == m_bNoRight && DIR_RIGHT == m_eDir)
		{
			return;
		}

		m_pMovement->AddMovement(GetWorldAxis(AXIS_X) * m_eDir);

		return;
	}

	

	
	FindPlayer(fTime);
	
	








}

void JumpBug::Render(float fTime)
{
	Bug::Render(fTime);
}

void JumpBug::MoveX(float fTime)
{
	Bug::MoveX(fTime);
}

void JumpBug::Reverse()
{
	Bug::Reverse();
}

void JumpBug::CheckFront()
{
	Bug::CheckFront();
}

void JumpBug::MoveBack(float fTime)
{
	Bug::MoveBack(fTime);
}

void JumpBug::JumpBack(float fTime)
{
	Bug::JumpBack(fTime);
}

void JumpBug::JumpEnd(float fTime)
{
	Bug::JumpEnd(fTime);
}



void JumpBug::RealJump(float fTime)
{
	// parent
	m_bJumping = true;
	m_bOnLand = false;


	// JB
	m_bIsJumping = true;

	m_fForce = 600.f;

	m_fMoveSpeed = m_fDisX;

	if (m_fMoveSpeed <= 0)
	{
		m_fMoveSpeed *= -1.f;
	}

	m_pMovement->SetMoveSpeed(m_fMoveSpeed);

}

void JumpBug::SetCurrentState(BUG_STATE eState)
{
	Bug::SetCurrentState(eState);
}


void JumpBug::OnBlock(CColliderBase * pSrc, CColliderBase * pDest, float fTime)
{
	int a = 0;

	if (nullptr == pDest)
	{
		return;
	}

	if ("PlayerProjectile" == pDest->GetCollisionProfile()->strName)
	{
		if (true == m_bDead)
		{
			return;
		}

		MonsterHitEffect* attack = m_pScene->SpawnObject<MonsterHitEffect>(GetWorldPos());
		SAFE_RELEASE(attack);






		HollowKnight* player = (HollowKnight*)(m_pScene->GetGameMode()->GetPlayer());

		m_eMoveBackDir = player->GetDirection();

		for (size_t i = 0; i < 6; ++i)
		{
			int x = RandomNumber::GetRandomNumber(1, 200) - 100;
			int y = RandomNumber::GetRandomNumber(1, 200) - 100;

			BloodDust* bd = m_pScene->SpawnObject<BloodDust>(GetWorldPos() + Vector3((float)x, (float)y, 0.f));
			bd->SetNormalMonster();
			bd->SetDir(m_eMoveBackDir);
			SAFE_RELEASE(bd);
		}

		for (size_t i = 0; i < 6; ++i)
		{
			int x = RandomNumber::GetRandomNumber(1, 200) - 100;
			int y = RandomNumber::GetRandomNumber(1, 200) - 100;

			Blob* bd = m_pScene->SpawnObject<Blob>(GetWorldPos() + Vector3((float)x, (float)y, 0.f));
			bd->SetNormalMonster();
			// 0���� ������ �ݴ�� ���ư���.

			bd->SetDir(m_eMoveBackDir);

			SAFE_RELEASE(bd);
		}

		m_iHP -= 1;

		

		if (0 >= m_iHP)
		{
			m_bJump = true;
			m_fMoveBackTimeMax = 0.2f;
			m_fMoveSpeed = 300.f;
			m_pMovement->SetMoveSpeed(m_fMoveSpeed);
			m_bLandPhysics = true;
			m_bJumping = false;
			SetCurrentState(BS_DIE);

			m_pBody->SetMonster(false);

			m_pRightSencer->Kill();
			m_pLeftSencer->Kill();

			HitOrange* ho1 = m_pScene->SpawnObject<HitOrange>(pDest->GetIntersect());
			ho1->SetBinding();
			SAFE_RELEASE(ho1);

			HitOrange* ho = m_pScene->SpawnObject<HitOrange>(pDest->GetIntersect());
			SAFE_RELEASE(ho);

			int count = RandomNumber::GetRandomNumber(1, 3);

			for (size_t i = 0; i < count; ++i)
			{
				Coin* coin = m_pScene->SpawnObject<Coin>(GetWorldPos());
				SAFE_RELEASE(coin);
			}

			return;
		}
		else
		{
			m_bMoveBack = true;

			HitOrange* ho = m_pScene->SpawnObject<HitOrange>(pDest->GetIntersect());
			SAFE_RELEASE(ho);

			BloodDust* bd = m_pScene->SpawnObject<BloodDust>(GetWorldPos());
			bd->SetNormalMonster();
			SAFE_RELEASE(bd);
		}
	}


	if (true == pDest->IsStage())
	{
		int playerPos = (int)pSrc->GetIntersect().z;

		float x;
		float y;
		float margin = -1.f;

		switch (playerPos)
		{
		case 1: // LEFT
			m_pMovement->AddMovement(Vector3(pSrc->GetIntersect().x * -2.f, 0.f, 0.f));

			x = pDest->GetColliderSectionMin().x - pSrc->GetRelativeScale().x * 0.5f - margin;
			SetWorldPos(Vector3(x, GetWorldPos().y, GetWorldPos().z));

			m_bNoRight = true;
			// m_bOnLand = false;
			break;
		case 2: // TOP

			ClearGravity();
			JumpEnd(fTime);
			m_bOnLand = true;
			m_bNoRight = false;
			m_bNoLeft = false;
			m_pLeftSencer->ClearOverlap();
			m_pRightSencer->ClearOverlap();

			y = pDest->GetColliderSectionMax().y + pSrc->GetRelativeScale().y * 0.5f - margin;

			SetWorldPos(Vector3(GetWorldPos().x, y, GetWorldPos().z));

			break;
		case 3: // RIGHT
			x = pDest->GetColliderSectionMax().x + pSrc->GetRelativeScale().x * 0.5f + margin;

			SetWorldPos(Vector3(x, GetWorldPos().y, GetWorldPos().z));


			m_bNoLeft = true;
			// m_bOnLand = false;
			break;
		case 4: // BOTTOM
			m_pMovement->AddMovement(Vector3(0.f, pSrc->GetIntersect().y * -2.f, 0.f));
			ClearGravity();
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

				float y = pDest->GetColliderSectionMax().y + pSrc->GetRelativeScale().y * 0.5f + 5.f;

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

void JumpBug::ClearState()
{

}

void JumpBug::FindPlayer(float fTime)
{
	// �÷��̾� ã��
	if (false == m_bWillJump)
	{
		Vector3 playerVec = m_pScene->GetGameMode()->GetPlayer()->GetWorldPos();
		Vector3 monsterVec = GetWorldPos();

		m_fDisX = playerVec.x - monsterVec.x;
		float disY = playerVec.y - monsterVec.y;

		// �÷��̾�� ������ �Ÿ��� ���Ʒ� 250 ����
		if (disY <= 250.f && disY >= -250.f)
		{
			if (m_fDisX <= 400.f && m_fDisX >= -400.f)
			{
				if (m_fDisX <= 400.f && m_fDisX >= 0.f)
				{
					// �÷��̾ ������
					// �ٵ� ���� ����
					if (DIR_LEFT == m_eDir)
					{
						SetCurrentState(BS_TURN);
						Reverse();
						m_eDir = DIR_RIGHT;
						m_bWillJump = true;
						return;
					}
					// ���� ������
					else
					{
						SetCurrentState(BS_BJUMP);
						RealJump(fTime);
						return;
					}
				}
				else
				{
					// �÷��̾ ����
					// ���� ����
					if (DIR_LEFT == m_eDir)
					{
						SetCurrentState(BS_BJUMP);
						RealJump(fTime);
						return;
					}
					else
					{
						SetCurrentState(BS_TURN);
						Reverse();
						m_eDir = DIR_LEFT;
						m_bWillJump = true;
						return;
					}
				}


			}
		}
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