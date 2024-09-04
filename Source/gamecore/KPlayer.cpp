#include "KCore.h"
#ifndef WM_MOUSEHOVER
#define WM_MOUSEHOVER 0x02A1
#endif
#include "engine/KEngine.h"
//#include "../../Headers/IClient.h"
//#include "Scene/KScenePlaceC.h"
//#include "KIme.h"
#include "KNpcAI.h"
#include "KNpc.h"
#include "KObj.h"
#include "KNpcSet.h"
#include "KSubWorld.h"
#include "KPlayer.h"
#include "engine/Text.h"
#include "LuaFuns.h"
#include "KSortScript.h"
//#include "KScriptValueSet.h"
#include "KObjSet.h"
#include "KSkills.h"
#include "KPlayerSet.h"
#include "KSubWorldSet.h"
#include "engine/KFile.h"
#include "GameDataDef.h"
#include "KBuySell.h"
//#include "MyAssert.h"
#include "MsgGenreDef.h"
#include "KItemSet.h"
#include "KTaskFuns.h"
#include "engine/Text.h"
#include "KSellItem.h"
//#include "KForBitGua.h"
#include  <map>
#include  "CoreShell.h"
extern int LuaGetNpcTalk(Lua_State *L);
#define		defPLAYER_LOGIN_TIMEOUT			20 * 20   // 10 sec
#define		PLAYER_LEVEL_1_EXP				48
#define		PLAYER_LEVEL_ADD_ATTRIBUTE		5
#define		PLAYER_LEVEL_ADD_SKILL			1
#define		PLAYER_SHARE_EXP_DISTANCE		768
#define		PLAYER_TEAM_EXP_ADD				50

#define		MAX_APPLY_TEAM_TIME				500

#define		BASE_WALK_SPEED					5    //����
#define		BASE_RUN_SPEED					10   //����

#define		BASE_ATTACK_SPEED				0       //�����Ĺ����ٶ�
#define		BASE_CAST_SPEED					0       //�����ĳ����ٶ�
#define		BASE_VISION_RADIUS				120     //
#define		BASE_HIT_RECOVER				6

#define		TOWN_PORTAL_TIME				1800    //�س�ʱ��


KPlayer	*Player = NULL;//Player[MAX_PLAYER];
int		g_nLastNetMsgLoop;

//-------------------------------------------------------------------------
//	���ܣ����캯��
//-------------------------------------------------------------------------
KPlayer::KPlayer()
{
	Release();
}

//-------------------------------------------------------------------------
//	���ܣ���������
//-------------------------------------------------------------------------
KPlayer::~KPlayer()
{
	Release();
}

//-------------------------------------------------------------------------
//	���ܣ����  �˳���Ϸʱ��
//-------------------------------------------------------------------------
void	KPlayer::Release()
{
	m_isopenjuli   = false;
	m_nLeftSkillID = 0;
	m_nLeftListidx = 0;
	m_nLeftSkillLevel = 0;
	m_nRightSkillID = 0;
	m_nRightListidx = 0;
	m_nRightSkillLevel = 0;

	m_DebugMode=FALSE;
	m_RunStatus = 0;
	m_dwNextLevelLeadExp = 0;
	/*m_nLeftSkillID = 0;
	m_nLeftSkillLevel = 0;
	m_nRightSkillID = 0;
	m_nRightSkillLevel = 0;*/
	m_nSendMoveFrames = defMAX_PLAYER_SEND_MOVE_FRAME;
	m_MouseDown[0] = FALSE;
	m_MouseDown[1] = FALSE;
	m_nImageNpcID  =0;
	m_ItemLinkDwid =0;
	//ZeroMemory(szClienResult,sizeof(szClienResult));

	m_MissionData.Clear();
	memset(m_MissionRank, 0, sizeof(m_MissionRank));

	m_nCurPinDao=-1;

	m_GameLeftTime = 0;
	m_Isinvalid=FALSE;
	m_dwID = 0;
	m_nIndex = 0;
	m_nNetConnectIdx = -1;
	m_cMenuState.Release();
	m_cTrade.Release();
	m_nAttributePoint = 0;
	m_nSkillPoint = 0;

	m_nStrength = 0;
	m_nDexterity = 0;
	m_nVitality = 0;
	m_nEngergy = 0;
	m_nLucky = 0;
	m_nCurStrength = 0;
	m_nCurDexterity = 0;
	m_nCurVitality = 0;
	m_nCurEngergy = 0;
	m_nCurLucky = 0;
	m_nTempLucky_p=0;
	m_btChatSpecialChannel = 0;
	m_nUpExp = 0;
	
	m_nExp = 0;
	m_nNextLevelExp = PLAYER_LEVEL_1_EXP;
	m_bExchangeServer = FALSE;
	m_CUnlocked = 0;  //Ĭ�Ͻ���
	
	m_dwLeadExp = 0;
	m_dwLeadLevel = 1;
	
	m_nPeapleIdx = 0;
	m_nObjectIdx = 0;
//	m_bWaitingPlayerFeedBack = false;
//	m_btTryExecuteScriptTimes = 0;	//
    m_nForbiddenFlag = 0; //Ĭ���ǽ������
	m_nExtPoint = 0;

	m_ExpTime= (g_SubWorldSet.m_nLoopRate)/(18*60);
	m_ExpPiontTime= (g_SubWorldSet.m_nLoopRate)/(18*60);
	m_YinLiangTime= (g_SubWorldSet.m_nLoopRate)/(18*60);
	m_JinBiTime= (g_SubWorldSet.m_nLoopRate)/(18*60);

}
//-------------------------------------------------------------------------
//	���ܣ��趨 m_nPlayerIndex (������ֻ������PlayerSet��Init�е���)
//-------------------------------------------------------------------------
void	KPlayer::SetPlayerIndex(int nNo)
{
	if (nNo < 0)
		m_nPlayerIndex = 0;
	else
		m_nPlayerIndex = nNo;
}

//-------------------------------------------------------------------------
//	���ܣ���ñ�ʵ���� Player �����е�λ��
//-------------------------------------------------------------------------
int		KPlayer::GetPlayerIndex()
{
	return m_nPlayerIndex;
}

//-------------------------------------------------------------------------
//	���ܣ����ÿ����Ϸѭ������Ҫ����Ķ���
//-------------------------------------------------------------------------
void	KPlayer::Active()
{
	// ���������˵Ĵ���
	if ( !m_cTeam.m_nFlag )
	{
		if (m_cTeam.m_nApplyCaptainID > 0)
		{
			if ( m_cTeam.m_dwApplyTimer == 0 )
			{
				m_cTeam.m_nApplyCaptainID = 0;	
			}
			else
			{
				m_cTeam.m_dwApplyTimer--;
				if ( !NpcSet.SearchID(m_cTeam.m_nApplyCaptainID) )
				{
					m_cTeam.m_nApplyCaptainID = 0;
					m_cTeam.m_dwApplyTimer = 0;
				}
			}
		}
	}
	// �ӳ��Ĵ���
	else if (m_cTeam.m_nFigure == TEAM_CAPTAIN)
	{
		for (int i = 0; i < MAX_TEAM_APPLY_LIST; ++i)
		{
			if (m_cTeam.m_sApplyList[i].m_dwNpcID > 0)
			{
				if (m_cTeam.m_sApplyList[i].m_dwTimer == 0)
				{
					m_cTeam.m_sApplyList[i].m_dwNpcID = 0;
					m_cTeam.UpdateInterface();
					m_cTeam.UpdateamUI(); //��ʾ�����������Ϣ
				}
				else
				{
					m_cTeam.m_sApplyList[i].m_dwTimer--;
					if ( !Npc[this->m_nIndex].SearchAroundID(m_cTeam.m_sApplyList[i].m_dwNpcID) )
					{
						m_cTeam.m_sApplyList[i].m_dwNpcID = 0;
						m_cTeam.m_sApplyList[i].m_dwTimer = 0;
						m_cTeam.UpdateInterface();
						m_cTeam.UpdateamUI(); //��ʾ�����������Ϣ
					}
				}
			}
		}
	}
	

	++m_nSendMoveFrames;
	// ��ɱ����ʱ
	this->m_cPK.Active();

	//this->m_cTong.Active();            //����ѭ����ս
	

}

/*
void	KPlayer::ProcessMsg(KWorldMsgNode *lpMsg)
{
	switch (lpMsg->m_dwMsgType)
	{
	case GWM_PLAYER_SKILL:
		break;
	case GWM_PLAYER_RUNTO:
		break;
	case GWM_PLAYER_WALKTO:
		break;
	case GWM_PLAYER_JUMPTO:
		break;
	default:
		break;
	}	
}
*/

void KPlayer::ProcessInputMsg(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	/*switch(uMsg)
	{
	case WM_MOUSEMOVE:   //����뿪
	case WM_MOUSEHOVER:  //��꾭��
		if (wParam & MK_LBUTTON)
			OnButtonMove(LOWORD(lParam), HIWORD(lParam),
			(wParam & ~(MK_LBUTTON | MK_MBUTTON | MK_RBUTTON)), button_left);
		else if (wParam & MK_RBUTTON)
			OnButtonMove(LOWORD(lParam), HIWORD(lParam),
			(wParam & ~(MK_LBUTTON | MK_MBUTTON | MK_RBUTTON)), button_right);
		else
			OnMouseMove(LOWORD(lParam), HIWORD(lParam));
		break;
		
	case WM_LBUTTONUP:   //�����
		OnButtonUp(LOWORD(lParam), HIWORD(lParam), button_left);
		break;
		
	case WM_LBUTTONDOWN: //�������
		OnButtonDown(LOWORD(lParam), HIWORD(lParam), 
			(wParam & ~(MK_LBUTTON | MK_MBUTTON | MK_RBUTTON)), button_left);
		break;
		
	case WM_RBUTTONUP:   //�Ҽ���
		OnButtonUp(LOWORD(lParam), HIWORD(lParam), button_right);
		break;	
		
	case WM_RBUTTONDOWN: //�Ҽ�����
		OnButtonDown(LOWORD(lParam), HIWORD(lParam),
			(wParam & ~(MK_LBUTTON | MK_MBUTTON | MK_RBUTTON)), button_right);
		break;
	}*/
}
//��������ʱ��
void KPlayer::OnButtonDown(int x,int y, int Key, MOUSE_BUTTON nButton)
{
	m_MouseDown[(int)nButton] = TRUE;

	FindSelectNpc(x, y, relation_all);	 //��ֵ���� NPC 
	FindSelectObject(x, y);	             //��ֵ���� ��Ʒ
	//	Npc[m_nIndex].m_nPeopleIdx = m_nPeapleIdx;
	ProcessMouse(x, y, Key, nButton);
}

void KPlayer::OnButtonMove(int x,int y,int Key, MOUSE_BUTTON nButton)
{
	if (m_MouseDown[(int)nButton])
	{
		ProcessMouse(x, y, Key, nButton);
	}
}

void KPlayer::OnButtonUp(int x,int y,MOUSE_BUTTON nButton)
{
	m_MouseDown[(int)nButton] = 0;
}
//�ͻ�����꾭����ʾ��ͼ��
void KPlayer::OnMouseMove(int x,int y)
{
	m_nPeapleIdx = 0;
	m_nObjectIdx = 0;
	FindSelectNpc(x, y, relation_all);
	FindSelectObject(x, y);

	if (m_nPeapleIdx) //m_nPeapleIdx
	{
		if (Npc[m_nPeapleIdx].m_Kind == kind_dialoger)
		{
			//char nMsg[64];
			//t_sprintf(nMsg,"��ʾ: %s �����..", Npc[m_nPeapleIdx].Name);
			//m_ItemList.ClientShowMsg(nMsg);
			CoreDataChanged(GDCNI_SWITCH_CURSOR, NULL, MOUSE_CURSOR_DIALOG);
		}
		else if (Npc[m_nPeapleIdx].m_Kind == kind_player && Npc[m_nPeapleIdx].m_BaiTan)
		{
			//char nMsg[64];
			//t_sprintf(nMsg,"��ʾ: %s ��̯��..", Npc[m_nPeapleIdx].Name);
			//m_ItemList.ClientShowMsg(nMsg);
			CoreDataChanged(GDCNI_SWITCH_CURSOR, NULL, MOUSE_CURSOR_DIALOG);  //�������ָ��ͼ��
		}
		else if (NpcSet.GetRelation(m_nIndex, m_nPeapleIdx) == relation_enemy)
			CoreDataChanged(GDCNI_SWITCH_CURSOR, NULL, MOUSE_CURSOR_FIGHT);
		else
			CoreDataChanged(GDCNI_SWITCH_CURSOR, NULL, MOUSE_CURSOR_NORMAL);
	}
	else if (m_nObjectIdx)
	{
		if (Object[m_nObjectIdx].m_nKind == Obj_Kind_MapObj)
			CoreDataChanged(GDCNI_SWITCH_CURSOR, NULL, MOUSE_CURSOR_DIALOG);
		else if (Object[m_nObjectIdx].m_nKind == Obj_Kind_Item || Object[m_nObjectIdx].m_nKind == Obj_Kind_Money)
			CoreDataChanged(GDCNI_SWITCH_CURSOR, NULL, MOUSE_CURSOR_PICK);
		else if (Object[m_nObjectIdx].m_nKind == Obj_Kind_Prop)
			CoreDataChanged(GDCNI_SWITCH_CURSOR, NULL, MOUSE_CURSOR_USE);
	}
	else
		CoreDataChanged(GDCNI_SWITCH_CURSOR, NULL, MOUSE_CURSOR_NORMAL);
}
//�����Ϊ ����߳�
void KPlayer::ProcessMouse(int x, int y, int Key, MOUSE_BUTTON nButton)
{

	if (CheckTrading())
		return;

	if (m_ItemList.m_Hand > 0)
	{
		if (nButton == button_left) //���
		{
			ThrowAwayItem();  //������Ʒ ������Ʒ
			return;
		}
	}

	int nX = x;
	int nY = y;
	int nZ = 0;
	//g_ScenePlace.ViewPortCoordToSpaceCoord(nX, nY, nZ);
	
	/*	if (Key & MK_SHIFT)
	{
	Npc[m_nIndex].SendCommand(do_jump, nX, nY);
	SendClientCmdJump(nX, nY);
	return;
}*/
	
	if (Npc[m_nIndex].IsCanInput())
	{

		int nIdx = 0;
		
		if (nButton == button_right)
		{
			nIdx = Npc[m_nIndex].m_SkillList.FindSame(m_nRightSkillID);
			//g_DebugLog("[skill]right");
		}
		else
		{
			nIdx = Npc[m_nIndex].m_SkillList.FindSame(m_nLeftSkillID);
			//g_DebugLog("[skill]left");
		}
		Npc[m_nIndex].SetActiveSkill(nIdx);
	}
	else
	{
		//g_DebugLog("[skill]return");
		Npc[m_nIndex].m_nPeopleIdx = 0;
		return;
	}
	
	//if ((Key & MK_SHIFT) || (nButton == button_right))  //ǿ�ƹ���
	{
		if (Npc[m_nIndex].m_ActiveSkillID > 0)
		{
			ISkill * pISkill = (KSkill *) g_SkillManager.GetSkill(Npc[m_nIndex].m_ActiveSkillID, 1);
			if (!pISkill) 
                return;
			
			if (pISkill->IsAura())
				return;
			int nAttackRange = pISkill->GetAttackRadius();
			
			int nTargetIdx = 0;
			//m_nPeapleIdx = 0;
			//����Object / Enemy / Ally �����ȼ��ҵ���Ҫ��Ķ���id
			
			
			if (pISkill->IsTargetAlly())
			{
				FindSelectNpc(x, y, relation_ally);
				if (m_nPeapleIdx)
				{
					nTargetIdx = m_nPeapleIdx;
				}
			}
			
			if (pISkill->IsTargetEnemy())
			{
				FindSelectNpc(x, y, relation_enemy);

				if (m_nPeapleIdx)
				{
					nTargetIdx = m_nPeapleIdx;
				}
			}
			
			if (pISkill->IsTargetObj())
			{
				FindSelectObject(x, y);
				if (m_nObjectIdx)
				{
					nTargetIdx = m_nObjectIdx;
				}
			}
			//������ܱ���ָ�����󣬶���ǰλ���޶���Ļ���ֱ���˳�
			if (pISkill->IsTargetOnly() && !nTargetIdx)
            {
				Npc[m_nIndex].m_nPeopleIdx = 0;
				m_nPeapleIdx = 0;
				return;
			}
			
			if (m_nIndex == nTargetIdx)
			{
				Npc[m_nIndex].m_nPeopleIdx = 0;
				m_nPeapleIdx = 0;
				return;
			}
		  /*
			if (Npc[m_nIndex].m_SkillList.GetNextCastTime(Npc[m_nIndex].m_ActiveSkillID)>0 && (Npc[m_nIndex].m_SkillList.GetNextCastTime(Npc[m_nIndex].m_ActiveSkillID)>SubWorld[Npc[m_nIndex].m_SubWorldIndex].m_dwCurrentTime))
			{//��û�ⶳ
				return;
			}
			
			if (Npc[m_nIndex].m_HorseType)
			{//����ʱ������
				if (Npc[m_nIndex].m_SkillList.GetHorseNextCastTime(Npc[m_nIndex].m_ActiveSkillID)>0 && (Npc[m_nIndex].m_SkillList.GetHorseNextCastTime(Npc[m_nIndex].m_ActiveSkillID)>SubWorld[Npc[m_nIndex].m_SubWorldIndex].m_dwCurrentTime))
				return;
			} */
		
			if ((!Npc[m_nIndex].m_SkillList.CanCast(Npc[m_nIndex].m_ActiveSkillID, SubWorld[Npc[m_nIndex].m_SubWorldIndex].m_dwCurrentTime))
				||(!Npc[m_nIndex].Cost(pISkill->GetSkillCostType() , pISkill->GetSkillCost(&Npc[m_nIndex]), TRUE)))
			{
				Npc[m_nIndex].m_nPeopleIdx = 0;
				m_nPeapleIdx = 0;
				return ;
			}
			
			//�޶���ֱ�ӷ�����
			if (!nTargetIdx)
			{
				Npc[m_nIndex].SendSerCommand(do_skill, Npc[m_nIndex].m_ActiveSkillID, nX, nY);		
				SendClientCmdSkill(Npc[m_nIndex].m_ActiveSkillID, nX, nY);
                return ;   //�޶���ֱ���˳� ��������
			}
			else
			{
				if (pISkill->IsTargetOnly())
				{
					int distance = NpcSet.GetDistance(m_nIndex , nTargetIdx);
					if (distance > pISkill->GetAttackRadius())
					{
						m_nPeapleIdx = nTargetIdx;
						return ;
					}
				}
				// 
				if (m_nIndex == nTargetIdx && pISkill->GetSkillStyle() == SKILL_SS_Missles) 
					return ;
				Npc[m_nIndex].SendSerCommand(do_skill, Npc[m_nIndex].m_ActiveSkillID, -1, nTargetIdx);
				// Send to Server		
				SendClientCmdSkill(Npc[m_nIndex].m_ActiveSkillID, -1, Npc[nTargetIdx].m_dwID);
			}
		}

		Npc[m_nIndex].m_nPeopleIdx = 0;
		return;
	}
	
	if (Key == 0 )
	{//û�м��̼�ʱ
		int nRelation = NpcSet.GetRelation(m_nIndex, m_nPeapleIdx);
		if(nRelation == relation_enemy || nRelation == relation_dialog ||(Npc[m_nPeapleIdx].m_Kind==kind_player && Npc[m_nPeapleIdx].m_BaiTan==1))
		{
			Npc[m_nIndex].m_nPeopleIdx = m_nPeapleIdx;
		}
		
		if (m_nSendMoveFrames >= defMAX_PLAYER_SEND_MOVE_FRAME)
		{
			m_nPickObjectIdx = m_nObjectIdx;
			Npc[m_nIndex].m_nObjectIdx = m_nPickObjectIdx;

			if (!m_RunStatus)
			{
				Npc[m_nIndex].SendSerCommand(do_walk, nX, nY);
				// Send to Server
				SendClientCmdWalk(nX, nY);
			}
			else
			{
				Npc[m_nIndex].SendSerCommand(do_run, nX, nY);
				// Send to Server
				SendClientCmdRun(nX, nY);
			}
			m_nSendMoveFrames = 0;
		}
		return;
	}
	
	/*	if (Key & MK_ALT)
	{
	Npc[m_nIndex].SendCommand(do_jump, nX, nY);
	SendClientCmdJump(nX, nY);
	}*/
}

void KPlayer::Walk(int nDir, int nSpeed)
{
	int	nMapX = Npc[m_nIndex].m_MapX;
	int nMapY = Npc[m_nIndex].m_MapY;
	int	nOffX = Npc[m_nIndex].m_OffX;
	int	nOffY = Npc[m_nIndex].m_OffY;
	int	nSubWorld = Npc[m_nIndex].m_SubWorldIndex;
	int	nRegion = Npc[m_nIndex].m_RegionIndex;
	int	nX, nY;
	
	SubWorld[nSubWorld].NewMap2Mps(nRegion, nMapX, nMapY, nOffX, nOffY, &nX, &nY);

	SubWorld[nSubWorld].GetMps(&nX, &nY, nSpeed * 2, nDir);
	
	if (m_RunStatus)
	{
		Npc[m_nIndex].SendSerCommand(do_run, nX, nY);
		// Send to Server
		if ( !CheckTrading() )
			SendClientCmdRun(nX, nY);
	}
	else
	{
		Npc[m_nIndex].SendSerCommand(do_walk, nX, nY);
		// Send to Server
		if (!CheckTrading())
			SendClientCmdWalk(nX, nY);
	}
}


void KPlayer::TurnLeft()
{
	if (Npc[m_nIndex].m_Doing != do_stand &&
		Npc[m_nIndex].m_Doing != do_sit)
		return;
	
	if (Npc[m_nIndex].m_Dir > 8)
		Npc[m_nIndex].m_Dir -= 8;
	else
		Npc[m_nIndex].m_Dir = MAX_NPC_DIR - 1;
}

void KPlayer::TurnRight()
{
	if (Npc[m_nIndex].m_Doing != do_stand &&
		Npc[m_nIndex].m_Doing != do_sit)
		return;
	
	if (Npc[m_nIndex].m_Dir < MAX_NPC_DIR - 9)
		Npc[m_nIndex].m_Dir += 8;
	else
		Npc[m_nIndex].m_Dir = 0;
}

void KPlayer::TurnBack()
{
	if (Npc[m_nIndex].m_Doing != do_stand &&
		Npc[m_nIndex].m_Doing != do_sit)
		return;
	
	if (Npc[m_nIndex].m_Dir < MAX_NPC_DIR / 2)
		Npc[m_nIndex].m_Dir += MAX_NPC_DIR / 2;
	else
		Npc[m_nIndex].m_Dir -= MAX_NPC_DIR / 2;
}

void KPlayer::FindSelectNpc(int x, int y, int nRelation)
{
	int	nNpcIdx = 0;
	
	    nNpcIdx = NpcSet.SearchNpcAt(x, y, nRelation, 16);

	if  (nNpcIdx>0)
	{
		if (Npc[nNpcIdx].m_Hide.nTime > 0 && NpcSet.GetRelation(m_nIndex,nNpcIdx) == relation_enemy)
			m_nPeapleIdx = 0; //ֻ�Ե�������
		else
		{
			m_nPeapleIdx  = nNpcIdx;
			m_nImageNpcID = nNpcIdx;
		} 
	}
	else
		m_nPeapleIdx = 0;
}

void KPlayer::FindSelectObject(int x, int y)
{
	int	nObjIdx = 0;
	
	nObjIdx = ObjSet.SearchObjAt(x, y, 40);
	if (nObjIdx)
		m_nObjectIdx = nObjIdx;
	else
		m_nObjectIdx = 0;
}
// need change
int	KPlayer::NetCommandPlayerTalk(BYTE * pProtocol)
{
/*	DWORD	dwNpcID;
int		nSize;
dwNpcID = *(DWORD *) &pProtocol[1];
nSize = pProtocol[5];
int nIdx = NpcSet.SearchID(dwNpcID);

  if (ConformIdx(nIdx))
  {
		strcpy(Npc[nIdx].m_szChatBuffer, (char *)(pProtocol+6));
		Npc[nIdx].m_nCurChatTime = NPC_SHOW_CHAT_TIME_LENGTH;
		}
		
		  return (7 + nSize);	// need test
	*/
	return 0;
}


BOOL KPlayer::ConformIdx(int nIdx)
{
	if (nIdx == m_nIndex || nIdx == 0)
		return FALSE;
	return TRUE;
}


//-------------------------------------------------------------------------
//	���ܣ�����ҵ�½ʱ�����������Բ��� ���� ���� ���� ���� ������ֵ   ----û�е���
//-------------------------------------------------------------------------
BOOL	KPlayer::NewPlayerGetBaseAttribute(int Series)
{
	if (Series < series_metal || Series > series_earth)
		return FALSE;
	
	Npc[m_nIndex].SetSeries(Series);
	
	m_nStrength = PlayerSet.m_cNewPlayerAttribute.m_nStrength[Series];
	m_nDexterity = PlayerSet.m_cNewPlayerAttribute.m_nDexterity[Series];
	m_nVitality = PlayerSet.m_cNewPlayerAttribute.m_nVitality[Series];
	m_nEngergy = PlayerSet.m_cNewPlayerAttribute.m_nEngergy[Series];
	m_nLucky = PlayerSet.m_cNewPlayerAttribute.m_nLucky[Series]; //����ֵ
	m_nUpExp = 0;  //ɱ�����˻�þ�������
	
	m_nCurStrength = m_nStrength;
	m_nCurDexterity = m_nDexterity;
	m_nCurVitality = m_nVitality;
	m_nCurEngergy = m_nEngergy;
	CalcCurLucky();
	
	m_cFaction.SetSeries(Series);
	
	return TRUE;
}

//-------------------------------------------------------------------------
//	���ܣ����㵱ǰ����
//-------------------------------------------------------------------------
//void	KPlayer::CalcCurStrength()
//{	// ����Ҫ���� װ�������ܡ�״̬ ��Ӱ��
//	m_nCurStrength = m_nStrength;
//}

//-------------------------------------------------------------------------
//	���ܣ����㵱ǰ����
//-------------------------------------------------------------------------
//void	KPlayer::CalcCurDexterity()
//{	// ����Ҫ���� װ�������ܡ�״̬ ��Ӱ��
//	m_nCurDexterity = m_nDexterity;
//}

//-------------------------------------------------------------------------
//	���ܣ����㵱ǰ����
//-------------------------------------------------------------------------
//void	KPlayer::CalcCurVitality()
//{	// ����Ҫ���� װ�������ܡ�״̬ ��Ӱ��
//	m_nCurVitality = m_nVitality;
//}

//-------------------------------------------------------------------------
//	���ܣ����㵱ǰ����
//-------------------------------------------------------------------------
//void	KPlayer::CalcCurEngergy()
//{	// ����Ҫ���� װ�������ܡ�״̬ ��Ӱ��
//	m_nCurEngergy = m_nEngergy;
//}

//-------------------------------------------------------------------------
//	���ܣ����㵱ǰ����
//-------------------------------------------------------------------------
void	KPlayer::CalcCurLucky()
{	// ����Ҫ���� װ�������ܡ�״̬ ��Ӱ��
	m_nCurLucky = m_nLucky;
}

//-------------------------------------------------------------------------
//	���ܣ��ı䵱ǰ����(�� nData С�� 0 ʱ������)
//-------------------------------------------------------------------------
void	KPlayer::ChangeCurStrength(int nData)
{

	m_nCurStrength +=nData;

	SetNpcPhysicsDamage();  //����������
	// �ѵ�ǰ���������ͻ���
}

//-------------------------------------------------------------------------
//	���ܣ��ı䵱ǰ����(�� nData С�� 0 ʱ������)
//-------------------------------------------------------------------------
void	KPlayer::ChangeCurDexterity(int nData)
{
/*	if (m_nCurDexterity + nData < 0)
return;
	*/
	m_nCurDexterity += nData;
	
	int nRating = nData * 4;
	Npc[m_nIndex].m_CurrentAttackRating += nRating;
	
	int nDefence = nData / 4;
	Npc[m_nIndex].m_CurrentDefend += nDefence;
	
	SetNpcPhysicsDamage();
	
}


//-------------------------------------------------------------------------
//	���ܣ��ı䵱ǰ����(�� nData С�� 0 ʱ������)  ��//���ӵ�ǰ���� ������ֵ
//-------------------------------------------------------------------------
void	KPlayer::ChangeCurVitality(int nData)	    //״̬
{
/*	if (nData + m_nAttributePoint < 0)
return;
	*/
	m_nCurVitality += nData;  //�⹦���Ӷ��ٵ�
	
	Npc[m_nIndex].AddCurLifeMax(PlayerSet.m_cLevelAdd.GetLifePerVitality(Npc[m_nIndex].m_Series) * nData);
	Npc[m_nIndex].AddCurStaminaMax(PlayerSet.m_cLevelAdd.GetStaminaPerVitality(Npc[m_nIndex].m_Series) * nData);	//����
	
}

//-------------------------------------------------------------------------
//	���ܣ��ı䵱ǰ����(�� nData С�� 0 ʱ������) �ڹ�
//-------------------------------------------------------------------------
void	KPlayer::ChangeCurEngergy(int nData)
{
/*	if (m_nAttributePoint + nData < 0)
return;
	*/
	m_nCurEngergy += nData;
	
	Npc[m_nIndex].AddCurManaMax(PlayerSet.m_cLevelAdd.GetManaPerEnergy(Npc[m_nIndex].m_Series) * nData);

	SetNpcPhysicsDamage();
}

//-------------------------------------------------------------------------
//	���ܣ����ӻ�������
//-------------------------------------------------------------------------
void	KPlayer::AddBaseLucky(int nData)
{
	m_nLucky += nData;
	CalcCurLucky();
}

//-------------------------------------------------------------------------
//	���ܣ��ɵ�ǰ���������Ӧnpc�������˺�(PhysicsDamage)---�����������˺�Ϊ����---��װ�ǵ���
//-------------------------------------------------------------------------
void	KPlayer::SetNpcPhysicsDamage()
{
	int		nMinDamage=0, nMaxDamage=0,mLeftskillidex=0;

	        m_ItemList.GetWeaponDamage(&nMinDamage, &nMaxDamage);  //��ȡ�����Ļ����˺�

	        mLeftskillidex=GetLeftSkill();                         //�������

		KSkill * pOrdinSkill = (KSkill *) g_SkillManager.GetSkill(mLeftskillidex,1);

		if (!pOrdinSkill) 
            return;

	if (m_ItemList.GetWeaponType() == equip_meleeweapon)	
	{// (��ǰװ���ǽ�������)��������
	  /*nMinDamage = nMinDamage * (m_nCurStrength + STRENGTH_SET_DAMAGE_VALUE) / STRENGTH_SET_DAMAGE_VALUE;
	    nMaxDamage = nMaxDamage * (m_nCurStrength + STRENGTH_SET_DAMAGE_VALUE) / STRENGTH_SET_DAMAGE_VALUE;
	  */
		if (Npc[m_nIndex].m_Series==0)
		{//��ϵ
		   nMinDamage += m_nCurStrength/STRENGTH_SET_DAMAGE_VALUE;
		   nMaxDamage += m_nCurStrength/STRENGTH_SET_DAMAGE_VALUE;
		}
		else if (Npc[m_nIndex].m_Series==1) 
		{//ľϵ
			if (m_ItemList.GetWeaponParticular()==1 && !pOrdinSkill->IsMagic())
			{//��������ǵ����Ļ� ���Ǽ�����
			   
			   nMinDamage += m_nCurStrength/STRENGTH_SET_DAMAGE_VALUE;
		       nMaxDamage += m_nCurStrength/STRENGTH_SET_DAMAGE_VALUE;
			}
			else
			{//���ڹ� ���ӹ���
				nMinDamage += m_nCurEngergy/STRENGTH_SET_DAMAGE_VALUE;
		        nMaxDamage += m_nCurEngergy/STRENGTH_SET_DAMAGE_VALUE;
			}
		}
		else if (Npc[m_nIndex].m_Series==2)
		{//ˮϵ
			if ((m_ItemList.GetWeaponParticular()==1 || m_ItemList.GetWeaponParticular()==0) && !pOrdinSkill->IsMagic())
			{//��������ǵ��� �� ���Ļ� ���Ǽ�����
				nMinDamage += m_nCurStrength/STRENGTH_SET_DAMAGE_VALUE;
				nMaxDamage += m_nCurStrength/STRENGTH_SET_DAMAGE_VALUE;
			}
			else
			{//���ڹ� ���ӹ���
				nMinDamage += m_nCurEngergy/STRENGTH_SET_DAMAGE_VALUE;
				nMaxDamage += m_nCurEngergy/STRENGTH_SET_DAMAGE_VALUE;
			}
		}
		else if (Npc[m_nIndex].m_Series==3)
		{//��ϵ
			if ((m_ItemList.GetWeaponParticular()==2 || m_ItemList.GetWeaponParticular()==3) && !pOrdinSkill->IsMagic())
			{//��������ǹ��� �� ǹ�Ļ� ���Ǽ�����
				nMinDamage += m_nCurStrength/STRENGTH_SET_DAMAGE_VALUE;
				nMaxDamage += m_nCurStrength/STRENGTH_SET_DAMAGE_VALUE;
			}
			else
			{//���ڹ� ���ӹ���
				nMinDamage += m_nCurEngergy/STRENGTH_SET_DAMAGE_VALUE;
				nMaxDamage += m_nCurEngergy/STRENGTH_SET_DAMAGE_VALUE;
			}
		}
		else if (Npc[m_nIndex].m_Series==4)
		{//��ϵ
			if ((m_ItemList.GetWeaponParticular()==0 || m_ItemList.GetWeaponParticular()==1) && !pOrdinSkill->IsMagic())
			{//��������ǽ� �� �����Ļ� ���Ǽ�����
				nMinDamage += m_nCurStrength/STRENGTH_SET_DAMAGE_VALUE;
				nMaxDamage += m_nCurStrength/STRENGTH_SET_DAMAGE_VALUE;
			}
			else
			{//���ڹ� ���ӹ���
				nMinDamage += m_nCurEngergy/STRENGTH_SET_DAMAGE_VALUE;
				nMaxDamage += m_nCurEngergy/STRENGTH_SET_DAMAGE_VALUE;
			}
		}

		Npc[m_nIndex].SetPhysicsDamage(nMinDamage,nMaxDamage);
	}
	else if (m_ItemList.GetWeaponType() == equip_rangeweapon)	
	{// (��ǰװ����Զ������)����(��)
	     /*nMinDamage = nMinDamage * (m_nCurDexterity + DEXTERITY_SET_DAMAGE_VALUE) / DEXTERITY_SET_DAMAGE_VALUE;
	       nMaxDamage = nMaxDamage * (m_nCurDexterity + DEXTERITY_SET_DAMAGE_VALUE) / DEXTERITY_SET_DAMAGE_VALUE;
		 */
		if (!pOrdinSkill->IsMagic())
		{//������⹦
			nMinDamage += m_nCurDexterity/DEXTERITY_SET_DAMAGE_VALUE;
			nMaxDamage += m_nCurDexterity/DEXTERITY_SET_DAMAGE_VALUE;
		}
		else
		{//�ڹ����ڹ�
			nMinDamage += m_nCurEngergy/STRENGTH_SET_DAMAGE_VALUE;
			nMaxDamage += m_nCurEngergy/STRENGTH_SET_DAMAGE_VALUE;
		}

		Npc[m_nIndex].SetPhysicsDamage(nMinDamage,nMaxDamage);
	
	}
	else
	{//���� 
		Npc[m_nIndex].SetPhysicsDamage(nMinDamage,nMaxDamage);
	}
}

//-------------------------------------------------------------------------
//	���ܣ��ɵ�ǰ���ݼ����Ӧnpc�Ĺ���������(AttackRating)
//-------------------------------------------------------------------------
void	KPlayer::SetNpcAttackRating()
{
	int		nRating;
	
	nRating = m_nDexterity * 4 - 28;
	Npc[m_nIndex].SetBaseAttackRating(nRating);
}

//-------------------------------------------------------------------------
//	���ܣ��ɵ�ǰ���ݼ����Ӧnpc�ķ�����
//-------------------------------------------------------------------------
void	KPlayer::SetNpcDefence()
{
	int		nDefence;
	
	nDefence = m_nDexterity / 4;
	Npc[m_nIndex].SetBaseDefence(nDefence);
}

/*
#ifdef _SERVER
//-------------------------------------------------------------------------
//	���ܣ��ɵ�ǰ���ݼ����Ӧnpc�������ٶ�
//-------------------------------------------------------------------------
void	KPlayer::SetNpcWalkSpeed()
{
int		nSpeed;
nSpeed = BASE_WALK_SPEED * (m_nCurDexterity + 320) / 320;
Npc[m_nIndex].SetBaseWalkSpeed(nSpeed);
}
#endif
*/

/*
#ifdef _SERVER
//-------------------------------------------------------------------------
//	���ܣ��ɵ�ǰ���ݼ����Ӧnpc���ܲ��ٶ�
//-------------------------------------------------------------------------
void	KPlayer::SetNpcRunSpeed()
{
int		nSpeed;
nSpeed = BASE_RUN_SPEED * (m_nCurDexterity + 320) / 320;
Npc[m_nIndex].SetBaseRunSpeed(nSpeed);
}
#endif
*/

//-------------------------------------------------------------------------
//	���ܣ�ֱ�����Ӿ���ֵ����������������
//-------------------------------------------------------------------------
void	KPlayer::DirectAddExp(int nExp)
{

	if (nExp == 0 || Npc[m_nIndex].m_Level >= MAX_LEVEL)
		return;
	
    int nMaxLevel=1000;
       g_GameSetTing.GetInteger("ServerConfig","PlayerMaxLevel",1000,&nMaxLevel);

     if (Npc[m_nIndex].m_Level>=nMaxLevel)
		 return;

	m_nExp += nExp;
 
}

//-------------------------------------------------------------------------
//	���ܣ������һ��
//-------------------------------------------------------------------------
void	KPlayer::LevelUp()
{
	m_nExp = 0;

	if (Npc[m_nIndex].m_Level >= MAX_LEVEL)
		return;

	Npc[m_nIndex].m_Level++;

	m_nNextLevelExp = PlayerSet.m_cLevelAdd.GetLevelExp(Npc[m_nIndex].m_Level);
	
	// -------------------------- �ȼ�����ʱ������������ ----------------------
	// ����δ�������Ե�
	m_nAttributePoint += PLAYER_LEVEL_ADD_ATTRIBUTE;   //һ������5������
	// ����δ���似�ܵ�
	m_nSkillPoint     += PLAYER_LEVEL_ADD_SKILL;       //һ������1�㼼�ܵ�
	
	// ��������������������ֵ��Ӱ������������������������ֵ�����أ��ȼ������Ե㣬Ӱ�쵱ǰ�������ֵ�����أ�װ�������ܡ�ҩ���ʱ����
	LevelAddBaseLifeMax();
	LevelAddBaseStaminaMax();
	LevelAddBaseManaMax();
	// Npc[m_nIndex].ResetLifeReplenish();
	
	// ������ֿ��Եı仯 �𡢱��������硢����  ԭʼ
	Npc[m_nIndex].m_FireResist				= PlayerSet.m_cLevelAdd.GetFireResist(Npc[m_nIndex].m_Series, Npc[m_nIndex].m_Level);
	Npc[m_nIndex].m_CurrentFireResist		= Npc[m_nIndex].m_FireResist;
	Npc[m_nIndex].m_ColdResist				= PlayerSet.m_cLevelAdd.GetColdResist(Npc[m_nIndex].m_Series, Npc[m_nIndex].m_Level);
	Npc[m_nIndex].m_CurrentColdResist		= Npc[m_nIndex].m_ColdResist;
	Npc[m_nIndex].m_PoisonResist			= PlayerSet.m_cLevelAdd.GetPoisonResist(Npc[m_nIndex].m_Series, Npc[m_nIndex].m_Level);
	Npc[m_nIndex].m_CurrentPoisonResist		= Npc[m_nIndex].m_PoisonResist;
	Npc[m_nIndex].m_LightResist				= PlayerSet.m_cLevelAdd.GetLightResist(Npc[m_nIndex].m_Series, Npc[m_nIndex].m_Level);
	Npc[m_nIndex].m_CurrentLightResist		= Npc[m_nIndex].m_LightResist;
	Npc[m_nIndex].m_PhysicsResist			= PlayerSet.m_cLevelAdd.GetPhysicsResist(Npc[m_nIndex].m_Series, Npc[m_nIndex].m_Level);
	Npc[m_nIndex].m_CurrentPhysicsResist	= Npc[m_nIndex].m_PhysicsResist;

	Npc[m_nIndex].m_FireResistMax			= BASE_FIRE_RESIST_MAX;
	Npc[m_nIndex].m_ColdResistMax			= BASE_COLD_RESIST_MAX;
	Npc[m_nIndex].m_PoisonResistMax			= BASE_POISON_RESIST_MAX;
	Npc[m_nIndex].m_LightResistMax			= BASE_LIGHT_RESIST_MAX;
	Npc[m_nIndex].m_PhysicsResistMax		= BASE_PHYSICS_RESIST_MAX;
/*
	Npc[m_nIndex].m_CurrentFireResistMax	= Npc[m_nIndex].m_FireResistMax;
	Npc[m_nIndex].m_CurrentColdResistMax	= Npc[m_nIndex].m_ColdResistMax;
	Npc[m_nIndex].m_CurrentPoisonResistMax	= Npc[m_nIndex].m_PoisonResistMax;
	Npc[m_nIndex].m_CurrentLightResistMax	= Npc[m_nIndex].m_LightResistMax;
	Npc[m_nIndex].m_CurrentPhysicsResistMax	= Npc[m_nIndex].m_PhysicsResistMax;
*/
	int nOldCurCamp = Npc[m_nIndex].m_CurrentCamp;
	// ����װ����������Ϣ���µ�ǰ����
	this->UpdataCurData();
	SetNpcPhysicsDamage();
	Npc[m_nIndex].m_CurrentCamp = nOldCurCamp;
	
	// ����������������ֵ����
	Npc[m_nIndex].m_CurrentLife = Npc[m_nIndex].m_CurrentLifeMax;
	Npc[m_nIndex].m_CurrentStamina = Npc[m_nIndex].m_CurrentStaminaMax;
	Npc[m_nIndex].m_CurrentMana = Npc[m_nIndex].m_CurrentManaMax;
}

// ͬ���ͻ�������״̬���ܵ�����
void	KPlayer::UpdataCurData(int nIsLoign,int nUnEquip) //������������Բ���
{
	if (m_nIndex <= 0 || m_nIndex >= MAX_NPC)
		return;

	for (int j = 0 ; j< MAX_NPCSKILL;++j)
	{
		int nAdd = 	Npc[m_nIndex].m_SkillList.GetAddPoint(j); //��ȡ��λ�õĶ��⼼�ܵ�
		if (nAdd > 0)
		{
			nAdd = -nAdd;  //���ڸ���
			Npc[m_nIndex].m_SkillList.QeuipAddPoint(j,nAdd); //���ø�λ�ü��ܵ�Ϊʵ�ʼ��ܵ�
		}

		Npc[m_nIndex].m_SkillList.m_Skills[j].mAddPoint = 0;
		Npc[m_nIndex].m_SkillList.m_Skills[j].EnChance = 0;  // ���ܵļӳ�
		Npc[m_nIndex].m_SkillList.m_Skills[j].nTempEnChance = 0;  // ���ܵļӳ�
	}

	ZeroMemory(&Npc[m_nIndex].m_CurrentPhysicsMagicDamageP, sizeof(KMagicAttrib));
	ZeroMemory(&Npc[m_nIndex].m_CurrentPhysicsMagicDamageV, sizeof(KMagicAttrib));

	int nCurbei=0; //,nFomoidx=Npc[m_nIndex].GetCurFuMoIdx()
	
	/*if 	(nFomoidx>-1)
	{
		if (Npc[m_nIndex].GetCurFoMoSkllLevel(nFomoidx)>=5) //��ȡ�񽫵�ǰ�ĵȼ�
		{
			nCurbei=Npc[m_nIndex].GetCurFoMoSkllLevel(nFomoidx)/5;
		}
	} */

	Npc[m_nIndex].m_CurrentLifeMax		= Npc[m_nIndex].m_LifeMax*(100+m_cReBorn.GetReBornNum()*m_cReBorn.GetReBornLifeMaxVal()+nCurbei*50)/100;
	//;Npc[m_nIndex].TakeTrader(Npc[m_nIndex].m_LifeMax*(100+m_cReBorn.GetReBornNum()*50+nCurbei*50),100);
	//Npc[m_nIndex].m_LifeMax*(100+m_cReBorn.GetReBornNum()*50+nCurbei*50)/100;
	Npc[m_nIndex].m_CurrentManaMax		= Npc[m_nIndex].m_ManaMax*(100+m_cReBorn.GetReBornNum()*m_cReBorn.GetReBornLifeMaxVal()+nCurbei*50)/100;
	//Npc[m_nIndex].TakeTrader(Npc[m_nIndex].m_ManaMax*(100+m_cReBorn.GetReBornNum()*50+nCurbei*50),100);
	//Npc[m_nIndex].m_ManaMax*(100+m_cReBorn.GetReBornNum()*50+nCurbei*50)/100;
	Npc[m_nIndex].m_CurrentStaminaMax	= Npc[m_nIndex].m_StaminaMax;
	Npc[m_nIndex].m_CurrentAttackRating	= Npc[m_nIndex].m_AttackRating;
	Npc[m_nIndex].m_CurrentAttackSpeed	= Npc[m_nIndex].m_AttackSpeed;
	Npc[m_nIndex].m_CurrentCastSpeed	= Npc[m_nIndex].m_CastSpeed;
    Npc[m_nIndex].m_CurrentRunSpeed	    = Npc[m_nIndex].m_RunSpeed; 
	Npc[m_nIndex].m_CurrentJumpSpeed	= Npc[m_nIndex].m_JumpSpeed;
	Npc[m_nIndex].m_CurrentVisionRadius	= Npc[m_nIndex].m_VisionRadius;
	Npc[m_nIndex].m_CurrentWalkSpeed	= Npc[m_nIndex].m_WalkSpeed;
	Npc[m_nIndex].m_CurrentNuQiMax		= Npc[m_nIndex].m_NuqiMax;//ŭ��
	m_nCurStrength						= m_nStrength;
	m_nCurDexterity						= m_nDexterity;
	m_nCurVitality						= m_nVitality;
	m_nCurEngergy						= m_nEngergy;
	m_nCurLucky							= m_nLucky;

	m_nTempLucky_p                      = 0;

	Npc[m_nIndex].m_TempFireResist = 0;	        // Npc�ĵ�ǰ����
	Npc[m_nIndex].m_TempColdResist = 0;	        // Npc�ĵ�ǰ������
	Npc[m_nIndex].m_TempPoisonResist = 0;	    // Npc�ĵ�ǰ������
	Npc[m_nIndex].m_TempLightResist = 0;	    // Npc�ĵ�ǰ�翹��
	Npc[m_nIndex].m_TempPhysicsResist = 0;	    // Npc�ĵ�ǰ������
	Npc[m_nIndex].m_CurrentHitNpcRecover = 0;
	Npc[m_nIndex].m_CurrentHitRank       = 0;

	Npc[m_nIndex].m_CurrentColdResist	= Npc[m_nIndex].m_ColdResist;  //ԭʼ�ķ���
	Npc[m_nIndex].m_CurrentDefend		= Npc[m_nIndex].m_Defend;

	Npc[m_nIndex].m_CurrentFireResist	= Npc[m_nIndex].m_FireResist;
	Npc[m_nIndex].m_CurrentLifeReplenish= Npc[m_nIndex].m_LifeReplenish;
	Npc[m_nIndex].m_CurrentLifeReplenish_p	= 0;
	Npc[m_nIndex].m_CurrentManaReplenish_p  = 0;
	Npc[m_nIndex].m_CurrentHitRecover	= Npc[m_nIndex].m_HitRecover;  //Npc�����˶���ʱ��
	Npc[m_nIndex].m_CurrentLightResist	= Npc[m_nIndex].m_LightResist;
	Npc[m_nIndex].m_CurrentManaReplenish= Npc[m_nIndex].m_ManaReplenish;
	Npc[m_nIndex].m_CurrentPhysicsResist= Npc[m_nIndex].m_PhysicsResist;
	Npc[m_nIndex].m_CurrentPoisonResist	= Npc[m_nIndex].m_PoisonResist;
	Npc[m_nIndex].m_CurrentStaminaGain	= Npc[m_nIndex].m_StaminaGain;
	Npc[m_nIndex].m_CurrentStaminaLoss	= Npc[m_nIndex].m_StaminaLoss;

	//SetBaseSpeedAndRadius();
	ZeroMemory(&Npc[m_nIndex].m_CurrentMagicColdDamage, sizeof(KMagicAttrib));
//	ZeroMemory(&Npc[m_nIndex].m_CurrentColdDamage, sizeof(KMagicAttrib));
	Npc[m_nIndex].m_CurrentColdEnhance	= 0;

	Npc[m_nIndex].m_CurrentColdResistMax	= BASE_FANGYU_ALL_MAX+m_cReBorn.GetReBornNum()*m_cReBorn.GetReBornFanYuMaxVal();//Npc[m_nIndex].m_ColdResistMax;
	Npc[m_nIndex].m_CurrentFireResistMax    = BASE_FANGYU_ALL_MAX+m_cReBorn.GetReBornNum()*m_cReBorn.GetReBornFanYuMaxVal();
	Npc[m_nIndex].m_CurrentLightResistMax   = BASE_FANGYU_ALL_MAX+m_cReBorn.GetReBornNum()*m_cReBorn.GetReBornFanYuMaxVal();//Npc[m_nIndex].m_LightResistMax;
	Npc[m_nIndex].m_CurrentPhysicsResistMax	= BASE_FANGYU_ALL_MAX+m_cReBorn.GetReBornNum()*m_cReBorn.GetReBornFanYuMaxVal();//Npc[m_nIndex].m_PhysicsResistMax;
	Npc[m_nIndex].m_CurrentPoisonResistMax	= BASE_FANGYU_ALL_MAX+m_cReBorn.GetReBornNum()*m_cReBorn.GetReBornFanYuMaxVal();//Npc[m_nIndex].m_PoisonResistMax;

	Npc[m_nIndex].m_CurrentDamage2Mana	= 0;
	Npc[m_nIndex].m_CurrentDamageReduce	= 0;
	Npc[m_nIndex].m_CurrentDeadlyStrike	= 0;
//	Npc[m_nIndex].m_CurrentElementDamageReduce = 0;
	ZeroMemory(&Npc[m_nIndex].m_CurrentMagicFireDamage, sizeof(KMagicAttrib));
//	ZeroMemory(&Npc[m_nIndex].m_CurrentFireDamage, sizeof(KMagicAttrib));
	Npc[m_nIndex].m_CurrentFireEnhance	= 0;
	
	Npc[m_nIndex].m_CurrentHandEnhance	= 0;
	Npc[m_nIndex].m_CurrentKnockBack	= 0;
	Npc[m_nIndex].m_CurrentLifeStolen	= 0;
	ZeroMemory(&Npc[m_nIndex].m_CurrentMagicLightDamage, sizeof(KMagicAttrib));
//	ZeroMemory(&Npc[m_nIndex].m_CurrentLightDamage, sizeof(KMagicAttrib));
//	ZeroMemory(&Npc[m_nIndex].m_WaiPhysicsDamage, sizeof(KMagicAttrib));
	Npc[m_nIndex].m_CurrentLightEnhance	= 0;
	Npc[m_nIndex].m_CurrentPoisonTime=0;

//	Npc[m_nIndex].m_CurrentManaPerEnemy	= 0;
	Npc[m_nIndex].m_CurrentManaStolen	= 0;
	Npc[m_nIndex].m_CurrentMeleeDmgRet	= 0;        // ���̷���
	Npc[m_nIndex].m_CurrentHulueMeleeDmgRet=0;      // ���Խ��̷������ٵ㡢
   	Npc[m_nIndex].m_CurrentHulueRangeDmgRet=0;      // ����Զ�̷̳������ٵ㡢
	Npc[m_nIndex].m_CurrentMeleeDmgRetPercent = 0;  // ���̷����ٷֱ�
	Npc[m_nIndex].m_CurrentTempSpeed=0;
	ZeroMemory(&Npc[m_nIndex].m_CurrentMeleeEnhance, sizeof(Npc[m_nIndex].m_CurrentMeleeEnhance));

//	Npc[m_nIndex].m_CurrentPiercePercent	= 0;
	ZeroMemory(&Npc[m_nIndex].m_CurrentMagicPoisonDamage, sizeof(KMagicAttrib));
//	ZeroMemory(&Npc[m_nIndex].m_CurrentPoisonDamage, sizeof(KMagicAttrib));
	Npc[m_nIndex].m_CurrentPoisonEnhance	= 0;

	Npc[m_nIndex].m_CurrentRangeDmgRet	= 0;         //Զ�̷���
	Npc[m_nIndex].m_CurrentRangeDmgRetPercent	= 0; //Զ�̷����ٷֱ�
	Npc[m_nIndex].m_CurrentRangeEnhance	= 0;
	Npc[m_nIndex].m_CurrentSlowMissle	= 0;         //�ӵ�����
	Npc[m_nIndex].m_CurrentStaminaStolen	= 0;
	Npc[m_nIndex].m_CurrentAddPhysicsDamage = 0;      //�⹦�յ�
	m_nUpExp							= 0;
	Npc[m_nIndex].m_CurrentUpExp        = 0;
	Npc[m_nIndex].m_CurrentFreezeTimeReducePercent = 0;
	Npc[m_nIndex].m_CurrentPoisonTimeReducePercent = 0;
	Npc[m_nIndex].m_EnemyPoisonTimeReducePercent   = 0;
	Npc[m_nIndex].m_CurrentStunTimeReducePercent = 0;
	Npc[m_nIndex].m_EnemyStunTimeReducePercent   = 0;
	Npc[m_nIndex].m_CurrentSerisesEnhance = 0;
	Npc[m_nIndex].m_CurrentDamageReduce = 0;
	Npc[m_nIndex].m_CurrentdanggeRate = 0;  //����
	Npc[m_nIndex].m_CurrentzhongjiRate = 0; //�ػ�
	 Npc[m_nIndex].m_CurrentcjdanggeRate	= 0;               //��⵵��
	 Npc[m_nIndex].m_CurrentcjzhongjiRate	= 0;               //����ػ�
	 Npc[m_nIndex].m_Currentsorbdamage	= 0;                   //�����˺�
	 Npc[m_nIndex].m_Currentsorbdamage_v=0;
	 Npc[m_nIndex].m_Currenadddamagev   = 0;
	 Npc[m_nIndex].m_Currenadddamagep   = 0;
	 Npc[m_nIndex].m_Currentpoisonres	= 0;                   //=���ԶԷ�����:#d1-%
	 Npc[m_nIndex].m_Currentfireres	= 0;                       //=���ԶԷ����:#d1-%
	 Npc[m_nIndex].m_Currentlightingres	= 0;                   //=���ԶԷ��׷�:#d1-%
	 Npc[m_nIndex].m_Currentphysicsres	= 0;                   //=���ԶԷ��շ�:#d1-%
	 Npc[m_nIndex].m_Currentcoldres	= 0;                       //=���ԶԷ�����:#d1-%
	 Npc[m_nIndex].m_Currentallres	= 0; 
	 Npc[m_nIndex].m_Currentnopkvalue	= 0;                   //=������PKֵ����:#d1+%
	 Npc[m_nIndex].m_Currentbossdamage	= 0;                   //=�Իƽ�boss�����˺�<color=orange>#d1+%<color>
	 Npc[m_nIndex].m_Currentelementsenhance	= 0;               //=����ǿ��ֵ��#d1-�㡣ǿ����������еĿ���Ч��
     Npc[m_nIndex].m_Currentelementsresist	= 0;               //=��������ֵ��#d1-�㡣������������еĿ���Ч��
	 Npc[m_nIndex].m_Currentskillenhance=0;                    //���ܵļӳ�
	  //ZeroMemory(Npc[m_nIndex].m_CurrentSkillEnhance,sizeof(Npc[m_nIndex].m_CurrentSkillEnhance));

	 //std::_EnhanceInfo::iterator it;
	 /*typedef std::map<INT, CEnhanceInfo>::iterator it;
	 for( it = Npc[m_nIndex].nEnhanceInfo.begin(); it != Npc[m_nIndex].nEnhanceInfo.end(); ++it)
	 {
		 it->second.nSkillIdx = 0;
		 it->second.nEnhance = 0;
	 }*/
	 Npc[m_nIndex].nEnhanceInfo.clear();
	 Npc[m_nIndex].m_CurrentFullManaskillenhance=0;
	 Npc[m_nIndex].m_CurrentautoReviverate=0;                  //�������
	 Npc[m_nIndex].m_CurrentAddPhysicsDamageP= 0;     // ��ǰ�������հٷֱ�
	 Npc[m_nIndex].m_CurrentAddFireDamagev= 0;        // ��ǰ��������
	 Npc[m_nIndex].m_CurrentAddColdDamagev= 0;        // ��ǰ��������� 
	 Npc[m_nIndex].m_CurrentAddLighDamagev= 0;        // ��ǰ�������׵�
	 Npc[m_nIndex].m_CurrentAddPoisonDamagev= 0;      // ��ǰ�����ⶾ��
	 Npc[m_nIndex].m_CurrentAddmagicphysicsDamage= 0; // ��ǰ�������յ�
	 Npc[m_nIndex].m_CurrentAddmagicphysicsDamageP= 0;// ��ǰ�������հٷֱ�
	 Npc[m_nIndex].m_CurrentAddmagicColdDamagicv= 0;  // ��ǰ�����ڱ���
	 Npc[m_nIndex].m_CurrentAddmagicFireDamagicv= 0;  // ��ǰ�����ڻ��  
	 Npc[m_nIndex].m_CurrentAddmagicLightDamagicv= 0; // ��ǰ�������׵�
     Npc[m_nIndex].m_CurrentAddmagicPoisonDamagicv= 0;// ��ǰ�����ڶ���
	 Npc[m_nIndex].m_CurrentPoisondamagereturnV=0;
	 Npc[m_nIndex].m_CurrentPoisondamagereturnP=0;
	 Npc[m_nIndex].m_Currentbaopoisondmax_p=0;
	 Npc[m_nIndex].m_CurrentReturnskillp=0;
	 Npc[m_nIndex].m_CurrentIgnoreskillp=0;
	 Npc[m_nIndex].m_CurrentReturnresp=0;
	 Npc[m_nIndex].m_CurrentCreatnpcv=0;
	 Npc[m_nIndex].m_CurrentAllJiHuo=0; //�Ƿ�ȫ����
	 Npc[m_nIndex].m_CurrentCreatStatus=0;
	 Npc[m_nIndex].m_CurrentAttackRatingEnhancep=0;
	 Npc[m_nIndex].m_CurrentAttackRatingEnhancev=0;
	 Npc[m_nIndex].m_CurrentIgnorenAttacRating  =0;
	 Npc[m_nIndex].m_Me2metaldamage_p=0;              //=�Խ�ϵ�˺����ӣ�#d1+%
	 Npc[m_nIndex].m_Metal2medamage_p=0;              //=�������Խ�ϵ���˺���#d1-%
	 Npc[m_nIndex].m_Me2wooddamage_p=0;              //=��ľϵ�˺����ӣ�#d1+%
	 Npc[m_nIndex].m_Wood2medamage_p=0;              //=��������ľϵ���˺���#d1-%
	 Npc[m_nIndex].m_Me2waterdamage_p=0;              //=��ˮϵ�˺����ӣ�#d1+%
	 Npc[m_nIndex].m_Water2medamage_p=0;              //=��������ˮϵ���˺���#d1-%
	 Npc[m_nIndex].m_Me2firedamage_p=0;              //=�Ի�ϵ�˺����ӣ�#d1+%
	 Npc[m_nIndex].m_Fire2medamage_p=0;              //=�������Ի�ϵ���˺���#d1-%
	 Npc[m_nIndex].m_Me2earthdamage_p=0;              //=����ϵ�˺����ӣ�#d1+%
	 Npc[m_nIndex].m_Earth2medamage_p=0;              //=����������ϵ���˺���#d1-%
     Npc[m_nIndex].m_CurrentStunRank_p=0;
	 Npc[m_nIndex].m_Staticmagicshield_p=0;
	 ReCalcEquip();         //���¼������ϵ�װ��
	 ReCalcState(nIsLoign); //���¼��㼼�ܵ�״̬
}

void KPlayer::ReCalcEquip()
{
	//CCAssert(m_nIndex > 0 && m_nIndex < MAX_NPC,"");
	if (m_nIndex <=0 || m_nIndex >= MAX_NPC)
		return;
	
	for (int i = 0; i < itempart_num; ++i)
	{
		int nIdx = m_ItemList.m_EquipItem[i];
		if (!nIdx)
			continue;

		if (Item[nIdx].GetIsCanUse()==0)  //�����õ�
		    continue;

		int nActive = m_ItemList.GetEquipEnhance(i);  //��ȡ���������

		if (m_ItemList.m_bActiveSet)  //����ǻƽ���װ����� ���� ȫ����
		{
			nActive = 3;              //Ĭ�ϵĻƽ�������
		}
//����ƥ
		if (i != itempart_horse || Npc[m_nIndex].m_bRideHorse)  //������Ĳ�λ �� ������
		{
			int nActiveInfo[2];
			
			ZeroMemory(nActiveInfo,sizeof(nActiveInfo));
			
			if (Item[nIdx].IsBlue())
			{//�������װ
				nActiveInfo[0] = 1;
				nActiveInfo[1] = Item[nIdx].GetIsWhere();
				
			} 
			else if  (Item[nIdx].IsPurple())
			{//�������װ
				nActiveInfo[0] = 2;
				nActiveInfo[1] = Item[nIdx].GetIsWhere();
			} 
			else if  (Item[nIdx].IsGold())
			{//����ǻ�װ
				nActiveInfo[0] = 3;
				nActiveInfo[1] = Item[nIdx].GetLevel();//�ȼ�
			} 
			else
			{
				nActiveInfo[0] = 0;
				nActiveInfo[1] = 0;
			}
			
			if (Item[nIdx].GetIsCanUse()==1)   //�ǿ���ʹ�õ�
			{     
			   Item[nIdx].ApplyMagicAttribToNPC(&Npc[m_nIndex], nActive,nActiveInfo);
			}
			else if (Item[nIdx].GetIsCanUse()==0 && Item[nIdx].GetGenre()==item_equip && Item[nIdx].GetDetailType()==equip_mask) 
			{//���
               Item[nIdx].ApplyMagicAttribToNPC(&Npc[m_nIndex], nActive,nActiveInfo);
			}

		}
	}
}

void KPlayer::ReCalcState(int nIsLoign)
{
	if (m_nIndex <=0 || m_nIndex >= MAX_NPC)
		return;
	
	Npc[m_nIndex].ReCalcStateEffect();        //����״̬Ч��

	Npc[m_nIndex].m_SkillList.ReEnChance();   //���üӳ�
	
	if (!nIsLoign)
	   Npc[m_nIndex].NpcNewInitJinMaiVal();   //���þ���״̬  
	       
	Npc[m_nIndex].ReSkillEnhance(m_nLeftSkillID,1); //���öԹ������ܵļӳ�!
}

//-------------------------------------------------------------------------
//	���ܣ�����ͳ��������
//-------------------------------------------------------------------------
void	KPlayer::AddLeadExp(int nExp)
{
	if (m_dwLeadLevel >= MAX_LEAD_LEVEL)
		return;
	if (Npc[m_nIndex].m_Doing == do_death || Npc[m_nIndex].m_Doing == do_revive)
		return;
	m_dwLeadExp += nExp;
	if (m_dwLeadExp >= (DWORD)PlayerSet.m_cLeadExp.GetLevelExp(m_dwLeadLevel))
	{
		// ����
		m_dwLeadLevel = PlayerSet.m_cLeadExp.GetLevel(m_dwLeadExp, m_dwLeadLevel);
		m_dwNextLevelLeadExp = PlayerSet.m_cLeadExp.GetLevelExp(m_dwLeadLevel);
	}	
}

//-------------------------------------------------------------------------
//	���ܣ�ԭʼ�����һ��������
//-------------------------------------------------------------------------
void KPlayer::LevelAddBaseLifeMax()
{
	Npc[m_nIndex].m_LifeMax += PlayerSet.m_cLevelAdd.GetLifePerLevel(Npc[m_nIndex].m_Series);
	Npc[m_nIndex].m_CurrentLifeMax = Npc[m_nIndex].m_LifeMax;
}

//-------------------------------------------------------------------------
//	���ܣ������һ��������
//-------------------------------------------------------------------------
void KPlayer::LevelAddBaseStaminaMax()
{
	Npc[m_nIndex].m_StaminaMax += PlayerSet.m_cLevelAdd.GetStaminaPerLevel(Npc[m_nIndex].m_Series);
	Npc[m_nIndex].m_CurrentStaminaMax = Npc[m_nIndex].m_StaminaMax;
}

//-------------------------------------------------------------------------
//	���ܣ������һ��������
//-------------------------------------------------------------------------
void KPlayer::LevelAddBaseManaMax()
{
	Npc[m_nIndex].m_ManaMax += PlayerSet.m_cLevelAdd.GetManaPerLevel(Npc[m_nIndex].m_Series);
	Npc[m_nIndex].m_CurrentManaMax = Npc[m_nIndex].m_ManaMax;
}

//-------------------------------------------------------------------------
//	���ܣ��ı������Ӫ
//-------------------------------------------------------------------------
void	KPlayer::ChangePlayerCamp(int nCamp)
{
	if (nCamp < camp_begin || nCamp >= camp_num)
		return;
	Npc[m_nIndex].m_Camp = (NPCCAMP)nCamp;
}

//-------------------------------------------------------------------------
//	���ܣ�������������ѯĳ��npc���ڶ������Ϣ
//-------------------------------------------------------------------------
void	KPlayer::ApplyTeamInfo(DWORD dwNpcID)
{
	if (dwNpcID <= 0)
		return;
	PLAYER_APPLY_TEAM_INFO	sApplyInfo;
	sApplyInfo.ProtocolType = (BYTE)c2s_teamapplyinfo;
	sApplyInfo.m_dwTarNpcID = dwNpcID;
	if (g_pClient)
		g_pClient->SendPackToServer(&sApplyInfo, sizeof(PLAYER_APPLY_TEAM_INFO));
}

//-------------------------------------------------------------------------
//	���ܣ���������������������
//-------------------------------------------------------------------------
void	KPlayer::ApplyRoomInfo(int nIsFull)
{
	if (nIsFull < 0)
		return;
	PLAYER_NO_ROOM_INFO	sApplyInfo;
	sApplyInfo.ProtocolType = (BYTE)c2s_roomisfull;
	sApplyInfo.m_IsNoRoom = nIsFull;
	m_nRoomFull           = nIsFull;
	if (g_pClient)
		g_pClient->SendPackToServer(&sApplyInfo, sizeof(PLAYER_NO_ROOM_INFO));
}


//-------------------------------------------------------------------------
//	���ܣ�������������ѯ�������Ķ������
//-------------------------------------------------------------------------
void	KPlayer::ApplySelfTeamInfo()
{
	ApplyTeamInfo(Npc[m_nIndex].m_dwID);
}



//-------------------------------------------------------------------------
//	���ܣ��������������봴������
//-------------------------------------------------------------------------
BOOL	KPlayer::ApplyCreateTeam()//char *lpszTeamName)
{
	return m_cTeam.ApplyCreate();//lpszTeamName);
}

//-------------------------------------------------------------------------
//	���ܣ��ӳ�����������뿪�š��رն����Ƿ���������Ա״̬
//-------------------------------------------------------------------------
BOOL	KPlayer::ApplyTeamOpenClose(BOOL bFlag)
{
	if (!m_cTeam.m_nFlag )					// δ���
		return FALSE;
	if (m_cTeam.m_nFigure != TEAM_CAPTAIN)	// ���Ƕӳ�
		return FALSE;
	if (this->CheckTrading())	// ���ڽ���״̬
		return FALSE;
	
	if (bFlag)
	{
		if (PlayerSet.m_cLeadExp.GetMemNumFromLevel(m_dwLeadLevel) <= g_Team[0].m_nMemNum)	// ͳ˧������
			return FALSE;
	}
	
	PLAYER_TEAM_OPEN_CLOSE	sTeamState;
	sTeamState.ProtocolType = c2s_teamapplyopenclose;
	sTeamState.m_btOpenClose = bFlag;
	if (g_pClient)
		g_pClient->SendPackToServer(&sTeamState, sizeof(PLAYER_TEAM_OPEN_CLOSE));
	return TRUE;
}

//-------------------------------------------------------------------------
//	���ܣ������������������ĳ������
//	������int nNpcIndex Ŀ�����ӳ��ڿͻ���Npc�����е�λ��
//-------------------------------------------------------------------------
void	KPlayer::ApplyAddTeam(int nNpcIndex)
{
	if (this->CheckTrading())
		return;

	if (m_cTeam.m_nFlag)					// �Ѿ���ӣ�������������Ķ���
		return;
	
	// ��ͬ��Ӫ���ڿ�����ӣ����ֲ��ܼ������˶��飬���˿��Լ������ֶ���
	if (Npc[nNpcIndex].m_Camp == camp_begin && Npc[m_nIndex].m_Camp != camp_begin)
		return;
	//	if (Npc[nNpcIndex].m_Camp != Npc[m_nIndex].m_Camp)	// ��Ӫ��ͬ
	//		return;
	
	// �ж�Ŀ�����ӳ��ǲ��Ǵ��ڶ��鿪��״̬
	if (Npc[nNpcIndex].GetMenuState() != PLAYER_MENU_STATE_TEAMOPEN)
	{
		m_ItemList.ClientShowMsg("�Է����鴦�ڹر�״̬!");
		return;
	}
	
	m_cTeam.m_nApplyCaptainID = Npc[nNpcIndex].m_dwID;
	m_cTeam.m_dwApplyTimer = MAX_APPLY_TEAM_TIME;
	PLAYER_APPLY_ADD_TEAM	sAddTeam;
	sAddTeam.ProtocolType = c2s_teamapplyadd;
	sAddTeam.m_dwTarNpcID = Npc[nNpcIndex].m_dwID;
	if (g_pClient)
		g_pClient->SendPackToServer(&sAddTeam, sizeof(PLAYER_APPLY_ADD_TEAM));
	
	KSystemMessage	sMsg;
	t_sprintf(sMsg.szMessage,strCoreInfo[MSG_TEAM_APPLY_ADD_SELF_MSG].c_str(), Npc[nNpcIndex].Name);
	sMsg.eType = SMT_NORMAL;
	sMsg.byConfirmType = SMCT_NONE;
	sMsg.byPriority = 0;
	sMsg.byParamSize = 0;
	sMsg.nMsgLen = TEncodeText_(sMsg.szMessage, strlen(sMsg.szMessage));
	CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (uintptr_t)&sMsg, 0);
}

//-------------------------------------------------------------------------
//	���ܣ����֪ͨ����������ĳ��npcΪ�����Ա
//-------------------------------------------------------------------------
void	KPlayer::AcceptTeamMember(DWORD dwNpcID)
{
	// �ж�״̬ ��ȱ�ٶ�npc���鿪��״̬�ļ�� not end��
	if ( !m_cTeam.m_nFlag || m_cTeam.m_nFigure != TEAM_CAPTAIN)
	{
		//ccMessageBox("���Ƕӳ�","error");
		ApplySelfTeamInfo();
		return;
	}
	if (dwNpcID == 0)
	{
		// ccMessageBox("dwNpcID = 0","ͬ�����");
		return;
	}
	
	// ���������б�
	int		i;
	for (i = 0; i < MAX_TEAM_APPLY_LIST; ++i)
	{
		if (m_cTeam.m_sApplyList[i].m_dwNpcID == dwNpcID)
		{
			m_cTeam.m_sApplyList[i].m_dwNpcID = 0;
			m_cTeam.UpdateInterface();
			break;
		}
	}
	// ���û�ҵ�
//	if (i >= MAX_TEAM_APPLY_LIST)
//		return;
	//char msg[64];
	/*int  nindex = NpcSet.SearchID(dwNpcID);
	if  (nindex>0)
	   ccMessageBox(Npc[nindex].Name,"ͬ�����");*/
	// �������������
	PLAYER_ACCEPT_TEAM_MEMBER	sAcceptTeam;
	sAcceptTeam.ProtocolType = c2s_teamacceptmember;
	sAcceptTeam.m_dwNpcID    = dwNpcID;
	if (g_pClient)
		g_pClient->SendPackToServer(&sAcceptTeam, sizeof(PLAYER_ACCEPT_TEAM_MEMBER));
}

//-------------------------------------------------------------------------
//	���ܣ��ӳ�ɾ��������������б��е�ĳ��npc
//-------------------------------------------------------------------------
void	KPlayer::TeamDropApplyOne(DWORD dwNpcID)
{
	if (dwNpcID == 0)
		return;
	// ���������б�
	int		i;
	for (i = 0; i < MAX_TEAM_APPLY_LIST; ++i)
	{
		if (m_cTeam.m_sApplyList[i].m_dwNpcID == dwNpcID)
		{
			m_cTeam.m_sApplyList[i].m_dwNpcID = 0;
			m_cTeam.m_sApplyList[i].m_dwTimer = 0;
			m_cTeam.m_sApplyList[i].m_nLevel = 0;
			m_cTeam.m_sApplyList[i].m_szName[0] = 0;
			break;
		}
	}
}

//-------------------------------------------------------------------------
//	���ܣ�֪ͨ������������뿪����
//-------------------------------------------------------------------------
void	KPlayer::LeaveTeam()
{
	PLAYER_APPLY_LEAVE_TEAM	sLeaveTeam;
	sLeaveTeam.ProtocolType = c2s_teamapplyleave;
	if (g_pClient)
		g_pClient->SendPackToServer(&sLeaveTeam, sizeof(PLAYER_APPLY_LEAVE_TEAM));
}

//-------------------------------------------------------------------------
//	���ܣ��ӳ�֪ͨ�������߳�ĳ����Ա
//-------------------------------------------------------------------------
void	KPlayer::TeamKickMember(DWORD dwNpcID)
{
	if ( !m_cTeam.m_nFlag || m_cTeam.m_nFigure != TEAM_CAPTAIN)
		return;
	
	PLAYER_TEAM_KICK_MEMBER	sKickOne;
	sKickOne.ProtocolType = c2s_teamapplykickmember;
	sKickOne.m_dwNpcID = dwNpcID;
	if (g_pClient)
		g_pClient->SendPackToServer(&sKickOne, sizeof(PLAYER_TEAM_KICK_MEMBER));
}

//-------------------------------------------------------------------------
//	���ܣ��ӳ��������������Լ��Ķӳ���ݽ�����Ķ�Ա
//-------------------------------------------------------------------------
void	KPlayer::ApplyTeamChangeCaptain(DWORD dwNpcID)
{
	if ( !m_cTeam.m_nFlag || m_cTeam.m_nFigure != TEAM_CAPTAIN)
		return;
	
	PLAYER_APPLY_TEAM_CHANGE_CAPTAIN	sTeamChange;
	sTeamChange.ProtocolType = c2s_teamapplychangecaptain;
	sTeamChange.m_dwNpcID = dwNpcID;
	if (g_pClient)
		g_pClient->SendPackToServer(&sTeamChange, sizeof(PLAYER_APPLY_TEAM_CHANGE_CAPTAIN));
}

//-------------------------------------------------------------------------
//	���ܣ��ӳ�������������ɢ����
//-------------------------------------------------------------------------
void	KPlayer::ApplyTeamDismiss()
{
	if ( !m_cTeam.m_nFlag || m_cTeam.m_nFigure != TEAM_CAPTAIN)
		return;
	
	PLAYER_APPLY_TEAM_DISMISS	sTeamDismiss;
	sTeamDismiss.ProtocolType = c2s_teamapplydismiss;
	if (g_pClient)
		g_pClient->SendPackToServer(&sTeamDismiss, sizeof(PLAYER_APPLY_TEAM_DISMISS));
}

//-------------------------------------------------------------------------
//	���ܣ���������������򿪡��ر�pk����
//-------------------------------------------------------------------------
void	KPlayer::ApplySetPK(BOOL bPK)
{
	PLAYER_SET_PK	sPK;
	sPK.ProtocolType = c2s_playerapplysetpk;
	sPK.m_btPKFlag = bPK;
	if (g_pClient)
		g_pClient->SendPackToServer(&sPK, sizeof(PLAYER_SET_PK));
}

//-------------------------------------------------------------------------
//	���ܣ�����������������������
//-------------------------------------------------------------------------
void	KPlayer::ApplyFactionData()
{
	PLAYER_APPLY_FACTION_DATA	sFaction;
	sFaction.ProtocolType = c2s_playerapplyfactiondata;
	if (g_pClient)
		g_pClient->SendPackToServer(&sFaction, sizeof(PLAYER_APPLY_FACTION_DATA));
}

//-------------------------------------------------------------------------
//	���ܣ��ͻ��˷�����������������
//-------------------------------------------------------------------------
void	KPlayer::SendChat(KUiMsgParam *pMsg, char *lpszSentence)
{
	if (!lpszSentence || !pMsg || pMsg->nMsgLength <= 0)
		return;
	int nLength = pMsg->nMsgLength;
	if (nLength >= MAX_SENTENCE_LENGTH)
		nLength = MAX_SENTENCE_LENGTH - 1;
	
	if (pMsg->eGenre == MSG_G_CHAT)
	{
		Npc[m_nIndex].SetChatInfo(pMsg->szName, lpszSentence, nLength);  //�����Լ��������� ��ֵ�� m_szChatBuffer
		//char m_szChatBuffer[256]={0};
		//Npc[m_nIndex].GetstrInfo(STR_CHATBUFF_CLIENT,m_szChatBuffer);
//		m_cChat.SendSentence(pMsg,Npc[m_nIndex].m_szChatBuffer);        //���͸�������
	}
	else if (pMsg->eGenre == MSG_G_CMD)	// not end
	{

	}
}

//-------------------------------------------------------------------------
//	���ܣ��ӳ��������������������������ĳһ��ĵ���(0=Strength 1=Dexterity 2=Vitality 3=Engergy)
//-------------------------------------------------------------------------
void	KPlayer::ApplyAddBaseAttribute(int nAttribute, int nNo)
{
	if (nAttribute < 0 || nAttribute > 3)
		return;
	if (nNo <= 0 || nNo >= 255)
		return;
	PLAYER_ADD_BASE_ATTRIBUTE_COMMAND	sAdd;
	sAdd.ProtocolType = c2s_playeraddbaseattribute;
	sAdd.m_btAttribute = nAttribute;
	sAdd.m_nAddNo = nNo;
	if (g_pClient)
		g_pClient->SendPackToServer(&sAdd, sizeof(PLAYER_ADD_BASE_ATTRIBUTE_COMMAND));
}

//-------------------------------------------------------------------------
//	���ܣ������������ĳ����������
//-------------------------------------------------------------------------
BOOL	KPlayer::ApplyAddSkillLevel(int nSkillID, int nAddPoint)
{
	if ( !Npc[m_nIndex].m_SkillList.FindSame(nSkillID)) //�����Ƿ����������
		return FALSE;
	if (m_nSkillPoint < nAddPoint || nAddPoint + Npc[m_nIndex].m_SkillList.GetLevel(nSkillID) >=MAX_SKILLLEVEL)
		return FALSE;
	
	PLAYER_ADD_SKILL_POINT_COMMAND	sSkill;
	sSkill.ProtocolType = c2s_playerapplyaddskillpoint;
	sSkill.m_nSkillID = nSkillID;
	sSkill.m_nAddPoint= nAddPoint; //���ӵļ��ܵ�
	
	if (g_pClient)
		g_pClient->SendPackToServer(&sSkill, sizeof(PLAYER_ADD_SKILL_POINT_COMMAND));
	
	return TRUE;
}
//�Զ����潻������,��������
BOOL	KPlayer::AutoChangeItem(int nItemID, ItemPos SrcPos,ItemPos DestPos)
{

}
//-------------------------------------------------------------------------
//	���ܣ������������ʹ��ĳ����Ʒ������Ҽ��������Ʒ��ֻ�����ڳ�ҩ��
//-------------------------------------------------------------------------
BOOL	KPlayer::ApplyUseItem(int nItemID, ItemPos SrcPos)
{
	if (this->CheckTrading())
		return FALSE;

	int nRet = m_ItemList.UseItem(nItemID,SrcPos.nX,SrcPos.nY);  //ͬ��ʹ����Ʒ
	if (nRet == 0)
		return FALSE;
	
	if (nRet == REQUEST_EQUIP_ITEM)         //��Ʒװ��---�Ҽ���װ-����
	{//����װ��
	}
	else if (nRet == REQUEST_EAT_MEDICINE)  // ҩƷ
	{
		PLAYER_EAT_ITEM_COMMAND	sEat;
		sEat.ProtocolType = c2s_playereatitem;
		sEat.m_nItemID    = nItemID;
		sEat.m_btPlace    = SrcPos.nPlace;
		sEat.m_btX        = SrcPos.nX;
		sEat.m_btY        = SrcPos.nY;
		//ccMessageBox("ʹ����Ʒ","��ʾ:");
		if (g_pClient)
			g_pClient->SendPackToServer(&sEat, sizeof(PLAYER_EAT_ITEM_COMMAND));
	}
	
	return TRUE;
}
//�ͻ���Ҫ���ƶ���Ʒ
BOOL KPlayer::ApplyAutoMoveItem(int nItemID, ItemPos SrcPos,int destRoom)
{
	   //if (this->CheckTrading())
	  //	return FALSE;

		PLAYER_AUTOMUVE_ITEM_COMMAND	sMoveItem;
		sMoveItem.ProtocolType = c2s_playerAutoMoveitem;
		sMoveItem.m_nItemID    = Item[nItemID].GetID();//nItemdwid;
		sMoveItem.m_btPlace    = SrcPos.nPlace;
		sMoveItem.m_btX        = SrcPos.nX;
		sMoveItem.m_btY        = SrcPos.nY;
        sMoveItem.m_destRoom   = destRoom;
		sMoveItem.m_ItemWidth  = Item[nItemID].GetWidth();
        sMoveItem.m_ItemHigth  = Item[nItemID].GetHeight();

		if (g_pClient)
			g_pClient->SendPackToServer(&sMoveItem, sizeof(PLAYER_AUTOMUVE_ITEM_COMMAND));

	return TRUE;
}


//-------------------------------------------------------------------------
//	���ܣ��ͻ��������obj����ĳ����Ʒ�������������Ϣ
//-------------------------------------------------------------------------
void	KPlayer::PickUpObj(int nObjIndex)
{
	if (this->CheckTrading())
		return;
	if (nObjIndex <= 0)
		return;
	if (Object[nObjIndex].m_nKind != Obj_Kind_Item && Object[nObjIndex].m_nKind != Obj_Kind_Money)
		return;
	
	PLAYER_PICKUP_ITEM_COMMAND	sPickUp;
	if (Object[nObjIndex].m_nKind == Obj_Kind_Money)
	{//Ǯ
		sPickUp.ProtocolType = c2s_playerpickupitem;
		sPickUp.m_nObjID = Object[nObjIndex].m_nID;
		sPickUp.m_btPosType = 0;
		sPickUp.m_btPosX = 0;
		sPickUp.m_btPosY = 0;

		 if (Npc[m_nIndex].m_AutoplayId==1)
		 {//�һ�״̬			
			if (m_Autoplay.nShiQuNum>m_Autoplay.nVal && m_Autoplay.nShiQuNum%m_Autoplay.nVal==0)
			{
                Object[nObjIndex].m_AttackerDwid=Npc[m_nIndex].m_dwID;
                Object[nObjIndex].m_IsHaveAttack=1;
			    Object[nObjIndex].m_AttackerTime=18*60*10;
				m_Autoplay.nShiQuNum=0;
				m_ItemList.ClientShowMsg("����:��ȡ��Ʒ��ʱ,ִ��ȡ����ȡ!");
				return;
			}
			  m_Autoplay.nShiQuNum++;
		 }
		 else
		 {
		      m_Autoplay.nShiQuNum=0;
		 }
	}
	else
	{//��������Ʒ
		ItemPos	sItemPos;
		if (FALSE == m_ItemList.SearchPosition(Object[nObjIndex].m_nItemWidth, Object[nObjIndex].m_nItemHeight, &sItemPos))
		{
			if (FALSE == m_ItemList.SearchItemPosition(Object[nObjIndex].m_nItemWidth, Object[nObjIndex].m_nItemHeight, &sItemPos,room_equipmentex))
			{
				if (Npc[m_nIndex].m_AutoplayId==1)
				{ //�һ�״̬
					Object[nObjIndex].m_AttackerDwid=Npc[m_nIndex].m_dwID;
					Object[nObjIndex].m_IsHaveAttack=1;
					Object[nObjIndex].m_AttackerTime=18*60*10;
					//��ʼ����������߷���������������
					ApplyRoomInfo(1);
				}
				
				m_ItemList.ClientShowMsg("��ʾ:ת����ĸ��ʧ��,�ռ䲻�㣡");
				return;
			}
			
			int nItemEX = Npc[m_nIndex].m_ExItemId; //��ĸ���Ƿ���
			
			if (nItemEX==0)
			{
				if (Npc[m_nIndex].m_AutoplayId==1)
				{ //�һ�״̬
					Object[nObjIndex].m_AttackerDwid=Npc[m_nIndex].m_dwID;
					Object[nObjIndex].m_IsHaveAttack=1;
					Object[nObjIndex].m_AttackerTime=18*60*10;
					//��ʼ����������߷���������������
					ApplyRoomInfo(1);
				}
				
				m_ItemList.ClientShowMsg("��ʾ:ת����ĸ��ʧ��,��ĸ����δ������");
				return;
			}	

		}
	
		 if (Npc[m_nIndex].m_AutoplayId==1)
		 {//�һ�״̬			
			if (m_Autoplay.nShiQuNum>m_Autoplay.nVal && m_Autoplay.nShiQuNum%m_Autoplay.nVal==0)
			{
                Object[nObjIndex].m_AttackerDwid=Npc[m_nIndex].m_dwID;
                Object[nObjIndex].m_IsHaveAttack=1;
			    Object[nObjIndex].m_AttackerTime=18*60*10;
				m_Autoplay.nShiQuNum=0;
				m_ItemList.ClientShowMsg("����:��ȡ��Ʒ��ʱ,ִ��ȡ����ȡ!");
				return;
			}
			  m_Autoplay.nShiQuNum++;
		 }
		 else
		 {
		      m_Autoplay.nShiQuNum=0;
		 }

/*		  char msg[64];
          t_sprintf(msg,"����:��ֹ��:%d,dwidx:%d",Object[nObjIndex].m_IsHaveAttack,Object[nObjIndex].m_AttackerDwid);
          m_ItemList.ClientShowMsg(msg);
*/
		sPickUp.ProtocolType = c2s_playerpickupitem;
		sPickUp.m_nObjID = Object[nObjIndex].m_nID;
		sPickUp.m_btPosType = sItemPos.nPlace;
		sPickUp.m_btPosX = sItemPos.nX;  //����������λ��
		sPickUp.m_btPosY = sItemPos.nY;  //����������λ��
	}
	
	if (g_pClient)
		g_pClient->SendPackToServer(&sPickUp, sizeof(PLAYER_PICKUP_ITEM_COMMAND));
}

//-------------------------------------------------------------------------
//	���ܣ��ͻ��������obj����ĳ��Ǯ�������������Ϣ
//-------------------------------------------------------------------------
void	KPlayer::PickUpMoney(int nObjIndex,int rid)
{
	if (this->CheckTrading())
		return;
	if (nObjIndex <= 0)
		return;
	if (Object[nObjIndex].m_nKind != Obj_Kind_Money)
	{
      Npc[rid].m_nObjectIdx=0;
	  return;
	}
	//m_ItemList.ClientShowMsg("���Թ����Ǯ����");
	PLAYER_PICKUP_ITEM_COMMAND	sPickUp; //����
	if (Object[nObjIndex].m_nKind == Obj_Kind_Money) //���ڵ��ϵĶ�������Ϊ==Ǯ
	{
		sPickUp.ProtocolType = c2s_playerpickupitem; //����Э���ʶ
		sPickUp.m_nObjID = Object[nObjIndex].m_nID;
		sPickUp.m_btPosType = 0;
		sPickUp.m_btPosX = 0;
		sPickUp.m_btPosY = 0;

		m_Autoplay.nShiQuNum=0;
	}
	
	if (g_pClient)
		g_pClient->SendPackToServer(&sPickUp, sizeof(PLAYER_PICKUP_ITEM_COMMAND));
}

//-------------------------------------------------------------------------
//	���ܣ��ͻ��������obj����ĳ��װ���������������Ϣ
//-------------------------------------------------------------------------
void	KPlayer::PickUpItem(int nObjIndex,int rid)
{
	if (this->CheckTrading())
		return;
	if (nObjIndex <= 0)
		return;
	if (Object[nObjIndex].m_nKind != Obj_Kind_Item)
	{
		Npc[rid].m_nObjectIdx=0;
		return;
	}
	PLAYER_PICKUP_ITEM_COMMAND	sPickUp; //����
	if (Object[nObjIndex].m_nKind == Obj_Kind_Item) //���ڵ��ϵĶ�������Ϊ==װ��
	{
		ItemPos	sItemPos;

		if (FALSE == m_ItemList.SearchPosition(Object[nObjIndex].m_nItemWidth, Object[nObjIndex].m_nItemHeight, &sItemPos) )
		{//������
			if (FALSE == m_ItemList.SearchItemPosition(Object[nObjIndex].m_nItemWidth, Object[nObjIndex].m_nItemHeight, &sItemPos,room_equipmentex))
			{
				if (Npc[m_nIndex].m_AutoplayId==1)
				{ //�һ�״̬
					Object[nObjIndex].m_AttackerDwid=Npc[m_nIndex].m_dwID;
					Object[nObjIndex].m_IsHaveAttack=1;
					Object[nObjIndex].m_AttackerTime=18*60*10;
					//��ʼ����������߷���������������
					ApplyRoomInfo(1);
				}

				m_ItemList.ClientShowMsg("��ʾ:ת����ĸ��ʧ��,�ռ䲻�㣡");
				return;
			}

			int nItemEX = Npc[m_nIndex].m_ExItemId; //��ĸ���Ƿ���

          	if (nItemEX==0)
			{
				if (Npc[m_nIndex].m_AutoplayId==1)
				{ //�һ�״̬
					Object[nObjIndex].m_AttackerDwid=Npc[m_nIndex].m_dwID;
					Object[nObjIndex].m_IsHaveAttack=1;
					Object[nObjIndex].m_AttackerTime=18*60*10;
					//��ʼ����������߷���������������
					ApplyRoomInfo(1);
				}

				m_ItemList.ClientShowMsg("��ʾ:ת����ĸ��ʧ��,��ĸ����δ������");
				return;
			}	
		}

         if (Npc[m_nIndex].m_AutoplayId==1)
		 {//�һ�״̬			
			if (m_Autoplay.nShiQuNum>m_Autoplay.nVal && m_Autoplay.nShiQuNum%m_Autoplay.nVal==0)
			{
                Object[nObjIndex].m_AttackerDwid=Npc[m_nIndex].m_dwID;
                Object[nObjIndex].m_IsHaveAttack=1;
			    Object[nObjIndex].m_AttackerTime=18*60*10;
				m_Autoplay.nShiQuNum=0;
				m_ItemList.ClientShowMsg("����:��ȡ��Ʒ��ʱ,ִ��ȡ����ȡ!");
				return;
			}
			  m_Autoplay.nShiQuNum++;
		 }
		 else
		 {
		      m_Autoplay.nShiQuNum=0;
		 }

		sPickUp.ProtocolType = c2s_playerpickupitem;
		sPickUp.m_nObjID = Object[nObjIndex].m_nID;
		sPickUp.m_btPosType = sItemPos.nPlace;  //Ŀ������
		sPickUp.m_btPosX = sItemPos.nX;         //Ŀ��λ��
		sPickUp.m_btPosY = sItemPos.nY;
	}
	
	if (g_pClient)
		g_pClient->SendPackToServer(&sPickUp, sizeof(PLAYER_PICKUP_ITEM_COMMAND));
}

//-------------------------------------------------------------------------
//	���ܣ��ͻ��������obj�������������Ϣ   ִ�нű�
//-------------------------------------------------------------------------
void	KPlayer::ObjMouseClick(int nObjIndex)
{
	if (this->CheckTrading())
		return;
	if (nObjIndex <= 0)
		return;
	if (Object[nObjIndex].m_nKind != Obj_Kind_Box && Object[nObjIndex].m_nKind != Obj_Kind_Prop)
		return;
	if (Object[nObjIndex].m_nRegionIdx<0)
		return;

	SendObjMouseClick(Object[nObjIndex].m_nID, SubWorld[0].m_Region[Object[nObjIndex].m_nRegionIdx].m_RegionID);
}

// DownPos Vi tri can chuyen den��UpPos vi tri goc
BOOL	KPlayer::MoveItem(ItemPos DownPos, ItemPos UpPos,int nIsComp)
{
	
	//Check vi tri can chuyen toi co item ko (DownPos)
	if (!Player[CLIENT_PLAYER_INDEX].m_ItemList.SearchItemPositionKienTM(1,1,DownPos.nX,DownPos.nY,room_equipment)){
		//Neu vi tri can chuyen toi (DownPos) da co item thi chuyen lai item ve vi tri cu (UpPos)
		//Player[CLIENT_PLAYER_INDEX].m_ItemList.AutoMoveItem(DownPos,UpPos,nIsComp);
		return FALSE;
	}
	
	SendClientCmdMoveItem(&UpPos,&DownPos,nIsComp); //DownPos Ŀ������
	return TRUE;
}

// DownPos ������ϵ���Ʒ�ĵ�ǰ���꣬UpPos ������������Ʒ�ŵ�����ϵ�����
void	KPlayer::MoveSkill(ItemPos DownPos, ItemPos UpPos)
{
	if (!CheckTrading() && DownPos.nPlace == pos_immediacy)
	{
		int nHandIdx = m_ItemList.Hand();
		if (nHandIdx)
		{
			if (m_ItemList.m_Room[room_immediacy].CheckSameDetailType(Item[nHandIdx].GetGenre(), Item[nHandIdx].GetDetailType(),Item[nHandIdx].GetParticular()))
			{
				KSystemMessage	sMsg;
				sMsg.eType = SMT_NORMAL;
				sMsg.byConfirmType = SMCT_NONE;
				sMsg.byPriority = 0;
				sMsg.byParamSize = 0;
				t_sprintf(sMsg.szMessage, "M?o: Thanh ph��m t?t ?? c�� k? n?ng t??ng t?��");
				sMsg.nMsgLen = TEncodeText(sMsg.szMessage, strlen(sMsg.szMessage));
				CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (uintptr_t)&sMsg, 0);
				return;
			}
		}
	}
	
//	SendClientCmdMoveItem(&DownPos, &UpPos);
}

//v?t b? v?t ph?m v?t b? v?t ph?m
int	KPlayer::ThrowAwayItem()
{
	if (this->CheckTrading())
	{
	    Player[CLIENT_PLAYER_INDEX].m_ItemList.ClientShowMsg("M?o: Trong qu�� tr��nh giao d?ch, b?n kh?ng ???c b? v?t ph?m!");
		return 0;
	}

	if (!m_ItemList.Hand())
	{

	    //Player[CLIENT_PLAYER_INDEX].m_ItemList.ClientShowMsg("��ƷΪ�գ�������");
		return 0;
	}
/*	else
	{
	   char msg[64];
	   t_sprintf(msg,"��Ʒ����:%d..",m_ItemList.Hand());
	   Player[CLIENT_PLAYER_INDEX].m_ItemList.ClientShowMsg(msg);
	}*/
	int	nIdx =m_ItemList.Hand();

	if (/*Item[nIdx].GetTime()->bYear>0 || */Item[nIdx].GetIsBang())
	{
       int nReg=0;
	       g_GameSetTing.GetInteger("SYSTEM","ThrowAwayItem",1,&nReg);	//�����Ƿ����ö�������

     if (!nReg)
	 { 
		KSystemMessage	sMsg;
		t_sprintf(sMsg.szMessage,"����:(��ʱ/��)װ�����ܶ���!");
		sMsg.eType = SMT_NORMAL;
		sMsg.byConfirmType = SMCT_NONE;
		sMsg.byPriority = 0;
		sMsg.byParamSize = 0;
		sMsg.nMsgLen = TEncodeText(sMsg.szMessage, strlen(sMsg.szMessage));
		CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (uintptr_t)&sMsg, 0);
	    return 0;
	 } 
	}

	PLAYER_THROW_AWAY_ITEM_COMMAND	sThrow;
	
  sThrow.ProtocolType = c2s_playerthrowawayitem;
  sThrow.m_Kind=0;
  sThrow.m_isAttack =0;
  if (Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_AutoplayId==1)
  {
    sThrow.m_ItemDwidx=Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_dwID;
	sThrow.m_IsAuToFenJie=m_Autoplay.nIsAuToFenJie;
  }
  else
  {
    sThrow.m_ItemDwidx=0;
	sThrow.m_IsAuToFenJie=0;
  }
	if (g_pClient)
		g_pClient->SendPackToServer(&sThrow, sizeof(PLAYER_THROW_AWAY_ITEM_COMMAND));
	
	return 1;
}


int	KPlayer::AutoAwayItem(DWORD nItmeDwIdx,bool isAttack)
{

    PLAYER_THROW_AWAY_ITEM_COMMAND	sThrow;
	
	sThrow.ProtocolType = c2s_playerthrowawayitem;
	sThrow.m_Kind=1;
    sThrow.m_ItemDwidx=nItmeDwIdx;
	sThrow.m_IsAuToFenJie=m_Autoplay.nIsAuToFenJie;
	sThrow.m_isAttack =isAttack;
	if (g_pClient)
		g_pClient->SendPackToServer(&sThrow, sizeof(PLAYER_THROW_AWAY_ITEM_COMMAND));
	
	return 1;
}

void	KPlayer::ChatAddFriend(int nPlayerIdx)
{
	CHAT_ADD_FRIEND_COMMAND	sAdd;
	sAdd.ProtocolType = c2s_chataddfriend;
	sAdd.m_nTargetPlayerIdx = nPlayerIdx;
	if (g_pClient)
		g_pClient->SendPackToServer(&sAdd, sizeof(CHAT_ADD_FRIEND_COMMAND));
}

void	KPlayer::ChatRefuseFriend(int nPlayerIdx)
{
	CHAT_REFUSE_FRIEND_COMMAND	sRefuse;
	sRefuse.ProtocolType = c2s_chatrefusefriend;
	sRefuse.m_nTargetPlayerIdx = nPlayerIdx;
	if (g_pClient)
		g_pClient->SendPackToServer(&sRefuse, sizeof(CHAT_REFUSE_FRIEND_COMMAND));
}


void	KPlayer::TradeApplyOpen(char *lpszSentence, int nLength)
{
	if (this->CheckTrading())
		return;

	if (!lpszSentence)
		nLength = 0;

	TRADE_APPLY_OPEN_COMMAND	sOpen;
	sOpen.ProtocolType = c2s_tradeapplystateopen;
	if (nLength >= MAX_SENTENCE_LENGTH)
		nLength = MAX_SENTENCE_LENGTH - 1;
	sOpen.m_wLength = sizeof(TRADE_APPLY_OPEN_COMMAND) - 1 - sizeof(sOpen.m_szSentence) + nLength;
	memset(m_cMenuState.m_szSentence, 0, sizeof(m_cMenuState.m_szSentence));
	if (lpszSentence)
		memcpy(m_cMenuState.m_szSentence, lpszSentence, nLength);

	memset(sOpen.m_szSentence, 0, sizeof(sOpen.m_szSentence));
	memcpy(sOpen.m_szSentence, m_cMenuState.m_szSentence, nLength);
	if (g_pClient)
		g_pClient->SendPackToServer(&sOpen, sOpen.m_wLength + 1);
}

void	KPlayer::TradeApplyClose()
{
	TRADE_APPLY_CLOSE_COMMAND	sApply;
	sApply.ProtocolType = c2s_tradeapplystateclose;
	if (g_pClient)
		g_pClient->SendPackToServer(&sApply, sizeof(TRADE_APPLY_CLOSE_COMMAND));
}

void	KPlayer::TradeApplyStart(int nNpcIdx)
{
	if (nNpcIdx < 0 || nNpcIdx >= MAX_NPC)
		return;
	TRADE_APPLY_START_COMMAND	sStart;
	sStart.ProtocolType = c2s_tradeapplystart;
	sStart.m_dwID       = Npc[nNpcIdx].m_dwID;
	if (g_pClient)
		g_pClient->SendPackToServer(&sStart, sizeof(TRADE_APPLY_START_COMMAND));
	
	KSystemMessage	sMsg;
	t_sprintf(sMsg.szMessage, strCoreInfo[MSG_TRADE_SEND_APPLY].c_str(), Npc[nNpcIdx].Name);
	sMsg.eType = SMT_NORMAL;
	sMsg.byConfirmType = SMCT_NONE;
	sMsg.byPriority = 0;
	sMsg.byParamSize = 0;
	sMsg.nMsgLen = TEncodeText_(sMsg.szMessage, strlen(sMsg.szMessage));
	CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (uintptr_t)&sMsg, 0);
}

BOOL	KPlayer::TradeMoveMoney(int nMoney)
{
	if (!CheckTrading())
		return FALSE;
	if (this->m_cTrade.m_nTradeLock)
		return FALSE;
	// Ǯ��������
	if (nMoney < 0 || nMoney > m_ItemList.GetEquipmentMoney())
		return FALSE;
	
	TRADE_MOVE_MONEY_COMMAND	sMoney;
	
	sMoney.ProtocolType = c2s_trademovemoney;
	sMoney.m_nMoney = nMoney;
	if (g_pClient)
		g_pClient->SendPackToServer(&sMoney, sizeof(TRADE_MOVE_MONEY_COMMAND));
	
	return TRUE;
}

//-------------------------------------------------------------------------
//	���ܣ�����ȷ����ȡ��
//	if nDecision == 0 �Ƴ�����  if nDecision == 1 ȷ������  if nDecision == 2 ȡ������ȷ��
//-------------------------------------------------------------------------
void	KPlayer::TradeDecision(int nDecision)
{
	if ( !CheckTrading() )
		return;
	TRADE_DECISION_COMMAND	sDecision;
	sDecision.ProtocolType = c2s_tradedecision;
	if (nDecision == 1)
	{
		sDecision.m_btDecision = 1;
	}
	else if (nDecision == 0)
	{
		sDecision.m_btDecision = 0;
	}
	else if (nDecision == 2)
	{
		sDecision.m_btDecision = 4;
	}
	if (g_pClient)
		g_pClient->SendPackToServer(&sDecision, sizeof(TRADE_DECISION_COMMAND));
}

//-------------------------------------------------------------------------
//	���ܣ�����������ȡ������
//-------------------------------------------------------------------------
void	KPlayer::TradeApplyLock(int nLockOrNot)
{
	if ( !CheckTrading() )
		return;
	TRADE_DECISION_COMMAND	sDecision;
	sDecision.ProtocolType = c2s_tradedecision;
	if (nLockOrNot)
	{
		sDecision.m_btDecision = 2;
	}
	else
	{
		sDecision.m_btDecision = 3;
	}
	if (g_pClient)
		g_pClient->SendPackToServer(&sDecision, sizeof(TRADE_DECISION_COMMAND));
}

//-------------------------------------------------------------------------
//	���ܣ��յ�������֪ͨ��������������
//-------------------------------------------------------------------------
void	KPlayer::s2cApplyAddTeam(BYTE* pProtocol)
{
	// ״̬��� (����Ҫ��� npc �Ķ��鿪��״̬ not end)
	if ( !m_cTeam.m_nFlag || m_cTeam.m_nFigure != TEAM_CAPTAIN)
	{
		ApplySelfTeamInfo();
		return;
	}
	
	// Ѱ�� npc
	PLAYER_APPLY_ADD_TEAM	*pAddTeam = (PLAYER_APPLY_ADD_TEAM*)pProtocol;
	int nNpcNo = NpcSet.SearchID(pAddTeam->m_dwTarNpcID);
	if (nNpcNo == 0)
		return;
	
	int i, nFreeListNo;
	// ����Ѿ����ڣ�����
	for (i = 0; i < MAX_TEAM_APPLY_LIST; ++i)
	{
		if (m_cTeam.m_sApplyList[i].m_dwNpcID == pAddTeam->m_dwTarNpcID)
		{
			m_cTeam.m_sApplyList[i].m_nLevel = Npc[nNpcNo].m_Level;
			m_cTeam.m_sApplyList[i].m_dwTimer = MAX_APPLY_TEAM_TIME;
			strcpy(m_cTeam.m_sApplyList[i].m_szName, Npc[nNpcNo].Name);
			m_cTeam.UpdateInterface();
			m_cTeam.UpdateamUI(); //��ʾ�����������Ϣ
			return;
		}
	}
	// Ѱ��������list��λ
	for (i = 0; i < MAX_TEAM_APPLY_LIST; ++i)
	{
		if (m_cTeam.m_sApplyList[i].m_dwNpcID == 0)
		{
			nFreeListNo = i;
			break;
		}
	}
	if (i >= MAX_TEAM_APPLY_LIST)	// û�ҵ������˿�λ
		return;
	
	m_cTeam.m_sApplyList[nFreeListNo].m_dwNpcID = pAddTeam->m_dwTarNpcID;
	m_cTeam.m_sApplyList[nFreeListNo].m_nLevel = Npc[nNpcNo].m_Level;
	m_cTeam.m_sApplyList[nFreeListNo].m_dwTimer = MAX_APPLY_TEAM_TIME;
	strcpy(m_cTeam.m_sApplyList[nFreeListNo].m_szName, Npc[nNpcNo].Name);
	
	m_cTeam.UpdateInterface();
	m_cTeam.UpdateamUI(); //��ʾ�����������Ϣ
//-------------------------------------------------//�Զ�Ӧ��
    //pPlayer = (KUiPlayerItem*)(&m_pHandlingMsg[1]);
			//g_pCoreShell->TeamOperation(TEAM_OI_APPLY_RESPONSE,(uintptr_t)pPlayer, (nSelAction == 0));	//nSelAction=0����Ӧ, nSelAction=1���ܾ�
   if (Player[CLIENT_PLAYER_INDEX].m_Autoplay.nCteam.nIsHuiDa==1 && Player[CLIENT_PLAYER_INDEX].m_Autoplay.nCteam.nIsDuiZhang==1)
   {
      Player[CLIENT_PLAYER_INDEX].AcceptTeamMember(pAddTeam->m_dwTarNpcID);
	  return;
   }
//-------------------------------------------------

	KSystemMessage	sMsg;
	t_sprintf(sMsg.szMessage, strCoreInfo[MSG_TEAM_APPLY_ADD].c_str(), Npc[nNpcNo].Name);
	sMsg.eType = SMT_TEAM;
	sMsg.byConfirmType = SMCT_UI_TEAM_APPLY;
	sMsg.byPriority = 3;   
	sMsg.byParamSize = sizeof(KUiPlayerItem);
    sMsg.nMsgLen=TEncodeText_(sMsg.szMessage, strlen(sMsg.szMessage));
	KUiPlayerItem	player;
	strcpy(player.Name, Npc[nNpcNo].Name);
	player.nIndex = 0;
	player.uId = pAddTeam->m_dwTarNpcID;
	
	CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (uintptr_t)&sMsg, (intptr_t)&player);
	
	return;
}

//���ÿ�ݼ��� --Ĭ��
void	KPlayer::SetDefaultImmedSkill()  
{
	//��ȡ������Ҽ�����ΪĬ��������
	int nDetailType = Player[CLIENT_PLAYER_INDEX].m_ItemList.GetWeaponType();
	int nParticularType = Player[CLIENT_PLAYER_INDEX].m_ItemList.GetWeaponParticular();
	
	//��������
	if (nDetailType == 0)
	{
		Player[CLIENT_PLAYER_INDEX].SetLeftSkill(g_nMeleeWeaponSkill[nParticularType]);
		Player[CLIENT_PLAYER_INDEX].SetRightSkill(g_nMeleeWeaponSkill[nParticularType]);
	}//Զ������
	else if (nDetailType == 1)
	{
		Player[CLIENT_PLAYER_INDEX].SetLeftSkill(g_nRangeWeaponSkill[nParticularType]);
		Player[CLIENT_PLAYER_INDEX].SetRightSkill(g_nRangeWeaponSkill[nParticularType]);
	}//����
	else if (nDetailType == -1)
	{
		Player[CLIENT_PLAYER_INDEX].SetLeftSkill(g_nHandSkill);
		Player[CLIENT_PLAYER_INDEX].SetRightSkill(g_nHandSkill);
	}

}
//���ÿ����ʰȡ����
void	KPlayer::SetShiquSkill(int nSkillID)
{

	if (Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_SkillList.GetCurrentLevel(nSkillID) <= 0) 
		return;

	KUiGameObject Info;
	Info.uGenre = CGOG_SKILL_SHORTCUT;
	Info.uId = nSkillID;
	CoreDataChanged(GDCNI_PLAYER_IMMED_ITEMSKILL, (uintptr_t)&Info, -1);

}

void KPlayer::SetLeftSkill(int nSkillID,BOOL nIsNoLogin)
{
	if (Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_SkillList.GetCurrentLevel(nSkillID) < 0) 
		return;

    Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].ReSkillEnhance(m_nLeftSkillID,0); //�ȼ�ȥ��һ��������ܼӳ�

	//д�ڹҼ��ܱ��
	if (nIsNoLogin)
	{
		 Player[CLIENT_PLAYER_INDEX].m_ItemList.ClientShowMsg("����:ִ�б����Զ��һ����ܳɹ�!");
		 KIniFile nAutoConfig;
		 char nConfig[128]={0};	   
		 t_sprintf(nConfig,"jx50ai/autoset/%u_set.ini",g_FileName2Id(Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].Name));
					   
		 if  (nAutoConfig.Load(nConfig))
		 { 
			 nAutoConfig.WriteInteger("AAAA","LeftSkill_0",nSkillID); 
		 }
		 else
		 {
			 Player[CLIENT_PLAYER_INDEX].m_ItemList.ClientShowMsg("����:��ɫ���ò�����!");

		 }
		 nAutoConfig.Save(nConfig);
		 nAutoConfig.Clear();
	}

	m_nLeftSkillID = nSkillID;	
	m_nLeftListidx = Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_SkillList.FindSame(m_nLeftSkillID);

	SKILL_LEFT_SYNC	sLeftSkill;
	sLeftSkill.ProtocolType = c2s_skillsync;
	sLeftSkill.m_nLeftskill = nSkillID;
    sLeftSkill.m_Type=0;

	Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].ReSkillEnhance(nSkillID,1);
	
	if (g_pClient)
		g_pClient->SendPackToServer(&sLeftSkill, sizeof(SKILL_LEFT_SYNC));
    
	//���洦��
	//KUiGameObject Info;
	//Info.uGenre = CGOG_SKILL_SHORTCUT;
	//Info.uId = m_nLeftSkillID;
	//CoreDataChanged(GDCNI_PLAYER_IMMED_ITEMSKILL, (uintptr_t)&Info, -1);
}


void	KPlayer::SetRightSkill(int nSkillID)
{
	if (Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_SkillList.GetCurrentLevel(nSkillID) < 0) 
		return;

	m_nRightSkillID = nSkillID;
	m_nRightListidx = Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_SkillList.FindSame(m_nRightSkillID);
	// ����Ƿ��ǹ⻷����
	ISkill * pOrdinSkill = g_SkillManager.GetSkill(nSkillID, 1);

	if (!pOrdinSkill) 
        return;
	
	if (pOrdinSkill->IsAura())
	{//����ǹ⻷���� ������ʹ��
		Npc[m_nIndex].SetAuraSkill(nSkillID);
	}
	else
	{//��������Ϊ���ǹ⻷����
		Npc[m_nIndex].SetAuraSkill(0);
	}
    //���洦��
	/*KUiGameObject Info;
	Info.uGenre = CGOG_SKILL_SHORTCUT;
	Info.uId = m_nRightSkillID;
	CoreDataChanged(GDCNI_PLAYER_IMMED_ITEMSKILL, (uintptr_t)&Info, -2);
	(*/
}
//��װ ���¿����������Ƽ�������
void KPlayer::UpdateWeaponSkill()
{
	if (m_nLeftSkillID > 0)
	{
		ISkill * pISkill = g_SkillManager.GetSkill(m_nLeftSkillID, 1);
		if (!pISkill) 
            return;
		
		if (m_nLeftSkillID==1 || m_nLeftSkillID==2 || m_nLeftSkillID==53)
		{//����������� id
			SetLeftSkill(Npc[m_nIndex].GetCurActiveWeaponSkill());
		}
		
	}
	if (m_nRightSkillID > 0)
	{//�����Ҽ����� id
		ISkill * pISkill = (KSkill *) g_SkillManager.GetSkill(m_nRightSkillID, 1);
		if (!pISkill) 
            return;
		
		//if (pISkill->IsPhysical()) //�⹦ϵ����
		if (m_nRightSkillID==1 || m_nRightSkillID==2 || m_nRightSkillID==53)
		{
			SetRightSkill(Npc[m_nIndex].GetCurActiveWeaponSkill());
		}
	}
}

//-------------------------------------------------------------------------
//	���ܣ��趨��ǰ����Ƶ��
//-------------------------------------------------------------------------
void	KPlayer::SetChatCurChannel(int nChannelNo)
{
//	m_cChat.SetCurChannel(nChannelNo);
}

//-------------------------------------------------------------------------
//	���ܣ�����������
//-------------------------------------------------------------------------
void	KPlayer::TeamInviteAdd(DWORD dwNpcID)
{
	m_cTeam.InviteAdd(dwNpcID);
}


#define MAX_ORDINSKILL_LEVEL_ALWAYS  20//��������������Լ��ܵȼ��ı䶯֮��ģ�һ������ܵȼ�
//������Ʒ �� ������Ʒ����һ������

BOOL KPlayer::CheckThisItem(int nSetings,char *nKey)
{
     
	int nRow = g_ForbitMap.GetHeight()+1,nReg=FALSE;
	
	for (int i=2;i<nRow;++i)
	{
		int nSkilid =0;
		g_ForbitMap.GetInteger(i,nKey,0,&nSkilid);
		if (nSkilid==nSetings)
		{
			nReg =TRUE;
			break;
		}
	}
	return nReg;   
}

BOOL	KPlayer::ExecuteScriptA(char * ScriptFileName, char * szFunName, int nParam,char * szCanshu,BOOL nIsGive)
{
	if (!ScriptFileName || !ScriptFileName[0] || !szFunName  || !szFunName[0]) return FALSE;
	char nCurFileName[128]={0};
	t_sprintf(nCurFileName,ScriptFileName);
	//g_StrLower(ScriptFileName);
	g_StrLower(nCurFileName);

	KLuaScript * pScript = (KLuaScript* )g_GetScript(g_CheckFileExist(nCurFileName));
	
	if (pScript==NULL)
	{
		printf("-------�ű�������,ִ��[%s][%s]ʧ��!-------- \n",ScriptFileName,szFunName);
		return FALSE;
	}

	pScript=NULL;


	return ExecuteScriptB(g_CheckFileExist(nCurFileName), szFunName, nParam,szCanshu,nIsGive);	
}
//�ͻ�����ִ�нű�����
#define MAX_TRYEXECUTESCRIPT_COUNT 5
BOOL	KPlayer::ExecuteScriptB(DWORD dwScriptId, char * szFunName, int nParam,char * szCanshu,BOOL nIsGive)
{
		int nTopIndex = 0;
//		m_btTryExecuteScriptTimes = 0;
		bool bExecuteScriptMistake = true;
		KLuaScript * pScript = (KLuaScript* )g_GetScript(dwScriptId);

		if (pScript==NULL)
		{
			printf("-------�ű�������,ִ��[%s]ʧ��!-------- \n",szFunName);
			return FALSE;
		}

     try
	 {
		if (pScript)
		{
			if (nIsGive)
			    Npc[m_nIndex].m_ActionScriptID = dwScriptId;

			Lua_PushNumber(pScript->m_LuaState, m_nPlayerIndex);  //��������
			pScript->SetGlobalName(SCRIPT_PLAYERINDEX);
			
			Lua_PushNumber(pScript->m_LuaState, m_dwID);
			pScript->SetGlobalName(SCRIPT_PLAYERID);
			
			Lua_PushNumber(pScript->m_LuaState, Npc[m_nIndex].m_SubWorldIndex);
			pScript->SetGlobalName(SCRIPT_SUBWORLDINDEX);

			nTopIndex=pScript->SafeCallBegin();

			if (!szCanshu ||!szCanshu[0])
			{
				if (pScript->CallFunction(szFunName,0,"")) 
				{
					bExecuteScriptMistake = false;
				}
			}
			else if (pScript->CallFunction(szFunName,0,"ds",nParam,szCanshu)) 
			{
				bExecuteScriptMistake = false;
			}
			nTopIndex=pScript->SafeCallBegin();
			pScript->SafeCallEnd(nTopIndex);
			//lua_pop(pScript->m_LuaState, -1); //��ջ������1��Ԫ�� -1���ջ
		}
		
		if (bExecuteScriptMistake)
		{
//			m_bWaitingPlayerFeedBack = false;
//			m_btTryExecuteScriptTimes = 0;
			Npc[m_nIndex].m_ActionScriptID = 0;
			return FALSE;
		}
		
		return TRUE;
	}
	catch(...)
	{
		if (pScript)
		{
			nTopIndex=pScript->SafeCallBegin();	
			pScript->SafeCallEnd(nTopIndex);
		}
		
		printf("ִ�нű���������B,Script[%s]!\n",szFunName);
	//	printf("Exception Have Caught When Execute Script[%d],[%s]!!!!!", dwScriptId,szFunName);
//		m_bWaitingPlayerFeedBack = false;
//		m_btTryExecuteScriptTimes = 0;
		Npc[m_nIndex].m_ActionScriptID = 0;
		return FALSE;
	}

	return TRUE;
}

BOOL KPlayer::ExecuteScriptD(DWORD dwScriptId, char * szFunName, char *  szParams,char * snParams,int inParams,int njb,int njxb,int inval,int invala,BOOL nIsGive)
{

//		m_btTryExecuteScriptTimes = 0;
		bool bExecuteScriptMistake = true;
		KLuaScript * pScript = (KLuaScript* )g_GetScript(dwScriptId);
		int nTopIndex = 0;

		if (pScript==NULL)
		{
			printf("-------�ű�������,Dִ��[%s]ʧ��!-------- \n",szFunName);
			return FALSE;
		}

	try
	{
		if (pScript)
		{
			if (nIsGive)
			   Npc[m_nIndex].m_ActionScriptID = dwScriptId;  //��ֵ�������ִ�нű�

			Lua_PushNumber(pScript->m_LuaState, m_nPlayerIndex);
			pScript->SetGlobalName(SCRIPT_PLAYERINDEX);
			
			Lua_PushNumber(pScript->m_LuaState, m_dwID);
			pScript->SetGlobalName(SCRIPT_PLAYERID);
			
			Lua_PushNumber(pScript->m_LuaState, Npc[m_nIndex].m_SubWorldIndex);
			pScript->SetGlobalName(SCRIPT_SUBWORLDINDEX);
			
			nTopIndex=pScript->SafeCallBegin();
			
			if (!szParams || !szParams[0]) 
			{
				if (pScript->CallFunction(szFunName,0,"%d",0))  //һ������
				{
					bExecuteScriptMistake = false;
				}
			}
			else if (pScript->CallFunction(szFunName,0,"ssddddd",szParams,snParams,inParams,njb,njxb,inval,invala))  //�в���
			{
					bExecuteScriptMistake = false;
			}
		  nTopIndex=pScript->SafeCallBegin();	
		  pScript->SafeCallEnd(nTopIndex); 
		  //lua_pop(pScript->m_LuaState, -1); //��ջ������1��Ԫ�� -1���ջ
		}
		
		if (bExecuteScriptMistake)
		{
//			m_bWaitingPlayerFeedBack = false;
//			m_btTryExecuteScriptTimes = 0;
			Npc[m_nIndex].m_ActionScriptID = 0;
			return FALSE;
		}
		 return TRUE;
	}
	catch(...)
	{

		if (pScript)
		{
			nTopIndex=pScript->SafeCallBegin();	
		    pScript->SafeCallEnd(nTopIndex);
			printf("ִ�нű���������C,Script[%s],[%s]!\n", pScript->m_szScriptName,szFunName);

			//pScript->Exit(); //�ͷŽű���Դ
		}
		//printf("Exception Have Caught When Execute Script[%d],[%s]", dwScriptId,szFunName);
//		m_bWaitingPlayerFeedBack = false;
//		m_btTryExecuteScriptTimes = 0;
		Npc[m_nIndex].m_ActionScriptID = 0;
		return FALSE;
	}

	return TRUE;
}

BOOL KPlayer::ExeNoBackScript(char * ScriptFileName, char * szFunName,int nKind,int nParama,int nParamb,int nParamc)
{
	if (!ScriptFileName || !ScriptFileName[0] || !szFunName  || !szFunName[0]) return FALSE;  
	char nCurFileName[128]={0};
	t_sprintf(nCurFileName,ScriptFileName);
	g_StrLower(nCurFileName);
	DWORD dwScriptId = g_CheckFileExist(nCurFileName);

	KLuaScript * pScript=NULL;
	pScript = (KLuaScript* )g_GetScript(dwScriptId);
	if (pScript==NULL)
	{
		printf("-------Script error,[%s][%s]!-------- \n",ScriptFileName,szFunName);
		return FALSE;
	}
    //��ʼִ�нű�
	bool bExecuteScriptMistake = true;
	int nSafeIndex = 0;
	int bResult=1;
	try
	{
		if (pScript)
		{
			Lua_PushNumber(pScript->m_LuaState, m_nPlayerIndex);
			pScript->SetGlobalName(SCRIPT_PLAYERINDEX);  //PlayerIndex
			Lua_PushNumber(pScript->m_LuaState, m_dwID);
			pScript->SetGlobalName(SCRIPT_PLAYERID);
			Lua_PushNumber(pScript->m_LuaState, Npc[m_nIndex].m_SubWorldIndex);
			pScript->SetGlobalName(SCRIPT_SUBWORLDINDEX);
			nSafeIndex=pScript->SafeCallBegin();

		    if (pScript->CallFunction(szFunName,1, "dddd", nKind,nParama,nParamb,nParamc))  //�в���
			{
				bExecuteScriptMistake = false;
			}

			if (bExecuteScriptMistake == false)
			{
				char *bResultchar=NULL; 
				if (Lua_IsNumber(pScript->m_LuaState, Lua_GetTopIndex(pScript->m_LuaState)) == 1)
				{//Lua_PushNumber
					bResult = (int)Lua_ValueToNumber(pScript->m_LuaState, Lua_GetTopIndex(pScript->m_LuaState));
				}
				else if (Lua_IsString(pScript->m_LuaState, Lua_GetTopIndex(pScript->m_LuaState)) == 1)  // �ַ�������
				{//Lua_PushSting
					bResultchar = (char *)Lua_ValueToString(pScript->m_LuaState, Lua_GetTopIndex(pScript->m_LuaState));
					bResult     = atoi(bResultchar);
				}
			}
			nSafeIndex=pScript->SafeCallBegin();
			pScript->SafeCallEnd(nSafeIndex); 
			return bResult;
		}

		if (bExecuteScriptMistake)
		{
			printf("script error,Script[%s][%s]!\n",ScriptFileName,szFunName);
		}
		return TRUE;
	}
	catch(...)
	{
		if (pScript)
		{
			nSafeIndex=pScript->SafeCallBegin();	
			pScript->SafeCallEnd(nSafeIndex);
		}
		printf("script error,Script[%s]!\n",ScriptFileName);
		return TRUE;
	}
}

BOOL KPlayer::ExecuteScriptC(char * ScriptFileName, char * szFunName, char *  szParams,char *  snParams, int inParams,int njb,int njxb,int inval,int invala,BOOL nIsGive)
{

	//if  (Npc[m_nIndex].m_nPeopleIdx >0)
	//	return FALSE;
	if (!ScriptFileName || !ScriptFileName[0] || !szFunName  || !szFunName[0]) return FALSE;  
	char nCurFileName[128]={0};
	t_sprintf(nCurFileName,ScriptFileName);
	g_StrLower(nCurFileName);
	//g_StrLower(ScriptFileName);
	DWORD dwScriptId = g_CheckFileExist(nCurFileName);

	KLuaScript * pScript=NULL;
			       pScript = (KLuaScript* )g_GetScript(dwScriptId);
	if (pScript==NULL)
	{
		printf("-------Script error,[%s][%s]!-------- \n",ScriptFileName,szFunName);
		return FALSE;
	}
	pScript=NULL;

	return ExecuteScriptD(dwScriptId, szFunName, szParams,snParams,inParams,njb,njxb,inval,invala,nIsGive);
}
////////////////////////��Ʒ�Ҽ�ִ�нű�/////////////////////////////////////////////////
BOOL	KPlayer::ExeItemScriptA(char * ScriptFileName, char * szFunName, char *  szParams,char *  snParams, int inParams,int njb,int njxb,int inGenre,int inDetail,int inParticular,int inStackNum,int inItemX,int inItemY)
{
	if (!ScriptFileName || !ScriptFileName[0] || !szFunName  || !szFunName[0]) return FALSE;
	char nCurFileName[128]={0};
	t_sprintf(nCurFileName,ScriptFileName);
	//g_StrLower(ScriptFileName);
	g_StrLower(nCurFileName);
	DWORD dwScriptId = g_CheckFileExist(nCurFileName);
	return ExeItemScript(dwScriptId, szFunName, szParams,snParams,inParams,njb,njxb,inGenre,inDetail,inParticular,inStackNum,inItemX,inItemY);
}

BOOL KPlayer::ExeItemScript(DWORD dwScriptId, char * szFunName, char *  szParams,char * snParams,int inParams,int njb,int njxb,int inGenre,int inDetail,int inParticular,int inStackNum,int inItemX,int inItemY)
{
//		m_btTryExecuteScriptTimes = 0;
		bool bExecuteScriptMistake = true;
		KLuaScript * pScript=NULL;
			       pScript = (KLuaScript* )g_GetScript(dwScriptId);
		int nSafeIndex = 0;
		int bResult=1; 

		if (pScript==NULL)
		{
			DWORD dwScriptIdx = g_CheckFileExist("\\script\\item\\noscript.lua");
			pScript = (KLuaScript* )g_GetScript(dwScriptIdx);
			if (pScript)
			{
			//printf("-------��Ʒ�ű�������,�滻:\\script\\item\\noscript.lua �ɹ�!-------- \n");
			}
			else
			{
			  printf("-------��Ʒ�ű�������!-------- \n");
			  return TRUE;
			} 
		}

  try
  {
		if (pScript)
		{
			//��ջ

			Npc[m_nIndex].m_ActionScriptID = dwScriptId;
			Lua_PushNumber(pScript->m_LuaState, m_nPlayerIndex);
			pScript->SetGlobalName(SCRIPT_PLAYERINDEX);
			
			Lua_PushNumber(pScript->m_LuaState, m_dwID);
			pScript->SetGlobalName(SCRIPT_PLAYERID);
			
			Lua_PushNumber(pScript->m_LuaState, Npc[m_nIndex].m_SubWorldIndex);
			pScript->SetGlobalName(SCRIPT_SUBWORLDINDEX);
			
			//myPrintLuaStackA(pScript->m_LuaState);

			nSafeIndex=pScript->SafeCallBegin();
			
			//printf("-------ʹ����Ʒǰ:ջ��ԭ��Ԫ����:%d ��--------\n",nSafeIndex);


			if ( (!szParams) || !szParams[0]) 
			{
				if (pScript->CallFunction(szFunName,1,""))  //�ղ���
				{
					bExecuteScriptMistake = false;
				}
			}
			else
			{
				if (( (!snParams) || !snParams[0]) )
                     snParams="";
				if (( (!szParams) || !szParams[0]) )
					 szParams="";

				if (pScript->CallFunction(szFunName,1, "ssdddddddddd", szParams,snParams,inParams,njb,njxb,inGenre,inDetail,inParticular,inStackNum,inItemX,inItemY,0))  //�в���
				{
					bExecuteScriptMistake = false;
				}
			}
		  ///////////////////�ű�����ֵ////////////////////////
          if (bExecuteScriptMistake == false)
		  {
			char *bResultchar=NULL; 
			//const char * szType = lua_typename(pScript->m_LuaState, Lua_GetTopIndex(pScript->m_LuaState));
			if (Lua_IsNumber(pScript->m_LuaState, Lua_GetTopIndex(pScript->m_LuaState)) == 1)
			{//Lua_PushNumber
				bResult = (int)Lua_ValueToNumber(pScript->m_LuaState, Lua_GetTopIndex(pScript->m_LuaState));
			}
			else if (Lua_IsString(pScript->m_LuaState, Lua_GetTopIndex(pScript->m_LuaState)) == 1)  // �ַ�������
			{//Lua_PushSting
				bResultchar = (char *)Lua_ValueToString(pScript->m_LuaState, Lua_GetTopIndex(pScript->m_LuaState));
			    bResult     = atoi(bResultchar);
			}
		  }
		  //myPrintLuaStackA(pScript->m_LuaState);
         /////////////////////////////////////////////////
		  nSafeIndex=pScript->SafeCallBegin();
		  //printf("-------ʹ����Ʒ��:ջ��ԭ��Ԫ����:%d ��--------\n",nSafeIndex);
		  pScript->SafeCallEnd(nSafeIndex); 
		  //lua_pop(pScript->m_LuaState, -1); //��ջ������1��Ԫ�� -1���ջ
		  return bResult;

		}

		if (bExecuteScriptMistake)
		{
//			m_bWaitingPlayerFeedBack = false;
//			m_btTryExecuteScriptTimes = 0;
			Npc[m_nIndex].m_ActionScriptID = 0;
			printf("ִ���Ҽ���Ʒ�ű�ʧ��,Script[%s]!\n",szFunName);
		}
		 return TRUE;
}
catch(...)
{
	if (pScript)
	{
		nSafeIndex=pScript->SafeCallBegin();	
		pScript->SafeCallEnd(nSafeIndex);
	}
	printf("ִ���Ҽ���Ʒ�ű�ʧ��,Script[%s]!\n",szFunName);
	//	printf("Exception Have Caught When Execute Script[%d],[%s]!!!!!", dwScriptId,szFunName);
//	m_bWaitingPlayerFeedBack = false;
//	m_btTryExecuteScriptTimes = 0;
	Npc[m_nIndex].m_ActionScriptID = 0;
	return TRUE;
}

}
//�ͻ���ִ�нű�
int  KPlayer::ClientExeItemScript(char *nScriptPath,char *nFunName,int nPaparm,int ninVal,char *nstrVala)
{  

	    if (!nScriptPath)
			return 0;
//	    m_btTryExecuteScriptTimes = 0;
		char nScriptNewPath[128]={0};
		t_sprintf(nScriptNewPath,"%s",nScriptPath);
		//_strupr(nScriptNewPath); //ת�ɴ�д _strupr
		//m_ItemList.ClientShowMsg(nScriptNewPath);
		bool bExecuteScriptMistake = true;
		KLuaScript * pScript=NULL;
		//char nStrInfo[128]={0};
		//t_sprintf(nStrInfo,nScriptPath);
		g_StrLower(nScriptNewPath);   //��дתСд  g_StrLower
        
		 //m_ItemList.ClientShowMsg(nScriptNewPath);
		 //return 0;

		DWORD dwScriptId   = g_CheckFileExist(nScriptNewPath);
			       pScript = (KLuaScript* )g_GetScript(dwScriptId);
		int nSafeIndex = 0;
		int bResult=1; 
 
		if (pScript==NULL)
		{
			DWORD dwScriptIdx = g_CheckFileExist("script\\item\\noscript.lua");
			pScript = (KLuaScript* )g_GetScript(dwScriptIdx);
			if (pScript)
			{
			   //printf("-------��Ʒ�ű�������,�滻:\\script\\item\\noscript.lua �ɹ�!-------- \n");
			}
			else
			{
				char nMsg[128]={0};
				t_sprintf(nMsg,"---�ű�������:%s--",nScriptNewPath);
			    m_ItemList.ClientShowMsg(nMsg);
			    return FALSE;
			} 
		}

  try
  {
		if (pScript)
		{
			//��ջ

			Npc[m_nIndex].m_ActionScriptID = dwScriptId;
			Lua_PushNumber(pScript->m_LuaState, m_nPlayerIndex);
			pScript->SetGlobalName(SCRIPT_PLAYERINDEX);
			
			Lua_PushNumber(pScript->m_LuaState, m_dwID);
			pScript->SetGlobalName(SCRIPT_PLAYERID);
			
			Lua_PushNumber(pScript->m_LuaState, Npc[m_nIndex].m_SubWorldIndex);
			pScript->SetGlobalName(SCRIPT_SUBWORLDINDEX);
			
			//myPrintLuaStackA(pScript->m_LuaState);

			nSafeIndex=pScript->SafeCallBegin();
			
			//printf("-------ʹ����Ʒǰ:ջ��ԭ��Ԫ����:%d ��--------\n",nSafeIndex);
		 if (pScript->CallFunction(nFunName,1, "dds",nPaparm,ninVal,nstrVala))  //�з���ֵ
  //�в���
		 {
			 bExecuteScriptMistake = false;
		 }
		  ///////////////////�ű�����ֵ////////////////////////
          if (bExecuteScriptMistake == false)
		  {
			char *bResultchar=NULL; 
			//const char * szType = lua_typename(pScript->m_LuaState, Lua_GetTopIndex(pScript->m_LuaState));
			if (Lua_IsNumber(pScript->m_LuaState, Lua_GetTopIndex(pScript->m_LuaState)) == 1)
			{//Lua_PushNumber
				bResult = (int)Lua_ValueToNumber(pScript->m_LuaState, Lua_GetTopIndex(pScript->m_LuaState));
			}
			else if (Lua_IsString(pScript->m_LuaState, Lua_GetTopIndex(pScript->m_LuaState)) == 1)  // �ַ�������
			{//Lua_PushSting
				bResultchar = (char *)Lua_ValueToString(pScript->m_LuaState, Lua_GetTopIndex(pScript->m_LuaState));
			    bResult     = atoi(bResultchar);
			}
		  }
		  nSafeIndex=pScript->SafeCallBegin();
		  pScript->SafeCallEnd(nSafeIndex); 
		  return bResult;

		}

		if (bExecuteScriptMistake)
		{
//			m_bWaitingPlayerFeedBack = false;
//			m_btTryExecuteScriptTimes = 0;
			Npc[m_nIndex].m_ActionScriptID = 0;
			m_ItemList.ClientShowMsg("-------�ű�����!-------- ");
		}
		 return TRUE;
}
catch(...)
{
	if (pScript)
	{
		nSafeIndex=pScript->SafeCallBegin();	
		pScript->SafeCallEnd(nSafeIndex);
	}
	m_ItemList.ClientShowMsg("-------�ű�����!-------- ");
//	m_bWaitingPlayerFeedBack = false;
//	m_btTryExecuteScriptTimes = 0;
	Npc[m_nIndex].m_ActionScriptID = 0;
	return TRUE;
}

	   /* ZeroMemory(szClienResult,sizeof(szClienResult));
	    KLuaScript nClentLua;
	    nClentLua.Init();                                                    //��ʼ�� ע���׼������
		nClentLua.RegisterFunctions(GameScriptFuns,g_GetGameScriptFunNum()); //��C�ű�����ע�ᵽ�ű���

		if (nClentLua.Load(nScriptPath))
		{
			//m_ItemList.ClientShowMsg("�ű�ִ�гɹ�C");
			t_sprintf(Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_ScriptPicPath,nScriptPath);
			//Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_ActionScriptID =g_FileName2Id(nScriptPath);
			int nSafeIndex = 0;
			nSafeIndex=nClentLua.SafeCallBegin();
			//ִ������ű��ĺ���				
			nClentLua.CallFunction(nFunName,1, "dds",nPaparm,ninVal,nstrVala);  //�з���ֵ
			const char * szType = lua_typename(nClentLua.m_LuaState, Lua_GetTopIndex(nClentLua.m_LuaState));
			if (Lua_IsNumber(nClentLua.m_LuaState, Lua_GetTopIndex(nClentLua.m_LuaState)) == 1)
			{//Lua_PushNumber
				int nResult = (int)Lua_ValueToNumber(nClentLua.m_LuaState, Lua_GetTopIndex(nClentLua.m_LuaState));
				t_sprintf(szClienResult, "%d", nResult);  //int����
			}
			else if (Lua_IsString(nClentLua.m_LuaState, Lua_GetTopIndex(nClentLua.m_LuaState)) ==1)//�ַ�������
			{
				t_sprintf(szClienResult, (char *)Lua_ValueToString(nClentLua.m_LuaState, Lua_GetTopIndex(nClentLua.m_LuaState)));
			}
			else
				t_sprintf(szClienResult,"<color=Red>�ű����ݴ���GC,����ϵGM����!<color>");  
			    //m_ItemList.ClientShowMsg("ִ�нű��ɹ�GC!");
			    nSafeIndex=nClentLua.SafeCallBegin();
			    nClentLua.SafeCallEnd(nSafeIndex);
		}
		else
		{
		  ZeroMemory(Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_ScriptPicPath,sizeof(Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_ScriptPicPath));
		  //Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_ActionScriptID=0;
		  m_ItemList.ClientShowMsg("ִ�нű�ʧ��GC,����ϵGM����!");
		}
		//m_ItemList.ClientShowMsg("�ű�ִ�гɹ�");
		nClentLua.Exit();
		return atoi(szClienResult);*/ 
}

//��ʾջ��Ϣ�ĺ���
void KPlayer::myPrintLuaStackA(lua_State *L)
{
    int stackTop=lua_gettop(L);//��ȡջ��������ֵ
    int index,t;
    printf("--ջ��(v)(%d)--\n",stackTop);
    //��ʾջ�е�Ԫ��
    for(index=stackTop;index>0;--index)
	{  t=lua_type(L,index);
	    printf("(i:%d) %s(%s)\n",index,lua_typename(L,t),lua_tostring(L,index));	     
	}
    printf("--ջ��--\n");
}
/////////////////////////////////////////////////////////////////
//GMִ�нű�
BOOL	KPlayer::DoScript(char * ScriptCommand)
{

	return TRUE;
/*	if (NULL == ScriptCommand) return FALSE;
	KLuaScript *Script = new KLuaScript;
	Script->Init();
	Script->RegisterFunctions(GameScriptFuns, g_GetGameScriptFunNum());
	
	//GM Standand Script Functions 
//	Script->Load("\\script\\system\\startserver--.lua.lua");  //GMִ�п�ʼ��Ϸ�ű�
	
	
	Lua_PushNumber(Script->m_LuaState, m_nPlayerIndex);
	Script->SetGlobalName(SCRIPT_PLAYERINDEX);
	Lua_PushNumber(Script->m_LuaState, m_dwID);
	Script->SetGlobalName(SCRIPT_PLAYERID);	
	Lua_PushNumber(Script->m_LuaState, Npc[m_nIndex].m_SubWorldIndex);
	Script->SetGlobalName(SCRIPT_SUBWORLDINDEX);
	
	if (Script->LoadBuffer((PBYTE)ScriptCommand, strlen(ScriptCommand))) 
	{
		BOOL bResult = Script->ExecuteCode();	
		delete Script; //Question!
         Script=NULL;
		return bResult;
	}
	delete Script; //Question!
	Script=NULL;
	return FALSE;*/
}


void	KPlayer::DoScriptAction(PLAYER_SCRIPTACTION_SYNC * pUIInfo) //Ҫ����ʾĳ��UI����
{
	if (!pUIInfo) return;
	
	//�������˽ű�ʱ
	if (pUIInfo->m_bParam2 < 1)
	{//�ͻ��˽ű�Ҫ����ʾ�ű� ֱ������
		OnScriptAction((PLAYER_SCRIPTACTION_SYNC *)pUIInfo);
	}
	
}

//�������˻�֪��ҽ���UIѡ����ĳ��󣬴���ִ�нű�����~~
void KPlayer::ProcessPlayerSelectFromUI(BYTE* pProtocol)			// ������Ҵ�ѡ��˵�ѡ��ĳ��ʱ�Ĳ���	
{
	PLAYER_SELECTUI_COMMAND * pSelUI = (PLAYER_SELECTUI_COMMAND*) pProtocol;
//	m_bWaitingPlayerFeedBack = false;
	//������ظ�������ʾ�˳��ýű�ִ�л���
	if (pSelUI->nSelectIndex < 0) 
		m_nAvailableAnswerNum = 0;

	if (m_nAvailableAnswerNum > pSelUI->nSelectIndex)
	{
		if (m_szTaskAnswerFun[pSelUI->nSelectIndex][0])
		{
			//g_SetFilePath("\\script");
			if (m_nIndex)
			{
			      //m_ItemList.ClientShowMsg(Npc[m_nIndex].m_ScriptPicPath);
				  //m_ItemList.ClientShowMsg(m_szTaskAnswerFun[pSelUI->nSelectIndex]);
				  char nstrPath[256]={0};
				  Npc[m_nIndex].GetstrInfo(STR_SCRIPTTATH_CLIENT,nstrPath);
				  ClientExeItemScript(nstrPath,m_szTaskAnswerFun[pSelUI->nSelectIndex],pSelUI->nSelectIndex,0,m_szTaskAnswerCanshu[pSelUI->nSelectIndex]) ;
			}
			/*else
			{//�ͻ���ִ��
			     Player[CLIENT_PLAYER_INDEX].ClientExeItemScript("\\Ui\\Fs_NewTaskWindow.lua","newtaskrequestinfo",0,0,"") ;
			} */
		}
	}
}
//�ͻ���

//����ڽ��潻����ѡ����ĳ�����������˷���
void	KPlayer::OnSelectFromUI(PLAYER_SELECTUI_COMMAND * pSelectUI, UIInfo eUIInfo)	//����Ҵ�ѡ�����ѡ��ĳ��󣬽������������			
{
	if (!pSelectUI) return;
	
	switch(eUIInfo)
	{
	case UI_SELECTDIALOG:
		{
			if (g_bUISelIntelActiveWithServer)
			{//�Ƿ��Ƿ�����ִ��
				pSelectUI->ProtocolType = (BYTE)c2s_playerselui;
			    if (g_pClient)
					g_pClient->SendPackToServer((BYTE*)pSelectUI, sizeof(PLAYER_SELECTUI_COMMAND));
			}
			else
			{//�ͻ���ִ��
				ProcessPlayerSelectFromUI((BYTE *)pSelectUI);// ������Ҵ�ѡ��˵�ѡ��ĳ��ʱ�Ĳ���	
			}
		}
		break;
	case UI_TALKDIALOG:
		{
			if (g_bUISpeakActiveWithServer)
			{//�Ƿ��Ƿ�����ִ��
				pSelectUI->ProtocolType = (BYTE)c2s_playerselui;
				if (g_pClient)
     				g_pClient->SendPackToServer((BYTE*)pSelectUI, sizeof(PLAYER_SELECTUI_COMMAND));
			}
			else
			{//�ͻ���ִ��
				ProcessPlayerSelectFromUI((BYTE *)pSelectUI);			// ������Ҵ�ѡ��˵�ѡ��ĳ��ʱ�Ĳ���	
			}
			
		}break;
	}
}


//-------------------------------------------------------------------------
//	���ܣ����֪ͨĳ����������
//-------------------------------------------------------------------------
void	KPlayer::ChatFriendOnLine(DWORD dwID, int nFriendIdx)
{
	/*if (nFriendIdx <= 0)
		return;
	for (int i = 0; i < MAX_FRIEND_TEAM; ++i)
	{
		if (m_cChat.m_cFriendTeam[i].m_nFriendNo == 0)
			continue;
		CChatFriend	*pFriend;
		pFriend = (CChatFriend*)m_cChat.m_cFriendTeam[i].m_cEveryOne.GetHead();
		while (pFriend)
		{
			if (pFriend->m_dwID == dwID)
			{
				pFriend->m_nPlayerIdx = nFriendIdx;
				// ֪ͨ�����к�������
				KUiPlayerItem	sPlayer;
				strcpy(sPlayer.Name, pFriend->m_szName);
				sPlayer.uId = dwID;
				sPlayer.nIndex = nFriendIdx;
				sPlayer.nData = CHAT_S_ONLINE;

				CoreDataChanged(GDCNI_CHAT_FRIEND_STATUS, (uintptr_t)&sPlayer, i);
				
				KSystemMessage	sMsg;
				t_sprintf(sMsg.szMessage, MSG_CHAT_FRIEND_ONLINE, pFriend->m_szName);
				sMsg.eType = SMT_NORMAL;
				sMsg.byConfirmType = SMCT_NONE;
				sMsg.byPriority = 0;
				sMsg.byParamSize = 0;
				sMsg.nMsgLen = TEncodeText(sMsg.szMessage, strlen(sMsg.szMessage));
				CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (uintptr_t)&sMsg, 0);
				return;
			}
			pFriend = (CChatFriend*)pFriend->GetNext();
		}
	}*/
}

// ��õ�ǰ��������
void	KPlayer::GetFactionName(char *lpszName, int nSize)
{
	this->m_cFaction.GetCurFactionName(lpszName);
	return;
}


void	KPlayer::OnScriptAction(PLAYER_SCRIPTACTION_SYNC * pMsg)
{
	if (!g_GameWorld) return;

	PLAYER_SCRIPTACTION_SYNC * pScriptAction = (PLAYER_SCRIPTACTION_SYNC *)pMsg;

	char szString[1024]={0};
	ZeroMemory(szString,sizeof(szString));
	//ccMessageBox("�򿪶Ի���","�򿪶Ի���...");
	switch(pScriptAction->m_nOperateType)
	{
	case SCRIPTACTION_UISHOW:
		{
			switch(pScriptAction->m_bUIId)
			{
			case UI_SELECTDIALOG://֪ͨ�ͻ�����ʾѡ�񴰿�
				{
					KUiQuestionAndAnswer	*pQuest = NULL;
					KUiNpcSpr               *pImage = NULL;
					if (pScriptAction->m_nBufferLen <= 0 || pScriptAction->m_nBufferLen>=1024) break;
					
					if (pScriptAction->m_bOptionNum <= 0)
						pQuest = (KUiQuestionAndAnswer *)malloc(sizeof(KUiQuestionAndAnswer));
					else
						pQuest = (KUiQuestionAndAnswer *)malloc(sizeof(KUiQuestionAndAnswer) + sizeof(KUiAnswer) * (pScriptAction->m_bOptionNum - 1));
					
					if (pQuest==NULL)
						break;
					pImage = (KUiNpcSpr *)malloc(sizeof(KUiNpcSpr));
					if (pImage==NULL)
						break;
					memset(pQuest,0,sizeof(KUiQuestionAndAnswer));
					memset(pImage,0,sizeof(KUiNpcSpr));

					char *strContent=NULL;
					      strContent=(char *)malloc(1024);
				    if (strContent==NULL)
					    break;	 
					ZeroMemory(strContent,sizeof(strContent));
					char * pAnswer = NULL;
					pQuest->AnswerCount = 0;
					//����ϢΪ�ַ���
					int nCurAnswerLen = 0;//strlen(pScriptAction->m_pContent)-pScriptAction->m_nBufferLen -1;
					if (pScriptAction->m_bParam1 == 0)
					{
						//g_StrCpyLen(strContent, pScriptAction->m_pContent, pScriptAction->m_nBufferLen + 1); //��������
						memcpy(&strContent[0],pScriptAction->m_pContent, pScriptAction->m_nBufferLen);
						strContent[pScriptAction->m_nBufferLen]='\0';
						pAnswer = strstr(strContent,"|");   //��ȡ������ ȡ�����ֵ

						if (!pAnswer)
						{
							pScriptAction->m_bOptionNum = 0;
							pQuest->AnswerCount = 0;
						}
						else
							*pAnswer++ = 0; //���� �ַ��� ��|�����������  ---��ȡ��ѡ������

						//g_StrCpyLen(pQuest->Question, strContent, sizeof(pQuest->Question));  //��������
						memcpy(&pQuest->Question[0],strContent,sizeof(pQuest->Question));
						pQuest->QuestionLen = TEncodeText_(pQuest->Question, strlen(pQuest->Question));
						pQuest->Question[pQuest->QuestionLen]='\0';
					}
					//����ϢΪ���ֱ�ʶ
					else 
					{
						g_StrCpyLen(pQuest->Question, g_GetStringRes(*(int *)pScriptAction->m_pContent, szString, sizeof(szString)), sizeof(pQuest->Question));
						pQuest->QuestionLen = TEncodeText_(pQuest->Question, strlen(pQuest->Question));
						g_StrCpyLen(strContent, pScriptAction->m_pContent + sizeof(int), pScriptAction->m_nBufferLen - sizeof(int) + 1);
						pAnswer = strContent + 1;
					}
					// ����Ϊѡ�������
					for (int i = 0; i < pScriptAction->m_bOptionNum; ++i)   //ѡ��ĸ���
					{
						char * pNewAnswer = strstr(pAnswer,"|");
						//pAnswer ǰ������� pNewAnswer ���������(���� | )
						if (pNewAnswer)                                     //������
						{
							*pNewAnswer = 0;                                //ȥ�� | ����
							strcpy(pQuest->Answer[i].AnswerText,pAnswer);  // ������
							pQuest->Answer[i].AnswerLen = -1;
                            pAnswer = pNewAnswer+1; //ȥ��һ�� |
						}
						else
						{//���һ��ѡ��
#ifdef WIN32
							strcpy(pQuest->Answer[i].AnswerText,pAnswer);
							pQuest->Answer[i].AnswerLen = -1;
                            pQuest->AnswerCount = i + 1;	
#else
							strcpy(pQuest->Answer[i].AnswerText,pAnswer);
							pQuest->Answer[i].AnswerLen = -1;
							pQuest->AnswerCount = i + 1;
#endif
		     				break;
						}
					}//end for

					if (pScriptAction->m_bParam2>=1)
					   g_bUISelIntelActiveWithServer =TRUE;

					//g_bUISelIntelActiveWithServer = pScriptAction->m_bParam2;
					g_bUISelLastSelCount = pQuest->AnswerCount;
                    /*******************************************Code by thienthanden2*******************************************************/ 
                    char szBuffer[256]={0}; 
					if  (pScriptAction->m_szSprPath[0] && strstr(pScriptAction->m_szSprPath,".spr"))
					{
						strcpy(pImage->ImageFile, pScriptAction->m_szSprPath);
						pImage->MaxFrame = 1;
						goto Next;
					}
					else if (!pScriptAction->m_szSprPath[0] && m_nImageNpcID)  //spr ���� NPC����
                    { 

                      /*for (int i = 0; i < 16; ++i) 
					  { 
						Npc[m_nImageNpcID].GetNpcRes()->m_pcResNode->GetFileName(i, 3, 0, "", szBuffer, sizeof(szBuffer)); 
                        if (szBuffer[0]) 
                            { 
                            strcpy(pImage->ImageFile, szBuffer); 
                            pImage->MaxFrame = (Npc[m_nImageNpcID].GetNpcRes()->m_pcResNode->GetTotalFrames(i, 3, 0, 16))/ 
                            (Npc[m_nImageNpcID].GetNpcRes()->m_pcResNode->GetTotalDirs(i, 3, 0, 16)); goto Next; 
                            } 
					  } 
                      for (int j = 0; j < 16; ++j) 
					  {  
                         Npc[m_nImageNpcID].GetNpcRes()->m_pcResNode->GetFileName(j, 0, 0, "", szBuffer, sizeof(szBuffer)); 
                        if (szBuffer[0]) 
                            { 
                              strcpy(pImage->ImageFile, szBuffer); 
                              pImage->MaxFrame = (Npc[m_nImageNpcID].GetNpcRes()->m_pcResNode->GetTotalFrames(j, 0, 0, 16))/ 
                              (Npc[m_nImageNpcID].GetNpcRes()->m_pcResNode->GetTotalDirs(j, 0, 0, 16)); goto Next; 
                            } 
					  } */
                     
                    } 
                    Next: 
                    if (pScriptAction->m_Select == 1 /*&& m_nImageNpcID*/) 
                          CoreDataChanged(GDCNI_QUESTION_CHOOSE, (uintptr_t)pQuest,(intptr_t)pImage,pScriptAction->m_bParam2);
                    else
						  CoreDataChanged(GDCNI_QUESTION_CHOOSE, (uintptr_t)pQuest, 0); 

                    free(pImage); 
                    pImage = NULL; 
					
                    free(pQuest); 
                    pQuest = NULL;
					
					free(strContent); 
                    strContent = NULL;
                } 
                break;  
				
			case UI_TALKDIALOG: 
				{//talk
					if  (pScriptAction->m_nBufferLen>=512 || pScriptAction->m_nBufferLen<=0)
						return;

					BOOL bUsingSpeakId = pScriptAction->m_bParam1;
					int	 nSentenceCount = pScriptAction->m_bOptionNum;
					if (nSentenceCount <= 0) return ;
					KUiInformationParam *pSpeakList  = new KUiInformationParam[nSentenceCount];
					memset(pSpeakList, 0, sizeof(KUiInformationParam) * nSentenceCount);
					
					char * pAnswer = new char [512];//pScriptAction->m_nBufferLen + 1
					//char *pAnswer=NULL
					 //   pAnswer=(char *)malloc(pScriptAction->m_nBufferLen + 1);
					if (pAnswer==NULL)
						break;

					ZeroMemory(pAnswer,sizeof(pAnswer));
					g_StrCpyLen(pAnswer, pScriptAction->m_pContent, pScriptAction->m_nBufferLen + 1);
					char * pBackupAnswer = pAnswer;

					int nCount = 0;
					for (int i = 0; i < pScriptAction->m_bOptionNum; ++i)
					{
						char * pNewAnswer = strstr(pAnswer, "|");
						
						if (pNewAnswer)
						{
							*pNewAnswer = 0;
							if (!bUsingSpeakId)
								strcpy(pSpeakList[i].sInformation, pAnswer);
							else
								strcpy(pSpeakList[i].sInformation, g_GetStringRes(atoi(pAnswer), szString, sizeof(szString)));

							if (i < pScriptAction->m_bOptionNum - 1)
								strcpy(pSpeakList[i].sConfirmText, "Kplayer so 1");
							else 
							{
								strcpy(pSpeakList[i].sConfirmText, "Kplayer so 2");
								if (pScriptAction->m_nParam == 1)						
									pSpeakList[i].bNeedConfirmNotify = TRUE;
							}
//#ifndef WIN32
//							t_sprintf (pSpeakList[i].sInformation,UTEXT(pSpeakList[i].sInformation,1).c_str());
//#endif
							pSpeakList[i].nInforLen = TEncodeText_(pSpeakList[i].sInformation, strlen(pSpeakList[i].sInformation));
							pAnswer = pNewAnswer + 1;
						}
						else
						{
							if (!bUsingSpeakId)
								strcpy(pSpeakList[i].sInformation, pAnswer);
							else
								strcpy(pSpeakList[i].sInformation,g_GetStringRes(atoi(pAnswer), szString ,sizeof(szString)));
							
							strcpy(pSpeakList[i].sConfirmText, "Kplayer so 3");
							
							if (pScriptAction->m_nParam == 1)						
								pSpeakList[i].bNeedConfirmNotify = TRUE;
//#ifndef WIN32
//							t_sprintf (pSpeakList[i].sInformation,UTEXT(pSpeakList[i].sInformation,1).c_str());
//#endif
							pSpeakList[i].nInforLen = TEncodeText_(pSpeakList[i].sInformation, strlen(pSpeakList[i].sInformation));
							nCount++;
							break;
						}
						nCount ++;
					} //END FOR

					if (pScriptAction->m_bParam2>=1)
					   g_bUISpeakActiveWithServer =TRUE;

					//g_bUISpeakActiveWithServer = pScriptAction->m_bParam2;
					//�ͻ��˵�������
					CoreDataChanged(GDCNI_SPEAK_WORDS, (uintptr_t)pSpeakList, nCount);
					
					if (pBackupAnswer)
					{
						delete [] pBackupAnswer;
						pBackupAnswer = NULL;
					}
					
					if (pSpeakList)
					{
						delete pSpeakList;	
						pSpeakList = NULL;
					}
				}
				break;
			case UI_MSGINFO:
				{
					if (pScriptAction->m_nBufferLen <= 0) 
						break;

					char strContent[1024];
					//����ϢΪ�ַ���
					if (pScriptAction->m_bParam1 == 0)
					{
						g_StrCpyLen(strContent, pScriptAction->m_pContent,  pScriptAction->m_nBufferLen + 1);
					}
					else
					{
						g_GetStringRes(*(int *)pScriptAction->m_pContent, strContent ,sizeof(strContent));
					}

					KSystemMessage	sMsg;
					sMsg.eType = SMT_PLAYER;
					sMsg.byConfirmType = SMCT_MSG_BOX;
					sMsg.byPriority = 0;
					sMsg.byParamSize = 0;
					g_StrCpyLen(sMsg.szMessage, strContent, sizeof(sMsg.szMessage));
					sMsg.nMsgLen = TEncodeText_(sMsg.szMessage, strlen(sMsg.szMessage)); //�����ӵ�
					CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (uintptr_t)&sMsg, 0);
					//CoreDataChanged(GDCNI_MISSION_RECORD, (uintptr_t)strContent, strlen(strContent));

				}break;
			case UI_NEWSINFO:  //����������Ϣ
				{
					if (pScriptAction->m_nBufferLen <= 0) 
						break;
					switch(pScriptAction->m_bOptionNum) //OPtionNum��������һ��News����
					{
					case NEWSMESSAGE_NORMAL:  //��ʾ������
						{
							KNewsMessage News;
							News.nType = NEWSMESSAGE_NORMAL;
							
							char strContent[1024];
							//����ϢΪ�ַ���
							if (pScriptAction->m_bParam1 == 0)
							{
								g_StrCpyLen(strContent, pScriptAction->m_pContent,  pScriptAction->m_nBufferLen + 1);
							}
							else
							{
								g_GetStringRes(*(int *)pScriptAction->m_pContent, strContent ,sizeof(strContent));
							}
							
							g_StrCpyLen(News.sMsg,  strContent, sizeof(News.sMsg));

							News.nMsgLen = TEncodeText_(News.sMsg, strlen(News.sMsg));
							CoreDataChanged(GDCNI_NEWS_MESSAGE, (uintptr_t)&News, 0);
						}
						break;
					case NEWSMESSAGE_SHUIJI:  //ϵͳ��� ��ʾ������
						{							
							KNewsMessage News;
							News.nType = NEWSMESSAGE_SHUIJI;
							
							char strContent[512];
							//����ϢΪ�ַ���
							if (pScriptAction->m_bParam1 == 0)
							{//������������ ��Ϣid
								g_StrCpyLen(strContent, pScriptAction->m_pContent,  pScriptAction->m_nBufferLen + 1);
							}
							else
							{//�ַ���
								g_GetStringRes(*(int *)pScriptAction->m_pContent, strContent ,sizeof(strContent));
							}
							
							g_StrCpyLen(News.sMsg,strContent, sizeof(News.sMsg));
							News.nMsgLen = TEncodeText_(News.sMsg, strlen(News.sMsg));
							CoreDataChanged(GDCNI_SHUIJI_MESSAGE, (uintptr_t)&News, 0);
						}
						break;
						//STRING|STRINGID + TIME(INT)
					case NEWSMESSAGE_COUNTING:  //��ʱ��Ϣ��ʱ�䵽������
						{
							KNewsMessage News;
							News.nType = pScriptAction->m_bOptionNum;
							
							char strContent[1024];
							int nTime = 0;
							//����ϢΪ�ַ���
							if (pScriptAction->m_bParam1 == 0)
							{
								g_StrCpyLen(strContent, pScriptAction->m_pContent,  pScriptAction->m_nBufferLen - sizeof(int) + 1);
							}
							else
							{
								g_GetStringRes(*(int *)pScriptAction->m_pContent, strContent ,sizeof(strContent));
							}
							
							g_StrCpyLen(News.sMsg,  strContent, sizeof(News.sMsg));
							News.nMsgLen = TEncodeText_(News.sMsg, strlen(News.sMsg));
							
							/*SYSTEMTIME systime;
							memset(&systime, 0, sizeof(SYSTEMTIME));
							systime.wSecond = *(int *)((pScriptAction->m_pContent + pScriptAction->m_nBufferLen - sizeof(int)));
							CoreDataChanged(GDCNI_NEWS_MESSAGE, (uintptr_t)&News, (uintptr_t)&systime);
							*/
						}break;

					case NEWSMESSAGE_TIMEEND:  //��ʱ��Ϣ
						{
							KNewsMessage News;
							News.nType = pScriptAction->m_bOptionNum;
							
							char strContent[1024];
							int nTime = 0;
							//����ϢΪ�ַ���
							if (pScriptAction->m_bParam1 == 0)
							{
								//g_StrCpyLen(strContent, pScriptAction->m_pContent,  pScriptAction->m_nBufferLen - sizeof(SYSTEMTIME) + 1);
							}
							else
							{
								g_GetStringRes(*(int *)pScriptAction->m_pContent, strContent ,sizeof(strContent));
							}
							
							g_StrCpyLen(News.sMsg,  strContent, sizeof(News.sMsg));
							News.nMsgLen = TEncodeText_(News.sMsg, strlen(News.sMsg));

							/*SYSTEMTIME systime;
							systime = *(SYSTEMTIME*)((pScriptAction->m_pContent + pScriptAction->m_nBufferLen - sizeof(SYSTEMTIME)));
							CoreDataChanged(GDCNI_NEWS_MESSAGE, (uintptr_t)&News, (uintptr_t)&systime);
							*/
						}break;
					
					}
					
				}break;
			case UI_PLAYMUSIC:
				{
					char szMusicFile[MAX_PATH];
					memcpy(szMusicFile, pScriptAction->m_pContent, pScriptAction->m_nBufferLen);
					szMusicFile[pScriptAction->m_nBufferLen] = 0;
//					g_SubWorldSet.m_cMusic.ScriptPlay(szMusicFile);
				}break;
			
			case UI_OPENTONGUI:		  //�򿪽���UI		
				{
					Player[CLIENT_PLAYER_INDEX].m_cTong.OpenCreateInterface();
					break;
				}
			}
	} break;
	case SCRIPTACTION_EXESCRIPT://Ҫ��ͻ��˵���ĳ���ű�
		{
			if (pScriptAction->m_nBufferLen <= 0 ) break;
			char szScriptInfo[1000];
			g_StrCpyLen(szScriptInfo, pScriptAction->m_pContent,pScriptAction->m_nBufferLen + 1);
			char * pDivPos = strstr(szScriptInfo, "/");  // б�ܺ�������� ���ǽű�������
			if (pDivPos)	
				*pDivPos++ = 0; 
			if (pDivPos)
				ExecuteScriptC(szScriptInfo, "OnCall", pDivPos,"");
			else
				ExecuteScriptC(szScriptInfo, "OnCall", "","");
		}
		break;
	}	
}


void  KPlayer::ReSkillEnhance()
{
	int nActiveSkillID;
	
	nActiveSkillID=Player[CLIENT_PLAYER_INDEX].GetLeftSkill();	
	
	if (Npc[m_nIndex].m_Currentskillenhance!= 0)
	{
		int nlistIndex = Npc[m_nIndex].m_SkillList.FindSame(nActiveSkillID);
		if (nlistIndex)                 //���Ҽ���ĵ�ǰ��������ID
		{
			if (Npc[m_nIndex].m_SkillList.GetCurrentLevelByIdx(nlistIndex) > 0)  //�ȼ�����0�Ĳ� ����
			{
				Npc[m_nIndex].m_SkillList.AddEnChance(nlistIndex,Npc[m_nIndex].m_Currentskillenhance);
			}			
		}
	}
	
//#ifdef _SERVER
	//char msg[64];
	//t_sprintf(msg,"PS�����ӳ�:%d",Npc[m_nIndex].m_Currentskillenhance);
	//m_ItemList.msgshow(msg);
//#else
	//char msg[64];
	//t_sprintf(msg,"PC�����ӳ�:%d",Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_Currentskillenhance);
	//Player[CLIENT_PLAYER_INDEX].m_ItemList.ClientShowMsg(msg);	
//#endif

}

void	KPlayer::s2cTradeChangeState(BYTE* pMsg)
{

	TRADE_CHANGE_STATE_SYNC	*pTrade = (TRADE_CHANGE_STATE_SYNC*)pMsg;
	
	switch (m_cMenuState.m_nState)
	{
	case PLAYER_MENU_STATE_NORMAL:		// ��ǰ�ͻ��� NORMAL
	case PLAYER_MENU_STATE_TRADEOPEN:	// ��ǰ�ͻ��� TRADEOPEN
		if (pTrade->m_btState == 0)
		{
			m_cMenuState.SetState(PLAYER_MENU_STATE_NORMAL);
			CoreDataChanged(GDCNI_TRADE_OPER_DATA, 0, 0);
		}
		else if (pTrade->m_btState == 1)
		{
			m_cMenuState.SetState(PLAYER_MENU_STATE_TRADEOPEN);
			CoreDataChanged(GDCNI_TRADE_OPER_DATA, 0, 0);
		}
		else if (pTrade->m_btState == 2)
		{
			m_cMenuState.SetState(PLAYER_MENU_STATE_TRADING);
			m_cTrade.StartTrade(pTrade->m_dwNpcID);
			m_ItemList.StartTrade();
			
			// ֪ͨ������뽻�׽���
			KUiPlayerItem	sTradePlayer;
			int	nIdx;
			
			nIdx = NpcSet.SearchID(pTrade->m_dwNpcID);
			if (nIdx > 0)
				strcpy(sTradePlayer.Name, Npc[nIdx].Name);
			else
				sTradePlayer.Name[0] = 0;
			sTradePlayer.nIndex = 0;
			sTradePlayer.uId = 0;
			sTradePlayer.nData = 0;
			CoreDataChanged(GDCNI_TRADE_START, (uintptr_t)(&sTradePlayer), 0);
			
			if (Npc[m_nIndex].m_Doing == do_sit)
			{
				Npc[m_nIndex].SendSerCommand(do_stand);
			}
		}
		break;
	case PLAYER_MENU_STATE_TEAMOPEN:	// ��ǰ�ͻ��� TEAMOPEN
		g_Team[0].SetTeamClose();
		if (pTrade->m_btState == 0)
		{
			m_cMenuState.SetState(PLAYER_MENU_STATE_NORMAL);
			CoreDataChanged(GDCNI_TRADE_OPER_DATA, 0, 0);
		}
		else if (pTrade->m_btState == 1)
		{
			m_cMenuState.SetState(PLAYER_MENU_STATE_TRADEOPEN);
			CoreDataChanged(GDCNI_TRADE_OPER_DATA, 0, 0);
		}
		else if (pTrade->m_btState == 2)
		{
			m_cMenuState.SetState(PLAYER_MENU_STATE_TRADING);
			m_cTrade.StartTrade(pTrade->m_dwNpcID);
			m_ItemList.StartTrade();
			
			// ֪ͨ������뽻�׽���
			KUiPlayerItem	sTradePlayer;
			int	nIdx;
			
			nIdx = NpcSet.SearchID(pTrade->m_dwNpcID);
			if (nIdx > 0)
				strcpy(sTradePlayer.Name, Npc[nIdx].Name);
			else
				sTradePlayer.Name[0] = 0;
			sTradePlayer.Name[0] = 0;
			sTradePlayer.nIndex = 0;
			sTradePlayer.uId = 0;
			sTradePlayer.nData = 0;
			CoreDataChanged(GDCNI_TRADE_START, (uintptr_t)(&sTradePlayer), 0);
			
			if (Npc[m_nIndex].m_Doing == do_sit)
			{
				Npc[m_nIndex].SendSerCommand(do_stand);
			}
		}
		break;
	case PLAYER_MENU_STATE_TRADING:		// ��ǰ�ͻ��� TRADING
		// ���ͻ��˴��� TRADING ʱ����Ӧ���յ���Э��
		if (pTrade->m_btState == 0)
		{
			m_cMenuState.SetState(PLAYER_MENU_STATE_NORMAL);
			CoreDataChanged(GDCNI_TRADE_OPER_DATA, 0, 0);
		}
		else if (pTrade->m_btState == 1)
		{
			m_cMenuState.SetState(PLAYER_MENU_STATE_TRADEOPEN);
			CoreDataChanged(GDCNI_TRADE_OPER_DATA, 0, 0);
		}
		else if (pTrade->m_btState == 2)	// �ͻ��˴�ʱ��Ӧ�������״̬���ͻ����ѳ���
		{
			m_cMenuState.m_nState = PLAYER_MENU_STATE_TRADING;
			m_cMenuState.m_nTradeDest = pTrade->m_dwNpcID;
			m_cMenuState.m_nTradeState = 0;
			m_cMenuState.m_nTradeDestState = 0;
			this->m_ItemList.RecoverTrade();
			this->m_ItemList.BackupTrade();
			this->m_ItemList.ClearRoom(room_trade);
			this->m_ItemList.ClearRoom(room_trade1);
			
			// ֪ͨ������뽻�׽���
			KUiPlayerItem	sTradePlayer;
			int	nIdx;
			
			nIdx = NpcSet.SearchID(pTrade->m_dwNpcID);
			if (nIdx > 0)
				strcpy(sTradePlayer.Name, Npc[nIdx].Name);
			else
				sTradePlayer.Name[0] = 0;
			sTradePlayer.Name[0] = 0;
			sTradePlayer.nIndex = 0;
			sTradePlayer.uId = 0;
			sTradePlayer.nData = 0;
			CoreDataChanged(GDCNI_TRADE_START, (uintptr_t)(&sTradePlayer), 0);
			
			if (Npc[m_nIndex].m_Doing == do_sit)
			{
				Npc[m_nIndex].SendSerCommand(do_stand);
			}
		}
		break;
	}
}

//�ȼ�����
void	KPlayer::s2cLevelUp(BYTE* pMsg)
{
	PLAYER_LEVEL_UP_SYNC	*pLevel = (PLAYER_LEVEL_UP_SYNC*)pMsg;

	if (Npc[m_nIndex].m_Level < pLevel->m_btLevel && m_nExp < PlayerSet.m_cLevelAdd.GetLevelExp(Npc[m_nIndex].m_Level))
	{
		KSystemMessage	sMsg;
		t_sprintf(sMsg.szMessage, strCoreInfo[MSG_GET_EXP].c_str(), PlayerSet.m_cLevelAdd.GetLevelExp(Npc[m_nIndex].m_Level) - m_nExp);
		sMsg.eType = SMT_NORMAL;
		sMsg.byConfirmType = SMCT_NONE;
		sMsg.byPriority = 0;
		sMsg.byParamSize = 0;
		sMsg.nMsgLen = TEncodeText_(sMsg.szMessage, strlen(sMsg.szMessage));
		CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (uintptr_t)&sMsg, 0);
	}
	
	m_nExp = 0;
	Npc[m_nIndex].m_Level = (WORD)pLevel->m_btLevel;
	m_nNextLevelExp = PlayerSet.m_cLevelAdd.GetLevelExp(Npc[m_nIndex].m_Level);
	
	// -------------------------- �ȼ�����ʱ��������ͬ�� ----------------------
	// ͬ��δ�������Ե�
	if (pLevel->m_nAttributePoint > m_nAttributePoint)
	{
		/*KSystemMessage Msg;
		t_sprintf(Msg.szMessage, strCoreInfo[MSG_GET_ATTRIBUTE_POINT].c_str(), pLevel->m_nAttributePoint - m_nAttributePoint);
		Msg.eType = SMT_PLAYER;
		Msg.byConfirmType = SMCT_UI_ATTRIBUTE;
		Msg.byPriority = 3;
		Msg.byParamSize = 0;
		Msg.nMsgLen = TEncodeText(Msg.szMessage, strlen(Msg.szMessage));
		CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (uintptr_t)&Msg, NULL);*/
	}
	m_nAttributePoint = pLevel->m_nAttributePoint;
	
	// ͬ��δ���似�ܵ�
	if (m_nSkillPoint < pLevel->m_nSkillPoint)
	{
		/*KSystemMessage Msg;
		t_sprintf(Msg.szMessage, strCoreInfo[MSG_GET_SKILL_POINT].c_str(), pLevel->m_nSkillPoint - m_nSkillPoint);
		Msg.eType = SMT_PLAYER;
		Msg.byConfirmType = SMCT_UI_SKILLS;
		Msg.byPriority = 3;
		Msg.byParamSize = 0;
		Msg.nMsgLen = TEncodeText(Msg.szMessage, strlen(Msg.szMessage));
		CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (uintptr_t)&Msg, NULL);*/
	}
	m_nSkillPoint = pLevel->m_nSkillPoint;
	
	// ��������������������ֵ��Ӱ������������������������ֵ�����أ��ȼ������Ե㣬Ӱ�쵱ǰ�������ֵ�����أ�װ�������ܡ�ҩ���ʱ����
	Npc[m_nIndex].m_LifeMax           = pLevel->m_nBaseLifeMax;
	Npc[m_nIndex].m_StaminaMax        = pLevel->m_nBaseStaminaMax;
	Npc[m_nIndex].m_ManaMax           = pLevel->m_nBaseManaMax;
	//	Npc[m_nIndex].ResetLifeReplenish();
	Npc[m_nIndex].m_CurrentLifeMax    = Npc[m_nIndex].m_LifeMax;
	Npc[m_nIndex].m_CurrentStaminaMax = Npc[m_nIndex].m_StaminaMax;
	Npc[m_nIndex].m_CurrentManaMax    = Npc[m_nIndex].m_ManaMax;
	Npc[m_nIndex].m_CurrentNuQiMax    = Npc[m_nIndex].m_NuqiMax;  //ŭ��
	
	// ������ֿ��Եı仯 �𡢱��������硢����
	Npc[m_nIndex].m_FireResist				= PlayerSet.m_cLevelAdd.GetFireResist(Npc[m_nIndex].m_Series, Npc[m_nIndex].m_Level);
	Npc[m_nIndex].m_CurrentFireResist		= Npc[m_nIndex].m_FireResist;
	Npc[m_nIndex].m_ColdResist				= PlayerSet.m_cLevelAdd.GetColdResist(Npc[m_nIndex].m_Series, Npc[m_nIndex].m_Level);
	Npc[m_nIndex].m_CurrentColdResist		= Npc[m_nIndex].m_ColdResist;
	Npc[m_nIndex].m_PoisonResist			= PlayerSet.m_cLevelAdd.GetPoisonResist(Npc[m_nIndex].m_Series, Npc[m_nIndex].m_Level);
	Npc[m_nIndex].m_CurrentPoisonResist		= Npc[m_nIndex].m_PoisonResist;
	Npc[m_nIndex].m_LightResist				= PlayerSet.m_cLevelAdd.GetLightResist(Npc[m_nIndex].m_Series, Npc[m_nIndex].m_Level);
	Npc[m_nIndex].m_CurrentLightResist		= Npc[m_nIndex].m_LightResist;
	Npc[m_nIndex].m_PhysicsResist			= PlayerSet.m_cLevelAdd.GetPhysicsResist(Npc[m_nIndex].m_Series, Npc[m_nIndex].m_Level);
	Npc[m_nIndex].m_CurrentPhysicsResist	= Npc[m_nIndex].m_PhysicsResist;
	Npc[m_nIndex].m_FireResistMax			= BASE_FIRE_RESIST_MAX;
	Npc[m_nIndex].m_ColdResistMax			= BASE_COLD_RESIST_MAX;
	Npc[m_nIndex].m_PoisonResistMax			= BASE_POISON_RESIST_MAX;
	Npc[m_nIndex].m_LightResistMax			= BASE_LIGHT_RESIST_MAX;
	Npc[m_nIndex].m_PhysicsResistMax		= BASE_PHYSICS_RESIST_MAX;
	/*
	Npc[m_nIndex].m_CurrentFireResistMax	= Npc[m_nIndex].m_FireResistMax;
	Npc[m_nIndex].m_CurrentColdResistMax	= Npc[m_nIndex].m_ColdResistMax;
	Npc[m_nIndex].m_CurrentPoisonResistMax	= Npc[m_nIndex].m_PoisonResistMax;
	Npc[m_nIndex].m_CurrentLightResistMax	= Npc[m_nIndex].m_LightResistMax;
	Npc[m_nIndex].m_CurrentPhysicsResistMax	= Npc[m_nIndex].m_PhysicsResistMax;
*/	
	int nOldCurCamp = Npc[m_nIndex].m_CurrentCamp;
	// ����װ����������Ϣ���µ�ǰ����
	this->UpdataCurData();
	SetNpcPhysicsDamage();
	Npc[m_nIndex].m_CurrentCamp    = nOldCurCamp;
	
	// ����������������ֵ����
	Npc[m_nIndex].m_CurrentLife    = Npc[m_nIndex].m_CurrentLifeMax;
	Npc[m_nIndex].m_CurrentStamina = Npc[m_nIndex].m_CurrentStaminaMax;
	Npc[m_nIndex].m_CurrentMana    = Npc[m_nIndex].m_CurrentManaMax;
	
	// �ͻ��˶������Լ��ĵȼ���һ
	if (this->m_cTeam.m_nFlag)
	{
		if (m_cTeam.m_nFigure == TEAM_CAPTAIN)
		{//�ӳ�
			g_Team[0].m_nMemLevel[0] = Npc[m_nIndex].m_Level;
		}
		else
		{
			for (int i = 0; i < MAX_TEAM_MEMBER; ++i)
			{
				if ((DWORD)g_Team[0].m_nMember[i] == Npc[m_nIndex].m_dwID)
				{
					g_Team[0].m_nMemLevel[i] = Npc[m_nIndex].m_Level;
					break;
				}
			}
		}
	}
	
	/*KSystemMessage Msg;	
	Msg.byConfirmType = SMCT_CLICK;
	Msg.byParamSize = 0;
	Msg.byPriority = 1;
	Msg.eType = SMT_PLAYER;
	t_sprintf(Msg.szMessage, strCoreInfo[MSG_LEVEL_UP].c_str(), Npc[m_nIndex].m_Level);
	Msg.nMsgLen = TEncodeText(Msg.szMessage, strlen(Msg.szMessage));
	CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (uintptr_t)&Msg, NULL);*/
	
	Npc[this->m_nIndex].SetInstantSpr(enumINSTANT_STATE_LEVELUP);  //�ͷ�����˲����Ч
}

void	KPlayer::s2cGetCurAttribute(BYTE* pMsg)
{
	PLAYER_ATTRIBUTE_SYNC	*pAttribute = (PLAYER_ATTRIBUTE_SYNC*)pMsg;
	m_nAttributePoint = pAttribute->m_nLeavePoint;
	int nData;
	switch (pAttribute->m_btAttribute)
	{
	case ATTRIBUTE_STRENGTH://����
		nData = pAttribute->m_nBasePoint - m_nStrength;
		m_nStrength = pAttribute->m_nBasePoint;
		m_nCurStrength = pAttribute->m_nCurPoint;
		UpdataCurData();
		SetNpcPhysicsDamage();
		break;
	case ATTRIBUTE_DEXTERITY://��
		nData = pAttribute->m_nBasePoint - m_nDexterity;
		m_nDexterity = pAttribute->m_nBasePoint;
		SetNpcAttackRating();
		SetNpcDefence();
		UpdataCurData();
		SetNpcPhysicsDamage();
		break;
	case ATTRIBUTE_VITALITY://�⹦
		nData = pAttribute->m_nBasePoint - m_nVitality;
		m_nVitality = pAttribute->m_nBasePoint;
		Npc[m_nIndex].AddBaseLifeMax(PlayerSet.m_cLevelAdd.GetLifePerVitality(Npc[m_nIndex].m_Series) * nData);
		Npc[m_nIndex].AddBaseStaminaMax(PlayerSet.m_cLevelAdd.GetStaminaPerVitality(Npc[m_nIndex].m_Series) * nData);
		UpdataCurData();
		break;
	case ATTRIBUTE_ENGERGY: //�ڹ�
		nData = pAttribute->m_nBasePoint - m_nEngergy;
		m_nEngergy = pAttribute->m_nBasePoint;
		Npc[m_nIndex].AddBaseManaMax(PlayerSet.m_cLevelAdd.GetManaPerEnergy(Npc[m_nIndex].m_Series) * nData);
		UpdataCurData();
		SetNpcPhysicsDamage();
		break;
	}
	//CoreDataChanged(GDCNI_PLAYER_RT_ATTRIBUTE, 0, 0);
	//CoreDataChanged(GDCNI_PLAYER_NEW_ATTRIBUTE, 0, 0);
}

void	KPlayer::s2cSetExp(int nExp)
{
	if (nExp > m_nExp)
	{
		KSystemMessage	sMsg;
		t_sprintf(sMsg.szMessage,"�����%d�㾭��ֵ��"/*strCoreInfo[MSG_GET_EXP].c_str()*/, nExp - m_nExp);
		sMsg.eType = SMT_NORMAL;
		sMsg.byConfirmType = SMCT_NONE;
		sMsg.byPriority = 0;
		sMsg.byParamSize = 0;
#ifdef WIN32
		sMsg.nMsgLen = TEncodeText(sMsg.szMessage, strlen(sMsg.szMessage));
		//ccMessageBox(sMsg.szMessage,"���Ӿ���");
#else
		char msg[256];
		t_sprintf(sMsg.szMessage,UTEXT(sMsg.szMessage,1).c_str());
		sMsg.nMsgLen = TEncodeText(sMsg.szMessage, strlen(sMsg.szMessage));
		//ccMessageBox(msg,"���Ӿ���");
#endif
		CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (uintptr_t)&sMsg, 0);

	}
	else if (nExp < m_nExp)
	{
		KSystemMessage	sMsg;
		t_sprintf(sMsg.szMessage, strCoreInfo[MSG_DEC_EXP].c_str(), m_nExp - nExp);//"��ʾ:����ʧ��(%d)�㾭��ֵ��"
		sMsg.eType = SMT_NORMAL;
		sMsg.byConfirmType = SMCT_NONE;
		sMsg.byPriority = 0;
		sMsg.byParamSize = 0;
		sMsg.nMsgLen = TEncodeText_(sMsg.szMessage, strlen(sMsg.szMessage));
		CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (uintptr_t)&sMsg, 0);
		//ccMessageBox("���پ���","���پ���");
	}
	
	this->m_nExp = nExp; //���鸳ֵ
}

void	KPlayer::s2cSyncMoney(BYTE* pMsg)
{
	PLAYER_FS_MONEY_SYNC	*pMoney = (PLAYER_FS_MONEY_SYNC*)pMsg;
	
	if ( CheckTrading() )
	{
		if (pMoney->m_nMoney1 >= pMoney->m_nMoney3)
		{
			m_ItemList.SetClient(pMoney->m_nMoney1 - pMoney->m_nMoney3, pMoney->m_nMoney2, pMoney->m_nMoney3);
		}
		else
		{
			m_ItemList.SetClient(0, pMoney->m_nMoney2 + pMoney->m_nMoney1 - pMoney->m_nMoney3, pMoney->m_nMoney3);
		}
		m_cTrade.m_nTradeState = 0;
		m_cTrade.m_nTradeDestState = 0;
		m_cTrade.m_nBackEquipMoney = pMoney->m_nMoney1;
		m_cTrade.m_nBackRepositoryMoney = pMoney->m_nMoney2;
	}
	else
	{
		// ��Ǯһ���ǵ�room_equipment
		int nMoney1 = m_ItemList.GetsMon(room_equipment);
		if (pMoney->m_nMoney1 - nMoney1 > 0)
		{
			KSystemMessage	sMsg;
			sMsg.eType = SMT_NORMAL;
			sMsg.byConfirmType = SMCT_NONE;
			sMsg.byPriority = 0;
			sMsg.byParamSize = 0;
			t_sprintf(sMsg.szMessage, strCoreInfo[MSG_EARN_MONEY].c_str(), pMoney->m_nMoney1 - nMoney1);
			sMsg.nMsgLen = TEncodeText_(sMsg.szMessage, strlen(sMsg.szMessage));
			CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (uintptr_t)&sMsg, 0);
		}
		m_ItemList.SetClient(pMoney->m_nMoney1, pMoney->m_nMoney2, pMoney->m_nMoney3);
	}
	m_Autoplay.nShiQuNum=0;
}
void	KPlayer::s2cSyncXu(BYTE* pMsg)
{
		PLAYER_FS_XU_SYNC	*pXu = (PLAYER_FS_XU_SYNC*)pMsg;
		int nXu = m_ItemList.GetxLient(room_equipment);
		if (pXu->m_nXu - nXu > 0)
		{
			KSystemMessage	sMsg;
			sMsg.eType = SMT_NORMAL;
			sMsg.byConfirmType = SMCT_NONE;
			sMsg.byPriority = 0;
			sMsg.byParamSize = 0;
			t_sprintf(sMsg.szMessage, strCoreInfo[MSG_EARN_XU].c_str(), pXu->m_nXu - nXu);
			sMsg.nMsgLen = TEncodeText_(sMsg.szMessage, strlen(sMsg.szMessage));
			CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (uintptr_t)&sMsg, 0);
		}
		m_ItemList.SetServer(pXu->m_nXu);//���ÿͻ��˽������
}

//---------------------------------------------------------------------
//	���ܣ��յ�������֪ͨ�������뽻��
//---------------------------------------------------------------------
void	KPlayer::s2cTradeApplyStart(BYTE* pMsg)
{
	if (!pMsg)
		return;
	TRADE_APPLY_START_SYNC	*pApply = (TRADE_APPLY_START_SYNC*)pMsg;
	int		nNpcIdx;
	nNpcIdx = NpcSet.SearchID(pApply->m_dwNpcId);
	if (nNpcIdx == 0)
		return;
	
	KSystemMessage	sMsg;
	KUiPlayerItem	sPlayer;
	char nTempName[32];
	ZeroMemory(nTempName,sizeof(nTempName));
	t_sprintf(nTempName,Npc[nNpcIdx].Name);
	strcpy(sPlayer.Name, nTempName);
	sPlayer.nIndex = pApply->m_nDestIdx;
	sPlayer.uId    = pApply->m_dwNpcId;
	sPlayer.nData  = 0;
	
	t_sprintf(sMsg.szMessage, strCoreInfo[MSG_TRADE_GET_APPLY].c_str(), nTempName);
	sMsg.eType = SMT_SYSTEM;
	sMsg.byConfirmType = SMCT_UI_TRADE;
	sMsg.byPriority = 3;
	sMsg.byParamSize = sizeof(KUiPlayerItem);
	sMsg.nMsgLen = TEncodeText_(sMsg.szMessage, strlen(sMsg.szMessage));
	CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (uintptr_t)&sMsg, (intptr_t)&sPlayer);
}

void	KPlayer::s2cTradeMoneySync(BYTE* pMsg)
{
	TRADE_MONEY_SYNC	*pMoney = (TRADE_MONEY_SYNC*)pMsg;
	
	m_ItemList.SetRoomMoney(room_trade1, pMoney->m_nMoney);
	
	// ֪ͨ����
	KUiObjAtRegion	sMoney;
	sMoney.Obj.uGenre = CGOG_MONEY;
	sMoney.Obj.uId = pMoney->m_nMoney;
	CoreDataChanged(GDCNI_TRADE_DESIRE_ITEM, (uintptr_t)&sMoney, 0);
}

//---------------------------------------------------------------------
//	���ܣ��յ�������֪ͨ������ɻ�ȡ��
//---------------------------------------------------------------------
void	KPlayer::s2cTradeDecision(BYTE* pMsg)
{
	TRADE_DECISION_SYNC	*pSync = (TRADE_DECISION_SYNC*)pMsg;
	if (pSync->m_btDecision == 1)	// ��ɽ���
	{
		KSystemMessage	sMsg;
		t_sprintf(sMsg.szMessage, strCoreInfo[MSG_TRADE_SUCCESS].c_str(), m_cTrade.m_szDestName);
		sMsg.eType = SMT_NORMAL;
		sMsg.byConfirmType = SMCT_NONE;
		sMsg.byPriority = 0;
		sMsg.byParamSize = 0;
		sMsg.nMsgLen = TEncodeText_(sMsg.szMessage, strlen(sMsg.szMessage));
		CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (uintptr_t)&sMsg, 0);
		
		// ����ItemList.Remove����� modify by spe 03/06/14
		//m_ItemList.ClearRoom(room_trade);
		m_ItemList.RemoveAllInOneRoom(room_trade1);
		//m_ItemList.ClearRoom(room_trade1);
		m_ItemList.ClearRoom(room_tradeback);
		this->m_cTrade.Release();
		m_cMenuState.SetState(PLAYER_MENU_STATE_NORMAL);
		
		// ֪ͨ���潻�׽���
		CoreDataChanged(GDCNI_TRADE_END, 0, 0);
		
		//m_ItemList.MenuSetMouseItem();
	}
	else if (pSync->m_btDecision == 0)	// ȡ������
	{
		if (!CheckTrading())
			return;
		
		KSystemMessage	sMsg;
		t_sprintf(sMsg.szMessage, strCoreInfo[MSG_TRADE_FAIL].c_str(), m_cTrade.m_szDestName);
		sMsg.eType = SMT_NORMAL;
		sMsg.byConfirmType = SMCT_NONE;
		sMsg.byPriority = 0;
		sMsg.byParamSize = 0;
		sMsg.nMsgLen = TEncodeText_(sMsg.szMessage, strlen(sMsg.szMessage));
		CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (uintptr_t)&sMsg, 0);
		
		// ȡ�����׵����ݻָ�
		m_ItemList.RecoverTrade();
		m_ItemList.SetClient(m_cTrade.m_nBackEquipMoney, m_cTrade.m_nBackRepositoryMoney, 0);
		m_ItemList.ClearRoom(room_trade);
		m_ItemList.RemoveAllInOneRoom(room_trade1);
		//		m_ItemList.ClearRoom(room_trade1);
		m_ItemList.ClearRoom(room_tradeback);
		m_cTrade.Release();
		m_cMenuState.SetState(PLAYER_MENU_STATE_NORMAL);
		
		// ֪ͨ���潻�׽���
		CoreDataChanged(GDCNI_TRADE_END, 0, 0);
		// ������Ʒ���
		// CoreDataChanged(GDCNI_CONTAINER_OBJECT_CHANGED,UOC_ITEM_TAKE_WITH, 0);
		
		//m_ItemList.MenuSetMouseItem();
	}
}
//�ͻ���  ������Ҳ�������
void	KPlayer::SyncCurPlayer(BYTE* pMsg)
{//����
	m_ItemList.RemoveAll();  //ԭ���е�
	//ccMessageBox("����ͬ������","SyncCurPlayer");
	CURPLAYER_SYNC* PlaySync = (CURPLAYER_SYNC *)pMsg;

	this->m_nIndex = NpcSet.SearchID(PlaySync->m_dwID); //NPC������
	//char mgs[128]={0};
	if (this->m_nIndex<=0)
	{
		//t_sprintf(mgs,"���ݴ���.����:%d,dwid:%u,�ȼ�:%d",this->m_nIndex,PlaySync->m_dwID,PlaySync->m_btLevel);
		//ccMessageBox(mgs,"SyncCurPlayer");
		return;
	}

	this->m_dwID          = g_FileName2Id(Npc[m_nIndex].Name); //ϡ����
	Npc[m_nIndex].m_Kind  = kind_player;
	Npc[m_nIndex].m_Level = (WORD)PlaySync->m_btLevel;
	Npc[m_nIndex].m_nSex  = PlaySync->m_btSex;
	Npc[m_nIndex].m_Series= PlaySync->m_btSeries;
	Npc[m_nIndex].SetPlayerIdx(CLIENT_PLAYER_INDEX); //=1
	
	m_nAttributePoint = PlaySync->m_wAttributePoint;
	m_nSkillPoint     = PlaySync->m_wSkillPoint;
	m_nStrength       = PlaySync->m_wStrength;
	m_nDexterity      = PlaySync->m_wDexterity;
	m_nVitality       = PlaySync->m_wVitality;
	m_nEngergy        = PlaySync->m_wEngergy;
	m_nLucky          = PlaySync->m_wLucky;
	m_nCurStrength    = m_nStrength;
	m_nCurDexterity   = m_nDexterity;
	m_nCurVitality    = m_nVitality;
	m_nCurEngergy     = m_nEngergy;
	m_nCurLucky       = m_nLucky;
	SetFirstDamage();   //���û����˺�
	SetBaseAttackRating();
	SetBaseDefence();
	Npc[m_nIndex].m_ActionScriptID = 0;
	Npc[m_nIndex].m_TrapScriptID = 0;
	m_nExp               = PlaySync->m_nExp;
	m_nNextLevelExp      =PlayerSet.m_cLevelAdd.GetLevelExp(PlaySync->m_btLevel);
	m_dwLeadExp          =PlaySync->m_dwLeadExp;

	m_dwLeadLevel        =PlayerSet.m_cLeadExp.GetLevel(m_dwLeadExp);
	m_dwNextLevelLeadExp =PlayerSet.m_cLeadExp.GetLevelExp(m_dwLeadLevel);
	
	m_cFaction.m_nCurFaction = (char)PlaySync->m_btCurFaction;
	m_cFaction.m_nFirstAddFaction = (char)PlaySync->m_btFirstFaction;
	m_cFaction.m_nAddTimes = PlaySync->m_nFactionAddTimes;
	
	m_nWorldStat = (int)PlaySync->m_wWorldStat;//��������
	m_nSectStat = (int)PlaySync->m_wSectStat; //�����е�����
	//char mgs[128]={0};
	//t_sprintf(mgs,"A����:%s ����:%d,dwid:%u,�ȼ�:%d",Npc[m_nIndex].Name,this->m_nIndex,PlaySync->m_dwID,PlaySync->m_btLevel);
	//ccMessageBox(mgs,"SyncCurPlayer");
	m_ItemList.Init(CLIENT_PLAYER_INDEX);
	m_ItemList.SetClient(PlaySync->m_nMoney1, PlaySync->m_nMoney2, 0);
	m_ItemList.SetServer(PlaySync->m_nXu);
	Npc[m_nIndex].m_LifeMax = PlaySync->m_wLifeMax;
	Npc[m_nIndex].m_ManaMax = PlaySync->m_wManaMax;
	Npc[m_nIndex].m_NuqiMax = PlaySync->m_wNuqiMax;  //ŭ��
	Npc[m_nIndex].m_StaminaMax = PlaySync->m_wStaminaMax;

	Npc[m_nIndex].m_LifeReplenish = PLAYER_LIFE_REPLENISH;
	Npc[m_nIndex].m_ManaReplenish = PLAYER_MANA_REPLENISH;
	Npc[m_nIndex].m_StaminaGain   = PLAYER_STAMINA_GAIN;
	Npc[m_nIndex].m_StaminaLoss   = PLAYER_STAMINA_LOSS;
	SetBaseResistData();
	SetBaseSpeedAndRadius();
	Npc[m_nIndex].RestoreNpcBaseInfo();
	m_BuyInfo.Clear();
	m_cMenuState.Release();
//	m_cChat.Release();
	memset(m_szTaskAnswerFun, 0, sizeof(m_szTaskAnswerFun));
	memset(m_szTaskAnswerCanshu, 0, sizeof(m_szTaskAnswerCanshu));
	m_nAvailableAnswerNum = 0;

	Npc[m_nIndex].m_Experience = 0;
	memset(Npc[m_nIndex].m_szChatBuffer, 0, sizeof(Npc[m_nIndex].m_szChatBuffer));
	Npc[m_nIndex].m_nCurChatTime = 0;
	
	m_RunStatus = 1;
	m_nLeftSkillID = 0;
	m_nLeftSkillLevel = 0;
	m_nRightSkillID = 0;
	m_nRightSkillLevel = 0;
	m_nPeapleIdx = 0;
	m_nObjectIdx = 0;
	m_MouseDown[0] = 0;
	m_MouseDown[1] = 0;
	Npc[m_nIndex].m_SyncSignal = 0;

	//m_bIsPlayerInGame = true;//��������Ѿ��ڽ�����Ϸ״̬
	/*this->m_nIndex = NpcSet.SearchID(PlaySync->m_dwID);
	this->m_dwID = g_FileName2Id(Npc[m_nIndex].Name); //ϡ����
	Npc[m_nIndex].m_Kind = kind_player;
	Npc[m_nIndex].m_Level = (WORD)PlaySync->m_btLevel;*/
	//char mgs[128]={0};
//	t_sprintf(mgs,"����:%s ����:%d,dwid:%u,�ȼ�:%d",Npc[m_nIndex].Name,this->m_nIndex,PlaySync->m_dwID,PlaySync->m_btLevel);
	//ccMessageBox(mgs,"SyncCurPlayer");
}


BOOL KPlayer::CheckTrading()
{
	return (m_cMenuState.m_nState == PLAYER_MENU_STATE_TRADING);
}
//���û����˺�
void	KPlayer::SetFirstDamage()
{
	int nDamageBase;

	nDamageBase = m_nCurStrength / STRENGTH_SET_DAMAGE_VALUE + 1;   //�������� 6 �� �����˺�
	
	Npc[m_nIndex].m_PhysicsDamage.nValue[0] = nDamageBase;
	Npc[m_nIndex].m_PhysicsDamage.nValue[2] = nDamageBase;
	Npc[m_nIndex].m_PhysicsDamage.nValue[1] = 0;

	Npc[m_nIndex].m_CurrentPhysicsMagicDamageV.nValue[0] = 0;
	Npc[m_nIndex].m_CurrentPhysicsMagicDamageV.nValue[2] = 0;
	Npc[m_nIndex].m_CurrentPhysicsMagicDamageV.nValue[1] = 0;

	Npc[m_nIndex].m_CurrentPhysicsMagicDamageP.nValue[0] = 0;
	Npc[m_nIndex].m_CurrentPhysicsMagicDamageP.nValue[2] = 0;
	Npc[m_nIndex].m_CurrentPhysicsMagicDamageP.nValue[1] = 0;
	
	Npc[m_nIndex].m_CurrentMagicFireDamage.nValue[0] = 0;
	Npc[m_nIndex].m_CurrentMagicFireDamage.nValue[1] = 0;
	Npc[m_nIndex].m_CurrentMagicFireDamage.nValue[2] = 0;
	
	Npc[m_nIndex].m_CurrentMagicColdDamage.nValue[0] = 0;
	Npc[m_nIndex].m_CurrentMagicColdDamage.nValue[1] = 0;
	Npc[m_nIndex].m_CurrentMagicColdDamage.nValue[2] = 0;
	
	Npc[m_nIndex].m_CurrentMagicLightDamage.nValue[0] = 0;
	Npc[m_nIndex].m_CurrentMagicLightDamage.nValue[1] = 0;
	Npc[m_nIndex].m_CurrentMagicLightDamage.nValue[2] = 0;
	
	Npc[m_nIndex].m_CurrentMagicPoisonDamage.nValue[0] = 0;
	Npc[m_nIndex].m_CurrentMagicPoisonDamage.nValue[1] = 0;
	Npc[m_nIndex].m_CurrentMagicPoisonDamage.nValue[2] = 0;
	
}

void	KPlayer::SetBaseAttackRating()
{
	Npc[m_nIndex].m_AttackRating = m_nDexterity * 4 - 28;
}

void	KPlayer::SetBaseDefence()
{
	Npc[m_nIndex].m_Defend = m_nDexterity >> 2;
}
//ԭʼ�����
void	KPlayer::SetBaseResistData()
{
	Npc[m_nIndex].m_FireResist			= PlayerSet.m_cLevelAdd.GetFireResist(Npc[m_nIndex].m_Series, Npc[m_nIndex].m_Level);
	Npc[m_nIndex].m_ColdResist			= PlayerSet.m_cLevelAdd.GetColdResist(Npc[m_nIndex].m_Series, Npc[m_nIndex].m_Level);
	Npc[m_nIndex].m_PoisonResist		= PlayerSet.m_cLevelAdd.GetPoisonResist(Npc[m_nIndex].m_Series, Npc[m_nIndex].m_Level);
	Npc[m_nIndex].m_LightResist			= PlayerSet.m_cLevelAdd.GetLightResist(Npc[m_nIndex].m_Series, Npc[m_nIndex].m_Level);
	Npc[m_nIndex].m_PhysicsResist		= PlayerSet.m_cLevelAdd.GetPhysicsResist(Npc[m_nIndex].m_Series, Npc[m_nIndex].m_Level);
	Npc[m_nIndex].m_FireResistMax		= BASE_FIRE_RESIST_MAX;
	Npc[m_nIndex].m_ColdResistMax		= BASE_COLD_RESIST_MAX;
	Npc[m_nIndex].m_PoisonResistMax		= BASE_POISON_RESIST_MAX;
	Npc[m_nIndex].m_LightResistMax		= BASE_LIGHT_RESIST_MAX;
	Npc[m_nIndex].m_PhysicsResistMax	= BASE_PHYSICS_RESIST_MAX;
}

void	KPlayer::SetBaseSpeedAndRadius()
{
	Npc[m_nIndex].m_WalkSpeed    = BASE_WALK_SPEED;
	Npc[m_nIndex].m_RunSpeed     = BASE_RUN_SPEED;
	Npc[m_nIndex].m_AttackSpeed  = BASE_ATTACK_SPEED;
	Npc[m_nIndex].m_CastSpeed    = BASE_CAST_SPEED;
	Npc[m_nIndex].m_VisionRadius = BASE_VISION_RADIUS;
	Npc[m_nIndex].m_HitRecover   = BASE_HIT_RECOVER;
}

//�ͻ��˰汾   �Ի�NPC ִ�нű�
void KPlayer::DialogNpc(int nIndex,BOOL nIsServer)
{
	if (nIndex > 0 && Npc[nIndex].m_Index > 0)
	{
		//char nMgs[256]={0};
		//if (Npc[nIndex].GetstrInfo(STR_ACTION_SCRIPT,nMgs))
		if (!nIsServer)
		{
			//ExecuteScriptD(Npc[nIndex].m_ActionScriptID,"main",Npc[nIndex].Name,"",nIndex,Npc[nIndex].m_dwID,0,0,0,TRUE);
		}
		else
		{
			PLAYER_DIALOG_NPC_COMMAND DialogNpcCmd;
			DialogNpcCmd.nNpcId = Npc[nIndex].m_dwID;
			DialogNpcCmd.ProtocolType = c2s_dialognpc;		
		if (g_pClient)
			g_pClient->SendPackToServer(&DialogNpcCmd, sizeof(PLAYER_DIALOG_NPC_COMMAND));
			
		}
	}
}

void KPlayer::OpenBaiTan(DWORD nIndwId)
{
	g_cSellItem.ApplyViewItem(nIndwId);
}

void KPlayer::CheckObject(int nIdx)
{
/*	enum	// �������
{
Obj_Kind_MapObj = 0,		// ��ͼ�������Ҫ���ڵ�ͼ����
Obj_Kind_Body,				// npc ��ʬ��
Obj_Kind_Box,				// ����
Obj_Kind_Item,				// ���ڵ��ϵ�װ��
Obj_Kind_Money,				// ���ڵ��ϵ�Ǯ
Obj_Kind_LoopSound,			// ѭ����Ч
Obj_Kind_RandSound,			// �����Ч
Obj_Kind_Light,				// ��Դ��3Dģʽ�з���Ķ�����
Obj_Kind_Door,				// ����
Obj_Kind_Trap,				// ����
Obj_Kind_Prop,				// С���ߣ������� ������
Obj_Kind_Num,				// �����������
};*/
	
	switch(Object[nIdx].m_nKind)
	{
//////ԭ��û�е�/////////////
	case Obj_Kind_Item:
		PickUpObj(nIdx);
		Npc[m_nIndex].m_nObjectIdx = 0;
		break;
	case Obj_Kind_Money:
		PickUpObj(nIdx);
		Npc[m_nIndex].m_nObjectIdx = 0;
		break;
	case Obj_Kind_Box:
	case Obj_Kind_Door:
	case Obj_Kind_Trap:  //������ 
	case Obj_Kind_Prop:	 //�������
		this->ObjMouseClick(nIdx);
		Npc[m_nIndex].m_nIsOver=TRUE;
		Npc[m_nIndex].m_nObjectIdx = 0;
		break;
	default:
		break;
	}
	m_nObjectIdx = 0;
	m_nPickObjectIdx = 0;
}

void KPlayer::DrawSelectInfo()   //ѡ��������� ����ʾ
{
	if (m_nIndex <= 0)
		return;

	if (m_nPeapleIdx)
	{//npc
		if (Npc[m_nPeapleIdx].m_Kind == kind_player) // ���
		{
			if (!NpcSet.CheckShowName())
			{
				Npc[m_nPeapleIdx].PaintInfo(Npc[m_nPeapleIdx].GetNpcPate(), true);
			}
		}
		else if (Npc[m_nPeapleIdx].m_Kind == kind_dialoger)  //�Ի���
		{
			if (!NpcSet.CheckShowName())
			{
			    Npc[m_nPeapleIdx].PaintInfo(Npc[m_nPeapleIdx].GetNpcPate(), true);
			}
		}
		else if (Npc[m_nPeapleIdx].m_Kind == kind_partner)  //ͬ����
		{
			if (!NpcSet.CheckShowName())
			{
				Npc[m_nPeapleIdx].PaintInfo(Npc[m_nPeapleIdx].GetNpcPate(), true);
			}
		}
		else   //����ðѪ����
		{
			Npc[m_nPeapleIdx].DrawBlood();   //����Ѫ��  ðѪ����
		}

		return;
	}
	if (m_nObjectIdx)
	{//��Ʒ
		if (!ObjSet.CheckShowName())
			Object[m_nObjectIdx].DrawInfo();
		//	Object[m_nObjectIdx].DrawBorder();
		return;
	}
}

//��ȡ����������С�����˺�
//#ifndef _SERVER
void KPlayer::GetEchoDamage(int* nMin, int* nMax, int nType)
{
	int nSkillId;
	*nMin = 0;
	*nMax = 0;
	
	if (m_nIndex <= 0 || m_nIndex >= MAX_NPC || nType < 0 || nType > 1 || NULL == nMin || NULL == nMax)
		return;
	//���Ҽ���ѡ��
	nSkillId = (0 == nType)?m_nLeftSkillID:m_nRightSkillID;

	int nListidx  = Npc[m_nIndex].m_SkillList.FindSame(nSkillId);
	int nLevel    = Npc[m_nIndex].m_SkillList.GetCurrentLevelByIdx(nListidx);
	int nEnChance = Npc[m_nIndex].m_SkillList.GetEnChance(nListidx);

	if (nSkillId <= 0 ||nLevel <= 0 || nLevel >= MAX_SKILLLEVEL)
		return;
	
	KMagicAttrib*	pMagicData = NULL;
	
	KSkill * pOrdinSkill = (KSkill *) g_SkillManager.GetSkill(nSkillId, nLevel);
	if (!pOrdinSkill) 
        return ;

	//ccMessageBox(pOrdinSkill->GetSkillName(),"setleftskill");
	switch(pOrdinSkill->GetSkillStyle())
	{
	case SKILL_SS_Missles:		        	//	�ӵ���		���������ڷ����ӵ���
        break;
	case SKILL_SS_Melee:                    //
	    break;
	case SKILL_SS_InitiativeNpcState:	    //	������		���������ڸı䵱ǰNpc������״̬
	    break;
	case SKILL_SS_PassivityNpcState:		//	������		���������ڸı�Npc�ı���״̬
		break;
	default:
		return;
	}
	
	BOOL	bIsPhysical = pOrdinSkill->IsPhysical();          //������
	BOOL	bIsMagic    = pOrdinSkill->IsMagic();             //1�⹦ϵ ��0�ڹ�ϵ

	        pMagicData = pOrdinSkill->GetDamageAttribs();     //������������
	
	if (!pMagicData)
		return;
	//�����˺�+���������˺�+���յ�=�������˺�
	int nMinNpcDamage = Npc[m_nIndex].m_PhysicsDamage.nValue[0]+Npc[m_nIndex].m_CurrentPhysicsMagicDamageV.nValue[0];
	int nMaxNpcDamage = Npc[m_nIndex].m_PhysicsDamage.nValue[2]+Npc[m_nIndex].m_CurrentPhysicsMagicDamageV.nValue[2];

	int nEnhancew=0;
	if (Npc[m_nIndex].IsPlayer())
	{//������
/*	   int nTempVal =m_cVip.GetRoleVipNum()*m_cVip.GetRoleVipDamageMaxVal();
		   nMinNpcDamage +=nTempVal;
		   nMaxNpcDamage +=nTempVal;
*/
	   if (equip_meleeweapon == m_ItemList.GetWeaponType())
	   { 
		nEnhancew = Npc[m_nIndex].m_CurrentMeleeEnhance[m_ItemList.GetWeaponParticular()];
	   } 
	   else if (equip_rangeweapon == m_ItemList.GetWeaponType())
	   {  
		nEnhancew = Npc[m_nIndex].m_CurrentRangeEnhance;
	   } 
	   else 
	   { 
		nEnhancew = Npc[m_nIndex].m_CurrentHandEnhance;
	   } 
	}
//�����˺�
	*nMin += nMinNpcDamage +(nMinNpcDamage*nEnhancew)/100;
	*nMax += nMaxNpcDamage +(nMinNpcDamage*nEnhancew)/100;

	if (magic_attackrating_v == pMagicData->nAttribType) //���е�
	{
	    Npc[m_nIndex].m_CurrentSkillMingZhong = Npc[m_nIndex].m_AttackRating+pMagicData->nValue[0];
	}
	else if (magic_attackrating_p == pMagicData->nAttribType)   //�����ʰٷֱ�
	{
		Npc[m_nIndex].m_CurrentSkillMingZhong = Npc[m_nIndex].m_AttackRating*(100+pMagicData->nValue[0])/100;
	}
	else
	   Npc[m_nIndex].m_CurrentSkillMingZhong = 0;

	pMagicData++; //1 //������
	if  (magic_ignoredefense_p== pMagicData->nAttribType)
	{

	}
	pMagicData++; //2 �յ��˺�+�հٷֱ��˺�

	if (magic_physicsenhance_p == pMagicData->nAttribType)
	{
		int Vmin=0,Vmax=0;

		    Vmin = (*nMin)*pMagicData->nValue[0]/100;
            Vmax = (*nMax)*pMagicData->nValue[0]/100;

		*nMin += Vmin;
		*nMax += Vmax;	

	//*nMin += nMinNpcDamage * nEnhancew / 100;
	//*nMax += nMaxNpcDamage * nEnhancew / 100;

	if (bIsMagic)
	{//�ű���������+����״̬(װ��+���ܵ�״̬)
		*nMin = (*nMin+Npc[m_nIndex].m_CurrentAddmagicphysicsDamage)*(100+Npc[m_nIndex].m_CurrentAddmagicphysicsDamageP)/100;
		*nMax = (*nMax+Npc[m_nIndex].m_CurrentAddmagicphysicsDamage)*(100+Npc[m_nIndex].m_CurrentAddmagicphysicsDamageP)/100;
	}
	else
	{//�ű���������+����״̬(װ��+���ܵ�״̬)
		*nMin = (*nMin+Npc[m_nIndex].m_CurrentAddPhysicsDamage)*(100+Npc[m_nIndex].m_CurrentAddPhysicsDamageP)/100;
		*nMax = (*nMax+Npc[m_nIndex].m_CurrentAddPhysicsDamage)*(100+Npc[m_nIndex].m_CurrentAddPhysicsDamageP)/100;
	} 
	}
	pMagicData++; // 3 ���˺�
	if (magic_colddamage_v == pMagicData->nAttribType)
	{
		*nMin += pMagicData->nValue[0];
		*nMax += pMagicData->nValue[2];//*(100+ Npc[m_nIndex].m_CurrentColdEnhance)/100;	

	if (bIsMagic)  //�ڱ�
	{
		*nMin += Npc[m_nIndex].m_CurrentMagicColdDamage.nValue[0]+Npc[m_nIndex].m_CurrentAddmagicColdDamagicv;
		*nMax += Npc[m_nIndex].m_CurrentMagicColdDamage.nValue[2]+Npc[m_nIndex].m_CurrentAddmagicColdDamagicv;
	}
	else
	{ 
		*nMin += Npc[m_nIndex].m_CurrentMagicColdDamage.nValue[0]+Npc[m_nIndex].m_CurrentAddColdDamagev;
		*nMax += Npc[m_nIndex].m_CurrentMagicColdDamage.nValue[2]+Npc[m_nIndex].m_CurrentAddColdDamagev;
	} 
	}
	pMagicData++; //4  ���˺�
	if (magic_firedamage_v == pMagicData->nAttribType)
	{
		*nMin += pMagicData->nValue[0];
		*nMax += pMagicData->nValue[2]*(100 + Npc[m_nIndex].m_CurrentFireEnhance)/ 100;
	if (bIsMagic)
	{//�ڹ�
		*nMin += Npc[m_nIndex].m_CurrentMagicFireDamage.nValue[0]+Npc[m_nIndex].m_CurrentAddmagicFireDamagicv;
		*nMax += Npc[m_nIndex].m_CurrentMagicFireDamage.nValue[2]+Npc[m_nIndex].m_CurrentAddmagicFireDamagicv;
	}
	else
	{//�⹦
		*nMin += Npc[m_nIndex].m_CurrentMagicFireDamage.nValue[0]+Npc[m_nIndex].m_CurrentAddFireDamagev;
		*nMax += Npc[m_nIndex].m_CurrentMagicFireDamage.nValue[2]+Npc[m_nIndex].m_CurrentAddFireDamagev;
	} 
	}
	pMagicData++;// 5 ���˺�
	if (magic_lightingdamage_v == pMagicData->nAttribType)
	{
		*nMin += pMagicData->nValue[0];
		*nMax += pMagicData->nValue[2]; 

	if (bIsMagic)
	{
		*nMin += Npc[m_nIndex].m_CurrentMagicLightDamage.nValue[0]+Npc[m_nIndex].m_CurrentAddmagicLightDamagicv;
		*nMax += Npc[m_nIndex].m_CurrentMagicLightDamage.nValue[2]+Npc[m_nIndex].m_CurrentAddmagicLightDamagicv;
	}
	else
	{ 
		*nMin += Npc[m_nIndex].m_CurrentMagicLightDamage.nValue[0]+Npc[m_nIndex].m_CurrentAddLighDamagev;
		*nMax += Npc[m_nIndex].m_CurrentMagicLightDamage.nValue[2]+Npc[m_nIndex].m_CurrentAddLighDamagev;
	}
	}
	pMagicData++; // 6 ���˺�
	if (magic_poisondamage_v == pMagicData->nAttribType)
	{
			*nMin += pMagicData->nValue[0];//* (100 +Npc[m_nIndex].m_CurrentPoisonEnhance) / 100;//���ܻ�������
			*nMax += pMagicData->nValue[0]; 

	  if (bIsMagic)
	  {//�ڶ�  
		*nMin += Npc[m_nIndex].m_CurrentMagicPoisonDamage.nValue[0]+Npc[m_nIndex].m_CurrentAddmagicPoisonDamagicv;
		*nMax += Npc[m_nIndex].m_CurrentMagicPoisonDamage.nValue[0]+Npc[m_nIndex].m_CurrentAddmagicPoisonDamagicv;
	  }  
	  else
	  {//�ⶾ  
		int nPoisonDamage = Npc[m_nIndex].m_CurrentMagicPoisonDamage.nValue[0]+Npc[m_nIndex].m_CurrentAddPoisonDamagev;
		*nMin += nPoisonDamage;
		*nMax += nPoisonDamage;
	  }  
	}
	pMagicData++;// 7 �����˺������ӷ�����

	if (magic_magicdamage_v == pMagicData->nAttribType)
	{ 
		*nMin += pMagicData->nValue[0];
		*nMax += pMagicData->nValue[2];
	}
	pMagicData++;//8
	pMagicData++;//9
	pMagicData++;//10
	pMagicData++;//11
	pMagicData++;//12
	pMagicData++;//13
	pMagicData++;//14
	pMagicData++;//15
	if (magic_physicsdamage_v == pMagicData->nAttribType) //�յ��˺� ��
	{
		//*nMin += pMagicData->nValue[0];
		//*nMax += pMagicData->nValue[2];
      if (bIsMagic)
	  {//�ű���������+����״̬(װ��+���ܵ�״̬)
		*nMin += (pMagicData->nValue[0]+Npc[m_nIndex].m_CurrentAddmagicphysicsDamage)*(100+Npc[m_nIndex].m_CurrentAddmagicphysicsDamageP)/100;
		*nMax += (pMagicData->nValue[2]+Npc[m_nIndex].m_CurrentAddmagicphysicsDamage)*(100+Npc[m_nIndex].m_CurrentAddmagicphysicsDamageP)/100;
	  }
	  else
	  {//�ű���������+����״̬(װ��+���ܵ�״̬)
		*nMin += (pMagicData->nValue[0]+Npc[m_nIndex].m_CurrentAddPhysicsDamage)*(100+Npc[m_nIndex].m_CurrentAddPhysicsDamageP)/100;
		*nMax += (pMagicData->nValue[2]+Npc[m_nIndex].m_CurrentAddPhysicsDamage)*(100+Npc[m_nIndex].m_CurrentAddPhysicsDamageP)/100;
	  } 
    }
	pMagicData++;//16
	pMagicData++;//17
	pMagicData++;//18
	
	int nCurbei=0;//nFomoidx=Npc[m_nIndex].GetCurFuMoIdx(),;
	
	/*if 	(nFomoidx>-1)
	{
		if (Npc[m_nIndex].GetCurFoMoSkllLevel(nFomoidx)>=5) //��ȡ�񽫵�ǰ�ĵȼ�
		{
			//nCurbei=Npc[m_nIndex].TakeTrader(Npc[m_nIndex].GetCurFoMoSkllLevel(nFomoidx),5);//Npc[m_nIndex].GetCurFoMoSkllLevel(nFomoidx)/5;
			nCurbei=Npc[m_nIndex].GetCurFoMoSkllLevel(nFomoidx)/5;
		}

	}*/
  /*
	*nMin += Npc[m_nIndex].TakeTrader(*nMin * nEnChance,100);//*nMin * nEnChance/100;
	*nMax += Npc[m_nIndex].TakeTrader(*nMax * nEnChance,100);//*nMax * nEnChance/100; 

	*nMin = Npc[m_nIndex].TakeTrader(*nMin * (nCurbei*5+100),100);//*nMin * (nCurbei*5+100)/100;
	*nMax = Npc[m_nIndex].TakeTrader(*nMax * (nCurbei*5+100),100);//*nMax * (nCurbei*5+100)/100;
   */
	*nMin += *nMin * nEnChance/100;//Npc[m_nIndex].TakeTrader((*nMin)*nEnChance,100);//*nMin * nEnChance/100;
	*nMax += *nMax * nEnChance/100;//Npc[m_nIndex].TakeTrader((*nMax)*nEnChance,100);//*nMax * nEnChance/100; 

	*nMin = *nMin * (nCurbei*5+100)/100;//Npc[m_nIndex].TakeTrader((*nMin)*(nCurbei*5+100),100);//*nMin * (nCurbei*5+100)/100;
	*nMax = *nMax * (nCurbei*5+100)/100;//Npc[m_nIndex].TakeTrader((*nMax)*(nCurbei*5+100),100);//*nMax * (nCurbei*5+100)/100;
}
//#endif

//��װ�ǵ��ú������������˺�
void KPlayer::SetNpcDamageAttrib()
{
	SetNpcPhysicsDamage();
}
//���ÿ���� Ŀ���ͼ��

void KPlayer::SetExtGameTime(DWORD nPoint)
{
	m_GameLeftTime       = nPoint;
}

UINT KPlayer::GetExtPoint()
{
	return m_nExtPoint;  //��չ�� ��Ա��ֵ
}

// ��Ʒ��Ӧ�Ĳ�����
//     �����Ʒ��1���ӵ��ϼ���Ʒ 2���ű�����ֱ�Ӹ� 3������ 4��player ֮����
//               ��(ͨ������ʵ��)
//         �ͻ�������������ϣ�Ȼ��ͻ���ͨ������ó���Ʒid��Ӧ�ó�����װ��������
//         ��Ʒ����λ�û��߸������ļ��㣬�Ѽ������������������������յ���������
//         ���������Ʒ��λ�ù�ϵ��Ȼ����ͻ��˵ļ������Ƿ���ȷ��Ȼ�������Ӧ��
//         �����Ѵ����������ͻ���
//     ��Ʒ��λ�õ������ͻ��˴������Ʒ��Դλ�á��������ƷĿ��λ�ã�Ȼ�󷢸���������
//     ������ȷ�Ϻ�֪ͨ�ͻ�������ȷ���Ĳ���������ɹ�����Э��ԭ������ȥ�����������
//     ֵ�������仯������֪ͨ�ͻ��ˣ�
//     ʹ����Ʒ��1����ҩ 2��װ��������Ҽ������
//         �ͻ�����������������ʲôλ�õ�ҩ��ͬʱ�ͻ��˵���Ӧ��ֵ������Ӧ�仯����
//         �����յ��ͻ�������󣬴�������Ӧ���ݣ�ͨ��playerͬ���ķ�ʽ֪ͨ�ͻ�������
//         �ı仯��
//         װ��������Ҽ�����һ��װ���ϣ��Զ�����װ���ϡ�ж�µ�λ����Ϣ��������������
//         �������������ԭ����������

//         ���׵Ĵ���û��
