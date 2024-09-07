#include "KCore.h"
#include "KNpc.h"
#include "KMissle.h"
#include "KItem.h"
#include "KBuySell.h"
#include "KPlayer.h"
//#include "../../Represent/iRepresent/iRepresentshell.h"
#include "KSubWorldSet.h"
#include "gamescene/KScenePlaceC.h"
#include "ImgRef.h"
#include "GameDataDef.h"
#include "KObjSet.h"
//=========================
#include "engine/Text.h"
#include "CoreShell.h"
#include "CoreDrawGameObj.h"
//=========================
#define  PHYSICSSKILLICON "\\spr\\Ui\\����ͼ��\\icon_sk_ty_ap.spr"
#define  ADDSKILLPIONT    "\\spr\\Ui4\\common\\�ӵ㰴ť.spr"
#define  SHOW_SPACE_HEIGHT 5
//=========================

int CoreDrawGameObj(unsigned int uObjGenre, unsigned int uId, int x, int y, int Width, int Height, int nParam,int inParam)
{
	return true;

	switch(uObjGenre)
	{
	case CGOG_NPC:
		if (uId > 0)
		{
			int nHeight = Npc[uId].GetNpcPate();

			if  (!Npc[uId].IsPlayer() && Npc[uId].m_Kind!=kind_dialoger)
				Npc[uId].PaintBlood(nHeight/2);	  //����ðѪ

			if ((nParam & IPOT_RL_INFRONTOF_ALL) == IPOT_RL_INFRONTOF_ALL)
			{
				int nnHeight = nHeight;
				    nHeight = Npc[uId].PaintChat(nHeight);	//ͷ��������Ϣ��������

			    if (nHeight == nnHeight)	//û��������Ϣʱ����������Ϣ
				{
					    //if (nnHeight != nHeight)	//��������ʾʱ
						//{
						//	if(Npc[uId].m_CurrentCamp != camp_event)   //�Ի��߲���ʾ������
						//		nHeight = Npc[uId].PaintMana(nHeight+3);
						//nHeight = nnHeight+3;//�ÿ�
						//}
					if (NpcSet.CheckShowLife())
					{
						if (Npc[uId].m_Hide.nTime <= 0 || Player[CLIENT_PLAYER_INDEX].m_nIndex == uId)
						{
							if(Npc[uId].m_CurrentCamp != camp_event)                     //·�˶Ի�NPC����ʾѪ��
								nHeight = Npc[uId].PaintLife(nnHeight, true);
						}
					}

					if (NpcSet.CheckShowName())
					{
						if (Npc[uId].m_Hide.nTime <= 0 || Player[CLIENT_PLAYER_INDEX].m_nIndex == uId)
						{//����
						    if (Player[CLIENT_PLAYER_INDEX].GetTargetNpc() && Player[CLIENT_PLAYER_INDEX].GetTargetNpc() == uId)
							{
							    nHeight = Npc[uId].PaintInfo(nHeight,false,14,0XFF000000);	//��ѡ�е������Ŵ���ʾ
							}
					     	else
							{
							    nHeight = Npc[uId].PaintInfo(nHeight, true);
						        nHeight += 0;
							}
						}
					}
				}
			}
			else if ((nParam & IPOT_RL_OBJECT) == IPOT_RL_OBJECT)
			{
				if (Npc[uId].m_Hide.nTime <= 0 ||  Player[CLIENT_PLAYER_INDEX].m_nIndex == uId)
				{
					 Npc[uId].Paint();  //�滭 NPC ���

				     if ((int)uId == Player[CLIENT_PLAYER_INDEX].GetTargetNpc())
					 {
					     Npc[uId].DrawBorder();//�滭 NPC ��ɫ�߿����	 3D����
					 }
				}
			}
		}
		break;
	case CGOG_MISSLE:
	if (uId > 0 && uId < MAX_MISSLE)
	{
	    if (!Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_bIsHideMissle)
		{//�滭 õ�廨 ����(�Ѿ��޸�)
        	Missle[uId].Paint();  //�����ӵ�ͼ
		}
	}
	break;

	case CGOG_IME_ITEM:        //�������Ʒͼ��--����--��������
		if (uId == 0)
			break;
		if (Width == 0 && Height == 0)
		{
        #define	ITEM_CELL_WIDTH		26
        #define	ITEM_CELL_HEIGHT	26
			Width = Item[uId].GetWidth() * ITEM_CELL_WIDTH;
			Height = Item[uId].GetHeight() * ITEM_CELL_HEIGHT;
			x -= Width / 2;
			y -= Height / 2;
		}
		else
		{
			x += (Width - Item[uId].GetWidth() * ITEM_CELL_WIDTH) / 2;
			y += (Height - Item[uId].GetHeight() * ITEM_CELL_HEIGHT) / 2;
		}

		if (Item[uId].GetGenre()==1)  //ҩƷ  ����ʾ�������������
		{
			int szStackNum=0;
			    Item[uId].GetDetailType();
			    Item[uId].GetParticular();
                szStackNum = Player[CLIENT_PLAYER_INDEX].m_ItemList.GetSameEquipmentItemNum(uId);
			    Item[uId].Paint(x, y,TRUE,szStackNum);
		}
        else
		   Item[uId].Paint(x, y);  //���ɵ�������,FALSE
/*
#ifndef _SERVER
		//-------------------------------------------------------------------------------
		//	���ܣ���ȡ������ͬ����Ʒ�� ��������
		//-------------------------------------------------------------------------------
		int		KItemList::GetSameEquipmentItemNum(int nIdx)
		{
			//if (nImmediatePos < 0 || nImmediatePos >= EQUIPMENT_ROOM_WIDTH * EQUIPMENT_ROOM_HEIGHT)
			//	return 0;
			//int		nIdx = m_Room[room_equipment].FindItem(nImmediatePos, 0);
			if (nIdx <= 0)
				return 0;
			return m_Room[room_equipment].CalcSameDetailType(Item[nIdx].GetGenre(), Item[nIdx].GetDetailType()) + Item[nIdx].GetStackNum();
		}
#endif
*/
		break;
  case CGOG_COM_ITEM:   //��������Ʒ
		if (uId == 0)
			break;
		if (Width == 0 && Height == 0)
		{
#define	ITEM_CELL_WIDTH		26
#define	ITEM_CELL_HEIGHT	26
			Width = Item[uId].GetWidth() * ITEM_CELL_WIDTH;
			Height = Item[uId].GetHeight() * ITEM_CELL_HEIGHT;
			x -= Width / 2;
			y -= Height / 2;
		}
		else
		{
			x += (Width - Item[uId].GetWidth() * ITEM_CELL_WIDTH) / 2;
			y += (Height - Item[uId].GetHeight() * ITEM_CELL_HEIGHT) / 2;
		}
		Item[uId].Paint(x, y);  //�����ɵ�������,FALSE
		break;
	case CGOG_ITEM_SET:     //��Ʒͼ����ʾ
		if (uId == 0)
			break;
		Item[uId].Paint(x, y,nParam);    //���ɵ�������
		break;
	case CGOG_CHESS_ITEM:   //����ͼ����ʾ
		if (uId == 0)
			break;
		//	Item[uId].Paint(x, y,nParam);    //���ɵ�������
		break;
	case CGOG_PLAYERSELLITEM:  //�滭��̯��Ʒͼ��
	case CGOG_ITEM:            //��Ʒͼ����ʾ
		if (uId == 0)
			break;

		if (Width == 0 && Height == 0)
		{
        #define	ITEM_CELL_WIDTH		26
        #define	ITEM_CELL_HEIGHT	26
			Width = Item[uId].GetWidth() * ITEM_CELL_WIDTH;
			Height = Item[uId].GetHeight() * ITEM_CELL_HEIGHT;
			x -= Width / 2;
			y -= Height / 2;
		}
		else
		{
			x += (Width - Item[uId].GetWidth() * ITEM_CELL_WIDTH) / 2;
			y += (Height - Item[uId].GetHeight() * ITEM_CELL_HEIGHT) / 2;
		}
		Item[uId].Paint(x, y);    //���ɵ�������
		break;
	case CGOG_MENU_NPC:  //�˵� �滭 NPC���
		if (nParam)
		{
			((KNpcRes *)nParam)->SetPos(0, x + Width / 2, y + Height / 2 + 28, 0, FALSE, TRUE);  //�趨λ��
             #define	STAND_TOTAL_FRAME	15
			int nFrame = g_SubWorldSet.m_nLoopRate % STAND_TOTAL_FRAME;   //��������ǰ��ʱ��
			((KNpcRes *)nParam)->Draw(0, 0, STAND_TOTAL_FRAME, nFrame, true);  // 2D �滭���
		}
		break;

	case CGOG_NPC_BLUR_DETAIL(1):
		//Npc[uId].GetNpcRes()->m_cNpcBlur.Draw(1); // �滭��Ӱ
		break;
	case CGOG_NPC_BLUR_DETAIL(2):
		//Npc[uId].GetNpcRes()->m_cNpcBlur.Draw(2);
		break;
	case CGOG_NPC_BLUR_DETAIL(3):
		//Npc[uId].GetNpcRes()->m_cNpcBlur.Draw(3);
		break;
	case CGOG_NPC_BLUR_DETAIL(4):
		//Npc[uId].GetNpcRes()->m_cNpcBlur.Draw(4);
		break;
	case CGOG_NPC_BLUR_DETAIL(5):
		//Npc[uId].GetNpcRes()->m_cNpcBlur.Draw(5);
		break;
	case CGOG_NPC_BLUR_DETAIL(6):
		//Npc[uId].GetNpcRes()->m_cNpcBlur.Draw(6);
		break;
	case CGOG_NPC_BLUR_DETAIL(7):
		//Npc[uId].GetNpcRes()->m_cNpcBlur.Draw(7);
		break;
	case CGOG_SKILL_SHIQU:
	case CGOG_SKILL:
	case CGOG_SKILL_FIGHT:
		{
			int nSkillId = (int)uId;
			if (nSkillId > 0)
			{
				ISkill *pSkill = g_SkillManager.GetSkill(nSkillId,1);
				if (pSkill)
				{
					pSkill->DrawSkillIcon(x, y, Width, Height,inParam);  //���Ƽ���ͼ��
				}
			}
			else
			{
				if(uId == -1)
				{
					KRUImage RUIconImage;
					RUIconImage.nType = ISI_T_SPR;
					RUIconImage.Color.Color_b.a = 255;
					RUIconImage.bRenderStyle = IMAGE_RENDER_STYLE_ALPHA;
					RUIconImage.uImage = 0;
					RUIconImage.nISPosition = IMAGE_IS_POSITION_INIT;
					RUIconImage.bRenderFlag = 0;
					strcpy(RUIconImage.szImage, PHYSICSSKILLICON); //ͨ��ͼ��
					RUIconImage.oPosition.nX = x;
					RUIconImage.oPosition.nY = y;
					RUIconImage.oPosition.nZ = 0;
					RUIconImage.nFrame = 0;
					//g_pRepresent->DrawPrimitives(1, &RUIconImage, RU_T_IMAGE, 1);
				}
			}
		}
		break;
	case CGOG_SKILL_LIVE:
	case CGOG_SKILL_SHORTCUT:  // ��ݼ������
		{
		int nSkillId = (int)uId;
		if (nSkillId > 0)
		{
			ISkill *pSkill = g_SkillManager.GetSkill(nSkillId,1);
			if (pSkill)
            {
				pSkill->DrawSkillIcon(x, y, Width, Height);  //���Ƽ���ͼ��
            }
		}
		else
		{
			if(uId == -1)
			{
				KRUImage RUIconImage;
				RUIconImage.nType = ISI_T_SPR;
				RUIconImage.Color.Color_b.a = 255;
				RUIconImage.bRenderStyle = IMAGE_RENDER_STYLE_ALPHA;
				RUIconImage.uImage = 0;
				RUIconImage.nISPosition = IMAGE_IS_POSITION_INIT;
				RUIconImage.bRenderFlag = 0;
				strcpy(RUIconImage.szImage, PHYSICSSKILLICON); //ͨ��ͼ��
				RUIconImage.oPosition.nX = x;
				RUIconImage.oPosition.nY = y;
				RUIconImage.oPosition.nZ = 0;
				RUIconImage.nFrame = 0;
				//g_pRepresent->DrawPrimitives(1, &RUIconImage, RU_T_IMAGE, 1);
			}
		}
		}
		break;
	case CGOG_OBJECT:   //������Ʒ
		if (uId)
		{
			if ((nParam & IPOT_RL_INFRONTOF_ALL) == IPOT_RL_INFRONTOF_ALL)
			//if ((nParam & IPOT_RL_COVER_GROUND) == IPOT_RL_COVER_GROUND)
			//if ((nParam & IPOT_RL_OBJECT) == IPOT_RL_OBJECT)
			{
//				if (ObjSet.CheckShowName())
					KObject[uId].DrawInfo();	  //�滭����
				//	KObject[uId].DrawBorder();
			}
			else
		    //if ((nParam & IPOT_RL_COVER_GROUND) == IPOT_RL_COVER_GROUND)
			//if ((nParam & IPOT_RL_INFRONTOF_ALL) == IPOT_RL_INFRONTOF_ALL)
			{
				    KObject[uId].Draw();
				if ((int)uId == Player[CLIENT_PLAYER_INDEX].GetTargetObj())
					KObject[uId].DrawBorder();
			}

/*//////////////////////////////��ʾ�ɵ�����////////////////////////////
if (Width == 0 && Height == 0)
{
#define	ITEM_CELL_WIDTH		26
#define	ITEM_CELL_HEIGHT	26
Width = Item[uId].GetWidth() * ITEM_CELL_WIDTH;
Height = Item[uId].GetHeight() * ITEM_CELL_HEIGHT;
x -= Width / 2;
y -= Height / 2;
}
else
{
x += (Width - Item[uId].GetWidth() * ITEM_CELL_WIDTH) / 2;
y += (Height - Item[uId].GetHeight() * ITEM_CELL_HEIGHT) / 2;
}

  Item[uId].Paint(x, y);

//////////////////////////////////////////////////////////////////////*/
		}
		break;
	case CGOG_NPCSELLITEM://�滭�̳ǡ��̵�Ķ���
		if (uId < 0)
			break;
		BuySell.PaintItem(uId, x, y);
		break;
	case CGOG_NPCSELLITEM_PAI://�滭�����еĶ���
		if (uId < 0)
			break;
		BuySell.PaintItemPai(uId, x, y,CGOG_NPCSELLITEM_PAI);
		break;
	default:
		break;
	}

	return TRUE;
}

void	CoreGetGameObjLightInfo(unsigned int uObjGenre, unsigned int uId, KLightInfo *pLightInfo)
{
	switch (uObjGenre)
	{
	case CGOG_NPC:   //NPC��ɫ����
		if (uId > 0 && uId < MAX_NPC)
		{
			int uMap=0;
			Npc[uId].GetMpsPos(&pLightInfo->oPosition.nX, &pLightInfo->oPosition.nY,&uMap);
			pLightInfo->oPosition.nZ = Npc[uId].m_Height;
			pLightInfo->dwColor = 0;
			if (Npc[uId].m_RedLum > 255)
			{
				Npc[uId].m_RedLum = 255;
			}
			if (Npc[uId].m_GreenLum > 255)
			{
				Npc[uId].m_GreenLum = 255;
			}
			if (Npc[uId].m_BlueLum > 255)
			{
				Npc[uId].m_BlueLum = 255;
			}
			pLightInfo->dwColor = 0xff000000 | Npc[uId].m_RedLum << 16 | Npc[uId].m_GreenLum << 8 | Npc[uId].m_BlueLum;
			pLightInfo->nRadius = Npc[uId].m_CurrentVisionRadius;
		}
		break;
	case CGOG_OBJECT:
		break;
	case CGOG_MISSLE:
		if (uId > 0 && uId < MAX_MISSLE)
		{
			if (Missle[uId].m_nMissleId > 0)
			{
				Missle[uId].GetLightInfo(pLightInfo);
			}
		}
		break;
	default:
		break;
	}
}
