#ifndef SKILL_H
#define SKILL_H

#include "SkillDef.h"
#include "KNpc.h"
class ISkill
{
public:
    virtual ~ISkill() {};
	virtual int				GetSkillId() = 0;
	virtual const char *	GetSkillName() = 0;
	virtual int				GetSkillStyle() =0;
	virtual void			LoadSkillLevelData(unsigned int  ulLevel, int nParam) = 0;
	virtual BOOL			CanCastSkill  (int nLauncher, int &nParam1, int &nParam2) const= 0;//
	virtual int 			NewCanCastSkill  (int nLauncher, int nParam1, int nParam2) = 0;//
	virtual NPCATTRIB		GetSkillCostType() const= 0;
	virtual int				GetSkillCost(void *) const = 0;
	virtual BOOL			IsTargetOnly()const = 0;
	virtual BOOL			IsTargetEnemy()const = 0;
	virtual BOOL			IsTargetAlly()const = 0;
	virtual BOOL			IsTargetOther()const = 0;
	virtual BOOL			IsTargetObj()const = 0;
	virtual BOOL			IsTargetSelf()const = 0;
	virtual int				GetAttackRadius() const = 0;
	virtual int				ChangeAttackRadius(int nDis) = 0;
	virtual int				getBackAttackRadius() = 0;
	virtual BOOL			IsAura()const{return FALSE;	};
	virtual int 			GetEquiptLimited()const{return -2;};     //����װ�����ƶ���ӿ�
    virtual int			    GetHorseLimited() const{return 0;};      //�����������ƶ���ӿ�
	virtual int			    GetDelayPerCast() const{return 180;};    //���ܽⶳ����ӿ�
	virtual int			    GetHorsePerCast() const{return 180;};    //��������ⶳ����ӿ�
	virtual BOOL			IsPhysical()const{return FALSE;};
	virtual unsigned int	GetSkillLevelUpScriptId()const{return 0;};
	virtual void			DrawSkillIcon(int x, int y, int Width, int Height,int nParam=-1) = 0;
	virtual std::string		getIconPath() = 0;
};

#endif
