#pragma once
#include "CommonInclude.h"
#include "playerInfo.h"

class Stat
{
public:
	Stat();
	~Stat() {};

public:
	
	void SetStat(const BaseStat& base, const DerivedStat& derived, int curHp, int curMp, int remainAp);

	void SetBaseStat(const BaseStat& base);
	void SetDerivedStat(const DerivedStat& derived);

	void SetCurHp(int cur_hp) { m_cur_hp = cur_hp; }
	void SetCurMp(int cur_mp) { m_cur_mp = cur_mp; }
	void SetRemainAp(int remainAp) { m_remain_ap = remainAp; }


private:

	/*
		int m_str;
		int m_dex;
		int m_intel;
		int m_luck;
	*/
	BaseStat m_base;

	/*
		int m_maxHP;
		int m_maxMP;
	*/
	DerivedStat m_derived;

	int m_cur_hp;
	int m_cur_mp;
	int m_remain_ap;

	
};

