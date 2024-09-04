#include "gamecore/KCore.h"
#include "gamecore/coreshell.h"
#include "gamecore/GameDataDef.h"
#include "gamecore/CoreObjGenreDef.h"
#include "engine/KEngine.h"
#include "gamecore/KTongProtocol.h"
#include "engine/Text.h"

extern iCoreShell*	g_pCoreShell;

enum WAIT_OTHER_WND_OPER_PARAM
{
	UIITEM_WAIT_GETNAME,
};

//--------------------------------------------------------------------------
//	���ܣ�������Ϸ�������ݸı�֪ͨ�ĺ���   ---�յ�������֪ͨ����Ӧ����
//--------------------------------------------------------------------------
void CoreDataChangedCallback(unsigned int uDataId, unsigned int uParam, int nParam,int inVal)
{
	//KUiTrade* pTradeBar = NULL;
	if (!g_GameWorld || !nisgetinfo) return;

	switch(uDataId)
	{
	case GDCNI_OPEN_TONG_CREATE_SHEET: //�򿪽���UI
		g_GameWorld->OpenCreatTongSheet();
		break;
	case GDCNI_TASK_INFO:           //����ϵͳ
		{
			if (uParam)
			{ 
				if  (nParam==0)
					g_GameWorld->taskCilentMsgArrival((KNewsMessage*)uParam, nParam,inVal);
					//KUiTask::MessageArrival((KNewsMessage*)uParam, nParam);
				else if (nParam==1)
					g_GameWorld->taskCilentFindPathArrival((KTaskPathInfo*)uParam, nParam,inVal);
					//KUiTask::MsgFindPathArrival((KTaskPathInfo*)uParam, nParam);
			}      
		}
		break;
	case GDCNI_NEWS_MESSAGE:	 //�ɹ���������Ϣ
		if (uParam)
			g_GameWorld->OpenFlyNewsDialog_a((KNewsMessage*)uParam, (SYSTEMTIME*)nParam);
		break;
	case GDCNI_SHUIJI_MESSAGE:	 //�����ɹ���Ϣ��ʾ
		if (uParam)
			g_GameWorld->OpenFlyNewsDialog_b((KNewsMessage*)uParam, (SYSTEMTIME*)nParam);
		break;
	case GDCNI_VIEW_LIAN_LITEM:  //�鿴װ������
		g_GameWorld->OpenItenLinkDialog((KUiLianItem*)uParam);
		break;

	case GDCNI_GAME_QUIT:
		{
			g_GameWorld->_quitGame();
		}
	case GDCNI_GAME_START:
		{
			//g_GameWorld->ReleaseActivateChannelAll();
			g_GameWorld->QueryAllChannel();
			//UIMessageBox2("��ʾ:��¼�ɹ�..!");
		}
		break;
		case GDCNI_PLAYER_BASE_INFO:	//���ǵ�һЩ���ױ������  �յ�����������������
		{
/*			UiNewStatus* pBar = UiNewStatus::GetIfVisible(); //���״̬���򿪵Ļ�
			if (pBar)
				pBar->UpdateBaseData(); //���»�����Ϣ
*/
			KUiPlayerBaseInfo	Info;
			memset(&Info, 0, sizeof(KUiPlayerBaseInfo));
			g_pCoreShell->GetGameData(GDI_PLAYER_BASE_INFO, (intptr_t)&Info, 0);

			/*if (Info.nCurFaction>= 0 ) //����������
			{
				KUiMsgCentrePad::QueryAllChannel();//��ѯȫ��Ƶ����
			}
			else if (Info.nCurTong!=0) //�Ѿ��������
			{
				KUiMsgCentrePad::CloseSelfChannel(KUiMsgCentrePad::ch_Faction); //�ر�����Ƶ��
				KUiMsgCentrePad::QueryAllChannel(); //��ѯȫ��Ƶ����
			}
			else
			{
				KUiMsgCentrePad::QueryAllChannel(); //��ѯȫ��Ƶ����
				//KUiMsgCentrePad::CloseSelfChannel(KUiMsgCentrePad::ch_Tong); //�رհ���Ƶ��
			} */
			   // KUiMsgCentrePad::SetMissGroupIdx(Info.nMissionGroup);

			if ((Info.nCurFaction >= 0) || 
				(Info.nCurTong != 0) || 
				(Info.nMissionGroup >= 0)
				)
				g_GameWorld->QueryAllChannel();
			
			
			if (Info.nCurFaction < 0)
				//KUiMsgCentrePad::CloseSelfChannel(KUiMsgCentrePad::ch_Faction);
			
			if (Info.nCurTong == 0)
				//KUiMsgCentrePad::CloseSelfChannel(KUiMsgCentrePad::ch_Tong);
			
			if (Info.nMissionGroup < 0)
			{
				//KUiMsgCentrePad::CloseSelfChannel(KUiMsgCentrePad::ch_Msgr);
			}
			
			//if (Info.nRoomId < 0)
			//	KUiMsgCentrePad::CloseSelfChannel(KUiMsgCentrePad::ch_Cr);
		}
		break;
	case GDCNI_TRADE_OPER_DATA:		//����״̬�仯
		//pTradeBar = KUiTrade::GetIfVisible();
		//if (pTradeBar)
		g_GameWorld->UpdateTradeOperData();
		break;
	case GDCNI_TRADE_END:			//���׽���
		//pTradeBar = KUiTrade::GetIfVisible();
		//if (pTradeBar)
		g_GameWorld->UpdateTradeEnd();
		break;
	case GDCNI_TRADE_DESIRE_ITEM:	//�Է������뽻�׵���Ʒ
		{
			g_GameWorld->OnTarChangedItem((KUiObjAtRegion*)uParam, nParam);
		}
		break;
	case GDCNI_TRADE_START:
		if (uParam)
		{
			g_GameWorld->OpenPlayerTradeDialog((KUiPlayerItem*)uParam);
		}
		break;
	case GDCNI_GIVE:  //�򿪸�����棿
		{
			PLAYER_GIVEUI* pObject = (PLAYER_GIVEUI*)uParam; //   
			if (pObject->m_szTitle==NULL)
				t_sprintf(pObject->m_szTitle,"%s","����(���ɽ���)����");

			g_GameWorld->openGive(pObject->m_szTitle,pObject->m_szName,nParam,pObject->m_Callback,pObject->m_Error);
		}
		break;	
	case GDCNI_CLOSE_BAITAN:
		{
			if (nParam==0)
			    g_GameWorld->ClosePlayerShop();
		}
		break;
	case GDCNI_VIEW_PLAYERSELLITEM: 
		{//�򿪰�̯���
           g_GameWorld->OpenPlayerShopDialog((KUiPlayerItem*)uParam);
		}
		break;	
	case GDCNI_VIEW_PLAYERUPDATEITEM:
		{//���°�̯�����Ϣ
			if (nParam==0)
		      g_GameWorld->UpPlayerShopData();
		}
		break;
	case GDCNI_VIEW_PLAYERITEM:  //�鿴���װ��
		    g_GameWorld->OpenViewStateDialog((KUiPlayerItem*)uParam);
		break;
	case GDCNI_TEAM:
		{
			//KUiTeamManage* pPad = KUiTeamManage::GetIfVisible();  //�򿪴��ڵ�ʱ��Ÿ���
			g_GameWorld->UpdateDataTeam((KUiPlayerTeam*)uParam); //���¶�����Ϣ
			if (uParam)
				g_GameWorld->QueryAllChannel();
			//else
			//	KUiMsgCentrePad::CloseSelfChannel(KUiMsgCentrePad::ch_Team);*/
		}
		break;
	case GDCNI_OPEN_DATAU_BOX: //��ͷ������
		g_GameWorld->openlaotou();
		break;
	case GDCNI_OPEN_JINDUTIAO:
		{
		    g_GameWorld->openProgressbar((KUiJinDuTiaoInfo*)uParam);	
		}
		break;
	case GDCNI_SKILL_CHANGE:			//��������һ������
		if (uParam)
		{//�п����д��� ����
			g_GameWorld->UpdateSkill((KUiSkillData*)uParam, nParam);
		}
		break;
	case GDCNI_NPC_TRADE:           //�ͻ��˴�NPC�̵�
		{
			if (nParam)
				g_GameWorld->openNpcshop(uParam);
		}
		break;
	case GDCNI_OPEN_STRING_BOX: //���ַ����������
		{//�ű��� �Ի���
			PLAYER_STRINGUI* pStr = (PLAYER_STRINGUI*)uParam;
			if (nParam<=5)
				nParam=512;
			g_GameWorld->openstringBox(pStr->m_szTitle,1,nParam,pStr->m_SCallback,pStr->m_SError,pStr->m_nNum);
			//KUiGetStringInUI::OpenWindow(pStr->m_szTitle,"",UIITEM_WAIT_GETNAME,1,nParam,pStr->m_SCallback,pStr->m_SError,pStr->m_nNum); //���ַ����������
		}
		break;
	case GDCNI_OPEN_STORE_BOX://�򿪴�����
		g_GameWorld->openStorebox();
		break;
	case GDCNI_SYSTEM_MESSAGE: 
		{//����ϵͳ������Ϣ��
			if (uParam)
				g_GameWorld->AMessageArrival((KSystemMessage*)uParam, (void*)nParam);
		}
		break;
	case GDCNI_SPEAK_WORDS:			//npc˵������  talk
		//uParam = (KUiInformationParam*) pWordDataList ָ��KUiInformationParam����
		//nParam = pWordDataList����KUiInformationParamԪ�ص���Ŀ
		if (uParam && nParam)
			g_GameWorld->CreatTalkDialog((KUiInformationParam*)uParam, nParam);
			//g_UiInformation2.SpeakWords((KUiInformationParam*)uParam, nParam);
		break;
	case GDCNI_QUESTION_CHOOSE:
		{
			if (nParam)
				g_GameWorld->CreatNpcDialog((KUiQuestionAndAnswer*)uParam, (KUiNpcSpr*)nParam,inVal);//�Ƿ�ʱ�ر�
			else
				g_GameWorld->CreatNpcDialog((KUiQuestionAndAnswer*)uParam);//�Ƿ�ʱ�ر�
		}
		break; 
	case GDCNI_OBJECT_CHANGED:
		{//����ɾ����Ʒ
			if (!g_GameWorld) break;
			if (uParam)
			{
				KUiObjAtContRegion* pObject = (KUiObjAtContRegion*)uParam;
				switch(pObject->eContainer)
				{
				case UOC_ITEM_TAKE_WITH:
					{
						//if (g_GameWorld->GetuiItemIfVisible())//�򿪵� �͸���
						g_GameWorld->UpdateItem((KUiObjAtRegion*)uParam, nParam);
					}
					break;
				case UOC_ITEM_EX:
					{
						//��ĸ��
						if (g_GameWorld->GetuiItemExIfVisible())//�򿪵� �͸���
							g_GameWorld->UpdateItemEx((KUiObjAtRegion*)uParam, nParam);
					}
					break;
				case UOC_EX_BOX1:           //��չ��1
					{
                       if (g_GameWorld->GetuistoreIfVisible())//�򿪵� �͸���
						   g_GameWorld->UpdateExBox((KUiObjAtRegion*)uParam, nParam,UOC_EX_BOX1);
					} 
					break;
				case UOC_EX_BOX2:
					{
						if (g_GameWorld->GetuistoreIfVisible())//�򿪵� �͸���
							g_GameWorld->UpdateExBox((KUiObjAtRegion*)uParam, nParam,UOC_EX_BOX2);
					}
					break;
				case UOC_EX_BOX3:
					{
						if (g_GameWorld->GetuistoreIfVisible())//�򿪵� �͸���
							g_GameWorld->UpdateExBox((KUiObjAtRegion*)uParam, nParam,UOC_EX_BOX3);
					}
					break;
				case UOC_STORE_BOX:
					{//������
						if (g_GameWorld->GetuistoreIfVisible())//�򿪵� �͸���
							g_GameWorld->UpdateStoreBox((KUiObjAtRegion*)uParam, nParam);
					}
					break;
				case UOC_IMMEDIA_ITEM:
					{
						if (g_GameWorld)
						{
							if (nParam) //����
								g_GameWorld->UpdateImmediaItem(pObject->Region.h,pObject->Obj.uGenre,pObject->Obj.uId);
							else//ɾ��
								g_GameWorld->UpdateImmediaItem(pObject->Region.h,CGOG_NOTHING,0);
						}
					}
					break;
				case UOC_ITEM_GIVE:
					{
						g_GameWorld->UpdateGiveItem((KUiObjAtRegion*)uParam, nParam);
					}
					break;
				case UOC_TO_BE_TRADE:
					{//�Լ�������ͼ��
						//ccMessageBox("��ʼ������Ʒ","TEST");
						g_GameWorld->OnSelfChangedItem(pObject, nParam);
					}
				default:
					break;
				}

				/*if (pObject->eContainer == UOC_ITEM_TAKE_WITH)
				{
					pTradeBar = KUiTrade::GetIfVisible();
					if (pTradeBar)
					{//������
						pTradeBar->OnChangedTakewithItem((KUiObjAtRegion*)uParam, nParam);
					}
					else
					{//����
						//KUiItem* pItemsBar = KUiItem::GetIfVisible();
						if (g_GameWorld->GetuiItemIfVisible())//�����Ǵ򿪵� �͸���
							g_GameWorld->UpdateItem((KUiObjAtRegion*)uParam, nParam);
					}
				}
				if (pObject->eContainer == UOC_ITEM_EX)
				{//��ĸ��
						if (g_GameWorld->GetuiItemExIfVisible())//�����Ǵ򿪵� �͸���
							g_GameWorld->UpdateItemEx((KUiObjAtRegion*)uParam, nParam);
				}  
				if (pObject->eContainer == UOC_STORE_BOX)
				{
					if (g_GameWorld->GetuistoreIfVisible())//�����Ǵ򿪵� �͸���
						g_GameWorld->UpdateStoreBox((KUiObjAtRegion*)uParam, nParam);
				}*/
			}
		}
		break;
	default:
		break;
	}
}