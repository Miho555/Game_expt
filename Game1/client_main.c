/*****************************************************************
ファイル名	: client_main.c
機能		: クライアントのメインルーチン
*****************************************************************/

//#include<SDL/SDL.h>
#include"common.h"
#include"client_func.h"
#include "system.h"
//（変更部分・・・アニメーションを変更するタイマー関数の追加  2018/11/7）
static Uint32 Timer1(Uint32 interval, void* pram);
static Uint32 Timer2(Uint32 interval, void* pram);

static int NetworkEvent(void *data);
static int WindowEvent(void *data);
wiimote_t wiimote       = WIIMOTE_INIT;        // Wiiリモコンの状態格納用
wiimote_report_t report = WIIMOTE_REPORT_INIT; // レポートタイプ用
Mix_Music *music;//2019.1.11
Mix_Music *music2;//2019.1.11
Mix_Chunk *WinBGM;
int joyx,joyy;

int main(int argc,char *argv[])
{
    int		num;
    char	name[MAX_CLIENTS][MAX_NAME_SIZE];
    int		endFlag=1;
    char	localHostName[]="localhost";
    char	*serverName;
    int		clientID;
    int     i;

	SDL_Thread	*thr1;
    SDL_Thread	*thr2;


    /* 引数チェック*/
    if(argc == 2){
    	serverName = localHostName;
    }
    else if(argc == 3){
    	serverName = argv[1];
    }
    else if(argc>3){
		fprintf(stderr, "Usage: %s, Cannot find a Server Name.\n", argv[0]);
		return -1;
    }

    /* サーバとの接続*/
    if(SetUpClient(serverName,&clientID,&num,name)==-1){
		fprintf(stderr,"setup failed : SetUpClient\n");
		return -1;
	}
    /* SDLの初期化 */
	if(SDL_Init(SDL_INIT_VIDEO) < 0) {
		fprintf(stderr,"failed to initialize SDL.\n");
		return -1;
	}
    /*システムの初期化*/
    if (InitSystem() < 0) {
        fprintf(stderr,"failed to initialize System\n");
        return -1;
    }
    /* ウィンドウの初期化 */
	if(InitWindows(clientID,num,name)==-1){
		fprintf(stderr,"setup failed : InitWindows\n");
		return -1;
	}
     /*タイトル音楽(2019.1.11)*/
    if(Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 2, 1526407) < 0) {
		printf("failed to initialize SDL_mixer.\n");
	    return -1;
	}
    /*タイトル音楽(2019.1.11)*/
    //if((music = Mix_LoadMUS("game_maoudamashii_5_village10.ogg")) == NULL) {
    if((music = Mix_LoadMUS("gameBGM1.ogg")) == NULL) {
		printf("failed to load music.\n");
	    return -1;
	}
    if((music2 = Mix_LoadMUS("gameTitle.ogg")) == NULL) {
		printf("failed to load music.\n");
	    return -1;
	}
    if((WinBGM = Mix_LoadWAV("WinBGM.ogg")) == NULL) {
		printf("failed to load music.\n");
	    return -1;
	}
     //2018.11.9 wiimote変更
    if (argc > 3) { // Wiiリモコン識別情報がコマンド引数で与えられなければ
        printf("Designate the wiimote ID to the application.\n");
        exit(1);
    }

    // Wiiリモコンの接続（１つのみ）
    if (wiimote_connect(&wiimote, argv[2]) < 0) { // コマンド引数に指定したWiiリモコン識別情報を渡して接続
        printf("unable to open wiimote: %s\n", wiimote_get_error());
        exit(1);
    }
    /* タイマー */
    SDL_TimerID timer1 = SDL_AddTimer(250, Timer1, NULL);
    if(timer1==0){
        fprintf(stderr,"Not timer1\n");
        return -1;
    }
    SDL_TimerID timer2 = SDL_AddTimer(100, Timer2, NULL);
    if(timer2==0){
        fprintf(stderr,"Not timer2\n");
        return -1;
    }

    /* ネットワークイベント処理スレッドの作成*/
	thr1 = SDL_CreateThread(NetworkEvent,"NetworkThread", &endFlag);

    thr2 = SDL_CreateThread(WindowEvent,"WindEveThread", &clientID);
	
    gTheGame.c_ID = clientID;

    Mix_PlayMusic(music2, -1);
    while(gTheGame.input.start == SDL_FALSE){
        TitleWindow();  
        if(endFlag == 0)break;//2019.1.12
    }
   Mix_HaltMusic();
   Mix_FadeOutMusic(2000);
    while(gTheGame.input.start == SDL_TRUE && gTheGame.input.ready == SDL_FALSE){
        WaitWindow();
        if(endFlag == 0)break;  
    }

    while(gTheGame.input.ready == SDL_TRUE && gTheGame.input.wait == SDL_FALSE){
        WaitWindow(); 
        if(endFlag == 0)break;
    }
    Mix_PlayMusic(music, -1);
    SendMask();
	/* メインイベントループ*/
    while(endFlag==1 && gTheGame.input.ready == SDL_TRUE && gTheGame.input.wait == SDL_TRUE && gTheGame.input.start == SDL_TRUE){
        //fprintf(stderr,"main_client\n");
		//WindowEvent(num,clientID);
        for(int i = 0; i < gTheGame.charaNum; i++){
            //キャラクターの状態変更
            UpdateCharaStatus(&(gTheGame.chara[i]));
            //キャラの移動計算
            MoveChara(&(gTheGame.chara[i]));
        }
        for(int f = 0; f < gTheGame.ItemNum; f++){
            //アイテムの移動計算
            MoveItem(&(gTheGame.item[f]));
        }
        for(int m = 0; m < gTheGame.MokuNum; m++){
            //木材の移動計算
            MoveMokuzai(&(gTheGame.mokuzai[m]));
        }
        for(int s = 0; s < gTheGame.baseNum; s++){
            //拠点の移動計算
            MoveBase(&(gTheGame.base[s]));
        }
         for(int o = 0; o < gTheGame.ObjeNum; o++){
            //障害物の移動計算
            MoveObje(&(gTheGame.object[o]));
        }
        for(int k = 0; k < gTheGame.FallNum; k++){
            //落とし穴の移動計算(2018.12.22)
            MoveFall(&(gTheGame.fall[k]));
        }
        //マスクを送信する
      
        RenderWindow();
		//SDL_Delay(10);
        if(gTheGame.stts==GS_End && gTheGame.EndFlag==0){
            Mix_HaltMusic();
            Mix_PlayChannel(1, WinBGM, 0); 
            gTheGame.EndFlag=1;
        }
    }
    
    /* 終了処理*/
    SDL_RemoveTimer(timer1);
    SDL_RemoveTimer(timer2);
	SDL_WaitThread(thr1,NULL);
    SDL_WaitThread(thr2,NULL);
    Mix_FreeMusic(music);
    Mix_FreeMusic(music2);
    Mix_FreeChunk(WinBGM);
    Mix_CloseAudio();
    DestroySystem();
	DestroyWindow();
	CloseSoc();

    return 0;
}

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

//（変更部分・・・アニメーションを変更するタイマー関数の追加  2018/11/7）
/*****************************************************************
関数名  : Timer1
機能    :タイマー処理1(アニメーションの更新)
引数    : interval    時間のタイマー
引数2   : parm    時間のパラメーター
出力    : スレッド終了時に0を返す
*****************************************************************/
Uint32 Timer1(Uint32 interval, void* pram)
{
    //キャラのアニメーション
    for (int i = 0; i < gTheGame.charaNum; i++) {
        gTheGame.chara[i].src.x = (gTheGame.chara[i].src.x + gTheGame.chara[i].src.w) % (gTheGame.chara[i].src.w * gTheGame.anipatNum[gTheGame.chara[i].type]);
        SDL_Rect r             = { gTheGame.chara[i].src.x,gTheGame.chara[i].src.y, gTheGame.chara[i].rect.w, gTheGame.chara[i].rect.h };
        gTheGame.chara[i].mask = MeasureMask(gTheGame.chara[i].s, r);
        gTheGame.chara[i].src5.x = (gTheGame.chara[i].src5.x + gTheGame.chara[i].src5.w) % (gTheGame.chara[i].src5.w * gTheGame.anipatNum[gTheGame.chara[i].type]);
        gTheGame.chara[i].src6.x = (gTheGame.chara[i].src6.x + gTheGame.chara[i].src6.w) % (gTheGame.chara[i].src6.w * gTheGame.anipatNum[gTheGame.chara[i].type]);
        gTheGame.chara[i].src7.x = (gTheGame.chara[i].src7.x + gTheGame.chara[i].src7.w) % (gTheGame.chara[i].src7.w * gTheGame.anipatNum[gTheGame.chara[i].type]);
        gTheGame.chara[i].src8.x = (gTheGame.chara[i].src8.x + gTheGame.chara[i].src8.w) % (gTheGame.chara[i].src8.w * gTheGame.anipatNum[gTheGame.chara[i].type]);
        gTheGame.chara[i].src9.x = (gTheGame.chara[i].src9.x + gTheGame.chara[i].src9.w) % (gTheGame.chara[i].src9.w * gTheGame.anipatNum[gTheGame.chara[i].type]); 
        gTheGame.chara[i].src10.x = (gTheGame.chara[i].src10.x + gTheGame.chara[i].src10.w) % (gTheGame.chara[i].src10.w * gTheGame.anipatNum[gTheGame.chara[i].type]);
        gTheGame.chara[i].src11.x = (gTheGame.chara[i].src11.x + gTheGame.chara[i].src11.w) % (gTheGame.chara[i].src11.w * gTheGame.anipatNum[gTheGame.chara[i].type]); 
        gTheGame.chara[i].src12.x = (gTheGame.chara[i].src12.x + gTheGame.chara[i].src12.w) % (gTheGame.chara[i].src12.w * gTheGame.anipatNum[gTheGame.chara[i].type]); 
        gTheGame.chara[i].src13.x = (gTheGame.chara[i].src13.x + gTheGame.chara[i].src13.w) % (gTheGame.chara[i].src13.w * gTheGame.anipatNum[gTheGame.chara[i].type]); 
    }
  
    //アイテムのアニメーション
    for(int j=0;j<gTheGame.ItemNum;j++){
        gTheGame.item[j].src.x = (gTheGame.item[j].src.x + gTheGame.item[j].src.w) % (gTheGame.item[j].src.w * gTheGame.anipatNum_Item[gTheGame.item[j].type]);
        SDL_Rect r             = { gTheGame.item[j].src.x,gTheGame.item[j].src.y, gTheGame.item[j].rect.w, gTheGame.item[j].rect.h };
        gTheGame.item[j].mask = MeasureMask(gTheGame.item[j].sd, r);
    }
    //木のアニメーション
    for(int j=0;j<gTheGame.ObjeNum;j++){
        gTheGame.object[j].src.x = (gTheGame.object[j].src.x + gTheGame.object[j].src.w) % (gTheGame.object[j].src.w * 4);
    }

    gTheGame.message[11].src.x=(gTheGame.message[11].src.x + gTheGame.message[11].src.w) % (gTheGame.message[11].src.w * 4);
    gTheGame.message[15].src.x=(gTheGame.message[15].src.x + gTheGame.message[15].src.w) % (gTheGame.message[15].src.w * 4);    
    //aボタンの時間
    if(gTheGame.aflag != 0){
        gTheGame.aflag++;
        if(gTheGame.aflag > 3){
            gTheGame.aflag = 0;
        }
    }
    return interval;
}

//（追加関数  2018/12/8）
/*****************************************************************
関数名  : Timer2
機能    :タイマー処理2(アイテム選択の更新)
引数    : interval    時間のタイマー
引数2   : parm    時間のパラメーター
出力    : スレッド終了時に0を返す
*****************************************************************/
Uint32 Timer2(Uint32 interval, void* pram)
{
    if(gTheGame.chuzeFlag!=0){
		gTheGame.chuzeFlag++;
		if(gTheGame.chuzeFlag>2){
			gTheGame.chuzeFlag=0;
		}
	}
    return interval;
}

/*****************************************************************
関数名  : WindowEvent
機能    :Wiiリモコンの状態を取得するイベントループ
引数    :*data    clientIDの番号
出力    : スレッド終了時に0を返す
*****************************************************************/
static int WindowEvent(void *data)
{
	SDL_Event event;

    int *clientID;
    int joyx_past;
 
    clientID = (int*)data;
    fprintf(stderr,"iD = %d\n",*clientID);

    //2018.11.9
    while (wiimote_is_open(&wiimote)) {

        //2018.11.12 画面暗くならないための呼び出し
        if (SDL_PollEvent(&event)) {
            }
        // Wiiリモコンの状態を取得・更新する
		if (wiimote_update(&wiimote) < 0) {
			wiimote_disconnect(&wiimote);
			break;
		}

        joyx = wiimote.ext.nunchuk.joyx;
        joyy = wiimote.ext.nunchuk.joyy;

        if(gTheGame.input.start == SDL_TRUE && gTheGame.input.wait == SDL_TRUE && 
            gTheGame.input.ready == SDL_TRUE){
                Radian(&(gTheGame.chara[*clientID]),joyx,joyy);
            }
        
    
        if (wiimote.keys.home || event.type == SDL_QUIT) {
            SendEndCommand();
            wiimote_disconnect(&wiimote); // Wiiリモコンとの接続を解除
            return 0;
        }
        if(wiimote.keys.a){
            if(gTheGame.input.start == SDL_FALSE){//2019.1.11
                gTheGame.input.start = SDL_TRUE;
                gTheGame.aflag = 1;
            }
            else if(gTheGame.input.start == SDL_TRUE && gTheGame.aflag == 0 && 
            gTheGame.input.ready == SDL_FALSE){
                Send_Start_Command(*clientID);
                gTheGame.input.ready = SDL_TRUE;
            }
            else  if(gTheGame.input.start == SDL_TRUE &&  gTheGame.input.wait == SDL_TRUE && gTheGame.aflag == 0 && 
            gTheGame.input.ready == SDL_TRUE){
                Send_ATT_Command(*clientID,0);
            }
        }
        else if(wiimote.keys.b){ //bボタンが押された時、取得
            if(gTheGame.input.start == SDL_TRUE && gTheGame.input.wait == SDL_TRUE && 
            gTheGame.input.ready == SDL_TRUE){
                Send_ATT_Command(*clientID,1);
            }
            
        }
        else{
            if(gTheGame.input.start == SDL_TRUE && gTheGame.input.wait == SDL_TRUE && 
            gTheGame.input.ready == SDL_TRUE){
                Send_ATT_Command(*clientID,2);
            }
        }

        if(gTheGame.input.start == SDL_TRUE && gTheGame.input.wait == SDL_TRUE && 
            gTheGame.input.ready == SDL_TRUE){
                if(gTheGame.chuzeFlag==0){
            if(wiimote.keys.left){
                Send_Chuze_Command(*clientID,1);
            }
            else if(wiimote.keys.right){
                Send_Chuze_Command(*clientID,2);
            }   
            else{
                Send_Chuze_Command(*clientID,0);
            }
            gTheGame.chuzeFlag=1;
        }
            }
        

        if(wiimote.keys.up){
            gTheGame.hideFlag=1;
            gTheGame.message[12].stts=MS_Disable;
        }
        else if(wiimote.keys.down){
            gTheGame.hideFlag=0;
            gTheGame.message[12].stts=MS_Enable;
        }
      
    }   
}