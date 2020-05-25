/*****************************************************************
ファイル名	: server_system.c
機能		: サーバーのシステム処理
*****************************************************************/

#include "server_system.h"
#include"server_common.h"
#include"server_func.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/* ゲームの情報の初期化 */
S_GameInfo gTheGame = {
    GS_Playing,
    { 0, 0, WD_Width *MAP_ChipSize, WD_Height *MAP_ChipSize },
    { 0, 0, WD_Width *MAP_ChipSize, WD_Height *MAP_ChipSize },
    { 2, 2, 2, 2},
    { 5, 5, 5, 5},
    { 2, 2, 2, 2, 2},
    CT_NUM,
    NULL,
    20,
    NULL,
    3,
    NULL,
    50,
    NULL,
    50,
    NULL,
    20,
    NULL,
    0,
    0,
    0,  //(2018/12.20)
    0, //(2019/1.12)
    { SDL_FALSE },   //(2019/1.12)
    0
};

/* キャラの情報の初期化 */
S_CharaInfo gCharaBaseS = {
    CT_Player,
    CS_Disable,
    CS_Disable,
    { SDL_FALSE, SDL_FALSE, SDL_FALSE, SDL_FALSE},
    CG_Group1,
    1,
    0,
    0,
    0,
    0,//(2018/12.20)
    0,//(2018/12.20)
    50,//(2018/12.20)
    0,
    0,
    30,//(2018.12.22)
    { 0.0, 0.0 },
    { 0.0, 0.0 },
    { 0.0, 0.0 },
    0,
    { 0, 0, 0, 0 },
    { 0, 0, 0, 0 },
    { 0, 0, 0, 0 },
    0,
    0,
    0,
    { SDL_FALSE },
    0,
    0
};

/* アイテムの情報の初期化 */
S_ItemInfo gItemBaseS = {
    IT_Piko,
    CS_Disable,
    { 0.0, 0.0 },
    50,
    5,
    0,
    3,
    { 0.0, 0.0 },
    0,
    { 0, 0, 0, 0 },
    { 0, 0, 0, 0 },
    { 0, 0, 0, 0 },
    NULL
};

/* 木材の情報の初期化*/
S_MokuzaiInfo gMokuzai= {
    IS_Disable,
    { 0.0, 0.0 },
    { 0, 0, 0, 0 },
    { 0, 0, 0, 0 },
    { 0, 0, 0, 0 }
};

/* 拠点の情報の初期化*/
S_BaseInfo gBaseBaseS = {
    BS_Secret,
    BS_Disable,
    1,
    1,
    0,
    { 0.0, 0.0 },
    { 0.0, 0.0 },
    { 0.0, 0.0 },
    { 0.0, 0.0 },
    { 0, 0, 0, 0 },
    { 0, 0, 0, 0 },
    { 0, 0, 0, 0 }, 
    { 0, 0, 0, 0 },
    { 0, 0, 0, 0 },
    { 0, 0, 0, 0 },
    0
};

/* 障害物の情報の初期化*/
S_Object gBaseObjeS = {
    OT_Ki,
    OS_Disable,
    { 0.0, 0.0 },
    { 0, 0, 0, 0 },
    { 0, 0, 0, 0 },
    { 0, 0, 0, 0 },
    { 0, 0, 0, 0 },
    0
};

/* 落とし穴の情報の初期化*/
S_Fall gPitFall = {
    FS_Disable,
    { 0.0, 0.0 },
    30,
    { 0, 0, 0, 0 },
    { 0, 0, 0, 0 },
    { 0, 0, 0, 0 }
};




static FloatPoint MoveByDir(S_CharaInfo *ch);
static FloatPoint MoveByDirItem(S_ItemInfo *ch);

int countfall;  //落とし穴用(2018.12.22)
/*****************************************************************
関数名	: InitSystemS
機能	:ゲームシステムの初期化
引数	: なし
出力	: 正常に設定できたとき0，失敗したとき-1
*****************************************************************/

int InitSystemS(void)
{
    int n=0;
    /* 乱数初期化 */
    srand(time(NULL));

    //キャラ情報
    gTheGame.chara = (S_CharaInfo *)malloc(sizeof(S_CharaInfo) * gTheGame.charaNum);
    if (gTheGame.chara == NULL) {
        printf("failed to allocate memory.");
        return -1;
    }
    for (int i = 0; i < gTheGame.charaNum; i++) {
        gTheGame.chara[i]      = gCharaBaseS;
        gTheGame.chara[i].type = i;
        gTheGame.chara[i].step = gTheGame.baseStep[gTheGame.chara[i].type];
        switch (gTheGame.chara[i].type) {
            case CT_Player:
                gTheGame.chara[i].hp =3;
                gTheGame.chara[i].group =0;
                gTheGame.chara[i].chuzeitem=0; 
                break;
            case CT_2P:
                gTheGame.chara[i].hp  = 3;
                gTheGame.chara[i].group =1;
                gTheGame.chara[i].chuzeitem=5;
                break;
            case CT_3P:
                gTheGame.chara[i].hp    = 3;
                gTheGame.chara[i].group = 0;
                gTheGame.chara[i].chuzeitem=10;
                break;
            case CT_4P:
                gTheGame.chara[i].hp    = 3;
                gTheGame.chara[i].group = 1;
                gTheGame.chara[i].chuzeitem=15;
                break;
            default:
                break;
        }
    }
    //アイテム情報
    gTheGame.item = (S_ItemInfo *)malloc(sizeof(S_ItemInfo) * gTheGame.ItemNum);
    if (gTheGame.item == NULL) {
        printf("failed to allocate memory.");
        return -1;
    }
    for (int f = 0; f < gTheGame.ItemNum; f++) {
        gTheGame.item[f]      = gItemBaseS;
        gTheGame.item[f].type = f-n;
        gTheGame.item[f].step = gTheGame.baseStepItem[gTheGame.item[f].type];
        if(f>=(IT_NUM-1)+n){
            n+=5;
        }
    }
    //木材情報
    gTheGame.mokuzai = (S_MokuzaiInfo *)malloc(sizeof(S_MokuzaiInfo) * gTheGame.MokuNum);
    if (gTheGame.mokuzai == NULL) {
        printf("failed to allocate memory.");
        return -1;
    }
    for (int m= 0; m< gTheGame.MokuNum; m++) {
        gTheGame.mokuzai[m]= gMokuzai;
        gTheGame.mokuzai[m].mask.h=40;
        gTheGame.mokuzai[m].mask.w=40;
        gTheGame.mokuzai[m].mask.x=0;
        gTheGame.mokuzai[m].mask.y=35;
    }
    //拠点情報
    gTheGame.base = (S_BaseInfo *)malloc(sizeof(S_BaseInfo) * gTheGame.BaseNum);
    if (gTheGame.base == NULL) {
        printf("failed to allocate memory.");
        return -1;
    }
     for (int s = 0; s < gTheGame.BaseNum; s++) {
        gTheGame.base[s]      = gBaseBaseS;
        gTheGame.base[s].type = s;
        switch (gTheGame.base[s].type) {
            case BS_Secret:
                gTheGame.base[s].hp_A =   15;
                gTheGame.base[s].hp_B =   15;
                break;
            case BS_2P:
                gTheGame.base[s].hp_A =   15;
                gTheGame.base[s].hp_B =   15;
                break;
            case BS_3P:
                gTheGame.base[s].hp_A =   15;
                gTheGame.base[s].hp_B =   15;
                break;
            default:
                break;
        }
    }

    //障害物の情報
    gTheGame.object = (S_Object *)malloc(sizeof(S_Object) * gTheGame.ObjeNum);
    if (gTheGame.object == NULL) {
        printf("failed to allocate memory.");
        return -1;
    }
    for (int o = 0; o < gTheGame.ObjeNum; o++) {
        gTheGame.object[o]      = gBaseObjeS;
        gTheGame.object[o].type =0;
        //障害物のマスク（木）
        //めんどくさかったから、これだけサーバーでマスク決めた。
        gTheGame.object[o].mask.h=45;
        gTheGame.object[o].mask.w=109;
        gTheGame.object[o].mask.x=68;
        gTheGame.object[o].mask.y=169;   
        gTheGame.object[o].mask2.h=250;
        gTheGame.object[o].mask2.w=250;
        gTheGame.object[o].mask2.x=20;
        gTheGame.object[o].mask2.y=20;   
    }

    //落とし穴の情報
    gTheGame.fall = (S_Fall *)malloc(sizeof(S_Fall) * gTheGame.FallNum);
    if (gTheGame.fall == NULL) {
        printf("failed to allocate memory.");
        return -1;
    }
    for (int a = 0; a < gTheGame.FallNum; a++) {
        gTheGame.fall[a]      = gPitFall;
    }
    return 0;
}



/*****************************************************************
関数名	: Set_Object
機能	:障害物、秘密基地の配置
引数	: なし
出力	: なし
*****************************************************************/
void Set_Object(void)
{
    //オブジェクトの配置
    for(int o = 0; o < gTheGame.ObjeNum; o++){
			UpdateObjectStatus(&(gTheGame.object[o]));
	}
    //オブジェクト同士のぶつかり回避
	for( int o = 0; o < gTheGame.ObjeNum; o++ ){
		for( int b = o+1; b < gTheGame.ObjeNum; b++ ){
			int result = CollisionOAO( &(gTheGame.object[o]), &(gTheGame.object[b]) );
			if( result == 1 ){
				gTheGame.object[b].stts = OS_Die; 
			}
		}
	}
    //秘密基地の配置
	for(int s = 0; s < gTheGame.BaseNum; s++){
			UpdateBaseStatus(&(gTheGame.base[s]),s);
	}
    //秘密基地同士を離れた位置に置くよう調整
	for( int s = 0; s < gTheGame.BaseNum; s++){
			for( int t = 0; t < gTheGame.ObjeNum; t++ ){
				int result = CollisionBaseObject( &(gTheGame.base[s]), &(gTheGame.object[t]) );
				if( result == 1 ){
					gTheGame.object[t].stts = OS_Die; 
				}
				if( abs( sqrt( (gTheGame.base[0].point.x - gTheGame.base[1].point.x)*(gTheGame.base[0].point.x - gTheGame.base[1].point.x)
				  + (gTheGame.base[0].point.y - gTheGame.base[1].point.y)*(gTheGame.base[0].point.y - gTheGame.base[1].point.y) ) ) <= 1500
				  || abs( sqrt( (gTheGame.base[1].point.x - gTheGame.base[2].point.x)*(gTheGame.base[1].point.x - gTheGame.base[2].point.x)
				  + (gTheGame.base[1].point.y - gTheGame.base[2].point.y)*(gTheGame.base[1].point.y - gTheGame.base[2].point.y) ) ) <= 1500 
				  || abs( sqrt( (gTheGame.base[0].point.x - gTheGame.base[2].point.x)*(gTheGame.base[0].point.x - gTheGame.base[2].point.x)
				  + (gTheGame.base[0].point.y - gTheGame.base[2].point.y)*(gTheGame.base[0].point.y - gTheGame.base[2].point.y) ) ) <= 1500 )
				{
					//fprintf(stderr,"kakunin\n");
					gTheGame.base[s].point.x=rand()%(MAP_Width*MAP_ChipSizeZ-300);
       				gTheGame.base[s].point.y=rand()%(MAP_Height*MAP_ChipSizeZ-300);
				}
			}
	    }
}


/********************************************************************************
座標処理
********************************************************************************/



/*****************************************************************
関数名	: DestroySystem
機能	:システムの終了処理
引数	: なし
出力	: 正常に設定できたとき0，失敗したとき-1
*****************************************************************/
void DestroySystemS(void)
{
    free(gTheGame.chara);
    return;
}


/*****************************************************************
関数名	: MoveByDir
機能	: dir方向にstep移動した時の座標を実数計算。
引数	: ch    キャラクター
出力	: なし
*****************************************************************/
static FloatPoint MoveByDir(S_CharaInfo *ch)
{
    FloatPoint ret = ch->point;
    if (!ch->dir.x && !ch->dir.y)
        return ret;
    float theta = atan2f(ch->dir.y, ch->dir.x);
    
    //(変更部分 2018/12/20)///////
    if(ch->collusion_F==1){
        ret.x += (ch->step * cosf(theta))/2;
        ret.y += (ch->step * sinf(theta))/2;
    }
    else{
        ret.x += ch->step * cosf(theta);
        ret.y += ch->step * sinf(theta);
    }
    //ここまで//////////////////
    return ret;
}



/*****************************************************************
関数名	: MoveByDirItem
機能	: dir方向にstep移動した時のアイテム座標を実数計算。
引数	: ch    キャラクター
出力	: なし
*****************************************************************/
static FloatPoint MoveByDirItem(S_ItemInfo *ch)
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
関数名	: MoveChara
機能	:キャラクターを移動させる処理
引数	: ch    キャラクター
出力	: 正常に設定できたとき0，失敗したとき-1
*****************************************************************/
void MoveCharaS(S_CharaInfo *ch){
    if (ch->stts == CS_Die ||ch->stts == CS_Fallout || ch->stts == CS_Wood) {
        return;
    }
    FloatPoint newpoint = ch->point;
    newpoint=MoveByDir(ch);
    ch->point  = newpoint;
    ch->rect.x = newpoint.x;
    ch->rect.y = newpoint.y;
    if( ch->rect.x < 0 ){
        ch->rect.x = 0;
        ch->point.x = 0;
    }else if( ch->rect.x > MAP_Width*MAP_ChipSizeZ - 100){
        ch->rect.x = MAP_Width*MAP_ChipSizeZ - 100;
        ch->point.x = MAP_Width*MAP_ChipSizeZ - 100;
    }

    if( ch->rect.y < 0 ){
        ch->rect.y = 0;
        ch->point.y = 0;
    }else if( ch->rect.y > MAP_Height*MAP_ChipSizeZ - 100){
        ch->rect.y = MAP_Height*MAP_ChipSizeZ - 100;
        ch->point.y = MAP_Height*MAP_ChipSizeZ - 100;
    }

}


/*****************************************************************
関数名	: MoveItem
機能	:アイテムを移動させる処理
引数	: ch    アイテム
出力	: 正常に設定できたとき0，失敗したとき-1
*****************************************************************/
void MoveItem(S_ItemInfo *ch){
     if (ch->stts == IS_Disable) {
        return;
    }
    if (ch->stts == IS_Mine) { 
        //アイテムでの攻撃移動////////////////////////////////
        if(gTheGame.chara[ch->c_num].stts==CS_Attack1){
            switch(gTheGame.chara[ch->c_num].Dir_Flag){
                //下
                case 0:
                    ch->dir.x=0.0;
                    ch->dir.y+=1.0;
                    break;
                //上
                case 1:
                    ch->dir.x=0.0;
                    ch->dir.y-=1.0;
                    break;
                //右
                case 2:
                    ch->dir.y=0.0;
                    ch->dir.x+=1.0;
                    break;
                //左
                case 3:
                    ch->dir.y=0.0;
                    ch->dir.x-=1.0;
                    break;
                //右上
                case 4:
                    ch->dir.y-=1.0;
                    ch->dir.x+=1.0;
                    break;
                //右下
                case 5:
                    ch->dir.y+=1.0;
                    ch->dir.x+=1.0;
                    break;
                //左上
                case 6:
                    ch->dir.y-=1.0;
                    ch->dir.x-=1.0;
                    break;
                //左下
                case 7:
                    ch->dir.y+=1.0;
                    ch->dir.x-=1.0;
                    break;
                default:
                    break;  
            }
            FloatPoint newpoint = ch->point;
            newpoint=MoveByDirItem(ch);
            //ある一定の距離まで移動すれば攻撃を終了
            if(gTheGame.chara[ch->c_num].Dir_Flag==1){
                if(gTheGame.chara[ch->c_num].point.y-newpoint.y>40){
                    ch->dir.x=0.0;
                    ch->dir.y=0.0;
                    newpoint = gTheGame.chara[ch->c_num].point;
                 }
            }
            else{
                if(abs(newpoint.x-gTheGame.chara[ch->c_num].point.x)>15 || 
                abs(gTheGame.chara[ch->c_num].point.y-newpoint.y)>15){
                    ch->dir.x=0.0;
                    ch->dir.y=0.0;
                    newpoint = gTheGame.chara[ch->c_num].point;
                 }
            }
            
            ch->point  = newpoint;
            ch->rect.x = newpoint.x;
            ch->rect.y = newpoint.y;
        }
        
        ////////////////////////////////////////////////////

        //所有物のときキャラについていく
        else if(gTheGame.chara[ch->c_num].stts==CS_Item){
            ch->dir.x=0.0;
            ch->dir.y=0.0;
            FloatPoint newpoint = gTheGame.chara[ch->c_num].point;
            ch->point  = newpoint;
            ch->rect.x = newpoint.x;
            ch->rect.y = newpoint.y;
        }
    }
}



/*****************************************************************
関数名	: MoveBase
機能	:拠点を移動させる処理
引数	: ch    拠点
出力	: 正常に設定できたとき0，失敗したとき-1
*****************************************************************/
void MoveBase(S_BaseInfo *ch){
    FloatPoint newpoint = ch->point;
    ch->point  = newpoint;
    ch->rect.x = newpoint.x;
    ch->rect.y = newpoint.y;

}



/*****************************************************************
関数名	: MoveObject
機能	:障害物を移動させる処理
引数	: ch    拠点
出力	: 正常に設定できたとき0，失敗したとき-1
*****************************************************************/
void MoveObject(S_Object *oh){
    FloatPoint newpoint = oh->point;
    oh->point  = newpoint;
    oh->rect.x = newpoint.x;
    oh->rect.y = newpoint.y;
}


/*****************************************************************
関数名	:  MoveFall
機能	:障害物を移動させる処理
引数	: ch    拠点
出力	: 正常に設定できたとき0，失敗したとき-1
*****************************************************************/
void MoveFall(S_Fall *fa){
    FloatPoint newpoint = fa->point;
    fa->point  = newpoint;
    fa->rect.x = newpoint.x;
    fa->rect.y = newpoint.y;

}

//木材を移動させる処理
void MoveMokuzai(S_MokuzaiInfo *mo){
    FloatPoint newpoint = mo->point;
    mo->point  = newpoint;
    mo->rect.x = newpoint.x;
    mo->rect.y = newpoint.y;
}

/*****************************************************************
関数名	:UpdateCharaStatus
機能	:キャラクターの状態を更新させる関数
引数  : ch      キャラクターの情報を示すポインタ
引数	: chara    キャラクターの番号
出力	: 正常に設定できたとき0，失敗したとき-1
*****************************************************************/
 void UpdateCharaStatus(S_CharaInfo *ch,int chara){
     if(ch->stts==CS_Disable){
        ch->stts = CS_Enable;
        ch->pre_stts=ch->stts;
        ch->point.x=rand()%(MAP_Width*MAP_ChipSizeZ-300);
        ch->point.y=rand()%(MAP_Height*MAP_ChipSizeZ-300);
     }
 }


/*****************************************************************
関数名	:UpdateItemStatus
機能	:アイテムの状態を更新させる関数
引数  : ih      アイテムの情報を示すポインタ
引数	: item    アイテムの番号
出力	: 正常に設定できたとき0，失敗したとき-1
*****************************************************************/
 void UpdateItemStatus(S_ItemInfo *ih,int item){
     if(ih->stts==IS_Disable){
         ih->stts   =IS_Enable;
         ih->point.x=rand()%(MAP_Width*MAP_ChipSizeZ-300);
         ih->point.y=rand()%(MAP_Height*MAP_ChipSizeZ-300);
     }
 }


/*****************************************************************
関数名	:UpdateItemStatus
機能	:障害物の状態を更新させる関数
引数  : oh      障害物の情報を示すポインタ
出力	: 正常に設定できたとき0，失敗したとき-1
*****************************************************************/
 void UpdateObjectStatus(S_Object *oh){
     if(oh->stts==OS_Disable){
        oh->stts   =OS_Enable;
        oh->point.x=rand()%(MAP_Width*MAP_ChipSizeZ-300);
        oh->point.y=rand()%(MAP_Height*MAP_ChipSizeZ-300);
        oh->rect.x = oh->point.x;
        oh->rect.y = oh->point.y;
     }
 }


/*****************************************************************
関数名	:UpdateBaseStatus
機能	:拠点の状態を更新させる関数
引数  : ih      拠点の情報を示すポインタ
引数	: base    拠点の番号
出力	: 正常に設定できたとき0，失敗したとき-1
*****************************************************************/
 void UpdateBaseStatus(S_BaseInfo *bs,int base){
    if(bs->stts==BS_Disable){
        bs->stts = BS_FLEE;
        bs->point.x=rand()%(MAP_Width*MAP_ChipSizeZ-300);
        bs->point.y=rand()%(MAP_Height*MAP_ChipSizeZ-300);
    }
  
 }



/*****************************************************************
関数名	: DirS
機能	:キャラクターの向きを決定させる処理
引数	: ch    キャラクター
引数	: Putkey   入力されたキー番号
出力	: 正常に設定できたとき0，失敗したとき-1
*****************************************************************/
void DirS(S_CharaInfo *ch,float rad){//変更必要12/3
    if(ch->stts==CS_Die){
         return;
    }
    int collusionO=0;
    ch->dir.x=0;
    ch->dir.y=0;
      //radの値からx,yを求める
    float deg = rad *(180.0 /M_PI);
    int   deg_int = deg;            //int型作成
    int   pre_mask_x,pre_mask_y;
    
    ch->collusion_F=0;
    //if(fabs(deg - deg_int) == 0.0f ){//少数点が０のとき、静止状態と判断する
    if(rad == 0.0f ){//少数点が０のとき、静止状態と判断する
        ch->input.left =SDL_FALSE;
        ch->input.right =SDL_FALSE;
        ch->input.down =SDL_FALSE;
        ch->input.up =SDL_FALSE;
        pre_mask_x=ch->mask.x;
        pre_mask_y=ch->mask.y;
       switch(ch->Dir_Flag){
           case 0:
               ch->mask.y+=40;
               break;
           case 1:
               ch->mask.y-=40;
               break;
           case 2:
               ch->mask.x+=40;
               break;
           case 3:
               ch->mask.x-=40;
               break;
           default:
               break;  
       }
       for (int c = 0; c < gTheGame.ObjeNum; c++) {
            if(ch->collusion_F==0){
                collusionO=CollisionObject(ch, &(gTheGame.object[c]));
            }
            if(collusionO==1){
                ch->collusion_F=1;
            }
        }
        ch->mask.x=pre_mask_x;
        ch->mask.y=pre_mask_y;
    }
    //(変更部分 2018/12/20)////////////////
     else{//8方向(2019.1.5)
        ch->input.left =SDL_FALSE;//初期化
        ch->input.right =SDL_FALSE;
        ch->input.down =SDL_FALSE;
        ch->input.up =SDL_FALSE;
        pre_mask_x=ch->mask.x;
        pre_mask_y=ch->mask.y;
        //左
        if(sin(rad)<= -0.8 && -0.3<= cos(rad) && cos(rad)<= 0.1){
            ch->input.left =SDL_TRUE;
            ch->Dir_Flag=3;
        }
        //右
        else if(sin(rad) >= 0.8 && -0.3<=cos(rad) && cos(rad)<=0.1){
            ch->input.right =SDL_TRUE;
            ch->Dir_Flag=2;
        }
        //上
        else if(cos(rad)>=0.8 && -0.3<=sin(rad) && sin(rad)<=0.3){
            ch->input.up =SDL_TRUE;
            ch->Dir_Flag=1;
        }
        //下
        else if(cos(rad)<=-0.8 && -0.3<=sin(rad) && sin(rad)<=0.3){
            ch->input.down =SDL_TRUE;
            ch->Dir_Flag=0;
        }
         //左上
        else if(-0.8<sin(rad) && sin(rad)<-0.3 && 0.1<cos(rad) && cos(rad)<0.8){
            ch->input.left =SDL_TRUE;
            ch->input.up =SDL_TRUE;
            ch->Dir_Flag=6;
        }
        //左下
        else if(-0.8<sin(rad) && sin(rad)<-0.3 && -0.8<cos(rad) && cos(rad)<-0.3){
            ch->input.left =SDL_TRUE;
            ch->input.down =SDL_TRUE;
            ch->Dir_Flag=7;
        }
        //右上
        else if(0.1<cos(rad) && cos(rad)<0.8 && 0.3<sin(rad) && sin(rad)<0.8){
            ch->input.right =SDL_TRUE;
            ch->input.up =SDL_TRUE;
            ch->Dir_Flag=4;
        }
        //右下
        else if(-0.8<cos(rad) && cos(rad)<-0.3 && 0.3<sin(rad) && sin(rad)<0.8){
            ch->input.right =SDL_TRUE;
            ch->input.down =SDL_TRUE;
            ch->Dir_Flag=5;
        }

        //木と衝突していなければ、移動
        ch->mask.x+=30*sin(rad);
        ch->mask.y-=40*cos(rad);
        for (int c = 0; c < gTheGame.ObjeNum; c++) {
            if(ch->collusion_F==0){
                collusionO=CollisionObject(ch, &(gTheGame.object[c]));
            }
            if(collusionO==1){
                ch->collusion_F=1;
            }
        }
        //キャラ同士の衝突
        if(ch->collusion_F==0){
            for (int i = 0; i < gTheGame.charaNum; i++) {
                if(ch->collusion_F==0){
                    collusionO=Collision(ch, &(gTheGame.chara[i]));
                }
                if(collusionO==1){
                    ch->collusion_F=1;
                }
            }
        }
        //キャラと拠点のフレームの衝突
        if(ch->collusion_F==0){
            for (int b = 0; b < gTheGame.BaseNum; b++) {
                if(ch->collusion_F==0){
                    collusionO=CollisionBaseFleam(ch, &(gTheGame.base[b]));
                }
                if(collusionO==1){
                    ch->collusion_F=1;
                }
            }
        }
        //木かキャラに当たったとき
        if(ch->collusion_F==1 && ch->stts!=CS_Ghost){ 
            if(ch->input.up ==SDL_TRUE  || ch->input.down ==SDL_TRUE){
                ch->dir.x += sin(rad);
            }
            else if(ch->input.left ==SDL_TRUE  || ch->input.right ==SDL_TRUE){
                ch->dir.y -= cos(rad);
            }
            
        }
        //当たらなかったとき
        else{
            ch->dir.x += sin(rad);
            ch->dir.y -= cos(rad);
        }
        ch->mask.x=pre_mask_x;
        ch->mask.y=pre_mask_y;
    }
    //ここまで///////////////////////////////////
}




/*****************************************************************
関数名	:ChuzeItem
機能	:アイテムを選択し、Bボタンで開放させる関数
引数  : ch      キャラクターの情報を示すポインタ
引数	: pos    キャラクターの番号
出力	: 正常に設定できたとき0，失敗したとき-1
*****************************************************************/
 void ChuzeItem(S_CharaInfo *ch,int pos){
    int pre_item,pre_item2;
if(ch->stts==CS_Die || ch->stts==CS_Ghost ||ch->stts==CS_Fallout){
        return;
    }
        //アイテム開放
    if(gTheGame.item[ch->chuzeitem].stts==IS_Disable){
        if(ch->stts==CS_Chuze){
            switch(ch->chuzeitem-5*pos){
                case IT_Piko:
                    if(ch->Mokuzai>=3){
                        gTheGame.item[ch->chuzeitem].stts=IS_Open;
                        ch->Mokuzai-=3;
                        fprintf(stderr,"Piko Maked!\n");
                    }
                    break;
                case IT_Kasa:
                    if(ch->Mokuzai>=3){
                        gTheGame.item[ch->chuzeitem].stts=IS_Open;
                        ch->Mokuzai-=3;
                        fprintf(stderr,"Mizu Maked!\n");
                    }
                    break;
                case IT_Ono:
                    if(ch->Mokuzai>=3){
                        gTheGame.item[ch->chuzeitem].stts=IS_Open;
                        ch->Mokuzai-=3;
                        gTheGame.item[ch->chuzeitem].count_ono=5;
                        fprintf(stderr,"Ono Maked!\n");
                    }
                    break;
                case IT_Scop:
                    if(ch->Mokuzai>=3){
                        gTheGame.item[ch->chuzeitem].stts=IS_Open;
                        ch->Mokuzai-=3;
                        fprintf(stderr,"Scop Maked!\n");
                    }
                    break;
            } 
            ch->stts=ch->pre_stts;         
        }
        else if( ch->stts==CS_Item){
            ch->stts=CS_Enable;
        }
    }
    //アイテム使用
    if(gTheGame.item[ch->chuzeitem].stts==IS_Open){
        ch->stts=CS_Item;
        gTheGame.item[ch->chuzeitem].stts=IS_Mine;
        ch->i_num=ch->chuzeitem;
        gTheGame.item[ch->chuzeitem].c_num=pos;
        ch->pre_stts=CS_Item;
    }
    //使用していたアイテムを開放状態に戻す(左側)
    pre_item=ch->chuzeitem-1;
    if(pre_item<5*pos){
        pre_item=3+5*pos;
    }
    if(gTheGame.item[pre_item].stts!=IS_Disable){
        gTheGame.item[pre_item].stts=IS_Open;
        gTheGame.item[pre_item].c_num=50;  
    }
        
    //使用していたアイテムの状態をもとに戻す(右側)
    pre_item2=ch->chuzeitem+1;
    if(pre_item2>3+5*pos){
        pre_item2=5*pos;
    }
        
    if(gTheGame.item[pre_item2].stts!=IS_Disable){
        gTheGame.item[pre_item2].stts=IS_Open;
        gTheGame.item[pre_item2].c_num=50;  
    }
 }



/*****************************************************************
関数名	: MakePitfall
機能	:   落とし穴の作成
引数1 : ci   クライアント構造体
引数2 : ij   アイテム構造体  
引数3 : pos   クライアント番号  
出力	: なし
*****************************************************************/
void MakePitfall(S_CharaInfo *ci,S_ItemInfo *ij,int pos){


    if(ci->Damege_Flag > 0){
        ci->stts = CS_Item;
        return;
    }       
    if(ij->countf ==  5){
        ci->stts = CS_Item;
        return;
    } 

    //追記(2019.1.13)
    if( ci->Dir_Flag== 1){//上
        gTheGame.fall[countfall].point.x = ci->point.x-10;
        gTheGame.fall[countfall].point.y = ci->point.y - 140.0;
    }else if( ci->Dir_Flag== 0){//下
        gTheGame.fall[countfall].point.x = ci->point.x+10;
        gTheGame.fall[countfall].point.y = ci->point.y + 140.0;
    }else if( ci->Dir_Flag== 3){//左
        gTheGame.fall[countfall].point.x = ci->point.x - 140.0;
        gTheGame.fall[countfall].point.y = ci->point.y;
    }else if( ci->Dir_Flag== 2){//右
        gTheGame.fall[countfall].point.x = ci->point.x + 140.0;
        gTheGame.fall[countfall].point.y = ci->point.y;
    }else if( ci->Dir_Flag== 6){//左上
        gTheGame.fall[countfall].point.x = ci->point.x - 140.0;
        gTheGame.fall[countfall].point.y = ci->point.y - 140.0;
    }else if( ci->Dir_Flag== 7){//左下
        gTheGame.fall[countfall].point.x = ci->point.x - 140.0;
        gTheGame.fall[countfall].point.y = ci->point.y + 140.0;
    }else if( ci->Dir_Flag== 4){//右上
        gTheGame.fall[countfall].point.x = ci->point.x + 140.0;
        gTheGame.fall[countfall].point.y = ci->point.y - 140.0;
    }else if( ci->Dir_Flag== 5){//右下
        gTheGame.fall[countfall].point.x = ci->point.x + 140.0;
        gTheGame.fall[countfall].point.y = ci->point.y + 140.0;
    }
    gTheGame.fall[countfall].rect.x= gTheGame.fall[countfall].point.x;
    gTheGame.fall[countfall].rect.y= gTheGame.fall[countfall].point.y;
    //fprintf(stderr,"fall[%d]=%f\n",countfall,gTheGame.fall[countfall].point.y);
    if(gTheGame.fall[countfall].point.x < 130.0 || gTheGame.fall[countfall].point.y < 130.0 ){//マップ外に落とし穴を作らない
        ci->stts = CS_Item;
        //fprintf(stderr,"not make fall!\n");
        return;
    }
    if(gTheGame.fall[countfall].point.x > 4670.0 || gTheGame.fall[countfall].point.y > 4670.0 ){
        ci->stts = CS_Item;
        //fprintf(stderr,"not make fall!\n");
        return;
    }
    int pastfall = countfall - 1;
    if(pastfall >= 0){//落とし穴がかぶったら作らない
        for(pastfall; 0 < pastfall ; pastfall--){
            if(gTheGame.fall[pastfall].stts == FS_Enable){
                if(fabs(gTheGame.fall[pastfall].point.x - gTheGame.fall[countfall].point.x) < 100.0 && fabs(gTheGame.fall[pastfall].point.y - gTheGame.fall[countfall].point.y) < 100.0){
                    ci->stts = CS_Item;
                    return;
                }
            }
        
        }
    }
    //gTheGame.fall[countfall].point.y = ci->point.y;
    gTheGame.fall[countfall].stts = FS_Enable;
    gTheGame.fall[countfall].c_num = pos;
    ci->stts = CS_Item;
    ci->Damege_Flag = 1;
    ij->countf++;
    
    Send_Fall_Command(pos,countfall);
    countfall++;
   
}



/********************************************************************************
当たり判定処理
********************************************************************************/



//(変更部分 voidをintに変更 2018/12/20)
/*****************************************************************
関数名	: Collision
機能	: 当たり判定の処理
引数	: ci    キャラクター1
引数2 : cj    キャラクター2
出力	: なし
*****************************************************************/
int Collision(S_CharaInfo *ci, S_CharaInfo *cj)
{
     /* 判定が不要な組み合わせを除外 */
    if (ci->stts == CS_Disable || cj->stts == CS_Disable)
        return 0;
    if (ci->type == CT_Player && cj->type == CT_Player)
        return  0;
    if (cj->type == CT_2P && ci->type == CT_2P)
        return  0;
    if (cj->type == CT_3P && ci->type == CT_3P)
        return  0;
    if (cj->type == CT_4P && ci->type == CT_4P)
        return  0;
    /* マスクをマップ座標に合わせる */
    SDL_Rect mi = ci->mask;
    mi.x += ci->rect.x;
    mi.y += ci->rect.y;
    SDL_Rect mj = cj->mask;
    mj.x += cj->rect.x;
    mj.y += cj->rect.y;
     /* 当たった */
    if (SDL_HasIntersection(&mi, &mj)) {
         return 1;
    }
    else{
        return 0;
    }
}


/*****************************************************************
関数名	: CollisionItem
機能	: キャラとアイテムの当たり判定の処理
引数	: ci    キャラクター
引数2 : ij    アイテム
引数3 : num   アイテムの番号
出力	: なし
*****************************************************************/
void CollisionItem(S_CharaInfo *ci, S_ItemInfo *ij,int num,int pos)
{
    if (ci->stts == CS_Disable ||ci->stts == CS_Die ||ij->stts == IS_Disable 
    || ci->stts==CS_Ghost || ci->stts==CS_Fallout)
        return;
    /* マスクをマップ座標に合わせる */
    SDL_Rect mi = ci->mask;
    mi.x += ci->rect.x;
    mi.y += ci->rect.y;
    SDL_Rect mj = ij->mask;
    mj.x += ij->rect.x;
    mj.y += ij->rect.y;
     /* 当たった */
    if (SDL_HasIntersection(&mi, &mj)) {
        //攻撃状態のアイテムとキャラとのダメージ処理
        if(ij->type==IT_Piko && ij->stts==IS_Mine && ij->c_num!=pos && gTheGame.chara[ij->c_num].stts==CS_Attack1
        && ci->Damege_Flag==0){
            if(gTheGame.chara[ij->c_num].group!=ci->group){
                ci->hp-=1;
                gTheGame.chara[ij->c_num].stts=CS_Item;
                ci->Damege_Flag=1;
                if(ci->hp==0){
                    ci->stts=CS_Die;
                    if(ci->Mokuzai>0){
                        int moku_count=0;
                        for(int i=0;i<gTheGame.MokuNum;i++){
                            if(gTheGame.mokuzai[i].stts==IS_Disable){
                                gTheGame.mokuzai[i].stts = IS_Enable;
                                FloatPoint newpoint = ci->point;
                                gTheGame.mokuzai[i].point.x=newpoint.x-30*moku_count;
                                gTheGame.mokuzai[i].point.y=newpoint.y-30*moku_count;
                                gTheGame.mokuzai[i].rect.x=newpoint.x-30*moku_count;
                                gTheGame.mokuzai[i].rect.y=newpoint.y-30*moku_count;
                                ci->Mokuzai-=1;
                                if(ci->Mokuzai==0){
                                    i=gTheGame.MokuNum;
                                }
                                moku_count++;
                                if(moku_count>2){
                                    i=gTheGame.MokuNum;
                                }
                            }
                        }
                    }
                    int my = ci->i_num;
                    if(gTheGame.item[my].stts == IS_Mine){
                        gTheGame.item[my].stts = IS_Disable;
                        ci->i_num = 50;
                    }
                }
            }
           
        }
    }
}

//(変更部分 Send_Rizult_Commandの消去  2018/12.20)
/*****************************************************************
関数名	: CollisionBase
機能	: 拠点とアイテムの当たり判定の処理
引数	: ij    アイテム
引数2 : jk    拠点
出力	: なし
*****************************************************************/
void CollisionBase(S_ItemInfo *ij, S_BaseInfo *jk,int num,int pos)
{
    SDL_Rect mi = ij->mask;
    mi.x += ij->rect.x;
    mi.y += ij->rect.y;
    SDL_Rect mj = jk->mask;
    mj.x += jk->rect.x;
    mj.y += jk->rect.y;
    if(SDL_HasIntersection(&mi, &mj)){
        //拠点への攻撃処理
        if( ij->type==IT_Kasa && ij->stts==IS_Mine && gTheGame.chara[ij->c_num].stts==CS_Attack1 
        && jk->Damege_Flag==0 && jk->stts!=BS_FLEE){
            //グループ2が立てた拠点への攻撃
            if(jk->stts==BS_Group2 &&gTheGame.chara[ij->c_num].group==0){
                jk->hp_B-=1;
                Send_Messege_Command(1,0);
            }
            //グループ1が立てた拠点への攻撃
            else if(jk->stts==BS_Group1 && gTheGame.chara[ij->c_num].group==1){
                jk->hp_A-=1;
                Send_Messege_Command(0,0);
            }
            //グループ1が立てた拠点への攻撃
            else if(jk->stts==BS_Group1_P && gTheGame.chara[ij->c_num].group==1){
                jk->hp_A-=1;
                Send_Messege_Command(0,0);
            }
            else if(jk->stts==BS_Group2_P &&gTheGame.chara[ij->c_num].group==0){
                jk->hp_B-=1;
                Send_Messege_Command(1,0);
            }
            gTheGame.chara[ij->c_num].stts=CS_Item;
            jk->Damege_Flag=1;
            //グループ1が立てた拠点が破壊されたときの処理
            if(jk->hp_A==0){
                jk->stts=BS_FLEE;
                jk->UPGreadFlag=0;
                gTheGame.Group1_point-=1;
                Send_Messege_Command(0,1);
                fprintf(stderr,"Group1_point=%dpoint!\n",gTheGame.Group1_point);
            }
            //グループ2が立てた拠点が破壊されたときの処理
            else if(jk->hp_B==0){
                jk->stts=BS_FLEE;
                jk->UPGreadFlag=0;
                gTheGame.Group2_point-=1;
                Send_Messege_Command(1,1);
                fprintf(stderr,"Group2_point=%dpoint!\n",gTheGame.Group2_point); 
            }
        }
    }    
}



/*****************************************************************
関数名	: CollisionCharaBase
機能	: キャラと拠点の当たり判定の処理
引数	: ci    キャラ    
引数2 : jk    拠点
出力	: なし
*****************************************************************/
void  CollisionCharaBase(S_CharaInfo *ci, S_BaseInfo *jk)
{
    SDL_Rect mi = ci->mask;
    mi.x += ci->rect.x;
    mi.y += ci->rect.y;
    SDL_Rect mj = jk->mask;
    mj.x += jk->rect.x;
    mj.y += jk->rect.y;

    //拠点を立てる処理
    if(SDL_HasIntersection(&mi, &mj)){
        ci->SteyFlag=10;
        if(jk->stts==BS_FLEE){
            ci->ActFlag=2;          
            if(ci->stts==CS_Act){
                Send_BCol_Command(ci->type,jk->type,ci->SteyFlag,1);
                //グループ1の拠点を立てる処理
                if(ci->group==0 && ci->Mokuzai >=10){
                    jk->stts=BS_Group1;
                    ci->Mokuzai-=10;
                    gTheGame.Group1_point+=1;
                    jk->hp_A=15;
                    jk->hp_B=15;
                    fprintf(stderr,"Group1_point=%dpoint!\n",gTheGame.Group1_point);
                }
                //グループ2の拠点を立てる処理
                else if(ci->group==1  && ci->Mokuzai >=10){
                    jk->stts=BS_Group2;
                    ci->Mokuzai-=10;
                    jk->hp_B=15;
                    jk->hp_A=15;
                    //fprintf(stderr,"chara[%d]=mokuzai%d\n",ci->type,ci->Mokuzai);
                    gTheGame.Group2_point+=1;
                    fprintf(stderr,"Group2_point=%dpoint!\n",gTheGame.Group2_point); 
                }
                ci->stts=ci->pre_stts;
            } 
            //メッセージの表示だけ
            else{
                Send_BCol_Command(ci->type,jk->type,ci->SteyFlag,0);
            }
        }
        //拠点を増築(グループ1)
        else if(jk->stts==BS_Group1){
            if(ci->group==0 &&  ci->Mokuzai >=10 && jk->UPGreadFlag==0){
                ci->ActFlag=2;  
                if(ci->stts==CS_Act){
                    jk->stts=BS_Group1_P;
                    ci->Mokuzai-=10;
                    jk->hp_A= jk->hp_A + 15;
                    jk->UPGreadFlag=1;
                    gTheGame.Group1_point+=1;
                    fprintf(stderr,"GreadUP!!->Group1\n");
                    fprintf(stderr,"Group1_point=%dpoint!\n",gTheGame.Group1_point);
                    ci->stts=ci->pre_stts;
                }
            }
            Send_BCol_Command(ci->type,jk->type,ci->SteyFlag,1);
        }
        //拠点を増築(グループ2)
        else if( jk->stts==BS_Group2 ){ 
            if(ci->group==1 &&  ci->Mokuzai >=10 && jk->UPGreadFlag==0){
                ci->ActFlag=2;  
                if(ci->stts==CS_Act){
                    jk->stts=BS_Group2_P;
                    ci->Mokuzai-=10;
                    jk->hp_B= jk->hp_B + 15;
                    jk->UPGreadFlag=1;
                    gTheGame.Group2_point+=1;
                    fprintf(stderr,"GreadUP!!->Group2\n");
                    fprintf(stderr,"Group2_point=%dpoint!\n",gTheGame.Group2_point);
                    ci->stts=ci->pre_stts;
                }
            }
            Send_BCol_Command(ci->type,jk->type,ci->SteyFlag,1);
        }
        else if(jk->stts==BS_Group1_P){
             Send_BCol_Command(ci->type,jk->type,ci->SteyFlag,1);
        }
        else if(jk->stts==BS_Group2_P){
             Send_BCol_Command(ci->type,jk->type,ci->SteyFlag,1);
        }
    }
    //拠点を離れたとき
    else{
        if(ci->SteyFlag>1){
            ci->SteyFlag-=1;
        }
        else if(ci->SteyFlag==1){   
            ci->SteyFlag=0;
            ci->ActFlag=0;  
            Send_BCol_Command(ci->type,jk->type,ci->SteyFlag,0);
        }
    }
}




/*****************************************************************
関数名	:CollisionBaseFleam
機能	:拠点の周りの柵の当たり判定
引数	: ci    キャラ    
引数2 : jk    拠点
出力	: なし
*****************************************************************/
int  CollisionBaseFleam(S_CharaInfo *ci, S_BaseInfo *jk)
{
    if (ci->stts == CS_Disable ||ci->stts == CS_Die)
        return 0;
    SDL_Rect mi = ci->mask;
    mi.x += ci->rect.x;
    mi.y += ci->rect.y;
    SDL_Rect mj = jk->mask2;
    mj.x += jk->rect.x;
    mj.y += jk->rect.y;
    SDL_Rect mj1 = jk->mask3;
    mj1.x += jk->rect.x;
    mj1.y += jk->rect.y;
    SDL_Rect mj2 = jk->mask4;
    mj2.x += jk->rect.x;
    mj2.y += jk->rect.y;
    //拠点に衝突する処理
    if(SDL_HasIntersection(&mi, &mj)){
         return 1;
    }
    else if(SDL_HasIntersection(&mi, &mj1)){
        return 1;
    }
    else if(SDL_HasIntersection(&mi, &mj2)){
        return 1;
    }
    else{
         return 0;    
    }
}


/*****************************************************************
関数名	: CollisionObject
機能	: キャラと木の当たり判定の処理
引数	: ci    キャラ
引数2 : oj    木    
出力	: なし
*****************************************************************/
int CollisionObject(S_CharaInfo *ci, S_Object *oj)
{ 
    if (ci->stts == CS_Disable ||ci->stts == CS_Die ||oj->stts ==OS_Die)
        return 0;
    SDL_Rect mi = ci->mask;
    mi.y += ci->rect.y;
    mi.x += ci->rect.x;
    SDL_Rect mj = oj->mask;
    mj.x += oj->rect.x;
    mj.y += oj->rect.y;
    //木と衝突したとき、動きを止める
    if(SDL_HasIntersection(&mi, &mj)){
         ci->ActFlag=1; 
         if( ci->stts==CS_Act){
            ci->stts=CS_Wood;
            oj->stts=OS_Shake;
            ci->fwood = 1; 
            oj->fwood = 1; 
            int pre_pos=0;
            for(int pos=0;pos<gTheGame.MokuNum;pos++){
                if(gTheGame.mokuzai[pos].stts==IS_Disable){
                    FloatPoint newpoint = oj->point;
                    if( gTheGame.mokuzai[pre_pos].stts==IS_Enable && 
                    gTheGame.mokuzai[pos].point.x==gTheGame.mokuzai[pre_pos].point.x &&
                    gTheGame.mokuzai[pos].point.y==gTheGame.mokuzai[pre_pos].point.y){
                        pos=gTheGame.MokuNum;
                    }
                    else{
                        gTheGame.mokuzai[pos].stts = IS_Enable;
                        gTheGame.mokuzai[pos].point.x  = newpoint.x + 20;
                        gTheGame.mokuzai[pos].point.y =  newpoint.y +300;
                        gTheGame.mokuzai[pos].rect.x = newpoint.x + 20;
                        gTheGame.mokuzai[pos].rect.y = newpoint.y + 300;
                        pos=gTheGame.MokuNum;
                    }
                    pre_pos=pos;   
                }
            }
        }
        return 1;
    }
    else{
        if(ci->ActFlag!=2){
            ci->ActFlag=0;
        }
        return 0;
    }
}


/*****************************************************************
関数名	: CollisionWood
機能	: アイテムと木の当たり判定の処理
引数	: ii    アイテム
引数2 : oj    オブジェクト    
出力	: なし
*****************************************************************/
void CollisionWood(S_ItemInfo *ii, S_Object *oj)
{ 
    
    SDL_Rect mi = ii->mask;
    mi.y += ii->rect.y;
    mi.x += ii->rect.x;
    SDL_Rect mj = oj->mask2;
    mj.x += oj->rect.x;
    mj.y += oj->rect.y;
    //木と衝突したとき、木を伐採する
    if(SDL_HasIntersection(&mi, &mj) && oj->stts==OS_Enable
    && ii->type==IT_Ono && gTheGame.chara[ii->c_num].stts==CS_Attack1){
        if(ii->count_ono>0){
            ii->count_ono-=1;
            fprintf(stderr,"count_Ono=%d\n",ii->count_ono);
        }
        if(ii->count_ono==0){
            ii->stts=IS_Disable;
        }
        oj->stts=OS_Die;
        int moku_count=0;
        //木を出す処理
        for(int i;i<gTheGame.MokuNum;i++){
            if(gTheGame.mokuzai[i].stts==IS_Disable){
                gTheGame.mokuzai[i].stts = IS_Enable;
                FloatPoint newpoint = oj->point;
                gTheGame.mokuzai[i].point.x  = newpoint.x-20*moku_count;
                gTheGame.mokuzai[i].point.y =  newpoint.y+20*moku_count;
                gTheGame.mokuzai[i].rect.x = newpoint.x-20*moku_count;
                gTheGame.mokuzai[i].rect.y = newpoint.y+20*moku_count;
                moku_count++;
                if(moku_count>2){
                    i=gTheGame.MokuNum;
                }
            }
        }
    }
}



/*****************************************************************
関数名	: Ghost
機能	:   ゴースト状態のときの処理
引数	: ci    キャラ
引数2 : pos   クライアント番号  
出力	: なし
*****************************************************************/
void Ghost(S_CharaInfo *xi,int pos){
    
    if(xi->stts == CS_Die){
        xi->stts = CS_Ghost;
        SendGhost(pos,3);
    }else if(xi->stts == CS_Ghost){
        xi->stts = CS_Enable;
        xi->hp = 3;
        xi->f_rireve = 1;
        SendGhost(pos,1);
    }else if(xi->stts == CS_Fallout){//(2018.12.21)
        if(xi->i_num != 50){
            xi->stts = CS_Item;
        }else{
            xi->stts = CS_Enable;
        }
        gTheGame.fall[xi->fall_num].stts = FS_Disable;
        xi->fall_num = 30;   //リセット
        SendGhost(pos,2);
    }

}



/*****************************************************************
関数名	: CollisionFall
機能	: キャラと落とし穴の当たり判定の処理
引数	: ci    キャラクター
引数2 : fi    落とし穴
引数3 : pos   clientの番号
引数4 : fall  落とし穴の番号
出力	: なし
*****************************************************************/
void CollisionFall(S_CharaInfo *ci, S_Fall *fi,int pos,int fall)
{
    if (ci->stts == CS_Disable ||ci->stts == CS_Die ||ci->stts == CS_Ghost ||fi->stts == FS_Disable ||fi->stts == FS_Catch)
        return;
    /* マスクをマップ座標に合わせる */
    int item = 50;
    SDL_Rect mi = ci->mask;
    mi.x += ci->rect.x;
    mi.y += ci->rect.y;
    SDL_Rect mj = fi->mask;
    mj.x += fi->rect.x;
    mj.y += fi->rect.y;
     /* 当たった */
    if (SDL_HasIntersection(&mi, &mj)){
            ci->stts =CS_Fallout;
            ci->fall_num = fall;
            fi->stts = FS_Catch;
            Send_Catch_Command(ci->type,fall);
        
    }
}

/*****************************************************************
関数名	: CollisionOAO
機能	: オブジェクトどうしの当たり判定の処理
引数	: oi    オブジェクト1
引数2 : oj    オブジェクト2
出力	: なし
*****************************************************************/
int CollisionOAO(S_Object *oi, S_Object *oj)
{
    /* マスクをマップ座標に合わせる */
    SDL_Rect mi = oi->mask2;
    mi.x += oi->rect.x;
    mi.y += oi->rect.y;
    SDL_Rect mj = oj->mask2;
    mj.x += oj->rect.x;
    mj.y += oj->rect.y;

    /* 当たった */
    if (SDL_HasIntersection(&mi, &mj)) {        
        return 1;
    }
    else{
        return 0;
    }
}

/*****************************************************************
関数名	: CollisionBaseObject
機能	: 拠点とオブジェクトの当たり判定の処理
引数	: jk 　 拠点    
引数2 : oj    オブジェクト    
出力	: なし
*****************************************************************/
int CollisionBaseObject(S_BaseInfo *jk, S_Object *oj ){
    /* マスクをマップ座標に合わせる */
    SDL_Rect mi = jk->mask;
    mi.x += jk->rect.x;
    mi.y += jk->rect.y;
    SDL_Rect mj = oj->mask2;
    mj.x += oj->rect.x;
    mj.y += oj->rect.y;

    /* 当たった */
    if (SDL_HasIntersection(&mi, &mj)) {        
        return 1;
    }
    else{
        return 0;
    }
}

/*****************************************************************
関数名	: CollisionMokuzai
機能	: 拠点とオブジェクトの当たり判定の処理
引数	: mo 　 木材    
引数2 : ci    キャラクター    
出力	: なし
*****************************************************************/
void CollisionMokuzai(S_MokuzaiInfo *mo, S_CharaInfo *ci ){
    if (ci->stts == CS_Disable ||ci->stts == CS_Die ||ci->stts == CS_Ghost)
        return;
    if (mo->stts == IS_Disable)
        return;
    /* マスクをマップ座標に合わせる */
    SDL_Rect mi = mo->mask;
    mi.x += mo->rect.x;
    mi.y += mo->rect.y;
    SDL_Rect mj = ci->mask;
    mj.x += ci->rect.x;
    mj.y += ci->rect.y;

    /* 当たった */
    if (SDL_HasIntersection(&mi, &mj)) {      
        ci->Mokuzai += 1;
        mo->stts = IS_Disable;
    }  

}

