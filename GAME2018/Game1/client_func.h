/*****************************************************************
ファイル名	: client_func.h
機能		: クライアントの外部関数の定義
*****************************************************************/

#ifndef _CLIENT_FUNC_H_
#define _CLIENT_FUNC_H_

#include"common.h"
#include"system.h"

//2018.11.9 wiimoteのヘッダファイル
#include <libcwiimote/wiimote.h>	// Wiiリモコンを用いるために必要なヘッダファイル
#include <libcwiimote/wiimote_api.h>	// Wiiリモコンを用いるために必要なヘッダファイル
#include <math.h> 
#include <SDL2/SDL_mixer.h> //音楽用(2019.1.11)

/* client_net.c */
extern int SetUpClient(char* hostName,int *clientID,int *num,char clientName[][MAX_NAME_SIZE]);
extern void CloseSoc(void);
extern int RecvIntData(int *intData);
extern int RecvfloatData(float *floatData);
extern void SendData(void *data,int dataSize);
extern int SendRecvManager(void);

/* client_win.c */
extern int InitWindows(int clientID,int num,char name[][MAX_NAME_SIZE]);
extern void DestroyWindow(void);
extern int RenderWindow(void);
extern int TitleWindow(void);//(2019.1.7)
extern int WaitWindow(void);//(2019.1.11)
//extern void WindowEvent(int num,int clientID);


/* client_command.c */
extern int ExecuteCommand(char command);
extern void Send_ATT_Command(int pos,int Pattern);
extern void SendEndCommand(void);
extern void Send_MOVE_Command(int pos,float rad);
extern void Send_Chuze_Command(int pos,int dir);
extern void SendMask(void);
extern void Send_Start_Command(int pos);


/*client_system.c*/
extern int InitSystem(void);
extern void DestroySystem(void);
extern void MoveChara(CharaInfo *ch);
extern void UpdateCharaStatus(CharaInfo *ch);
extern SDL_Rect MeasureMask(SDL_Surface *s, SDL_Rect r);
extern void MoveItem(ItemInfo *ch);
extern void MoveBase(BaseInfo *ch);
extern void MoveObje(S_Object *ch);
extern void MoveMokuzai(MokuzaiInfo *mo);
extern void Radian(CharaInfo *ch,int joyx,int joyy);
extern void MoveFall(S_Fall *fa);//2018.12.22
#endif
