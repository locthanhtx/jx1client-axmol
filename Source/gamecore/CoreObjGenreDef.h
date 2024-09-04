/*****************************************************************************************
//	Core��Ҫ�������õ���Ʒ����������
//	Copyright : Kingsoft 2002
//	Author	:   Wooy(Wu yue)
//	CreateTime:	2002-9-17
------------------------------------------------------------------------------------------
*****************************************************************************************/

#ifndef COREOBJGENREDEF_H
#define COREOBJGENREDEF_H

//���ͼ�����õ� ��ʮ��λΪ�ַ�����
#define	CGOG_NOTHING			 0	//ɶ������
#define	CGOG_MONEY				 1	//Ǯ������
#define	CGOG_PLAYER_FACE		 2	//���ͷ��
#define	CGOG_OBJECT				 3	//������Ʒ
#define	CGOG_SKILL				 4	//����
#define	CGOG_ITEM				 5	//��Ʒ
#define	CGOG_NPC				 6	//NPC
#define	CGOG_MENU_NPC			 7	//�����ϵ�NPC
#define	CGOG_NPC_BLUR			 8	//�����Ӱ
#define	CGOG_MISSLE				 9	//�ӵ���Դ
#define	CGOG_EQUIP				 10 // û����
#define	CGOG_NPCSELLITEM		 11	// Npc�����Ķ���
#define CGOG_PLAYERSELLITEM		 12 // ��̯�����Ķ���
#define CGOG_IME_ITEM			 13  //�����Ʒ��
#define CGOG_SKILL_SHIQU		 14  //
#define CGOG_COM_ITEM		     15  //���� ͼ����ʾ 
#define CGOG_ITEM_SET            16  //
#define CGOG_CHESS_ITEM          17  //����
#define	CGOG_NPCSELLITEM_PAI	 18	// �����еĶ���
//��16λΪ����ϸ����ֵ���еĶ���û��ϸ������
//����ϸ�ֵ�ȡֵ�涨��Ϊ��0
#define	CGOG_MAKE_DETAIL(_base, _detail)		(((_detail) << 16) + (_base))
//--װ��ϸ��--
#define	CGOG_EQUIP_CAP			CGOG_MAKE_DETAIL(CGOG_EQUIP, 1)	//װ��-ñ��
#define	CGOG_EQUIP_WEAPON		CGOG_MAKE_DETAIL(CGOG_EQUIP, 2)	//װ��-����
#define	CGOG_EQUIP_NECKLACE		CGOG_MAKE_DETAIL(CGOG_EQUIP, 3)	//װ��-����
#define	CGOG_EQUIP_BANGLE		CGOG_MAKE_DETAIL(CGOG_EQUIP, 4)	//װ��-����
#define	CGOG_EQUIP_CLOTHING		CGOG_MAKE_DETAIL(CGOG_EQUIP, 5)	//װ��-�·�
#define	CGOG_EQUIP_CAESTUS		CGOG_MAKE_DETAIL(CGOG_EQUIP, 6)	//װ��-����
#define	CGOG_EQUIP_RING			CGOG_MAKE_DETAIL(CGOG_EQUIP, 7)	//װ��-��ָ
#define	CGOG_EQUIP_WAISTDECOR	CGOG_MAKE_DETAIL(CGOG_EQUIP, 8)	//װ��-��׺
#define	CGOG_EQUIP_SHOE			CGOG_MAKE_DETAIL(CGOG_EQUIP, 9)	//װ��-Ь��
#define	CGOG_EQUIP_HORSE		CGOG_MAKE_DETAIL(CGOG_EQUIP, 10)	//װ��-��
#define	CGOG_EQUIP_MASK			CGOG_MAKE_DETAIL(CGOG_EQUIP, 11)	//װ��-���
#define	CGOG_EQUIP_PIFENG		CGOG_MAKE_DETAIL(CGOG_EQUIP, 12)	//װ��-����
#define	CGOG_EQUIP_YINJIAN		CGOG_MAKE_DETAIL(CGOG_EQUIP, 13)	//װ��-ӡ��
#define	CGOG_EQUIP_SHIPIN		CGOG_MAKE_DETAIL(CGOG_EQUIP, 14)	//װ��-��Ʒ

//--����ϸ��--
#define	CGOG_SKILL_LIVE			CGOG_MAKE_DETAIL(CGOG_SKILL, 1)	//�����
#define	CGOG_SKILL_FIGHT		CGOG_MAKE_DETAIL(CGOG_SKILL, 2)	//ս������
#define	CGOG_SKILL_CUSTOM		CGOG_MAKE_DETAIL(CGOG_SKILL, 3)	//�Դ��书
#define CGOG_SKILL_SHORTCUT		CGOG_MAKE_DETAIL(CGOG_SKILL, 4)	//����书
//--��Ʒϸ��--
#define	CGOG_ITEM_CORDIAL		CGOG_MAKE_DETAIL(CGOG_ITEM, 1)	//��Ʒ-�̲�ҩƷ
//#define	CGOG_ITEM_OTHER			CGOG_MAKE_DETAIL(CGOG_ITEM, 2)	//��Ʒ-����
//--�����Ӱϸ��--
#define	CGOG_NPC_BLUR_DETAIL(x)	CGOG_MAKE_DETAIL(CGOG_NPC_BLUR, x)
#endif
