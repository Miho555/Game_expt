/*****************************************************************
ファイル名	: server_main.c
機能		: サーバーのメインルーチン
*****************************************************************/
//(変更部分・・・コマンドを処理する関数SendRecvManagerをスレッドとして起動するように変更)
//(2018/11/17)
#include"server_common.h"
#include"server_system.h"
#include"server_func.h"

static Uint32 SignalHandler(Uint32 interval, void *param);
static Uint32 Timer1(Uint32 interval, void *param);
static Uint32 Timer2(Uint32 interval, void *param);

static int NetworkEvent(void *data);

//CS_Die、CS_Ghostのとき、5s後に状態変化するための時間計測用(2018.12.9)
unsigned int previous_time[4];
unsigned int current_time[4];
int flag_ghost[4];//初回のprevious_timeの更新用のフラグ(2018.12.9)
unsigned int severtime;

int main(int argc,char *argv[])
{
	int	num;
	int	endFlag = 1;
	SDL_Thread	*thr;	
	/* 引き数チェック */
	if(argc != 2){
		fprintf(stderr,"Usage: number of clients\n");
		exit(-1);
	}
	if((num = atoi(argv[1])) < 0 ||  num > MAX_CLIENTS){
		fprintf(stderr,"clients limit = %d \n",MAX_CLIENTS);
		exit(-1);
	}
	gTheGame.numnum = num;
	/* SDLの初期化 */
	if(SDL_Init(SDL_INIT_TIMER) < 0) {
		printf("failed to initialize SDL.\n");
		exit(-1);
	}

	/* クライアントとの接続 */
	if(SetUpServer(num) == -1){
		fprintf(stderr,"Cannot setup server\n");
		exit(-1);
	}
	/*システムの初期化*/
    if (InitSystemS() < 0) {
        fprintf(stderr,"failed to initialize System\n");
        return -1;
    }
	 /* タイマー */
    SDL_TimerID timer1 = SDL_AddTimer(200, Timer1, NULL);
    if(timer1==0){
        fprintf(stderr,"Not timer1\n");
        return -1;
    }
	/* タイマー:CS_Woodの切り替え (2019.1.12)*/
    SDL_TimerID timer2 = SDL_AddTimer(400, Timer2, NULL);
    if(timer2==0){
        fprintf(stderr,"Not timer2\n");
        return -1;
    }
	
	/* ネットワークイベント処理スレッドの作成(追加部分 2018/11/17)*/
	thr = SDL_CreateThread(NetworkEvent,"NetworkThread", &endFlag);
	
	while(gTheGame.love.start == SDL_FALSE){//(2019.1.12)
		if(endFlag == 0)break;
	}
	Set_Object();
	SendObjectXY();
	
	
	severtime=SDL_GetTicks();
	/* メインイベントループ */
	while(endFlag){
	if(gTheGame.stts==GS_Playing){
		for(int i = 0; i < gTheGame.charaNum; i++){
            UpdateCharaStatus(&(gTheGame.chara[i]),0);
			//キャラの移動計算
            MoveCharaS(&(gTheGame.chara[i]));
			ChuzeItem(&(gTheGame.chara[i]),i);
        }
		for(int f = 0; f < gTheGame.ItemNum; f++){
            //アイテムの移動計算
            MoveItem(&(gTheGame.item[f]));
        }
		for(int m = 0; m < gTheGame.MokuNum; m++){
            //アイテムの移動計算
            MoveMokuzai(&(gTheGame.mokuzai[m]));
        }
		//拠点の移動計算(追加部分　2018/11.26)
		for(int s = 0; s < gTheGame.BaseNum; s++){
			MoveBase(&(gTheGame.base[s]));
		}
		//障害物の移動計算(追加部分　2018/12.8)
		for(int o = 0; o < gTheGame.ObjeNum; o++){
			MoveObject(&(gTheGame.object[o]));
		}
		for(int k = 0; k < gTheGame.FallNum; k++){
            //落とし穴の移動計算
            MoveFall(&(gTheGame.fall[k]));
        }
		for (int i = 0; i < gTheGame.charaNum; i++) {
				//キャラとアイテムの当たり判定
				for (int g = 0; g < gTheGame.ItemNum; g++) {
                    CollisionItem(&(gTheGame.chara[i]), &(gTheGame.item[g]),g,i);
                }
				//キャラと拠点の当たり判定
				for (int c = 0; c < gTheGame.BaseNum; c++) {
                    CollisionCharaBase(&(gTheGame.chara[i]), &(gTheGame.base[c]));
                }
				//キャラと落とし穴の当たり判定(2018.12.21)
				for (int m = 0; m < gTheGame.FallNum; m++) {
                   CollisionFall(&(gTheGame.chara[i]), &(gTheGame.fall[m]),i,m);
                }
				//キャラと木材の当たり判定(2018.12.21)
				for (int m = 0; m < gTheGame.MokuNum; m++) {
                   CollisionMokuzai(&(gTheGame.mokuzai[m]),&(gTheGame.chara[i]));
                }
        }
		for (int b = 0; b < gTheGame.ItemNum; b++) {
				//アイテムと拠点の当たり判定
                for (int r = 0; r < gTheGame.BaseNum; r++) {
                    CollisionBase(&(gTheGame.item[b]), &(gTheGame.base[r]),r,b);
                }
				//アイテムと障害物の当たり判定
				for (int s = 0; s < gTheGame.ObjeNum; s++) {
                    CollisionWood(&(gTheGame.item[b]), &(gTheGame.object[s]));
                }
		}
		//CD_Ghost→CS_Enableへの時間による変化(2018.12.9)
		for (int i = 0; i < gTheGame.charaNum; i++) {
			if(gTheGame.chara[i].stts == CS_Die){
				if(flag_ghost[i] == 0){//初回のみ更新
					previous_time[i] = SDL_GetTicks();
					flag_ghost[i] = 1;
				}
				
				current_time[i] = SDL_GetTicks(); //経過時間を整数型変数に格納
				if (current_time[i] > previous_time[i] + 5000){ //前のタイミングから1秒経過
					previous_time[i] = current_time[i]; //前のタイミングを現在で更新
					
					Ghost(&(gTheGame.chara[i]), i);
				}
			}else if(gTheGame.chara[i].stts == CS_Ghost){
				current_time[i] = SDL_GetTicks(); //経過時間を整数型変数に格納
				if (current_time[i] > previous_time[i] + 10000){ //前のタイミングから1秒経過
					previous_time[i] = current_time[i]; //前のタイミングを現在で更新
					Ghost(&(gTheGame.chara[i]), i);
					//UpdateCharaStatus(&(gTheGame.chara[i]));
					flag_ghost[i] = 0;
				}
			}else if(gTheGame.chara[i].stts == CS_Fallout){
				if(flag_ghost[i] == 0){//初回のみ更新
					previous_time[i] = SDL_GetTicks();
					flag_ghost[i] = 1;
				}
				
				current_time[i] = SDL_GetTicks(); //経過時間を整数型変数に格納
				if (current_time[i] > previous_time[i] + 5000){ //前のタイミングから1秒経過
					previous_time[i] = current_time[i]; //前のタイミングを現在で更新
					
					Ghost(&(gTheGame.chara[i]), i);
					flag_ghost[i] = 0;
				}
			}
			
		}
		//スコップ(2018.12.22)
		for (int b = 0; b < gTheGame.charaNum; b++) {
			if(gTheGame.chara[b].stts == CS_Attack1){
				int bi = gTheGame.chara[b].i_num;
				if(gTheGame.item[bi].type == IT_Scop){
					MakePitfall(&(gTheGame.chara[b]),&(gTheGame.item[bi]),b);
				}
			}
		}
		//全キャラの座標を送信
		SendHP();
		//全キャラのHPを送信(追加部分 2018/11/26)
		SendXY();
		SendStts();
		SendSttO();
		
		//(追加部分	2018/12/20)//////
		SendTime(severtime);
		if(gTheGame.Limit<=0 && gTheGame.stts==GS_Playing){
			Send_Rizult_Command();
			gTheGame.stts=GS_End;
		}
		}
		
		//ここまで////////////////
		SDL_Delay(10);
	}
	SDL_RemoveTimer(timer1);
	SDL_RemoveTimer(timer2);
	DestroySystemS();
	SDL_WaitThread(thr,NULL);
	/* 終了処理 */
	Ending();

	return 0;
}
//(追加部分・・ネットワークのイベントループの関数を追加)
/*****************************************************************
関数名  : NetworkEvent
機能    : ネットワークのイベントループ
引数    : void		*data		: 終了フラグ
出力    : スレッド終了時に0を返す
*****************************************************************/
static int NetworkEvent(void *data)
{
	int *endFlag;
	
	endFlag = (int*)data;
    while(*endFlag){
        *endFlag = SendRecvManager();
    }
	return 0;
}

//（追加関数  2018/12/8）
/*****************************************************************
関数名  : Timer1
機能    :タイマー処理1(キャラクターの無敵時間の更新)
引数    : interval    時間のタイマー
引数2   : parm    時間のパラメーター
出力    : スレッド終了時に0を返す
*****************************************************************/
Uint32 Timer1(Uint32 interval, void* pram)
{
    for (int i = 0; i < gTheGame.charaNum; i++) {
        if(gTheGame.chara[i].Damege_Flag!=0){
			gTheGame.chara[i].Damege_Flag++;
			if(gTheGame.chara[i].Damege_Flag>2){
				gTheGame.chara[i].Damege_Flag=0;
			}
		}
    }
	for (int k = 0; k < gTheGame.BaseNum; k++) {
        if(gTheGame.base[k].Damege_Flag!=0){
			gTheGame.base[k].Damege_Flag++;
			if(gTheGame.base[k].Damege_Flag>2){
				gTheGame.base[k].Damege_Flag=0;
			}
		}
    }
    return interval;
}

/*CS_Woodの切り替え(2019.1.12)*/
Uint32 Timer2(Uint32 interval, void* pram)
{
    for (int i = 0; i < gTheGame.charaNum; i++) {
        if(gTheGame.chara[i].stts == CS_Wood){
			if(gTheGame.chara[i].fwood != 0){
				gTheGame.chara[i].fwood++;
				if(gTheGame.chara[i].fwood > 2){
					gTheGame.chara[i].fwood = 0;
					gTheGame.chara[i].stts = gTheGame.chara[i].pre_stts;
 				}
			}
		}
    }
	for (int j = 0; j < gTheGame.ObjeNum; j++) {
        if(gTheGame.object[j].stts == OS_Shake){
			if(gTheGame.object[j].fwood != 0){
				gTheGame.object[j].fwood++;
				if(gTheGame.object[j].fwood > 2){
					gTheGame.object[j].fwood = 0;
					gTheGame.object[j].stts = OS_Enable;
 				}
			}
		}
    }
    return interval;
}