#include "SmallBug.h"

#include "EngineGlobals.h"

#include "Resource/Animation2DSequence.h"
#include "Component/Animation2D.h"

#include "Component/ColliderRect.h"
#include "Component/ColliderOBB2D.h"

#include "MonsterHitEffect.h"
#include "HollowKnight.h"
#include "Sencer.h"

SmallBug::SmallBug()
{
}


SmallBug::~SmallBug()
{
	SAFE_RELEASE(m_pRTSencer);
	SAFE_RELEASE(m_pLTSencer);
}

bool SmallBug::Init()
{
	if (false == Bug::Init())
	{
		BOOM
			return false;
	}

	Bug::SetAnimation("SMB");

	m_pMesh->SetPivot(0.5f, 0.56f, 0.f);

	m_pBody->AddBlockCallback<SmallBug>(this, &SmallBug::OnBlock);
	m_pBody->SetExtent(100.f, 100.f);

	SetPhysics(true);

	m_eDir = DIR_RIGHT;
	m_eNewDir = DIR_RIGHT;

	// 센서 장착
	// RIGHT
	m_pRTSencer = m_pScene->SpawnObject<Sencer>();
	m_pRTSencer->SetCheck(4);
	CColliderRect* pSencerBody = m_pRTSencer->GetBody();
	m_pMesh->AddChild(pSencerBody, TR_POS);

	pSencerBody->SetRelativePos((m_pBody->GetExtent().x * 0.5f + 1),  (m_pBody->GetExtent().y * 0.5f + 1), 0.f);

	// LEFT
	m_pLTSencer = m_pScene->SpawnObject<Sencer>();
	m_pLTSencer->SetCheck(3);
	pSencerBody = m_pLTSencer->GetBody();
	m_pMesh->AddChild(pSencerBody, TR_POS);

	pSencerBody->SetRelativePos(-(m_pBody->GetExtent().x * 0.5f + 1), (m_pBody->GetExtent().y * 0.5f + 1), 0.f);

	// RIGHT
	pSencerBody = m_pRightSencer->GetBody();
	m_pMesh->AddChild(pSencerBody, TR_POS);

	pSencerBody->SetRelativePos((m_pBody->GetExtent().x * 0.5f + 1), -(m_pBody->GetExtent().y * 0.5f + 1), 0.f);

	// LEFT
	pSencerBody = m_pLeftSencer->GetBody();
	m_pMesh->AddChild(pSencerBody, TR_POS);

	pSencerBody->SetRelativePos(-(m_pBody->GetExtent().x * 0.5f + 1), -(m_pBody->GetExtent().y * 0.5f + 1), 0.f);













	m_pBody->SetMonster(true);

	

	m_bParentUpdate = false;

	return true;
}

void SmallBug::Begin()
{
	Bug::Begin();
}

void SmallBug::Update(float fTime)
{
	Bug::Update(fTime);

	// 죽을 때 날아가다가 땅바닥에 ...
	if (true == m_bOnLand)
	{
		ClearGravity();
	}

	bool anim = m_pAnimation->IsSequenceEnd();

	// 죽음
	if (true == m_bDead)
	{
		return;
	}
	
	if (BS_DEAD == m_eState && true == anim)
	{
		m_bDead = true;

		return;
	}


	char	strText[256] = {};
	sprintf_s(strText, "%.4f\n", GetWorldRot().z);
	OutputDebugStringA(strText);


	///////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////

	if(true == m_bStop)
	{ 
		m_fStopTime += fTime;

		if (m_fStopTime >= m_fStopMaxTime)
		{
			m_fStopTime = 0;
			m_bStop = false;

		}
	
		return;
	}

	if (true == m_bTurn)
	{
		Turn(fTime);
		m_pMovement->SetMoveSpeed(140.f);
		MoveX(fTime);
		return;
	}
	else
	{
		m_pMovement->SetMoveSpeed(150.f);
		MoveX(fTime);
	}





	if (true == m_bJump)
	{
		if (false == m_bOnLand)
		{
			bool leftFree = m_pLeftSencer->IsOverlap();
			bool rightFree = m_pRightSencer->IsOverlap();

			// 점프중에 바닥에
			if (false == leftFree && false == rightFree)
			{
				if (DIR_LEFT == m_eDir)
				{
					// 왼쪽으로 가고 있는데 왼쪽 센서가...
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
					// 오른쪽으로 가고 있는데 왼쪽 센서가...
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
			// 점프중인데 오른쪽이 걸렸다.
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
			}
		}

		JumpBack(fTime);
	}
	else if (BS_DEAD != m_eState)
	{
		if (true == m_bOnLand && false == m_bLandPhysics)
		{
			CheckFront();
		}
		else
		{
			CheckCollision();
		}	
	}








}

void SmallBug::Render(float fTime)
{
	Bug::Render(fTime);
}

void SmallBug::MoveX(float fTime)
{
	// Bug::MoveX(fTime);


	m_pMovement->AddMovement(GetWorldAxis(AXIS_X) * 1.f);
	

}

void SmallBug::MoveY(float fTime)
{

}

void SmallBug::Reverse()
{

}


// 항상 오른쪽으로 돌자
void SmallBug::CheckFront()
{
	if (DIR_LEFT == m_eDir)
	{
		// 왼쪽으로 가고 있는데 왼쪽이 절벽이면 위로
		if (true == m_pLTSencer->IsOverlap())
		{
			m_eNewDir = DIR_UP;
		}
		// 왼쪽으로 가고 있는데 왼쪽이 벽이면 아래로
		else if (true == m_bNoLeft)
		{			
			m_eNewDir = DIR_RIGHT;
		}
	}

	else if (DIR_RIGHT == m_eDir)
	{
		// 오른쪽으로 가고 있는데 오른쪽이 절벽이면 아래로
		if (true == m_pRightSencer->IsOverlap())
		{
			m_eNewDir = DIR_BOT;
		}
		// 오른쪽으로 가고 있는데 오른쪽이 벽이면 위로
		else if (true == m_bNoRight)
		{
			m_eNewDir = DIR_UP;
		}
	}
	else if (DIR_UP == m_eDir)
	{
		// 오른쪽으로 가고 있는데 오른쪽이 절벽이면 아래로
		if (true == m_pRTSencer->IsOverlap())
		{
			m_eNewDir = DIR_RIGHT;
		}
		// 오른쪽으로 가고 있는데 오른쪽이 벽이면 위로
		else if (true == m_bNoUp && false == m_pLTSencer->IsOverlap())
		{
			m_eNewDir = DIR_LEFT;
		}
	}
	else if (DIR_BOT == m_eDir)
	{
		// 오른쪽으로 가고 있는데 오른쪽이 절벽이면 아래로
		if (true == m_pLeftSencer->IsOverlap())
		{
			m_eNewDir = DIR_LEFT;
		}
		// 오른쪽으로 가고 있는데 오른쪽이 벽이면 위로
		else if (true == m_bNoBot && false == m_pRightSencer->IsOverlap())
		{
			m_eNewDir = DIR_RIGHT;
		}
	}
	
	if(m_eDir == m_eNewDir)
	{
		m_bTurn = false;
		return;
	}

	m_bTurn = true;

	/*m_pLeftSencer	  ->ClearOverlap();
	m_pRightSencer	  ->ClearOverlap();
	m_pRTSencer		  ->ClearOverlap();
	m_pLTSencer		  ->ClearOverlap();

	m_bNoLeft = false;
	m_bNoRight = false;
	m_bNoUp = false;
	m_bNoBot = false;*/
}

void SmallBug::MoveBack(float fTime)
{
	Bug::MoveBack(fTime);
}

void SmallBug::JumpBack(float fTime)
{
	Bug::JumpBack(fTime);
}

void SmallBug::JumpEnd(float fTime)
{
	Bug::JumpEnd(fTime);
}

void SmallBug::Stop(float fTime)
{

}

void SmallBug::Turn(float fTime)
{
	float z = GetWorldRot().z;


	switch (m_eDir)
	{
	case DIR_BOT:
		// 왼쪽 또는 오른쪽			
	
		if (DIR_LEFT == m_eNewDir)
		{
			AddRelativeRotationZ(-90.f * fTime);
			if (z <= -180.f)
			{
				SetWorldRotationZ(-180.f);
				FinishTurning();
				return;
			}
		}
		else
		{
			m_pMovement->AddRotationZ(-90.f * fTime);
		}
		break;
	case DIR_UP:
		// 왼쪽 또는 오른쪽
		if (DIR_LEFT == m_eNewDir)
		{
			AddRelativeRotationZ(-90.f * fTime);
			if (z <= -360.f)
			{
				SetWorldRotationZ(0.f);
				FinishTurning();
				return;
			}



		}
		else
		{
			AddRelativeRotationZ(-90.f * fTime);
			if (z <= -360.f)
			{
				SetWorldRotationZ(0.f);
				FinishTurning();
				return;
			}



		}
		break;
	case DIR_LEFT:
		// 위 도는 아래



		if (DIR_UP == m_eNewDir)
		{
			AddRelativeRotationZ(-90.f * fTime);
			if (z <= -270.f)
			{
				SetWorldRotationZ(-270.f);
				FinishTurning();
				return;
			}

		}
		else
		{
			m_pMovement->AddRotationZ(-90.f * fTime);
		}
		break;
	case DIR_RIGHT:
		// 위 또는 아래
		if (DIR_UP == m_eNewDir)
		{
			m_pMovement->AddRotationZ(-90.f * fTime);
		}
		else
		{
			AddRelativeRotationZ(-90.f * fTime);

			if (z <= -90.f)
			{
				SetWorldRotationZ(-90.f);
				FinishTurning();
				return;
			}


		}
		break;
	default:
		BOOM;
		break;
	}
}

void SmallBug::SetCurrentState(BUG_STATE eState)
{
	Bug::SetCurrentState(eState);
}

void SmallBug::OnBlock(CColliderBase * pSrc, CColliderBase * pDest, float fTime)
{
	// Bug::OnBlock(pSrc, pDest, fTime);

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

		m_iHP -= 1;

		if (true == m_bDead)
		{
			return;
		}

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
			return;
		}
		else
		{
			m_bMoveBack = true;
		}
	}


	if (true == pDest->IsStage())
	{
	//	SetPhysics(false);

		int playerPos = (int)pSrc->GetIntersect().z;

		switch (playerPos)
		{
		case 1: // LEFT
			m_pMovement->AddMovement(Vector3(pSrc->GetIntersect().x * -2.f, 0.f, 0.f));
			m_bNoRight = true;
			m_bOnLand = false;
			break;
		case 2: // TOP
			m_pMovement->AddMovement(Vector3(0.f, pSrc->GetIntersect().y * 2.f, 0.f));
			ClearGravity();
			JumpEnd(fTime);
			m_bOnLand = true;
			m_bNoRight = false;
			m_bNoLeft = false;
			m_bNoUp = true;
			m_bOnLand = true;
			break;
		case 3: // RIGHT
			m_pMovement->AddMovement(Vector3(pSrc->GetIntersect().x * 2.f, 0.f, 0.f));
			m_bNoLeft = true;
			m_bOnLand = false;
			break;
		case 4: // BOTTOM
			m_pMovement->AddMovement(Vector3(0.f, pSrc->GetIntersect().y * -2.f, 0.f));
			m_bOnLand = false;
			m_bNoBot = true;
			// m_bCeiling = true;
			break;
		default:
			BOOM
				break;
		}

		// 바운스
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
			// 다시 한번 튕김
			else
			{
				ClearGravity();
				// m_pMovement->AddMovement(Vector3(0.f, pSrc->GetIntersect().y * 2.f, 0.f));

				float y = pDest->GetColliderSectionMax().y + pSrc->GetRelativeScale().y * 0.5f + 5.f;

				SetWorldPos(Vector3(GetWorldPos().x, y, GetWorldPos().z));

				SetForce(m_fCurrentForce);
				m_bJump = true;
				m_bJumping = false;

				return;
			}


		}

		ClearGravity();
		JumpEnd(fTime);


		// m_pMovement->AddMovement(Vector3(0.f, pSrc->GetIntersect().y * 2.f, 0.f));

		return;
	}
}

void SmallBug::FinishTurning()
{
	m_bTurn = false;
	m_eDir = m_eNewDir;
}
