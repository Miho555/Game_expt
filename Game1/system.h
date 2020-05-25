/*
 *  ファイル名	: system.h
 *  機能	: 共通のデータ構造体の宣言
 */

#ifndef _SYSTEM_H_
#define _SYSTEM_H_

#include <SDL2/SDL.h>

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
    MT_None = 0,
    MT_Road = 1 /* 路 */
} MapType;



/////////////////////////////////////////////////
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
    IT_Kasa = 1, /* 水鉄砲 */
    IT_Ono  = 2, /* 斧 */
    IT_Scop = 3, /* スコップ */
    IT_Mokuzai  = 4, /* バケツ */
    IT_NUM    = 5 /* タイプの数 */
} ItemType;

/* 拠点タイプ　(11/24追加)*/
typedef enum {
    BS_Secret = 0,  /* 拠点 */
    BS_2P = 1,  /* 拠点1 */
    BS_3P = 2,  /* 拠点2 */
    BS_NUM =  3 /* 拠点の数　 */
} BaseType;

/* 障害物のタイプ　(12/8追加)*/
typedef enum{
    OT_Ki = 0,  /* 木 */
    OT_NUM =  1 /* 障害物の数  */
}ObjeType;

/* メッセージタイプ　(11/24追加)*/
typedef enum {
    MS_WIN = 0,  /* 勝ち */
    MS_LOSE = 1,  /* 負け */
    MS_DRA = 2,  /* 引き分け */
    MS_CharaHead2 = 3,  /*拠点に必要な素材の数 */
    MS_kazu = 4,  /*拠点に必要な素材の数 */
    MS_Kn = 5, /* ポイント */
    MS_CharaHead = 6, /* キャラクターの頭上に表示 */
    MS_Fuchi= 7,/*アイテム選択の縁(追加変数 2019/1/5)*/
    MS_Select=8,/*アイテムの選択肢(追加変数 2019/1/5)*/
    MS_Heart =9, /* ハート(HP)の表示 */
    MS_Heart2 =10, /* 減少しているハート(HP)の表示 */
    MS_Pinchi=11,
    MS_Kanban=12,
    MS_Ki=13,
    MS_END=14,
    MS_Load=15,
    MS_Mini1 = 16, /* ミニマップでのグループ1の拠点 */
    MS_Mini2 = 17, /* ミニマップでのグループ2の拠点 */
    MS_Mini3 = 18, /* ミニマップでのキャラクターの表示 */
    MS_Mini4 = 19, /* ミニマップでのキャラクターの表示 */
    MS_Mini5 = 20, /* ミニマップでのおばけ状態の表示 */
    MS_Sun = 21, /* タイマー表示での太陽 */
    MS_Sun2 = 22, /* タイマー表示での半円 */
    MS_NUM =23,
    
} MessageType;
////////////////////////////////////////////////


/* アイテムの状態 */
typedef enum {
    IS_Disable = 0, /* 非表示 */
    IS_Enable  = 1, /* 表示 */
    IS_Mine    = 2, /* 所有物状態 */
    IS_Open    = 3,/*アイテム開放状態(追加変数 2019/1/5)*/
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
    OS_Shake = 3, /* 表示 */
} ObjeStts;

/* 落とし穴の状態(12/16追加) */
typedef enum {
    FS_Disable = 0, /* 非表示 */
    FS_Enable  = 1, /* 表示 */
    FS_Catch   = 2, /*はまった*/
} FallStts;

/* メッセージの状態 */
typedef enum {
    MS_Disable = 0, /* 非表示 */
    MS_Enable  = 1, /* 表示 */
} MessegeStts;

/* キャラクターの状態 */
typedef enum {
    CS_Disable = 0, /* 非表示 */
    CS_Enable  = 1, /* 表示 */
    CS_Die     = 2, /* 気絶 */
    CS_Ghost   = 3, /* 幽霊 */
    CS_Item    = 4, /*アイテム所持*/
    CS_Attack1 = 5, /* 攻撃1 */
    CS_Act = 6,     /* 調べる */
    CS_Chuze = 7,   /* 捨てる */
    CS_Fallout =8,
    CS_Wood =9,   /*木を揺らす*///追記(2019.1.13)
} CharaStts;

/* ゲームの状態 */
typedef enum {
    GS_End     = 0, /* 終了 */
    GS_Playing = 1  /* 1プレイ中 */
} GameStts;

/* 入力の状態 */
typedef struct {
    SDL_bool up;
    SDL_bool down;
    SDL_bool right;
    SDL_bool left;
    SDL_bool start;
    SDL_bool wait;
    SDL_bool ready;
} InputStts;

////////////////////////////////////////////////
/* 実数座標 */
typedef struct {
    float x;
    float y;
} FloatPoint;

//(追加構造体・・・xy座標を格納する構造体　2018/11/10)
/* 座標（map用） */
typedef struct {
    int x;
    int y;
} IntPoint;


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

/////////////////////////////////////////////////////////////////////////

/* キャラクターの情報 */
//(追加部分・・・マップ全体でのキャラの座標を保管する変数の宣言 2018/11/10)
typedef struct {
    CharaType type;   /*キャラクターのタイプ*/
    CharaStts stts;   /*キャラクターの状態*/
    int hp;           /* HP */
    int i_num;         /*アイテムの番号 */
    int fall_num;     /*落ちた穴の番号 (2018.12.22)*/
    int SteyFlag;     /*待機フラグ   (変数名変更  2018/12.20)*/
    FloatPoint point; /* 計算上の座標 */
    SDL_Rect start_joy;/* 初めのjoyを格納 */
    FloatPoint base;  /* 基準位置，目的地などの座標 */
    FloatPoint dir;   /* 方向 */
    float step;       /* 移動量 */
    SDL_Rect rect;    /* 位置(マップ全体でのキャラの座標) (変更部分 2018/11/10)*/
    SDL_Rect rect2;    /* 位置(マップ全体でのキャラの座標)2 (変更部分 2018/12/2)*/
    SDL_Rect rect3;    /* 位置(マップ全体での幽霊の座標)2 (変更部分 2018/12/8)*/
    SDL_Rect rect4;    /* 位置(落とし穴) (変更部分 2018/12/23)*/
    SDL_Rect rect5;    /* 位置(木を揺らす) (変更部分 2019/1/12)*/
    SDL_Rect rect6;    /* アイテムもつ (変更部分 2019/1/17)*/
    SDL_Rect rect7;    /* アイテムもつ (変更部分 2019/1/17)*/
    SDL_Rect rect8;    /* 斧もつ (変更部分 2019/1/25)*/
    SDL_Rect rect9;    /* 斧攻撃 (変更部分 2019/1/25)*/
    SDL_Rect rect10;    /* 斧もつ (変更部分 2019/1/25)*/
    SDL_Rect rect11;    /* 斧攻撃 (変更部分 2019/1/25)*/
    SDL_Rect rect12;    /* スコップ持つ (変更部分 2019/1/25)*/
    SDL_Rect rect13;    /* 勝敗(変更部分 2019/1/25)*/
    SDL_Rect mask;    /* 当たり判定の範囲(rectの相対位置) */
    SDL_Rect mask2;    /* 当たり判定の範囲(rectの相対位置)2 */
    SDL_Rect src;     /* テクスチャの範囲(アニメーション用) */
    SDL_Rect src2;     /* テクスチャの範囲(アニメーション用死んだ時) */
    SDL_Rect src3;     /* テクスチャの範囲(アニメーション用死んだ時)(変更部分 2018.12.8) */
    SDL_Rect src4;     /* テクスチャの範囲(落とし穴)(変更部分 2018.12.23) */
    SDL_Rect src5;     /* テクスチャの範囲(木を揺らす)(変更部分 2019.1.12) */
    SDL_Rect src6;     /*ピコピコハンマーを持つ*/
    SDL_Rect src7;     /*ピコピコハンマーで攻撃*/
    SDL_Rect src8;     /*斧を持つ(2019.1.25)*/
    SDL_Rect src9;     /*斧で攻撃(2019.1.25)*/
    SDL_Rect src10;     /*傘を持つ(2019.1.25)*/
    SDL_Rect src11;     /*傘で攻撃(2019.1.25)*/
    SDL_Rect src12;     /*スコップを持つ(2019.1.25)*/
    SDL_Rect src13;     /*勝敗(2019.1.25)*/
    SDL_Surface *s;    //キャラクターサーフェース(2019/1/11)
    SDL_Texture *image;//キャラクター表示
    SDL_Texture *image2;//キャラクター表示2
    SDL_Texture *image3;//幽霊表示
    SDL_Texture *image4;//落とし穴表示
    SDL_Texture *image5;//木を揺らす表示
    SDL_Texture *image6;//ピコを持つ
    SDL_Texture *image7;//ピコで攻撃
    SDL_Texture *image8;//斧を持つ(2019.1.25)
    SDL_Texture *image9;//斧で攻撃(2019.1.25)
    SDL_Texture *image10;//傘を持つ(2019.1.25)
    SDL_Texture *image11;//傘で攻撃(2019.1.25)
    SDL_Texture *image12;//傘で攻撃(2019.1.25)
    SDL_Texture *image13;//傘で攻撃(2019.1.25)
    int Mokuzai;    //木材の数  (追加 2018/12.19)
    int f_start;        //スタートするためのフラグ(2019.1.7)
    int HPFlag;       /* 拠点と接したときに拠点のHPを表示するフラグ*/
    int basecollision;  /* キャラクターと接している拠点の番号 */
    int dir_flag;
    int wood;
    int f_die;
    int f_atta;
} CharaInfo;


/* アイテムの情報 */
typedef struct {
    ItemType type;   /*アイテムのタイプ*/
    ItemStts stts;   /*アイテムの状態*/
    FloatPoint point; /* 計算上の座標 */
    int c_num;        /* clientの番号(2018.11.24) */
    SDL_Rect rect;    /* 位置(マップ全体でのアイテムの座標) (変更部分 2018/11/10)*/
    SDL_Rect mask;    /* 当たり判定の範囲(rectの相対位置) */
    SDL_Rect src;     /* テクスチャの範囲(アニメーション用) */
    SDL_Surface *sd;   /*アイテムサーフェース(2019/1/11)*/
    SDL_Texture *image;//アイテム表示
} ItemInfo;

/* 木材の情報 */
typedef struct {
    ItemStts stts;   /*木材の状態*/
    FloatPoint point; /* 計算上の座標 */
    SDL_Rect rect;    /* 位置(マップ全体でのアイテムの座標) (変更部分 2018/11/10)*/
    SDL_Rect mask;    /* 当たり判定の範囲(rectの相対位置) */
    SDL_Rect src;     /* テクスチャの範囲(アニメーション用) */
    SDL_Texture *image;//アイテム表示
} MokuzaiInfo;


/* 拠点の情報 */
typedef struct{
    BaseType type;      /*拠点のタイプ*/
    BaseStts stts;      /*拠点の状態*/
    int hp_A;           /*拠点のHP_A(追加部分 2018/11/26)*/
    int hp_B;           /*拠点のHP_B(追加部分 2018/11/26)*/
    FloatPoint point; /*計算上の座標*/
    FloatPoint base;  /* 基準位置，目的地などの座標 */
    FloatPoint dir;   /* 方向 */
    SDL_Rect rect;    /* 位置(マップ全体での拠点の座標) (変更部分 2018/11/10)*/
    SDL_Rect rect2;    /* 位置(マップ全体での拠点の座標) (変更部分 2018/11/10)*/
    SDL_Rect mask;    /* 当たり判定の範囲(rectの相対位置) */
    SDL_Rect mask2;    /* 当たり判定の範囲(rectの相対位置) */
    SDL_Rect mask3;    /* 当たり判定の範囲(rectの相対位置) */
    SDL_Rect mask4;    /* 当たり判定の範囲(rectの相対位置) */
    SDL_Rect src;     /* テクスチャの範囲(アニメーション用) */
    SDL_Rect src2;     /* テクスチャの範囲(アニメーション用) */
    SDL_Texture *image;//拠点の表示
    SDL_Texture *image2;//拠点の表示
    int upgradeFlag; /*　拠点がアップグレードされたときのHP増加フラグ　(1/26)変更*/
} BaseInfo;


/* 障害物の情報 (12/8追加)*/
typedef struct{
    ObjeType type;    /*障害物のタイプ*/
    ObjeStts stts;    /*障害物の状態*/
    FloatPoint point; /*計算上の座標*/
    SDL_Rect rect;    /* 位置(マップ全体での拠点の座標)*/
    SDL_Rect mask;    /* 当たり判定の範囲(rectの相対位置) */
    SDL_Rect src;     /* テクスチャの範囲(アニメーション用) */
    SDL_Texture *image;//拠点の表示
} S_Object;

/* 落とし穴の情報 (2018.12.16)*/
typedef struct{
    FallStts stts;    /*障害物の状態*/
    FloatPoint point; /*計算上の座標*/
    int c_num;        /*client番号*/
    SDL_Rect rect;    /* 位置(マップ全体での拠点の座標)*/
    SDL_Rect mask;    /* 当たり判定の範囲(rectの相対位置) */
    SDL_Rect src;     /* テクスチャの範囲(アニメーション用) */
    SDL_Texture *image;//拠点の表示
} S_Fall;

/* メッセージの情報 (11/24追加)*/
typedef struct{
    MessageType type;  /*メッセージのタイプ*/
    MessegeStts stts;  /*メッセージの状態*/
    SDL_Rect rect;    /* 位置(マップ全体での拠点の座標) (変更部分 2018/11/10)*/
    SDL_Rect src;     /* テクスチャの範囲(アニメーション用) */
    SDL_Texture *image;//メッセージの画像
}Message;

/*画像の描画順番を変更するための情報*/
typedef struct{
    int rect;
    int type;
    int number;
}Sort;


/* ゲームの情報 */
typedef struct {
    GameStts stts;          /*ゲームの状態*/
    InputStts input;        /*入力の状態*/
    SDL_Rect rectMap;       /* マップの画面表示領域 */
    SDL_Rect rectBg;        /* 背景の画面表示領域 */
    SDL_Rect rectBg2;        /* 背景の画面表示領域 */
    int anipatNum[CT_NUM];  /* キャラ別アニメーションパターン数 */
    float baseStep[CT_NUM]; /* キャラ別移動量の基準値 */
    int anipatNum_Item[IT_NUM];  /* アイテム別アニメーションパターン数 (2019/1/11)*/
    int charaNum;           /*キャラの数*/
    CharaInfo *chara;       /*キャラの情報*/
    int ItemNum;           /*アイテムの数*/
    ItemInfo *item;       /*アイテムの情報(追加部分 2018/11/19)*/
    SDL_Window *window;    /*ウィンドウの情報*/    
    SDL_Renderer *render; /*レンダラー*/
    SDL_Texture *bg;      /*背景のテクスチャ*/
    SDL_Texture *map;      /*マップのテクスチャ*/
    SDL_Texture *title;    /*タイトル(2019.1.7)*/
    SDL_Texture *waitwin;    /*ロード(2019.1.11)*/
    SDL_Surface *mapMask;   /*キャラクター表示*/
    int c_ID;
    int baseNum;            /*拠点の数*/
    BaseInfo *base;         /*拠点の情報*/
    int MsgNum;             /*メッセージの数*/
    Message *message;       /*メッセージの情報*/
    int MokuNum;            /*木材の数*/
    MokuzaiInfo *mokuzai;   /*木材の情報*/
    int ObjeNum;            /*障害物の数*/
    S_Object *object;       /*障害物の情報*/
    int FallNum;            /*落とし穴の数*/
    S_Fall *fall;           /*落とし穴の情報*/
    int Number_Total;       /*すべての数*/
    Sort *sort;             /*画像の描画順番を変更するための情報*/
    int Limit;              /*残り時間*/
    int Group1_point;       /*グループ1のポイント*/
    int Group2_point;       /*グループ2のポイント*/
    int chuze;              /*アイテムの選択肢(追加変数 2019/1/5)*/
    int chuzeFlag;
    int aflag;
    int hideFlag;
    SDL_Texture *msgTexts[8];
    int rizultFLag;
    int EndFlag;
} GameInfo;

/* 変数 */
extern GameInfo gTheGame;


#endif
/* end of system.h */
