#pragma once



#include "Scene/GameMode.h"

class HKMode : public CGameMode
{
public:
	HKMode();
	~HKMode();

	virtual bool Init();
	// �������� ��ȣ, ��ŸƮ����? ������?, ���� HP, ���� ��

	void SetMode(int iStage, bool bStart, int iHP, int iCoin);

private:
	bool CreateMaterial();
	bool CreateAnimation2DSequence();
	void MakePlayerAnimation(const std::string& strName, int iFrameCount, int iFrameSecond, const std::string& strFileName);


	void SetCollision();



	// HOLLOW KNIGHT
	void SetHK();
	void SetBug();
	void SetUI();
	void SetTileMap();
	
	// ����� ��ġ ��Ų��.
	void SetStage(int stage = 6);

	// ���͵��� ��ġ ��Ų��.
	void PlaceMonster();

	void SetObject();

	bool IsDebug() const { return m_bDebug; }
private:
	class HollowKnight* m_pHK = nullptr;
	bool m_bSet = false;

	int m_iCount = 0;
	int m_iStart = 0;

	bool m_bDebug = false;
};

