
#ifndef COREUSENAMEDEF_H
#define COREUSENAMEDEF_H

#define MAX_PLAYER_IN_ACCOUNT 3  // ½ÇÉ«ÊýÁ¿

// #define		SETTING_PATH					"\\settings"

#define PLAYER_MENU_STATE_RES_FILE \
    "\\settings\\npcres\\½çÃæ×´Ì¬ÓëÍ¼ÐÎ¶ÔÕÕ±í.txt"  // Ui_Stateandimgtable.txt"//½çÃæ×´Ì¬ÓëÍ¼ÐÎ¶ÔÕÕ±í.txt"
#define NPC_RES_KIND_FILE_NAME "\\settings\\npcres\\ÈËÎïÀàÐÍ.txt"  // PlayerType.txt" //ÈËÎïÀàÐÍ.txt"

//---------------------------- npc res Ïà¹Ø ------------------------------
#ifndef _SERVER

#    define RES_INI_FILE_PATH       "\\settings\\npcres"

#    define RES_SOUND_FILE_PATH     "sound"

#    define NPC_NORMAL_RES_FILE     "\\settings\\npcres\\ÆÕÍ¨npc×ÊÔ´.txt"  // Nor_Npcres.txt"//ÆÕÍ¨npc×ÊÔ´.txt"
#    define NPC_NORMAL_SPRINFO_FILE "\\settings\\npcres\\ÆÕÍ¨npc×ÊÔ´ÐÅÏ¢.txt"  // Nor_Npcres_info.txt" //ÆÕÍ¨npc×ÊÔ´ÐÅÏ¢
#    define STATE_MAGIC_TABLE_NAME  "\\settings\\npcres\\×´Ì¬Í¼ÐÎ¶ÔÕÕ±í.txt"  // StateImgtable.txt"//×´Ì¬Í¼ÐÎ¶ÔÕÕ±í.txt"
#    define PLAYER_RES_SHADOW_FILE \
        "\\settings\\npcres\\Ö÷½Ç¶¯×÷ÒõÓ°¶ÔÓ¦±í.txt"  // roleactshadowtable.txt"//Ö÷½Ç¶¯×÷ÒõÓ°¶ÔÓ¦±í.txt"

#    define PLAYER_SOUND_FILE           "\\settings\\npcres\\Ö÷½Ç¶¯×÷ÉùÒô±í.txt"  // roleactvoicetable.txt"//Ö÷½Ç¶¯×÷ÉùÒô±í.txt"
#    define NPC_SOUND_FILE              "\\settings\\npcres\\npc¶¯×÷ÉùÒô±í.txt"  // npcactvoicetable.txt"//npc¶¯×÷ÉùÒô±í.txt"

#    define NPC_ACTION_NAME             "npc¶¯×÷±í.txt"   //"npcacttable.txt"//"npc¶¯×÷±í.txt"
#    define ACTION_FILE_NAME            "¶¯×÷±àºÅ±í.txt"  //"actnumbertable.txt"//"¶¯×÷±àºÅ±í.txt"

#    define PLAYER_INSTANT_SPECIAL_FILE "\\settings\\npcres\\Ë²¼äÌØÐ§.txt"  // one_effects.txt"//Ë²¼äÌØÐ§.txt"

#    define SPR_INFO_NAME               "ÐÅÏ¢"                 //"info"        //ÐÅÏ¢
#    define KIND_NAME_SECT              "ÈËÎïÀàÐÍ"             //"roletype"    //"ÈËÎïÀàÐÍ"
#    define KIND_NAME_SPECIAL           "SpecialNpc"           // ÌØÊânpc
#    define KIND_NAME_NORMAL            "NormalNpc"            // ÆÕÍ¨npc
#    define KIND_FILE_SECT1             "²¿¼þËµÃ÷ÎÄ¼þÃû"       //"bujianfilename"//"²¿¼þËµÃ÷ÎÄ¼þÃû"
#    define KIND_FILE_SECT2             "wuqiactconnettable1"  //"wuqiactconnettable1"//"ÎäÆ÷ÐÐÎª¹ØÁª±í1"
#    define KIND_FILE_SECT3             "wuqiactconnettable2"  //"wuqiactconnettable2"//"ÎäÆ÷ÐÐÎª¹ØÁª±í2"
#    define KIND_FILE_SECT4             "¶¯×÷ÌùÍ¼Ë³Ðò±í"       //"actimgordertable"//"¶¯×÷ÌùÍ¼Ë³Ðò±í"
#    define KIND_FILE_SECT5             "×ÊÔ´ÎÄ¼þÂ·¾­"         //"resfilepath"//"×ÊÔ´ÎÄ¼þÂ·¾­"
//---------------------------- Ð¡µØÍ¼Ïà¹Ø -------------------------------
#    define defLITTLE_MAP_SET_FILE "\\Ui\\Default\\Ð¡µØÍ¼ÑÕÉ«.ini"  // minimapcolor.ini" //"Ð¡µØÍ¼ÑÕÉ«"
//----------------------------- ÁÄÌìÏà¹Ø ------------------------------
#    define CHAT_PATH                   "\\chat"
#    define CHAT_TEAM_INFO_FILE_NAME    "Team.cht"
#    define CHAT_CHANNEL_INFO_FILE_NAME "Channel.cht"
//----------------------------- ÉùÒôÏà¹Ø --------------------------------
#    define defINSTANT_SOUND_FILE               "\\settings\\SoundList.txt"
#    define defMUSIC_SET_FILE                   "\\settings\\music\\MusicSet.txt"
#    define defMUSIC_FIGHT_SET_FILE             "\\settings\\music\\MusicFightSet.ini"

#    define OBJ_NAME_COLOR_FILE                 "\\settings\\obj\\ObjNameColor.ini"  // ÎïÆ·ÑÕÉ«ÉèÖÃ

#    define STRINGRESOURSE_TABFILE              "\\settings\\StringResource.txt"
#    define PLAYER_RANK_SETTING_TABFILE         "\\settings\\RankSetting.txt"

#    define CHAT_CHANNEL_NAME_ALL               "ËùÓÐÍæ¼Ò"
#    define CHAT_CHANNEL_NAME_SCREEN            "¸½½üÍæ¼Ò"
#    define CHAT_CHANNEL_NAME_SINGLE            "ºÃÓÑ"
#    define CHAT_CHANNEL_NAME_TEAM              "¶ÓÓÑ"
#    define CHAT_CHANNEL_NAME_FACTION           "Í¬ÃÅ"
#    define CHAT_CHANNEL_NAME_TONG              "°ïÖÚ"
#    define CHAT_CHANNEL_NAME_SCREENSINGLE      "Ä°ÉúÈË"
#    define CHAT_CHANNEL_NAME_SYSTEM            "ÏµÍ³"

#    define CHAT_TAKE_CHANNEL_NAME_TEAM         "¶ÓÎéÆµµÀ"
#    define CHAT_TAKE_CHANNEL_NAME_FACTION      "ÃÅÅÉÆµµÀ"
#    define CHAT_TAKE_CHANNEL_NAME_TONG         "°ï»áÆµµÀ"
#    define CHAT_TAKE_CHANNEL_NAME_SCREENSINGLE "Ë½ÁÄÆµµÀ"

#endif
//--------------------------- player ÃÅÅÉÏà¹Ø ------------------------------
// ×¢£ºÏÂÃæÕâ¸öÉè¶¨ÎÄ¼þ±ØÐë°´ ½ð ½ð Ä¾ Ä¾ Ë® Ë® »ð »ð ÍÁ ÍÁ µÄË³ÐòÅÅÁÐ
#define FACTION_FILE "\\settings\\faction\\MonPhai.ini"
//--------------------------- player °ï»áÏà¹Ø ------------------------------
#define defPLAYER_TONG_PARAM_FILE "\\settings\\tong\\TongSet.ini"

//---------------------------- player ÊýÖµÏà¹Ø ------------------------------
#define PLAYER_LEVEL_EXP_FILE      "\\settings\\npc\\player\\level_exp.txt"
#define PLAYER_LEVEL_ADD_FILE      "\\settings\\npc\\player\\level_add.txt"
#define PLAYER_LEVEL_LEAD_EXP_FILE "\\settings\\npc\\player\\level_lead_exp.txt"
#define BASE_ATTRIBUTE_FILE_NAME   "\\settings\\npc\\player\\NewPlayerBaseAttribute.ini"
#define PLAYER_PK_RATE_FILE        "\\settings\\npc\\PKRate.ini"
#define PLAYER_BASE_VALUE          "\\settings\\npc\\player\\BaseValue.ini"
#define NPC_LEVELSCRIPT_FILENAME   "\\script\\npclevelscript\\npclevelscript.lua"

// 00ÄÐ½ð 01Å®½ð 02ÄÐÄ¾ 03Å®Ä¾ 04ÄÐË® 05Å®Ë® 06ÄÐ»ð 07Å®»ð 08ÄÐÍÁ 09Å®ÍÁ
// #define		NEW_PLAYER_INI_FILE_NAME		"\\settings\\player\\NewPlayerIni%02d.ini"

//---------------------------- player ½»Ò×Ïà¹Ø ------------------------------
#define BUYSELL_FILE "\\settings\\buysella.txt"
#define GOODS_FILE   "\\settings\\goodsa.txt"

//----------------------------- ÎïÆ·Íâ¹ÛÏà¹Ø ------------------------------
#define CHANGERES_MELEE_FILE   "\\settings\\item\\MeleeRes.txt"
#define CHANGERES_RANGE_FILE   "\\settings\\item\\RangeRes.txt"
#define CHANGERES_ARMOR_FILE   "\\settings\\item\\ArmorRes.txt"
#define CHANGERES_HELM_FILE    "\\settings\\item\\HelmRes.txt"
#define CHANGERES_HORSE_FILE   "\\settings\\item\\HorseRes.txt"
#define CHANCERES_GOLD_FILE    "\\settings\\item\\GolditemRes.txt"
#define CHANCERES_PLAT_FILE    "\\settings\\item\\PlatinaequipRes.txt"

#define ITEM_ABRADE_FILE       "\\settings\\item\\AbradeRate.ini"
#define CHANGERES_PIFENG_FILE  "\\settings\\item\\PiFengRes.txt"
#define CHANGERES_CHIBANG_FILE "\\settings\\item\\ChiBangRes.txt"
//--------------- npc skill missles Éè¶¨ÎÄ¼þ£¬ÓÃÓÚÉú³ÉÄ£°å -------------
#define SKILL_SETTING_FILE     "\\settings\\Skills.txt"
#define MISSLES_SETTING_FILE   "\\settings\\Missles.txt"
#define NPC_SETTING_FILE       "\\settings\\NpcS.txt"
#define NPC_GOLD_TEMPLATE_FILE "\\settings\\npc\\NpcGoldTemplate.txt"

//---------------------------- object Ïà¹Ø ------------------------------
#define OBJ_DATA_FILE_NAME  "\\settings\\obj\\ObjData.txt"
#define MONEY_OBJ_FILE_NAME "\\settings\\obj\\MoneyObj.txt"

//------------------------------------------------------------------------

// #define NPC_TEMPLATE_BINFILEPATH		"\\settings"

#define WEAPON_PHYSICSSKILLFILE "\\settings\\ÎäÆ÷ÎïÀí¹¥»÷¶ÔÕÕ±í.txt"  // wuqiattacktabel.txt"//ÎäÆ÷ÎïÀí¹¥»÷¶ÔÕÕ±í.txt"
#define WEAPON_PARTICULARTYPE   "DetailType"                          //"ÏêÏ¸Àà±ð"
#define WEAPON_DETAILTYPE       "ParticularType"                      // ¾ßÌåÀà±ð"
#define WEAPON_SKILLID          "¶ÔÓ¦ÎïÀí¼¼ÄÜ±àºÅ"  //"physicskillnumber"//"¶ÔÓ¦ÎïÀí¼¼ÄÜ±àºÅ"

//---------------------------------ÈÎÎñ--------------------------

// #define TASK_CONFIG_FILE                "\\script\\task\\tasklist.ini"
#define MAP_DROPRATE_FILE      "\\Settings\\mapList.ini"
#define GAME_SETTING_FILE      "\\Settings\\gamesetting.ini"

#define GAME_FSJINMAI_FILE     "\\Settings\\meridian\\meridian_level.txt"  //"\\Settings\\FsJinMai\\fsjinmai.txt"
#define GAME_FORBITMAP_FILE    "\\Settings\\ForbititMap.txt"
#define GAME_FORBITWAIGUA_FILE "\\Settings\\ForbititWaiGua.txt"
// #define GAME_ZHUCE_FILE                 "\\×¢²áÐÅÏ¢.ini"

#define GAME_ATLAS_FILE              "\\Settings\\item\\atlas_compound.txt"  // Í¼Æ×ÎÄ¼þ
#define GAME_COMP_FILE               "\\Settings\\item\\other_compound.txt"  // ÆäËû²ÄÁÏ
#define GAME_MSG_FILE                "\\Settings\\EducationMessage.txt"      // ½ÌÓýÐÅÏ¢

#define MESSAGE_SYSTEM_ANNOUCE_HEAD  "MESSAGE_SYSTEM_ANNOUCE_HEAD"   //"¹«¸æ"
#define MESSAGE_SYSTEM_WAIGUA_HEAD   "MESSAGE_SYSTEM_WAIGUA_HEAD"    //"Íâ¹Ò"
#define MESSAGE_SYSTEM_TONGZHI_HEAD  "MESSAGE_SYSTEM_TONGZHI_HEAD"   //"Í¨Öª"
#define MESSAGE_SYSTEM_DUCHUAN_HEAD  "MESSAGE_SYSTEM_DUCHUAN_HEAD"   //"Õ½³¡"
#define MESSAGE_SYSTEM_LIANJIE_HEAD  "MESSAGE_SYSTEM_LIANJIE_HEAD"   //"Á´½Ó"
#define MESSAGE_SYSTEM_PAIMING_HEAD  "MESSAGE_SYSTEM_PAIMING_HEAD"   //"<bclr=blue><color=white>ÅÅÃû<color><bclr>"
#define MESSAGE_SYSTEM_BANGZHU_HEAD  "MESSAGE_SYSTEM_BANGZHU_HEAD"   //"<bclr=yellow><color=red>°ïÖ÷<color><bclr>"
#define MESSAGE_SYSTEM_CITYHEAR_HEAD "MESSAGE_SYSTEM_CITYHEAR_HEAD"  //"<bclr=red><color=yellow>³ÇÖ÷<color><bclr>"

#define L_NPC_15                     "L_NPC_15"        //[error]ÖÂÃü´íÎó,ÎÞ·¨ÕýÈ·¶ÁÈ¡%s
#define L_SortScript_4               "L_SortScript_4"  //-------½Å±¾²»´æÔÚ,Ö´ÐÐ[%s][%s]Ê§°Ü!-------- \n
#define L_SortScript_5               "L_SortScript_5"
// ÌØ±ð×¢Òâ£ºÒÔÏÂ×Ö·û´®³¤¶È²»ÄÜ³¬¹ý32×Ö½Ú£¬°üÀ¨ %d %s µÈ½ÓÊÕ¾ßÌåÄÚÈÝÒÔºóµÄ³¤¶È
#ifndef _SERVER  // ¿Í»§¶Ë
#    define MSG_GET_EXP                     "MSG_GET_EXP"
#    define MSG_DEC_EXP                     "MSG_DEC_EXP"  // Ã»ÓÃ
#    define MSG_LEVEL_UP                    "MSG_LEVEL_UP"
#    define MSG_LEADER_LEVEL_UP             "MSG_LEADER_LEVEL_UP"
#    define MSG_GET_ATTRIBUTE_POINT         "MSG_GET_ATTRIBUTE_POINT"
#    define MSG_GET_SKILL_POINT             "MSG_GET_SKILL_POINT"
#    define MSG_GET_ATTRIBUTE_SKILL_POINT   "MSG_GET_ATTRIBUTE_SKILL_POINT"  // Ã»ÓÃ

#    define MSG_TEAM_AUTO_REFUSE_INVITE     "MSG_TEAM_AUTO_REFUSE_INVITE"
#    define MSG_TEAM_NOT_AUTO_REFUSE_INVITE "MSG_TEAM_NOT_AUTO_REFUSE_INVITE"  //"ÌáÊ¾:ÏÔÊ¾ËûÈË¶ÔÄúµÄ×é¶ÓÑûÇë¡£"
#    define MSG_TEAM_SEND_INVITE            "MSG_TEAM_SEND_INVITE"  //"ÌáÊ¾:ÄúÏò(%s)·¢³ö×é¶ÓÑûÇë!"
#    define MSG_TEAM_GET_INVITE             "MSG_TEAM_GET_INVITE"   //"ÌáÊ¾:(%s)ÑûÇëÄú×é¶Ó!"
#    define MSG_TEAM_REFUSE_INVITE          "MSG_TEAM_REFUSE_INVITE"  //"ÌáÊ¾:(%s)¾Ü¾øÁËÄúµÄ×é¶ÓÑûÇë!"  //Ã»ÓÃ
#    define MSG_TEAM_CREATE                 "MSG_TEAM_CREATE"         //"ÌáÊ¾:Äú´´½¨ÁËÒ»Ö§¶ÓÎé¡£"
#    define MSG_TEAM_CREATE_FAIL            "MSG_TEAM_CREATE_FAIL"    //"ÌáÊ¾:¶ÓÎé´´½¨Ê§°Ü¡£"
#    define MSG_TEAM_CANNOT_CREATE          "MSG_TEAM_CANNOT_CREATE"  //"ÌáÊ¾:ÄúÏÖÔÚ²»ÄÜ×é¶Ó!"
#    define MSG_TEAM_TARGET_CANNOT_ADD_TEAM "MSG_TEAM_TARGET_CANNOT_ADD_TEAM"  //"ÌáÊ¾:¶Ô·½ÏÖÔÚ²»ÄÜ×é¶Ó!"
#    define MSG_TEAM_OPEN                   "MSG_TEAM_OPEN"  //"ÌáÊ¾:ÄúµÄ¶ÓÎéÏÖÔÚÔÊÐí½ÓÊÕÐÂ¶ÓÔ±¡£"
#    define MSG_TEAM_CLOSE                  "MSG_TEAM_CLOSE"  //"ÌáÊ¾:ÄúµÄ¶ÓÎéÏÖÔÚ²»ÔÊÐí½ÓÊÕÐÂ¶ÓÔ±¡£"
#    define MSG_TEAM_ADD_MEMBER             "MSG_TEAM_ADD_MEMBER"  //"ÌáÊ¾:(%s)³ÉÎªÄúµÄ¶ÓÓÑÁË¡£"
#    define MSG_TEAM_SELF_ADD               "MSG_TEAM_SELF_ADD"    //"ÌáÊ¾:Äú¼ÓÈë(%s)µÄ¶ÓÎé¡£"
#    define MSG_TEAM_DISMISS_CAPTAIN        "MSG_TEAM_DISMISS_CAPTAIN"  //"ÌáÊ¾:Äú½âÉ¢ÁË×Ô¼ºµÄ¶ÓÎé!"
#    define MSG_TEAM_DISMISS_MEMBER         "MSG_TEAM_DISMISS_MEMBER"  //"ÌáÊ¾:(%s)½âÉ¢ÁË¶ÓÎé!"
#    define MSG_TEAM_KICK_ONE               "MSG_TEAM_KICK_ONE"        //"ÌáÊ¾:(%s)±»¿ª³ý³ö¶ÓÎé!"
#    define MSG_TEAM_BE_KICKEN              "MSG_TEAM_BE_KICKEN"       //"ÌáÊ¾:Äú±»¿ª³ý³ö¶ÓÎé!"
#    define MSG_TEAM_APPLY_ADD              "MSG_TEAM_APPLY_ADD"       //"ÌáÊ¾:(%s)ÉêÇë¼ÓÈë¶ÓÎé!"
#    define MSG_TEAM_APPLY_ADD_SELF_MSG     "MSG_TEAM_APPLY_ADD_SELF_MSG"  //"ÌáÊ¾:ÄúÉêÇë¼ÓÈë(%s)µÄ¶ÓÎé!"
#    define MSG_TEAM_LEAVE                  "MSG_TEAM_LEAVE"               //"ÌáÊ¾:(%s)Àë¿ª¶ÓÎé¡£"
#    define MSG_TEAM_LEAVE_SELF_MSG         "MSG_TEAM_LEAVE_SELF_MSG"  //"ÌáÊ¾:ÄúÀë¿ª(%s)µÄ¶ÓÎé¡£"
#    define MSG_TEAM_CHANGE_CAPTAIN_FAIL1   "MSG_TEAM_CHANGE_CAPTAIN_FAIL1"  //"ÌáÊ¾:¶Ó³¤ÈÎÃüÊ§°Ü!"
#    define MSG_TEAM_CHANGE_CAPTAIN_FAIL2   "MSG_TEAM_CHANGE_CAPTAIN_FAIL2"  //"ÌáÊ¾:(%s)Í³Ë§Á¦²»¹»!"
#    define MSG_TEAM_CHANGE_CAPTAIN_FAIL3   "MSG_TEAM_CHANGE_CAPTAIN_FAIL3"  //"ÌáÊ¾:ÄúµÄ¶ÓÎé²»ÄÜÒÆ½»¸øÐÂÊÖ!"
#    define MSG_TEAM_CHANGE_CAPTAIN         "MSG_TEAM_CHANGE_CAPTAIN"       //"ÌáÊ¾:(%s)±»ÈÎÃüÎª¶Ó³¤!"
#    define MSG_TEAM_CHANGE_CAPTAIN_SELF    "MSG_TEAM_CHANGE_CAPTAIN_SELF"  //"ÌáÊ¾:Äú±»(%s)ÈÎÃüÎª¶Ó³¤!"

#    define MSG_CHAT_APPLY_ADD_FRIEND       "MSG_CHAT_APPLY_ADD_FRIEND"  //"ÌáÊ¾:ÄúÏò(%s)ÉêÇëºÃÓÑ¡£"
#    define MSG_CHAT_FRIEND_HAD_IN          "MSG_CHAT_FRIEND_HAD_IN"     //"ÌáÊ¾:(%s)ÒÑ±»¼ÓÎªºÃÓÑ¡£"
#    define MSG_CHAT_GET_FRIEND_APPLY       "MSG_CHAT_GET_FRIEND_APPLY"  //"ÌáÊ¾:(%s)ÉêÇë¼ÓÎªºÃÓÑ!"
#    define MSG_CHAT_REFUSE_FRIEND          "MSG_CHAT_REFUSE_FRIEND"     //"ÌáÊ¾:(%s)¾Ü¾øºÃÓÑÉêÇë!"
#    define MSG_CHAT_ADD_FRIEND_FAIL        "MSG_CHAT_ADD_FRIEND_FAIL"   //"ÌáÊ¾:Ìí¼Ó(%s)ÎªºÃÓÑÊ§°Ü!"
#    define MSG_CHAT_ADD_FRIEND_SUCCESS     "MSG_CHAT_ADD_FRIEND_SUCCESS"  //"ÌáÊ¾:ÄúÓë(%s)³ÉÎªºÃÓÑ!"
#    define MSG_CHAT_CREATE_TEAM_FAIL1      "MSG_CHAT_CREATE_TEAM_FAIL1"  //"ÌáÊ¾:ºÃÓÑ·Ö×é´´½¨Ê§°Ü!"
#    define MSG_CHAT_CREATE_TEAM_FAIL2      "MSG_CHAT_CREATE_TEAM_FAIL2"  //"ÌáÊ¾:ÒÑ´æÔÚÍ¬Ãû×é!"
#    define MSG_CHAT_CREATE_TEAM_FAIL3      "MSG_CHAT_CREATE_TEAM_FAIL3"  //"ÌáÊ¾:ÒÑ´ï×î´ó·Ö×éÊý!"
#    define MSG_CHAT_RENAME_TEAM_FAIL       "MSG_CHAT_RENAME_TEAM_FAIL"   //"ÌáÊ¾:×éÃûÐÞ¸ÄÊ§°Ü!"
#    define MSG_CHAT_DELETE_TEAM_FAIL1      "MSG_CHAT_DELETE_TEAM_FAIL1"  //"ÌáÊ¾:×éÉ¾³ýÊ§°Ü!"
#    define MSG_CHAT_DELETE_TEAM_FAIL2      "MSG_CHAT_DELETE_TEAM_FAIL2"  //"ÌáÊ¾:Õâ¸ö×é²»ÔÊÐíÉ¾³ý!"
#    define MSG_CHAT_FRIEND_ONLINE          "MSG_CHAT_FRIEND_ONLINE"      //"ÌáÊ¾:(%s)ÉÏÏßÁË!"
#    define MSG_CHAT_FRIEND_OFFLINE         "MSG_CHAT_FRIEND_OFFLINE"     //"ÌáÊ¾:(%s)ÏÂÏßÁË!"
#    define MSG_CHAT_DELETE_FRIEND          "MSG_CHAT_DELETE_FRIEND"  //"ÌáÊ¾:ÄúÓë(%s)½â³ýÁËºÃÓÑ¹ØÏµ!"
#    define MSG_CHAT_DELETED_FRIEND         "MSG_CHAT_DELETED_FRIEND"  //"ÌáÊ¾:(%s)ÓëÄú½â³ýÁËºÃÓÑ¹ØÏµ!"
#    define MSG_CHAT_MSG_FROM_FRIEND        "MSG_CHAT_MSG_FROM_FRIEND"    //"ÌáÊ¾:(%s)·¢À´ÏûÏ¢!"
#    define MSG_CHAT_FRIEND_NOT_NEAR        "MSG_CHAT_FRIEND_NOT_NEAR"    //"ÌáÊ¾:(%s)²»ÔÚ¸½½ü!"
#    define MSG_CHAT_FRIEND_NOT_ONLINE      "MSG_CHAT_FRIEND_NOT_ONLINE"  //"ÌáÊ¾:(%s)²»ÔÚÏß!"
#    define MSG_CHAT_TAR_REFUSE_SINGLE_TALK "MSG_CHAT_TAR_REFUSE_SINGLE_TALK"  //"ÌáÊ¾:(%s)Ã»ÓÐ¶©ÔÄË½ÁÄÆµµÀ!"

#    define MSG_SHOP_NO_ROOM                "MSG_SHOP_NO_ROOM"  //"ÌáÊ¾:±³°ü¿Õ¼ä²»×ã!"
#    define MSG_SHOP_NO_MONEY               "MSG_SHOP_NO_MONEY"  //"ÌáÊ¾C:Áí¼Ó<color=green>Ë°ÊÕ(%d%s)<color>,½ðÇ®²»×ã!"
#    define MSG_SHOP_YOU_MONEY              "MSG_SHOP_YOU_MONEY"  //"ÌáÊ¾:Õ¼°ï³ÉÔ±ÓÅ»Ý<color=green>(%d%s)<color>,½ðÇ®²»×ã!"
#    define MSG_SHOP_YOUS_MONEY \
        "MSG_SHOP_YOUS_MONEY"  //"ÌáÊ¾:Õ¼°ï³ÉÔ±ÓÅ»Ý<color=green>(%d%s)<color>,»¨·ÑÁË<color=green>(%d)<color>Á½!"
#    define MSG_SHOP_YOUF_MONEY \
        "MSG_SHOP_YOUF_MONEY"  //"ÌáÊ¾:Áí¼Ó³ÇÊÐ<color=green>Ë°ÊÕ(%d%s)<color>,»¨·ÑÁË<color=green>(%d)<color>Á½!"
#    define MSG_HUANZHUANG_NO_ROOM            "MSG_HUANZHUANG_NO_ROOM"  //"ÌáÊ¾:<color=yellow>ÓÒ¼ü»»×°¹¦ÄÜ¼´½«ÍÆ³ö!<color>"
#    define MSG_SKILL_EXISTS_IN_IMMEDIA       "MSG_SKILL_EXISTS_IN_IMMEDIA"  // ¿ì½ÝÀ¸ÉÏÒÑ¾­ÓÐ¸Ã¼¼ÄÜÁË
#    define MSG_SKILL_IS_NOT_IMMEDIA          "MSG_SKILL_IS_NOT_IMMEDIA"  // ¸Ã¼¼ÄÜ²»ÊÇ¿ì½Ý¼¼ÄÜ£¬²»ÄÜ·Åµ½¿ì½ÝÀ¸ÉÏ
#    define MSG_ITEM_CANNOT_IN_IMMEDIATE      "MSG_ITEM_CANNOT_IN_IMMEDIATE"  // ¸ÃÀàÎïÆ·²»ÄÜ·Åµ½¿ì½ÝÀ¸ÉÏ

#    define MSG_NPC_NO_MANA                   "MSG_NPC_NO_MANA"     //"ÌáÊ¾:ÄÚÁ¦²»×ã!"
#    define MSG_NPC_NO_STAMINA                "MSG_NPC_NO_STAMINA"  //"ÌáÊ¾:ÌåÁ¦²»×ã!"
#    define MSG_NPC_NO_LIFE                   "MSG_NPC_NO_LIFE"     //"ÌáÊ¾:ÉúÃü²»×ã!"
#    define MSG_NPC_DEATH                     "MSG_NPC_DEATH"  //"ÌáÊ¾:Ð¯´ø<color=green>¸´»îÓ¡¼ø<color>¼´¿ÉÔ­µØÖØÉú!"

#    define MSG_OBJ_CANNOT_PICKUP             "MSG_OBJ_CANNOT_PICKUP"  //"ÌáÊ¾:Äú²»ÄÜÊ°È¡±ðÈËµÄÎïÆ·!"
#    define MSG_MONEY_CANNOT_PICKUP           "MSG_MONEY_CANNOT_PICKUP"  //"ÌáÊ¾:Äú²»ÄÜÊ°È¡±ðÈËµÄÇ®!"
#    define MSG_OBJ_TOO_FAR                   "MSG_OBJ_TOO_FAR"      //"ÌáÊ¾:ÎïÆ·Ì«Ô¶£¬ÎÞ·¨Ê°È¡!"
#    define MSG_DEC_MONEY                     "MSG_DEC_MONEY"        //"ÌáÊ¾:ÄúËðÊ§ÁË(%d)Á½Òø×Ó!"
#    define MSG_EARN_MONEY                    "MSG_EARN_MONEY"       //"ÌáÊ¾:Äú»ñµÃÁË(%d)Á½Òø×Ó!"
#    define MSG_DEATH_LOSE_ITEM               "MSG_DEATH_LOSE_ITEM"  //"ÌáÊ¾:Äú¶ªÊ§ÁËÎïÆ·%s!"
#    define MSG_ADD_ITEM                      "MSG_ADD_ITEM"         //"ÌáÊ¾:Äú»ñµÃÁË(%s)!"
#    define MSG_ITEM_SAME_DETAIL_IN_IMMEDIATE "MSG_ITEM_SAME_DETAIL_IN_IMMEDIATE"  //"ÌáÊ¾:¿ì½ÝÀ¸ÒÑ¾­ÓÐÍ¬ÀàÐÍµÄÎïÆ·ÁË!"
#    define MSG_CAN_NOT_VIEW_ITEM             "MSG_CAN_NOT_VIEW_ITEM"  //"ÌáÊ¾:ÏÖÔÚ²»ÄÜ²ì¿´ËûÈË×°±¸!"//Ã»ÓÃ
#    define MSG_ITEM_DAMAGED                  "MSG_ITEM_DAMAGED"  //"ÌáÊ¾:Äú×°±¸µÄ(%s)³¤ÆÚÄ¥Ëð£¬ÒÑ¾­³¹µ×Ëð»µ!"
#    define MSG_ITEM_NEARLY_DAMAGED           "MSG_ITEM_NEARLY_DAMAGED"  //"ÌáÊ¾:Äú×°±¸ÒÑ¾­½Ó½üËð»µ,Çë¼°Ê±ÐÞÀí!"

#    define MSG_TRADE_STATE_OPEN              "MSG_TRADE_STATE_OPEN"   //"ÌáÊ¾:ÄúÏÖÔÚ¿ÉÒÔ½»Ò×¡£"
#    define MSG_TRADE_STATE_CLOSE             "MSG_TRADE_STATE_CLOSE"  //"ÌáÊ¾:ÄúÏÖÔÚ²»ÄÜ½»Ò×¡£"
#    define MSG_TRADE_SELF_LOCK               "MSG_TRADE_SELF_LOCK"  //"ÌáÊ¾:Äú½»Ò×ÎïÆ·ÒÑ¾­Ëø¶¨¡£"
#    define MSG_TRADE_SELF_UNLOCK             "MSG_TRADE_SELF_UNLOCK"  //"ÌáÊ¾:Äú½»Ò×ÎïÆ·½â³ýËø¶¨¡£"
#    define MSG_TRADE_DEST_LOCK               "MSG_TRADE_DEST_LOCK"  //"ÌáÊ¾:(%s)½»Ò×ÎïÆ·ÒÑ¾­Ëø¶¨¡£"
#    define MSG_TRADE_DEST_UNLOCK             "MSG_TRADE_DEST_UNLOCK"  //"ÌáÊ¾:(%s)½»Ò×ÎïÆ·½â³ýËø¶¨¡£"
#    define MSG_TRADE_SUCCESS                 "MSG_TRADE_SUCCESS"      //"ÌáÊ¾:ÄúÓë(%s)½»Ò×³É¹¦¡£"
#    define MSG_TRADE_FAIL                    "MSG_TRADE_FAIL"         //"ÌáÊ¾:ÄúÓë(%s)½»Ò×Ê§°Ü¡£"
#    define MSG_TRADE_SELF_ROOM_FULL          "MSG_TRADE_SELF_ROOM_FULL"  //"ÌáÊ¾:ÄúµÄ±³°ü¿Õ¼ä²»×ã!"
#    define MSG_TRADE_DEST_ROOM_FULL          "MSG_TRADE_DEST_ROOM_FULL"  //"ÌáÊ¾:(%s)µÄ±³°ü¿Õ¼ä²»×ã!"
#    define MSG_TRADE_SEND_APPLY              "MSG_TRADE_SEND_APPLY"      //"ÌáÊ¾:ÄúÏò(%s)ÉêÇë½»Ò×!"
#    define MSG_TRADE_GET_APPLY               "MSG_TRADE_GET_APPLY"       //"ÌáÊ¾:(%s)ÉêÇëÓëÄú½»Ò×!"
#    define MSG_TRADE_REFUSE_APPLY            "MSG_TRADE_REFUSE_APPLY"    //"ÌáÊ¾:(%s)¾Ü¾øÓëÄú½»Ò×!"
#    define MSG_TRADE_TASK_ITEM               "MSG_TRADE_TASK_ITEM"  //"ÌáÊ¾:ÈÎÎñÏÞÊ±°ó¶¨ÎïÆ·²»ÄÜ½»Ò×!"

#    define G_ProtocolProcess_20              "G_ProtocolProcess_20"  // Äãµ±Ç°×´Ì¬½ûÖ¹ÒÆ¶¯»òÕß½»Ò×ÎïÆ·£¡

#    define MSG_PK_NORMAL_FLAG_OPEN           "MSG_PK_NORMAL_FLAG_1"  //"MSG_PK_NORMAL_FLAG_OPEN"//"ÌáÊ¾:PK¿ª¹Ø´ò¿ª!"
#    define MSG_PK_NORMAL_FLAG_CLOSE          "MSG_PK_NORMAL_FLAG_0"  //"MSG_PK_NORMAL_FLAG_CLOSE"//"ÌáÊ¾:PK¿ª¹Ø¹Ø±Õ!"
#    define MSG_PK_VALUE                      "MSG_PK_VALUE"    //"ÌáÊ¾:ÄúÄ¿Ç°µÄPKÖµÊÇ%d!"
#    define MSG_PK_ERROR_1                    "MSG_PK_ERROR_1"  //"ÌáÊ¾:ÄúÏÖÔÚ´¦ÓÚ·ÇÕ½¶·×´Ì¬!"
#    define MSG_PK_ERROR_2                    "MSG_PK_ERROR_2"  //"ÌáÊ¾:ÐÂÊÖ²»ÄÜ³ðÉ±!"
#    define MSG_PK_ERROR_3                    "MSG_PK_ERROR_3"  //"ÌáÊ¾:ÄúÕýÓëËûÈËÇÐ´è!"
#    define MSG_PK_ERROR_4                    "MSG_PK_ERROR_4"  //"ÌáÊ¾:ÄúÕýÓëËûÈË³ðÉ±!"
#    define MSG_PK_ERROR_5                    "MSG_PK_ERROR_5"  //"ÌáÊ¾:Äú²»ÄÜ³ðÉ±ÐÂÊÖ!"
#    define MSG_PK_ERROR_6                    "MSG_PK_ERROR_6"  //"ÌáÊ¾:¶Ô·½Õý´¦ÓÚ·ÇÕ½¶·×´Ì¬!"
#    define MSG_PK_ERROR_7                    "MSG_PK_ERROR_7"  //"ÌáÊ¾:¶Ô·½ÕýÓëËûÈËÇÐ´è!"
#    define MSG_PK_ENMITY_SUCCESS_1           "MSG_PK_ENMITY_SUCCESS_1"  //"ÌáÊ¾:ÄúÓë(%s)½¨Á¢³ðÉ±¹ØÏµ!"
#    define MSG_PK_ENMITY_SUCCESS_2           "MSG_PK_ENMITY_SUCCESS_2"  //"ÌáÊ¾:10ÃëÖÓºó³ðÉ±¿ªÊ¼Ö´ÐÐ!"
#    define MSG_PK_ENMITY_CLOSE               "MSG_PK_ENMITY_CLOSE"  //"ÌáÊ¾:ÄúµÄ³ðÉ±¹ØÏµ½áÊø!"
#    define MSG_PK_ENMITY_OPEN                "MSG_PK_ENMITY_OPEN"   //"ÌáÊ¾:³ðÉ±ÏÖÔÚ¿ªÊ¼Ö´ÐÐ!"

#    define MSG_TONG_CREATE_ERROR01           "MSG_TONG_CREATE_ERROR01"  //"ÌáÊ¾:°ï»áÃû¹ý³¤!»ò°ï»áÃûÎÊÌâ"
#    define MSG_TONG_CREATE_ERROR06           "MSG_TONG_CREATE_ERROR06"  //"ÌáÊ¾:°ï»áÕóÓª´íÎó»òÍ³Ë§µÈ¼¶²»¹»!"
#    define MSG_TONG_CREATE_ERROR05           "MSG_TONG_CREATE_ERROR05"  //"ÌáÊ¾:°ï»á³ÉÔ±²»ÄÜÁíÍâ½¨Á¢°ï»á!"
#    define MSG_TONG_CREATE_ERROR04           "MSG_TONG_CREATE_ERROR04"  //"ÌáÊ¾:É±ÊÖ²ÅÄÜ½¨Á¢°ï»á!"
#    define MSG_TONG_CREATE_ERROR07           "MSG_TONG_CREATE_ERROR07"  //"ÌáÊ¾:½¨Á¢°ï»áµÄµÈ¼¶²»¹»!"
#    define MSG_TONG_CREATE_ERROR08           "MSG_TONG_CREATE_ERROR08"  //"ÌáÊ¾:½¨Á¢°ï»áÐèÒª(%d)½ðÇ®!"
#    define MSG_TONG_CREATE_ERROR09           "MSG_TONG_CREATE_ERROR09"  //"ÌáÊ¾:×é¶ÓÖÐ£¬²»ÄÜ½¨Á¢°ï»á!"
#    define MSG_TONG_CREATE_ERROR10           "MSG_TONG_CREATE_ERROR10"  //"ÌáÊ¾:°ï»áÄ£¿é³ö´í!ÇëÁªÏµGM!"
#    define MSG_TONG_CREATE_ERROR11           "MSG_TONG_CREATE_ERROR11"  //"ÌáÊ¾:°ï»áÃû²»ºÏ·¨»òÃû×Ö×Ö·û´®³ö´í!"
#    define MSG_TONG_CREATE_ERROR12           "MSG_TONG_CREATE_ERROR12"  //"ÌáÊ¾:°ï»áÃû²»ºÏ·¨»òÃû×Ö×Ö·û´®¹ý³¤!"
#    define MSG_TONG_CREATE_ERROR13           "MSG_TONG_CREATE_ERROR13"  //"ÌáÊ¾:°ï»áÃû²»ºÏ·¨»òÓë±ðµÄ°ï»áÖØÃû!"
#    define MSG_TONG_APPLY_CREATE             "MSG_TONG_APPLY_CREATE"    //"ÌáÊ¾:ÄúÉêÇë´´½¨°ï»á!"
#    define MSG_TONG_CREATE_SUCCESS           "MSG_TONG_CREATE_SUCCESS"  //"ÌáÊ¾:°ï»á´´½¨³É¹¦!"
#    define MSG_TONG_APPLY_ADD                "MSG_TONG_APPLY_ADD"       //"ÌáÊ¾:ÄúÉêÇë¼ÓÈë°ï»á!"
#    define MSG_TONG_APPLY_ADD_ERROR1         "MSG_TONG_APPLY_ADD_ERROR1"  //"ÌáÊ¾:°ï»á³ÉÔ±²»ÄÜÁíÍâ¼ÓÈë°ï»á!"
#    define MSG_TONG_APPLY_ADD_ERROR2         "MSG_TONG_APPLY_ADD_ERROR2"  //"ÌáÊ¾:É±ÊÖ²ÅÄÜ¼ÓÈë°ï»á!"
#    define MSG_TONG_APPLY_ADD_ERROR3         "MSG_TONG_APPLY_ADD_ERROR3"  //"ÌáÊ¾:×é¶ÓÖÐ£¬²»ÄÜ¼ÓÈë°ï»á!"
#    define MSG_TONG_REFUSE_ADD               "MSG_TONG_REFUSE_ADD"     //"ÌáÊ¾:(%s)¾Ü¾øÄú¼ÓÈë°ï»á!"
#    define MSG_TONG_ADD_SUCCESS              "MSG_TONG_ADD_SUCCESS"    //"ÌáÊ¾:Äú¼ÓÈë°ï»á!"
#    define MSG_TONG_CANNOT_LEAVE1            "MSG_TONG_CANNOT_LEAVE1"  //"ÌáÊ¾:°ïÖ÷²»ÄÜÀë¿ª°ï»á!"
#    define MSG_TONG_CANNOT_LEAVE2            "MSG_TONG_CANNOT_LEAVE2"  //"ÌáÊ¾:³¤ÀÏ²»ÄÜÀë¿ª°ï»á!"
#    define MSG_TONG_BE_KICKED                "MSG_TONG_BE_KICKED"      //"ÌáÊ¾:Äú±»Ìß³ö°ï»á!"
#    define MSG_TONG_LEAVE_SUCCESS            "MSG_TONG_LEAVE_SUCCESS"  //"ÌáÊ¾:ÄúÖ÷¶¯Àë¿ª°ï»á³É¹¦!"
#    define MSG_TONG_LEAVE_FAIL               "MSG_TONG_LEAVE_FAIL"  //"ÌáÊ¾:ÄúÖ÷¶¯Àë¿ª°ï»áÊ§°Ü!"
#    define MSG_TONG_CHANGE_AS_MASTER         "MSG_TONG_CHANGE_AS_MASTER"  //"ÌáÊ¾:Äú±»ÈÎÃüÎª°ïÖ÷!"
#    define MSG_TONG_CHANGE_AS_MEMBER         "MSG_TONG_CHANGE_AS_MEMBER"  //"ÌáÊ¾:ÄúÏÖÔÚ³ÉÎªÆÕÍ¨°ïÖÚ!"

#    define MSG_EARN_XU                       "MSG_EARN_XU"  //"ÌáÊ¾:½ð±ÒÔö¼Ó(%d)¸ö!"
#    define MSG_PK_NORMAL_FLAG_DS             "MSG_PK_NORMAL_FLAG_2"  //"MSG_PK_NORMAL_FLAG_DS"//"ÌáÊ¾:ÍÀÉ±Ä£Ê½¿ªÆô!"
#    define MSG_NPC_CANNOT_RIDE               "MSG_NPC_CANNOT_RIDE"  //"ÌáÊ¾:ÄúÌ«ÀÛÁË,²»ÄÜÆµ·±µÄÉÏÏÂÂí!"
#    define MSG_SUNYI_FAILED                  "MSG_SUNYI_FAILED"  //"ÌáÊ¾:Ë²¼äÒÆ¶¯Ö´ÐÐÊ§°Ü"  //Ã»ÓÐ
#    define MSG_SUNYI_SUCCESS                 "MSG_SUNYI_SUCCESS"  //"ÌáÊ¾:Ë²¼äÒÆ¶¯Ö´ÐÐ³É¹¦"

#    define MSG_GUAJI_SUCCESS                 "MSG_GUAJI_SUCCESS"  //"ÌáÊ¾:×Ô¶¯¹Ò»ú¿ªÆô!"
#    define MSG_COMP_FAILED                   "MSG_COMP_FAILED"    //"ÌáÊ¾:ºÏ³ÉÊ§°Ü!"
#    define MSG_COMP_SUCCESS                  "MSG_COMP_SUCCESS"   //"ÌáÊ¾:ºÏ³É³É¹¦!"
#    define MSG_COMP_ERITEM                   "MSG_COMP_ERITEM"    //"ÌáÊ¾:ÎïÆ·²»¶Ô!"
#    define MSG_COMP_MONEY                    "MSG_COMP_MONEY"     //"ÌáÊ¾:½ðÇ®²»×ã!"
#    define MSG_ID_COMP_NO                    "MSG_ID_COMP_NO"     //"ÌáÊ¾:¸ÃÎïÆ·½ûÖ¹ÖýÔì!"
#    define MSG_SHOP_NO_XU                    "MSG_SHOP_NO_XU"  //"ÌáÊ¾:Áí¼Ó<color=green>Ë°ÊÕ(%d%s)<color>,½ð±Ò»òÒøÁ½²»×ã!"
#    define MSG_SHOP_NO_FUYUAN                "MSG_SHOP_NO_FUYUAN"
#    define MSG_SHOP_NO_SHENGWANG             "MSG_SHOP_NO_SHENGWANG"
#    define MSG_SHOP_NO_DIANKA                "MSG_SHOP_NO_DIANKA"
#    define MSG_SHOP_NO_JIFEN                 "MSG_SHOP_NO_JIFEN"
#    define MSG_SHOP_YOU_XU                   "MSG_SHOP_YOU_XU"  //"ÌáÊ¾:Õ¼°ï³ÉÔ±ÓÅ»Ý<color=green>(%d%s)<color>,½ð±Ò»òÒøÁ½²»×ã!"
#    define MSG_SHOP_YOUS_XU \
        "MSG_SHOP_YOUS_XU"  //"ÌáÊ¾:Õ¼°ï³ÉÔ±ÓÅ»Ý<color=green>(%d%s)<color>,»¨·ÑÁË<color=green>(%s)<color>½ð±Ò!" //Ã»ÓÐ
#    define MSG_SHOP_YOUF_XU \
        "MSG_SHOP_YOUF_XU"  //"ÌáÊ¾:Áí¼Ó³ÇÊÐ<color=green>Ë°ÊÕ(%d%s)<color>,»¨·ÑÁË<color=green>(%s)<color>½ð±Ò!"//Ã»ÓÐ
#    define MSG_DROP_DEST_FAILED  "MSG_DROP_DEST_FAILED"   //"ÌáÊ¾:ÎïÆ·²»ÄÜËæ±ã¶ªµô!" Ã»ÓÐ
#    define MSG_ID_LIANJIE_FAILED "MSG_ID_LIANJIE_FAILED"  //"ÌáÊ¾:<color=white>¸ÃÁ´½ÓÒÑ¾­Ê§Ð§!<color>"
#    define MSG_TEAM_CREAT_FRIEND "MSG_TEAM_CREAT_FRIEND"  //"ÌáÊ¾:´´½¨×é¶Ó½çÃæÊ§°Ü!" Ã»ÓÐ
#    define MSG_ID_SHUA_SHUA      "MSG_ID_SHUA_SHUA"       //"ÌáÊ¾:<color=white>·ÅÖÃÊ§°Ü!<color>"
#    define MSG_CAN_NOT_PK_SUNYI  "MSG_CAN_NOT_PK_SUNYI"  //"ÌáÊ¾:·ÇÕ½¶·Ä£Ê½×´Ì¬,Æô¶¯Ñ°Â·¹¦ÄÜ!"
#    define MSG_ID_FUHUO_YUANDIAN \
        "MSG_ID_FUHUO_YUANDIAN"  //"<color=yellow>¸´»î:Çë´øÉÏÄúµÄ<color=green>¸´»î½ä×Ó<color>¾Í¿ÉÒÔÔ­µØ¸´»îÁË!"
#    define MSG_SKILL_CANNOT_PICKUP "MSG_SKILL_CANNOT_PICKUP"  // ²»ÄÜÊ°È¡¸Ã¼¼ÄÜ

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
    enumMSG_ID_RIDE_CANNOT,  // ÆïÂí
    enumMSG_ID_COMP_FAILED,  // ´òÔìºÏ³É
    enumMSG_ID_COMP_SUCCESS,
    enumMSG_ID_COMP_ERITEM,
    enumMSG_ID_SUNYI_FAILED,   // Ë²ÒÆ
    enumMSG_ID_GUAJI_SUCCESS,  // ¹Ò»ú
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
    enumMSG_ID_LIANJIE_FAILED,  // Á´½ÓÊ§Ð§
    enumMSG_ID_SHUA_SHUA,
    enumMSG_ID_RENOVE_TIME_ITEM,  // É¾³ý¹ýÆÚÎïÆ·
    enumMSG_ID_FUHUO_FAILED,
    enumMSG_ID_ERROR_INFO,
    enumTONG_COMMAND_ID_APPLY_SAVE,
    enumTONG_COMMAND_ID_APPLY_GET,
    enumTONG_COMMAND_ID_APPLY_SND,
    enumMSG_ID_COMP_MONEY,
    enumMSG_ID_COMP_NO,
    enumMSG_ID_NUM,
};

//---------------------------- Êó±êÖ¸ÕëÏà¹Ø ------------------------------
#define MOUSE_CURSOR_NORMAL 0
#define MOUSE_CURSOR_FIGHT  1
#define MOUSE_CURSOR_DIALOG 2
#define MOUSE_CURSOR_PICK   3
#define MOUSE_CURSOR_USE    8

#endif
