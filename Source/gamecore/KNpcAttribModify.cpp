#include "KCore.h"
#include "KNpc.h"
#include "KPlayer.h"
#include "KNpcAttribModify.h"
#include "KNpcTemplate.h"
#ifndef max
#define max(a,b)            (((a) > (b)) ? (a) : (b))
#endif
#include "KNpcAI.h"
KNpcAttribModify	g_NpcAttribModify;

KNpcAttribModify::KNpcAttribModify()
{
	ZeroMemory(ProcessFunc, sizeof(ProcessFunc));
	ProcessFunc[magic_physicsresmax_p] = &KNpcAttribModify::PhysicsResMaxP;
	ProcessFunc[magic_coldresmax_p] = &KNpcAttribModify::ColdResMaxP;
	ProcessFunc[magic_fireresmax_p] = &KNpcAttribModify::FireResMaxP;
	ProcessFunc[magic_lightingresmax_p] = &KNpcAttribModify::LightingResMaxP;
	ProcessFunc[magic_poisonresmax_p] = &KNpcAttribModify::PoisonResMaxP;
	ProcessFunc[magic_allresmax_p] = &KNpcAttribModify::AllResMaxP;
	ProcessFunc[magic_lifepotion_v] = &KNpcAttribModify::LifePotionV;
	ProcessFunc[magic_manapotion_v] = &KNpcAttribModify::ManaPotionV;
	ProcessFunc[magic_meleedamagereturn_v] = &KNpcAttribModify::MeleeDamageReturnV;
	ProcessFunc[magic_meleedamagereturn_p] = &KNpcAttribModify::MeleeDamageReturnP;
	ProcessFunc[magic_rangedamagereturn_v] = &KNpcAttribModify::RangeDamageReturnV;
	ProcessFunc[magic_rangedamagereturn_p] = &KNpcAttribModify::RangeDamageReturnP;
	ProcessFunc[magic_damagetomana_p] = &KNpcAttribModify::Damage2ManaP;        //�˺�ת��Ϊ��������
	ProcessFunc[magic_adddefense_v] = &KNpcAttribModify::ArmorDefenseV;			// ��װ���ӷ���ͳһ����
	ProcessFunc[magic_poisonenhance_p] = &KNpcAttribModify::PoisonEnhanceP;     //�������ʱ��
	ProcessFunc[magic_lightingenhance_p] = &KNpcAttribModify::LightingEnhanceP; //��ǿ����
	ProcessFunc[magic_fireenhance_p] = &KNpcAttribModify::FireEnhanceP;         //��ǿ�ڻ�
	ProcessFunc[magic_coldenhance_p] = &KNpcAttribModify::ColdEnhanceP;         //���ӳٻ�ʱ��ٷֱ�
	ProcessFunc[magic_armordefense_v] = &KNpcAttribModify::ArmorDefenseV;
	ProcessFunc[magic_lifemax_v] = &KNpcAttribModify::LifeMaxV;
	ProcessFunc[magic_lifemax_p] = &KNpcAttribModify::LifeMaxP;                 //�������ֵ
	ProcessFunc[magic_life_v] = &KNpcAttribModify::LifeV;
	ProcessFunc[magic_lifereplenish_v] = &KNpcAttribModify::LifeReplenishV;     //�����ָ��� װ��
	ProcessFunc[magic_manamax_v] = &KNpcAttribModify::ManaMaxV;
	ProcessFunc[magic_manamax_p] = &KNpcAttribModify::ManaMaxP;
	ProcessFunc[magic_mana_v] = &KNpcAttribModify::ManaV;
	ProcessFunc[magic_manareplenish_v] = &KNpcAttribModify::ManaReplenishV;
	ProcessFunc[magic_staminamax_v] = &KNpcAttribModify::StaminaMaxV;
	ProcessFunc[magic_staminamax_p] = &KNpcAttribModify::StaminaMaxP;
	ProcessFunc[magic_stamina_v] = &KNpcAttribModify::StaminaV;
	ProcessFunc[magic_staminareplenish_v] = &KNpcAttribModify::StaminaReplenishV;
	ProcessFunc[magic_strength_v] = &KNpcAttribModify::StrengthV;
	ProcessFunc[magic_dexterity_v] = &KNpcAttribModify::DexterityV;
	ProcessFunc[magic_vitality_v] = &KNpcAttribModify::VitalityV;
	ProcessFunc[magic_energy_v] = &KNpcAttribModify::EnergyV;
	ProcessFunc[magic_poisonres_p] = &KNpcAttribModify::PoisonresP;
	ProcessFunc[magic_fireres_p] = &KNpcAttribModify::FireresP;
	ProcessFunc[magic_lightingres_p] = &KNpcAttribModify::LightingresP;
	ProcessFunc[magic_physicsres_p] = &KNpcAttribModify::PhysicsresP;
	ProcessFunc[magic_coldres_p] = &KNpcAttribModify::ColdresP;
	ProcessFunc[magic_freezetimereduce_p] = &KNpcAttribModify::FreezeTimeReduceP;
	ProcessFunc[magic_burntimereduce_p] = &KNpcAttribModify::BurnTimeReduceP;
	ProcessFunc[magic_poisontimereduce_p] = &KNpcAttribModify::PoisonTimeReduceP;
	ProcessFunc[magic_poisondamagereduce_v] = &KNpcAttribModify::PoisonDamageReduceV;
	ProcessFunc[magic_stuntimereduce_p] = &KNpcAttribModify::StunTimeReduceP;
	ProcessFunc[magic_fastwalkrun_p] = &KNpcAttribModify::FastWalkRunP;
	ProcessFunc[magic_visionradius_p] = &KNpcAttribModify::VisionRadiusP;
	ProcessFunc[magic_fasthitrecover_v] = &KNpcAttribModify::FastHitRecoverV;        //���˶���ʱ��
	ProcessFunc[magic_allres_p] = &KNpcAttribModify::AllresP;
	ProcessFunc[magic_attackratingenhance_v] = &KNpcAttribModify::AddAttackRatingV;   //״̬װ�����е��ǿ
	ProcessFunc[magic_attackratingenhance_p] = &KNpcAttribModify::AddAttackRatingP;   //״̬װ�����аٷֱȼ�ǿ
	ProcessFunc[magic_attackspeed_v] = &KNpcAttribModify::AttackSpeedV;               //�⹦����
	ProcessFunc[magic_castspeed_v] = &KNpcAttribModify::CastSpeedV;                   //�ڹ�����
	//=====================================������������==============================================
	ProcessFunc[magic_attackrating_v] = &KNpcAttribModify::AttackRatingV;             //����������
	ProcessFunc[magic_attackrating_p] = &KNpcAttribModify::AttackRatingP;             //�����������аٷֱ�
	ProcessFunc[magic_ignoredefense_p] = &KNpcAttribModify::Ignoredefense_p;          //���Ե���������	
	ProcessFunc[magic_physicsdamage_v] = &KNpcAttribModify::AddPhysicsMagic;          //�չ��˺�:#d1+��
	ProcessFunc[magic_colddamage_v] = &KNpcAttribModify::AddColdMagic;                //��������
	ProcessFunc[magic_firedamage_v] = &KNpcAttribModify::AddFireMagic;                //�������
	ProcessFunc[magic_lightingdamage_v] = &KNpcAttribModify::AddLightingMagic;        //�����׵�
	ProcessFunc[magic_poisondamage_v] = &KNpcAttribModify::AddPoisonMagic;            //��������
	ProcessFunc[magic_physicsenhance_p]  = &KNpcAttribModify::Add_neiphysicsenhance_p;//�յ�ٷֱ�
	//====================�¼�������ʱû��===========================================================
	/*ProcessFunc[magic_xinphysicsdamage_v] = &KNpcAttribModify::XinPhysicsDamageV;      //�¼����յ�
	ProcessFunc[magic_xinfiredamage_v] = &KNpcAttribModify::XinFireDamageV;            //�¼��ܻ��
	ProcessFunc[magic_xincolddamage_v] = &KNpcAttribModify::XinColdDamageV;            //�¼��ܱ���
	ProcessFunc[magic_xinlightingdamage_v] = &KNpcAttribModify::XinLightingDamageV;    //�¼����׵�
	ProcessFunc[magic_xinpoisondamage_v] = &KNpcAttribModify::XinPoisonDamageV;        //�¼��ܶ��� 
	ProcessFunc[magic_xinphysicsdamage_p] = &KNpcAttribModify::XinPhysicsDamageP;      //�¼����հٷֱ�
	*/
	//======================================״̬����=================================================
	ProcessFunc[magic_addphysicsdamage_p] = &KNpcAttribModify::AddPhysicsDamagePP;    //���հٷֱ�-��������״̬
	ProcessFunc[magic_addphysicsdamage_v] = &KNpcAttribModify::Addphysicsdamagevp;    //���յ�-��������״̬
	ProcessFunc[magic_addfiredamage_v] = &KNpcAttribModify::Aaddfiredamagevp;         //����-��������״̬
	ProcessFunc[magic_addcolddamage_v] = &KNpcAttribModify::Addcolddamagevp;          //�����-��������״̬
    ProcessFunc[magic_addpoisondamage_v] = &KNpcAttribModify::Addpoisondamagevp;      //�ⶾ��-��������״̬
	ProcessFunc[magic_addlightingdamage_v] = &KNpcAttribModify::Addlightingdamagevp;  //���׵�-��������״̬
    ProcessFunc[magic_addphysicsmagic_v] = &KNpcAttribModify::Addphysicsmagicvb;      //���յ�-��������״̬
	ProcessFunc[magic_addcoldmagic_v] = &KNpcAttribModify::Addcoldmagicvp;            //�ڱ���-��������״̬
	ProcessFunc[magic_addfiremagic_v] = &KNpcAttribModify::Addfiremagicv;             //�ڻ��-��������״̬
	ProcessFunc[magic_addlightingmagic_v] = &KNpcAttribModify::Addlightingmagicv;     //���׵�-��������״̬
	ProcessFunc[magic_addpoisonmagic_v] = &KNpcAttribModify::Addpoisonmagicv;         //�ڶ���-��������״̬
	ProcessFunc[magic_addphysicsmagic_p] = &KNpcAttribModify::Addphysicsmagicp;       //���հٷֱ�-��������״̬
	//================================================================================================
	ProcessFunc[magic_slowmissle_b] = &KNpcAttribModify::SlowMissleB;
	ProcessFunc[magic_changecamp_b] = &KNpcAttribModify::ChangeCampV;
	ProcessFunc[magic_physicsarmor_v] = &KNpcAttribModify::PhysicsArmorV;
	ProcessFunc[magic_coldarmor_v] = &KNpcAttribModify::ColdArmorV;
	ProcessFunc[magic_firearmor_v] = &KNpcAttribModify::FireArmorV;
	ProcessFunc[magic_poisonarmor_v] = &KNpcAttribModify::PoisonArmorV;
	ProcessFunc[magic_lightingarmor_v] = &KNpcAttribModify::LightingArmorV;
	ProcessFunc[magic_lucky_v] = &KNpcAttribModify::LuckyV;
	ProcessFunc[magic_steallife_p] = &KNpcAttribModify::StealLifeP;
	ProcessFunc[magic_steallifeenhance_p] = &KNpcAttribModify::StealLifeP;
	ProcessFunc[magic_stealstamina_p] = &KNpcAttribModify::StealStaminaP;
	ProcessFunc[magic_stealstaminaenhance_p] = &KNpcAttribModify::StealStaminaP;
	ProcessFunc[magic_stealmana_p] = &KNpcAttribModify::StealManaP;
	ProcessFunc[magic_stealmanaenhance_p] = &KNpcAttribModify::StealManaP;
	ProcessFunc[magic_allskill_v] = &KNpcAttribModify::AllSkillV;
	ProcessFunc[magic_metalskill_v] = &KNpcAttribModify::MetalSkillV;
	ProcessFunc[magic_woodskill_v] = &KNpcAttribModify::WoodSkillV;
	ProcessFunc[magic_waterskill_v] = &KNpcAttribModify::WaterSkillV;
	ProcessFunc[magic_fireskill_v] = &KNpcAttribModify::FireSkillV;
	ProcessFunc[magic_earthskill_v] = &KNpcAttribModify::EarthSkillV;
	ProcessFunc[magic_knockback_p] = &KNpcAttribModify::KnockBackP; //���˰ٷֱ�����
	ProcessFunc[magic_knockbackenhance_p] = &KNpcAttribModify::KnockBackP;  //���˰ٷֱ�����
	ProcessFunc[magic_fatallystrike_p] = &KNpcAttribModify::DeadlyStrikeP;        //����һ�����������ܣ�
	ProcessFunc[magic_fatallystrikeenhance_p] =&KNpcAttribModify::DeadlyStrikeP;  //����һ����״̬��
	ProcessFunc[magic_deadlystrike_p] = &KNpcAttribModify::DeadlyStrikeP;         //����һ���ٷֱ����ӣ��������ܣ�
	ProcessFunc[magic_deadlystrikeenhance_p] = &KNpcAttribModify::DeadlyStrikeP;  //��������һ���ʣ�״̬��
	ProcessFunc[magic_badstatustimereduce_v] = &KNpcAttribModify::BadStatusTimeReduceV; //����״̬ʱ������
	ProcessFunc[magic_manashield_p] = &KNpcAttribModify::ManaShieldP;             //���������˺�
	ProcessFunc[magic_fatallystrikeres_p] = &KNpcAttribModify::fatallystrikeresP;
	ProcessFunc[magic_addskilldamage1] = &KNpcAttribModify::addskilldamage1;
	ProcessFunc[magic_addskilldamage2] = &KNpcAttribModify::addskilldamage2;
    ProcessFunc[magic_expenhance_p] = &KNpcAttribModify::expenhanceP;//ɱ�����˻�þ��� �� 
	ProcessFunc[magic_addskilldamage3] = &KNpcAttribModify::addskilldamage3;
	ProcessFunc[magic_addskilldamage4] = &KNpcAttribModify::addskilldamage4;
	ProcessFunc[magic_addskilldamage5] = &KNpcAttribModify::addskilldamage5;
	ProcessFunc[magic_addskilldamage6] = &KNpcAttribModify::addskilldamage6;
	ProcessFunc[magic_dynamicmagicshield_v] = &KNpcAttribModify::dynamicmagicshieldV;
	ProcessFunc[magic_addstealfeatureskill] = &KNpcAttribModify::addstealfeatureskill;
	ProcessFunc[magic_lifereplenish_p] = &KNpcAttribModify::lifereplenishP;
	ProcessFunc[magic_ignoreskill_p] = &KNpcAttribModify::ignoreskillP;
	ProcessFunc[magic_poisondamagereturn_v] = &KNpcAttribModify::poisondamagereturnV;  //��������
	ProcessFunc[magic_poisondamagereturn_p] = &KNpcAttribModify::poisondamagereturnP;  //�������ٷֱ�
	ProcessFunc[magic_returnskill_p] = &KNpcAttribModify::returnskillP;
	ProcessFunc[magic_autoreplyskill] = &KNpcAttribModify::autoreplyskill;
	ProcessFunc[magic_skill_mintimepercast_v] = &KNpcAttribModify::skill_mintimepercastV;
	ProcessFunc[magic_mintimepercastonhorse_v] = &KNpcAttribModify::mintimepercastonhorseV;// �����ͷż��ʱ��
	ProcessFunc[magic_poison2decmana_p] = &KNpcAttribModify::poison2decmanaP;	 //��ʧ����
	ProcessFunc[magic_skill_appendskill] = &KNpcAttribModify::skill_appendskil;  //��������
	ProcessFunc[magic_hide] = &KNpcAttribModify::hide;
	ProcessFunc[magic_clearnegativestate] = &KNpcAttribModify::clearnegativestate;
	ProcessFunc[magic_returnres_p] = &KNpcAttribModify::returnresP;
	ProcessFunc[magic_dec_percasttimehorse] = &KNpcAttribModify::decPercasttimehorse;//����#l3-��ȴʱ��:����<color=orange>#f7-��<color>,����<color=orange>#f4-��<color>
	ProcessFunc[magic_dec_percasttime] = &KNpcAttribModify::decPercasttime;
	ProcessFunc[magic_enhance_autoSkill] = &KNpcAttribModify::enhance_autoSkill;
	ProcessFunc[magic_enhance_life_p] = &KNpcAttribModify::enhance_lifeP;
	ProcessFunc[magic_enhance_life_v] = &KNpcAttribModify::enhance_lifeV;
	ProcessFunc[magic_enhance_711_auto] = &KNpcAttribModify::enhance_711_auto;
	ProcessFunc[magic_enhance_714_auto] = &KNpcAttribModify::enhance_714_auto;
	ProcessFunc[magic_enhance_717_auto] = &KNpcAttribModify::enhance_717_auto;
	ProcessFunc[magic_enhance_723_miss_p] = &KNpcAttribModify::enhance_723_missP;
	ProcessFunc[magic_seriesdamage_p] = &KNpcAttribModify::SerisesDamage;
	ProcessFunc[magic_autoattackskill] = &KNpcAttribModify::autoskill;
	ProcessFunc[magic_block_rate] = &KNpcAttribModify::block_rate;
	ProcessFunc[magic_enhancehit_rate] = &KNpcAttribModify::enhancehit_rate;
	ProcessFunc[magic_anti_block_rate] = &KNpcAttribModify::anti_block_rate;                   //=���񵲸��ʣ�#d1+%
	ProcessFunc[magic_anti_enhancehit_rate] = &KNpcAttribModify::anti_enhancehit_rate;         //=�ֵ��ػ����ʣ�#d1+%
	ProcessFunc[magic_sorbdamage_p] = &KNpcAttribModify::sorbdamage_p;                         //=�����˺�:#d1+
	ProcessFunc[magic_anti_poisonres_p] = &KNpcAttribModify::anti_poisonres_p;                 //=���ԶԷ�����:#d1-%
	ProcessFunc[magic_anti_fireres_p] = &KNpcAttribModify::anti_fireres_p;                     //=���ԶԷ����:#d1-%
	ProcessFunc[magic_anti_lightingres_p] = &KNpcAttribModify::anti_lightingres_p;             //=���ԶԷ��׷�:#d1-%
	ProcessFunc[magic_anti_physicsres_p] = &KNpcAttribModify::anti_physicsres_p;               //=���ԶԷ��շ�:#d1-%
	ProcessFunc[magic_anti_coldres_p] = &KNpcAttribModify::anti_coldres_p;                     //=���ԶԷ�����:#d1-%
	ProcessFunc[magic_not_add_pkvalue_p] = &KNpcAttribModify::not_add_pkvalue_p;               //=������PKֵ����:#d1+%
	ProcessFunc[magic_add_boss_damage] = &KNpcAttribModify::add_boss_damage;                   //=�Իƽ�boss�����˺�<color=orange>#d1+%<color>
	ProcessFunc[magic_five_elements_enhance_v] = &KNpcAttribModify::five_elements_enhance_v;   //=����ǿ��ֵ��#d1-�㡣ǿ����������еĿ���Ч��
    ProcessFunc[magic_five_elements_resist_v] = &KNpcAttribModify::five_elements_resist_v;     //=��������ֵ��#d1-�㡣������������еĿ���Ч��
    ProcessFunc[magic_skill_enhance] = &KNpcAttribModify::skill_enhance;                       //�Թ������ܼӳ�
    ProcessFunc[magic_anti_allres_p] = &KNpcAttribModify::anti_allres_p; 
    ProcessFunc[magic_add_alldamage_p] = &KNpcAttribModify::add_alldamage_p;                   //��ǿ�˺�
    ProcessFunc[magic_auto_Revive_rate] = &KNpcAttribModify::auto_Revive_rate; 
    ProcessFunc[magic_addcreatnpc_v]  = &KNpcAttribModify::addcreatnpc_v;  //ץ������BB
    ProcessFunc[magic_addzhuabu_v]  = &KNpcAttribModify::addzhuabu_v;      //ץ������BB
    ProcessFunc[magic_reduceskillcd1]  = &KNpcAttribModify::reduceskillcd1;   
	ProcessFunc[magic_reduceskillcd2]  = &KNpcAttribModify::reduceskillcd2;
	ProcessFunc[magic_reduceskillcd3]  = &KNpcAttribModify::reduceskillcd3;
	ProcessFunc[magic_clearallcd]  = &KNpcAttribModify::clearallcd;
	ProcessFunc[magic_addblockrate]  = &KNpcAttribModify::addblockrate;
	ProcessFunc[magic_walkrunshadow]  = &KNpcAttribModify::walkrunshadow;
	ProcessFunc[magic_returnskill2enemy]  = &KNpcAttribModify::returnskill2enemy;
	ProcessFunc[magic_manatoskill_enhance]  = &KNpcAttribModify::manatoskill_enhance;
	ProcessFunc[magic_add_alldamage_v]  = &KNpcAttribModify::add_alldamage_v;
	ProcessFunc[magic_addskilldamage7]  = &KNpcAttribModify::addskilldamage7;
	ProcessFunc[magic_ignoreattacrating_v]  = &KNpcAttribModify::ignoreattacrating_v;
	ProcessFunc[magic_alljihuo_v]  = &KNpcAttribModify::alljihuo_v;
	ProcessFunc[magic_addexp_v]  = &KNpcAttribModify::addexp_v;
	ProcessFunc[magic_doscript_v]  = &KNpcAttribModify::doscript_v;
    ProcessFunc[magic_me2metaldamage_p]  = &KNpcAttribModify::me2metaldamage_p;
	ProcessFunc[magic_metal2medamage_p]  = &KNpcAttribModify::metal2medamage_p;
	ProcessFunc[magic_me2wooddamage_p]  = &KNpcAttribModify::me2wooddamage_p;
	ProcessFunc[magic_wood2medamage_p]  = &KNpcAttribModify::wood2medamage_p;
	ProcessFunc[magic_me2waterdamage_p]  = &KNpcAttribModify::me2waterdamage_p;
	ProcessFunc[magic_water2medamage_p]  = &KNpcAttribModify::water2medamage_p;
	ProcessFunc[magic_me2firedamage_p]  = &KNpcAttribModify::me2firedamage_p;
	ProcessFunc[magic_fire2medamage_p]  = &KNpcAttribModify::fire2medamage_p;
	ProcessFunc[magic_me2earthdamage_p]  = &KNpcAttribModify::me2earthdamage_p;
	ProcessFunc[magic_earth2medamage_p]  = &KNpcAttribModify::earth2medamage_p;
	ProcessFunc[magic_manareplenish_p] = &KNpcAttribModify::ManaReplenishp;
	ProcessFunc[magic_fasthitrecover_p] = &KNpcAttribModify::fasthitrecover_p;
    ProcessFunc[magic_stuntrank_p] = &KNpcAttribModify::stuntrank_p;
	ProcessFunc[magic_sorbdamage_v] = &KNpcAttribModify::sorbdamage_v;
    ProcessFunc[magic_creatstatus_v] = &KNpcAttribModify::creatstatus_v;
	ProcessFunc[magic_randmove] = &KNpcAttribModify::randmove;
	ProcessFunc[magic_addbaopoisondmax_p] = &KNpcAttribModify::addbaopoisondmax_p;
	ProcessFunc[magic_dupotion_v] = &KNpcAttribModify::dupotion_v;
	ProcessFunc[magic_npcallattackSpeed_v] = &KNpcAttribModify::npcallattackSpeed_v;
	ProcessFunc[magic_eqaddskill_v] = &KNpcAttribModify::eqaddskill_v;
	ProcessFunc[magic_autodeathskill] = &KNpcAttribModify::autodeathskill;
	ProcessFunc[magic_autorescueskill] = &KNpcAttribModify::autorescueskill;
	ProcessFunc[magic_staticmagicshield_p] = &KNpcAttribModify::staticmagicshield_p;
	ProcessFunc[magic_ignorenegativestate_p] = &KNpcAttribModify::ignorenegativestate_p;
	//=============================��������==================================
	ProcessFunc[magic_poisonres_yan_p] = &KNpcAttribModify::poisonres_yan_p;
	ProcessFunc[magic_fireres_yan_p] = &KNpcAttribModify::fireres_yan_p;
	ProcessFunc[magic_lightingres_yan_p] = &KNpcAttribModify::lightingres_yan_p;
	ProcessFunc[magic_physicsres_yan_p] = &KNpcAttribModify::physicsres_yan_p;
	ProcessFunc[magic_coldres_yan_p] = &KNpcAttribModify::coldres_yan_p;
	ProcessFunc[magic_lifemax_yan_v] = &KNpcAttribModify::lifemax_yan_v;
	ProcessFunc[magic_lifemax_yan_p] = &KNpcAttribModify::lifemax_yan_p;
	ProcessFunc[magic_sorbdamage_yan_p] = &KNpcAttribModify::sorbdamage_yan_p;
	ProcessFunc[magic_attackspeed_yan_v] = &KNpcAttribModify::attackspeed_yan_v;
	ProcessFunc[magic_castspeed_yan_v] = &KNpcAttribModify::castspeed_yan_v;
	ProcessFunc[magic_allres_yan_p] = &KNpcAttribModify::allres_yan_p;
	ProcessFunc[magic_fasthitrecover_yan_v] = &KNpcAttribModify::fasthitrecover_yan_v;
	ProcessFunc[magic_anti_physicsres_yan_p] = &KNpcAttribModify::anti_physicsres_yan_p;
	ProcessFunc[magic_anti_poisonres_yan_p] = &KNpcAttribModify::anti_poisonres_yan_p;
	ProcessFunc[magic_anti_coldres_yan_p] = &KNpcAttribModify::anti_coldres_yan_p;
	ProcessFunc[magic_anti_fireres_yan_p] = &KNpcAttribModify::anti_fireres_yan_p;
	ProcessFunc[magic_anti_lightingres_yan_p] = &KNpcAttribModify::anti_lightingres_yan_p;
	ProcessFunc[magic_anti_allres_yan_p] = &KNpcAttribModify::anti_allres_yan_p;
	ProcessFunc[magic_anti_sorbdamage_yan_p] = &KNpcAttribModify::anti_sorbdamage_yan_p;
	ProcessFunc[magic_anti_hitrecover] = &KNpcAttribModify::anti_hitrecover;
	ProcessFunc[magic_do_hurt_p] = &KNpcAttribModify::do_hurt_p;
	ProcessFunc[magic_addskillexp1] = &KNpcAttribModify::addskillexp1;
	ProcessFunc[magic_anti_poisontimereduce_p] = &KNpcAttribModify::anti_poisontimereduce_p;
    ProcessFunc[magic_anti_stuntimereduce_p] = &KNpcAttribModify::anti_stuntimereduce_p;
	ProcessFunc[magic_addskilldamage8] = &KNpcAttribModify::addskilldamage8;
	ProcessFunc[magic_addskilldamage9] = &KNpcAttribModify::addskilldamage9;
	ProcessFunc[magic_addskilldamage10] = &KNpcAttribModify::addskilldamage10;
	/*
	magic_poisonres_yan_p,                            //=������#d1+%������
    magic_fireres_yan_p,                              //=�����#d1+%������
	magic_lightingres_yan_p,                    //=�׷���#d1+%������
	magic_physicsres_yan_p,                     //=�շ���#d1+%������
	magic_coldres_yan_p,                        //=������#d1+%������
	magic_lifemax_yan_v,                        //=�������ֵ��#d1+�㣨����
	magic_lifemax_yan_p,                        //=�������ֵ��#d1+%������
	magic_manamax_yan_v,                        //=�������ֵ��#d1+�㣨����
	magic_manamax_yan_p,                        //=�������ֵ��#d1+%������
	magic_sorbdamage_yan_p,                     //=�����˺���#d1+������
	magic_fastwalkrun_yan_p,                    //=�ƶ��ٶȣ�#d1+%������
	magic_attackspeed_yan_v,                    //=�⹦ϵ�书���ж�����#d1+%������
	magic_castspeed_yan_v,                      //=�ڹ�ϵ�书���ж�����#d1+%������
	magic_allres_yan_p,                         //=���п��ԣ�#d1+%������
	magic_fasthitrecover_yan_v,                 //=���˶���ʱ�䣺#d1~������
	magic_anti_physicsres_yan_p,                //=���ԶԷ��շ���#d1-%������
	magic_anti_poisonres_yan_p,                 //=���ԶԷ�������#d1-%������
	magic_anti_coldres_yan_p,                   //=���ԶԷ�������#d1-%������
	magic_anti_fireres_yan_p,                   //=���ԶԷ������#d1-%������
	magic_anti_lightingres_yan_p,               //=���ԶԷ��׷���#d1-%������
	magic_anti_allres_yan_p,                    //=���ԶԷ����п��ԣ�#d1-%������
	magic_anti_sorbdamage_yan_p,                //=�����˺���#d1+��������

	magic_me2metaldamage_p,//=�Խ�ϵ�˺����ӣ�#d1+%
    magic_metal2medamage_p,//=�������Խ�ϵ���˺���#d1-%
    magic_me2wooddamage_p,//=��ľϵ�˺����ӣ�#d1+%
    magic_wood2medamage_p,//=��������ľϵ���˺���#d1-%
    magic_me2waterdamage_p,//=��ˮϵ�˺����ӣ�#d1+%
    magic_water2medamage_p,//=��������ˮϵ���˺���#d1-%
    magic_me2firedamage_p,//=�Ի�ϵ�˺����ӣ�#d1+%
    magic_fire2medamage_p,//=�������Ի�ϵ���˺���#d1-%
    magic_me2earthdamage_p,//=����ϵ�˺����ӣ�#d1+%
    magic_earth2medamage_p,//=����������ϵ���˺���#d1-%
	
	
	*/
		
}

KNpcAttribModify::~KNpcAttribModify()
{
	
}
//װ���ͼ�����������Ӧ�õ�NPC����
void KNpcAttribModify::ModifyAttrib(KNpc* pNpc, void* pData,int nAttacker)
{
	if (!pData || !pNpc)
		return;

	KMagicAttrib* pMagic = (KMagicAttrib *)pData;

	if (pMagic->nAttribType < 0 || pMagic->nAttribType >= magic_normal_end || NULL == ProcessFunc[pMagic->nAttribType])
		return;
	
	(this->*ProcessFunc[pMagic->nAttribType])(pNpc, pData);  //����ָ��  
}

//ȫ��
void KNpcAttribModify::AllresP(KNpc* pNpc, void* pData)
{
	KMagicAttrib* pMagic = (KMagicAttrib *)pData;
	int nRes=0;
	int nRer=0;
	int nIsadd=0;

	/*pNpc->m_CurrentFireResist    += pMagic->nValue[0];
	pNpc->m_CurrentColdResist    += pMagic->nValue[0];
	pNpc->m_CurrentLightResist   += pMagic->nValue[0];
	pNpc->m_CurrentPoisonResist  += pMagic->nValue[0];
	pNpc->m_CurrentPhysicsResist += pMagic->nValue[0];*/
//--------���	
	pNpc->m_TempFireResist += pMagic->nValue[0];
	
	if (pNpc->m_TempFireResist>=0)
	{
		nIsadd=1;
		nRes=(pNpc->m_TempFireResist*BASE_FANGYU_ALL_MAX)/100;//TakeTrader(pNpc->m_TempFireResist*BASE_FANGYU_ALL_MAX,100); //ȡ��
		nRer=(pNpc->m_TempFireResist*BASE_FANGYU_ALL_MAX)%100;;//TakeRemainder(pNpc->m_TempFireResist*BASE_FANGYU_ALL_MAX,100);
		
		if (pNpc->m_TempFireResist==1)
			nRes=1;
		
		if (nRer>=50)
			nRes +=1; //4��5��
		
	}
	else
	{
		nRes=(-pNpc->m_TempFireResist*BASE_FANGYU_ALL_MAX)/100;//TakeTrader(-pNpc->m_TempFireResist*BASE_FANGYU_ALL_MAX,100); //ȡ��
		nRer=(-pNpc->m_TempFireResist*BASE_FANGYU_ALL_MAX)%100;//TakeTrader(-pNpc->m_TempFireResist*BASE_FANGYU_ALL_MAX,100);
		
		if  (-pNpc->m_TempFireResist==1)
			nRes=1;
		
		if (nRer>=50)
			nRes +=1; //4��5��
	}
	
	
	if (nIsadd==1)	//����
        pNpc->m_CurrentFireResist=pNpc->m_FireResist+nRes;
	else           //����
		pNpc->m_CurrentFireResist=pNpc->m_FireResist-nRes;
//--------����	
	nRes=0;
	nRer=0;
	nIsadd=0;
	pNpc->m_TempColdResist += pMagic->nValue[0];
	
	if (pNpc->m_TempColdResist>=0)
	{
		nIsadd=1;
		nRes=(pNpc->m_TempColdResist*BASE_FANGYU_ALL_MAX)/100;//TakeTrader(pNpc->m_TempColdResist*BASE_FANGYU_ALL_MAX,100); //ȡ��
		nRer=(pNpc->m_TempColdResist*BASE_FANGYU_ALL_MAX)%100;//TakeRemainder(pNpc->m_TempColdResist*BASE_FANGYU_ALL_MAX,100);
		
		if (pNpc->m_TempColdResist==1)
			nRes=1;
		
		if (nRer>=50)
			nRes +=1; //4��5��
		
	}
	else
	{
		nRes=(-pNpc->m_TempColdResist*BASE_FANGYU_ALL_MAX)/100;//TakeTrader(-pNpc->m_TempColdResist*BASE_FANGYU_ALL_MAX,100); //ȡ��
		nRer=(-pNpc->m_TempColdResist*BASE_FANGYU_ALL_MAX)%100;//TakeTrader(-pNpc->m_TempColdResist*BASE_FANGYU_ALL_MAX,100);
		
		if  (-pNpc->m_TempColdResist==1)
			nRes=1;
		
		if (nRer>=50)
			nRes +=1; //4��5��
	}
	
	
	if (nIsadd==1)	//����
        pNpc->m_CurrentColdResist=pNpc->m_ColdResist+nRes;
	else           //����
		pNpc->m_CurrentColdResist=pNpc->m_ColdResist-nRes;

//--------�׷�	
	nRes=0;
	nRer=0;
	nIsadd=0;
	pNpc->m_TempLightResist += pMagic->nValue[0];
	
	if (pNpc->m_TempLightResist>=0)
	{
		nIsadd=1;
		nRes=(pNpc->m_TempLightResist*BASE_FANGYU_ALL_MAX)/100;//TakeTrader(pNpc->m_TempLightResist*BASE_FANGYU_ALL_MAX,100); //ȡ��
		nRer=(pNpc->m_TempLightResist*BASE_FANGYU_ALL_MAX)%100;//TakeRemainder(pNpc->m_TempLightResist*BASE_FANGYU_ALL_MAX,100);
		
		if (pNpc->m_TempLightResist==1)
			nRes=1;
		
		if (nRer>=50)
			nRes +=1; //4��5��
		
	}
	else
	{
		nRes=(-pNpc->m_TempLightResist*BASE_FANGYU_ALL_MAX)/100;//TakeTrader(-pNpc->m_TempLightResist*BASE_FANGYU_ALL_MAX,100); //ȡ��
		nRer=(-pNpc->m_TempLightResist*BASE_FANGYU_ALL_MAX)%100;//TakeTrader(-pNpc->m_TempLightResist*BASE_FANGYU_ALL_MAX,100);
		
		if  (-pNpc->m_TempLightResist==1)
			nRes=1;
		
		if (nRer>=50)
			nRes +=1; //4��5��
	}
	
	
	if (nIsadd==1)	//����
        pNpc->m_CurrentLightResist=pNpc->m_LightResist+nRes;
	else           //����
		pNpc->m_CurrentLightResist=pNpc->m_LightResist-nRes;
//--------�շ�	
	nRes=0;
	nRer=0;
	nIsadd=0;
	pNpc->m_TempPhysicsResist += pMagic->nValue[0];
	
	if (pNpc->m_TempPhysicsResist>=0)
	{
		nIsadd=1;
		nRes=(pNpc->m_TempPhysicsResist*BASE_FANGYU_ALL_MAX)/100;//TakeTrader(pNpc->m_TempPhysicsResist*BASE_FANGYU_ALL_MAX,100); //ȡ��
		nRer=(pNpc->m_TempPhysicsResist*BASE_FANGYU_ALL_MAX)%100;//TakeRemainder(pNpc->m_TempPhysicsResist*BASE_FANGYU_ALL_MAX,100);
		
		if (pNpc->m_TempPhysicsResist==1)
			nRes=1;
		
		if (nRer>=50)
			nRes +=1; //4��5��
		
	}
	else
	{
		nRes=(-pNpc->m_TempPhysicsResist*BASE_FANGYU_ALL_MAX)/100;//TakeTrader(-pNpc->m_TempPhysicsResist*BASE_FANGYU_ALL_MAX,100); //ȡ��
		nRer=(-pNpc->m_TempPhysicsResist*BASE_FANGYU_ALL_MAX)%100;//TakeTrader(-pNpc->m_TempPhysicsResist*BASE_FANGYU_ALL_MAX,100);
		
		if  (-pNpc->m_TempPhysicsResist==1)
			nRes=1;
		
		if (nRer>=50)
			nRes +=1; //4��5��
	}
	
	
	if (nIsadd==1)	//����
        pNpc->m_CurrentPhysicsResist=pNpc->m_PhysicsResist+nRes;
	else           //����
		pNpc->m_CurrentPhysicsResist=pNpc->m_PhysicsResist-nRes;


//--------����	
	nRes=0;
	nRer=0;
	nIsadd=0;
	pNpc->m_TempPoisonResist += pMagic->nValue[0];
	
	if (pNpc->m_TempPoisonResist>=0)
	{
		nIsadd=1;
		nRes=(pNpc->m_TempPoisonResist*BASE_FANGYU_ALL_MAX)/100;//TakeTrader(pNpc->m_TempPoisonResist*BASE_FANGYU_ALL_MAX,100); //ȡ��
		nRer=(pNpc->m_TempPoisonResist*BASE_FANGYU_ALL_MAX)%100;//TakeRemainder(pNpc->m_TempPoisonResist*BASE_FANGYU_ALL_MAX,100);
		
		if (pNpc->m_TempPoisonResist==1)
			nRes=1;
		
		if (nRer>=50)
			nRes +=1; //4��5��
		
	}
	else
	{
		nRes=(-pNpc->m_TempPoisonResist*BASE_FANGYU_ALL_MAX)/100;//TakeTrader(-pNpc->m_TempPoisonResist*BASE_FANGYU_ALL_MAX,100); //ȡ��
		nRer=(-pNpc->m_TempPoisonResist*BASE_FANGYU_ALL_MAX)%100;//TakeTrader(-pNpc->m_TempPoisonResist*BASE_FANGYU_ALL_MAX,100);
		
		if  (-pNpc->m_TempPoisonResist==1)
			nRes=1;
		
		if (nRer>=50)
			nRes +=1; //4��5��
	}
	
	
	if (nIsadd==1)	//����
        pNpc->m_CurrentPoisonResist=pNpc->m_PoisonResist+nRes;
	else           //����
		pNpc->m_CurrentPoisonResist=pNpc->m_PoisonResist-nRes;

//---------------------------------------------------------
   
	if (pNpc->m_CurrentFireResist>BASE_FANGYU_ALL_MAX)
		pNpc->m_CurrentFireResistMax    = pNpc->m_CurrentFireResist;
	   
	if (pNpc->m_CurrentColdResist>BASE_FANGYU_ALL_MAX)
		pNpc->m_CurrentColdResistMax    = pNpc->m_CurrentColdResist;

	if (pNpc->m_CurrentLightResist>BASE_FANGYU_ALL_MAX)
	   pNpc->m_CurrentLightResistMax    = pNpc->m_CurrentLightResist;
	
	if (pNpc->m_CurrentPoisonResist>BASE_FANGYU_ALL_MAX)   
	   pNpc->m_CurrentPoisonResistMax   = pNpc->m_CurrentPoisonResist;

	if (pNpc->m_CurrentPhysicsResist>BASE_FANGYU_ALL_MAX) 
	   pNpc->m_CurrentPhysicsResistMax  = pNpc->m_CurrentPhysicsResist;

	if (pNpc->m_CurrentPoisonResistMax > MAX_RESIST)
        pNpc->m_CurrentPoisonResistMax = MAX_RESIST;

	if (pNpc->m_CurrentLightResistMax > MAX_RESIST)
        pNpc->m_CurrentLightResistMax = MAX_RESIST;

	if (pNpc->m_CurrentFireResistMax > MAX_RESIST)
        pNpc->m_CurrentFireResistMax = MAX_RESIST;

	if (pNpc->m_CurrentColdResistMax > MAX_RESIST)
        pNpc->m_CurrentColdResistMax = MAX_RESIST;

	if (pNpc->m_CurrentPhysicsResistMax > MAX_RESIST)
        pNpc->m_CurrentPhysicsResistMax = MAX_RESIST;


	if (pNpc->m_CurrentPoisonResistMax<=BASE_FANGYU_ALL_MAX)
		pNpc->m_CurrentPoisonResistMax=BASE_FANGYU_ALL_MAX;

	if (pNpc->m_CurrentLightResistMax<=BASE_FANGYU_ALL_MAX)
		pNpc->m_CurrentLightResistMax=BASE_FANGYU_ALL_MAX;

	if (pNpc->m_CurrentFireResistMax<=BASE_FANGYU_ALL_MAX)
		pNpc->m_CurrentFireResistMax=BASE_FANGYU_ALL_MAX;

	if (pNpc->m_CurrentColdResistMax<=BASE_FANGYU_ALL_MAX)
		pNpc->m_CurrentColdResistMax=BASE_FANGYU_ALL_MAX;

	if (pNpc->m_CurrentPhysicsResistMax<=BASE_FANGYU_ALL_MAX)
		pNpc->m_CurrentPhysicsResistMax=BASE_FANGYU_ALL_MAX;


	if (pNpc->m_CurrentPoisonResist > pNpc->m_CurrentPoisonResistMax)
		  pNpc->m_CurrentPoisonResist = pNpc->m_CurrentPoisonResistMax;
	if (pNpc->m_CurrentLightResist > pNpc->m_CurrentLightResistMax)
		  pNpc->m_CurrentLightResist  = pNpc->m_CurrentLightResistMax;
	if (pNpc->m_CurrentFireResist > pNpc->m_CurrentFireResistMax)
		  pNpc->m_CurrentFireResist   = pNpc->m_CurrentFireResistMax;
	if (pNpc->m_CurrentColdResist >pNpc->m_CurrentColdResistMax)
		  pNpc->m_CurrentColdResist   = pNpc->m_CurrentColdResistMax;
	if (pNpc->m_CurrentPhysicsResist >pNpc->m_CurrentPhysicsResistMax)
		  pNpc->m_CurrentPhysicsResist= pNpc->m_CurrentPhysicsResistMax;


/*	if (pNpc->m_CurrentPoisonResistMax<=0 ||\
	        pNpc->m_CurrentLightResistMax<=0 ||\
			pNpc->m_CurrentFireResistMax<=0 ||\
			pNpc->m_CurrentColdResistMax<=0 ||\
			pNpc->m_CurrentPhysicsResistMax<=0)
	{//������������Բ���
		 if (pNpc->IsPlayer())
	        Player[pNpc->m_nPlayerIdx].UpdataCurData(); 
		 else
		 {
		   pNpc->m_CurrentPoisonResistMax = MAX_RESIST;
		   pNpc->m_CurrentLightResistMax  = MAX_RESIST;
		   pNpc->m_CurrentFireResistMax = MAX_RESIST;
		   pNpc->m_CurrentColdResistMax = MAX_RESIST;
		   pNpc->m_CurrentPhysicsResistMax = MAX_RESIST;
		 }

    } */
	//g_DebugLog("[��ֵ]%s�����������%d", pNpc->Name, pMagic->nValue[0]);
}
//���Ӽ��ܵȼ� ���� �� װ������
void KNpcAttribModify::AllSkillV(KNpc* pNpc, void* pData)
{
	
	if(!pNpc->IsPlayer())
		return;

	KMagicAttrib* pMagic = (KMagicAttrib *)pData;

	/*if (pMagic->nValue[2] != 0 && pMagic->nValue[2]<=2000 && pMagic->nValue[2]>=-2000) //����ID
	{
		int Same,nSkillPos=0;
		if (pMagic->nValue[2] > 0)
			Same = pMagic->nValue[2];
		else
			Same = -pMagic->nValue[2];


#ifdef _SERVER
		char Msg[64];
		sprintf(Msg,"1S:���ܵȼ�(%d)(%d)(%d)�ɹ�",Same,pMagic->nValue[0],pMagic->nValue[2]);
		Player[pNpc->GetPlayerIdx()].m_ItemList.msgshow(Msg);	
#else
		char Msg[64];
		sprintf(Msg,"1C:���ܵȼ�(%d)(%d)(%d)�ɹ�",Same,pMagic->nValue[0],pMagic->nValue[2]);
		Player[CLIENT_PLAYER_INDEX].m_ItemList.ClientShowMsg(Msg);	
#endif


        nSkillPos=pNpc->m_SkillList.FindSame(Same);

		if (nSkillPos)  //���Ҹü����ڼ����б��е�λ��
		{//int nRet = Npc[Player[nPlayerIndex].m_nIndex].m_SkillList.Add(nSkillId, nLevel);
			if (pNpc->m_SkillList.GetCurrentLevel(Same) > 0)
			{//�ȼ�������ʱ			
				pNpc->m_SkillList.QeuipAddPoint(nSkillPos,pMagic->nValue[0]); //�������ӻ���ٵĵļ��ܵ�                             
#ifdef _SERVER	
				// �ѵ�ǰ�˼��ܵĵ�����ʣ�༼�ܵ㷢���ͻ���	
				pNpc->m_SkillList.IncreaseLevel(nSkillPos,pMagic->nValue[0]); //�����趨��������
				//Player[pNpc->m_nPlayerIdx].UpdataCurData();
				PLAYER_SKILL_LEVEL_SYNC	sSkill;
				sSkill.ProtocolType  = s2c_playerskilllevel;
				sSkill.m_nSkillID    = Same;
				sSkill.m_nSkillLevel = pNpc->m_SkillList.GetCurrentLevel(Same);
				sSkill.m_nLeavePoint = Player[pNpc->m_nPlayerIdx].m_nSkillPoint;
				sSkill.m_nAddPoint   = pNpc->m_SkillList.GetAddPoint(nSkillPos);
				g_pServer->PackDataToClient(Player[pNpc->m_nPlayerIdx].m_nNetConnectIdx, (BYTE*)&sSkill, sizeof(PLAYER_SKILL_LEVEL_SYNC));
#endif
			}			
		}
		else
		{//
          //int nRet = Npc[Player[nPlayerIndex].m_nIndex].m_SkillList.Add(Same, 1);
		}
	}
    else */
	if (pMagic->nValue[2]!= 0)//����ID
	{//�����ɼ���
        int  nSkillRows=0,nSkillid=0;
		if (pMagic->nValue[2] > 0)

			nSkillid = pMagic->nValue[2];
		else
			nSkillid = -pMagic->nValue[2];

		//KTabFile nMagci;
            // if (nMagci.Load(TABFILE_MAGICAGOLD_PATH))
			 {
                   // int nSkillid=0;
                   // nMagci.GetInteger(nSkillRows,"����3��Сֵ",0,&nSkillid);

/*#ifdef _SERVER
					char Msg[64];
					sprintf(Msg,"2S:���ܵȼ�(%d)(%d)(%d)�ɹ�",nSkillid,pMagic->nValue[0],pMagic->nValue[2]);
					Player[pNpc->GetPlayerIdx()].m_ItemList.msgshow(Msg);	
#endif

#ifndef _SERVER

					char Msg[64];
					sprintf(Msg,"2C:���ܵȼ�(%d)(%d)(%d)�ɹ�",nSkillid,pMagic->nValue[0],pMagic->nValue[2]);
					Player[CLIENT_PLAYER_INDEX].m_ItemList.ClientShowMsg(Msg);	
#endif */			int nSkillIdx=pNpc->m_SkillList.FindSame(nSkillid);

					if (nSkillIdx)
					{//int nRet = Npc[Player[nPlayerIndex].m_nIndex].m_SkillList.Add(nSkillId, nLevel);
						if (pNpc->m_SkillList.GetCurrentLevelByIdx(nSkillIdx) > 0)
						{
							pNpc->m_SkillList.QeuipAddPoint(nSkillIdx,pMagic->nValue[0]); 
						}			
					} 
			 }
			 //nMagci.Clear();
	} 
	else if (pMagic->nValue[2] == 0) 
	{//�����м��ܼ� �ȼ�
		for (int i = 1;i < MAX_NPCSKILL;++i)
		{//int nRet = Npc[Player[nPlayerIndex].m_nIndex].m_SkillList.Add(nSkillId, nLevel);
/*			
#ifdef _SERVER
			char Msg[64];
			sprintf(Msg,"3S:���ܵȼ�(%d)(%d)(%d)�ɹ�",pNpc->m_SkillList.m_Skills[i].SkillId,pMagic->nValue[0],pMagic->nValue[2]);
			Player[pNpc->GetPlayerIdx()].m_ItemList.msgshow(Msg);	
#else
			char Msg[64];
			sprintf(Msg,"3C:���ܵȼ�(%d)(%d)(%d)�ɹ�",pNpc->m_SkillList.m_Skills[i].SkillId,pMagic->nValue[0],pMagic->nValue[2]);
			Player[CLIENT_PLAYER_INDEX].m_ItemList.ClientShowMsg(Msg);	
#endif
*/			

			int mReg=0;
		        mReg=pNpc->m_SkillList.CheckNoSkill("NoCanXiSui",pNpc->m_SkillList.m_Skills[i].SkillId);

		    if (mReg)
				continue;

			if(pNpc->m_SkillList.m_Skills[i].SkillId && pNpc->m_SkillList.m_Skills[i].SkillId!=1 && pNpc->m_SkillList.m_Skills[i].SkillId!=2 && pNpc->m_SkillList.m_Skills[i].SkillId!=53)
			{
				if (pNpc->m_SkillList.GetCurrentLevelByIdx(i) > 0)
				{
					pNpc->m_SkillList.QeuipAddPoint(i,pMagic->nValue[0]);//��λ�õļ����ܵ�
				}
			}
		}
	}
//	//g_DebugLog("[��ֵ]%s���м��ܼ�%d��", pNpc->Name, pMagic->nValue[0]);
}

 
//���ܺ��Եз�������
void KNpcAttribModify::Ignoredefense_p(KNpc* pNpc, void* pData)
{
	KMagicAttrib* pMagic = (KMagicAttrib *)pData;

	pNpc->m_CurrentIgnoredefensep += pMagic->nValue[0];


	if (pNpc->m_CurrentIgnoredefensep<=0)
		pNpc->m_CurrentIgnoredefensep = 0;

	if (pNpc->m_CurrentIgnoredefensep >100)
		pNpc->m_CurrentIgnoredefensep = 100;


	//g_DebugLog("[��ֵ]%s���Եз������ʰٷ�֮%d", pNpc->Name, pMagic->nValue[0], pNpc->m_AttackRating * pMagic->nValue[0] / 100);
}

//�������аٷֱ�
void KNpcAttribModify::AttackRatingP(KNpc* pNpc, void* pData)
{
	KMagicAttrib* pMagic = (KMagicAttrib *)pData;
	pNpc->m_CurrentAttackRating += pNpc->m_AttackRating * pMagic->nValue[0] / 100;
	//g_DebugLog("[��ֵ]%s���������Ӱٷ�֮%d", pNpc->Name, pMagic->nValue[0], pNpc->m_AttackRating * pMagic->nValue[0] / 100);
}
//�������е�
void KNpcAttribModify::AttackRatingV(KNpc* pNpc, void* pData)
{
	KMagicAttrib* pMagic = (KMagicAttrib *)pData;
	pNpc->m_CurrentAttackRating += pMagic->nValue[0];
	//g_DebugLog("[��ֵ]%s����������%d��", pNpc->Name, pMagic->nValue[0]);
}
//װ��״̬������
void KNpcAttribModify::AddAttackRatingV(KNpc* pNpc, void* pData)
{
   	KMagicAttrib* pMagic = (KMagicAttrib *)pData;
	pNpc->m_CurrentAttackRatingEnhancev += pMagic->nValue[0];
}
//װ��״̬�����ʰٷֱ�
void KNpcAttribModify::AddAttackRatingP(KNpc* pNpc, void* pData)
{
   	KMagicAttrib* pMagic = (KMagicAttrib *)pData;
	pNpc->m_CurrentAttackRatingEnhancep += pMagic->nValue[0];

    if (pNpc->m_CurrentAttackRatingEnhancep<-100)
	   pNpc->m_CurrentAttackRatingEnhancep=-100;
	//g_DebugLog("[��ֵ]%s���������Ӱٷֱ�%d", pNpc->Name, pMagic->nValue[0]);
}
//////////////////��������--------------------------------------
void KNpcAttribModify::lightingres_yan_p( KNpc* pNpc, void* pData)
{
	
}

void KNpcAttribModify::fireres_yan_p( KNpc* pNpc, void* pData)
{

}

void KNpcAttribModify::physicsres_yan_p( KNpc* pNpc, void* pData)
{
	
}

void KNpcAttribModify::poisonres_yan_p( KNpc* pNpc, void* pData)
{
	
}


void KNpcAttribModify::lifemax_yan_p( KNpc* pNpc, void* pData)
{
	
}

void KNpcAttribModify::lifemax_yan_v( KNpc* pNpc, void* pData)
{
	
}
void KNpcAttribModify::coldres_yan_p( KNpc* pNpc, void* pData)
{
	
}

void KNpcAttribModify::manamax_yan_v( KNpc* pNpc, void* pData)
{
	
}
void KNpcAttribModify::manamax_yan_p( KNpc* pNpc, void* pData)
{
	
}

void KNpcAttribModify::sorbdamage_yan_p( KNpc* pNpc, void* pData)
{
	
}

void KNpcAttribModify::fastwalkrun_yan_p( KNpc* pNpc, void* pData)
{
	
}

void KNpcAttribModify::attackspeed_yan_v( KNpc* pNpc, void* pData)
{
	
}

void KNpcAttribModify::castspeed_yan_v( KNpc* pNpc, void* pData)
{
	
}
void KNpcAttribModify::allres_yan_p( KNpc* pNpc, void* pData)
{
	
}
void KNpcAttribModify::fasthitrecover_yan_v( KNpc* pNpc, void* pData)
{
	
}
void KNpcAttribModify::anti_physicsres_yan_p( KNpc* pNpc, void* pData)
{
	
}
void KNpcAttribModify::anti_poisonres_yan_p( KNpc* pNpc, void* pData)
{
	
}
void KNpcAttribModify::anti_coldres_yan_p( KNpc* pNpc, void* pData)
{
	
}
void KNpcAttribModify::anti_fireres_yan_p( KNpc* pNpc, void* pData)
{
	
}
void KNpcAttribModify::anti_lightingres_yan_p( KNpc* pNpc, void* pData)
{
	
}
void KNpcAttribModify::anti_allres_yan_p( KNpc* pNpc, void* pData)
{
	
}
void KNpcAttribModify::anti_sorbdamage_yan_p( KNpc* pNpc, void* pData)
{
	
}
//��ɶԷ����˶���ʱ�䣺#d1+
void KNpcAttribModify::anti_hitrecover( KNpc* pNpc, void* pData)
{
	KMagicAttrib* pMagic = (KMagicAttrib *)pData;
    pNpc->m_CurrentHitNpcRecover +=pMagic->nValue[0];

	if (pNpc->m_CurrentHitNpcRecover<0)
		pNpc->m_CurrentHitNpcRecover=0;
	
}

void    KNpcAttribModify::addskillexp1( KNpc* pNpc, void* pData)
{

}

//������˶�������
void KNpcAttribModify::do_hurt_p( KNpc* pNpc, void* pData)
{
	KMagicAttrib* pMagic = (KMagicAttrib *)pData;
    pNpc->m_CurrentHitRank +=pMagic->nValue[0]; 
}

//�Ը��ָ���״̬�ͼ�������
void KNpcAttribModify::ignorenegativestate_p( KNpc* pNpc, void* pData)
{
	KMagicAttrib* pMagic = (KMagicAttrib *)pData;
	
}

//ħ���� �ɵ����������ֵ�Ķ��ٱ���
void KNpcAttribModify::staticmagicshield_p( KNpc* pNpc, void* pData)
{
	KMagicAttrib* pMagic = (KMagicAttrib *)pData;

	pNpc->m_Staticmagicshield_p +=pMagic->nValue[0]/100;

}
//��������
void KNpcAttribModify::autorescueskill( KNpc* pNpc, void* pData)
{
}

//�����Զ��ͷż���--ħ���ɻ�
void KNpcAttribModify::autodeathskill( KNpc* pNpc, void* pData)
{
   KMagicAttrib* pMagic = (KMagicAttrib *)pData;
   if (pMagic->nValue[0]>0)	//����ID
   {                        //���Ӽ���--��װ
	   pNpc->m_Deathkill.nValue[0] = pMagic->nValue[0];	//����ID
	   pNpc->m_Deathkill.nValue[1] = pMagic->nValue[2]; //����
	   pNpc->m_Deathkill.nTime     = pMagic->nValue[1];	//�м�ֵ
   }
   else
   {
	   pNpc->m_Deathkill.nValue[0] = 0;	//����ID
	   pNpc->m_Deathkill.nValue[1] = 0; //����
	   pNpc->m_Deathkill.nTime     = 0;	//�м�ֵ
   }

}

void KNpcAttribModify::eqaddskill_v( KNpc* pNpc, void* pData)
{
}

//��������--NPC����
void KNpcAttribModify::npcallattackSpeed_v(KNpc* pNpc, void* pData)
{	
}


//��������--�⹦
void KNpcAttribModify::AttackSpeedV(KNpc* pNpc, void* pData)
{
}
//�ڹ���
void KNpcAttribModify::CastSpeedV(KNpc* pNpc, void* pData)
{
}
//����״̬ʱ������
void KNpcAttribModify::BadStatusTimeReduceV(KNpc* pNpc, void* pData)
{
	KMagicAttrib* pMagic = (KMagicAttrib *)pData;

// if (pNpc->m_PowerState.nTime>0)			// ŭ��״̬
//            pNpc->m_PowerState.nTime+=pMagic->nValue[0];
   if (pNpc->m_FreezeState.nTime>0)
            pNpc->m_FreezeState.nTime +=pMagic->nValue[0];
   if (pNpc->m_BurnState.nTime>0)
            pNpc->m_BurnState.nTime +=pMagic->nValue[0];
   if (pNpc->m_ConfuseState.nTime>0)
            pNpc->m_ConfuseState.nTime +=pMagic->nValue[0];
   if (pNpc->m_StunState.nTime>0)
            pNpc->m_StunState.nTime +=pMagic->nValue[0];

/*  pNpc->m_PoisonState.nTime;			// �ж�״̬
    pNpc->m_FreezeState.nTime;			// ����״̬
//	pNpc->m_CurrentFreezeTimeReducePercent+=pMagic->nValue[0];  // ����ʱ����ٰٷֱ�
//	pNpc->m_CurrentPoisonTimeReducePercent+=pMagic->nValue[0];	// �ж�ʱ����ٰٷֱ�
//	pNpc->m_CurrentStunTimeReducePercent  +=pMagic->nValue[0];  // ѣ��ʱ����ٰٷֱ�
    pNpc->m_BurnState.nTime;			// ȼ��״̬
    pNpc->m_ConfuseState.nTime;			// ����״̬
    pNpc->m_StunState.nTime;*/			// ѣ��״̬
	return;
}
//������ʱ�����
void KNpcAttribModify::BurnTimeReduceP(KNpc* pNpc, void* pData)
{
    KMagicAttrib* pMagic = (KMagicAttrib *)pData;
	pNpc->m_CurrentBurnTimeReducePercent +=pMagic->nValue[0];
	return;
}

//�Ի����  �ı���Ӫ
void KNpcAttribModify::ChangeCampV(KNpc* pNpc, void* pData)
{
	KMagicAttrib* pMagic = (KMagicAttrib *)pData;
	if (pNpc->m_Kind != kind_player)
	{
		if (pMagic->nValue[0] > 0 && pMagic->nValue[0] < camp_num)
			pNpc->m_CurrentCamp = pMagic->nValue[0];
		else
			pNpc->m_CurrentCamp = pNpc->m_Camp;
		//g_DebugLog("[��ֵ]%s��ǰ��Ӫ��Ϊ%d", pNpc->Name, pMagic->nValue[0]);
	}
	
}
//��״̬�ĳ���ʱ��
void KNpcAttribModify::ColdArmorV(KNpc* pNpc, void* pData)
{
	KMagicAttrib* pMagic = (KMagicAttrib *)pData;
	if (pMagic->nValue[0] > 0)
	{
		pNpc->m_ColdArmor.nValue[0] = pMagic->nValue[0];
		pNpc->m_ColdArmor.nTime = pMagic->nValue[1];
		//g_DebugLog("[��ֵ]%s�����ܱ�Ϊ%d�����%d֡", pNpc->Name, pMagic->nValue[0], pMagic->nValue[1]);	
	}
	else
	{
		pNpc->m_ColdArmor.nValue[0] = 0;
		pNpc->m_ColdArmor.nTime = 0;
		//g_DebugLog("[��ֵ]%s��������ʧ", pNpc->Name);	
	}
}

void KNpcAttribModify::ColdresP(KNpc* pNpc, void* pData)
{
	KMagicAttrib* pMagic = (KMagicAttrib *)pData;
	int nRes=0;
	int nRer=0;
	int nIsadd=0;
	
	pNpc->m_TempColdResist += pMagic->nValue[0];
	
	if (pNpc->m_TempColdResist>=0)
	{
		nIsadd=1;
		nRes=(pNpc->m_TempColdResist*BASE_FANGYU_ALL_MAX)/100;//TakeTrader(pNpc->m_TempColdResist*BASE_FANGYU_ALL_MAX,100); //ȡ��
		nRer=(pNpc->m_TempColdResist*BASE_FANGYU_ALL_MAX)%100;//TakeRemainder(pNpc->m_TempColdResist*BASE_FANGYU_ALL_MAX,100);
		
		if (pNpc->m_TempColdResist==1)
			nRes=1;
		
		if (nRer>=50)
			nRes +=1; //4��5��
		
	}
	else
	{
		nRes=(-pNpc->m_TempColdResist*BASE_FANGYU_ALL_MAX)/100;//TakeTrader(-pNpc->m_TempColdResist*BASE_FANGYU_ALL_MAX,100); //ȡ��
		nRer=(-pNpc->m_TempColdResist*BASE_FANGYU_ALL_MAX)%100;//TakeTrader(-pNpc->m_TempColdResist*BASE_FANGYU_ALL_MAX,100);
		
		if  (-pNpc->m_TempColdResist==1)
			nRes=1;
		
		if (nRer>=50)
			nRes +=1; //4��5��
	}
	
	
	if (nIsadd==1)	//����
        pNpc->m_CurrentColdResist=pNpc->m_ColdResist+nRes;
	else           //����
		pNpc->m_CurrentColdResist=pNpc->m_ColdResist-nRes;


	if (pNpc->m_CurrentColdResist>BASE_FANGYU_ALL_MAX)
	    pNpc->m_CurrentColdResistMax = pNpc->m_CurrentColdResist;
	
	if (pNpc->m_CurrentColdResistMax > MAX_RESIST)
        pNpc->m_CurrentColdResistMax = MAX_RESIST;

	if (pNpc->m_CurrentColdResistMax<=BASE_FANGYU_ALL_MAX)
		pNpc->m_CurrentColdResistMax=BASE_FANGYU_ALL_MAX;

	if (pNpc->m_CurrentColdResist >pNpc->m_CurrentColdResistMax)
		pNpc->m_CurrentColdResist =pNpc->m_CurrentColdResistMax;

	//g_DebugLog("[��ֵ]%s������������%d��", pNpc->Name, pMagic->nValue[0]);
}
//����һ���ٷֱ�
void KNpcAttribModify::DeadlyStrikeP(KNpc* pNpc, void* pData)
{
	KMagicAttrib* pMagic = (KMagicAttrib *)pData;
	pNpc->m_CurrentDeadlyStrike += pMagic->nValue[0];
	//g_DebugLog("[��ֵ]%s����һ���ٷֱ�����%d", pNpc->Name, pMagic->nValue[0]);
}

void KNpcAttribModify::DexterityV(KNpc* pNpc, void* pData)
{
	KMagicAttrib* pMagic = (KMagicAttrib *)pData;
	if (pNpc->m_Kind != kind_player)
		return;
	if (pNpc->m_nPlayerIdx <= 0)
		return;
	Player[pNpc->m_nPlayerIdx].ChangeCurDexterity(pMagic->nValue[0]);
	//g_DebugLog("[��ֵ]%s��ǰ����������%d��", pNpc->Name, pMagic->nValue[0]);	
}

void KNpcAttribModify::EarthSkillV(KNpc* pNpc, void* pData)
{
	if(!pNpc->IsPlayer())
		return;

	KMagicAttrib* pMagic = (KMagicAttrib *)pData;
	
    if (pNpc->m_Series == 4) //����ϵ���м��ܼ� �ȼ�
	{
		for (int i = 1;i < MAX_NPCSKILL;++i)
		{
			int mReg=0;
			
		        mReg=pNpc->m_SkillList.CheckNoSkill("NoCanXiSui",pNpc->m_SkillList.m_Skills[i].SkillId);

			if (mReg)
				continue;

			if(pNpc->m_SkillList.m_Skills[i].SkillId && pNpc->m_SkillList.m_Skills[i].SkillId!=1&&pNpc->m_SkillList.m_Skills[i].SkillId!=2&&pNpc->m_SkillList.m_Skills[i].SkillId!=53)
			{
				if (pNpc->m_SkillList.GetCurrentLevelByIdx(i) > 0)
				{
					pNpc->m_SkillList.QeuipAddPoint(i,pMagic->nValue[0]);
				}
			}
		}
	}
	//g_DebugLog("[��ֵ]%sľϵ���м��ܼ�%d��", pNpc->Name, pMagic->nValue[0]);
}


void KNpcAttribModify::EnergyV(KNpc* pNpc, void* pData)
{
	KMagicAttrib* pMagic = (KMagicAttrib *)pData;
	if (pNpc->m_Kind != kind_player)
		return;
	if (pNpc->m_nPlayerIdx <= 0)
		return;
	Player[pNpc->m_nPlayerIdx].ChangeCurEngergy(pMagic->nValue[0]);
	//g_DebugLog("[��ֵ]%s��ǰ����������%d��", pNpc->Name, pMagic->nValue[0]);	
}
//���˶�����ʱ�䣩��
void KNpcAttribModify::FastHitRecoverV(KNpc* pNpc, void* pData)
{
	KMagicAttrib* pMagic = (KMagicAttrib *)pData;

	//pMagic->nValue[0]=-pMagic->nValue[0];

	pNpc->m_CurrentHitRecover += pMagic->nValue[0];
	//g_DebugLog("[��ֵ]%s���˶������ʣ�ʱ�䣩����%d", pNpc->Name, pMagic->nValue[0]);
}

//ѣ�μ��ʼ���
void KNpcAttribModify::stuntrank_p(KNpc* pNpc, void* pData)
{
	KMagicAttrib* pMagic = (KMagicAttrib *)pData;
	pNpc->m_CurrentStunRank_p += pMagic->nValue[0];

    if (pNpc->m_CurrentStunRank_p<-100)
        pNpc->m_CurrentStunRank_p=-100;
	//g_DebugLog("[��ֵ]%sѣ�μ������Ӽ���%d", pNpc->Name, pMagic->nValue[0]);
}

//���˶������ʣ�ʱ�䣩 (�ٷֱ�)
void KNpcAttribModify::fasthitrecover_p(KNpc* pNpc, void* pData)
{
	KMagicAttrib* pMagic = (KMagicAttrib *)pData;
	if (pMagic->nValue[0]==0)
		return;

	if  (pMagic->nValue[0]>0)
	{
		if (pNpc->m_HitRecover*pMagic->nValue[0]>=50 && pNpc->m_HitRecover*pMagic->nValue[0]<=100)
		{
		   pNpc->m_CurrentHitRecover +=1;
		}
		else
		{
			//pNpc->m_CurrentHitRecover +=pNpc->TakeTrader(pNpc->m_HitRecover * pMagic->nValue[0],100);
			pNpc->m_CurrentHitRecover +=pNpc->m_HitRecover * pMagic->nValue[0]/100;

			//if (TakeRemainder(pNpc->m_HitRecover * pMagic->nValue[0],100)>=50)
			if ((pNpc->m_HitRecover * pMagic->nValue[0]%100)>=50)
				pNpc->m_CurrentHitRecover +=1;
		}

	}
	else
	{
	   pMagic->nValue[0] =-pMagic->nValue[0];

	   if (pNpc->m_HitRecover*pMagic->nValue[0]>=50 && pNpc->m_HitRecover*pMagic->nValue[0]<=100)
	   {
		   pNpc->m_CurrentHitRecover -=1;
	   }
	   else
	   {
		   pNpc->m_CurrentHitRecover -=(pNpc->m_HitRecover * pMagic->nValue[0])/100;//pNpc->TakeTrader(pNpc->m_HitRecover * pMagic->nValue[0],100);
		   
		   //if (TakeRemainder(pNpc->m_HitRecover * pMagic->nValue[0],100)>=50)
		   if ((pNpc->m_HitRecover * pMagic->nValue[0]%100)>=50)
			   pNpc->m_CurrentHitRecover -=1;
	   }
	}


	//pNpc->m_CurrentHitRecover += (pNpc->m_HitRecover*pMagic->nValue[0])/100;
	//g_DebugLog("[��ֵ]%s���˶������ʣ�ʱ�䣩����%d", pNpc->Name, pMagic->nValue[0]);
}


void KNpcAttribModify::FastWalkRunP(KNpc* pNpc, void* pData)
{
	KMagicAttrib* pMagic = (KMagicAttrib *)pData;
	// �ĵ�Ҫ���޸�Ϊֱ�Ӽӵ���
	if (pMagic->nValue[0]==0)
		return;

	pNpc->m_CurrentTempSpeed+=pMagic->nValue[0];

	if (pMagic->nValue[0]>0)
	{//+�ٶ�
		if  (pNpc->m_WalkSpeed * pMagic->nValue[0]>=50 && pNpc->m_WalkSpeed * pMagic->nValue[0]<=100)
		{
			pNpc->m_CurrentWalkSpeed +=1;
		}
		else
		{
			pNpc->m_CurrentWalkSpeed +=(pNpc->m_WalkSpeed * pMagic->nValue[0])/100;//pNpc->TakeTrader(pNpc->m_WalkSpeed * pMagic->nValue[0],100);

			//if (TakeRemainder(pNpc->m_WalkSpeed * pMagic->nValue[0],100)>=50)
			if ((pNpc->m_WalkSpeed * pMagic->nValue[0]%100)>=50)
				pNpc->m_CurrentWalkSpeed +=1;

		}

		if  (pNpc->m_RunSpeed * pMagic->nValue[0]>=50 && pNpc->m_RunSpeed * pMagic->nValue[0]<=100)
		{
			pNpc->m_CurrentRunSpeed +=1;
		}
		else
		{
			pNpc->m_CurrentRunSpeed +=(pNpc->m_RunSpeed * pMagic->nValue[0])/100;//pNpc->TakeTrader(pNpc->m_RunSpeed * pMagic->nValue[0],100);

			//if (TakeRemainder(pNpc->m_RunSpeed * pMagic->nValue[0],100)>=50)
			if ((pNpc->m_RunSpeed * pMagic->nValue[0]%100)>=50)
				pNpc->m_CurrentRunSpeed +=1;	  //��1

		}

	}
	else
	{//�����ٶ� <=0
		int nTempSeed=-pMagic->nValue[0];
		//pMagic->nValue[0] =-pMagic->nValue[0];
		if  (pNpc->m_WalkSpeed * nTempSeed>=50 && pNpc->m_WalkSpeed * nTempSeed<=100)
		{
			pNpc->m_CurrentWalkSpeed -=1;
		}
		else
		{
			pNpc->m_CurrentWalkSpeed -=(pNpc->m_WalkSpeed * nTempSeed)/100;//pNpc->TakeTrader(pNpc->m_WalkSpeed * pMagic->nValue[0],100);

			//if (TakeRemainder(pNpc->m_WalkSpeed * pMagic->nValue[0],100)>=50)
			if ((pNpc->m_WalkSpeed * nTempSeed%100)>=50)
				pNpc->m_CurrentWalkSpeed -=1;

		}

		if  (pNpc->m_RunSpeed * nTempSeed>=50 && pNpc->m_RunSpeed * nTempSeed<=100)
		{
			pNpc->m_CurrentRunSpeed -=1;
		}
		else
		{
			pNpc->m_CurrentRunSpeed -=(pNpc->m_RunSpeed * nTempSeed)/100;//pNpc->TakeTrader(pNpc->m_RunSpeed * pMagic->nValue[0],100);

			//if (TakeRemainder(pNpc->m_RunSpeed * pMagic->nValue[0],100)>=50)
			if ((pNpc->m_RunSpeed * nTempSeed%100)>=50)
				pNpc->m_CurrentRunSpeed -=1;	  //��1	
		}
	}

	//pNpc->m_CurrentWalkSpeed += (pNpc->m_WalkSpeed * pMagic->nValue[0]) / 100;
	//pNpc->m_CurrentRunSpeed  += (pNpc->m_RunSpeed * pMagic->nValue[0]) / 100;

	if (pNpc->m_CurrentRunSpeed<10)
		pNpc->m_CurrentRunSpeed=10;

	if (pNpc->m_CurrentWalkSpeed<=5)
		pNpc->m_CurrentWalkSpeed=5;
}

void KNpcAttribModify::FireArmorV(KNpc* pNpc, void* pData)
{
	KMagicAttrib* pMagic = (KMagicAttrib *)pData;
	if (pMagic->nValue[0] > 0)
	{
		pNpc->m_FireArmor.nValue[0] = pMagic->nValue[0];
		pNpc->m_FireArmor.nTime = pMagic->nValue[1];
		//g_DebugLog("[��ֵ]%s�𻤶ܱ�Ϊ%d�����%d֡", pNpc->Name, pMagic->nValue[0], pMagic->nValue[1]);
	}
	else
	{
		pNpc->m_FireArmor.nValue[0] = 0;
		pNpc->m_FireArmor.nTime = 0;
		//g_DebugLog("[��ֵ]%s�𻤶���ʧ", pNpc->Name);
	}
}

void KNpcAttribModify::FireSkillV(KNpc* pNpc, void* pData)
{
	if(!pNpc->IsPlayer())
		return;

	KMagicAttrib* pMagic = (KMagicAttrib *)pData;
	
    if (pNpc->m_Series == 3) //�Ի�ϵ���м��ܼ� �ȼ�
	{
		for (int i = 1;i < MAX_NPCSKILL;++i)
		{
			int mReg=0;
			
		        mReg=pNpc->m_SkillList.CheckNoSkill("NoCanXiSui",pNpc->m_SkillList.m_Skills[i].SkillId);
			if (mReg)
				continue;

			if(pNpc->m_SkillList.m_Skills[i].SkillId && pNpc->m_SkillList.m_Skills[i].SkillId!=1&&pNpc->m_SkillList.m_Skills[i].SkillId!=2&&pNpc->m_SkillList.m_Skills[i].SkillId!=53)
			{
				if (pNpc->m_SkillList.GetCurrentLevelByIdx(i) > 0)
				{
					pNpc->m_SkillList.QeuipAddPoint(i,pMagic->nValue[0]);
				}
			}
		}
	}
	//g_DebugLog("[��ֵ]%s��ϵ���м��ܼ�%d��", pNpc->Name, pMagic->nValue[0]);
}


void KNpcAttribModify::FreezeTimeReduceP(KNpc* pNpc, void* pData)
{
//	KMagicAttrib* pMagic = (KMagicAttrib *)pData;
//	pNpc->m_FreezeState.nTime *= (100 - pMagic->nValue[0]);
//	pNpc->m_FreezeState.nTime /= 100;
//	if (pNpc->m_FreezeState.nTime <= 0)
//	{
//		pNpc->m_FreezeState.nTime = 0;
//	}
//	//g_DebugLog("[��ֵ]%s����ʱ����ٰٷ�֮%d", pNpc->Name, pMagic->nValue[0]);
//	װ��TODO	
	KMagicAttrib* pMagic = (KMagicAttrib *)pData;
	pNpc->m_CurrentFreezeTimeReducePercent += pMagic->nValue[0];
	//g_DebugLog("[��ֵ]%s����ʱ��ٷ�֮%d", pNpc->Name, pMagic->nValue[0]);
}
//û�����ӵ��˺����� �����Ǳ���ô�
void KNpcAttribModify::KnockBackP(KNpc* pNpc, void* pData)
{
	KMagicAttrib* pMagic = (KMagicAttrib *)pData;
	pNpc->m_CurrentKnockBack += pMagic->nValue[0];
	//g_DebugLog("[��ֵ]%s���˰ٷֱ�����%d", pNpc->Name, pMagic->nValue[0]);	
}

void KNpcAttribModify::LifeMaxP(KNpc* pNpc, void* pData)
{
	KMagicAttrib* pMagic = (KMagicAttrib *)pData;

   /* if (pMagic->nValue[0]<=-100)
	    pMagic->nValue[0]=-GetRandomNumber(80,90);*/

	pNpc->m_CurrentLifeMax +=pNpc->m_LifeMax*pMagic->nValue[0]/100;	//ԭʼ����

	if (pNpc->m_CurrentLifeMax<=0)
	      pNpc->m_CurrentLifeMax=100;

	//g_DebugLog("[��ֵ]%s�������������˰ٷ�֮%d(%d��)", pNpc->Name, pMagic->nValue[0], pNpc->m_LifeMax * pMagic->nValue[0] / 100);	
}

void KNpcAttribModify::LifeMaxV(KNpc* pNpc, void* pData)
{
	KMagicAttrib* pMagic = (KMagicAttrib *)pData;
	pNpc->m_CurrentLifeMax += pMagic->nValue[0];

	if (pNpc->m_CurrentLifeMax<=0)
		pNpc->m_CurrentLifeMax=100;

	//g_DebugLog("[��ֵ]%s��������������%d��", pNpc->Name, pMagic->nValue[0]);
}

void KNpcAttribModify::LifeReplenishV(KNpc* pNpc, void* pData)
{
	KMagicAttrib* pMagic = (KMagicAttrib *)pData;

	pNpc->m_CurrentLifeReplenish += pMagic->nValue[0];

	//if (pNpc->m_CurrentLifeReplenish<0)
	//	pNpc->m_CurrentLifeReplenish=0;
	//g_DebugLog("[��ֵ]%s�����ظ��ٶ�������%d��", pNpc->Name, pMagic->nValue[0]);
}

void KNpcAttribModify::LifeV(KNpc* pNpc, void* pData)
{
	KMagicAttrib* pMagic = (KMagicAttrib *)pData;
	pNpc->m_CurrentLife += pMagic->nValue[0];
	//if (pNpc->m_CurrentLife<0)
	//	 pNpc->m_CurrentLife=100;
	//g_DebugLog("[��ֵ]%s����������%d��", pNpc->Name, pMagic->nValue[0]);
}

void KNpcAttribModify::LightingArmorV(KNpc* pNpc, void* pData)
{
	KMagicAttrib* pMagic = (KMagicAttrib *)pData;
	if (pMagic->nValue[0] > 0)
	{
		pNpc->m_LightArmor.nValue[0] = pMagic->nValue[0];
		pNpc->m_LightArmor.nTime = pMagic->nValue[1];
		//g_DebugLog("[��ֵ]%s�绤�ܱ�Ϊ%d�����%d֡", pNpc->Name, pMagic->nValue[0], pMagic->nValue[1]);	
	}
	else
	{
		pNpc->m_LightArmor.nValue[0] = 0;
		pNpc->m_LightArmor.nTime = 0;
		//g_DebugLog("[��ֵ]%s�绤����ʧ", pNpc->Name);	
	}
}

void KNpcAttribModify::LightingresP(KNpc* pNpc, void* pData)
{
	KMagicAttrib* pMagic = (KMagicAttrib *)pData;
	int nRes=0;
	int nRer=0;
	int nIsadd=0;
	
	pNpc->m_TempLightResist += pMagic->nValue[0];
	
	if (pNpc->m_TempLightResist>=0)
	{
		nIsadd=1;
		nRes=(pNpc->m_TempLightResist*BASE_FANGYU_ALL_MAX)/100;//TakeTrader(pNpc->m_TempLightResist*BASE_FANGYU_ALL_MAX,100); //ȡ��
		nRer=(pNpc->m_TempLightResist*BASE_FANGYU_ALL_MAX)%100;//TakeRemainder(pNpc->m_TempLightResist*BASE_FANGYU_ALL_MAX,100);
		
		if (pNpc->m_TempLightResist==1)
			nRes=1;
		
		if (nRer>=50)
			nRes +=1; //4��5��
		
	}
	else
	{
		nRes=(-pNpc->m_TempLightResist*BASE_FANGYU_ALL_MAX)/100;//TakeTrader(-pNpc->m_TempLightResist*BASE_FANGYU_ALL_MAX,100); //ȡ��
		nRer=(-pNpc->m_TempLightResist*BASE_FANGYU_ALL_MAX)%100;//TakeTrader(-pNpc->m_TempLightResist*BASE_FANGYU_ALL_MAX,100);
		
		if  (-pNpc->m_TempLightResist==1)
			nRes=1;
		
		if (nRer>=50)
			nRes +=1; //4��5��
	}
	
	
	if (nIsadd==1)	//����
        pNpc->m_CurrentLightResist=pNpc->m_LightResist+nRes;
	else           //����
		pNpc->m_CurrentLightResist=pNpc->m_LightResist-nRes;

	if (pNpc->m_CurrentLightResist>BASE_FANGYU_ALL_MAX)
	    pNpc->m_CurrentLightResistMax = pNpc->m_CurrentLightResist;
	
	if (pNpc->m_CurrentLightResistMax > MAX_RESIST)
        pNpc->m_CurrentLightResistMax = MAX_RESIST;

	if (pNpc->m_CurrentLightResistMax<=BASE_FANGYU_ALL_MAX)
		pNpc->m_CurrentLightResistMax=BASE_FANGYU_ALL_MAX;


	if (pNpc->m_CurrentLightResist >pNpc->m_CurrentLightResistMax)
		pNpc->m_CurrentLightResist =pNpc->m_CurrentLightResistMax;

    

	//g_DebugLog("[��ֵ]%s�翹��������%d��", pNpc->Name, pMagic->nValue[0]);	
}
//����ֵ
void KNpcAttribModify::LuckyV(KNpc* pNpc, void* pData)
{
	KMagicAttrib* pMagic = (KMagicAttrib *)pData;

	if (pNpc->IsPlayer())
	{
		Player[pNpc->m_nPlayerIdx].m_nTempLucky_p += pMagic->nValue[0];

		if (Player[pNpc->m_nPlayerIdx].m_nTempLucky_p<0)
			Player[pNpc->m_nPlayerIdx].m_nTempLucky_p=0;

		Player[pNpc->m_nPlayerIdx].m_nCurLucky = Player[pNpc->m_nPlayerIdx].m_nLucky * (Player[pNpc->m_nPlayerIdx].m_nTempLucky_p+100)/100;    //���� ��
		//g_DebugLog("[��ֵ]%sMFֵ������%d", pNpc->Name, pMagic->nValue[0]);
	}
	else
	{
		pNpc->m_nCurNpcLucky +=	pMagic->nValue[0]; //���������ֵ
	}
}

void KNpcAttribModify::ManaMaxP(KNpc* pNpc, void* pData)
{
	KMagicAttrib* pMagic = (KMagicAttrib *)pData;  //NPC��

	pNpc->m_CurrentManaMax += pNpc->m_ManaMax * pMagic->nValue[0]/100;

	if (pNpc->m_CurrentManaMax<=0)
		pNpc->m_CurrentManaMax=100;
	//g_DebugLog("[��ֵ]%s�������������˰ٷ�֮%d(%d��)", pNpc->Name, pMagic->nValue[0], pNpc->m_ManaMax * pMagic->nValue[0] / 100);	
}

void KNpcAttribModify::ManaMaxV(KNpc* pNpc, void* pData)
{
	KMagicAttrib* pMagic = (KMagicAttrib *)pData;
	pNpc->m_CurrentManaMax += pMagic->nValue[0];

	if (pNpc->m_CurrentManaMax<=0)
		pNpc->m_CurrentManaMax=100;
	//g_DebugLog("[��ֵ]%s��������������%d��", pNpc->Name, pMagic->nValue[0]);
}

void KNpcAttribModify::ManaReplenishV(KNpc* pNpc, void* pData)
{
	KMagicAttrib* pMagic = (KMagicAttrib *)pData;
	pNpc->m_CurrentManaReplenish += pMagic->nValue[0];

	//if (pNpc->m_CurrentManaReplenish<=0)
	//	pNpc->m_CurrentManaReplenish=0;

	//g_DebugLog("[��ֵ]%s�����ظ��ٶ�������%d��", pNpc->Name, pMagic->nValue[0]);
}

void KNpcAttribModify::ManaReplenishp(KNpc* pNpc, void* pData)
{
	KMagicAttrib* pMagic = (KMagicAttrib *)pData;
	pNpc->m_CurrentManaReplenish_p += pMagic->nValue[0];

	if (pNpc->m_CurrentManaReplenish_p<=-100)
		pNpc->m_CurrentManaReplenish_p=-100;

	//g_DebugLog("[��ֵ]%s�����ظ��ٶ�������%d��", pNpc->Name, pMagic->nValue[0]);
}

void KNpcAttribModify::ManaV(KNpc* pNpc, void* pData)
{
	KMagicAttrib* pMagic = (KMagicAttrib *)pData;
	pNpc->m_CurrentMana += pMagic->nValue[0];
	//if (pNpc->m_CurrentMana<0)
	//	pNpc->m_CurrentMana=100;
	//g_DebugLog("[��ֵ]%s����������%d��", pNpc->Name, pMagic->nValue[0]);
}
//���������˺�
void KNpcAttribModify::ManaShieldP(KNpc* pNpc, void* pData)
{
	KMagicAttrib* pMagic = (KMagicAttrib *)pData;
	if (pMagic->nValue[0] > 0)
	{
		pNpc->m_ManaShield.nValue[0] = pMagic->nValue[0];
		pNpc->m_ManaShield.nTime = pMagic->nValue[1];
		//g_DebugLog("[��ֵ]%s�����˺��ٷֱ�Ϊ%d������%d֡", pNpc->Name, pMagic->nValue[0], pMagic->nValue[1]);	
	}
	else
	{
		pNpc->m_ManaShield.nValue[0] = 0;
		pNpc->m_ManaShield.nTime = 0;
		//g_DebugLog("[��ֵ]�����˺�Ч����ʧ");
	}
}

void KNpcAttribModify::MeleeDamageReturnP(KNpc* pNpc, void* pData)
{
	KMagicAttrib* pMagic = (KMagicAttrib *)pData;
	pNpc->m_CurrentMeleeDmgRetPercent += pMagic->nValue[0];


   if (pNpc->m_CurrentMeleeDmgRetPercent <=0)
       pNpc->m_CurrentMeleeDmgRetPercent=0;

	//g_DebugLog("[��ֵ]%s���������˺������ٷֱ����Ӽ���%d", pNpc->Name, pMagic->nValue[0]);
}

void KNpcAttribModify::MeleeDamageReturnV(KNpc* pNpc, void* pData)
{
	KMagicAttrib* pMagic = (KMagicAttrib *)pData;
	pNpc->m_CurrentMeleeDmgRet += pMagic->nValue[0];
	//g_DebugLog("[��ֵ]%s���������˺���������%d", pNpc->Name, pMagic->nValue[0]);
}

void KNpcAttribModify::MetalSkillV(KNpc* pNpc, void* pData)
{

	   if(!pNpc->IsPlayer())
		   return;


		KMagicAttrib* pMagic = (KMagicAttrib *)pData;
		
		if (pNpc->m_Series == 0) //�Խ�ϵ���м��ܼ� �ȼ�
		{
			for (int i = 1;i < MAX_NPCSKILL;++i)
			{
				int mReg=0;
				
		        mReg=pNpc->m_SkillList.CheckNoSkill("NoCanXiSui",pNpc->m_SkillList.m_Skills[i].SkillId);

				if (mReg)
					continue;

				if(pNpc->m_SkillList.m_Skills[i].SkillId && pNpc->m_SkillList.m_Skills[i].SkillId!=1&&pNpc->m_SkillList.m_Skills[i].SkillId!=2&&pNpc->m_SkillList.m_Skills[i].SkillId!=53)
				{
					if (pNpc->m_SkillList.GetCurrentLevelByIdx(i) > 0)
					{
						pNpc->m_SkillList.QeuipAddPoint(i,pMagic->nValue[0]);
					}
				}
			}
		}
		//g_DebugLog("[��ֵ]%s��ϵ���м��ܼ�%d��", pNpc->Name, pMagic->nValue[0]);
	}

void KNpcAttribModify::PhysicsArmorV(KNpc* pNpc, void* pData)
{
	KMagicAttrib* pMagic = (KMagicAttrib *)pData;
	if (pMagic->nValue[0] > 0)
	{
		pNpc->m_PhysicsArmor.nValue[0] = pMagic->nValue[0];
		pNpc->m_PhysicsArmor.nTime = pMagic->nValue[1];
		//g_DebugLog("[��ֵ]%s�����ܱ�Ϊ%d�����%d֡", pNpc->Name, pMagic->nValue[0], pMagic->nValue[1]);	
	}
	else
	{
		pNpc->m_PhysicsArmor.nValue[0] = 0;
		pNpc->m_PhysicsArmor.nTime = 0;
		//g_DebugLog("[��ֵ]%s��������ʧ", pNpc->Name);	
	}
}

void KNpcAttribModify::PhysicsresP(KNpc* pNpc, void* pData)
{
	KMagicAttrib* pMagic = (KMagicAttrib *)pData;

	int nRes=0;
	int nRer=0;
	int nIsadd=0;

	pNpc->m_TempPhysicsResist += pMagic->nValue[0];

	if (pNpc->m_TempPhysicsResist>=0)
	{
	  nIsadd=1;
	  nRes=(pNpc->m_TempPhysicsResist*BASE_FANGYU_ALL_MAX)/100;//TakeTrader(pNpc->m_TempPhysicsResist*BASE_FANGYU_ALL_MAX,100); //ȡ��
	  nRer=(pNpc->m_TempPhysicsResist*BASE_FANGYU_ALL_MAX)%100;//TakeRemainder(pNpc->m_TempPhysicsResist*BASE_FANGYU_ALL_MAX,100);

	  if (pNpc->m_TempPhysicsResist==1)
		  nRes=1;

	  if (nRer>=50)
		  nRes +=1; //4��5��
 
	}
	else
	{
	  nRes=(-pNpc->m_TempPhysicsResist*BASE_FANGYU_ALL_MAX)/100;//TakeTrader(-pNpc->m_TempPhysicsResist*BASE_FANGYU_ALL_MAX,100); //ȡ��
	  nRer=(-pNpc->m_TempPhysicsResist*BASE_FANGYU_ALL_MAX)%100;//TakeTrader(-pNpc->m_TempPhysicsResist*BASE_FANGYU_ALL_MAX,100);

	  if  (-pNpc->m_TempPhysicsResist==1)
		  nRes=1;

	  if (nRer>=50)
		  nRes +=1; //4��5��
	}

	 if (nIsadd==1)	//����
        pNpc->m_CurrentPhysicsResist= pNpc->m_PhysicsResist+nRes;
	 else           //����
		pNpc->m_CurrentPhysicsResist= pNpc->m_PhysicsResist-nRes;

	if (pNpc->m_CurrentPhysicsResist>BASE_FANGYU_ALL_MAX)
	    pNpc->m_CurrentPhysicsResistMax = pNpc->m_CurrentPhysicsResist;
	
	if (pNpc->m_CurrentPhysicsResistMax > MAX_RESIST)
        pNpc->m_CurrentPhysicsResistMax = MAX_RESIST;

	if (pNpc->m_CurrentPhysicsResistMax<=BASE_FANGYU_ALL_MAX)
		pNpc->m_CurrentPhysicsResistMax=BASE_FANGYU_ALL_MAX;

	if (pNpc->m_CurrentPhysicsResist >pNpc->m_CurrentPhysicsResistMax)
		pNpc->m_CurrentPhysicsResist =pNpc->m_CurrentPhysicsResistMax;
	//g_DebugLog("[��ֵ]%s������������%d��", pNpc->Name, pMagic->nValue[0]);	
}

void KNpcAttribModify::Damage2ManaP(KNpc* pNpc, void* pData)
{
	KMagicAttrib* pMagic = (KMagicAttrib *)pData;
	pNpc->m_CurrentDamage2Mana += pMagic->nValue[0];
	//g_DebugLog("[��ֵ]%s�˺�ת�����ٷֱ�������%d", pNpc->Name, pMagic->nValue[0]);	
}

void KNpcAttribModify::PoisonArmorV(KNpc* pNpc, void* pData)
{
	KMagicAttrib* pMagic = (KMagicAttrib *)pData;
	if (pMagic->nValue[0] > 0)
	{
		pNpc->m_PoisonArmor.nValue[0] = pMagic->nValue[0];
		pNpc->m_PoisonArmor.nTime = pMagic->nValue[1];
		//g_DebugLog("[��ֵ]%s�����ܱ�Ϊ%d�����%d֡", pNpc->Name, pMagic->nValue[0], pMagic->nValue[1]);
	}
	else
	{
		pNpc->m_PoisonArmor.nValue[0] = 0;
		pNpc->m_PoisonArmor.nTime = 0;
		//g_DebugLog("[��ֵ]%s��������ʧ", pNpc->Name);
	}
}
//�ⶾҩ
void KNpcAttribModify::PoisonDamageReduceV(KNpc* pNpc, void* pData)
{
	KMagicAttrib* pMagic = (KMagicAttrib *)pData;

	/*pNpc->m_PoisonState.nValue[0] -= pMagic->nValue[0];

	if (pNpc->m_PoisonState.nValue[0] <= 0)
	{
		pNpc->m_PoisonState.nTime = 0;
	}
   */
	pNpc->m_PoisonState.nValue[0] = pNpc->m_PoisonState.nValue[0] - pMagic->nValue[0];

	if (pNpc->m_PoisonState.nValue[0]<=0)
		pNpc->m_PoisonState.nValue[0] = 0;

}

void KNpcAttribModify::PoisonresP(KNpc* pNpc, void* pData)
{
	KMagicAttrib* pMagic = (KMagicAttrib *)pData;
	int nRes=0;
	int nRer=0;
	int nIsadd=0;
	
	pNpc->m_TempPoisonResist += pMagic->nValue[0];
	
	if (pNpc->m_TempPoisonResist>=0)
	{
		nIsadd=1;
		nRes=(pNpc->m_TempPoisonResist*BASE_FANGYU_ALL_MAX)/100;//TakeTrader(pNpc->m_TempPoisonResist*BASE_FANGYU_ALL_MAX,100); //ȡ��
		nRer=(pNpc->m_TempPoisonResist*BASE_FANGYU_ALL_MAX)%100;//TakeRemainder(pNpc->m_TempPoisonResist*BASE_FANGYU_ALL_MAX,100);
		
		if (pNpc->m_TempPoisonResist==1)
			nRes=1;
		
		if (nRer>=50)
			nRes +=1; //4��5��
		
	}
	else
	{
		nRes=(-pNpc->m_TempPoisonResist*BASE_FANGYU_ALL_MAX)/100;//TakeTrader(-pNpc->m_TempPoisonResist*BASE_FANGYU_ALL_MAX,100); //ȡ��
		nRer=(-pNpc->m_TempPoisonResist*BASE_FANGYU_ALL_MAX)%100;//TakeTrader(-pNpc->m_TempPoisonResist*BASE_FANGYU_ALL_MAX,100);
		
		if  (-pNpc->m_TempPoisonResist==1)
			nRes=1;
		
		if (nRer>=50)
			nRes +=1; //4��5��
	}
	
	if (nIsadd==1)	//����
        pNpc->m_CurrentPoisonResist=pNpc->m_PoisonResist+nRes;
	else           //����
		pNpc->m_CurrentPoisonResist=pNpc->m_PoisonResist-nRes;


    if (pNpc->m_CurrentPoisonResist>BASE_FANGYU_ALL_MAX)
	    pNpc->m_CurrentPoisonResistMax = pNpc->m_CurrentPoisonResist;
	
	if (pNpc->m_CurrentPoisonResistMax > MAX_RESIST)
        pNpc->m_CurrentPoisonResistMax = MAX_RESIST;

	if (pNpc->m_CurrentPoisonResistMax<=BASE_FANGYU_ALL_MAX)
		pNpc->m_CurrentPoisonResistMax=BASE_FANGYU_ALL_MAX;


	if (pNpc->m_CurrentPoisonResist >pNpc->m_CurrentPoisonResistMax)
		pNpc->m_CurrentPoisonResist =pNpc->m_CurrentPoisonResistMax;

	//g_DebugLog("[��ֵ]%s������������%d��", pNpc->Name, pMagic->nValue[0]);
}

void KNpcAttribModify::PoisonTimeReduceP(KNpc* pNpc, void* pData)
{
// װ�����Ҫ�޸�TODO
//	KMagicAttrib* pMagic = (KMagicAttrib *)pData;
//	pNpc->m_PoisonState.nTime *= (100 - pMagic->nValue[0]);
//	pNpc->m_PoisonState.nTime /= 100;
//	if (pNpc->m_PoisonState.nTime <= 0)
//	{
//		pNpc->m_PoisonState.nTime = 0;
//	}
//	//g_DebugLog("[��ֵ]%s�ж�ʱ����ٰٷ�֮%d", pNpc->Name, pMagic->nValue[0]);	
	KMagicAttrib* pMagic = (KMagicAttrib *)pData;

	/* if (pMagic->nValue[0]>500)
		 pMagic->nValue[0]=0;

	 if  (pMagic->nValue[0]<-500)
		  pMagic->nValue[0]=-GetRandomNumber(5,30);	*/

	pNpc->m_CurrentPoisonTimeReducePercent += pMagic->nValue[0];
	if (pNpc->m_CurrentPoisonTimeReducePercent<-100)
		pNpc->m_CurrentPoisonTimeReducePercent =-100;
	//g_DebugLog("[��ֵ]%s�ж�ʱ�����Ӽ��ٰٷ�֮%d", pNpc->Name, pMagic->nValue[0]);	
}

//��Է��ж�ʱ������ �� ����
void    KNpcAttribModify::anti_poisontimereduce_p( KNpc* pNpc, void* pData)
{
	KMagicAttrib* pMagic = (KMagicAttrib *)pData;

	pNpc->m_EnemyPoisonTimeReducePercent += pMagic->nValue[0];

}

void KNpcAttribModify::RangeDamageReturnV(KNpc* pNpc, void* pData)
{
	KMagicAttrib* pMagic = (KMagicAttrib *)pData;
	pNpc->m_CurrentRangeDmgRet += pMagic->nValue[0];
	//g_DebugLog("[��ֵ]%sԶ���˺���������%d", pNpc->Name, pMagic->nValue[0]);
}

void KNpcAttribModify::RangeDamageReturnP(KNpc* pNpc, void* pData)
{
	KMagicAttrib* pMagic = (KMagicAttrib *)pData;
	pNpc->m_CurrentRangeDmgRetPercent += pMagic->nValue[0];

	if (pNpc->m_CurrentRangeDmgRetPercent<=0)
	    pNpc->m_CurrentRangeDmgRetPercent=0;
	//g_DebugLog("[��ֵ]%sԶ���˺������ٷֱ�����%d", pNpc->Name, pMagic->nValue[0]);
}
//�����ӵ������ٶ�
void KNpcAttribModify::SlowMissleB(KNpc* pNpc, void* pData)
{
	KMagicAttrib* pMagic = (KMagicAttrib *)pData;

	if (pMagic->nValue[0] > 0)
		pNpc->m_CurrentSlowMissle = 1;
	else
		pNpc->m_CurrentSlowMissle = 0;
	//g_DebugLog("[��ֵ]%s�ӵ�����״̬��Ϊ%d", pNpc->Name, pMagic->nValue[0]);
}

void KNpcAttribModify::StaminaMaxP(KNpc* pNpc, void* pData)
{
	KMagicAttrib* pMagic = (KMagicAttrib *)pData;
	pNpc->m_CurrentStaminaMax += pNpc->m_StaminaMax * pMagic->nValue[0] / 100;
	//g_DebugLog("[��ֵ]%s�������������˰ٷ�֮%d(%d��)", pNpc->Name, pMagic->nValue[0], pNpc->m_StaminaMax * pMagic->nValue[0] / 100);	
}

void KNpcAttribModify::StaminaMaxV(KNpc* pNpc, void* pData)
{
	KMagicAttrib* pMagic = (KMagicAttrib *)pData;
	pNpc->m_CurrentStaminaMax += pMagic->nValue[0];
	//g_DebugLog("[��ֵ]%s��������������%d��", pNpc->Name, pMagic->nValue[0]);
}

void KNpcAttribModify::StaminaReplenishV(KNpc* pNpc, void* pData)
{
	KMagicAttrib* pMagic = (KMagicAttrib *)pData;
	pNpc->m_CurrentStaminaGain += pMagic->nValue[0];
	//g_DebugLog("[��ֵ]%s�����ظ��ٶ�������%d��", pNpc->Name, pMagic->nValue[0]);
}

void KNpcAttribModify::StaminaV(KNpc* pNpc, void* pData)
{
	KMagicAttrib* pMagic = (KMagicAttrib *)pData;
	pNpc->m_CurrentStamina += pMagic->nValue[0];
	//g_DebugLog("[��ֵ]%s����������%d��", pNpc->Name, pMagic->nValue[0]);
}


void KNpcAttribModify::StealLifeP(KNpc* pNpc, void* pData)
{
	KMagicAttrib* pMagic = (KMagicAttrib *)pData;
	pNpc->m_CurrentLifeStolen += pMagic->nValue[0];
	//g_DebugLog("[��ֵ]%s͵�����ٷֱ�Ϊ%d(��ԭ��������%d��)", pNpc->Name, pNpc->m_CurrentLifeStolen, pMagic->nValue[0]);	
}

void KNpcAttribModify::StealManaP(KNpc* pNpc, void* pData)
{
	KMagicAttrib* pMagic = (KMagicAttrib *)pData;
	pNpc->m_CurrentManaStolen += pMagic->nValue[0];
	//g_DebugLog("[��ֵ]%s͵�����ٷֱ�Ϊ%d(��ԭ��������%d��)", pNpc->Name, pNpc->m_CurrentManaStolen, pMagic->nValue[0]);	
}

void KNpcAttribModify::StealStaminaP(KNpc* pNpc, void* pData)
{
	KMagicAttrib* pMagic = (KMagicAttrib *)pData;
	pNpc->m_CurrentStaminaStolen += pMagic->nValue[0];
	//g_DebugLog("[��ֵ]%s͵�����ٷֱ�Ϊ%d(��ԭ��������%d��)", pNpc->Name, pNpc->m_CurrentStaminaStolen, pMagic->nValue[0]);	
}

void KNpcAttribModify::StrengthV(KNpc* pNpc, void* pData)
{
	KMagicAttrib* pMagic = (KMagicAttrib *)pData;
	if (pNpc->m_Kind != kind_player)
		return;
	if (pNpc->m_nPlayerIdx <= 0)
		return;
	Player[pNpc->m_nPlayerIdx].ChangeCurStrength(pMagic->nValue[0]);
	//g_DebugLog("[��ֵ]%s��ǰ����������%d��", pNpc->Name, pMagic->nValue[0]);
}

void KNpcAttribModify::StunTimeReduceP(KNpc* pNpc, void* pData)
{
// װ������Ҫ�޸�TODO
//	KMagicAttrib* pMagic = (KMagicAttrib *)pData;
//	pNpc->m_StunState.nTime -= pMagic->nValue[0];
//	if (pNpc->m_StunState.nTime <= 0)
//	{
//		pNpc->m_StunState.nTime = 0;
//	}
//	//g_DebugLog("[��ֵ]%sѣ��ʱ�����%d", pNpc->Name, pMagic->nValue[0]);
	
	KMagicAttrib* pMagic = (KMagicAttrib *)pData;
	pNpc->m_CurrentStunTimeReducePercent += pMagic->nValue[0];
    
	if (pNpc->m_CurrentStunTimeReducePercent<-200)
		pNpc->m_CurrentStunTimeReducePercent=-200;

	//g_DebugLog("[��ֵ]%sѣ��ʱ�����%d", pNpc->Name, pMagic->nValue[0]);
}


void KNpcAttribModify::addskilldamage10( KNpc* pNpc, void* pData )
{
	KMagicAttrib* pMagic = (KMagicAttrib *)pData;

	if (pMagic->nValue[0] != 0)
	{
		int Same;
		if (pMagic->nValue[0] > 0)
			Same = pMagic->nValue[0];
		else
			Same = -pMagic->nValue[0];
		int nListIndex = pNpc->m_SkillList.FindSame(Same);
		if (nListIndex)
		{
			if (pNpc->m_SkillList.GetCurrentLevelByIdx(nListIndex) > 0)
			{
				pNpc->m_SkillList.AddEnChance(nListIndex,pMagic->nValue[2]);
			}			
		}
	}
	else if (pMagic->nValue[0] == 0)
	{
		for (int i = 1;i < MAX_NPCSKILL;++i)
		{
			if(pNpc->m_SkillList.m_Skills[i].SkillId&&pNpc->m_SkillList.m_Skills[i].SkillId!=1&&pNpc->m_SkillList.m_Skills[i].SkillId!=2&&pNpc->m_SkillList.m_Skills[i].SkillId!=53)
			{
				if (pNpc->m_SkillList.GetCurrentLevelByIdx(i) > 0)
				{
					pNpc->m_SkillList.AddEnChance(i,pMagic->nValue[2]);					
				}
			}
		}
	}
}
void KNpcAttribModify::addskilldamage9( KNpc* pNpc, void* pData )
{
	KMagicAttrib* pMagic = (KMagicAttrib *)pData;

	if (pMagic->nValue[0] != 0)
	{
		int Same;
		if (pMagic->nValue[0] > 0)
			Same = pMagic->nValue[0];
		else
			Same = -pMagic->nValue[0];
		int nListIndex = pNpc->m_SkillList.FindSame(Same);
		if (nListIndex)
		{
			if (pNpc->m_SkillList.GetCurrentLevelByIdx(nListIndex) > 0)
			{
				pNpc->m_SkillList.AddEnChance(nListIndex,pMagic->nValue[2]);
			}			
		}
	}
	else if (pMagic->nValue[0] == 0)
	{
		for (int i = 1;i < MAX_NPCSKILL;++i)
		{
			if(pNpc->m_SkillList.m_Skills[i].SkillId&&pNpc->m_SkillList.m_Skills[i].SkillId!=1&&pNpc->m_SkillList.m_Skills[i].SkillId!=2&&pNpc->m_SkillList.m_Skills[i].SkillId!=53)
			{
				if (pNpc->m_SkillList.GetCurrentLevelByIdx(i) > 0)
				{
					pNpc->m_SkillList.AddEnChance(i,pMagic->nValue[2]);					
				}
			}
		}
	}
}

void KNpcAttribModify::addskilldamage8( KNpc* pNpc, void* pData )
{
	KMagicAttrib* pMagic = (KMagicAttrib *)pData;

	if (pMagic->nValue[0] != 0)
	{
		int Same;
		if (pMagic->nValue[0] > 0)
			Same = pMagic->nValue[0];
		else
			Same = -pMagic->nValue[0];
		int nListIndex = pNpc->m_SkillList.FindSame(Same);
		if (nListIndex)
		{
			if (pNpc->m_SkillList.GetCurrentLevelByIdx(nListIndex) > 0)
			{
				pNpc->m_SkillList.AddEnChance(nListIndex,pMagic->nValue[2]);
			}			
		}
	}
	else if (pMagic->nValue[0] == 0)
	{
		for (int i = 1;i < MAX_NPCSKILL;++i)
		{
			if(pNpc->m_SkillList.m_Skills[i].SkillId&&pNpc->m_SkillList.m_Skills[i].SkillId!=1&&pNpc->m_SkillList.m_Skills[i].SkillId!=2&&pNpc->m_SkillList.m_Skills[i].SkillId!=53)
			{
				if (pNpc->m_SkillList.GetCurrentLevelByIdx(i) > 0)
				{
					pNpc->m_SkillList.AddEnChance(i,pMagic->nValue[2]);					
				}
			}
		}
	}
}

//��ɵ��˵�ѣ��ʱ������ �� ����
void KNpcAttribModify::anti_stuntimereduce_p( KNpc* pNpc, void* pData)
{
	KMagicAttrib* pMagic = (KMagicAttrib *)pData;
	pNpc->m_EnemyStunTimeReducePercent += pMagic->nValue[0];
}

void KNpcAttribModify::VisionRadiusP(KNpc* pNpc, void* pData)
{
	KMagicAttrib* pMagic = (KMagicAttrib *)pData;
	pNpc->m_CurrentVisionRadius += pMagic->nValue[0];
	//g_DebugLog("[��ֵ]%s��Ұ��Χ����%d", pNpc->Name, pMagic->nValue[0]);	
}

void KNpcAttribModify::VitalityV(KNpc* pNpc, void* pData)
{
	KMagicAttrib* pMagic = (KMagicAttrib *)pData;
	if (pNpc->m_Kind != kind_player)
		return;
	if (pNpc->m_nPlayerIdx <= 0)
		return;
	Player[pNpc->m_nPlayerIdx].ChangeCurVitality(pMagic->nValue[0]);  //�ı䵱ǰ������ ���ֵ ������ֵ
	//g_DebugLog("[��ֵ]%s��ǰ�⹦������%d��", pNpc->Name, pMagic->nValue[0]);
}

void KNpcAttribModify::WaterSkillV(KNpc* pNpc, void* pData)
{

	if(!pNpc->IsPlayer())
		return;

	KMagicAttrib* pMagic = (KMagicAttrib *)pData;
	
    if (pNpc->m_Series == 2) //��ľϵ���м��ܼ� �ȼ�
	{
		for (int i = 1;i < MAX_NPCSKILL;++i)
		{
			int mReg=0;
			
		        mReg=pNpc->m_SkillList.CheckNoSkill("NoCanXiSui",pNpc->m_SkillList.m_Skills[i].SkillId);

			if (mReg)
			   continue;

			if(pNpc->m_SkillList.m_Skills[i].SkillId && pNpc->m_SkillList.m_Skills[i].SkillId!=1&&pNpc->m_SkillList.m_Skills[i].SkillId!=2&&pNpc->m_SkillList.m_Skills[i].SkillId!=53)
			{
				if (pNpc->m_SkillList.GetCurrentLevelByIdx(i) > 0)
				{
					pNpc->m_SkillList.QeuipAddPoint(i,pMagic->nValue[0]);
				}
			}
		}
	}
	//g_DebugLog("[��ֵ]%sˮϵ���м��ܼ�%d��", pNpc->Name, pMagic->nValue[0]);
}


void KNpcAttribModify::WoodSkillV(KNpc* pNpc, void* pData)
{

	if(!pNpc->IsPlayer())
		return;

	KMagicAttrib* pMagic = (KMagicAttrib *)pData;
	
    if (pNpc->m_Series == 1) //��ľϵ���м��ܼ� �ȼ�
	{
		for (int i = 1;i < MAX_NPCSKILL;++i)
		{

            int mReg=0;
			
		        mReg=pNpc->m_SkillList.CheckNoSkill("NoCanXiSui",pNpc->m_SkillList.m_Skills[i].SkillId);
			if (mReg)
				continue;

			if(pNpc->m_SkillList.m_Skills[i].SkillId && pNpc->m_SkillList.m_Skills[i].SkillId!=1&&pNpc->m_SkillList.m_Skills[i].SkillId!=2&&pNpc->m_SkillList.m_Skills[i].SkillId!=53)
			{
				if (pNpc->m_SkillList.GetCurrentLevelByIdx(i) > 0)
				{
					pNpc->m_SkillList.QeuipAddPoint(i,pMagic->nValue[0]);
				}
			}
		}
	}
	//g_DebugLog("[��ֵ]%sľϵ���м��ܼ�%d��", pNpc->Name, pMagic->nValue[0]);
}

void KNpcAttribModify::FireresP(KNpc* pNpc, void* pData)
{
	KMagicAttrib* pMagic = (KMagicAttrib *)pData;
	int nRes=0;
	int nRer=0;
	int nIsadd=0;
	
	pNpc->m_TempFireResist += pMagic->nValue[0];
	
	if (pNpc->m_TempFireResist>=0)
	{
		nIsadd=1;
		nRes=(pNpc->m_TempFireResist*BASE_FANGYU_ALL_MAX)/100;//TakeTrader(pNpc->m_TempFireResist*BASE_FANGYU_ALL_MAX,100); //ȡ��
		nRer=(pNpc->m_TempFireResist*BASE_FANGYU_ALL_MAX)%100;//TakeRemainder(pNpc->m_TempFireResist*BASE_FANGYU_ALL_MAX,100);
		
		if (pNpc->m_TempFireResist==1)
			nRes=1;
		
		if (nRer>=50)
			nRes +=1; //4��5��
		
	}
	else
	{
		nRes=(-pNpc->m_TempFireResist*BASE_FANGYU_ALL_MAX)/100;//TakeTrader(-pNpc->m_TempFireResist*BASE_FANGYU_ALL_MAX,100); //ȡ��
		nRer=(-pNpc->m_TempFireResist*BASE_FANGYU_ALL_MAX)%100;//TakeTrader(-pNpc->m_TempFireResist*BASE_FANGYU_ALL_MAX,100);
		
		if  (-pNpc->m_TempFireResist==1)
			nRes=1;
		
		if (nRer>=50)
			nRes +=1; //4��5��
	}
	
	
	if (nIsadd==1)	//����
        pNpc->m_CurrentFireResist=pNpc->m_FireResist+nRes;	 //pNpc->m_FireResist ԭʼ�Ļ��
	else           //����
		pNpc->m_CurrentFireResist=pNpc->m_FireResist-nRes;

	if (pNpc->m_CurrentFireResist>BASE_FANGYU_ALL_MAX)
	    pNpc->m_CurrentFireResistMax = pNpc->m_CurrentFireResist;
	
	if (pNpc->m_CurrentFireResistMax > MAX_RESIST)
        pNpc->m_CurrentFireResistMax = MAX_RESIST;

	if (pNpc->m_CurrentFireResistMax<=BASE_FANGYU_ALL_MAX)
		pNpc->m_CurrentFireResistMax=BASE_FANGYU_ALL_MAX;


	if (pNpc->m_CurrentFireResist>pNpc->m_CurrentFireResistMax)
		pNpc->m_CurrentFireResist=pNpc->m_CurrentFireResistMax;
	//g_DebugLog("[��ֵ]%s����������%d��", pNpc->Name, pMagic->nValue[0]);
}

void KNpcAttribModify::ArmorDefenseV(KNpc* pNpc, void* pData)
{
	KMagicAttrib* pMagic = (KMagicAttrib *)pData;
	pNpc->m_CurrentDefend += pMagic->nValue[0];

	if (pNpc->m_CurrentDefend<0)
	    pNpc->m_CurrentDefend=0;
	//g_DebugLog("[��ֵ]%s����������%d��", pNpc->Name, pMagic->nValue[0]);
}
//��ɵط��ٻ�ʱ�� 
void KNpcAttribModify::ColdEnhanceP(KNpc* pNpc, void* pData)
{
	KMagicAttrib* pMagic = (KMagicAttrib *)pData;
	pNpc->m_CurrentColdEnhance += pMagic->nValue[0];

    //if (pNpc->m_CurrentColdEnhance<-100)
	//    pNpc->m_CurrentColdEnhance=-90;
    if (pNpc->m_CurrentColdEnhance<=-100)
	    pNpc->m_CurrentColdEnhance=-90;
    

	//g_DebugLog("[��ֵ]%s�ٻ�ʱ�����ֵ%d��", pNpc->Name, pMagic->nValue[0]);
}

void KNpcAttribModify::FireEnhanceP(KNpc* pNpc, void* pData)
{
	KMagicAttrib* pMagic = (KMagicAttrib *)pData;
	pNpc->m_CurrentFireEnhance += pMagic->nValue[0];
	//g_DebugLog("[��ֵ]%s����ǿ������%d��", pNpc->Name, pMagic->nValue[0]);
}

void KNpcAttribModify::LightingEnhanceP(KNpc* pNpc, void* pData)
{
	KMagicAttrib* pMagic = (KMagicAttrib *)pData;
	pNpc->m_CurrentLightEnhance += pMagic->nValue[0];

	if  (pNpc->m_CurrentLightEnhance<0)
	    pNpc->m_CurrentLightEnhance=0;
	//g_DebugLog("[��ֵ]%s����ǿ������%d��", pNpc->Name, pMagic->nValue[0]);
}

void KNpcAttribModify::PoisonEnhanceP(KNpc* pNpc, void* pData)
{
	KMagicAttrib* pMagic = (KMagicAttrib *)pData;
	pNpc->m_CurrentPoisonEnhance += pMagic->nValue[0];

    if (pNpc->m_CurrentPoisonEnhance<-100)
	    pNpc->m_CurrentPoisonEnhance=-100;
	   
	//g_DebugLog("[��ֵ]%s����ʱ�����ֵ����%d�ٷֱ�", pNpc->Name, pMagic->nValue[0]);
}
//ҩƷ��Ѫ״̬
void KNpcAttribModify::LifePotionV(KNpc* pNpc, void* pData)
{
	KMagicAttrib* pMagic = (KMagicAttrib *)pData;

	if (pMagic->nValue[1] <= 0)
		return;

	int nX1, nY1, nX2, nY2;
	nX1 = pNpc->m_LifeState.nValue[0];
	nY1 = pNpc->m_LifeState.nTime;
	nX2 = pMagic->nValue[0];  
	nY2 = pMagic->nValue[1];
	pNpc->m_LifeState.nTime = max(nY1, nY2);
	// ���������֤ʱ������㣬�������ֵһ�������㣬���Բ����жϳ���
	if  (pNpc->m_LifeState.nTime!=0)
		pNpc->m_LifeState.nValue[0] = (nX1 * nY1 + nX2 * nY2) / pNpc->m_LifeState.nTime;
	else
		pNpc->m_LifeState.nValue[0] = 0;
	//g_DebugLog("[��ֵ]%s���˲�Ѫҩ�����%d֡��ÿ10֡�ظ�%d��������Ч��", pNpc->Name,
	//	pNpc->m_LifeState.nTime, 
	//	pNpc->m_LifeState.nValue[0]);
}

//ҩƷ����״̬
void KNpcAttribModify::dupotion_v(KNpc* pNpc, void* pData)
{
	KMagicAttrib* pMagic = (KMagicAttrib *)pData;
	
	if (pMagic->nValue[1] <= 0)
		return;

	//if (pNpc->m_PoisonState.nTime<=0)
	//	return;

	if (pNpc->m_PoisonState.nTime <= 0)
		return;

    //m_PoisonState.nValue[1] ����ʱ��   m_PoisonState.nValue[0]  ���˺�
	pNpc->m_PoisonState.nTime = pNpc->m_PoisonState.nTime + pMagic->nValue[0];
	
	if (pNpc->m_PoisonState.nTime<0)
		pNpc->m_PoisonState.nTime = 0;
	/*
	pNpc->m_PoisonState.nTime +=pMagic->nValue[0];

	if (pNpc->m_PoisonState.nTime<0)
		pNpc->m_PoisonState.nTime=0;
	 */
	//pMagic->nValue[0];  //����ֵ  
	//pMagic->nValue[1];  //����ʱ��

}

void KNpcAttribModify::ManaPotionV(KNpc* pNpc, void* pData)
{
	KMagicAttrib* pMagic = (KMagicAttrib *)pData;

	if (pMagic->nValue[1] <= 0)
		return;
	int nX1, nY1, nX2, nY2;
	nX1 = pNpc->m_ManaState.nValue[0];
	nY1 = pNpc->m_ManaState.nTime;
	nX2 = pMagic->nValue[0];
	nY2 = pMagic->nValue[1];
	pNpc->m_ManaState.nTime = max(nY1, nY2);
	// ���������֤ʱ������㣬�������ֵһ�������㣬���Բ����жϳ���
	if (pNpc->m_ManaState.nTime!=0) 
		pNpc->m_ManaState.nValue[0] = (nX1 * nY1 + nX2 * nY2) / pNpc->m_ManaState.nTime;
	else
		pNpc->m_ManaState.nValue[0] = 0;
	//g_DebugLog("[��ֵ]%s���˲�����ҩ�����%d֡��ÿ10֡�ظ�%d��������Ч��", pNpc->Name,
	//	pNpc->m_ManaState.nTime, 
	//	pNpc->m_ManaState.nValue[0]);
}

void KNpcAttribModify::PhysicsResMaxP(KNpc* pNpc, void* pData)
{
	KMagicAttrib* pMagic = (KMagicAttrib *)pData;

	pNpc->m_CurrentPhysicsResistMax += pMagic->nValue[0];

	if (pNpc->m_CurrentPhysicsResistMax>pNpc->m_PhysicsResistMax)
		pNpc->m_CurrentPhysicsResistMax=pNpc->m_PhysicsResistMax;

	//g_DebugLog("[��ֵ]%s����������������%d��", pNpc->Name, pMagic->nValue[0]);	
}

void KNpcAttribModify::ColdResMaxP(KNpc* pNpc, void* pData)
{
	KMagicAttrib* pMagic = (KMagicAttrib *)pData;
	pNpc->m_CurrentColdResistMax += pMagic->nValue[0];

	if (pNpc->m_CurrentColdResistMax>pNpc->m_ColdResistMax)
		pNpc->m_CurrentColdResistMax=pNpc->m_ColdResistMax;

    

	//g_DebugLog("[��ֵ]%s����������������%d��", pNpc->Name, pMagic->nValue[0]);	
}

void KNpcAttribModify::FireResMaxP(KNpc* pNpc, void* pData)
{
	KMagicAttrib* pMagic = (KMagicAttrib *)pData;
	pNpc->m_CurrentFireResistMax += pMagic->nValue[0];

	if (pNpc->m_CurrentFireResistMax>pNpc->m_FireResistMax)
		pNpc->m_CurrentFireResistMax=pNpc->m_FireResistMax;

	//g_DebugLog("[��ֵ]%s��������������%d��", pNpc->Name, pMagic->nValue[0]);	
}

void KNpcAttribModify::LightingResMaxP(KNpc* pNpc, void* pData)
{
	KMagicAttrib* pMagic = (KMagicAttrib *)pData;
	pNpc->m_CurrentLightResistMax += pMagic->nValue[0];

	if (pNpc->m_CurrentLightResistMax>pNpc->m_LightResistMax)
		pNpc->m_CurrentLightResistMax=pNpc->m_LightResistMax;
	//g_DebugLog("[��ֵ]%s�翹������������%d��", pNpc->Name, pMagic->nValue[0]);	
}

void KNpcAttribModify::PoisonResMaxP(KNpc* pNpc, void* pData)
{
	KMagicAttrib* pMagic = (KMagicAttrib *)pData;
	pNpc->m_CurrentPoisonResistMax += pMagic->nValue[0];

	if (pNpc->m_CurrentPoisonResistMax>pNpc->m_PoisonResistMax)
		pNpc->m_CurrentPoisonResistMax=pNpc->m_PoisonResistMax;

	//g_DebugLog("[��ֵ]%s����������������%d��", pNpc->Name, pMagic->nValue[0]);	
}
//ȫ������ �ٷֱ�
void KNpcAttribModify::AllResMaxP(KNpc* pNpc, void* pData)
{
	KMagicAttrib* pMagic = (KMagicAttrib *)pData;
	pNpc->m_CurrentFireResistMax    += pMagic->nValue[0];
	pNpc->m_CurrentColdResistMax    += pMagic->nValue[0];
	pNpc->m_CurrentLightResistMax   += pMagic->nValue[0];
	pNpc->m_CurrentPoisonResistMax  += pMagic->nValue[0];
	pNpc->m_CurrentPhysicsResistMax += pMagic->nValue[0];

	if (pNpc->m_CurrentPoisonResistMax>pNpc->m_PoisonResistMax)
		pNpc->m_CurrentPoisonResistMax=pNpc->m_PoisonResistMax;
	if (pNpc->m_CurrentLightResistMax>pNpc->m_LightResistMax)
		pNpc->m_CurrentLightResistMax=pNpc->m_LightResistMax;
	if (pNpc->m_CurrentFireResistMax>pNpc->m_FireResistMax)
		pNpc->m_CurrentFireResistMax=pNpc->m_FireResistMax;
	if (pNpc->m_CurrentColdResistMax>pNpc->m_ColdResistMax)
		pNpc->m_CurrentColdResistMax=pNpc->m_ColdResistMax;
	if (pNpc->m_CurrentPhysicsResistMax>pNpc->m_PhysicsResistMax)
		pNpc->m_CurrentPhysicsResistMax=pNpc->m_PhysicsResistMax;

	//g_DebugLog("[��ֵ]%s���������������%d", pNpc->Name, pMagic->nValue[0]);
}

void KNpcAttribModify::FatallyStrikeP(KNpc* pNpc, void* pData)
{
}
//�ϳɶ��˺�
void KNpcAttribModify::MixPoisonDamage(KMagicAttrib* pDes, KMagicAttrib* pSrc)
{
	int d1, d2, t1, t2, c1, c2;
	d1 = pDes->nValue[0];
	d2 = pSrc->nValue[0];
	t1 = pDes->nValue[1];
	t2 = pSrc->nValue[1];
	c1 = pDes->nValue[2];
	c2 = pSrc->nValue[2];
	if (c1 == 0 || d1 == 0)
	{
		memcpy(pDes, pSrc, sizeof(KMagicAttrib));
		return;
	}
	if (c2 == 0 || d2 == 0)
	{
		return;
	}
	pDes->nValue[0] = ((c1 + c2) * d1 / c1 + (c1 + c2) * d2 / c2) / 2;
	pDes->nValue[1] = (t1 * d1 * c2 + t2 *d2 * c1) /(d1 * c2 + d2 * c1);
	pDes->nValue[2] += (c1 + c2) / 2;

/*	int NpcPoisonDamageMax=0,PlayerPoisonDamageMax=0; //�ϳɶ��˺������ֵ    
    g_GameSetTing.GetInteger("ServerConfig","NpcPoisonDamageMax",0,&NpcPoisonDamageMax);
    g_GameSetTing.GetInteger("ServerConfig","PlayerPoisonDamageMax",0,&PlayerPoisonDamageMax);
	if (pDes->nValue[2] >= NpcPoisonDamageMax)
		pDes->nValue[2]  = NpcPoisonDamageMax;*/

}
//�յ㣨�������ݣ�
void KNpcAttribModify::AddPhysicsMagic( KNpc* pNpc, void* pData )
{
	KMagicAttrib* pMagic = (KMagicAttrib *)pData;
	pNpc->m_CurrentPhysicsMagicDamageV.nValue[0] += pMagic->nValue[0];
	pNpc->m_CurrentPhysicsMagicDamageV.nValue[2] += pMagic->nValue[0];
	//g_DebugLog("[��ֵ]���յ�%s���˺�����(%d)", pNpc->Name, pMagic->nValue[0]);
}
//�յ�ٷֱȣ��������ݣ�
void KNpcAttribModify::Add_neiphysicsenhance_p(KNpc* pNpc, void* pData)
{
	KMagicAttrib* pMagic = (KMagicAttrib *)pData;
	pNpc->m_CurrentPhysicsMagicDamageP.nValue[0] += pMagic->nValue[0];
	pNpc->m_CurrentPhysicsMagicDamageP.nValue[2] += pMagic->nValue[0];

	if (pNpc->m_CurrentPhysicsMagicDamageP.nValue[0]<0)
	   pNpc->m_CurrentPhysicsMagicDamageP.nValue[0]=GetRandomNumber(10,20);

	if (pNpc->m_CurrentPhysicsMagicDamageP.nValue[2]<0)
	    pNpc->m_CurrentPhysicsMagicDamageP.nValue[2]=pNpc->m_CurrentPhysicsMagicDamageP.nValue[0]+GetRandomNumber(10,20);
/*#ifdef _SERVER
    char nmsg[64];
	sprintf(nmsg,"Cԭʼ�ٷֱ�%d,���ܰ׷ֱ�:%d",pNpc->m_CurrentPhysicsMagicDamageP.nValue[0],pMagic->nValue[0]);
    //pNpc->ClientShowMsg(nmsg);
	Player[pNpc->m_nPlayerIdx].m_ItemList.msgshow(nmsg);
#endif
	*/
}
//���˺����������ݣ�Player[pNpc->m_nPlayerIdx].
void KNpcAttribModify::AddColdMagic( KNpc* pNpc, void* pData )
{
#define	defNORMAL_COLD_TIME	60
	KMagicAttrib* pMagic = (KMagicAttrib *)pData;
	pNpc->m_CurrentMagicColdDamage.nValue[0] += pMagic->nValue[0]; //��С
	pNpc->m_CurrentMagicColdDamage.nValue[2] += pMagic->nValue[0]; //���
	if (pNpc->m_CurrentMagicColdDamage.nValue[0] > 0 && pNpc->m_CurrentMagicColdDamage.nValue[2] > 0)
		pNpc->m_CurrentMagicColdDamage.nValue[1] = defNORMAL_COLD_TIME;
	else
	{
		pNpc->m_CurrentMagicColdDamage.nValue[0] = 0;
		pNpc->m_CurrentMagicColdDamage.nValue[1] = 0;
		pNpc->m_CurrentMagicColdDamage.nValue[2] = 0;
	}
	//g_DebugLog("[��ֵ]%s���˺�����(%d)", pNpc->Name, pMagic->nValue[0]);
}
//���˺����������ݣ�
void KNpcAttribModify::AddFireMagic( KNpc* pNpc, void* pData )
{
	KMagicAttrib* pMagic = (KMagicAttrib *)pData;
	pNpc->m_CurrentMagicFireDamage.nValue[0] += pMagic->nValue[0];
	pNpc->m_CurrentMagicFireDamage.nValue[2] += pMagic->nValue[0];
	//g_DebugLog("[��ֵ]%s���˺�����(%d)", pNpc->Name, pMagic->nValue[0]);

    if (pNpc->m_CurrentMagicFireDamage.nValue[0]<0)
	   pNpc->m_CurrentMagicFireDamage.nValue[0]=GetRandomNumber(10,20);

	if (pNpc->m_CurrentMagicFireDamage.nValue[2]<0)
	    pNpc->m_CurrentMagicFireDamage.nValue[2]=pNpc->m_CurrentMagicFireDamage.nValue[0]+GetRandomNumber(10,20);

}
//���˺����������ݣ�
void KNpcAttribModify::AddLightingMagic( KNpc* pNpc, void* pData )
{
	KMagicAttrib* pMagic = (KMagicAttrib *)pData;
	pNpc->m_CurrentMagicLightDamage.nValue[0] += pMagic->nValue[0];
	pNpc->m_CurrentMagicLightDamage.nValue[2] += pMagic->nValue[0];
	//g_DebugLog("[��ֵ]%s���˺�����(%d)", pNpc->Name, pMagic->nValue[0]);
	if (pNpc->m_CurrentMagicLightDamage.nValue[0]<0)
	   pNpc->m_CurrentMagicLightDamage.nValue[0]=GetRandomNumber(10,20);

	if (pNpc->m_CurrentMagicLightDamage.nValue[2]<0)
	    pNpc->m_CurrentMagicLightDamage.nValue[2]=pNpc->m_CurrentMagicLightDamage.nValue[0]+GetRandomNumber(10,20);
}
//���˺����������ݣ�
void KNpcAttribModify::AddPoisonMagic( KNpc* pNpc, void* pData )
{
#define	defPOISON_DAMAGE_TIME		60  //����ʱ��
#define	defPOISON_DAMAGE_INTERVAL	10  //����ʱ��

	KMagicAttrib* pMagic = (KMagicAttrib *)pData;
	pNpc->m_CurrentMagicPoisonDamage.nValue[0] += pMagic->nValue[0];
	
	if (pNpc->m_CurrentMagicPoisonDamage.nValue[0] > 0)  //���˺�
	{
		pNpc->m_CurrentMagicPoisonDamage.nValue[1] = defPOISON_DAMAGE_TIME;    //pMagic->nValue[1];
		pNpc->m_CurrentMagicPoisonDamage.nValue[2] = defPOISON_DAMAGE_INTERVAL;//pMagic->nValue[2];
	}
	else
	{
		pNpc->m_CurrentMagicPoisonDamage.nValue[0] = 0;
		pNpc->m_CurrentMagicPoisonDamage.nValue[1] = 0;
		pNpc->m_CurrentMagicPoisonDamage.nValue[2] = 0;
	}	
	//g_DebugLog("[��ֵ]%s��״̬��Ϊ�����%d֡����һ��%d�˺���������%d֡", pNpc->Name, pNpc->m_CurrentPoisonDamage.nValue[2], pNpc->m_CurrentPoisonDamage.nValue[0], pNpc->m_CurrentPoisonDamage.nValue[1]);
}

////////////////////////�¹�������////////////////////////////////
//���յ�
/*void KNpcAttribModify::XinPhysicsDamageV(KNpc* pNpc, void* pData)
{
	KMagicAttrib* pMagic = (KMagicAttrib *)pData;

	pNpc->m_WaiPhysicsDamage.nValue[0] += pMagic->nValue[0];
	pNpc->m_WaiPhysicsDamage.nValue[2] += pMagic->nValue[2];
	//g_DebugLog("[��ֵ]�⹦�յ�%s�����˺�������(%d)", pNpc->Name, pMagic->nValue[0]);
}

//�±��˺�
void KNpcAttribModify::XinColdDamageV(KNpc* pNpc, void* pData)
{	
#define	defNORMAL_COLD_TIME	60  //����ʱ��
	KMagicAttrib* pMagic = (KMagicAttrib *)pData;
	pNpc->m_CurrentColdDamage.nValue[0] += pMagic->nValue[0];
	pNpc->m_CurrentColdDamage.nValue[2] += pMagic->nValue[0];
	if (pNpc->m_CurrentColdDamage.nValue[0] > 0 && pNpc->m_CurrentColdDamage.nValue[2] > 0)
		pNpc->m_CurrentColdDamage.nValue[1] = defNORMAL_COLD_TIME;
	else
	{
		pNpc->m_CurrentColdDamage.nValue[0] = 0;
		pNpc->m_CurrentColdDamage.nValue[1] = 0;
		pNpc->m_CurrentColdDamage.nValue[2] = 0;
	}
	//g_DebugLog("[��ֵ]%s���˺�����(%d)", pNpc->Name, pMagic->nValue[0]);
}
//�»��˺�
void KNpcAttribModify::XinFireDamageV(KNpc* pNpc, void* pData)
{
	KMagicAttrib* pMagic = (KMagicAttrib *)pData;
	pNpc->m_CurrentFireDamage.nValue[0] += pMagic->nValue[0];
	pNpc->m_CurrentFireDamage.nValue[2] += pMagic->nValue[0];
	//g_DebugLog("[��ֵ]%s���˺�����(%d)", pNpc->Name, pMagic->nValue[0]);
}
//�����˺�
void KNpcAttribModify::XinLightingDamageV(KNpc* pNpc, void* pData)
{
	KMagicAttrib* pMagic = (KMagicAttrib *)pData;
	pNpc->m_CurrentLightDamage.nValue[0] += pMagic->nValue[0];
	pNpc->m_CurrentLightDamage.nValue[2] += pMagic->nValue[0];
	//g_DebugLog("[��ֵ]%s���˺�����(%d)", pNpc->Name, pMagic->nValue[0]);
}
//�¶��˺�
void KNpcAttribModify::XinPoisonDamageV(KNpc* pNpc, void* pData)
{
#define	defPOISON_DAMAGE_TIME		60
#define	defPOISON_DAMAGE_INTERVAL	10
	KMagicAttrib* pMagic = (KMagicAttrib *)pData;
	pNpc->m_CurrentPoisonDamage.nValue[0] += pMagic->nValue[0];
	
	if (pNpc->m_CurrentPoisonDamage.nValue[0] > 0)
	{
		pNpc->m_CurrentPoisonDamage.nValue[1] = defPOISON_DAMAGE_TIME;//pMagic->nValue[1];
		pNpc->m_CurrentPoisonDamage.nValue[2] = defPOISON_DAMAGE_INTERVAL;//pMagic->nValue[2];
	}
	else
	{
		pNpc->m_CurrentPoisonDamage.nValue[0] = 0;   //�˺�
		pNpc->m_CurrentPoisonDamage.nValue[1] = 0;   //����ʱ��
		pNpc->m_CurrentPoisonDamage.nValue[2] = 0;   //���ʱ�䷢��һ��
	}	
	//g_DebugLog("[��ֵ]%s��״̬��Ϊ�����%d֡����һ��%d�˺���������%d֡", pNpc->Name, pNpc->m_CurrentPoisonDamage.nValue[2], pNpc->m_CurrentPoisonDamage.nValue[0], pNpc->m_CurrentPoisonDamage.nValue[1]);	
}
//���հٷֱ��˺�
void KNpcAttribModify::XinPhysicsDamageP(KNpc* pNpc, void* pData)
{
#define WEAPON_ALL			((MAX_MELEE_WEAPON) + 0)
#define	WEAPON_RANGE_ALL	((MAX_MELEE_WEAPON) + 1)
#define	WEAPON_MELEE_ALL	((MAX_MELEE_WEAPON) + 2)
#define	WEAPON_NONE			((MAX_MELEE_WEAPON) + 3)

	KMagicAttrib* pMagic = (KMagicAttrib *)pData;

	int nType = abs(pMagic->nValue[2]);

	if (nType == WEAPON_ALL)
	{
		for (int i = 0; i < MAX_MELEE_WEAPON; ++i)
		{
			pNpc->m_CurrentMeleeEnhance[i] += pMagic->nValue[0];
		}
		pNpc->m_CurrentHandEnhance += pMagic->nValue[0];
		pNpc->m_CurrentRangeEnhance += pMagic->nValue[0];
		//g_DebugLog("[��ֵ]%s���������������Ӱٷ�֮%d", pNpc->Name, pMagic->nValue[0]);
	}
	else if (nType == WEAPON_RANGE_ALL)
	{
		pNpc->m_CurrentRangeEnhance += pMagic->nValue[0];
		//g_DebugLog("[��ֵ]%sԶ�������������Ӱٷ�֮%d", pNpc->Name, pMagic->nValue[0]);
	}
	else if (nType == WEAPON_NONE)
	{
		pNpc->m_CurrentHandEnhance += pMagic->nValue[0];
		//g_DebugLog("[��ֵ]%s���������������Ӱٷ�֮%d", pNpc->Name, pMagic->nValue[0]);
	}
	else if (nType == WEAPON_MELEE_ALL)
	{
		for (int i = 0; i < MAX_MELEE_WEAPON; ++i)
		{
			pNpc->m_CurrentMeleeEnhance[i] += pMagic->nValue[0];
		}
		//g_DebugLog("[��ֵ]%s���������������Ӱٷ�֮%d", pNpc->Name, pMagic->nValue[0]);
	}
	else if (nType >= 0 && nType < MAX_MELEE_WEAPON)
	{
		pNpc->m_CurrentMeleeEnhance[nType] += pMagic->nValue[0];
		//g_DebugLog("[��ֵ]%s��������%d�����������Ӱٷ�֮%d", pNpc->Name, nType, pMagic->nValue[0]);
	}
	
//	pNpc->m_CurrentAddPhysicsDamageP+=pMagic->nValue[0]; //��ǰ���հٷֱ�
}
*/
/////////////////////////װ�����ݣ���״̬���ͼ�������(��״̬��/////////////////////////
//��ǰ���յ�ٷֱ� ����
void KNpcAttribModify::AddPhysicsDamagePP(KNpc* pNpc, void* pData)
{
	KMagicAttrib* pMagic = (KMagicAttrib *)pData;
	pNpc->m_CurrentAddPhysicsDamageP +=pMagic->nValue[0]; //��ǰ���հٷֱ�

    if (pNpc->m_CurrentAddPhysicsDamageP<-100)
	   pNpc->m_CurrentAddPhysicsDamageP=-100;
	   
}
//��ǰ���յ� ����
void KNpcAttribModify::Addphysicsdamagevp(KNpc* pNpc, void* pData)
{
    KMagicAttrib* pMagic = (KMagicAttrib *)pData;
    pNpc->m_CurrentAddPhysicsDamage +=pMagic->nValue[0]; 

	 if (pNpc->m_CurrentAddPhysicsDamage<0)
	   pNpc->m_CurrentAddPhysicsDamage=GetRandomNumber(10,20);

}
//��ǰ���� ����
void KNpcAttribModify::Aaddfiredamagevp(KNpc* pNpc, void* pData)
{
    KMagicAttrib* pMagic = (KMagicAttrib *)pData;
    pNpc->m_CurrentAddFireDamagev +=pMagic->nValue[0]; 

	 if (pNpc->m_CurrentAddFireDamagev<0)
	   pNpc->m_CurrentAddFireDamagev=GetRandomNumber(10,20);
}
//��ǰ����� ����
void KNpcAttribModify::Addcolddamagevp(KNpc* pNpc, void* pData)
{
    KMagicAttrib* pMagic = (KMagicAttrib *)pData;
    pNpc->m_CurrentAddColdDamagev +=pMagic->nValue[0]; 

	 if (pNpc->m_CurrentAddColdDamagev<0)
	   pNpc->m_CurrentAddColdDamagev=GetRandomNumber(10,20);
}
//��ǰ���׵� ����
void KNpcAttribModify::Addlightingdamagevp(KNpc* pNpc, void* pData)
{
    KMagicAttrib* pMagic = (KMagicAttrib *)pData;
    pNpc->m_CurrentAddLighDamagev +=pMagic->nValue[0]; 

    if (pNpc->m_CurrentAddLighDamagev<0)
	   pNpc->m_CurrentAddLighDamagev=GetRandomNumber(10,20);
}
//��ǰ�ⶾ�� ����
void KNpcAttribModify::Addpoisondamagevp(KNpc* pNpc, void* pData)
{
    KMagicAttrib* pMagic = (KMagicAttrib *)pData;
    pNpc->m_CurrentAddPoisonDamagev +=pMagic->nValue[0]; 

	if (pNpc->m_CurrentAddPoisonDamagev<0)
	   pNpc->m_CurrentAddPoisonDamagev=GetRandomNumber(10,20);
}


//��ǰ���հٷֱ� ����
void KNpcAttribModify::Addphysicsmagicp(KNpc* pNpc, void* pData)
{
    KMagicAttrib* pMagic = (KMagicAttrib *)pData;
    pNpc->m_CurrentAddmagicphysicsDamageP +=pMagic->nValue[0]; 

	if (pNpc->m_CurrentAddmagicphysicsDamageP<-100)
	   pNpc->m_CurrentAddmagicphysicsDamageP=-100;
}

//��ǰ���յ� ����
void KNpcAttribModify::Addphysicsmagicvb(KNpc* pNpc, void* pData)
{
    KMagicAttrib* pMagic = (KMagicAttrib *)pData;
    pNpc->m_CurrentAddmagicphysicsDamage +=pMagic->nValue[0]; 

	if (pNpc->m_CurrentAddmagicphysicsDamage<0)
	   pNpc->m_CurrentAddmagicphysicsDamage=GetRandomNumber(10,20);
}

//��ǰ�ڱ��� ����
void KNpcAttribModify::Addcoldmagicvp(KNpc* pNpc, void* pData)
{
    KMagicAttrib* pMagic = (KMagicAttrib *)pData;
    pNpc->m_CurrentAddmagicColdDamagicv +=pMagic->nValue[0]; 

	if (pNpc->m_CurrentAddmagicColdDamagicv<0)
	   pNpc->m_CurrentAddmagicColdDamagicv=GetRandomNumber(10,20);
}

//��ǰ�ڻ�� ����
void KNpcAttribModify::Addfiremagicv(KNpc* pNpc, void* pData)
{
    KMagicAttrib* pMagic = (KMagicAttrib *)pData;
    pNpc->m_CurrentAddmagicFireDamagicv +=pMagic->nValue[0];
	if (pNpc->m_CurrentAddmagicFireDamagicv<0)
	   pNpc->m_CurrentAddmagicFireDamagicv=GetRandomNumber(10,20);
}
//��ǰ���׵� ����
void KNpcAttribModify::Addlightingmagicv(KNpc* pNpc, void* pData)
{
    KMagicAttrib* pMagic = (KMagicAttrib *)pData;
    pNpc->m_CurrentAddmagicLightDamagicv +=pMagic->nValue[0]; 

	if (pNpc->m_CurrentAddmagicLightDamagicv<0)
	   pNpc->m_CurrentAddmagicLightDamagicv=GetRandomNumber(10,20);
}
//��ǰ�ڶ��� ����
void KNpcAttribModify::Addpoisonmagicv(KNpc* pNpc, void* pData)
{
    KMagicAttrib* pMagic = (KMagicAttrib *)pData;
    pNpc->m_CurrentAddmagicPoisonDamagicv +=pMagic->nValue[0]; 
	if (pNpc->m_CurrentAddmagicPoisonDamagicv<0)
	   pNpc->m_CurrentAddmagicPoisonDamagicv=GetRandomNumber(10,20);
}

///////////////////////////////////////////////////////////////////

void KNpcAttribModify::fatallystrikeresP( KNpc* pNpc, void* pData ) //
{
	
}

void KNpcAttribModify::me2metaldamage_p( KNpc* pNpc, void* pData ) //=�Խ�ϵ�˺����ӣ�#d1+%
{
    KMagicAttrib* pMagic = (KMagicAttrib *)pData;
    pNpc->m_Me2metaldamage_p +=pMagic->nValue[0]; 

	if (pNpc->m_Me2metaldamage_p>=100)
	   pNpc->m_Me2metaldamage_p=GetRandomNumber(80,90);

}
void KNpcAttribModify::metal2medamage_p( KNpc* pNpc, void* pData ) //=�������Խ�ϵ���˺���#d1-%
{
    KMagicAttrib* pMagic = (KMagicAttrib *)pData;
	pNpc->m_Metal2medamage_p +=pMagic->nValue[0];

	if (pNpc->m_Metal2medamage_p>=100)
	   pNpc->m_Metal2medamage_p=GetRandomNumber(80,90);
}
void KNpcAttribModify::me2wooddamage_p( KNpc* pNpc, void* pData ) //=��ľϵ�˺����ӣ�#d1+%
{
    KMagicAttrib* pMagic = (KMagicAttrib *)pData;
	pNpc->m_Me2wooddamage_p +=pMagic->nValue[0];
		if (pNpc->m_Me2wooddamage_p>=100)
	   pNpc->m_Me2wooddamage_p=GetRandomNumber(80,90);
}
void KNpcAttribModify::wood2medamage_p( KNpc* pNpc, void* pData ) //=��������ľϵ���˺���#d1-%
{
    KMagicAttrib* pMagic = (KMagicAttrib *)pData;
	pNpc->m_Wood2medamage_p +=pMagic->nValue[0];
	 if (pNpc->m_Wood2medamage_p>=100)
	   pNpc->m_Wood2medamage_p=GetRandomNumber(80,90);
}
void KNpcAttribModify::me2waterdamage_p( KNpc* pNpc, void* pData ) //=��ˮϵ�˺����ӣ�#d1+%
{
    KMagicAttrib* pMagic = (KMagicAttrib *)pData;
	pNpc->m_Me2waterdamage_p +=pMagic->nValue[0];
	 if (pNpc->m_Me2waterdamage_p>=100)
	   pNpc->m_Me2waterdamage_p=GetRandomNumber(80,90);
}
void KNpcAttribModify::water2medamage_p( KNpc* pNpc, void* pData ) //=��������ˮϵ���˺���#d1-%
{
    KMagicAttrib* pMagic = (KMagicAttrib *)pData;
	pNpc->m_Water2medamage_p +=pMagic->nValue[0];
	if (pNpc->m_Water2medamage_p>=100)
	   pNpc->m_Water2medamage_p=GetRandomNumber(80,90);
}
void KNpcAttribModify::me2firedamage_p( KNpc* pNpc, void* pData ) //=�Ի�ϵ�˺����ӣ�#d1+%
{
    KMagicAttrib* pMagic = (KMagicAttrib *)pData;
	pNpc->m_Me2firedamage_p +=pMagic->nValue[0];
	if (pNpc->m_Me2firedamage_p>=100)
	   pNpc->m_Me2firedamage_p=GetRandomNumber(80,90);
}
void KNpcAttribModify::fire2medamage_p( KNpc* pNpc, void* pData ) //=�������Ի�ϵ���˺���#d1-%
{
    KMagicAttrib* pMagic = (KMagicAttrib *)pData;
	pNpc->m_Fire2medamage_p +=pMagic->nValue[0];
	if (pNpc->m_Fire2medamage_p>=100)
	   pNpc->m_Fire2medamage_p=GetRandomNumber(80,90);
}
void KNpcAttribModify::me2earthdamage_p( KNpc* pNpc, void* pData ) //=����ϵ�˺����ӣ�#d1+%
{
    KMagicAttrib* pMagic = (KMagicAttrib *)pData;
	pNpc->m_Me2earthdamage_p +=pMagic->nValue[0];
		if (pNpc->m_Me2earthdamage_p>=100)
	   pNpc->m_Me2earthdamage_p=GetRandomNumber(80,90);


}
void KNpcAttribModify::earth2medamage_p( KNpc* pNpc, void* pData ) //=����������ϵ���˺���#d1-%
{
    KMagicAttrib* pMagic = (KMagicAttrib *)pData;
	pNpc->m_Earth2medamage_p +=pMagic->nValue[0];
		if (pNpc->m_Earth2medamage_p>=100)
	   pNpc->m_Earth2medamage_p=GetRandomNumber(80,90);
}

//״ִ̬�нű�
void KNpcAttribModify::doscript_v(KNpc* pNpc, void* pData)
{
	   KMagicAttrib* pMagic = (KMagicAttrib *)pData;
	   pNpc->m_DoScriptState.nMagicAttrib=0;
       pNpc->m_DoScriptState.nValue[0]=pMagic->nValue[0]; //ÿ�ȼ��ľ���
	   pNpc->m_DoScriptState.nValue[1]=pMagic->nValue[1]; //ÿ����Ķ�����ִ��һ��
	   pNpc->m_DoScriptState.nValue[2]=pMagic->nValue[2]; //������ʱ��Ϊ������
	   pNpc->m_DoScriptState.nTime=pMagic->nValue[2];     //������ʱ��Ϊ������
}

void KNpcAttribModify::addexp_v(KNpc* pNpc, void* pData)
{
	   KMagicAttrib* pMagic = (KMagicAttrib *)pData;
	   pNpc->m_ExpState.nMagicAttrib=0;
       pNpc->m_ExpState.nValue[0]=pMagic->nValue[0]; //ÿ�ȼ��ľ���
	   pNpc->m_ExpState.nValue[1]=pMagic->nValue[1]; //ÿ����Ķ�����ִ��һ��
	   pNpc->m_ExpState.nValue[2]=pMagic->nValue[2]; //����
	   pNpc->m_ExpState.nTime=pMagic->nValue[1]*2;   //������ʱ��Ϊ������
}

void KNpcAttribModify::randmove( KNpc* pNpc, void* pData)
{
    KMagicAttrib* pMagic = (KMagicAttrib *)pData;
//#ifdef _SERVER
     pNpc->m_randmove.nMagicAttrib=0;
	 pNpc->m_randmove.nValue[0]=pMagic->nValue[0]; //�Ƿ�Ⱥ���ƶ�
	 pNpc->m_randmove.nValue[1]=pMagic->nValue[2]; //����
     pNpc->m_randmove.nTime=pMagic->nValue[1];     //������ʱ��
//#endif
	 NpcAI.Setscpos(pNpc->m_Index);


	 /*#ifdef _SERVER
	  if (pNpc->IsPlayer()) 
	  {
		char msg[64];
		 sprintf(msg,"Ⱥ���ƶ�:%d ʱ��!",pNpc->m_randmove.nTime);
		 Player[pNpc->GetPlayerIdx()].m_ItemList.msgshow(msg); 

	  }
	 #endif	*/

}

void KNpcAttribModify::addbaopoisondmax_p( KNpc* pNpc, void* pData)
{
    KMagicAttrib* pMagic = (KMagicAttrib *)pData;
}




//�Զ�����ȫ��
void KNpcAttribModify::creatstatus_v( KNpc* pNpc, void* pData )
{
       KMagicAttrib* pMagic = (KMagicAttrib *)pData;
	   pNpc->m_DoScriptState.nMagicAttrib=0;
       pNpc->m_DoScriptState.nValue[0]=pMagic->nValue[0]; //ÿ�ȼ��ľ���
	   pNpc->m_DoScriptState.nValue[1]=pMagic->nValue[1]; //ÿ����Ķ�����ִ��һ��
	   pNpc->m_DoScriptState.nValue[2]=pMagic->nValue[2]; //������ʱ��Ϊ������
	   pNpc->m_DoScriptState.nTime=pMagic->nValue[2];     //������ʱ��Ϊ������
}

//�Զ�����ȫ��
void KNpcAttribModify::alljihuo_v( KNpc* pNpc, void* pData )
{
	KMagicAttrib* pMagic = (KMagicAttrib *)pData;
    pNpc->m_CurrentAllJiHuo +=pMagic->nValue[0];
}
//���ԶԷ�������
void KNpcAttribModify::ignoreattacrating_v( KNpc* pNpc, void* pData )
{
	KMagicAttrib* pMagic = (KMagicAttrib *)pData;
    pNpc->m_CurrentIgnorenAttacRating +=pMagic->nValue[0];
}


//��ĳ���ܼӳ�
void KNpcAttribModify::addskilldamage7( KNpc* pNpc, void* pData )
{
	KMagicAttrib* pMagic = (KMagicAttrib *)pData;

	/*pNpc->_EnhanceInfo::iterator it;
	for( it = pNpc->nEnhanceInfo.begin(); it != nEnhanceInfo.end(); ++it)
	{
		if (it->second.nSkillIdx>0)
		{
			ReWhereSkillEnhance(it->second.nSkillIdx,it->second.nEnhance,1);	 		
		}
	} */
	if (pMagic->nValue[0]< 0)
		pMagic->nValue[0] = -pMagic->nValue[0];

	if (pNpc->nEnhanceInfo.count(pMagic->nValue[0])>0)
	{
		pNpc->nEnhanceInfo[pMagic->nValue[0]].nSkillIdx = pMagic->nValue[0];
		int nTempval = pNpc->nEnhanceInfo[pMagic->nValue[0]].nEnhance + pMagic->nValue[2];
		pNpc->nEnhanceInfo[pMagic->nValue[0]].nEnhance = nTempval;
/*#ifdef _SERVER
		printf("--[����]ԭʼ����(%d),�ӳ� %d --\n",pMagic->nValue[0],pMagic->nValue[2]);
		printf("--[����]�Լ���(%d)�ӳ� %d --\n",pNpc->nEnhanceInfo[pMagic->nValue[0]].nSkillIdx,pNpc->nEnhanceInfo[pMagic->nValue[0]].nEnhance);
#endif */

	}
	else
	{	
		/*
		���·��������⿪����
		
           enumMap.insert(map<int, CString> :: value_type(2, "Two"))
		
		CEnhanceInfo nVal;
		nVal.nSkillIdx = pMagic->nValue[0];
		nVal.nEnhance = pMagic->nValue[2];
		pNpc->nEnhanceInfo.insert(pair<INT, CEnhanceInfo> :: value_type(pMagic->nValue[0], nVal));
	  	 */
		pNpc->nEnhanceInfo[pMagic->nValue[0]].nSkillIdx = pMagic->nValue[0];
		pNpc->nEnhanceInfo[pMagic->nValue[0]].nEnhance  = pMagic->nValue[2];
		
/*#ifdef _SERVER
		printf("--[�½�]ԭʼ����(%d),�ӳ� %d --\n",pMagic->nValue[0],pMagic->nValue[2]);
		printf("--[�½�]�Լ���(%d)�ӳ� %d    --\n",pNpc->nEnhanceInfo[pMagic->nValue[0]].nSkillIdx,pNpc->nEnhanceInfo[pMagic->nValue[0]].nEnhance);
#endif*/
	}

	if (pMagic->nValue[0]>0 && pMagic->nValue[2]!= 0)
	{   
		int nActiveSkillID = pMagic->nValue[0];
		int nListIndex = pNpc->m_SkillList.FindSame(nActiveSkillID);
		if (nListIndex)
		{
			if (pNpc->m_SkillList.GetCurrentLevelByIdx(nListIndex) > 0)    //�ȼ�����0�Ĳ� ����
			{
				pNpc->m_SkillList.AddEnChance(nListIndex,pMagic->nValue[2]);
			}	 		
		}
	}
}

//��ĳ���ܼӳ�
void KNpcAttribModify::addskilldamage1( KNpc* pNpc, void* pData )
{
	KMagicAttrib* pMagic = (KMagicAttrib *)pData;

	if (pMagic->nValue[0] != 0)
	{
		int Same;
		if (pMagic->nValue[0] > 0)
			Same = pMagic->nValue[0];
		else
			Same = -pMagic->nValue[0];

		int nlistIndex = pNpc->m_SkillList.FindSame(Same);
		if (nlistIndex)
		{
			if (pNpc->m_SkillList.GetCurrentLevelByIdx(nlistIndex) > 0)
			{
//				pNpc->m_CurrentStaminaMax += pMagic->nValue[1];

				pNpc->m_SkillList.AddEnChance(nlistIndex,pMagic->nValue[2]);// ��ǰ���ܵ��ܼӳ���
			}			
		}
	}
	else if (pMagic->nValue[0] == 0)  //���û��ָ�����ܣ����ȫ�����ܼӳ�
	{
		for (int i = 1;i < MAX_NPCSKILL;++i)
		{
			if (pNpc->m_SkillList.m_Skills[i].SkillId&&
				pNpc->m_SkillList.m_Skills[i].SkillId!=1&&
				pNpc->m_SkillList.m_Skills[i].SkillId!=2&&
				pNpc->m_SkillList.m_Skills[i].SkillId!=53)
			{
				if (pNpc->m_SkillList.GetCurrentLevel(pNpc->m_SkillList.GetSkillId(i)) > 0)
				{
					pNpc->m_SkillList.AddEnChance(i,pMagic->nValue[2]);					
				}
			}
		}
	}
}

void KNpcAttribModify::addskilldamage2( KNpc* pNpc, void* pData )
{
	KMagicAttrib* pMagic = (KMagicAttrib *)pData;

	if (pMagic->nValue[0] != 0)
	{
		int Same;
		if (pMagic->nValue[0] > 0)
			Same = pMagic->nValue[0];
		else
			Same = -pMagic->nValue[0];

		int nlistIndex = pNpc->m_SkillList.FindSame(Same);
		if (nlistIndex)
		{
			if (pNpc->m_SkillList.GetCurrentLevelByIdx(nlistIndex) > 0)
			{
				pNpc->m_SkillList.AddEnChance(nlistIndex,pMagic->nValue[2]);
			}			
		}
	}
	else if (pMagic->nValue[0] == 0)
	{
		for (int i = 1;i < MAX_NPCSKILL;++i)
		{
			if(pNpc->m_SkillList.m_Skills[i].SkillId&&pNpc->m_SkillList.m_Skills[i].SkillId!=1&&pNpc->m_SkillList.m_Skills[i].SkillId!=2&&pNpc->m_SkillList.m_Skills[i].SkillId!=53)
			{
				if (pNpc->m_SkillList.GetCurrentLevelByIdx(i) > 0)
				{
					pNpc->m_SkillList.AddEnChance(i,pMagic->nValue[2]);					
				}
			}
		}
	}
}

void KNpcAttribModify::addskilldamage3( KNpc* pNpc, void* pData )
{
	KMagicAttrib* pMagic = (KMagicAttrib *)pData;

	if (pMagic->nValue[0] != 0)
	{
		int Same;
		if (pMagic->nValue[0] > 0)
			Same = pMagic->nValue[0];
		else
			Same = -pMagic->nValue[0];
		int nlistIndex = pNpc->m_SkillList.FindSame(Same);
		if (nlistIndex)
		{
			if (pNpc->m_SkillList.GetCurrentLevelByIdx(nlistIndex) > 0)
			{
				pNpc->m_SkillList.AddEnChance(nlistIndex,pMagic->nValue[2]);
			}			
		}
	}
	else if (pMagic->nValue[0] == 0)
	{
		for (int i = 1;i < MAX_NPCSKILL;++i)
		{
			if(pNpc->m_SkillList.m_Skills[i].SkillId&&pNpc->m_SkillList.m_Skills[i].SkillId!=1&&pNpc->m_SkillList.m_Skills[i].SkillId!=2&&pNpc->m_SkillList.m_Skills[i].SkillId!=53)
			{
				if (pNpc->m_SkillList.GetCurrentLevelByIdx(i) > 0)
				{
					pNpc->m_SkillList.AddEnChance(i,pMagic->nValue[2]);					
				}
			}
		}
	}
}

void KNpcAttribModify::addskilldamage4( KNpc* pNpc, void* pData )
{
	KMagicAttrib* pMagic = (KMagicAttrib *)pData;

	if (pMagic->nValue[0] != 0)
	{
		int Same;
		if (pMagic->nValue[0] > 0)
			Same = pMagic->nValue[0];
		else
			Same = -pMagic->nValue[0];
		int nlistIndex = pNpc->m_SkillList.FindSame(Same);
		if (nlistIndex)
		{
			if (pNpc->m_SkillList.GetCurrentLevelByIdx(nlistIndex) > 0)
			{
				pNpc->m_SkillList.AddEnChance(nlistIndex,pMagic->nValue[2]);
			}			
		}
	}
	else if (pMagic->nValue[0] == 0)
	{
		for (int i = 1;i < MAX_NPCSKILL;++i)
		{
			if(pNpc->m_SkillList.m_Skills[i].SkillId&&pNpc->m_SkillList.m_Skills[i].SkillId!=1&&pNpc->m_SkillList.m_Skills[i].SkillId!=2&&pNpc->m_SkillList.m_Skills[i].SkillId!=53)
			{
				if (pNpc->m_SkillList.GetCurrentLevelByIdx(i) > 0)
				{
					pNpc->m_SkillList.AddEnChance(i,pMagic->nValue[2]);					
				}
			}
		}
	}
}

void KNpcAttribModify::addskilldamage5( KNpc* pNpc, void* pData )
{
	KMagicAttrib* pMagic = (KMagicAttrib *)pData;

	if (pMagic->nValue[0] != 0)
	{
		int Same;
		if (pMagic->nValue[0] > 0)
			Same = pMagic->nValue[0];
		else
			Same = -pMagic->nValue[0];
		int nlistIndex = pNpc->m_SkillList.FindSame(Same);
		if (nlistIndex)
		{
			if (pNpc->m_SkillList.GetCurrentLevelByIdx(nlistIndex) > 0)
			{
				pNpc->m_SkillList.AddEnChance(nlistIndex,pMagic->nValue[2]);
			}			
		}
	}
	else if (pMagic->nValue[0] == 0)
	{
		for (int i = 1;i < MAX_NPCSKILL;++i)
		{
			if(pNpc->m_SkillList.m_Skills[i].SkillId&&pNpc->m_SkillList.m_Skills[i].SkillId!=1&&pNpc->m_SkillList.m_Skills[i].SkillId!=2&&pNpc->m_SkillList.m_Skills[i].SkillId!=53)
			{
				if (pNpc->m_SkillList.GetCurrentLevelByIdx(i) > 0)
				{
					pNpc->m_SkillList.AddEnChance(i,pMagic->nValue[2]);					
				}
			}
		}
	}
}
//��ĳ���ܼӳ�
void KNpcAttribModify::addskilldamage6( KNpc* pNpc, void* pData )
{
	KMagicAttrib* pMagic = (KMagicAttrib *)pData;

	if (pMagic->nValue[0] != 0)
	{
		int Same;
		if (pMagic->nValue[0] > 0)
			Same = pMagic->nValue[0];
		else
			Same = -pMagic->nValue[0];
		int nlistIndex = pNpc->m_SkillList.FindSame(Same);
		if (nlistIndex)
		{
			if (pNpc->m_SkillList.GetCurrentLevelByIdx(nlistIndex) > 0)
			{
				pNpc->m_SkillList.AddEnChance(nlistIndex,pMagic->nValue[2]);
			}			
		}
	}
	else if (pMagic->nValue[0] == 0)
	{
		for (int i = 1;i < MAX_NPCSKILL;++i)
		{
			if(pNpc->m_SkillList.m_Skills[i].SkillId&&pNpc->m_SkillList.m_Skills[i].SkillId!=1&&pNpc->m_SkillList.m_Skills[i].SkillId!=2&&pNpc->m_SkillList.m_Skills[i].SkillId!=53)
			{
				if (pNpc->m_SkillList.GetCurrentLevelByIdx(i) > 0)
				{
					pNpc->m_SkillList.AddEnChance(i,pMagic->nValue[2]);					
				}
			}
		}
	}
}
//�Թ������ܼӳ� ---����
void KNpcAttribModify::skill_enhance(KNpc* pNpc, void* pData)     //=//�Թ������ܼӳ�
{
	if (!pNpc->IsPlayer())
		return;

	KMagicAttrib* pMagic = (KMagicAttrib *)pData;

	pNpc->m_Currentskillenhance += pMagic->nValue[0];  //����״̬���� ������

	int nActiveSkillID=0,nListIndex=0;
            nActiveSkillID=Player[CLIENT_PLAYER_INDEX].GetLeftSkill();
			nListIndex    =Player[CLIENT_PLAYER_INDEX].GetLeftSkillListidx();
			//char Msg[64];
			//sprintf(Msg,"C:����(%d)(%d)�ӳɳɹ�",nActiveSkillID,pNpc->m_Currentskillenhance);
			//Player[CLIENT_PLAYER_INDEX].m_ItemList.ClientShowMsg(Msg);
	if (pMagic->nValue[0]!= 0)
	{   
		//int nlistIndex = pNpc->m_SkillList.FindSame(nActiveSkillID);
		if (nListIndex)//   ���Ҽ���ĵ�ǰ��������ID
		{
			if (pNpc->m_SkillList.GetCurrentLevelByIdx(nListIndex) > 0)  //�ȼ�����0�Ĳ� ����
			{
				pNpc->m_SkillList.AddEnChance(nListIndex,pMagic->nValue[0]);						
			}			
		}
	}
}

//�����˺�	   �����˺�
void KNpcAttribModify::dynamicmagicshieldV(KNpc* pNpc, void* pData )
{
	KMagicAttrib* pMagic = (KMagicAttrib *)pData;
	pNpc->m_CurrentDamageReduce += pMagic->nValue[0];

	if (pNpc->m_CurrentDamageReduce<0)
		pNpc->m_CurrentDamageReduce=0;
}

void KNpcAttribModify::addstealfeatureskill( KNpc* pNpc, void* pData )
{
	
}
//���������ָ�������
void KNpcAttribModify::lifereplenishP( KNpc* pNpc, void* pData )
{
	KMagicAttrib* pMagic = (KMagicAttrib *)pData;
	pNpc->m_CurrentLifeReplenish_p +=pMagic->nValue[0];

	if (pNpc->m_CurrentLifeReplenish_p<-100)
		pNpc->m_CurrentLifeReplenish_p=-100;

}
// ���ӽ��Ϳ�������ĸ���
void KNpcAttribModify::ignoreskillP( KNpc* pNpc, void* pData )
{
	KMagicAttrib* pMagic = (KMagicAttrib *)pData;
	pNpc->m_CurrentIgnoreskillp += pMagic->nValue[0];	
}
//��������
void KNpcAttribModify::poisondamagereturnV( KNpc* pNpc, void* pData )
{
	KMagicAttrib* pMagic = (KMagicAttrib *)pData;
	pNpc->m_CurrentPoisondamagereturnV += pMagic->nValue[0];
	
}
//�������ٷֱ�
void KNpcAttribModify::poisondamagereturnP( KNpc* pNpc, void* pData )
{
	KMagicAttrib* pMagic = (KMagicAttrib *)pData;
	pNpc->m_CurrentPoisondamagereturnP += pMagic->nValue[0];
}
//������ǰ����ĸ���
void KNpcAttribModify::returnskillP( KNpc* pNpc, void* pData )
{
	KMagicAttrib* pMagic = (KMagicAttrib *)pData;
	pNpc->m_CurrentReturnskillp += pMagic->nValue[0];
}
//������ʱ��<color=orange>#d9-%<color>�ĸ����Զ�ʩ��<color=orange>#d7-��<color><color=water>#l4-<color>��ȴʱ��<color=orange>#f6-<color>��
void KNpcAttribModify::autoreplyskill(KNpc* pNpc, void* pData)
{
}
//ÿ���ͷż��ܵ�ʱ��
void KNpcAttribModify::skill_mintimepercastV( KNpc* pNpc, void* pData )
{
	KMagicAttrib* pMagic = (KMagicAttrib *)pData;
    
}
// �����ͷż��ʱ��
void KNpcAttribModify::mintimepercastonhorseV( KNpc* pNpc, void* pData )
{
	KMagicAttrib* pMagic = (KMagicAttrib *)pData;
}
// ʹĿ���ܵ����˺���ͬʱ��<color=orange>#d1-%<color>������ʧ����������<color=orange>#f2-��<color>
void KNpcAttribModify::poison2decmanaP( KNpc* pNpc, void* pData )
{
	KMagicAttrib* pMagic = (KMagicAttrib *)pData;
	if (pNpc->m_LoseMana.nTime <= 0)
	{
		if (pMagic->nValue[1] > 0 && pMagic->nValue[0] > 0)
		{
			pNpc->m_LoseMana.nValue[0] = pMagic->nValue[0];     // ������
			pNpc->m_LoseMana.nTime     = pMagic->nValue[1];     // ����ʱ��
			pNpc->m_LoseMana.nValue[1] = pMagic->nValue[1] * 2; // ����ʱ��
		}
	}
}
//��ü120 ��������<color=water>#l1-<color>����ߵȼ�<color=orange>#d2-��<color>
void KNpcAttribModify::skill_appendskil( KNpc* pNpc, void* pData )
{
	KMagicAttrib* pMagic = (KMagicAttrib *)pData;
	if (pMagic->nValue[0] > 0)
	{
		pNpc->m_IsMoreAura=true;
		for (int i = 0;i < 5;++i)
		{
			if (pNpc->m_TmpAuraID[i].skillid == pMagic->nValue[0])
				break;
			if (!pNpc->m_TmpAuraID[i].skillid)
			{
				pNpc->m_TmpAuraID[i].skillid = pMagic->nValue[0];
				pNpc->m_TmpAuraID[i].level   = pMagic->nValue[1];
				pNpc->m_TmpAuraID[i].skilllistIndex = pNpc->m_SkillList.FindSame(pNpc->m_TmpAuraID[i].skillid);
				break;
			}
		}
	}
	else
	{
		pNpc->m_IsMoreAura=false;
		for (int i = 0;i < 5;++i)
		{
			if (pNpc->m_TmpAuraID[i].skillid == -pMagic->nValue[0])
			{
				pNpc->m_TmpAuraID[i].skillid = 0;
				pNpc->m_TmpAuraID[i].skilllistIndex = 0;
				pNpc->m_TmpAuraID[i].level = 0;
			}
		}
	}
}

void KNpcAttribModify::hide( KNpc* pNpc, void* pData )
{
	KMagicAttrib* pMagic = (KMagicAttrib *)pData;
	int nFrame = pMagic->nValue[1];
	if (nFrame <= 0)
			nFrame = 1;

		pNpc->m_Hide.nTime = nFrame;  //�趨NPC ��ʱ��֡��

}
//��<color=orange>#d1-%<color>�ĸ��������Χһ����Χ���ѷ���ҵ��쳣״̬
void KNpcAttribModify::clearnegativestate( KNpc* pNpc, void* pData )
{
	
}
//�Է����˺��Ŀ���
void KNpcAttribModify::returnresP( KNpc* pNpc, void* pData )
{
	KMagicAttrib* pMagic = (KMagicAttrib *)pData;
	pNpc->m_CurrentReturnresp += pMagic->nValue[0];
}

//��ǰץ��BB���� ����״̬
void KNpcAttribModify::addcreatnpc_v( KNpc* pNpc, void* pData )
{
	KMagicAttrib* pMagic = (KMagicAttrib *)pData;
	pNpc->m_CurrentCreatnpcv += pMagic->nValue[0];
}

//��ǰץ��BB���� ����
void KNpcAttribModify::addzhuabu_v( KNpc* pNpc, void* pData )
{
	KMagicAttrib* pMagic = (KMagicAttrib *)pData;
	pNpc->m_CurrentCreatnpcv += pMagic->nValue[0];
}

//reduceskillcd1=����#l1-��ȴʱ��#f3-��
void KNpcAttribModify::reduceskillcd1( KNpc* pNpc, void* pData )
{
	KMagicAttrib* pMagic = (KMagicAttrib *)pData;

}

//reduceskillcd2=����#l1-��ȴʱ��#f3-��
void KNpcAttribModify::reduceskillcd2( KNpc* pNpc, void* pData )
{
	KMagicAttrib* pMagic = (KMagicAttrib *)pData;
	
}

//reduceskillcd3=����#l1-��ȴʱ��#f3-��
void KNpcAttribModify::reduceskillcd3( KNpc* pNpc, void* pData )
{
	KMagicAttrib* pMagic = (KMagicAttrib *)pData;
	
}

//clearallcd=#d1+%�ļ�������Լ����м�����ȴʱ��
void KNpcAttribModify::clearallcd( KNpc* pNpc, void* pData )
{
	KMagicAttrib* pMagic = (KMagicAttrib *)pData;

	
}

//addblockrate=��Χÿ#d1+����#d3+%����
void KNpcAttribModify::addblockrate( KNpc* pNpc, void* pData )
{
	KMagicAttrib* pMagic = (KMagicAttrib *)pData;
	
}

//walkrunshadow=�貨΢��
void KNpcAttribModify::walkrunshadow( KNpc* pNpc, void* pData )
{
	KMagicAttrib* pMagic = (KMagicAttrib *)pData;

}

//returnskill2enemy=�����˹���ʱ��#d1+%����ʹ���˱�����#l3-����
void KNpcAttribModify::returnskill2enemy( KNpc* pNpc, void* pData )
{
	KMagicAttrib* pMagic = (KMagicAttrib *)pData;

	//if (pNpc->m_Returnskill.nTime <= 0)
	{
			if (pMagic->nValue[0] > 0 && pMagic->nValue[1] > 0)
			{
				pNpc->m_Returnskill.nValue[0] = pMagic->nValue[0];     // ����
				//pNpc->m_Returnskill.nTime     = pMagic->nValue[1];   // �ܳ���ʱ��
				pNpc->m_Returnskill.nValue[1] = pMagic->nValue[1];     // ����ID
			}
			else
			{
                pNpc->m_Returnskill.nValue[0] = 0;     // ����
				//pNpc->m_Returnskill.nTime     = pMagic->nValue[1];   // �ܳ���ʱ��
				pNpc->m_Returnskill.nValue[1] = 0;     // ����ID
			}
	}
}

//manatoskill_enhance=������ʱ�Թ������ܼӳɣ�#d1+%
void KNpcAttribModify::manatoskill_enhance( KNpc* pNpc, void* pData )
{
	  if (!pNpc->IsPlayer())
		return;

	  KMagicAttrib* pMagic = (KMagicAttrib *)pData;
	  pNpc->m_CurrentFullManaskillenhance += pMagic->nValue[0];

	  int nActiveSkillID=0,nListIndex=0;
	  nActiveSkillID=Player[CLIENT_PLAYER_INDEX].GetLeftSkill();
	  nListIndex    =Player[CLIENT_PLAYER_INDEX].GetLeftSkillListidx();
	  if (pMagic->nValue[0]!= 0)
	  { 
		//int nlistIndex = pNpc->m_SkillList.FindSame(nActiveSkillID);
		if (nListIndex)                 //���Ҽ���ĵ�ǰ��������ID
		{
			if (pNpc->m_SkillList.GetCurrentLevelByIdx(nListIndex) > 0)  //�ȼ�����0�Ĳ� ����
			{
				pNpc->m_SkillList.AddEnChance(nListIndex,pMagic->nValue[0]);
			}			
		}
	  } 
}
//add_alldamage_v=�����˺���#d1+��
void KNpcAttribModify::add_alldamage_v( KNpc* pNpc, void* pData )
{
	KMagicAttrib* pMagic = (KMagicAttrib *)pData;

	pNpc->m_Currenadddamagev += pMagic->nValue[0];
	
}

/*
//reduceskillcd2=����#l1-��ȴʱ��#f3-��  //ĳ���ܵ���ȴʱ��
//reduceskillcd3=����#l1-��ȴʱ��#f3-��
//clearallcd=#d1+%�ļ�������Լ����м�����ȴʱ��
//addblockrate=��Χÿ#d1+����#d3+%����
//walkrunshadow=�貨΢��
//returnskill2enemy=�����˹���ʱ��#d1+%����ʹ���˱�����#l3-����
//manatoskill_enhance=������ʱ�Թ������ܼӳɣ�#d1+%
*/
//����#l3-��ȴʱ��:����<color=orange>#f7-��<color>,����<color=orange>#f4-��<color>
void KNpcAttribModify::decPercasttimehorse( KNpc* pNpc, void* pData )
{

	
}
//����#l3-��ȴʱ��:<color=orange>#f1-��<color>
void KNpcAttribModify::decPercasttime( KNpc* pNpc, void* pData )
{
	
}

//�Զ�����ĳ���ܵĸ���
void KNpcAttribModify::enhance_autoSkill( KNpc* pNpc, void* pData )
{
	
}
//����ĳ�������Ե� �����ָ��ٷֱ�
void KNpcAttribModify::enhance_lifeP( KNpc* pNpc, void* pData )
{
	
}
//����ĳ�������Ե� �����ָ��ٷֵ�
void KNpcAttribModify::enhance_lifeV( KNpc* pNpc, void* pData )
{
	
}

void KNpcAttribModify::enhance_711_auto( KNpc* pNpc, void* pData )
{
	
}

void KNpcAttribModify::enhance_714_auto( KNpc* pNpc, void* pData )
{
	
}

void KNpcAttribModify::enhance_717_auto( KNpc* pNpc, void* pData )
{
	
}

void KNpcAttribModify::enhance_723_missP( KNpc* pNpc, void* pData )
{
	
}
//ɱ�����˻�þ�������
void KNpcAttribModify::expenhanceP( KNpc* pNpc, void* pData )
{
	KMagicAttrib* pMagic = (KMagicAttrib *)pData;

	if (pNpc->IsPlayer())
	{

		if 	(pMagic->nValue[0]>5000)
			 pMagic->nValue[0]=100;

		/*if (pMagic->nValue[0]<-2000)
			 pMagic->nValue[0]=0;*/
	  
		Player[pNpc->m_nPlayerIdx].m_nUpExp += pMagic->nValue[0];

		if (Player[pNpc->m_nPlayerIdx].m_nUpExp<0)
		    Player[pNpc->m_nPlayerIdx].m_nUpExp=0;

	    /*if (Player[pNpc->m_nPlayerIdx].m_nUpExp>5000)
		    Player[pNpc->m_nPlayerIdx].m_nUpExp=100; */


	}
	//else
	//pNpc->m_CurrentUpExp +=pMagic->nValue[0];
}
void KNpcAttribModify::SerisesDamage( KNpc* pNpc, void* pData ) //�����˺�
{
	KMagicAttrib* pMagic = (KMagicAttrib *)pData;
	pNpc->m_CurrentSerisesEnhance += pMagic->nValue[0];
}
//����
void KNpcAttribModify::block_rate( KNpc* pNpc, void* pData )  //�����˺�
{
    KMagicAttrib* pMagic = (KMagicAttrib *)pData;	
    pNpc->m_CurrentdanggeRate += pMagic->nValue[0];
	if (pNpc->m_CurrentdanggeRate<0)
		pNpc->m_CurrentdanggeRate=0;
}
//�ػ�
void KNpcAttribModify::enhancehit_rate( KNpc* pNpc, void* pData )//�����󹥻���
{
	
	KMagicAttrib* pMagic = (KMagicAttrib *)pData;
    pNpc->m_CurrentzhongjiRate += pMagic->nValue[0];
	
	if (pNpc->m_CurrentzhongjiRate<0)
		pNpc->m_CurrentzhongjiRate=0;

}

/*
  int                 m_CurrentcjdanggeRate;                 //��⵵��
  int                 m_CurrentcjzhongjiRate;                //����ػ�
  int                 m_Currentsorbdamage;                   //�����˺�
  int                 m_Currentpoisonres;                    //=���ԶԷ�����:#d1-%
  int                 m_Currentfireres;                      //=���ԶԷ����:#d1-%
  int                 m_Currentlightingres;                  //=���ԶԷ��׷�:#d1-%
  int                 m_Currentphysicsres;                   //=���ԶԷ��շ�:#d1-%
  int                 m_Currentcoldres;                      //=���ԶԷ�����:#d1-%
  int                 m_Currentnopkvalue;                    //=������PKֵ����:#d1+%
  int                 m_Currentbossdamage;                   //=�Իƽ�boss�����˺�<color=orange>#d1+%<color>
  int                 m_Currentelementsenhance;              //=����ǿ��ֵ��#d1-�㡣ǿ����������еĿ���Ч��
  int                 m_Currentelementsresist;               //=��������ֵ��#d1-�㡣������������еĿ���Ч
*/
void KNpcAttribModify::anti_block_rate(KNpc* pNpc, void* pData)           //=���񵲸��ʣ�#d1+%
{
	KMagicAttrib* pMagic = (KMagicAttrib *)pData;
    pNpc->m_CurrentcjdanggeRate += pMagic->nValue[0];
	if (pNpc->m_CurrentcjdanggeRate<0)
		pNpc->m_CurrentcjdanggeRate=0;
}
void KNpcAttribModify::anti_enhancehit_rate(KNpc* pNpc, void* pData)      //=�ֵ��ػ����ʣ�#d1+%
{
	KMagicAttrib* pMagic = (KMagicAttrib *)pData;
    pNpc->m_CurrentcjzhongjiRate += pMagic->nValue[0];
	
	if (pNpc->m_CurrentcjzhongjiRate<0)
		pNpc->m_CurrentcjzhongjiRate=0;
}
void KNpcAttribModify::sorbdamage_p(KNpc* pNpc, void* pData)              //=�����˺�:#d1+��
{
	KMagicAttrib* pMagic = (KMagicAttrib *)pData;
    pNpc->m_Currentsorbdamage += pMagic->nValue[0];		
}

void KNpcAttribModify::sorbdamage_v(KNpc* pNpc, void* pData)              //=�����˺�:#d1+%
{
	KMagicAttrib* pMagic = (KMagicAttrib *)pData;
    pNpc->m_Currentsorbdamage_v += pMagic->nValue[0];		
}


void KNpcAttribModify::anti_poisonres_p(KNpc* pNpc, void* pData)          //=���ԶԷ�����:#d1-%
{
	KMagicAttrib* pMagic = (KMagicAttrib *)pData;
    pNpc->m_Currentpoisonres += pMagic->nValue[0];		
}
void KNpcAttribModify::anti_fireres_p(KNpc* pNpc, void* pData)            //=���ԶԷ����:#d1-%
{
	KMagicAttrib* pMagic = (KMagicAttrib *)pData;
    pNpc->m_Currentfireres += pMagic->nValue[0];		
}

void KNpcAttribModify::anti_lightingres_p(KNpc* pNpc, void* pData)        //=���ԶԷ��׷�:#d1-%
{
	KMagicAttrib* pMagic = (KMagicAttrib *)pData;
    pNpc->m_Currentlightingres += pMagic->nValue[0];		
}

void KNpcAttribModify::anti_physicsres_p(KNpc* pNpc, void* pData)         //=���ԶԷ��շ�:#d1-%
{
	KMagicAttrib* pMagic = (KMagicAttrib *)pData;
    pNpc->m_Currentphysicsres += pMagic->nValue[0];		
}

void KNpcAttribModify::anti_coldres_p(KNpc* pNpc, void* pData)            //=���ԶԷ�����:#d1-%
{
	KMagicAttrib* pMagic = (KMagicAttrib *)pData;
    pNpc->m_Currentcoldres += pMagic->nValue[0];		
}

void KNpcAttribModify::not_add_pkvalue_p(KNpc* pNpc, void* pData)         //=������PKֵ����:#d1+%
{
	KMagicAttrib* pMagic = (KMagicAttrib *)pData;
    pNpc->m_Currentnopkvalue += pMagic->nValue[0];		
}

void KNpcAttribModify::add_boss_damage(KNpc* pNpc, void* pData)           //=�Իƽ�boss�����˺�<color=orange>#d1+%<color>
{
	KMagicAttrib* pMagic = (KMagicAttrib *)pData;
    pNpc->m_Currentbossdamage += pMagic->nValue[0];		
}

void KNpcAttribModify::five_elements_enhance_v(KNpc* pNpc, void* pData)   //=����ǿ��ֵ��#d1-�㡣ǿ����������еĿ���Ч��
{
	KMagicAttrib* pMagic = (KMagicAttrib *)pData;
    pNpc->m_Currentelementsenhance += pMagic->nValue[0];		
}

void KNpcAttribModify::five_elements_resist_v(KNpc* pNpc, void* pData)     //=��������ֵ��#d1-�㡣������������еĿ���Ч��
{
	KMagicAttrib* pMagic = (KMagicAttrib *)pData;
    pNpc->m_Currentelementsresist += pMagic->nValue[0];		
}
//��ǿ�˺�
void KNpcAttribModify::add_alldamage_p(KNpc* pNpc, void* pData)
{	
		
}
//�Զ�����
void KNpcAttribModify::auto_Revive_rate(KNpc* pNpc, void* pData)
{	
	KMagicAttrib* pMagic = (KMagicAttrib *)pData;
	 pNpc->m_CurrentautoReviverate += pMagic->nValue[0];		
}
//���ԶԷ�ȫ������
void KNpcAttribModify::anti_allres_p(KNpc* pNpc, void* pData)
{	
	KMagicAttrib* pMagic = (KMagicAttrib *)pData;
     pNpc->m_Currentallres += pMagic->nValue[0];		
}

//�嶾120 �Զ��ͷŵļ���
void KNpcAttribModify::autoskill( KNpc* pNpc, void* pData )
{
	KMagicAttrib* pMagic = (KMagicAttrib *)pData;
}
