
#ifndef COREUSENAMEDEF_H
#define COREUSENAMEDEF_H

#define		MAX_PLAYER_IN_ACCOUNT			3   //��ɫ����

//#define		SETTING_PATH					"\\settings"

#define	PLAYER_MENU_STATE_RES_FILE		"\\settings\\npcres\\����״̬��ͼ�ζ��ձ�.txt"//Ui_Stateandimgtable.txt"//����״̬��ͼ�ζ��ձ�.txt"
#define	NPC_RES_KIND_FILE_NAME			"\\settings\\npcres\\��������.txt"//PlayerType.txt" //��������.txt"

//---------------------------- npc res ��� ------------------------------
#ifndef _SERVER

#define	RES_INI_FILE_PATH				"\\settings\\npcres"

#define	RES_SOUND_FILE_PATH				"sound"

#define	NPC_NORMAL_RES_FILE				"\\settings\\npcres\\��ͨnpc��Դ.txt"//Nor_Npcres.txt"//��ͨnpc��Դ.txt"
#define		NPC_NORMAL_SPRINFO_FILE		"\\settings\\npcres\\��ͨnpc��Դ��Ϣ.txt"//Nor_Npcres_info.txt" //��ͨnpc��Դ��Ϣ
#define		STATE_MAGIC_TABLE_NAME		"\\settings\\npcres\\״̬ͼ�ζ��ձ�.txt"//StateImgtable.txt"//״̬ͼ�ζ��ձ�.txt"
#define		PLAYER_RES_SHADOW_FILE		"\\settings\\npcres\\���Ƕ�����Ӱ��Ӧ��.txt"//roleactshadowtable.txt"//���Ƕ�����Ӱ��Ӧ��.txt"

#define		PLAYER_SOUND_FILE			"\\settings\\npcres\\���Ƕ���������.txt"//roleactvoicetable.txt"//���Ƕ���������.txt"
#define		NPC_SOUND_FILE				"\\settings\\npcres\\npc����������.txt"//npcactvoicetable.txt"//npc����������.txt"

#define		NPC_ACTION_NAME				"npc������.txt"//"npcacttable.txt"//"npc������.txt"
#define		ACTION_FILE_NAME			"������ű�.txt"//"actnumbertable.txt"//"������ű�.txt"

#define		PLAYER_INSTANT_SPECIAL_FILE		"\\settings\\npcres\\˲����Ч.txt"//one_effects.txt"//˲����Ч.txt"

#define		SPR_INFO_NAME					"��Ϣ"//"info"        //��Ϣ
#define		KIND_NAME_SECT					"��������"//"roletype"    //"��������"
#define		KIND_NAME_SPECIAL				"SpecialNpc"  //����npc
#define		KIND_NAME_NORMAL				"NormalNpc"   //��ͨnpc
#define		KIND_FILE_SECT1					"����˵���ļ���"//"bujianfilename"//"����˵���ļ���"
#define		KIND_FILE_SECT2					"������Ϊ������1"//"wuqiactconnettable1"//"������Ϊ������1"
#define		KIND_FILE_SECT3					"������Ϊ������2"//"wuqiactconnettable2"//"������Ϊ������2"
#define		KIND_FILE_SECT4					"������ͼ˳���"//"actimgordertable"//"������ͼ˳���"
#define		KIND_FILE_SECT5					"��Դ�ļ�·��"//"resfilepath"//"��Դ�ļ�·��"
//---------------------------- С��ͼ��� -------------------------------
#define		defLITTLE_MAP_SET_FILE	       "\\Ui\\Default\\С��ͼ��ɫ.ini"//minimapcolor.ini" //"С��ͼ��ɫ"
//----------------------------- ������� ------------------------------
#define		CHAT_PATH						"\\chat"
#define		CHAT_TEAM_INFO_FILE_NAME		"Team.cht"
#define		CHAT_CHANNEL_INFO_FILE_NAME		"Channel.cht"
//----------------------------- ������� --------------------------------
#define		defINSTANT_SOUND_FILE	"\\settings\\SoundList.txt"
#define		defMUSIC_SET_FILE		"\\settings\\music\\MusicSet.txt"
#define		defMUSIC_FIGHT_SET_FILE	"\\settings\\music\\MusicFightSet.ini"

#define		OBJ_NAME_COLOR_FILE		"\\settings\\obj\\ObjNameColor.ini"  //��Ʒ��ɫ����

#define STRINGRESOURSE_TABFILE			"\\settings\\StringResource.txt"
#define PLAYER_RANK_SETTING_TABFILE		"\\settings\\RankSetting.txt"

#define		CHAT_CHANNEL_NAME_ALL			"�������"
#define		CHAT_CHANNEL_NAME_SCREEN		"�������"
#define		CHAT_CHANNEL_NAME_SINGLE		"����"
#define		CHAT_CHANNEL_NAME_TEAM			"����"
#define		CHAT_CHANNEL_NAME_FACTION		"ͬ��"
#define		CHAT_CHANNEL_NAME_TONG			"����"
#define		CHAT_CHANNEL_NAME_SCREENSINGLE	"İ����"
#define		CHAT_CHANNEL_NAME_SYSTEM		"ϵͳ"

#define		CHAT_TAKE_CHANNEL_NAME_TEAM			"����Ƶ��"
#define		CHAT_TAKE_CHANNEL_NAME_FACTION		"����Ƶ��"
#define		CHAT_TAKE_CHANNEL_NAME_TONG			"���Ƶ��"
#define		CHAT_TAKE_CHANNEL_NAME_SCREENSINGLE	"˽��Ƶ��"


#endif
//--------------------------- player ������� ------------------------------
// ע����������趨�ļ����밴 �� �� ľ ľ ˮ ˮ �� �� �� �� ��˳������
#define		FACTION_FILE					"\\settings\\faction\\MonPhai.ini"
//--------------------------- player ������ ------------------------------
#define		defPLAYER_TONG_PARAM_FILE		"\\settings\\tong\\TongSet.ini"

//---------------------------- player ��ֵ��� ------------------------------
#define		PLAYER_LEVEL_EXP_FILE			"\\settings\\npc\\player\\level_exp.txt"
#define		PLAYER_LEVEL_ADD_FILE			"\\settings\\npc\\player\\level_add.txt"
#define		PLAYER_LEVEL_LEAD_EXP_FILE		"\\settings\\npc\\player\\level_lead_exp.txt"
#define		BASE_ATTRIBUTE_FILE_NAME		"\\settings\\npc\\player\\NewPlayerBaseAttribute.ini"
#define		PLAYER_PK_RATE_FILE				"\\settings\\npc\\PKRate.ini"
#define		PLAYER_BASE_VALUE				"\\settings\\npc\\player\\BaseValue.ini"
#define     NPC_LEVELSCRIPT_FILENAME		"\\script\\npclevelscript\\npclevelscript.lua"


// 00�н� 01Ů�� 02��ľ 03Ůľ 04��ˮ 05Ůˮ 06�л� 07Ů�� 08���� 09Ů��
//#define		NEW_PLAYER_INI_FILE_NAME		"\\settings\\player\\NewPlayerIni%02d.ini"

//---------------------------- player ������� ------------------------------
#define		BUYSELL_FILE					"\\settings\\buysella.txt"
#define		GOODS_FILE						"\\settings\\goodsa.txt"

//----------------------------- ��Ʒ������ ------------------------------
#define		CHANGERES_MELEE_FILE			"\\settings\\item\\MeleeRes.txt"
#define		CHANGERES_RANGE_FILE			"\\settings\\item\\RangeRes.txt"
#define		CHANGERES_ARMOR_FILE			"\\settings\\item\\ArmorRes.txt"
#define		CHANGERES_HELM_FILE				"\\settings\\item\\HelmRes.txt"
#define		CHANGERES_HORSE_FILE			"\\settings\\item\\HorseRes.txt"
#define		CHANCERES_GOLD_FILE				"\\settings\\item\\GolditemRes.txt"
#define		CHANCERES_PLAT_FILE			    "\\settings\\item\\PlatinaequipRes.txt"

#define		ITEM_ABRADE_FILE				"\\settings\\item\\AbradeRate.ini"
#define		CHANGERES_PIFENG_FILE           "\\settings\\item\\PiFengRes.txt"
#define		CHANGERES_CHIBANG_FILE          "\\settings\\item\\ChiBangRes.txt"
//--------------- npc skill missles �趨�ļ�����������ģ�� -------------
#define		SKILL_SETTING_FILE				"\\settings\\Skills.txt"
#define		MISSLES_SETTING_FILE			"\\settings\\Missles.txt"
#define		NPC_SETTING_FILE				"\\settings\\NpcS.txt"
#define		NPC_GOLD_TEMPLATE_FILE			"\\settings\\npc\\NpcGoldTemplate.txt"


//---------------------------- object ��� ------------------------------
#define		OBJ_DATA_FILE_NAME		"\\settings\\obj\\ObjData.txt"
#define		MONEY_OBJ_FILE_NAME		"\\settings\\obj\\MoneyObj.txt"

//------------------------------------------------------------------------

//#define NPC_TEMPLATE_BINFILEPATH		"\\settings"

#define WEAPON_PHYSICSSKILLFILE			"\\settings\\�������������ձ�.txt"//wuqiattacktabel.txt"//�������������ձ�.txt"				
#define WEAPON_PARTICULARTYPE			"DetailType"//"��ϸ���"
#define	WEAPON_DETAILTYPE				"ParticularType"//�������"
#define	WEAPON_SKILLID					"��Ӧ�����ܱ��"//"physicskillnumber"//"��Ӧ�����ܱ��"

//---------------------------------����--------------------------

//#define TASK_CONFIG_FILE                "\\script\\task\\tasklist.ini"
#define MAP_DROPRATE_FILE               "\\Settings\\mapList.ini"
#define GAME_SETTING_FILE               "\\Settings\\gamesetting.ini"

#define GAME_FSJINMAI_FILE              "\\Settings\\meridian\\meridian_level.txt"//"\\Settings\\FsJinMai\\fsjinmai.txt"
#define GAME_FORBITMAP_FILE             "\\Settings\\ForbititMap.txt"
#define GAME_FORBITWAIGUA_FILE          "\\Settings\\ForbititWaiGua.txt"
//#define GAME_ZHUCE_FILE                 "\\ע����Ϣ.ini"

#define GAME_ATLAS_FILE                  "\\Settings\\item\\atlas_compound.txt" //ͼ���ļ�
#define GAME_COMP_FILE                   "\\Settings\\item\\other_compound.txt" //��������
#define GAME_MSG_FILE                    "\\Settings\\EducationMessage.txt"     //������Ϣ

#define		MESSAGE_SYSTEM_ANNOUCE_HEAD		"MESSAGE_SYSTEM_ANNOUCE_HEAD"//"����"
#define		MESSAGE_SYSTEM_WAIGUA_HEAD		"MESSAGE_SYSTEM_WAIGUA_HEAD" //"���"
#define		MESSAGE_SYSTEM_TONGZHI_HEAD		"MESSAGE_SYSTEM_TONGZHI_HEAD"//"֪ͨ"
#define		MESSAGE_SYSTEM_DUCHUAN_HEAD		"MESSAGE_SYSTEM_DUCHUAN_HEAD"//"ս��"
#define     MESSAGE_SYSTEM_LIANJIE_HEAD     "MESSAGE_SYSTEM_LIANJIE_HEAD"//"����"
#define		MESSAGE_SYSTEM_PAIMING_HEAD		"MESSAGE_SYSTEM_PAIMING_HEAD"//"<bclr=blue><color=white>����<color><bclr>"
#define		MESSAGE_SYSTEM_BANGZHU_HEAD		"MESSAGE_SYSTEM_BANGZHU_HEAD"//"<bclr=yellow><color=red>����<color><bclr>"
#define		MESSAGE_SYSTEM_CITYHEAR_HEAD	"MESSAGE_SYSTEM_CITYHEAR_HEAD"//"<bclr=red><color=yellow>����<color><bclr>"

#define		L_NPC_15                        "L_NPC_15"      //[error]��������,�޷���ȷ��ȡ%s
#define		L_SortScript_4                  "L_SortScript_4"//-------�ű�������,ִ��[%s][%s]ʧ��!-------- \n
#define		L_SortScript_5                  "L_SortScript_5"
// �ر�ע�⣺�����ַ������Ȳ��ܳ���32�ֽڣ����� %d %s �Ƚ��վ��������Ժ�ĳ���
#ifndef _SERVER  //�ͻ���
#define		MSG_GET_EXP						"MSG_GET_EXP"
#define		MSG_DEC_EXP						"MSG_DEC_EXP"//û��
#define		MSG_LEVEL_UP					"MSG_LEVEL_UP"
#define		MSG_LEADER_LEVEL_UP				"MSG_LEADER_LEVEL_UP"
#define		MSG_GET_ATTRIBUTE_POINT			"MSG_GET_ATTRIBUTE_POINT"
#define		MSG_GET_SKILL_POINT				"MSG_GET_SKILL_POINT"
#define		MSG_GET_ATTRIBUTE_SKILL_POINT	"MSG_GET_ATTRIBUTE_SKILL_POINT" //û��

#define		MSG_TEAM_AUTO_REFUSE_INVITE		"MSG_TEAM_AUTO_REFUSE_INVITE"
#define		MSG_TEAM_NOT_AUTO_REFUSE_INVITE	"MSG_TEAM_NOT_AUTO_REFUSE_INVITE"//"��ʾ:��ʾ���˶�����������롣"
#define		MSG_TEAM_SEND_INVITE			"MSG_TEAM_SEND_INVITE"//"��ʾ:����(%s)�����������!"
#define		MSG_TEAM_GET_INVITE				"MSG_TEAM_GET_INVITE"//"��ʾ:(%s)���������!"
#define		MSG_TEAM_REFUSE_INVITE			"MSG_TEAM_REFUSE_INVITE"//"��ʾ:(%s)�ܾ��������������!"  //û��
#define		MSG_TEAM_CREATE					"MSG_TEAM_CREATE"//"��ʾ:��������һ֧���顣"
#define		MSG_TEAM_CREATE_FAIL			"MSG_TEAM_CREATE_FAIL"//"��ʾ:���鴴��ʧ�ܡ�"
#define		MSG_TEAM_CANNOT_CREATE			"MSG_TEAM_CANNOT_CREATE"//"��ʾ:�����ڲ������!"
#define		MSG_TEAM_TARGET_CANNOT_ADD_TEAM	"MSG_TEAM_TARGET_CANNOT_ADD_TEAM"//"��ʾ:�Է����ڲ������!"
#define		MSG_TEAM_OPEN					"MSG_TEAM_OPEN"//"��ʾ:���Ķ���������������¶�Ա��"
#define		MSG_TEAM_CLOSE					"MSG_TEAM_CLOSE"//"��ʾ:���Ķ������ڲ���������¶�Ա��"
#define		MSG_TEAM_ADD_MEMBER				"MSG_TEAM_ADD_MEMBER"//"��ʾ:(%s)��Ϊ���Ķ����ˡ�"
#define		MSG_TEAM_SELF_ADD				"MSG_TEAM_SELF_ADD"//"��ʾ:������(%s)�Ķ��顣"
#define		MSG_TEAM_DISMISS_CAPTAIN		"MSG_TEAM_DISMISS_CAPTAIN"//"��ʾ:����ɢ���Լ��Ķ���!"
#define		MSG_TEAM_DISMISS_MEMBER			"MSG_TEAM_DISMISS_MEMBER"//"��ʾ:(%s)��ɢ�˶���!"
#define		MSG_TEAM_KICK_ONE				"MSG_TEAM_KICK_ONE"//"��ʾ:(%s)������������!"
#define		MSG_TEAM_BE_KICKEN				"MSG_TEAM_BE_KICKEN"//"��ʾ:��������������!"
#define		MSG_TEAM_APPLY_ADD				"MSG_TEAM_APPLY_ADD"//"��ʾ:(%s)����������!"
#define		MSG_TEAM_APPLY_ADD_SELF_MSG		"MSG_TEAM_APPLY_ADD_SELF_MSG"//"��ʾ:���������(%s)�Ķ���!"
#define		MSG_TEAM_LEAVE					"MSG_TEAM_LEAVE"//"��ʾ:(%s)�뿪���顣"
#define		MSG_TEAM_LEAVE_SELF_MSG			"MSG_TEAM_LEAVE_SELF_MSG"//"��ʾ:���뿪(%s)�Ķ��顣"
#define		MSG_TEAM_CHANGE_CAPTAIN_FAIL1	"MSG_TEAM_CHANGE_CAPTAIN_FAIL1"//"��ʾ:�ӳ�����ʧ��!"
#define		MSG_TEAM_CHANGE_CAPTAIN_FAIL2	"MSG_TEAM_CHANGE_CAPTAIN_FAIL2"//"��ʾ:(%s)ͳ˧������!"
#define		MSG_TEAM_CHANGE_CAPTAIN_FAIL3	"MSG_TEAM_CHANGE_CAPTAIN_FAIL3"//"��ʾ:���Ķ��鲻���ƽ�������!"
#define		MSG_TEAM_CHANGE_CAPTAIN			"MSG_TEAM_CHANGE_CAPTAIN"     //"��ʾ:(%s)������Ϊ�ӳ�!"
#define		MSG_TEAM_CHANGE_CAPTAIN_SELF	"MSG_TEAM_CHANGE_CAPTAIN_SELF"//"��ʾ:����(%s)����Ϊ�ӳ�!"

#define		MSG_CHAT_APPLY_ADD_FRIEND		"MSG_CHAT_APPLY_ADD_FRIEND"//"��ʾ:����(%s)������ѡ�"
#define		MSG_CHAT_FRIEND_HAD_IN			"MSG_CHAT_FRIEND_HAD_IN"//"��ʾ:(%s)�ѱ���Ϊ���ѡ�"
#define		MSG_CHAT_GET_FRIEND_APPLY		"MSG_CHAT_GET_FRIEND_APPLY"//"��ʾ:(%s)�����Ϊ����!"
#define		MSG_CHAT_REFUSE_FRIEND			"MSG_CHAT_REFUSE_FRIEND"//"��ʾ:(%s)�ܾ���������!"
#define		MSG_CHAT_ADD_FRIEND_FAIL		"MSG_CHAT_ADD_FRIEND_FAIL"//"��ʾ:���(%s)Ϊ����ʧ��!"
#define		MSG_CHAT_ADD_FRIEND_SUCCESS		"MSG_CHAT_ADD_FRIEND_SUCCESS"//"��ʾ:����(%s)��Ϊ����!"
#define		MSG_CHAT_CREATE_TEAM_FAIL1		"MSG_CHAT_CREATE_TEAM_FAIL1"//"��ʾ:���ѷ��鴴��ʧ��!"
#define		MSG_CHAT_CREATE_TEAM_FAIL2		"MSG_CHAT_CREATE_TEAM_FAIL2"//"��ʾ:�Ѵ���ͬ����!"
#define		MSG_CHAT_CREATE_TEAM_FAIL3		"MSG_CHAT_CREATE_TEAM_FAIL3"//"��ʾ:�Ѵ���������!"
#define		MSG_CHAT_RENAME_TEAM_FAIL		"MSG_CHAT_RENAME_TEAM_FAIL"//"��ʾ:�����޸�ʧ��!"
#define		MSG_CHAT_DELETE_TEAM_FAIL1		"MSG_CHAT_DELETE_TEAM_FAIL1"//"��ʾ:��ɾ��ʧ��!"
#define		MSG_CHAT_DELETE_TEAM_FAIL2		"MSG_CHAT_DELETE_TEAM_FAIL2"//"��ʾ:����鲻����ɾ��!"
#define		MSG_CHAT_FRIEND_ONLINE			"MSG_CHAT_FRIEND_ONLINE"//"��ʾ:(%s)������!"
#define		MSG_CHAT_FRIEND_OFFLINE			"MSG_CHAT_FRIEND_OFFLINE"//"��ʾ:(%s)������!"
#define		MSG_CHAT_DELETE_FRIEND			"MSG_CHAT_DELETE_FRIEND"//"��ʾ:����(%s)����˺��ѹ�ϵ!"
#define		MSG_CHAT_DELETED_FRIEND			"MSG_CHAT_DELETED_FRIEND"//"��ʾ:(%s)��������˺��ѹ�ϵ!"
#define		MSG_CHAT_MSG_FROM_FRIEND		"MSG_CHAT_MSG_FROM_FRIEND"//"��ʾ:(%s)������Ϣ!"
#define		MSG_CHAT_FRIEND_NOT_NEAR		"MSG_CHAT_FRIEND_NOT_NEAR"//"��ʾ:(%s)���ڸ���!"
#define		MSG_CHAT_FRIEND_NOT_ONLINE		"MSG_CHAT_FRIEND_NOT_ONLINE"//"��ʾ:(%s)������!"
#define		MSG_CHAT_TAR_REFUSE_SINGLE_TALK	"MSG_CHAT_TAR_REFUSE_SINGLE_TALK"//"��ʾ:(%s)û�ж���˽��Ƶ��!"

#define		MSG_SHOP_NO_ROOM				"MSG_SHOP_NO_ROOM"//"��ʾ:�����ռ䲻��!"
#define		MSG_SHOP_NO_MONEY				"MSG_SHOP_NO_MONEY"//"��ʾC:���<color=green>˰��(%d%s)<color>,��Ǯ����!"
#define		MSG_SHOP_YOU_MONEY				"MSG_SHOP_YOU_MONEY"//"��ʾ:ռ���Ա�Ż�<color=green>(%d%s)<color>,��Ǯ����!"
#define		MSG_SHOP_YOUS_MONEY				"MSG_SHOP_YOUS_MONEY"//"��ʾ:ռ���Ա�Ż�<color=green>(%d%s)<color>,������<color=green>(%d)<color>��!"
#define		MSG_SHOP_YOUF_MONEY				"MSG_SHOP_YOUF_MONEY"//"��ʾ:��ӳ���<color=green>˰��(%d%s)<color>,������<color=green>(%d)<color>��!"
#define		MSG_HUANZHUANG_NO_ROOM			"MSG_HUANZHUANG_NO_ROOM"//"��ʾ:<color=yellow>�Ҽ���װ���ܼ����Ƴ�!<color>"
#define		MSG_SKILL_EXISTS_IN_IMMEDIA     "MSG_SKILL_EXISTS_IN_IMMEDIA"    //��������Ѿ��иü�����
#define		MSG_SKILL_IS_NOT_IMMEDIA        "MSG_SKILL_IS_NOT_IMMEDIA"       //�ü��ܲ��ǿ�ݼ��ܣ����ܷŵ��������
#define		MSG_ITEM_CANNOT_IN_IMMEDIATE    "MSG_ITEM_CANNOT_IN_IMMEDIATE"   //������Ʒ���ܷŵ��������

#define		MSG_NPC_NO_MANA					"MSG_NPC_NO_MANA"//"��ʾ:��������!"
#define		MSG_NPC_NO_STAMINA				"MSG_NPC_NO_STAMINA"//"��ʾ:��������!"
#define		MSG_NPC_NO_LIFE					"MSG_NPC_NO_LIFE"//"��ʾ:��������!"
#define		MSG_NPC_DEATH					"MSG_NPC_DEATH"//"��ʾ:Я��<color=green>����ӡ��<color>����ԭ������!"

#define		MSG_OBJ_CANNOT_PICKUP			"MSG_OBJ_CANNOT_PICKUP"//"��ʾ:������ʰȡ���˵���Ʒ!"
#define		MSG_MONEY_CANNOT_PICKUP			"MSG_MONEY_CANNOT_PICKUP"//"��ʾ:������ʰȡ���˵�Ǯ!"
#define		MSG_OBJ_TOO_FAR					"MSG_OBJ_TOO_FAR"//"��ʾ:��Ʒ̫Զ���޷�ʰȡ!"
#define		MSG_DEC_MONEY					"MSG_DEC_MONEY"//"��ʾ:����ʧ��(%d)������!"
#define		MSG_EARN_MONEY					"MSG_EARN_MONEY"//"��ʾ:�������(%d)������!"
#define		MSG_DEATH_LOSE_ITEM				"MSG_DEATH_LOSE_ITEM"//"��ʾ:����ʧ����Ʒ%s!"
#define		MSG_ADD_ITEM					"MSG_ADD_ITEM"//"��ʾ:�������(%s)!"
#define		MSG_ITEM_SAME_DETAIL_IN_IMMEDIATE	"MSG_ITEM_SAME_DETAIL_IN_IMMEDIATE"//"��ʾ:������Ѿ���ͬ���͵���Ʒ��!"
#define		MSG_CAN_NOT_VIEW_ITEM			"MSG_CAN_NOT_VIEW_ITEM"//"��ʾ:���ڲ��ܲ쿴����װ��!"//û��
#define		MSG_ITEM_DAMAGED				"MSG_ITEM_DAMAGED"//"��ʾ:��װ����(%s)����ĥ���Ѿ�������!"
#define		MSG_ITEM_NEARLY_DAMAGED			"MSG_ITEM_NEARLY_DAMAGED"//"��ʾ:��װ���Ѿ��ӽ���,�뼰ʱ����!"

#define		MSG_TRADE_STATE_OPEN			"MSG_TRADE_STATE_OPEN"//"��ʾ:�����ڿ��Խ��ס�"
#define		MSG_TRADE_STATE_CLOSE			"MSG_TRADE_STATE_CLOSE"//"��ʾ:�����ڲ��ܽ��ס�"
#define		MSG_TRADE_SELF_LOCK				"MSG_TRADE_SELF_LOCK"//"��ʾ:��������Ʒ�Ѿ�������"
#define		MSG_TRADE_SELF_UNLOCK			"MSG_TRADE_SELF_UNLOCK"//"��ʾ:��������Ʒ���������"
#define		MSG_TRADE_DEST_LOCK				"MSG_TRADE_DEST_LOCK"//"��ʾ:(%s)������Ʒ�Ѿ�������"
#define		MSG_TRADE_DEST_UNLOCK			"MSG_TRADE_DEST_UNLOCK"//"��ʾ:(%s)������Ʒ���������"
#define		MSG_TRADE_SUCCESS				"MSG_TRADE_SUCCESS"//"��ʾ:����(%s)���׳ɹ���"
#define		MSG_TRADE_FAIL					"MSG_TRADE_FAIL"//"��ʾ:����(%s)����ʧ�ܡ�"
#define		MSG_TRADE_SELF_ROOM_FULL		"MSG_TRADE_SELF_ROOM_FULL"//"��ʾ:���ı����ռ䲻��!"
#define		MSG_TRADE_DEST_ROOM_FULL		"MSG_TRADE_DEST_ROOM_FULL"//"��ʾ:(%s)�ı����ռ䲻��!"
#define		MSG_TRADE_SEND_APPLY			"MSG_TRADE_SEND_APPLY"//"��ʾ:����(%s)���뽻��!"
#define		MSG_TRADE_GET_APPLY				"MSG_TRADE_GET_APPLY"//"��ʾ:(%s)������������!"
#define		MSG_TRADE_REFUSE_APPLY			"MSG_TRADE_REFUSE_APPLY"//"��ʾ:(%s)�ܾ���������!"
#define		MSG_TRADE_TASK_ITEM				"MSG_TRADE_TASK_ITEM"//"��ʾ:������ʱ����Ʒ���ܽ���!"

#define		G_ProtocolProcess_20            "G_ProtocolProcess_20"  //�㵱ǰ״̬��ֹ�ƶ����߽�����Ʒ��

#define		MSG_PK_NORMAL_FLAG_OPEN			"MSG_PK_NORMAL_FLAG_1" //"MSG_PK_NORMAL_FLAG_OPEN"//"��ʾ:PK���ش�!"
#define		MSG_PK_NORMAL_FLAG_CLOSE		"MSG_PK_NORMAL_FLAG_0"//"MSG_PK_NORMAL_FLAG_CLOSE"//"��ʾ:PK���عر�!"
#define		MSG_PK_VALUE					"MSG_PK_VALUE"//"��ʾ:��Ŀǰ��PKֵ��%d!"
#define		MSG_PK_ERROR_1					"MSG_PK_ERROR_1"//"��ʾ:�����ڴ��ڷ�ս��״̬!"
#define		MSG_PK_ERROR_2					"MSG_PK_ERROR_2"//"��ʾ:���ֲ��ܳ�ɱ!"
#define		MSG_PK_ERROR_3					"MSG_PK_ERROR_3"//"��ʾ:�����������д�!"
#define		MSG_PK_ERROR_4					"MSG_PK_ERROR_4"//"��ʾ:���������˳�ɱ!"
#define		MSG_PK_ERROR_5					"MSG_PK_ERROR_5"//"��ʾ:�����ܳ�ɱ����!"
#define		MSG_PK_ERROR_6					"MSG_PK_ERROR_6"//"��ʾ:�Է������ڷ�ս��״̬!"
#define		MSG_PK_ERROR_7					"MSG_PK_ERROR_7"//"��ʾ:�Է����������д�!"
#define		MSG_PK_ENMITY_SUCCESS_1			"MSG_PK_ENMITY_SUCCESS_1"//"��ʾ:����(%s)������ɱ��ϵ!"
#define		MSG_PK_ENMITY_SUCCESS_2			"MSG_PK_ENMITY_SUCCESS_2"//"��ʾ:10���Ӻ��ɱ��ʼִ��!"
#define		MSG_PK_ENMITY_CLOSE				"MSG_PK_ENMITY_CLOSE"//"��ʾ:���ĳ�ɱ��ϵ����!"
#define		MSG_PK_ENMITY_OPEN				"MSG_PK_ENMITY_OPEN"//"��ʾ:��ɱ���ڿ�ʼִ��!"

#define		MSG_TONG_CREATE_ERROR01			"MSG_TONG_CREATE_ERROR01"//"��ʾ:���������!����������"
#define		MSG_TONG_CREATE_ERROR06			"MSG_TONG_CREATE_ERROR06"//"��ʾ:�����Ӫ�����ͳ˧�ȼ�����!"
#define		MSG_TONG_CREATE_ERROR05		    "MSG_TONG_CREATE_ERROR05"//"��ʾ:����Ա�������⽨�����!"
#define		MSG_TONG_CREATE_ERROR04			"MSG_TONG_CREATE_ERROR04"//"��ʾ:ɱ�ֲ��ܽ������!"
#define		MSG_TONG_CREATE_ERROR07			"MSG_TONG_CREATE_ERROR07"//"��ʾ:�������ĵȼ�����!"
#define		MSG_TONG_CREATE_ERROR08			"MSG_TONG_CREATE_ERROR08"//"��ʾ:���������Ҫ(%d)��Ǯ!"
#define		MSG_TONG_CREATE_ERROR09			"MSG_TONG_CREATE_ERROR09"//"��ʾ:����У����ܽ������!"
#define		MSG_TONG_CREATE_ERROR10		    "MSG_TONG_CREATE_ERROR10"//"��ʾ:���ģ�����!����ϵGM!"
#define		MSG_TONG_CREATE_ERROR11			"MSG_TONG_CREATE_ERROR11"//"��ʾ:��������Ϸ��������ַ�������!"
#define		MSG_TONG_CREATE_ERROR12			"MSG_TONG_CREATE_ERROR12"//"��ʾ:��������Ϸ��������ַ�������!"
#define		MSG_TONG_CREATE_ERROR13			"MSG_TONG_CREATE_ERROR13"//"��ʾ:��������Ϸ������İ������!"
#define		MSG_TONG_APPLY_CREATE			"MSG_TONG_APPLY_CREATE"//"��ʾ:�����봴�����!"
#define		MSG_TONG_CREATE_SUCCESS			"MSG_TONG_CREATE_SUCCESS"//"��ʾ:��ᴴ���ɹ�!"
#define		MSG_TONG_APPLY_ADD				"MSG_TONG_APPLY_ADD"//"��ʾ:�����������!"
#define		MSG_TONG_APPLY_ADD_ERROR1		"MSG_TONG_APPLY_ADD_ERROR1"//"��ʾ:����Ա�������������!"
#define		MSG_TONG_APPLY_ADD_ERROR2		"MSG_TONG_APPLY_ADD_ERROR2"//"��ʾ:ɱ�ֲ��ܼ�����!"
#define		MSG_TONG_APPLY_ADD_ERROR3		"MSG_TONG_APPLY_ADD_ERROR3"//"��ʾ:����У����ܼ�����!"
#define		MSG_TONG_REFUSE_ADD				"MSG_TONG_REFUSE_ADD"//"��ʾ:(%s)�ܾ���������!"
#define		MSG_TONG_ADD_SUCCESS			"MSG_TONG_ADD_SUCCESS"//"��ʾ:��������!"
#define		MSG_TONG_CANNOT_LEAVE1			"MSG_TONG_CANNOT_LEAVE1"//"��ʾ:���������뿪���!"
#define		MSG_TONG_CANNOT_LEAVE2			"MSG_TONG_CANNOT_LEAVE2"//"��ʾ:���ϲ����뿪���!"
#define		MSG_TONG_BE_KICKED				"MSG_TONG_BE_KICKED"//"��ʾ:�����߳����!"
#define		MSG_TONG_LEAVE_SUCCESS			"MSG_TONG_LEAVE_SUCCESS"//"��ʾ:�������뿪���ɹ�!"
#define		MSG_TONG_LEAVE_FAIL				"MSG_TONG_LEAVE_FAIL"//"��ʾ:�������뿪���ʧ��!"
#define		MSG_TONG_CHANGE_AS_MASTER		"MSG_TONG_CHANGE_AS_MASTER"//"��ʾ:��������Ϊ����!"
#define		MSG_TONG_CHANGE_AS_MEMBER		"MSG_TONG_CHANGE_AS_MEMBER"//"��ʾ:�����ڳ�Ϊ��ͨ����!"

#define		MSG_EARN_XU		                "MSG_EARN_XU"          //"��ʾ:�������(%d)��!"
#define		MSG_PK_NORMAL_FLAG_DS		    "MSG_PK_NORMAL_FLAG_2" //"MSG_PK_NORMAL_FLAG_DS"//"��ʾ:��ɱģʽ����!"
#define		MSG_NPC_CANNOT_RIDE		        "MSG_NPC_CANNOT_RIDE"  //"��ʾ:��̫����,����Ƶ����������!"
#define     MSG_SUNYI_FAILED                "MSG_SUNYI_FAILED"     //"��ʾ:˲���ƶ�ִ��ʧ��"  //û��
#define     MSG_SUNYI_SUCCESS               "MSG_SUNYI_SUCCESS"    //"��ʾ:˲���ƶ�ִ�гɹ�"

#define     MSG_GUAJI_SUCCESS               "MSG_GUAJI_SUCCESS"//"��ʾ:�Զ��һ�����!"
#define		MSG_COMP_FAILED					"MSG_COMP_FAILED"//"��ʾ:�ϳ�ʧ��!"
#define		MSG_COMP_SUCCESS				"MSG_COMP_SUCCESS"//"��ʾ:�ϳɳɹ�!"
#define		MSG_COMP_ERITEM					"MSG_COMP_ERITEM"//"��ʾ:��Ʒ����!"
#define		MSG_COMP_MONEY					"MSG_COMP_MONEY"//"��ʾ:��Ǯ����!"
#define		MSG_ID_COMP_NO                  "MSG_ID_COMP_NO"//"��ʾ:����Ʒ��ֹ����!"
#define		MSG_SHOP_NO_XU		            "MSG_SHOP_NO_XU"//"��ʾ:���<color=green>˰��(%d%s)<color>,��һ���������!"
#define		MSG_SHOP_NO_FUYUAN	            "MSG_SHOP_NO_FUYUAN"
#define		MSG_SHOP_NO_SHENGWANG	        "MSG_SHOP_NO_SHENGWANG"
#define		MSG_SHOP_NO_DIANKA	            "MSG_SHOP_NO_DIANKA"
#define		MSG_SHOP_NO_JIFEN	            "MSG_SHOP_NO_JIFEN"
#define		MSG_SHOP_YOU_XU		            "MSG_SHOP_YOU_XU"//"��ʾ:ռ���Ա�Ż�<color=green>(%d%s)<color>,��һ���������!"
#define		MSG_SHOP_YOUS_XU				"MSG_SHOP_YOUS_XU"//"��ʾ:ռ���Ա�Ż�<color=green>(%d%s)<color>,������<color=green>(%s)<color>���!" //û��
#define		MSG_SHOP_YOUF_XU				"MSG_SHOP_YOUF_XU"//"��ʾ:��ӳ���<color=green>˰��(%d%s)<color>,������<color=green>(%s)<color>���!"//û��
#define	    MSG_DROP_DEST_FAILED            "MSG_DROP_DEST_FAILED"//"��ʾ:��Ʒ������㶪��!" û��
#define	    MSG_ID_LIANJIE_FAILED           "MSG_ID_LIANJIE_FAILED"//"��ʾ:<color=white>�������Ѿ�ʧЧ!<color>"           
#define		MSG_TEAM_CREAT_FRIEND			"MSG_TEAM_CREAT_FRIEND"//"��ʾ:������ӽ���ʧ��!" û��
#define	    MSG_ID_SHUA_SHUA                "MSG_ID_SHUA_SHUA"//"��ʾ:<color=white>����ʧ��!<color>"
#define		MSG_CAN_NOT_PK_SUNYI			"MSG_CAN_NOT_PK_SUNYI"//"��ʾ:��ս��ģʽ״̬,����Ѱ·����!"
#define		MSG_ID_FUHUO_YUANDIAN           "MSG_ID_FUHUO_YUANDIAN"//"<color=yellow>����:���������<color=green>�������<color>�Ϳ���ԭ�ظ�����!"
#define		MSG_SKILL_CANNOT_PICKUP         "MSG_SKILL_CANNOT_PICKUP" //����ʰȡ�ü���

#endif

enum enumMSG_ID
{
	enumMSG_ID_TEAM_KICK_One,
	enumMSG_ID_TEAM_DISMISS,
	enumMSG_ID_TEAM_LEAVE,
	enumMSG_ID_TEAM_REFUSE_INVITE,
	enumMSG_ID_TEAM_SELF_ADD,
	enumMSG_ID_TEAM_CHANGE_CAPTAIN_FAIL,
	enumMSG_ID_TEAM_CHANGE_CAPTAIN_FAIL2,
	enumMSG_ID_OBJ_CANNOT_PICKUP,
	enumMSG_ID_OBJ_TOO_FAR,
	enumMSG_ID_DEC_MONEY,
	enumMSG_ID_TRADE_SELF_ROOM_FULL,
	enumMSG_ID_TRADE_DEST_ROOM_FULL,
	enumMSG_ID_TRADE_REFUSE_APPLY,
	enumMSG_ID_TRADE_TASK_ITEM,
	enumMSG_ID_GET_ITEM,
	enumMSG_ID_ITEM_DAMAGED,
	enumMSG_ID_MONEY_CANNOT_PICKUP,
	enumMSG_ID_CANNOT_ADD_TEAM,
	enumMSG_ID_TARGET_CANNOT_ADD_TEAM,
	enumMSG_ID_PK_ERROR_1,
	enumMSG_ID_PK_ERROR_2,
	enumMSG_ID_PK_ERROR_3,
	enumMSG_ID_PK_ERROR_4,
	enumMSG_ID_PK_ERROR_5,
	enumMSG_ID_PK_ERROR_6,
	enumMSG_ID_PK_ERROR_7,
	enumMSG_ID_DEATH_LOSE_ITEM,
	enumMSG_ID_TONG_REFUSE_ADD,
	enumMSG_ID_TONG_BE_KICK,
	enumMSG_ID_TONG_LEAVE_SUCCESS,
	enumMSG_ID_TONG_LEAVE_FAIL,
	enumMSG_ID_TONG_CHANGE_AS_MASTER,
	enumMSG_ID_TONG_CHANGE_AS_MEMBER,
	enumMSG_ID_RIDE_CANNOT, //����
	enumMSG_ID_COMP_FAILED,//����ϳ�
	enumMSG_ID_COMP_SUCCESS,
	enumMSG_ID_COMP_ERITEM,
	enumMSG_ID_SUNYI_FAILED, //˲��
	enumMSG_ID_GUAJI_SUCCESS,//�һ�
	enumMSG_ID_GUAJI_SHIBAIA,
	enumMSG_ID_GUAJI_SHIBAIB,
	enumMSG_ID_GUAJI_SHIBAIC,
	enumMSG_ID_GUAJI_SHIBAID,
	enumMSG_ID_GUAJI_SHIBAIE,
	enumMSG_ID_GUAJI_SHIBAIF,
	enumMSG_ID_GUAJI_SHIBAIG,
	enumMSG_ID_SUNYI_SUCCESS,
	enumMSG_CAN_NOT_PK_SUNYI,
	enumMSG_ID_DROP_DEST_FAILED,
    enumMSG_SHOP_YOUS_MONEY,
	enumMSG_SHOP_YOUF_MONEY,
	enumMSG_SHOP_YOUS_XU,
	enumMSG_SHOP_YOUF_XU,
	enumMSG_ID_LIANJIE_FAILED,//����ʧЧ
	enumMSG_ID_SHUA_SHUA,
	enumMSG_ID_RENOVE_TIME_ITEM, //ɾ��������Ʒ
    enumMSG_ID_FUHUO_FAILED,
	enumMSG_ID_ERROR_INFO,
	enumTONG_COMMAND_ID_APPLY_SAVE,
	enumTONG_COMMAND_ID_APPLY_GET,
	enumTONG_COMMAND_ID_APPLY_SND,
	enumMSG_ID_COMP_MONEY,
	enumMSG_ID_COMP_NO,
	enumMSG_ID_NUM,
};

//---------------------------- ���ָ����� ------------------------------
#define		MOUSE_CURSOR_NORMAL				0
#define		MOUSE_CURSOR_FIGHT				1
#define		MOUSE_CURSOR_DIALOG				2
#define		MOUSE_CURSOR_PICK				3
#define		MOUSE_CURSOR_USE				8




#endif
