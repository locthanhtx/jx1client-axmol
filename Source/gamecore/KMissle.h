//////////////////////////////////////////////////////////////////////
// KMissle.h: interface for the KMissle class.
//////////////////////////////////////////////////////////////////////
#ifndef KMissleH
#define KMissleH
#define MAX_MISSLE 30000  // 10000子弹限制
#include "KCore.h"
#include "SkillDef.h"
#include "KObj.h"

#include "KMissleRes.h"

#include "engine/KNode.h"
#include "engine/KITabFile.h"
#include "KSkills.h"
#include "KNpcSet.h"
#include "gamescene/ObstacleDef.h"
#include "KSubWorld.h"
#include "KIndexNode.h"
#include "GameDataDef.h"

enum
{
    Interupt_None,
    Interupt_EndNewMissleWhenMove,
    Interupt_EndOldMissleLifeWhenMove,
};

struct TMissleForShow
{
    int nPX;
    int nPY;
    int nPZ;
    int nNpcIndex;
    int nLauncherIndex;
};

class KMissle
{
    friend class KSkill;

private:
public:
    //	子弹设定文件获得的数据
    KIndexNode m_Node;
    int m_nEnChance;                  //  子弹技能加成
    int m_nShangBei;                  //  伤害的倍率
                                      //	char				m_szMissleName[32];		//	子弹的名称
    int m_nAction;                    //	当前行为
    int m_bIsSlow;                    //	是否被减速
    int m_bClientSend;                //	是否需要
    int m_bRemoving;                  //	获知需要下一个循环删除该子弹所有资源
    int m_bIsMelee;                   //	子弹是否是近身攻击
    eMissleMoveKind m_eMoveKind;      //	子弹运动类型(爆炸、直线飞行等……)
    eMissleFollowKind m_eFollowKind;  //	子碟发出时的参照类型
    int m_nHeight;                    //	子弹高度
    int m_nHeightSpeed;               //	子弹纵行的飞行速度
    int m_nLifeTime;                  //	生命周期
    int m_nSpeed;                     //	飞行速度
    int m_nRate;                      //  子弹击中的概率
    int m_nHitCount;                  //  子弹击中的人数

    int m_nSkillId;      //	对应哪个技能
    int m_bRangeDamage;  //	是否为区域伤害，即是否多人受到伤害
    int m_eRelation;     //	目标与发射者的关系
    int m_bAutoExplode;  //  消亡前是否再次碰撞
    int m_bTargetSelf;
    int m_bBaseSkill;  //	是否为最基本技能（直接使用子技能）
    int m_bByMissle;   //	当由父技能产生时，是否是根据玩家为基点还是以当前的子弹为基点
    INT m_nInteruptTypeWhenMove;  // 子弹的激活是否受发送者的移动而中止
    int m_bHeelAtParent;  //	当子弹实际激活时，位置根据父当前位置而确定,而不是由产生那刻parent位置决定
    int m_nLauncherSrcPX;
    int m_nLauncherSrcPY;
    int m_nCollideRange;   //	碰撞范围（简化多边形碰撞用）
    int m_nDamageRange;    //	伤害范围
    int m_bCollideVanish;  //	碰撞后是否消亡
    int m_bCollideFriend;  //	是否会碰撞到同伴
    int m_bCanSlow;        //	是否会被减速（比如说Slow Missle类的技能）
    int m_nKnockBack;      //	震退距离
    // int				m_nStunTime;			//	晕眩时间

    int m_bFlyEvent;  //	整个飞行过程中的
    int m_nFlyEventTime;
    int m_bSubEvent;       //	是否需要在飞行过程消息发生是，调用相关回调函数
    int m_bStartEvent;     //	是否需要在技能第一次Active时，调用相关回调函数
    int m_bCollideEvent;   //	是否需要在子技能魔法碰撞时，调用相关回调函数
    int m_bVanishedEvent;  //	是否需要在子技能消亡时，调用相关的回调函数

    unsigned long m_ulDamageInterval;  //	伤害计算的间隔时间,主要指对类型火墙技能
    int m_bMustBeHit;  //  必中，即子弹无特殊情况一定能打中对方，无论是否实际碰撞到对方.
    //	技能获得的数据
    int m_nCurrentLife;          //	当前生命时间
    int m_nStartLifeTime;        //	当技能发生后，第几帧开始
    int m_nCollideOrVanishTime;  //  子弹消亡的时间
    int m_nCurrentMapX;          //	当前的X坐标
    int m_nCurrentMapY;          //	当前的Y坐标
    int m_nCurrentMapZ;          //	当前的Z坐标
    int m_nXOffset;              //	当前的X方向偏移
    int m_nYOffset;              //	当前的Y方向偏移
    int m_nRefPX;
    int m_nRefPY;

    int m_nDesMapX;  // 单颗子单时，目的坐标
    int m_nDesMapY;
    int m_nDesRegion;
    int m_bNeedReclaim;  // 是否已纠正过一次子单的方向问题
    // TMisslePos			m_NeedReclaimPos[4];

    int m_bDoHurt;   // 受伤时是否需要作受伤动作(概率)
    int m_nIsMagic;  // 内外功

    // 单一飞行子单时，精确命中！
    int m_nXFactor;
    int m_nYFactor;
    int m_nLevel;  //	技能等级

    int m_nFollowNpcIdx;  //	跟随谁
    // int                 m_nLockNpcIndex;        //  锁定的那个NPC的索引
    // unsigned long				m_dwFollowNpcID;		//

    int m_nLauncher;               //	发射者在NpcSet中的Index
    unsigned long m_dwLauncherId;  //	发射者的唯一ID
    int m_nParentMissleIndex;      // if 0 then means parent is npclauncher

    //	自生成的动态数据
    int m_nCurrentSpeed;            //	当前速度（可能被减速）
    int m_nZAcceleration;           //	Z轴的加速度
    eMissleStatus m_eMissleStatus;  //	子弹当前的状态
    int m_nMissleId;                //	子弹在Missle中的ID
    int m_nSubWorldId;              //	子世界ID
    int m_nRegionId;                //	区域ID
    // 各项伤害数据
    // int					m_nMaxDamage;			//	最大伤害
    // int					m_nElementType;			//	元素伤害类型
    // int					m_nMaxElementDamage;	//	最大元素伤害(首次)
    // int					m_nElementTime;			//	元素持续时间
    // int					m_nElementInterval;		//	元素间隔时间
    // int					m_nElementPerDamage;	//	元素伤害时，每次发作所受的伤害值
    int m_nParam1;  //	参数一
    int m_nParam2;  //	参数二
    int m_nParam3;  //	参数三

    int m_nFirstReclaimTime;
    int m_nEndReclaimTime;

    int m_nTempParam1;  //	运行期使用的参数
    int m_nTempParam2;

    int m_nDirIndex;             //	当前运动方向的索引
    int m_nDir;                  //	当前的运行方向
    int m_nAngle;                //	飞行方向
    unsigned long m_dwBornTime;  //	该子弹产生时的时间
    int m_bUseAttackRating;

    char m_szMissleName[32];     //	子弹的名称
    int m_bMultiShow;            //	子弹有两个显示
    int m_bFollowNpcWhenCollid;  //	爆炸效果跟随被击中的人物
    int m_btRedLum;
    int m_btGreenLum;
    int m_btBlueLum;
    unsigned short m_usLightRadius;
    KMissleRes m_MissleRes;  //	子弹的资源
    unsigned int m_SceneID;

private:
    int Init(int nLauncher, int nMissleId, int nXFactor, int nYFactor, int nLevel);
    void OnVanish();     // 即将消失
    void OnCollision();  // 碰撞
    void OnFly();        // 飞行过程中
    void OnWait();
    void DoWait();
    void DoFly();
    int PrePareFly();
    void DoVanish();
    void DoCollision();
    int CheckNearestCollision();
    friend class KMissleSet;
    void Release();
    int FsCheckCollision();  // 检测是否碰撞// 1表示正常碰撞到物体，0表示未碰撞到任何物体, -1表示落地
    int CheckBeyondRegion(int nDOffsetX, int nDOffsetY);  // 检测是否越界   //FALSE表示越到一个无效的位置，TRUE表示OK
    // int				GetDir(int dx,int dy);
    KMissle& operator=(KMissle& Missle);
    unsigned long GetCurrentSubWorldTime();
    int ProcessDamage(int nNpcId, int nRata = 0);
    int ProcessCollision();  // 处理碰撞
    int
    ProcessCollision(int nLauncherIdx, int nRegionId, int nMapX, int nMapY, int nRange, int eRelation, int mNpcIdx = 0);

    inline void ZAxisMove()
    {
        if (m_nZAcceleration)
        {
            m_nHeight += m_nHeightSpeed;

            if (m_nHeight < 0)
                m_nHeight = 0;

            m_nHeightSpeed -= m_nZAcceleration;

            m_nCurrentMapZ = m_nHeight >> 10;
        }
    }
    // TRUE表示遇到障碍，FALSE表示未遇到，一切正常
    inline int TestBarrier()
    {
        // int nBarrierKind = SubWorld[m_nSubWorldId].TestBarrier(m_nRegionId, m_nCurrentMapX, m_nCurrentMapY,
        // m_nXOffset, m_nYOffset, 0, 0); 精确查找
        int nBarrierKind = SubWorld[m_nSubWorldId].TestBarrierMin(m_nRegionId, m_nCurrentMapX, m_nCurrentMapY,
                                                                  m_nXOffset, m_nYOffset, 0, 0);
        if (nBarrierKind == Obstacle_Normal /* || nBarrierKind == Obstacle_Jump*/)
        {
            return TRUE;
        }
        return FALSE;
    }

public:
    KMissle();
    virtual ~KMissle();
    int GetInfoFromTabFile(int nMissleId);
    int GetInfoFromTabFile(KTabFile* pTabFile, int nMissleId);
    void GetMpsPos(int* pPosX, int* pPosY);
    int Activate();
    int Remove();
    static int GetOffsetAxis(int nSubWorld,
                             int nSrcRegionId,
                             int nSrcMapX,
                             int nSrcMapY,
                             int nOffsetMapX,
                             int nOffsetMapY,
                             int& nDesRegionId,
                             int& nDesMapX,
                             int& nDesMapY);

    void Paint();
    void GetLightInfo(KLightInfo* pLightInfo);
    int CreateSpecialEffect(eMissleStatus eStatus, int nPX, int nPY, int nPZ, int nNpcIndex = 0);
    static int CreateMissleForShow(char* szMovie, char* szSprInfo, char* szSound, TMissleForShow* pShowParam);
};
extern KMissle* Missle;                        // Missle[MAX_MISSLE];
extern KMissle g_MisslesLib[MAX_MISSLESTYLE];  // Base 1  *g_MisslesLib;//
#endif
