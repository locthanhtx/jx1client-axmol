// ***************************************************************************************
// Chỉnh sửa lại bởi duccom0123 07/09/2024 6:42:22 CH
// ***************************************************************************************

#ifndef COREUSENAMEDEF_H
#define COREUSENAMEDEF_H

#define MAX_PLAYER_IN_ACCOUNT               3

#define PLAYER_MENU_STATE_RES_FILE          "\\settings\\npcres\\界面状态与图形对照表.txt"
#define NPC_RES_KIND_FILE_NAME              "\\settings\\npcres\\人物类型.txt"

#define RES_INI_FILE_PATH                   "\\settings\\npcres"

#define RES_SOUND_FILE_PATH                 "sound"

#define NPC_NORMAL_RES_FILE                 "\\settings\\npcres\\普通npc资源.txt"
#define NPC_NORMAL_SPRINFO_FILE             "\\settings\\npcres\\普通npc资源信息.txt"
#define STATE_MAGIC_TABLE_NAME              "\\settings\\npcres\\状态图形对照表.txt"
#define PLAYER_RES_SHADOW_FILE              "\\settings\\npcres\\主角动作阴影对应表.txt"

#define PLAYER_SOUND_FILE                   "\\settings\\npcres\\主角动作声音表.txt"
#define NPC_SOUND_FILE                      "\\settings\\npcres\\npc动作声音表.txt"

#define NPC_ACTION_NAME                     "npc动作表.txt"
#define ACTION_FILE_NAME                    "动作编号表.txt"

#define PLAYER_INSTANT_SPECIAL_FILE         "\\settings\\npcres\\瞬间特效.txt"

#define SPR_INFO_NAME                       "信息"
#define KIND_NAME_SECT                      "人物类型"
#define KIND_NAME_SPECIAL                   "SpecialNpc"
#define KIND_NAME_NORMAL                    "NormalNpc"
#define KIND_FILE_SECT1                     "部件说明文件名"
#define KIND_FILE_SECT2                     "武器行为关联表1"
#define KIND_FILE_SECT3                     "武器行为关联表2"
#define KIND_FILE_SECT4                     "动作贴图顺序表"
#define KIND_FILE_SECT5                     "资源文件路经"
#define defLITTLE_MAP_SET_FILE              "\\Ui\\Default\\小地图颜色.ini"
#define CHAT_PATH                           "\\chat"
#define CHAT_TEAM_INFO_FILE_NAME            "Team.cht"
#define CHAT_CHANNEL_INFO_FILE_NAME         "Channel.cht"
#define defINSTANT_SOUND_FILE               "\\settings\\SoundList.txt"
#define defMUSIC_SET_FILE                   "\\settings\\music\\MusicSet.txt"
#define defMUSIC_FIGHT_SET_FILE             "\\settings\\music\\MusicFightSet.ini"

#define OBJ_NAME_COLOR_FILE                 "\\settings\\obj\\ObjNameColor.ini"

#define STRINGRESOURSE_TABFILE              "\\settings\\StringResource.txt"
#define PLAYER_RANK_SETTING_TABFILE         "\\settings\\RankSetting.txt"

#define CHAT_CHANNEL_NAME_ALL               "所有玩家"
#define CHAT_CHANNEL_NAME_SCREEN            "附近玩家"
#define CHAT_CHANNEL_NAME_SINGLE            "好友"
#define CHAT_CHANNEL_NAME_TEAM              "队友"
#define CHAT_CHANNEL_NAME_FACTION           "同门"
#define CHAT_CHANNEL_NAME_TONG              "帮众"
#define CHAT_CHANNEL_NAME_SCREENSINGLE      "陌生人"
#define CHAT_CHANNEL_NAME_SYSTEM            "系统"

#define CHAT_TAKE_CHANNEL_NAME_TEAM         "队伍频道"
#define CHAT_TAKE_CHANNEL_NAME_FACTION      "门派频道"
#define CHAT_TAKE_CHANNEL_NAME_TONG         "帮会频道"
#define CHAT_TAKE_CHANNEL_NAME_SCREENSINGLE "私聊频道"

#define FACTION_FILE                        "\\settings\\faction\\MonPhai.ini"
#define defPLAYER_TONG_PARAM_FILE           "\\settings\\tong\\TongSet.ini"

#define PLAYER_LEVEL_EXP_FILE               "\\settings\\npc\\player\\level_exp.txt"
#define PLAYER_LEVEL_ADD_FILE               "\\settings\\npc\\player\\level_add.txt"
#define PLAYER_LEVEL_LEAD_EXP_FILE          "\\settings\\npc\\player\\level_lead_exp.txt"
#define BASE_ATTRIBUTE_FILE_NAME            "\\settings\\npc\\player\\NewPlayerBaseAttribute.ini"
#define PLAYER_PK_RATE_FILE                 "\\settings\\npc\\PKRate.ini"
#define PLAYER_BASE_VALUE                   "\\settings\\npc\\player\\BaseValue.ini"
#define NPC_LEVELSCRIPT_FILENAME            "\\script\\npclevelscript\\npclevelscript.lua"

#define BUYSELL_FILE                        "\\settings\\buysella.txt"
#define GOODS_FILE                          "\\settings\\goodsa.txt"

#define CHANGERES_MELEE_FILE                "\\settings\\item\\MeleeRes.txt"
#define CHANGERES_RANGE_FILE                "\\settings\\item\\RangeRes.txt"
#define CHANGERES_ARMOR_FILE                "\\settings\\item\\ArmorRes.txt"
#define CHANGERES_HELM_FILE                 "\\settings\\item\\HelmRes.txt"
#define CHANGERES_HORSE_FILE                "\\settings\\item\\HorseRes.txt"
#define CHANCERES_GOLD_FILE                 "\\settings\\item\\GolditemRes.txt"
#define CHANCERES_PLAT_FILE                 "\\settings\\item\\PlatinaequipRes.txt"

#define ITEM_ABRADE_FILE                    "\\settings\\item\\AbradeRate.ini"
#define CHANGERES_PIFENG_FILE               "\\settings\\item\\PiFengRes.txt"
#define CHANGERES_CHIBANG_FILE              "\\settings\\item\\ChiBangRes.txt"
#define SKILL_SETTING_FILE                  "\\settings\\Skills.txt"
#define MISSLES_SETTING_FILE                "\\settings\\Missles.txt"
#define NPC_SETTING_FILE                    "\\settings\\NpcS.txt"
#define NPC_GOLD_TEMPLATE_FILE              "\\settings\\npc\\NpcGoldTemplate.txt"

#define OBJ_DATA_FILE_NAME                  "\\settings\\obj\\ObjData.txt"
#define MONEY_OBJ_FILE_NAME                 "\\settings\\obj\\MoneyObj.txt"

#define WEAPON_PHYSICSSKILLFILE             "\\settings\\武器物理攻击对照表.txt"
#define WEAPON_PARTICULARTYPE               "DetailType"
#define WEAPON_DETAILTYPE                   "ParticularType"
#define WEAPON_SKILLID                      "对应物理技能编号"

#define MAP_DROPRATE_FILE                   "\\Settings\\mapList.ini"
#define GAME_SETTING_FILE                   "\\Settings\\gamesetting.ini"

#define GAME_FSJINMAI_FILE                  "\\Settings\\meridian\\meridian_level.txt"
#define GAME_FORBITMAP_FILE                 "\\Settings\\ForbititMap.txt"
#define GAME_FORBITWAIGUA_FILE              "\\Settings\\ForbititWaiGua.txt"

#define GAME_ATLAS_FILE                     "\\Settings\\item\\atlas_compound.txt"
#define GAME_COMP_FILE                      "\\Settings\\item\\other_compound.txt"
#define GAME_MSG_FILE                       "\\Settings\\EducationMessage.txt"

#define MESSAGE_SYSTEM_ANNOUCE_HEAD         "MESSAGE_SYSTEM_ANNOUCE_HEAD"
#define MESSAGE_SYSTEM_WAIGUA_HEAD          "MESSAGE_SYSTEM_WAIGUA_HEAD"
#define MESSAGE_SYSTEM_TONGZHI_HEAD         "MESSAGE_SYSTEM_TONGZHI_HEAD"
#define MESSAGE_SYSTEM_DUCHUAN_HEAD         "MESSAGE_SYSTEM_DUCHUAN_HEAD"
#define MESSAGE_SYSTEM_LIANJIE_HEAD         "MESSAGE_SYSTEM_LIANJIE_HEAD"
#define MESSAGE_SYSTEM_PAIMING_HEAD         "MESSAGE_SYSTEM_PAIMING_HEAD"
#define MESSAGE_SYSTEM_BANGZHU_HEAD         "MESSAGE_SYSTEM_BANGZHU_HEAD"
#define MESSAGE_SYSTEM_CITYHEAR_HEAD        "MESSAGE_SYSTEM_CITYHEAR_HEAD"

#define L_NPC_15                            "L_NPC_15"
#define L_SortScript_4                      "L_SortScript_4"
#define L_SortScript_5                      "L_SortScript_5"

#define MSG_GET_EXP                         "MSG_GET_EXP"
#define MSG_DEC_EXP                         "MSG_DEC_EXP"
#define MSG_LEVEL_UP                        "MSG_LEVEL_UP"
#define MSG_LEADER_LEVEL_UP                 "MSG_LEADER_LEVEL_UP"
#define MSG_GET_ATTRIBUTE_POINT             "MSG_GET_ATTRIBUTE_POINT"
#define MSG_GET_SKILL_POINT                 "MSG_GET_SKILL_POINT"
#define MSG_GET_ATTRIBUTE_SKILL_POINT       "MSG_GET_ATTRIBUTE_SKILL_POINT"

#define MSG_TEAM_AUTO_REFUSE_INVITE         "MSG_TEAM_AUTO_REFUSE_INVITE"
#define MSG_TEAM_NOT_AUTO_REFUSE_INVITE     "MSG_TEAM_NOT_AUTO_REFUSE_INVITE"
#define MSG_TEAM_SEND_INVITE                "MSG_TEAM_SEND_INVITE"
#define MSG_TEAM_GET_INVITE                 "MSG_TEAM_GET_INVITE"
#define MSG_TEAM_REFUSE_INVITE              "MSG_TEAM_REFUSE_INVITE"
#define MSG_TEAM_CREATE                     "MSG_TEAM_CREATE"
#define MSG_TEAM_CREATE_FAIL                "MSG_TEAM_CREATE_FAIL"
#define MSG_TEAM_CANNOT_CREATE              "MSG_TEAM_CANNOT_CREATE"
#define MSG_TEAM_TARGET_CANNOT_ADD_TEAM     "MSG_TEAM_TARGET_CANNOT_ADD_TEAM"
#define MSG_TEAM_OPEN                       "MSG_TEAM_OPEN"
#define MSG_TEAM_CLOSE                      "MSG_TEAM_CLOSE"
#define MSG_TEAM_ADD_MEMBER                 "MSG_TEAM_ADD_MEMBER"
#define MSG_TEAM_SELF_ADD                   "MSG_TEAM_SELF_ADD"
#define MSG_TEAM_DISMISS_CAPTAIN            "MSG_TEAM_DISMISS_CAPTAIN"
#define MSG_TEAM_DISMISS_MEMBER             "MSG_TEAM_DISMISS_MEMBER"
#define MSG_TEAM_KICK_ONE                   "MSG_TEAM_KICK_ONE"
#define MSG_TEAM_BE_KICKEN                  "MSG_TEAM_BE_KICKEN"
#define MSG_TEAM_APPLY_ADD                  "MSG_TEAM_APPLY_ADD"
#define MSG_TEAM_APPLY_ADD_SELF_MSG         "MSG_TEAM_APPLY_ADD_SELF_MSG"
#define MSG_TEAM_LEAVE                      "MSG_TEAM_LEAVE"
#define MSG_TEAM_LEAVE_SELF_MSG             "MSG_TEAM_LEAVE_SELF_MSG"
#define MSG_TEAM_CHANGE_CAPTAIN_FAIL1       "MSG_TEAM_CHANGE_CAPTAIN_FAIL1"
#define MSG_TEAM_CHANGE_CAPTAIN_FAIL2       "MSG_TEAM_CHANGE_CAPTAIN_FAIL2"
#define MSG_TEAM_CHANGE_CAPTAIN_FAIL3       "MSG_TEAM_CHANGE_CAPTAIN_FAIL3"
#define MSG_TEAM_CHANGE_CAPTAIN             "MSG_TEAM_CHANGE_CAPTAIN"
#define MSG_TEAM_CHANGE_CAPTAIN_SELF        "MSG_TEAM_CHANGE_CAPTAIN_SELF"

#define MSG_CHAT_APPLY_ADD_FRIEND           "MSG_CHAT_APPLY_ADD_FRIEND"
#define MSG_CHAT_FRIEND_HAD_IN              "MSG_CHAT_FRIEND_HAD_IN"
#define MSG_CHAT_GET_FRIEND_APPLY           "MSG_CHAT_GET_FRIEND_APPLY"
#define MSG_CHAT_REFUSE_FRIEND              "MSG_CHAT_REFUSE_FRIEND"
#define MSG_CHAT_ADD_FRIEND_FAIL            "MSG_CHAT_ADD_FRIEND_FAIL"
#define MSG_CHAT_ADD_FRIEND_SUCCESS         "MSG_CHAT_ADD_FRIEND_SUCCESS"
#define MSG_CHAT_CREATE_TEAM_FAIL1          "MSG_CHAT_CREATE_TEAM_FAIL1"
#define MSG_CHAT_CREATE_TEAM_FAIL2          "MSG_CHAT_CREATE_TEAM_FAIL2"
#define MSG_CHAT_CREATE_TEAM_FAIL3          "MSG_CHAT_CREATE_TEAM_FAIL3"
#define MSG_CHAT_RENAME_TEAM_FAIL           "MSG_CHAT_RENAME_TEAM_FAIL"
#define MSG_CHAT_DELETE_TEAM_FAIL1          "MSG_CHAT_DELETE_TEAM_FAIL1"
#define MSG_CHAT_DELETE_TEAM_FAIL2          "MSG_CHAT_DELETE_TEAM_FAIL2"
#define MSG_CHAT_FRIEND_ONLINE              "MSG_CHAT_FRIEND_ONLINE"
#define MSG_CHAT_FRIEND_OFFLINE             "MSG_CHAT_FRIEND_OFFLINE"
#define MSG_CHAT_DELETE_FRIEND              "MSG_CHAT_DELETE_FRIEND"
#define MSG_CHAT_DELETED_FRIEND             "MSG_CHAT_DELETED_FRIEND"
#define MSG_CHAT_MSG_FROM_FRIEND            "MSG_CHAT_MSG_FROM_FRIEND"
#define MSG_CHAT_FRIEND_NOT_NEAR            "MSG_CHAT_FRIEND_NOT_NEAR"
#define MSG_CHAT_FRIEND_NOT_ONLINE          "MSG_CHAT_FRIEND_NOT_ONLINE"
#define MSG_CHAT_TAR_REFUSE_SINGLE_TALK     "MSG_CHAT_TAR_REFUSE_SINGLE_TALK"

#define MSG_SHOP_NO_ROOM                    "MSG_SHOP_NO_ROOM"
#define MSG_SHOP_NO_MONEY                   "MSG_SHOP_NO_MONEY"
#define MSG_SHOP_YOU_MONEY                  "MSG_SHOP_YOU_MONEY"
#define MSG_SHOP_YOUS_MONEY                 "MSG_SHOP_YOUS_MONEY"
#define MSG_SHOP_YOUF_MONEY                 "MSG_SHOP_YOUF_MONEY"
#define MSG_HUANZHUANG_NO_ROOM              "MSG_HUANZHUANG_NO_ROOM"
#define MSG_SKILL_EXISTS_IN_IMMEDIA         "MSG_SKILL_EXISTS_IN_IMMEDIA"
#define MSG_SKILL_IS_NOT_IMMEDIA            "MSG_SKILL_IS_NOT_IMMEDIA"
#define MSG_ITEM_CANNOT_IN_IMMEDIATE        "MSG_ITEM_CANNOT_IN_IMMEDIATE"

#define MSG_NPC_NO_MANA                     "MSG_NPC_NO_MANA"
#define MSG_NPC_NO_STAMINA                  "MSG_NPC_NO_STAMINA"
#define MSG_NPC_NO_LIFE                     "MSG_NPC_NO_LIFE"
#define MSG_NPC_DEATH                       "MSG_NPC_DEATH"

#define MSG_OBJ_CANNOT_PICKUP               "MSG_OBJ_CANNOT_PICKUP"
#define MSG_MONEY_CANNOT_PICKUP             "MSG_MONEY_CANNOT_PICKUP"
#define MSG_OBJ_TOO_FAR                     "MSG_OBJ_TOO_FAR"
#define MSG_DEC_MONEY                       "MSG_DEC_MONEY"
#define MSG_EARN_MONEY                      "MSG_EARN_MONEY"
#define MSG_DEATH_LOSE_ITEM                 "MSG_DEATH_LOSE_ITEM"
#define MSG_ADD_ITEM                        "MSG_ADD_ITEM"
#define MSG_ITEM_SAME_DETAIL_IN_IMMEDIATE   "MSG_ITEM_SAME_DETAIL_IN_IMMEDIATE"
#define MSG_CAN_NOT_VIEW_ITEM               "MSG_CAN_NOT_VIEW_ITEM"
#define MSG_ITEM_DAMAGED                    "MSG_ITEM_DAMAGED"
#define MSG_ITEM_NEARLY_DAMAGED             "MSG_ITEM_NEARLY_DAMAGED"

#define MSG_TRADE_STATE_OPEN                "MSG_TRADE_STATE_OPEN"
#define MSG_TRADE_STATE_CLOSE               "MSG_TRADE_STATE_CLOSE"
#define MSG_TRADE_SELF_LOCK                 "MSG_TRADE_SELF_LOCK"
#define MSG_TRADE_SELF_UNLOCK               "MSG_TRADE_SELF_UNLOCK"
#define MSG_TRADE_DEST_LOCK                 "MSG_TRADE_DEST_LOCK"
#define MSG_TRADE_DEST_UNLOCK               "MSG_TRADE_DEST_UNLOCK"
#define MSG_TRADE_SUCCESS                   "MSG_TRADE_SUCCESS"
#define MSG_TRADE_FAIL                      "MSG_TRADE_FAIL"
#define MSG_TRADE_SELF_ROOM_FULL            "MSG_TRADE_SELF_ROOM_FULL"
#define MSG_TRADE_DEST_ROOM_FULL            "MSG_TRADE_DEST_ROOM_FULL"
#define MSG_TRADE_SEND_APPLY                "MSG_TRADE_SEND_APPLY"
#define MSG_TRADE_GET_APPLY                 "MSG_TRADE_GET_APPLY"
#define MSG_TRADE_REFUSE_APPLY              "MSG_TRADE_REFUSE_APPLY"
#define MSG_TRADE_TASK_ITEM                 "MSG_TRADE_TASK_ITEM"

#define G_ProtocolProcess_20                "G_ProtocolProcess_20"

#define MSG_PK_NORMAL_FLAG_OPEN             "MSG_PK_NORMAL_FLAG_1"
#define MSG_PK_NORMAL_FLAG_CLOSE            "MSG_PK_NORMAL_FLAG_0"
#define MSG_PK_VALUE                        "MSG_PK_VALUE"
#define MSG_PK_ERROR_1                      "MSG_PK_ERROR_1"
#define MSG_PK_ERROR_2                      "MSG_PK_ERROR_2"
#define MSG_PK_ERROR_3                      "MSG_PK_ERROR_3"
#define MSG_PK_ERROR_4                      "MSG_PK_ERROR_4"
#define MSG_PK_ERROR_5                      "MSG_PK_ERROR_5"
#define MSG_PK_ERROR_6                      "MSG_PK_ERROR_6"
#define MSG_PK_ERROR_7                      "MSG_PK_ERROR_7"
#define MSG_PK_ENMITY_SUCCESS_1             "MSG_PK_ENMITY_SUCCESS_1"
#define MSG_PK_ENMITY_SUCCESS_2             "MSG_PK_ENMITY_SUCCESS_2"
#define MSG_PK_ENMITY_CLOSE                 "MSG_PK_ENMITY_CLOSE"
#define MSG_PK_ENMITY_OPEN                  "MSG_PK_ENMITY_OPEN"

#define MSG_TONG_CREATE_ERROR01             "MSG_TONG_CREATE_ERROR01"
#define MSG_TONG_CREATE_ERROR06             "MSG_TONG_CREATE_ERROR06"
#define MSG_TONG_CREATE_ERROR05             "MSG_TONG_CREATE_ERROR05"
#define MSG_TONG_CREATE_ERROR04             "MSG_TONG_CREATE_ERROR04"
#define MSG_TONG_CREATE_ERROR07             "MSG_TONG_CREATE_ERROR07"
#define MSG_TONG_CREATE_ERROR08             "MSG_TONG_CREATE_ERROR08"
#define MSG_TONG_CREATE_ERROR09             "MSG_TONG_CREATE_ERROR09"
#define MSG_TONG_CREATE_ERROR10             "MSG_TONG_CREATE_ERROR10"
#define MSG_TONG_CREATE_ERROR11             "MSG_TONG_CREATE_ERROR11"
#define MSG_TONG_CREATE_ERROR12             "MSG_TONG_CREATE_ERROR12"
#define MSG_TONG_CREATE_ERROR13             "MSG_TONG_CREATE_ERROR13"
#define MSG_TONG_APPLY_CREATE               "MSG_TONG_APPLY_CREATE"
#define MSG_TONG_CREATE_SUCCESS             "MSG_TONG_CREATE_SUCCESS"
#define MSG_TONG_APPLY_ADD                  "MSG_TONG_APPLY_ADD"
#define MSG_TONG_APPLY_ADD_ERROR1           "MSG_TONG_APPLY_ADD_ERROR1"
#define MSG_TONG_APPLY_ADD_ERROR2           "MSG_TONG_APPLY_ADD_ERROR2"
#define MSG_TONG_APPLY_ADD_ERROR3           "MSG_TONG_APPLY_ADD_ERROR3"
#define MSG_TONG_REFUSE_ADD                 "MSG_TONG_REFUSE_ADD"
#define MSG_TONG_ADD_SUCCESS                "MSG_TONG_ADD_SUCCESS"
#define MSG_TONG_CANNOT_LEAVE1              "MSG_TONG_CANNOT_LEAVE1"
#define MSG_TONG_CANNOT_LEAVE2              "MSG_TONG_CANNOT_LEAVE2"
#define MSG_TONG_BE_KICKED                  "MSG_TONG_BE_KICKED"
#define MSG_TONG_LEAVE_SUCCESS              "MSG_TONG_LEAVE_SUCCESS"
#define MSG_TONG_LEAVE_FAIL                 "MSG_TONG_LEAVE_FAIL"
#define MSG_TONG_CHANGE_AS_MASTER           "MSG_TONG_CHANGE_AS_MASTER"
#define MSG_TONG_CHANGE_AS_MEMBER           "MSG_TONG_CHANGE_AS_MEMBER"

#define MSG_EARN_XU                         "MSG_EARN_XU"
#define MSG_PK_NORMAL_FLAG_DS               "MSG_PK_NORMAL_FLAG_2"
#define MSG_NPC_CANNOT_RIDE                 "MSG_NPC_CANNOT_RIDE"
#define MSG_SUNYI_FAILED                    "MSG_SUNYI_FAILED"
#define MSG_SUNYI_SUCCESS                   "MSG_SUNYI_SUCCESS"

#define MSG_GUAJI_SUCCESS                   "MSG_GUAJI_SUCCESS"
#define MSG_COMP_FAILED                     "MSG_COMP_FAILED"
#define MSG_COMP_SUCCESS                    "MSG_COMP_SUCCESS"
#define MSG_COMP_ERITEM                     "MSG_COMP_ERITEM"
#define MSG_COMP_MONEY                      "MSG_COMP_MONEY"
#define MSG_ID_COMP_NO                      "MSG_ID_COMP_NO"
#define MSG_SHOP_NO_XU                      "MSG_SHOP_NO_XU"
#define MSG_SHOP_NO_FUYUAN                  "MSG_SHOP_NO_FUYUAN"
#define MSG_SHOP_NO_SHENGWANG               "MSG_SHOP_NO_SHENGWANG"
#define MSG_SHOP_NO_DIANKA                  "MSG_SHOP_NO_DIANKA"
#define MSG_SHOP_NO_JIFEN                   "MSG_SHOP_NO_JIFEN"
#define MSG_SHOP_YOU_XU                     "MSG_SHOP_YOU_XU"
#define MSG_SHOP_YOUS_XU                    "MSG_SHOP_YOUS_XU"
#define MSG_SHOP_YOUF_XU                    "MSG_SHOP_YOUF_XU"
#define MSG_DROP_DEST_FAILED                "MSG_DROP_DEST_FAILED"
#define MSG_ID_LIANJIE_FAILED               "MSG_ID_LIANJIE_FAILED"
#define MSG_TEAM_CREAT_FRIEND               "MSG_TEAM_CREAT_FRIEND"
#define MSG_ID_SHUA_SHUA                    "MSG_ID_SHUA_SHUA"
#define MSG_CAN_NOT_PK_SUNYI                "MSG_CAN_NOT_PK_SUNYI"
#define MSG_ID_FUHUO_YUANDIAN               "MSG_ID_FUHUO_YUANDIAN"
#define MSG_SKILL_CANNOT_PICKUP             "MSG_SKILL_CANNOT_PICKUP"

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
    enumMSG_ID_RIDE_CANNOT,
    enumMSG_ID_COMP_FAILED,
    enumMSG_ID_COMP_SUCCESS,
    enumMSG_ID_COMP_ERITEM,
    enumMSG_ID_SUNYI_FAILED,
    enumMSG_ID_GUAJI_SUCCESS,
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
    enumMSG_ID_LIANJIE_FAILED,
    enumMSG_ID_SHUA_SHUA,
    enumMSG_ID_RENOVE_TIME_ITEM,
    enumMSG_ID_FUHUO_FAILED,
    enumMSG_ID_ERROR_INFO,
    enumTONG_COMMAND_ID_APPLY_SAVE,
    enumTONG_COMMAND_ID_APPLY_GET,
    enumTONG_COMMAND_ID_APPLY_SND,
    enumMSG_ID_COMP_MONEY,
    enumMSG_ID_COMP_NO,
    enumMSG_ID_NUM,
};

#define MOUSE_CURSOR_NORMAL 0
#define MOUSE_CURSOR_FIGHT  1
#define MOUSE_CURSOR_DIALOG 2
#define MOUSE_CURSOR_PICK   3
#define MOUSE_CURSOR_USE    8

#endif
