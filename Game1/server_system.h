/*
 *  ファイル名	: system.h
 *  機能	: 共通のデータ構造体の宣言
 */

#ifndef _SYSTEM_H_
#define _SYSTEM_H_

#include <SDL2/SDL.h>


//////////////////////////////////////////

enum {
    MAP_Width    = 50,//全体マップの幅
    MAP_Height   = 50,//全体マップの高さ
    WD_Width     = 17,//ウィンドウの幅
    WD_Height    = 12,//ウィンドウの高さ
    MAP_ChipSize = 48,//ウィンドウ作成する際、使用する1ブロックのサイズ
    MAP_ChipSizeZ = 96,//全体マップを作成する際、使用する1ブロックのサイズ
};

/* マップの種類 */
typedef enum {
    MT_None = 0,/*なし*/
    MT_Road = 1 /* 路 */
} MapType;


//グループ番号
typedef enum {
    CG_Group1=0,/*グループ1*/
    CG_Group2=1,/*グループ2*/
}CharaGroup;


//タイプ///////////////////////////////////////////////////////////

/* キャラクタータイプ */
typedef enum {
    CT_Player = 0, /* プレイヤー */
    CT_2P = 1, /* プレイヤー2 */
    CT_3P  = 2, /* プレイヤー3 */
    CT_4P = 3, /* プレイヤー4 */
    CT_NUM    = 4 /* タイプの数 */
} CharaType;

/* アイテムタイプ */
typedef enum {
    IT_Piko = 0, /* ピコピコハンマー */
    IT_Kasa = 1, /* 傘 */
    IT_Ono  = 2, /* 斧 */
    IT_Scop = 3, /* スコップ */
    IT_Mokuzai  = 4, /* バケツ */
    IT_NUM    = 5 /* タイプの数 */
} ItemType;

/* 拠点タイプ　(11/24追加)*/
typedef enum{
    BS_Secret = 0,/* 拠点 */
    BS_2P = 1,  /* 拠点1 */
    BS_3P = 2,  /* 拠点2 */
    BS_NUM =  3 /* 拠点の数  */
}BaseType;

/* 障害物のタイプ　(12/8追加)*/
typedef enum{
    OT_Ki = 0,  /* 木 */
    OT_NUM =  1 /* 障害物の数  */
}ObjeType;


//状態/////////////////////////////////////////////////////////

/* キャラクターの状態 */
typedef enum {
    CS_Disable = 0, /* 非表示 */
    CS_Enable  = 1, /* 表示 */
    CS_Die     = 2, /* 気絶 */
    CS_Ghost   = 3, /* 幽霊 */
    CS_Item    = 4, /*アイテム所持*/
    CS_Attack1 = 5, /* 攻撃1 */
    CS_Act = 6,     /* 調べる */
    CS_Chuze = 7,    /* 捨てる */
    CS_Fallout = 8,    /* 落ちた */
    CS_Wood = 9,    /* 木を揺らす *///追記(2019.1.13)
} CharaStts;


/* アイテムの状態 */
typedef enum {
    IS_Disable = 0, /* 非表示 */
    IS_Enable  = 1, /* 表示 */
    IS_Mine    = 2, /* 所有物状態 */
    IS_Open    = 3, /* アイテム開放状態(追加変数 2019/1/5)*/
} ItemStts;


/* 拠点の状態(11/24追加) */
typedef enum {
    BS_Disable = 0, /* 非表示 */
    BS_Enable  = 1, /* 表示 */
    BS_FLEE  = 2, /* 空き地 */
    BS_Group1  = 3, /* 空き地 */
    BS_Group2  = 4, /* 空き地 */
    BS_Group1_P  = 5, /* 空き地 */
    BS_Group2_P  = 6, /* 空き地 */
} BaseStts;

/* 障害物の状態(12/8追加) */
typedef enum {
    OS_Disable = 0, /* 非表示 */
    OS_Enable  = 1, /* 表示 */
    OS_Die =2,/*木がなくなったとき*/
    OS_Shake = 3, /* 振る */
} ObjeStts;

/* 落とし穴の状態(12/16追加) */
typedef enum {
    FS_Disable = 0, /* 非表示 */
    FS_Enable  = 1, /* 表示 */
    FS_Catch   = 2, /*はまった*/
} FallStts;


/* ゲームの状態 */
typedef enum {
    GS_End     = 0, /* 終了 */
    GS_Playing = 1  /* 1プレイ中 */
} GameStts;

/* 入力の状態 */
typedef struct {
    SDL_bool stay;
    SDL_bool up;
    SDL_bool down;
    SDL_bool right;
    SDL_bool left;
} InputStts;

/*キャラごとのスタート(2019.1.11)*/
typedef struct {
    SDL_bool start;
} CharaStart;
/*キャラごとのスタート(2019.1.11)*/
typedef struct {
    SDL_bool start;
} GameStart;

/////////////////////////////////////////////////////////

/* 実数座標 */
typedef struct {
    float x;
    float y;
} FloatPoint;

//(追加構造体・・floatの値とintの値を切り枯れられる構造体　2018/11/17)
union IntAndFloat{
    int ival;
    float fval;
};
//(追加構造体・・floatの値とintの値を切り枯れられる構造体(受信用)　2018/11/19)
union newFloat {
    int ival;
    float fval;
};


/////////////////////////////////////////////////////////////////////////////////

/* キャラクターの情報 */
typedef struct {
    CharaType type;   /*キャラクターのタイプ*/
    CharaStts stts;   /*キャラクターの状態*/
    CharaStts pre_stts;/*前のキャラクターの状態*/
    InputStts input;  /*キャラクターの入力の状態*/
    CharaGroup group; /*キャラクターのグループ*/
    int Dir_Flag;     /*キャラクターの向きのフラグ*/
    int Damege_Flag;  /*キャラクターのダメージフラグ*/
    int hp;           /* HP */
    int SteyFlag;     /*待機フラグ   (変数名変更  2018/12.20)*/
    int collusion_F;  /*衝突フラグ   (追加変数  2018/12.20)*/
    int ThrowFlag;    /*アイテムを捨てるフラグ(追加変数  2018/12.20)*/
    int i_num;        /*アイテムの番号(追加変数  2018/12.20)*/
    int f_rireve;     /* 生き返った際のフラグ */
    int ActFlag;      /*調べるフラグ(追加変数  2018/12.20)*/
    int fall_num;         /* 落とし穴の番号(変更部分 2018/12/21) */
    FloatPoint point; /* 計算上の座標(マップ全体でのキャラの座標) */
    FloatPoint base;  /* 基準位置，目的地などの座標 */
    FloatPoint dir;   /* 方向 */
    float step;       /* 移動量 */
    SDL_Rect rect;    /* 位置(マップ全体でのキャラの座標)*/
    SDL_Rect mask;    /* 当たり判定の範囲(rectの相対位置) */
    SDL_Rect src;     /* テクスチャの範囲(アニメーション用) */
    int Mokuzai;      /* 木材の所有数　*/
    int chuzeitem;    /*選択中のアイテム(追加変数 2019/1/5)*/
    int chuzedir;    /*選択中のアイテム(追加変数 2019/1/5)*/
    CharaStart now;        /*スタートの状態*/
    int fwood;
    int f_atta;      /*攻撃切り替え*/
} S_CharaInfo;


/* アイテムの情報 */
typedef struct {
    ItemType type;   /*アイテムのタイプ*/
    ItemStts stts;   /*アイテムの状態*/
    FloatPoint point; /* 計算上の座標 */
    int c_num;        /* clientの番号*/
    int f_away;         /*  捨てたフラグ(変更部分 2018/12/20) */
    int countf;        /* 落とし穴のカウント(2018.12.20) */
    int count_ono;        /* 斧のカウント(2018.12.20) */
    FloatPoint dir;   /* アイテムの移動方向*/
    float step;       /* アイテムの移動量 */
    SDL_Rect rect;    /* 位置(マップ全体でのアイテムの座標)*/
    SDL_Rect mask;    /* 当たり判定の範囲(rectの相対位置) */
    SDL_Rect src;     /* テクスチャの範囲(アニメーション用) */
    SDL_Texture *image;//アイテム表示
} S_ItemInfo;


/* 木材の情報 */
typedef struct {
    ItemStts stts;   /*木材の状態*/
    FloatPoint point; /* 計算上の座標 */
    SDL_Rect rect;    /* 位置(マップ全体でのアイテムの座標) (変更部分 2018/11/10)*/
    SDL_Rect mask;    /* 当たり判定の範囲(rectの相対位置) */
    SDL_Rect src;     /* テクスチャの範囲(アニメーション用) */
} S_MokuzaiInfo;


/* 拠点の情報*/
typedef struct{
    BaseType type; /*拠点のタイプ*/
    BaseStts stts; /*拠点の状態*/
    int hp_A; /*拠点のHP_A*/
    int hp_B; /*拠点のHP_B*/
    int Damege_Flag;  /*拠点のダメージフラグ*/
    FloatPoint point; /*計算上の座標*/
    SDL_Rect map_point; /*マップ全体での拠点の座標*/
    FloatPoint base;  /* 基準位置，目的地などの座標 */
    FloatPoint dir;   /* 方向 */
    SDL_Rect rect;    /* 位置(マップ全体での拠点の座標)*/
    SDL_Rect mask;    /* 当たり判定の範囲(rectの相対位置) */
    SDL_Rect mask2;    /* 当たり判定の範囲(rectの相対位置) */
    SDL_Rect mask3;    /* 当たり判定の範囲(rectの相対位置) */
    SDL_Rect mask4;    /* 当たり判定の範囲(rectの相対位置) */
    SDL_Rect src;     /* テクスチャの範囲(アニメーション用) */
    int UPGreadFlag;
} S_BaseInfo;



/* 障害物の情報 (12/8追加)*/
typedef struct{
    ObjeType type; /*障害物のタイプ*/
    ObjeStts stts; /*障害物の状態*/
    FloatPoint point; /*計算上の座標*/
    SDL_Rect rect;    /* 位置(マップ全体での拠点の座標)*/
    SDL_Rect mask;    /* 当たり判定の範囲(rectの相対位置) */
    SDL_Rect mask2;
    SDL_Rect src;     /* テクスチャの範囲(アニメーション用) */
    int fwood;
} S_Object;

/* 落とし穴の情報 (2018.12.16)*/
typedef struct{
    FallStts stts;    /*障害物の状態*/
    FloatPoint point; /*計算上の座標*/
    int c_num;        /*client番号*/
    SDL_Rect rect;    /* 位置(マップ全体での拠点の座標)*/
    SDL_Rect mask;    /* 当たり判定の範囲(rectの相対位置) */
    SDL_Rect src;     /* テクスチャの範囲(アニメーション用) */
} S_Fall;


/* ゲームの情報 */
typedef struct {
    GameStts stts;          /*ゲームの状態*/
    SDL_Rect rectMap;       /* マップの画面表示領域 */
    SDL_Rect rectBg;        /* 背景の画面表示領域 */
    int anipatNum[CT_NUM];  /* キャラ別アニメーションパターン数 */
    float baseStep[CT_NUM]; /* キャラ別移動量の基準値 */
    float baseStepItem[IT_NUM]; /* アイテム別移動量の基準値 (2018.11.29)*/
    int charaNum;           /*キャラの数*/
    S_CharaInfo *chara;       /*キャラの情報*/
    int ItemNum;           /*アイテムの数*/
    S_ItemInfo *item;       /*アイテムの情報(追加部分 2018/11/19)*/
    int BaseNum;            /*拠点の数*/
    S_MokuzaiInfo *mokuzai;   /*木材の情報*/
    int MokuNum;            /*木材の数*/
    S_BaseInfo *base;       /*拠点の情報*/
    int ObjeNum;            /*障害物の数*/
    S_Object *object;       /*障害物の情報*/
    int FallNum;            /*落とし穴の数*/
    S_Fall *fall;           /*落とし穴の情報*/
    int Group1_point;       /*グループ1のポイント*/
    int Group2_point;       /*グループ2のポイント*/
    int Limit;              /*タイマーの保管   (2018/12.20)*/
    int c_start;            /*スタートするためのカウント(2019.1.7)*/
    GameStart love;         /*スタートするフラグ(2019.1.11)*/
    int numnum;
} S_GameInfo;

/* 変数 */
extern S_GameInfo gTheGame;


#endif
/* end of system.h */
