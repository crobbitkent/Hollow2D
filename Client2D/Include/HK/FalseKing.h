#pragma once

#include "Bug.h"

class FalseKing : public Bug
{
public:
	FalseKing();
	~FalseKing();

	virtual bool Init();
	virtual void Begin();
	virtual void Update(float fTime);
	virtual void Render(float fTime);

	void MoveX(float fTime);
	void Reverse();
	void CheckFront();
	void MoveBack(float fTime);
	void JumpBack(float fTime);

	void JumpEnd(float fTime);


	void RealJump(float fTime);



	void SetCurrentState(BUG_STATE  eState);
	void SetAnim(BUG_STATE eState);

	void OnBlock(class CColliderBase* pSrc, class CColliderBase* pDest, float fTime);

	void ClearState();

	void PatternA(); // ������� -> ������
	void PatternB(); // �׳� ����
	void PatternC(); // ���� ����

	void PatternD(); // ������� -> ��������

	void FindPlayer(float fTime);

	void Stand(float fTime);
	void Jump(float fTime);
	void JumpB(float fTime);
	void GetHit(float fTIme);
	void Attack(float fTime);
	void AttackB(float fTime);
	void AttackC(float fTime);

	void Land(float fTime);
	void GetUp(float fTime);

	void Die(float fTime);
	void Dead(float fTime);

	void CheckDir();
	void CheckHP();

	void SetBarrel();
	void ResetState();

private:
	class HollowKnight* m_pHK = nullptr;
	Vector3 m_vHK;

	class Sencer* m_pPlayerLeftSencer;
	class Sencer* m_pPlayerRightSencer;
	class CColliderRect* m_pSencerBody;

	float m_fTimer = 0.f;
	float m_fTimer2 = 0.f;
	float m_fTimer3 = 0.f;


	float m_fStandTime = 1.5f;
	int m_iPrep = 0; // ������ ���� �غ� ������ ��� �� ���̳� // -1 �̸� �غ� �ǳ� �ڴ�.

	BUG_STATE m_eAnimState = (BUG_STATE)-1;

	// �÷��̾���� �Ÿ�
	float m_fDis = 0.f;
	
	bool m_bFKJump = false;
	bool m_bGetHit = false;

	bool m_bEffect = false;

	bool m_iStage1 = false;
	bool m_iStage2 = false;

	bool m_bOpen = false;
	float m_fOpenTime = 0.f;
	float m_fOpenTotalTime = 7.f;

	// ���������� OPEN, �׸��� �´´�.

	// �߰����� ������ ��Ȳ�� �����.
	float m_fCenter = 46 * 50.f + 25.f;

	bool m_bDead = false;

	// int m_iHP = 30;

	bool m_bLowHP = false;
	bool m_bOneChange = true;

	bool m_bNoGetHit = false;
};

