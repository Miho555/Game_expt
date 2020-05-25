/*****************************************************************
ファイル名	: client_system.c
機能		: クライアントのシステム処理
*****************************************************************/

#include "system.h"
#include"common.h"
#include"client_func.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
static FloatPoint MoveByDir(CharaInfo *ch);
/* ゲームの情報の初期化 */
GameInfo gTheGame = {
    GS_Playing,
    { SDL_FALSE, SDL_FALSE, SDL_FALSE, SDL_FALSE},
    { 0, 0, WD_Width *MAP_ChipSize, WD_Height *MAP_ChipSize },
    { 0, 0, WD_Width *MAP_ChipSize, WD_Height *MAP_ChipSize },
    { 0, 0, 816, 576 },
    { 4, 4, 4, 4},
    { 3, 3, 3, 3},
    { 2, 2, 2, 1 ,1},//ここがアイテムのアニメーション数
    CT_NUM,
    NULL,
    20,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    0,
    3,
    NULL,
    MS_NUM,
    NULL,
    50,
    NULL,
    50,
    NULL,
    20,
    NULL,
    81,
    NULL,
    0,
    0,
    0,
    0,
    0,
    0, //(2019.1.12)
    0, //(2019.1.25)
    0,
    0,
    0
};

/* キャラの情報の初期化 */
CharaInfo gCharaBase = {
    CT_Player,
    CS_Disable,
    1,
    50,
    30,//(2018.12.22)
    0,//(2018/12.21)
    { 0.0, 0.0 },
    { 0, 0, 0, 0 },
    { 0.0, 0.0 },
    { 0.0, 0.0 },
    0,
    { 0, 0, 0, 0 },
    { 0, 0, 0, 0 },
    { 0, 0, 0, 0 },
    { 0, 0, 0, 0 },
    { 0, 0, 0, 0 },
    { 0, 0, 0, 0 },
    { 0, 0, 0, 0 },
    { 0, 0, 0, 0 },
    { 0, 0, 0, 0 },
    { 0, 0, 0, 0 },
    { 0, 0, 0, 0 },
    { 0, 0, 0, 0 },
    { 0, 0, 0, 0 },
    { 0, 0, 0, 0 },
    { 0, 0, 0, 0 },
    { 0, 0, 0, 0 },
    { 0, 0, 0, 0 },
    { 0, 0, 0, 0 },
    { 0, 0, 0, 0 },
    { 0, 0, 0, 0 },
    { 0, 0, 0, 0 },
    { 0, 0, 0, 0 },
    { 0, 0, 0, 0 },
    { 0, 0, 0, 0 },
    { 0, 0, 0, 0 },
    { 0, 0, 0, 0 },
    { 0, 0, 0, 0 },
    { 0, 0, 0, 0 },
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,//(2019.1.25)
    NULL,//(2019.1.25)
    NULL,//(2019.1.25)
    NULL,//(2019.1.25)
    0,//(追加 2018/12.19)
    0,   //(追加 2019.1.7)
    0,
    0,
    0,
    0,
    0,
    0 //(追加 2019/1.25)
};


/* アイテムの情報の初期化 */
ItemInfo gItemBase = {
    IT_Piko,
    CS_Disable,
    { 0.0, 0.0 },
    50,
    { 0, 0, 0, 0 },
    { 0, 0, 0, 0 },
    { 0, 0, 0, 0 },
    NULL,/*アイテムサーフェース(2019/1/11)*/
    NULL
};

/*木材の情報の初期化*/
MokuzaiInfo gMokuzai= {
    IS_Disable,
    { 0.0, 0.0 },
    { 0, 0, 0, 0 },
    { 0, 0, 0, 0 },
    { 0, 0, 0, 0 },
    NULL
};


/* 拠点の情報の初期化 (11/24追加) */
//(変更部分 拠点のHPを2つに追加　2018/11/26)
BaseInfo gBaseBase = {
    BS_Secret,
    BS_Disable,
    1,
    1,
    { 0.0, 0.0 },
    { 0.0, 0.0 },
    { 0.0, 0.0 },
    { 0, 0, 0, 0 },
    { 0, 0, 0, 0 },
    { 0, 0, 0, 0 },
    { 0, 0, 0, 0 },
    { 0, 0, 0, 0 },
    { 0, 0, 0, 0 },
    { 0, 0, 0, 0 },
    { 0, 0, 0, 0 },
    NULL,
    NULL,
    0
};



/* 障害物の情報の初期化 (12/8追加) */
S_Object gBaseObje = {
    OT_Ki,
    OS_Disable,
    { 0.0, 0.0 },
    { 0, 0, 0, 0 },
    { 0, 0, 0, 0 },
    { 0, 0, 0, 0 },
    NULL
};

/* 落とし穴の情報の初期化 (12/8追加) */
S_Fall gPitFall = {
    FS_Disable,
    { 0.0, 0.0 },
    30,
    { 0, 0, 0, 0 },
    { 0, 0, 0, 0 },
    { 0, 0, 0, 0 },
    NULL
};
//メッセージの情報の初期化
Message gMassageBase ={
    MS_WIN,
    MS_Disable,
    { 0, 0, 0, 0 },
    { 0, 0, 0, 0 },
    NULL
};

Sort gSoetBase ={
    0,
    0,
    0
};

/*****************************************************************
関数名	: InitSystem
機能	:ゲームシステムの初期化
引数	: なし
出力	: 正常に設定できたとき0，失敗したとき-1
*****************************************************************/

int InitSystem(void)
{
    int n=0,nb=0;
    //キャラ情報
    gTheGame.chara = (CharaInfo *)malloc(sizeof(CharaInfo) * gTheGame.charaNum);
    if (gTheGame.chara == NULL) {
        printf("failed to allocate memory.");
        return -1;
    }
    for (int i = 0; i < gTheGame.charaNum; i++) {
        gTheGame.chara[i]      = gCharaBase;
        gTheGame.chara[i].type = i;
        gTheGame.chara[i].step = gTheGame.baseStep[gTheGame.chara[i].type];
        gTheGame.chara[i].stts           = CS_Enable;
       
    }
    //アイテム情報
    gTheGame.item = (ItemInfo *)malloc(sizeof(ItemInfo) * gTheGame.ItemNum);
    if (gTheGame.item == NULL) {
        printf("failed to allocate memory.");
        return -1;
    }
    for (int f = 0; f < gTheGame.ItemNum; f++) {
        gTheGame.item[f]      = gItemBase;
        gTheGame.item[f].type = f-n;
        if(f>=(IT_NUM-1)+n){
            n+=5;
        }
    }

    //拠点情報
    gTheGame.base = (BaseInfo *)malloc(sizeof(BaseInfo) * gTheGame.baseNum);
    if (gTheGame.base == NULL) {
        printf("failed to allocate memory.");
        return -1;
    }
    for (int s = 0; s < gTheGame.baseNum; s++) {
        gTheGame.base[s]      = gBaseBase;
        gTheGame.base[s].type = s;
        gTheGame.base[s].stts           = BS_Enable;
    }

    //木材情報
    gTheGame.mokuzai = (MokuzaiInfo *)malloc(sizeof(MokuzaiInfo ) * gTheGame.MokuNum);
    if (gTheGame.mokuzai == NULL) {
        printf("failed to allocate memory.");
        return -1;
    }
    for (int s = 0; s < gTheGame.MokuNum; s++) {
        gTheGame.mokuzai[s]      = gMokuzai;
    }

    //障害物情報
    gTheGame.object = (S_Object *)malloc(sizeof(S_Object) * gTheGame.ObjeNum);
    if (gTheGame.object == NULL) {
        printf("failed to allocate memory.");
        return -1;
    }
    for (int o = 0; o < gTheGame.ObjeNum; o++) {
        gTheGame.object[o]      = gBaseObje;
        gTheGame.object[o].type = o-nb;
        gTheGame.object[o].stts           = OS_Enable;
        if(o>=(OT_NUM)+nb){
            nb+=1;
        }
    }

    //落とし穴情報
    gTheGame.fall = (S_Fall *)malloc(sizeof(S_Fall) * gTheGame.FallNum);
    if (gTheGame.fall == NULL) {
        printf("failed to allocate memory.");
        return -1;
    }
    for (int a = 0; a < gTheGame.FallNum; a++) {
        gTheGame.fall[a]      = gPitFall;
        gTheGame.fall[a].stts           = FS_Disable;
    }

    //メッセージの情報
    gTheGame.message = (Message *)malloc(sizeof(Message) *gTheGame.MsgNum);
    if (gTheGame.message == NULL) {
        printf("failed to allocate memory.");
        return -1;
    }
    for (int m = 0; m < gTheGame.MsgNum; m++) {
        gTheGame.message[m]      = gMassageBase;
        gTheGame.message[m].type = m;
        gTheGame.message[m].rect.x = 300;
        gTheGame.message[m].rect.y = 300;
        if(m==12){
            gTheGame.message[m].stts=MS_Enable;
            gTheGame.message[m].rect.x=0;
            gTheGame.message[m].rect.y=0;
        }
    }


    //ヒープソート用の情報
    gTheGame.Number_Total=gTheGame.charaNum+gTheGame.baseNum+gTheGame.ObjeNum;
    gTheGame.sort= (Sort *)malloc(sizeof(Sort) *gTheGame.Number_Total);
    if (gTheGame.sort == NULL) {
        printf("failed to allocate memory.");
        return -1;
    }
    for (int m = 0; m < gTheGame.Number_Total; m++) {
        gTheGame.sort[m]      = gSoetBase;
    }
    return 0;
}
    

/*****************************************************************
関数名	: DestroySystem
機能	:システムの終了処理
引数	: なし
出力	: 正常に設定できたとき0，失敗したとき-1
*****************************************************************/
void DestroySystem(void)
{
    free(gTheGame.chara);
    return;
}
 

/*****************************************************************
関数名	:MeasureMask
機能	:画像のマスクから当たり判定用矩形の算出
引数  : s: マスクのサーフェイス
引数2	: r: 検出範囲
出力	: 算出した矩形
*****************************************************************/
SDL_Rect MeasureMask(SDL_Surface *s, SDL_Rect r)
{
    SDL_Rect ret = { 0 };
    Uint32 *p    = (Uint32 *)(s->pixels) + s->w * r.y + r.x;
    /* 左上を探す */
    for (ret.y = 0; ret.y < r.h; ret.y++, p += s->w) {
        for (ret.x = 0; ret.x < r.w; ret.x++) {
            if (p[ret.x] & s->format->Amask) {
                break;
            }
        }
        if (p[ret.y] & s->format->Amask) {
            break;
        }
    }
    /* 幅を測る */
    for (ret.w = 0; ret.x + ret.w < r.w; ret.w++) {
        if (!(p[ret.x + ret.w] & s->format->Amask))
            break;
    }
    /* 高さを測る */
    for (ret.h = 0; ret.y + ret.h < r.h; ret.h++, p += s->w) {
        if (!(p[ret.x] & s->format->Amask))
            break;
    }
    //fprintf(stderr,"OKmask\n");
    return ret;
}

/*****************************************************************
関数名	: MoveChara
機能	:キャラクターを移動させる処理
引数	: ch    キャラクター
出力	: 正常に設定できたとき0，失敗したとき-1
*****************************************************************/
void MoveChara(CharaInfo *ch){
    if (ch->stts == CS_Die ||ch->stts == CS_Wood ||ch->stts == CS_Fallout) {
        return;
    }
    ch->f_die=0;
    ch->wood=0;
    FloatPoint newpoint = ch->point;
    //newpoint=MoveByDir(ch);
    ch->point  = newpoint;
    if(ch->stts == CS_Ghost){
        ch->rect3.x = newpoint.x;
        ch->rect3.y = newpoint.y;
    }else{
        ch->rect.x = newpoint.x;
        ch->rect.y = newpoint.y;
    }

      //y座標のスクロール
    if(gTheGame.chara[gTheGame.c_ID].stts == CS_Ghost){
        if( gTheGame.chara[gTheGame.c_ID].rect3.y >WD_Height*MAP_ChipSize/2
         && gTheGame.chara[gTheGame.c_ID].rect3.y < MAP_Height*MAP_ChipSizeZ-(WD_Height*MAP_ChipSize/2) ){
            gTheGame.rectMap.y = gTheGame.chara[gTheGame.c_ID].rect3.y - ( WD_Height*MAP_ChipSize/2);
            gTheGame.rectBg.y = gTheGame.chara[gTheGame.c_ID].rect3.y - ( WD_Height*MAP_ChipSize/2);
        }
        //x座標のスクロール
        if( gTheGame.chara[gTheGame.c_ID].rect3.x > WD_Width*MAP_ChipSize/2 
         && gTheGame.chara[gTheGame.c_ID].rect3.x < MAP_Width*MAP_ChipSizeZ-(WD_Width*MAP_ChipSize/2)){
            gTheGame.rectMap.x = gTheGame.chara[gTheGame.c_ID].rect3.x - ( WD_Width*MAP_ChipSize/2 );
            gTheGame.rectBg.x = gTheGame.chara[gTheGame.c_ID].rect3.x - ( WD_Width*MAP_ChipSize/2 );
         }
    
    }
    else if(gTheGame.chara[gTheGame.c_ID].stts != CS_Die){//y座標のスクロール
        if( gTheGame.chara[gTheGame.c_ID].rect.y >WD_Height*MAP_ChipSize/2
        && gTheGame.chara[gTheGame.c_ID].rect.y <MAP_Height*MAP_ChipSizeZ-(WD_Height*MAP_ChipSize/2)){
            gTheGame.rectMap.y = gTheGame.chara[gTheGame.c_ID].rect.y - ( WD_Height*MAP_ChipSize/2);
            gTheGame.rectBg.y = gTheGame.chara[gTheGame.c_ID].rect.y - ( WD_Height*MAP_ChipSize/2);
        }
        //x座標のスクロール
        if( gTheGame.chara[gTheGame.c_ID].rect.x >  WD_Width*MAP_ChipSize/2 
        && gTheGame.chara[gTheGame.c_ID].rect.x <MAP_Width*MAP_ChipSizeZ-(WD_Width*MAP_ChipSize/2) ){
            gTheGame.rectMap.x = gTheGame.chara[gTheGame.c_ID].rect.x - ( WD_Width*MAP_ChipSize/2 );
            gTheGame.rectBg.x = gTheGame.chara[gTheGame.c_ID].rect.x - ( WD_Width*MAP_ChipSize/2 );
         }
    
    } 
}
//(追加部分・・・アイテムの位置を更新させる関数を追加  2018/11/10)
/*****************************************************************
関数名	: MoveItem
機能	:アイテムを移動させる処理
引数	: ch    アイテム
出力	: 正常に設定できたとき0，失敗したとき-1
*****************************************************************/
void MoveItem(ItemInfo *ch){
    if (ch->stts == IS_Disable) {
        return;
    }
    FloatPoint newpoint = ch->point;
    ch->point  = newpoint;
    ch->rect.x = newpoint.x;
    ch->rect.y = newpoint.y;

}

/*****************************************************************
関数名	: MoveBase
機能	:拠点を移動させる処理
引数	: ch    拠点
出力	: 正常に設定できたとき0，失敗したとき-1
*****************************************************************/
void MoveBase(BaseInfo *ch){
    FloatPoint newpoint = ch->point;
    ch->point  = newpoint;
    ch->rect.x = newpoint.x;
    ch->rect.y = newpoint.y;
}

/*****************************************************************
関数名	: MoveObje
機能	:拠点を移動させる処理
引数	: ch    拠点
出力	: 正常に設定できたとき0，失敗したとき-1
*****************************************************************/
void MoveObje(S_Object *ch){
    FloatPoint newpoint = ch->point;
    ch->point  = newpoint;
    ch->rect.x = newpoint.x;
    ch->rect.y = newpoint.y;
}

//落とし穴を移動させる処理
void MoveFall(S_Fall *fa){
    FloatPoint newpoint = fa->point;
    fa->point  = newpoint;
    fa->rect.x = newpoint.x;
    fa->rect.y = newpoint.y;
}

//木材を移動させる処理
void MoveMokuzai(MokuzaiInfo *mo){
    FloatPoint newpoint = mo->point;
    mo->point  = newpoint;
    mo->rect.x = newpoint.x;
    mo->rect.y = newpoint.y;
}



/*****************************************************************
関数名	:UpdateCharaStatus
機能	:キャラクターの状態を更新させる処理
引数	: ch    キャラクター
出力	: 正常に設定できたとき0，失敗したとき-1
*****************************************************************/
/* キャラの状態更新 (アニメーション変化はここでする)*/
void UpdateCharaStatus(CharaInfo *ch)
{
    if (ch->stts == CS_Disable) {
        if(ch->point.y-gTheGame.rectMap.y<(WD_Height*MAP_ChipSize) && (ch->point.y-gTheGame.rectMap.y)>0){
            if(ch->point.x-gTheGame.rectMap.x<(WD_Width*MAP_ChipSize) && (ch->point.x-gTheGame.rectMap.x)>0 ){
                     ch->stts    =CS_Enable;
            }
        }
    }
}
/*****************************************************************
関数名	: MoveByDir
機能	: dir方向にstep移動した時の座標を実数計算。
引数	: ch    キャラクター
出力	: なし
*****************************************************************/
static FloatPoint MoveByDir(CharaInfo *ch)
{
    FloatPoint ret = ch->point;
    if (!ch->dir.x && !ch->dir.y)
        return ret;
    float theta = atan2f(ch->dir.y, ch->dir.x);
    ret.x += ch->step * cosf(theta);
    ret.y += ch->step * sinf(theta);
    return ret;
}


/*****************************************************************
関数名	: Radian
機能	:キャラクターを移動させる処理
引数	: ch    キャラクター
出力	: 正常に設定できたとき0，失敗したとき-1
*****************************************************************/
void Radian(CharaInfo *ch,int joyx,int joyy){
    float rad;
    int pos;

    pos = ch->type;
    
    if(ch->start_joy.x == 0){
        ch->start_joy.x = joyx;
        ch->start_joy.y = joyy;
    }
    if((110 < joyx) && (joyx < 144) && (115 < joyy) && (joyy < 150)){
        rad = 0.0000;
    }else{//2018.11.19 ラジアン求める
        rad = atan2l(joyx - ch->start_joy.x,joyy - ch->start_joy.y);
        if(rad <= 0.0f) {
            rad = rad + 2.0 * M_PI;
        }
    } 
    Send_MOVE_Command(pos,rad);
}