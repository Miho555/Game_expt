/*****************************************************************
ファイル名	: common.h
機能		: サーバーとクライアントで使用する定数の宣言を行う
*****************************************************************/

#ifndef _COMMON_H_
#define _COMMON_H_

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>
#include<assert.h>
#include<math.h>

#define PORT			(u_short)8888	/* ポート番号 */

#define MAX_CLIENTS		4				/* クライアント数の最大値 */
#define MAX_NAME_SIZE	10 				/* ユーザー名の最大値*/

#define MAX_DATA		1000				/* 送受信するデータの最大値 */


#define Win_COMMAND	'A'				/* 勝利コマンド */
#define Los_COMMAND 'B'				/* 敗北コマンド */
#define Dra_COMMAND 'C'				/* 引き分けコマンド */
#define Mov_COMMAND	'D'				/* 移動コマンド */
#define Ite_COMMAND	'E'				/* アイテムコマンド */
#define Att_COMMAND	'F'				/* 攻撃コマンド */
#define END_COMMAND 'G'             /* 終了コマンド*/
#define BCol_COMMAND 'H'            /* 拠点当たり判定コマンド*/
#define Msg_COMMAND 'I'            /*メッセージコマンド*/
#define HP_COMMAND 'J'              /* HPコマンド*/
#define Stts_COMMAND 'K'            /* 状態のコマンド*/
#define SttsO_COMMAND 'L'            /* 状態のコマンド*/
#define Mask_COMMAND 'M'            /* Maskのコマンド*/
#define Obje_COMMAND 'N'            /* 障害物のコマンド*/
#define Ghost_COMMAND 'O'           /* 幽霊状態のコマンド*/
#define Time_COMMAND  'P'           /*現在の時間の送受信*/
#define Chu_COMMAND   'Q'
#define Catch_COMMAND 'R'           /* 落とし穴のコマンド*/
#define Fall_COMMAND 'S'           /* 落とし穴のコマンド*/ 
#define Start_COMMAND 'T'            /* スタートのコマンド*/          

#endif
