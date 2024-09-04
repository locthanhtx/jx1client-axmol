#ifndef	KProtocolDefH
#define	KProtocolDefH


// Add by Freeway chen in 2003.7.1
// ����Э����ݵİ汾������޸ĵ�Э�飬����ԭ�а汾�޷�ʹ�ã���Ҫ�޸������ֵ

#define USE_KPROTOCOL_VERSION   1
//#undef USE_KPROTOCOL_VERSION

#define KPROTOCOL_VERSION   1
/*
 *
 */

/*
 * It was to judge a package type that 
 * it is a larger package or it is a small package
 */
const UINT g_nGlobalProtocolType = 31;

enum s2c_PROTOCOL
{
	s2c_rolehjdkalritopq_getroleinfo_result = 10,
	s2c_syncogjfkdnvhf_roleinfo_cipher,
	s2c_gamestatistic_bigpackage,
	/*
	 * This value must be equal to c2s_micropackbegin
	 */
	s2c_micfghjkdtropackbegin = g_nGlobalProtocolType,  //С����ʼ
	s2c_accountbegin = 32,
	s2c_accountfdfsglogin,
	s2c_gamelogin,
	s2c_accountlogout,
	s2c_gatewayverify,		    //�������Ӻ��һ����
	s2c_gatewayverifyagain,		//�����������һ����
	s2c_gatewayinfo,
	s2c_multiserverbegin = 48,
	s2c_querymapinfo,
	s2c_querygameserverinfo,
	s2c_identitymapping,
	s2c_notifypl_ayerlogin_hfkqotivbhd,
	s2c_notifyplayerexchange,
	s2c_notifysvrip,
	s2c_roleserver_getrolelist_result,
	s2c_roleserver_saverole_result,
	s2c_roleserver_createrole_result,
	s2c_roleserver_deleterole_result,
	s2c_logiclogout,
	s2c_gateway_broadcast,    //ͬ������
	s2c_gamestatistic,
	s2c_rolecheckname_result, //����ɫ���� ���ؽ��
	s2c_gwsyn_broadcast,      //ͬ������
	s2c_clientbegin = 64,
	s2c_login,		//ref: ../../S3Client/Login/LoginDef.h
	s2c_logout,
	s2c_syncclientend,
	s2c_synonlineplayer,
	s2c_synccurplayerskill,
	s2c_synccurplayernormal,
	s2c_newplayer,
	s2c_removeplayer,
	s2c_syncworld,
	s2c_syncplayer,
	s2c_syncplayermin,
	s2c_syncnpc,
	s2c_syncnpcmin,
	s2c_syncnpcminplayer,
	s2c_objadd,
	s2c_syncobjstate,
	s2c_syncobjdir,
	s2c_objremove,
	s2c_objTrapAct,
	s2c_npcremove,
	s2c_npcwalk,
	s2c_npcrun,
	s2c_npcattack,
	s2c_npcmagic,
	s2c_npcjump,
	s2c_npctalk,
	s2c_npchurt,
	s2c_npcdeath,
	s2c_npcchgcurcamp,
	s2c_npcchgcamp,
	s2c_skillcast,
	s2c_playertalk,
	s2c_playerexp,
	s2c_teaminfo,
	s2c_teamselfinfo,
	s2c_teamapplyinfofalse,
	s2c_teamcreatesuccess,
	s2c_teamcreatefalse,
	s2c_teamopenclose,
	s2c_teamgetapply,
	s2c_teamaddmember,
	s2c_teamleave,
	s2c_teamchangecaptain,
	s2c_playerfactiondata,
	s2c_playerleavefaction,
	s2c_playerfactionskilllevel,
	s2c_playersendchat,
	s2c_playersyncleadexp,
	s2c_playerlevelup,
	s2c_teammatelevel,
	s2c_playersyncattribute,
	s2c_playerskilllevel,
	s2c_syncitem,
	s2c_removeitem,
	s2c_syncmonfs,
	s2c_syncqianfs,
	s2c_playermoveitem,
	s2c_scriptaction,
	s2c_chatapplyaddfriend,
	s2c_chataddfriend,
	s2c_chatrefusefriend,
	s2c_chataddfriendfail,
	s2c_chatloginfriendnoname,
	s2c_chatloginfriendname,
	s2c_chatonefrienddata,
	s2c_chatfriendonline,
	s2c_chatdeletefriend,
	s2c_chatfriendoffline,
	s2c_syncrolelist,   //û�и�����
	s2c_tradechangestate,
	s2c_npcsetmenustate,
	s2c_trademoneysync,
	s2c_tradedecision,
	s2c_chatscreensingleerror,
	s2c_syncnpcstate,
	s2c_teaminviteadd,
	s2c_tradepressoksync,
	s2c_ping,
	s2c_npcsit,
	s2c_opensalebox,
	s2c_castskilldirectly,	  //��BUG
	s2c_msgshow,
	s2c_syncstateeffect,
	s2c_openstorebox,
	s2c_openresetpass,
	s2c_openstringbox,
	s2c_opendataubox,
	s2c_playerrevive,
	s2c_requestnpcfail,
	s2c_tradeapplystart,
	s2c_rolenewdelresponse,	//�½���ɾ����ɫ�Ľ������,��������Ϊ�ṹtagNewDelRoleResponse
	s2c_ItemAutoMove,
	s2c_itemexchangefinish,
	s2c_changeweather,
	s2c_pksyncnormalflag,
	s2c_pksyncenmitystate,
	s2c_pksyncexercisestate,
	s2c_pksyncpkvalue,
	s2c_reputesyncreputevalue,
	s2c_fuyuansyncfuyuanvalue,
	s2c_rebornsyncrebornvalue,
	s2c_openmarketbox,
	s2c_npcsleepmode,
	s2c_viewequip,
	s2c_ladderresult,
	s2c_ladderlist,
	s2c_tongcreate,
	s2c_replyclientping,
	s2c_npcgoldchange,
	s2c_itemdurabilitychange,
    s2c_opentremble,
	s2c_rankname,
	s2c_viewsellitem,
	s2c_viewupdateitem,
	s2c_playergetcount,
    s2c_shopname,
    s2c_syncmasklock,	// mat na
    s2c_opengive,
	s2c_npcsecmove,
	s2c_pkvalue,
	s2c_viewlianitem,   //����
	s2c_openwengang,    //�Ƹ�
	s2c_openstone,      //��ʯ��Ƕ
//	s2c_gmgateway2relaysvr,		//GM��½������֪ͨ��ת�������кϷ����ӵ�Э��
    s2c_updateviewequip,
	s2c_gettongback,
	s2c_syncequipitem,
	s2c_taskcallback,
	s2c_openjindutiao,   //�򿪽�����
	s2c_syncitemposition,//ͬ��λ��(û��)
	s2c_openduanzhao,
	s2c_syncurupdata,
	s2c_JinMaicallback,
	s2c_openvipdazao,
	s2c_synnpcinfocallback,
	s2c_pautoMovecallback,
	s2c_plyerguzicallback,
	s2c_syncplayermap,
	s2c_synconestate,
	s2c_syncnodataeffect,
	s2c_npcsetpos,
	s2c_playermissiondata,
	s2c_syncrankdata,
	s2c_syncsupershop,
	s2c_syncitem_shop,
	s2c_itemdata_shop,
	s2c_kickoutgame,
	s2c_objadd_vn,
	s2c_rolesyncvipvalue,
	s2c_extend = 250,
	s2c_extendchat = 251,
	s2c_extendfriend = 252,
	s2c_extendtong = 253,
	s2c_end,
};

enum c2s_PROTOCOL
{
	c2s_rolekfjhu_saveroleinfo = 10,
	c2s_roleserver_createroleinfo,	
	c2s_gmsvr2gateway_saverole,
	/*
	 * This value must be equal to s2c_micropackbegin
	 */
	c2s_micropackbegin = g_nGlobalProtocolType,
	c2s_accountbegin = 32,
	c2s_accountlogin,  //�˺ŵ�½
	c2s_gamelogin_dkiqopjgurnmvbgc,     //��ʼ��½��Ϸ �Ƿ��ܳɹ�������ʾ����������
	c2s_accountlogout,
	c2s_gatewayverify,
	c2s_gatewayverifyagain,
	c2s_gatewayinfo,

	c2s_multiserverbegin = 48,
	c2s_permit_logftihmcginqknd,
	c2s_updatemapinfo,
	c2s_updategameserverinfo,
	c2s__entergamegkjyiruqine,     //GS ֪ͨ��ʼ������Ϸ
	c2s_leavegame,
	c2s_registeraccount,
	c2s_requestsvrip,
	c2s_roleserver_getrolelist,
	c2s_roleserver_getroleinfo,
	c2s_roleserver_deleteplayer,
	c2s_gamestatistic,
	c2s_roleserver_lock,	
	c2s_gameserverbegin = 64,
	c2s_login_fhgyrinhkmvnsheypo,
	c2s__loginfs_kfjghtueodnchsf,
	c2s_syncclientend,
	c2s_loadplayer,
	c2s_newplayer,
	c2s_removeplayer,
	c2s_requestworld,
	c2s_requestplayer,
	c2s_requestnpc,
	c2s_requestobj,
	c2s_npcwalk,
	c2s_npcrun,
	c2s_npcskill,
	c2s_npcjump,
	c2s_npctalk,
	c2s_npchurt,
	c2s_npcdeath,
	c2s_playertalk,
	c2s_teamapplyinfo,
	c2s_teamapplycreate,
	c2s_teamapplyopenclose,
	c2s_teamapplyadd,
	c2s_teamacceptmember,
	c2s_teamapplyleave,
	c2s_teamapplykickmember,
	c2s_teamapplychangecaptain,
	c2s_teamapplydismiss,
	c2s_playerapplysetpk,
	c2s_playerapplyfactiondata,
	c2s_playersendchat,
	c2s_playeraddbaseattribute,
	c2s_playerapplyaddskillpoint,
	c2s_playereatitem,
	c2s_playerpickupitem,
	c2s_playermoveitem,
	c2s_playersellitem,
	c2s_playerbuyitem,
	c2s_playerthrowawayitem,
	c2s_playerselui,
	c2s_chatsetchannel,
	c2s_chatapplyaddfriend,
	c2s_chataddfriend,
	c2s_chatrefusefriend,
	c2s_dbplayerselect,
	c2s_chatapplyresendallfriendname,
	c2s_chatapplysendonefriendname,
	c2s_chatdeletefriend,
	c2s_chatredeletefriend,
	c2s_tradeapplystateopen,
	c2s_tradeapplystateclose,
	c2s_tradeapplystart,
	c2s_trademovemoney,
	c2s_tradedecision,
	c2s_dialognpc,
	c2s_teaminviteadd,
	c2s_changeauraskill,
	c2s_teamreplyinvite,
	c2s_ping,
	c2s_cpunlock,
	c2s_cplock,
    c2s_cpchange,
    c2s_cpreset,
    c2s_market,//KTC
    c2s_string,
    c2s_datau,
	c2s_npcsit,
	c2s_objmouseclick,
	c2s_storemoney,
	c2s_playerrevive,
	c2s_tradereplystart,
	c2s_pkapplychangenormalflag,
	c2s_pkapplyenmity,
	c2s_viewequip,
//	c2s_gmlogin,		//��Чͬc2s_login
	c2s_ladderquery,
	c2s_repairitem,
	c2s_npcride,
	c2s_breakitem,
	c2s_playercomp,
	c2s_playersetprice,
    c2s_playerstarttrade,
    c2s_viewitem,
    c2s_playertradebuyitem,
    c2s_updateviewitem,
    c2s_playerneedcount,
    c2s_playershopname,
    c2s_playersysshop,
    c2s_playerlixian,
    c2s_playergiveback,
    c2s_pkvalue,
    c2s_autoplay,//�һ�
    c2s_npcsunyi,//˲��
    c2s_guzi,
    c2s_gufarm,
    c2s_addjinbi,
    c2s_lianjie,
    c2s_fsjingmai,
    c2s_guaforbit,
    c2s_skillsync,
	c2s_playerrong,         //����
	c2s_playeradditem,      //������Ʒ û���õ�
	c2s_playerAutoMoveitem,
	c2s_playerdelitem,
	c2s_playersetLianFlg,
	c2s_updateviewequip,   //�ͻ���
	c2s_playertongset,
	c2s_playerdoScript,
    c2s_gettaskval,//��ȡ����ŵ�ֵ
	c2s_jinducallback,
	c2s_syncurdata,
	c2s_vipdoScript,
	c2s_roomisfull,
	c2s_synnpcinfo,	 //ͬ��NPC��Ϣ
	c2s_playerautoequip,
	c2s_playerguzgame,
	c2s_playersetpos,
	c2s_synshopidx,//KTC
	c2s_settaskval,
	c2s_runscript,
	c2s_player_req,           //�����ٴ�ͬ��������Ϣ��������
	c2s_rolecheckname_result, //��ѯ��ɫ�Ƿ����
	c2s_msginfo,              //bishopȫ����Ϣ
	_c2s_begin_relay = 250,
	c2s_extend = _c2s_begin_relay,
	c2s_extendchat,
	c2s_extendfriend,
	_c2s_end_relay = c2s_extendfriend,
	c2s_extendtong,
	c2s_end,
};

enum c2c_PROTOCOL		//��Э���������server��server֮��
{
	c2c_transferroleinfo = 14,
	c2c_micropackbegin = g_nGlobalProtocolType,
	c2c_protocolbegin = 32,
	c2c_permitplayerexchangeout = 64,
	c2c_permitplayerexchangein,
	c2c_notifyexchange,
	s2s_broadcast = 96,		//���ڴ�Relay����Ϸ����Ĺ㲥
	s2s_execute = 97,		//���ڴ�Relay����Ϸ�����ִ�нű�
	c2c_end,
};

//����Э��Ϊc2s_extendЭ�������Э�飬�μ�KRelayProtocol.h
//��չЭ�������EXTEND_HEADER��ͷ

enum gm_PROTOCOL		//��Э���������gm��server֮��
{
	gm_begin = 32,
	gm_c2s_execute,				//����Э����GM�ͻ�����relay_c2c_askwaydataת������Ϸ����
	gm_c2s_disable,
	gm_c2s_enable,
	gm_c2s_tracking,
	gm_c2s_setrole,
	gm_c2s_getrole,	
	gm_c2s_findplayer,
	gm_c2s_unlock,
	gm_c2s_getrolelist,
	gm_c2s_broadcast_chat,
	gm_s2c_tracking,			//����Э������Ϸ������relay_c2c_dataת����GM�ͻ���
	gm_s2c_getrole,
	gm_s2c_findplayer,
	gm_s2c_getrolelist,
	gm_end,
};


enum relay_PROTOCOL		//��Э���������server��relay֮��
{
	relay_begin = 32,
	relay_c2c_data,
	relay_c2c_askwaydata,
	relay_s2c_loseway,
	relay_end,
};

enum chat_PROTOCOL
{
	chat_micropackbegin = g_nGlobalProtocolType,
	chat_someonechat,
	chat_channelchat,
	chat_feedback,
	chat_everyone,
	chat_groupman,
	chat_specman,
	chat_relegate,
	chat_filterplayer,
};

enum playercomm_PROTOCOL		//��Э���������server��client֮��,����server����ת����relay������Ϊrelay�е��˲���server�Ĺ���
{
	playercomm_begin = 32,
	playercomm_c2s_querychannelid,
	playercomm_s2c_notifychannelid,
	playercomm_c2s_freechannelid,
	playercomm_c2s_subscribe,
	playercomm_s2c_relegate,
	playercomm_c2s_rollback,
	playercomm_s2c_gmquerychannelid,
	playercomm_s2c_gmfreechannelid,
	playercomm_s2c_gmsubscribe,
	playercomm_c2s_someoneact,
	playercomm_c2s_channelact,
	playercomm_someonechat,
	playercomm_channelchat,
};

// game server ���� s3client �����չЭ�� id
enum
{
	enumTONG_SYNC_ID_CREATE_FAIL = 0,
	enumTONG_SYNC_ID_TRANSFER_ADD_APPLY,
	enumTONG_SYNC_ID_ADD,
	enumTONG_SYNC_ID_HEAD_INFO,
	enumTONG_SYNC_ID_SELF_INFO,
	enumTONG_SYNC_ID_MANAGER_INFO,
	enumTONG_SYNC_ID_MEMBER_INFO,
	enumTONG_SYNC_ID_INSTATE,
	enumTONG_SYNC_ID_KICK,
	enumTONG_SYNC_ID_CHANGE_MASTER_FAIL,
	enumTONG_SYNC_ID_LIST_INFO,
	enumTONG_SYNC_ID_ATTACK_INFO,
	enumTONG_SYNC_ID_CITY_INFO,
	enumTONG_SYNC_ID_NUM,
};

// s3client ���� game server �����չЭ�� id
enum
{
	enumTONG_COMMAND_ID_START = 0,
	enumTONG_COMMAND_ID_APPLY_CREATE,
	enumTONG_COMMAND_ID_APPLY_ADD,            //����������
	enumTONG_COMMAND_ID_ACCEPT_ADD,
	enumTONG_COMMAND_ID_APPLY_INFO,
	enumTONG_COMMAND_ID_APPLY_INSTATE,        //����ְλ
	enumTONG_COMMAND_ID_APPLY_KICK,
	enumTONG_COMMAND_ID_APPLY_LEAVE,
	enumTONG_COMMAND_ID_APPLY_CHANGE_MASTER,
	enumTONG_COMMAND_ID_APPLY_ATTACK_GET,	  //��ȡ��ս����
	enumTONG_COMMAND_ID_APPLY_ATTACK_SEND,	  //������ս
	enumTONG_COMMAND_ID_APPLY_SET_SEND,       //����
	enumTONG_COMMAND_ID_NUM,
};


#endif
