#ifndef KMagicAttribH
#define KMagicAttribH

#define	INVALID_ATTRIB	-1
// ö���������һ����ĸ��ʾ����ֵ�����ͣ�vΪ���ͣ�ֱ�Ӽӵĵ�����pΪ�ٷֱȣ�bΪ������
enum MAGIC_ATTRIB
{
//�ı似�ܵ�����
	magic_skill_begin,
	magic_skill_cost_v,								// ����MANA
	magic_skill_costtype_v,							//	
	magic_skill_mintimepercast_v,					// ÿ�η�ħ���ļ��ʱ��
	magic_skill_misslenum_v,						// �ӵ�����
	magic_skill_misslesform_v,		
	magic_skill_param1_v,		
	magic_skill_param2_v,
	magic_skill_attackradius,                        //magic_skill_reserve1, ������Χ����
	magic_skill_mintimepercastonhorse_v,             //magic_skill_reserve2,//�����䶳ʱ��
	magic_skill_skillexp_v,                          //magic_skill_reserve3,����������
	magic_skill_desc,                                // ��������
	magic_skill_eventskilllevel,
	magic_skill_end,
	//�ı䷢���ӵ�������		magic_skill_mintimepercastonhorse_v,
	magic_missle_begin,
	magic_missle_movekind_v,						// �ӵ���ʽ
	magic_missle_speed_v,							// �ӵ������ٶ�
	magic_missle_lifetime_v,						// �ӵ�������ʱ��
	magic_missle_height_v,							// �ӵ��ĸ߶�
	magic_missle_damagerange_v,						// �ӵ��ƻ���Χ
	magic_missle_radius_v,							// �ӵ����а뾶
	magic_missle_missrate,                          // �ӵ����еĸ���
	magic_missle_hitcount,                          // �ӵ����е�����
	//magic_missle_reserve1,
	//magic_missle_reserve2,
	magic_missile_drag,//magic_missle_reserve3,
	magic_missle_reserve4,
	magic_missle_reserve5,
	magic_missle_end,
//װ������	
	magic_item_begin,
	magic_weapondamagemin_v,
	magic_weapondamagemax_v,
	magic_armordefense_v,
	magic_durability_v,
	magic_requirestr,
	magic_requiredex,
	magic_requirevit,
	magic_requireeng,
	magic_requirelevel,
	magic_requireseries,
	magic_requiresex,	//�� ��Ů 
	magic_requiremenpai,
	magic_weapondamageenhance_p,
	magic_armordefenseenhance_p,
	magic_requirementreduce_p,
	magic_indestructible_b,  //����ĥ��
	magic_item_nouser,		 //����ʹ�õ�����
	magic_item_needskill,	 //��Ҫѧ��ʲô���ܲ���ʹ��
	magic_item_needreborn,	 //��Ҫת������
	magic_item_needtongban,	 //��Ҫ��ͬ��ſ���װ��
	magic_item_needbangzhu,	 //��Ҫ����
	magic_item_needcity,	 //��Ҫ����
	magic_item_noseries,	 //ĳ���в����õ�
	magic_item_reserve8,
	magic_item_reserve9,
	magic_item_reserve10,
	magic_item_end,
//���������˺�����---��ǰ��������--���⹥����
	magic_damage_begin,		
	magic_attackrating_v,   //���е�
	magic_attackrating_p,   //�����ʰٷֱ�
	magic_ignoredefense_p,  //���Ե���������
	magic_physicsdamage_v,  //���������յ�
	magic_colddamage_v,     //�������ܱ���
	magic_firedamage_v,     //�������ܻ�
	magic_lightingdamage_v, //����������
	magic_poisondamage_v,   //�������ܶ�
	magic_magicdamage_v,    //
	magic_physicsenhance_p, //�յ��ǿ�ٷֱ�
	magic_steallife_p,      //��������
	magic_stealmana_p,
	magic_stealstamina_p,
	magic_knockback_p,          //���ӹ������˶��ּ���:#d1-%
	magic_deadlystrike_p,       //��������һ����
	magic_fatallystrike_p,      //����һ��
	magic_stun_p,               //=ѣ��ʱ���ǿ
    //magic_coldenhance_p,        //���ӳٻ�ʱ��ٷֱ�
	//magic_fireenhance_p,        //��ʱ���ǿ�ٷֱ�
	//magic_lightingenhance_p,    //����ʱ���ǿ��ǿ
	//magic_poisonenhance_p,      //�������ʱ��
	//magic_magicenhance_p,
	magic_xinphysicsdamage_v,   //=�����⹦ϵ�չ��˺�:#d1+��
	magic_xinfiredamage_v,      //=�����⹦ϵ���˺�:#d1+��
	magic_xincolddamage_v,      //=�����⹦ϵ�����˺�:#d1+��
	magic_xinlightingdamage_v,  //=�����⹦ϵ�׹��˺�:#d1+��
	magic_xinpoisondamage_v,    //=�����⹦ϵ�����˺�:#d1+��/��
	magic_xinphysicsdamage_p,   //=�����⹦ϵ�չ��˺�(%):#d1+%
    magic_xincoldmagic_v,       //=�����ڹ�ϵ�����˺�:#d1+��
//	magic_damage_reserve1,
//	magic_damage_reserve2,
//	magic_damage_reserve3,
//	magic_damage_reserve4,
//	magic_damage_reserve5,
//	magic_damage_reserve6,
//	magic_damage_reserve7,
	magic_addzhuabu_v,      //ץ������
//	magic_damage_reserve9,
	magic_autoattackskill,  //�Զ�����
	magic_seriesdamage_p,   //������������˺�
//	magic_magicenhance_p,   /////
	magic_damage_end,
//������״̬���⻷������--����״̬-��ǰֵ
	magic_normal_begin,
	magic_lifemax_v,         //���ֵ��
	magic_lifemax_p,         //�������ֵ�ٷֱ�
	magic_life_v,            //��ǰ����
	magic_lifereplenish_v,   //�����ָ� ��
	magic_manamax_v,
	magic_manamax_p,
	magic_mana_v,            //��ǰ����
	magic_manareplenish_v,   //�����ָ� ��
	magic_staminamax_v,
	magic_staminamax_p,
	magic_stamina_v,
	magic_staminareplenish_v,
	magic_strength_v,
	magic_dexterity_v,
	magic_vitality_v,	        //�����⹦���Ӷ��ٵ�
	magic_energy_v,
	magic_poisonres_p,          //���� �ٷֱ�
	magic_fireres_p,            //��
	magic_lightingres_p,        //�׷�
	magic_physicsres_p,         //�շ�
	magic_coldres_p,            //����  
	magic_freezetimereduce_p,   //���ٱ���ʱ��ٷֱ�
	magic_burntimereduce_p,     //ȼ��ʱ�� ��
	magic_poisontimereduce_p,   //�ж�ʱ��ٷֱ�
	magic_poisondamagereduce_v, //�ж��˺�����
	magic_stuntimereduce_p,     //����ѣ��ʱ��ٷֱ�
	magic_fastwalkrun_p,        //�ƶ��ٶ�
	magic_visionradius_p,
	magic_fasthitrecover_v,     //�Լ����˶���ʱ�����
	magic_allres_p,             //ȫ��  ��
	magic_attackspeed_v,        //�⹦����
	magic_castspeed_v,          //�ڹ�����
	magic_meleedamagereturn_v,
	magic_meleedamagereturn_p,  //���ٽ����˺������ٷֱ�
	magic_rangedamagereturn_v,
	magic_rangedamagereturn_p,
	magic_addphysicsdamage_v,     // ״̬�⹦�յ�
	magic_addfiredamage_v,        // ״̬���
	magic_addcolddamage_v,        // ״̬���
	magic_addlightingdamage_v,    // ״̬����
	magic_addpoisondamage_v,      // ״̬�ⶾ
	magic_addphysicsdamage_p,     // ״̬���հٷֱ� --��������
	magic_slowmissle_b,           // �����ӵ������ٶ�
	magic_changecamp_b,           // �Ի����  �ı���Ӫ
	magic_physicsarmor_v,         // ������
	magic_coldarmor_v,
	magic_firearmor_v,
	magic_poisonarmor_v,
	magic_lightingarmor_v,
	magic_damagetomana_p,          //��ת��
	magic_lucky_v,		           //����
	magic_steallifeenhance_p,      //��Ѫ
	magic_stealmanaenhance_p,      //����
	magic_stealstaminaenhance_p,   //����
	magic_allskill_v,              //�Ӽ��� �ȼ�
	magic_metalskill_v,            //�ӽ�ϵ����
	magic_woodskill_v,             //��ľϵ����
	magic_waterskill_v,            //��ˮϵ����
	magic_fireskill_v,             //�ӻ�ϵ����
	magic_earthskill_v,            //����ϵ����
	magic_knockbackenhance_p,      //���е������˶Է��ļ���:#d1-%
	magic_deadlystrikeenhance_p,   //��������һ����
	magic_stunenhance_p,           //���е������ζԷ��ļ���:#d1+%
	magic_badstatustimereduce_v,   //����״̬ʱ������
	magic_manashield_p,            //���������˺�
	magic_adddefense_v,
	magic_adddefense_p,
	magic_fatallystrikeenhance_p,  //��������һ����:#d1+%
	magic_lifepotion_v,		       //ҩƷ�ĺ�Ѫ״̬
	magic_manapotion_v,	           //ҩƷ�ĺ���״̬ 
	magic_physicsresmax_p,         //�շ����ֵ                                                                                                                    
	magic_coldresmax_p,
	magic_fireresmax_p,
	magic_lightingresmax_p,
	magic_poisonresmax_p,
	magic_allresmax_p,		    //���п������ֵ
	magic_coldenhance_p,        //���ӶԷ��ٻ�ʱ��ٷֱ�
	magic_fireenhance_p,        //��ʱ���ǿ�ٷֱ�
	magic_lightingenhance_p,    //����ʱ���ǿ��ǿ
	magic_poisonenhance_p,      //�������ʱ��
	magic_magicenhance_p,       
	magic_attackratingenhance_v, //�����ʼ�ǿ��
	magic_attackratingenhance_p, //���������Ӽ��ٰٷֱ�
	magic_addphysicsmagic_v,	 //�����ڹ��̵�
	magic_addcoldmagic_v,        //״̬�ڱ�
	magic_addfiremagic_v,        //״̬�ڻ�
	magic_addlightingmagic_v,    //״̬����
	magic_addpoisonmagic_v,      //״̬�ڶ�
	magic_fatallystrikeres_p,	 // ����һ���ֿ���
	magic_addskilldamage1,		 // �ԡ�#l1-�����ܼӳ�#d3-%
	magic_addskilldamage2,       // �ԡ�#l2-�����ܼӳ�#d3-%
	magic_expenhance_p,          // ɱ�����˻�þ��� �� 
	magic_addskilldamage3,
	magic_addskilldamage4,
	magic_addskilldamage5,
	magic_addskilldamage6,
	magic_dynamicmagicshield_v,		        // �����˺����� 
	magic_addstealfeatureskill,		        // αװ����ʣ��ʹ�ô���
	magic_lifereplenish_p,			        // �����ظ�Ч������ �ٷֱ�
	magic_ignoreskill_p,					// ���ӽ��Ϳ�������ĸ���
	magic_poisondamagereturn_v,				// �ܵ����˺�ʱ������
	magic_poisondamagereturn_p,				// �ܵ����˺�ʱ����%
	magic_returnskill_p,					// �������ܵ�����ĸ���
	magic_autoreplyskill,					// ����ʱ��<color=orange>#d9-%<color>�ĸ����Զ�ʩ��
//	magic_skill_mintimepercast_v,	        // �ͷż��ʱ��
//	magic_autoattackskill,					// �Զ��ͷż���
    magic_mintimepercastonhorse_v,	// �����ͷż��ʱ��
	magic_poison2decmana_p,			// ʹĿ���ܵ����˺���ͬʱ�� 
	magic_skill_appendskill,		// ��������<color=water>#l1-<color>����ߵȼ�<color=orange>#d3-��<color>
	magic_hide,						// ����<color=orange>#f2-��<color>���ͷ������书������
	magic_clearnegativestate,       // ��<color=orange>#d1-%<color>�ĸ��������Χһ����Χ���ѷ���ҵ��쳣״̬
	magic_returnres_p,              // �Է����˺��Ŀ���
	magic_dec_percasttimehorse,     // ����#l3-��ȴʱ��:����<color=orange>#f7-��<color>������<color=orange>#f4-��<color>
	magic_dec_percasttime,          // ����#l3-��ȴʱ��:<color=orange>#f1-��<color>
	magic_enhance_autoSkill,        // ������ʨ�Ӻ𡹸���<color=orange>#d1+%<color>          //�Զ�����ĳ���ܵĸ���
	magic_enhance_life_p,           // �������졹�����ظ�Ч������<color=orange>#d1+%<color>  //��ĳ���ܵ������ظ�Ч�ʼӳɰٷֱ�
	magic_enhance_life_v,           // ���Ⱥ��նɡ������ָ�����<color=orange>#d1+<color>��   //��ĳ���ܵ������ظ�Ч�ʼӳɵ�
	magic_enhance_711_auto,         // �����������ʡ�����<color=orange>#d1+%<color>
	magic_enhance_714_auto,         // ��������������������<color=orange>#d1+%<color>
	magic_enhance_717_auto,         // ��������������������<color=orange>#d1+%<color>
	magic_enhance_723_miss_p,       // ��ħ�����ǡ��־���˼���<color=orange>#d1+%<color>
	magic_no,                       // δ����
	magic_skill_collideevent,       // ��ײ�ӵ�ʱ
	magic_skill_vanishedevent,      // �ӵ�����ʱ
	magic_skill_startevent,         // �ӵ���ʼʱ
	magic_skill_flyevent,		    // �ӵ�����ʱ
	magic_block_rate,                          //�������
    magic_enhancehit_rate,                     //�ػ�����
	magic_anti_block_rate,                     //=���񵲸��ʣ�#d1+%
	magic_anti_enhancehit_rate,                //=�ֵ��ػ����ʣ�#d1+%
	magic_sorbdamage_p,                        //=�����˺�:#d1+
	magic_anti_poisonres_p,                    //=���ԶԷ�����:#d1-%
	magic_anti_fireres_p,                      //=���ԶԷ����:#d1-%
	magic_anti_lightingres_p,                  //=���ԶԷ��׷�:#d1-%
	magic_anti_physicsres_p,                   //=���ԶԷ��շ�:#d1-%
	magic_anti_coldres_p,                      //=���ԶԷ�����:#d1-%
	magic_not_add_pkvalue_p,                   //=������PKֵ����:#d1+%
	magic_add_boss_damage,                     //=�Իƽ�boss�����˺�<color=orange>#d1+%<color>
	magic_five_elements_enhance_v,             //=����ǿ��ֵ��#d1-�㡣ǿ����������еĿ���Ч��
    magic_five_elements_resist_v,              //=��������ֵ��#d1-�㡣������������еĿ���Ч��
	magic_skill_enhance,                       //=�Թ������ܼӳɣ�#d1+%
	magic_anti_allres_p,                       //���ԶԷ����п���:#d1+%
	magic_add_alldamage_p,                     //��ǿ�˺�:#d1+%
	magic_auto_Revive_rate,                    //�Զ�ԭ�ظ���
	magic_addphysicsmagic_p,	               //�ڹ��̰ٷֱ�
	magic_addcreatnpc_v,	                   //ץ��BB
	magic_reduceskillcd1,                      //=���ټ���#l1-��ȴʱ��#f3-��  ��ĳ���ܵ���ȴʱ�䣩
	magic_reduceskillcd2,                      //=���ټ���#l1-��ȴʱ��#f3-��
	magic_reduceskillcd3,                      //=���ټ���#l1-��ȴʱ��#f3-��
	magic_clearallcd,                          //#d1,+%�ļ�������Լ����м�����ȴʱ��
	magic_addblockrate,                        //=��Χÿ#d1+����#d3+%����
	magic_walkrunshadow,                       //=�貨΢��
	magic_returnskill2enemy,                   //=�����˹���ʱ��#d1+%����ʹ���˱�����#l3-����  
	magic_manatoskill_enhance,                 //=������ʱ�Թ������ܼӳɣ�#d1+%
	magic_add_alldamage_v,                     //�����˺���
	magic_addskilldamage7,                     //=���ӡ�#l1�������˺�#d3-%
	magic_ignoreattacrating_v,                 //���ԶԷ�������
	magic_alljihuo_v,                          //����ȫ��
	magic_addexp_v,                            //��������� ���Ӿ���
    magic_doscript_v,                          //ִ�нű�״̬
	//magic_addskillexp_v,                     //���Ӽ���������
    magic_me2metaldamage_p,                    //=�Խ�ϵ�˺����ӣ�#d1+%
    magic_metal2medamage_p,                    //=�������Խ�ϵ���˺���#d1-%
    magic_me2wooddamage_p,                     //=��ľϵ�˺����ӣ�#d1+%
    magic_wood2medamage_p,                     //=��������ľϵ���˺���#d1-%
    magic_me2waterdamage_p,                    //=��ˮϵ�˺����ӣ�#d1+%
    magic_water2medamage_p,                    //=��������ˮϵ���˺���#d1-%
    magic_me2firedamage_p,                     //=�Ի�ϵ�˺����ӣ�#d1+%
    magic_fire2medamage_p,                     //=�������Ի�ϵ���˺���#d1-%
    magic_me2earthdamage_p,                    //=����ϵ�˺����ӣ�#d1+%
    magic_earth2medamage_p,                    //=����������ϵ���˺���#d1-%
	magic_manareplenish_p,                     //�����ָ��ٷֱ�
	magic_fasthitrecover_p,                    //�Լ����˶���ʱ�����Ӽ��ٰٷֱ�
	magic_stuntrank_p,                         //ѣ�μ��ʼ��ٰٷֱ�
	magic_sorbdamage_v,                        //�����˺��ٷֱ�
	magic_creatstatus_v,
	magic_randmove,	                            //���ܳ��� �Լ����Ѳ��
	magic_addbaopoisondmax_p,	                //���ӱ�������
	magic_dupotion_v,	                        //ҩƷ�ļ���״̬ 
	magic_npcallattackSpeed_v,                  //ͬ�鹥������
	magic_eqaddskill_v,                         //װ���˾�����ʲô����
	magic_autodeathskill,
	magic_autorescueskill,                      //��������
	magic_staticmagicshield_p,					//����120������ħ����
	magic_ignorenegativestate_p,				//
	magic_poisonres_yan_p,                      //=������#d1+%������
    magic_fireres_yan_p,                        //=�����#d1+%������
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
	magic_anti_hitrecover,		                //=��ɶԷ����˶���ʱ�䣺#d1+
	magic_do_hurt_p,                            //=������˸��ʣ�#d1+%
	magic_skill_showevent,                      //�����¼�
	magic_addskillexp1,
	magic_anti_poisontimereduce_p,              //����ж�ʱ������  �Ե��� 
	magic_anti_stuntimereduce_p,                //���ѣ��ʱ������  �Ե���
	magic_addskilldamage8,
	magic_addskilldamage9,
	magic_addskilldamage10,
	magic_normal_end,
};

/*
poisonres_yan_p=������#d1+%������
fireres_yan_p=�����#d1+%������
lightingres_yan_p=�׷���#d1+%������
physicsres_yan_p=�շ���#d1+%������
coldres_yan_p=������#d1+%������
lifemax_yan_v=�������ֵ��#d1+�㣨����
lifemax_yan_p=�������ֵ��#d1+%������
manamax_yan_v=�������ֵ��#d1+�㣨����
manamax_yan_p=�������ֵ��#d1+%������
sorbdamage_yan_p=�����˺���#d1+������
fastwalkrun_yan_p=�ƶ��ٶȣ�#d1+%������
attackspeed_yan_v=�⹦ϵ�书���ж�����#d1+%������
castspeed_yan_v=�ڹ�ϵ�书���ж�����#d1+%������
allres_yan_p=���п��ԣ�#d1+%������
fasthitrecover_yan_v=���˶���ʱ�䣺#d1~������
anti_physicsres_yan_p=���ԶԷ��շ���#d1-%������
anti_poisonres_yan_p=���ԶԷ�������#d1-%������
anti_coldres_yan_p=���ԶԷ�������#d1-%������
anti_fireres_yan_p=���ԶԷ������#d1-%������
anti_lightingres_yan_p=���ԶԷ��׷���#d1-%������
anti_allres_yan_p=���ԶԷ����п��ԣ�#d1-%������
anti_sorbdamage_yan_p=�����˺���#d1+��������






me2metaldamage_p=�Խ�ϵ�˺����ӣ�#d1+%
metal2medamage_p=�������Խ�ϵ���˺���#d1-%
me2wooddamage_p=��ľϵ�˺����ӣ�#d1+%
wood2medamage_p=��������ľϵ���˺���#d1-%
me2waterdamage_p=��ˮϵ�˺����ӣ�#d1+%
water2medamage_p=��������ˮϵ���˺���#d1-%
me2firedamage_p=�Ի�ϵ�˺����ӣ�#d1+%
fire2medamage_p=�������Ի�ϵ���˺���#d1-%
me2earthdamage_p=����ϵ�˺����ӣ�#d1+%
earth2medamage_p=����������ϵ���˺���#d1-%
*/
extern const char MAGIC_ATTRIB_STRING[magic_normal_end + 1][64] ; 


#endif
