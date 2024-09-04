#include "gamecore/KCore.h"
#include "gamecore/KObjSet.h"
#include "gamecore/KNpcSet.h"
#include "gamecore/KSubWorld.h"
#include "gamecore/KNpc.h"
#include "engine/KIniFile.h"
#include "KDrawGameWorld.h"
#include "gamecore/KMissleSet.h"
#include "gamecore/KPlayerSet.h"
#include "gamecore/KPlayer.h"
#include "gameui/Uiglobaldata.h"
///////////////////////////////////
KDrawGameWorld g_DrawGameWorld;

KDrawGameWorld::KDrawGameWorld()
{
	/*ParentNode_ui =NULL;
	ParentNode_map=NULL;
	ParentNode_npc=NULL;
	ParentNode_player=NULL;
	ParentNode_miss=NULL;
	ParentNode_item=NULL;*/
}

KDrawGameWorld::~KDrawGameWorld()
{
	/*ParentNode_ui =NULL;
	ParentNode_map=NULL;
	ParentNode_npc=NULL;
	ParentNode_player=NULL;
	ParentNode_miss=NULL;
	ParentNode_item=NULL;*/

#ifdef WIN32
	Terminate();
#else
	Terminate(1);
#endif
}

void KDrawGameWorld::ClearUp()
{
#ifdef WIN32
	Terminate();
#else
	Terminate(1);
#endif
}

void KDrawGameWorld::StartUp()
{//����һ���߳�ѭ����Ϸ����
	Start();
}
//�滭�߳�
void KDrawGameWorld::Run()
{
	//ccMessageBox("�滭�߳�����","�滭�߳�����");
	/*unsigned long g_nServiceLoop=0;
	while(true)
	{
		if(g_GameWorld && m_bIsPlayerInGame) 
		{//��� ���ӽڵ�
		  if (ParentNode_npc && ParentNode_npc->getChildrenCount())
		  {
			Ref *temp;
			CCArray *nodeArray_Npc = ParentNode_npc->getChildren();//��ȡ���ڵ��µ� �����ӽڵ������
			CCARRAY_FOREACH(nodeArray_Npc,temp)
			{
				if(temp)
				{
					int nNpcIndex = ((Node*)temp)->getTag();
					//char msg[64]={0};
					//t_sprintf(msg,"NPC:%d",nNpcIndex);
					//ccMessageBox(msg,"KDrawGameWorld");
					//if  (nNpcIndex>0)
						//Npc[nNpcIndex].Paint();
				}
			}
		  }//end ParentNode_npc

		}

#ifdef WIN32
		if (++g_nServiceLoop & 0x80000000)
		{
			g_nServiceLoop = 0;
		}
		if (g_nServiceLoop & 0x1)
			Sleep(10);       //����1����
#else
		usleep(100000); //΢�� ����=1000΢��  sleep Ϊ��
		//sleep(1);
#endif
	}//end while
	*/
}

void  KDrawGameWorld::SetGameWorlNode(Node * ngameNode,int nKind)
{

	switch(nKind)
	{
	case OBJ_NODE_UI:
		ParentNode_ui=ngameNode;
		break;
	case OBJ_NODE_MAP:
		ParentNode_map=ngameNode;
		break;
	case OBJ_NODE_NPC:
		{//NPC �ĸ�����λ����Ϣ
			ParentNode_npc=ngameNode;
		}
		break;
	case OBJ_NODE_MISS:
		{
			ParentNode_miss=ngameNode;
		}
		break;
	case OBJ_NODE_ITEM:
		{
			ParentNode_item=ngameNode;
		}
		break;
	default:
		break;
	}
}

void KDrawGameWorld::DrawPrimitives(int nNpcIndex,int nPos, KRUImage* pPrimitives, unsigned int uGenre, int bpartNo,int nOrDer)
{//ֻ������Ļ滭
	/*char msg[256]={0};//�߳��ڲ������������ʾ
	t_sprintf(msg,"index:%d,nPos:%d,uGenre:%d,partNo:%d\n Path:%s",nNpcIndex,nPos,uGenre,bpartNo,pPrimitives[nPos].szImage);
	//ccMessageBox(msg,"DrawPrimitives");
	DrawInfoLabel->setString(msg);*/
	//return;

	if  (!ParentNode_npc || nNpcIndex<0)
		return;

	if  (pPrimitives[nPos].szImage[0] && nNpcIndex>0 && Npc[nNpcIndex].m_Kind==kind_player)
	{   
		char nSprName[128]={0};
		ZeroMemory(nSprName,sizeof(nSprName));
		t_sprintf(nSprName,pPrimitives[nPos].szImage);
		g_StrLower(nSprName);
		int m_nWidth,m_nHeight,nFrams;
		Texture2D *bgCur = NULL;
		Rect CCRectZero = ax::Rect(0,0,0,0);
		char nSprFilePath[64]={0};
		ZeroMemory(nSprFilePath,sizeof(nSprFilePath));
		DWORD nFielpahtdwid = g_FileName2Id(nSprName);
		t_sprintf(nSprFilePath,"%u-%d",nFielpahtdwid,pPrimitives[nPos].nFrame);

		switch(uGenre)
		{
		case EQ_PART_HEAD:
			{//ͷ��
				switch(bpartNo)
				{
				case 0:
					{//ͷ��
                        auto textureCache = ax::Director::getInstance()->getTextureCache();
						if ((bgCur = textureCache->getTextureForKey(nSprFilePath))==NULL)
							bgCur = _getinidata.getinidata_one(nSprName,pPrimitives[nPos].nFrame,&m_nWidth,&m_nHeight,&nFrams);
						if (bgCur)
						{//�޸ľ��������
							Sprite * nPartSpr = (Sprite *)(ParentNode_npc->getChildByTag(nNpcIndex)->getChildByTag(UI_EQ_PART_HEAD));
							//bgCur->getContentSize();
							nPartSpr->setTexture(bgCur);
							CCRectZero.size = bgCur->getContentSize();
							nPartSpr->setTextureRect(CCRectZero,false,CCRectZero.size);
							//nPartSpr->initWithTexture(bgCur);
							nPartSpr->setLocalZOrder(nOrDer);
							nPartSpr->setPosition(ax::Vec2(1,33));
						}
					}
					break;
				default:
					break;
				}
			}
			break;
		case EQ_PART_BODY:
			{//Ĭ��λ���ڽڵ���м�
				switch(bpartNo)
				{
				case 5:
					{//����
						if ((bgCur = ax::Director::getInstance()->getTextureCache()->getTextureForKey(nSprFilePath))==NULL)
						     bgCur = _getinidata.getinidata_one(nSprName,pPrimitives[nPos].nFrame,&m_nWidth,&m_nHeight,&nFrams);
						
						if (bgCur)
						{//�޸ľ��������
							Sprite * nQutiSpr = (Sprite *)(ParentNode_npc->getChildByTag(nNpcIndex)->getChildByTag(UI_EQ_PART_BOBY));
							nQutiSpr->setTexture(bgCur);
							CCRectZero.size = bgCur->getContentSize();
							nQutiSpr->setTextureRect(CCRectZero,false,CCRectZero.size);
							//nPartSpr->initWithTexture(bgCur);
							nQutiSpr->setLocalZOrder(nOrDer);
							//nPartSpr->setScaleX(0.5);
							//nPartSpr->setScaleY(0.5);
						}
					}
					break;
				case 6:
					{//����
						//ccMessageBox(nSprName,"DrawPrimitives5");
						if ((bgCur = ax::Director::getInstance()->getTextureCache()->getTextureForKey(nSprFilePath))==NULL)
							bgCur = _getinidata.getinidata_one(nSprName,pPrimitives[nPos].nFrame,&m_nWidth,&m_nHeight,&nFrams);
						if (bgCur)
						{//�޸ľ��������
							Sprite * nPartSpr = (Sprite *)(ParentNode_npc->getChildByTag(nNpcIndex)->getChildByTag(UI_EQ_PART_LEFTHD));
							//bgCur->getContentSize();
							nPartSpr->setTexture(bgCur);
							CCRectZero.size = bgCur->getContentSize();
							nPartSpr->setTextureRect(CCRectZero,false,CCRectZero.size);
							nPartSpr->setLocalZOrder(nOrDer);
							//nPartSpr->setPosition(ax::Vec2(1,33));
						}

						
					}
					break;
				case 7:
					{//����
						//ccMessageBox(nSprName,"DrawPrimitives6");
						if ((bgCur = ax::Director::getInstance()->getTextureCache()->getTextureForKey(nSprFilePath))==NULL)
							bgCur = _getinidata.getinidata_one(nSprName,pPrimitives[nPos].nFrame,&m_nWidth,&m_nHeight,&nFrams);
						if (bgCur)
						{//�޸ľ��������
							Sprite * nPartSpr = (Sprite *)(ParentNode_npc->getChildByTag(nNpcIndex)->getChildByTag(UI_EQ_PART_RIGTHHD));
							//bgCur->getContentSize();
							nPartSpr->setTexture(bgCur);
							CCRectZero.size = bgCur->getContentSize();
							nPartSpr->setTextureRect(CCRectZero,false,CCRectZero.size);
							nPartSpr->setLocalZOrder(nOrDer);
							//nPartSpr->setPosition(ax::Vec2(1,33));
						}
					}
					break;
				default:
					break;
				}
			}
			break;
		case EQ_PART_HAND_W: //��������
			{//Ĭ��λ���ڽڵ���м�
				switch(bpartNo)
				{
				case 8:
					{//��������
						if ((bgCur = ax::Director::getInstance()->getTextureCache()->getTextureForKey(nSprFilePath))==NULL)
							bgCur = _getinidata.getinidata_one(nSprName,pPrimitives[nPos].nFrame,&m_nWidth,&m_nHeight,&nFrams);
						if (bgCur)
						{//�޸ľ��������
							Sprite * nPartSpr = (Sprite *)(ParentNode_npc->getChildByTag(nNpcIndex)->getChildByTag(UI_EQ_PART_LEFTHD_W));
							//bgCur->getContentSize();
							nPartSpr->setTexture(bgCur);
							CCRectZero.size = bgCur->getContentSize();
							nPartSpr->setTextureRect(CCRectZero,false,CCRectZero.size);
							nPartSpr->setLocalZOrder(nOrDer);
							//nPartSpr->setPosition(ax::Vec2(1,33));
						}
					}
					break;
				case 9:
					{//��������
						if ((bgCur = ax::Director::getInstance()->getTextureCache()->getTextureForKey(nSprFilePath))==NULL)
							bgCur = _getinidata.getinidata_one(nSprName,pPrimitives[nPos].nFrame,&m_nWidth,&m_nHeight,&nFrams);
						if (bgCur)
						{//�޸ľ��������
							Sprite * nPartSpr = (Sprite *)(ParentNode_npc->getChildByTag(nNpcIndex)->getChildByTag(UI_EQ_PART_RIGTHHD_W));
							//bgCur->getContentSize();
							nPartSpr->setTexture(bgCur);
							CCRectZero.size = bgCur->getContentSize();
							nPartSpr->setTextureRect(CCRectZero,false,CCRectZero.size);
							nPartSpr->setLocalZOrder(nOrDer);
							//nPartSpr->setPosition(ax::Vec2(1,33));
						}
					}
					break;
				default:
					break;
				}
			}
			break;
		case EQ_PART_HORSE:
			{
				switch(bpartNo)
				{
				case 10:
					{
						if ((bgCur = ax::Director::getInstance()->getTextureCache()->getTextureForKey(nSprFilePath))==NULL)
							bgCur = _getinidata.getinidata_one(nSprName,pPrimitives[nPos].nFrame,&m_nWidth,&m_nHeight,&nFrams);
						if (bgCur)
						{//�޸ľ��������
							Sprite * nPartSpr = (Sprite *)(ParentNode_npc->getChildByTag(nNpcIndex)->getChildByTag(UI_EQ_PART_HORSE_Q));
							//bgCur->getContentSize();
							if (Npc[nNpcIndex].m_HorseType<0)
								nPartSpr->setVisible(false);//���ò��ɼ�
							else
								nPartSpr->setVisible(true); //���ÿɼ�

							nPartSpr->setTexture(bgCur);
							CCRectZero.size = bgCur->getContentSize();
							nPartSpr->setTextureRect(CCRectZero,false,CCRectZero.size);
							nPartSpr->setLocalZOrder(nOrDer);

						}
					}
					break;
				case 11:
					{
						if ((bgCur = ax::Director::getInstance()->getTextureCache()->getTextureForKey(nSprFilePath))==NULL)
							bgCur = _getinidata.getinidata_one(nSprName,pPrimitives[nPos].nFrame,&m_nWidth,&m_nHeight,&nFrams);
						if (bgCur)
						{//�޸ľ��������
							Sprite * nPartSpr = (Sprite *)(ParentNode_npc->getChildByTag(nNpcIndex)->getChildByTag(UI_EQ_PART_HORSE_Z));
							//bgCur->getContentSize();
							if (Npc[nNpcIndex].m_HorseType<0)
								nPartSpr->setVisible(false);//���ò��ɼ�
							else
								nPartSpr->setVisible(true); //���ÿɼ�

							nPartSpr->setTexture(bgCur);
							CCRectZero.size = bgCur->getContentSize();
							nPartSpr->setTextureRect(CCRectZero,false,CCRectZero.size);
							nPartSpr->setLocalZOrder(nOrDer);
						}
					}
					break;
				case 12:
					{
						if ((bgCur = ax::Director::getInstance()->getTextureCache()->getTextureForKey(nSprFilePath))==NULL)
							bgCur = _getinidata.getinidata_one(nSprName,pPrimitives[nPos].nFrame,&m_nWidth,&m_nHeight,&nFrams);
						if (bgCur)
						{//�޸ľ��������
							Sprite * nPartSpr = (Sprite *)(ParentNode_npc->getChildByTag(nNpcIndex)->getChildByTag(UI_EQ_PART_HORSE_H));
							//bgCur->getContentSize();
							if (Npc[nNpcIndex].m_HorseType<0)
								nPartSpr->setVisible(false);//���ò��ɼ�
							else
								nPartSpr->setVisible(true); //���ÿɼ�

							nPartSpr->setTexture(bgCur);
							
							CCRectZero.size = bgCur->getContentSize();
							nPartSpr->setTextureRect(CCRectZero,false,CCRectZero.size);
							nPartSpr->setLocalZOrder(nOrDer);
							
						}
					}
					break;
				default:
					break;
				}
			}
			break;
		case EQ_PART_PIFENG:
			{//����
				switch(bpartNo)
				{
				case 16:
					{
						if ((bgCur = ax::Director::getInstance()->getTextureCache()->getTextureForKey(nSprFilePath))==NULL)
							bgCur = _getinidata.getinidata_one(nSprName,pPrimitives[nPos].nFrame,&m_nWidth,&m_nHeight,&nFrams);
						if (bgCur)
						{//�޸ľ��������
							Sprite * nPartSpr = (Sprite *)(ParentNode_npc->getChildByTag(nNpcIndex)->getChildByTag(UI_EQ_PART_PIFENG));
							//bgCur->getContentSize();
							nPartSpr->setTexture(bgCur);
							CCRectZero.size = bgCur->getContentSize();
							nPartSpr->setTextureRect(CCRectZero,false,CCRectZero.size);
							nPartSpr->setLocalZOrder(nOrDer);
						}
					}
					break;
				default:
					break;
				}
			}
			break;
		default:
			break;
		}
	}
	//��ʼͳһ����λ��
}
/*#ifndef _SERVER
void KDrawGameWorld::Paint()
{
	SubWorld[0].Paint();
}
#endif*/
