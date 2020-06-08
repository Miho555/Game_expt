/*****************************************************************
ファイル名	: server_func.h
機能		: サーバーの外部関数の定義
*****************************************************************/

#ifndef _SERVER_FUNC_H_
#define _SERVER_FUNC_H_

#include"server_common.h"
#include"server_system.h"

/* server_net.c */
extern int SetUpServer(int num);
extern void Ending(void);
extern int RecvIntData(int pos,int *intData);
extern void SendData(int pos,void *data,int dataSize);
extern int SendRecvManager(void);

/* server_command.c */

extern int ExecuteCommand(char command,int pos);
/*自動で送信する部分*/
extern void SendXY(void);
extern void SendHP(void);
extern void SendStts(void);
extern void SendObjectXY(void);
extern void SendSttO(void);
extern void SendTime(unsigned int severtime);

/*条件を満たすと送信する部分*/
extern void SendGhost(int pos,int sttsG);
extern void Recv_Mask(int pos);
extern void Send_BCol_Command(int chara,int base,int Flag,int Build);
extern void Send_Rizult_Command(void);
extern void Send_Fall_Command(int chara,int fall);//2018.12.22
extern void Send_Catch_Command(int chara,int fall);//2018.12.22
void Send_Messege_Command(int group,int change);

/*server_system.c*/
/*初期化*/
extern int InitSystemS(void);
extern void DestroySystemS(void);

/*移動*/
extern void MoveCharaS(S_CharaInfo *ch);
extern void MoveItem(S_ItemInfo *ch);
extern void MoveBase(S_BaseInfo *ch);
extern void MoveObject(S_Object *oh);
extern void MoveFall(S_Fall *fa);
extern void MoveMokuzai(S_MokuzaiInfo *mo);


/*更新*/
extern void UpdateItemStatus(S_ItemInfo *ih,int item);
extern void UpdateBaseStatus(S_BaseInfo *bs,int base);
extern void UpdateCharaStatus(S_CharaInfo *ch,int chara);
extern void UpdateObjectStatus(S_Object *oh);
extern void DirS(S_CharaInfo *ch,float rad);
extern void Ghost(S_CharaInfo *xi,int pos);
extern void ThrowItem(S_CharaInfo *ch,int pos);
extern void ChuzeItem(S_CharaInfo *ch,int pos);
extern void Set_Object(void);


/*当たり判定*/
extern int Collision(S_CharaInfo *ci, S_CharaInfo *cj);
extern void CollisionItem(S_CharaInfo *ci, S_ItemInfo *ij,int num,int pos);
extern void CollisionBase(S_ItemInfo *ij, S_BaseInfo *jk,int num,int pos);
extern void CollisionCharaBase(S_CharaInfo *ci, S_BaseInfo *jk);
extern int  CollisionObject(S_CharaInfo *ci, S_Object *oj);
extern void CollisionWood(S_ItemInfo *ii, S_Object *oj);
extern void MakePitfall(S_CharaInfo *ch,S_ItemInfo *ij,int pos);
extern void CollisionFall(S_CharaInfo *ci, S_Fall *fi,int pos,int fall);
extern int  CollisionBaseFleam(S_CharaInfo *ci, S_BaseInfo *jk);
extern int CollisionOAO(S_Object *oi, S_Object *oj);
extern int CollisionBaseObject(S_BaseInfo *jk, S_Object *oj);
extern void CollisionMokuzai(S_MokuzaiInfo *mo, S_CharaInfo *ci );

#endif
