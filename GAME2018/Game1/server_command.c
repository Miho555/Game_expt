/*****************************************************************
ファイル名	: server_command.c
機能		: サーバーのコマンド処理
*****************************************************************/

#include"server_common.h"
#include"server_func.h"
#include"server_system.h"

static void SetIntData2DataBlock(void *data,int intData,int *dataSize);
static int SetfloatData(float floatData);
static float ChangefloatData(int intData);
static void SetCharData2DataBlock(void *data,char charData,int *dataSize);
static int GetRandomInt(int n);


/*****************************************************************
関数名	: ExecuteCommand
機能	: クライアントから送られてきたコマンドを元に，
		引き数を受信し，実行する
引数	: char	command	  : コマンド
		int	pos			: コマンドを送ったクライアント番号
出力	: プログラム終了コマンドが送られてきた時には0を返す．
		それ以外は1を返す
*****************************************************************/
int ExecuteCommand(char command,int pos)
{
    unsigned char	data[MAX_DATA];
    int         putkey;
    int         chara1,chara2;
    int			dataSize,intData,intData2,intData3;
    int         x,y;
	int			rizult;
    int			endFlag = 1;
	int 		i,j;
    float       rad;
    /* 引き数チェック */
    assert(0<=pos && pos<MAX_CLIENTS);

#ifndef NDEBUG
    printf("#####\n");
    printf("ExecuteCommand()\n");
    printf("Get command %c\n",command);
#endif
    switch(command){
        case END_COMMAND:
			dataSize = 0;
			/* コマンドのセット */
			SetCharData2DataBlock(data,command,&dataSize);
			
			/* 全ユーザーに送る */
			SendData(ALL_CLIENTS,data,dataSize);

			endFlag = 0;
			break;
	    case Mov_COMMAND:
            //クライアントから、クライアント番号を受信
		    RecvIntData(pos,&intData);
            //クライアントから、クライアントが押したキーを受信
            RecvIntData(pos,&intData2);

            rad = ChangefloatData(intData2);
           
            DirS(&(gTheGame.chara[pos]),rad);
			break;
       
        case Att_COMMAND:
            //fprintf(stderr,"mask_att[%d]\n",pos);
            //クライアントから、クライアント番号を受信
		    RecvIntData(pos,&intData);

            //クライアントから、攻撃パターンを受信
            RecvIntData(pos,&intData2);
            dataSize = 0;
            //aボタンを押したとき
            if(intData2==0){
                if(gTheGame.chara[pos].stts==CS_Item){
                    if(gTheGame.chara[pos].ActFlag>=1){
                         gTheGame.chara[pos].stts=CS_Act;
                    }
                    else{
                        gTheGame.chara[pos].stts=CS_Attack1;
                    }
                    gTheGame.chara[pos].pre_stts=CS_Item;
                }
                else if(gTheGame.chara[pos].stts==CS_Enable){
                    gTheGame.chara[pos].stts=CS_Act;
                    gTheGame.chara[pos].pre_stts=CS_Enable;
                }
            }
            //bボタンを押した
            else if(intData2 == 1){
                if(gTheGame.chara[pos].stts==CS_Enable){
                    gTheGame.chara[intData].stts=CS_Chuze;
                }
            }
            //a、bボタンを離した
            else if(intData2==2){
                if(gTheGame.chara[pos].stts==CS_Act){
                    gTheGame.chara[pos].stts=gTheGame.chara[pos].pre_stts;
                }
                else if(gTheGame.chara[pos].stts==CS_Attack1){
                    gTheGame.chara[pos].stts=gTheGame.chara[pos].pre_stts;
                }
            }
			break;
        case Mask_COMMAND:
            Recv_Mask(pos);
            fprintf(stderr,"mask_recive[%d]\n",pos);
            break;
        case Chu_COMMAND:
		    RecvIntData(pos,&intData);
            RecvIntData(pos,&intData2);
            gTheGame.chara[intData].chuzeitem=intData2+5*pos;
            break;

        case Start_COMMAND://(2019.1.5)
            //クライアントから、クライアント番号を受信
		    RecvIntData(pos,&intData);
            gTheGame.c_start++;
            fprintf(stderr,"c_start = %d\n",gTheGame.c_start);
            fprintf(stderr,"numnum = %d\n",gTheGame.numnum);
            dataSize = 0;

            if(gTheGame.c_start == gTheGame.numnum){//人数で変える
                /* コマンドのセット */
			    SetCharData2DataBlock(data,Start_COMMAND,&dataSize);
                 /* 全ユーザーに送る */
			    SendData(ALL_CLIENTS,data,dataSize);
                gTheGame.c_start = 10;
                gTheGame.love.start = SDL_TRUE;
            }
            break;
	    default:
			/* 未知のコマンドが送られてきた */
			fprintf(stderr,"0x%02x is not command!\n",command);
    }
    return endFlag;
}

/************************************************************************************
static
************************************************************************************/



/*****************************************************************
関数名	: SetIntData2DataBlock
機能	: int 型のデータを送信用データの最後にセットする
引数	: void		*data		: 送信用データ
		  int		intData		: セットするデータ
		  int		*dataSize	: 送信用データの現在のサイズ
出力	: なし
*****************************************************************/
static void SetIntData2DataBlock(void *data,int intData,int *dataSize)
{
    int tmp2;

    /* 引き数チェック */
    assert(data!=NULL);
    assert(0<=(*dataSize));

    tmp2 = htonl(intData);

    /* int 型のデータを送信用データの最後にコピーする */
    memcpy(data + (*dataSize),&tmp2,sizeof(int));
    /* データサイズを増やす */
    (*dataSize) += sizeof(int);
}


/*****************************************************************
関数名	: SetfloatData
機能	 : float 型のデータをint型に変更する関数
引数	 :  float	  floatdata	  : セットするデータ
出力	 : なし
*****************************************************************/
static int SetfloatData(float floatData)
{
    union IntAndFloat target;
    target.fval=floatData;
    return target.ival;
}




/*****************************************************************
関数名	: ChangefloatData
機能	: int型のデータをfloat 型のデータに変更する関数
引数	: int 	  intData	  : セットするデータ
出力	: なし
*****************************************************************/
static float ChangefloatData(int intData){
     union newFloat newtarget;
     newtarget.ival=intData;
     return newtarget.fval;
}


/*****************************************************************
関数名	: SetCharData2DataBlock
機能	: char 型のデータを送信用データの最後にセットする
引数	: void		*data		: 送信用データ
		  int		intData		: セットするデータ
		  int		*dataSize	: 送信用データの現在のサイズ
出力	: なし
*****************************************************************/
static void SetCharData2DataBlock(void *data,char charData,int *dataSize)
{
    /* 引き数チェック */
    assert(data!=NULL);
    assert(0<=(*dataSize));

    /* int 型のデータを送信用データの最後にコピーする */
    *(char *)(data + (*dataSize)) = charData;
    /* データサイズを増やす */
    (*dataSize) += sizeof(char);
}

/******************************************************************************************
自動的に送られる送信部分
******************************************************************************************/


/*****************************************************************
関数名	: SendXY
機能	: 全キャラのpoint、アイテムのpointの値を全クライアントに送信する関数
引数	: なし 
出力	: なし
*****************************************************************/
void SendXY(void){
    unsigned char	data[MAX_DATA];
    int			dataSize;
    int         i,j,k,n,m;
    int         x,y; 
    dataSize=0;
     /* コマンドのセット */
	SetCharData2DataBlock(data,Mov_COMMAND,&dataSize);
    for(i=0;i<MAX_CLIENTS;i++){
        /*プレイヤーのx座標を1バイトずつに変換*/
        x=SetfloatData(gTheGame.chara[i].point.x);
        /*プレイヤーのy座標を1バイトずつに変換*/
        y=SetfloatData(gTheGame.chara[i].point.y);
        SetIntData2DataBlock(data,x,&dataSize);
        SetIntData2DataBlock(data,y,&dataSize);
    }
    for(j=0;j<gTheGame.ItemNum;j++){
        /*プレイヤーのx座標を1バイトずつに変換*/
        x=SetfloatData(gTheGame.item[j].point.x);
        /*プレイヤーのy座標を1バイトずつに変換*/
        y=SetfloatData(gTheGame.item[j].point.y);
        SetIntData2DataBlock(data,x,&dataSize);
        SetIntData2DataBlock(data,y,&dataSize);
    }
    for(k=0;k<gTheGame.BaseNum;k++){
        /*プレイヤーのx座標を1バイトずつに変換*/
        x=SetfloatData(gTheGame.base[k].point.x);
        /*プレイヤーのy座標を1バイトずつに変換*/
        y=SetfloatData(gTheGame.base[k].point.y);
        SetIntData2DataBlock(data,x,&dataSize);
        SetIntData2DataBlock(data,y,&dataSize);
    }
    for(n=0;n<gTheGame.FallNum;n++){
        /*プレイヤーのx座標を1バイトずつに変換*/
        x=SetfloatData(gTheGame.fall[n].point.x);
        /*プレイヤーのy座標を1バイトずつに変換*/
        y=SetfloatData(gTheGame.fall[n].point.y);
        //fprintf(stderr,"->Player[%d]=(%d,%d)",i,x,y);
        SetIntData2DataBlock(data,x,&dataSize);
        SetIntData2DataBlock(data,y,&dataSize);
    }
    for(m=0;m<gTheGame.MokuNum;m++){
        x=SetfloatData(gTheGame.mokuzai[m].point.x);
        y=SetfloatData(gTheGame.mokuzai[m].point.y);
        SetIntData2DataBlock(data,x,&dataSize);
        SetIntData2DataBlock(data,y,&dataSize);
    }
    //fprintf(stderr,"OK_Sendxy\n");
    /* 全ユーザーに送る */
	SendData(ALL_CLIENTS,data,dataSize);
}


/*****************************************************************
関数名	: SendObjectXY
機能	: 障害物のpointの値を全クライアントに送信する関数
引数	: なし 
出力	: なし
*****************************************************************/
void SendObjectXY(void){
    unsigned char	data[MAX_DATA];
    int			dataSize;
    int         i;
    int         x,y;
    dataSize=0;
    /* コマンドのセット */
	SetCharData2DataBlock(data,Obje_COMMAND,&dataSize);
    for(i=0;i<gTheGame.ObjeNum;i++){
        /*プレイヤーのx座標を1バイトずつに変換*/
        x=SetfloatData(gTheGame.object[i].point.x);
        /*プレイヤーのy座標を1バイトずつに変換*/
        y=SetfloatData(gTheGame.object[i].point.y);
        //fprintf(stderr,"->Player[%d]=(%d,%d)",i,x,y);
        SetIntData2DataBlock(data,x,&dataSize);
        SetIntData2DataBlock(data,y,&dataSize);
    }
    /* 全ユーザーに送る */
	SendData(ALL_CLIENTS,data,dataSize);

}

/*****************************************************************
関数名	: SendHP
機能	: 全キャラ、拠点のHPの値を全クライアントに送信する関数
引数	: なし 
出力	: なし
*****************************************************************/
void SendHP(void)
{
    unsigned char	data[MAX_DATA];
    int			dataSize;
    int i,j;

    dataSize = 0;
    /* コマンドCのセット */
    SetCharData2DataBlock(data,HP_COMMAND,&dataSize);
    for(i=0;i<MAX_CLIENTS;i++){
        /* キャラのHPのセット */
        SetIntData2DataBlock(data,gTheGame.chara[i].hp,&dataSize);
        //fprintf(stderr,"chara[%d].HP=%d\n",i,gTheGame.chara[i].hp);
    }
    for(j=0;j<gTheGame.BaseNum;j++){
        /* キャラのHPのセット */
        SetIntData2DataBlock(data,gTheGame.base[j].hp_A,&dataSize);
        SetIntData2DataBlock(data,gTheGame.base[j].hp_B,&dataSize);
        //fprintf(stderr,"base[%d],HP(A)=%d,HP(B)=%d\n",j,gTheGame.base[j].hp_A,gTheGame.base[j].hp_B);
    }
     /* 全ユーザーに送る */
	SendData(ALL_CLIENTS,data,dataSize);
}


/*****************************************************************
関数名	: SendStts
機能	: 全キャラの状態を送信する関数
引数	: なし 
出力	: なし
*****************************************************************/
void SendStts(void)
{
    unsigned char	data[MAX_DATA];
    int			dataSize;
    int i,j,b;
    int stts;

    dataSize = 0;

     /* コマンドCのセット */
    SetCharData2DataBlock(data,Stts_COMMAND,&dataSize);

    //キャラクターの状態
    for(i=0;i<MAX_CLIENTS;i++){
        //移動しているときの状態
        if(gTheGame.chara[i].input.up==SDL_TRUE || gTheGame.chara[i].input.down==SDL_TRUE || 
            gTheGame.chara[i].input.left==SDL_TRUE || gTheGame.chara[i].input.right==SDL_TRUE){
                SetIntData2DataBlock(data,gTheGame.chara[i].Dir_Flag+8,&dataSize);//8方向(2019.1.5)
        }
        //止まっているときの状態
        else{
            SetIntData2DataBlock(data,gTheGame.chara[i].Dir_Flag,&dataSize);
        }
        //キャラのステータスの送信
        stts=gTheGame.chara[i].stts;
        SetIntData2DataBlock(data,stts,&dataSize);
        //木材の数
        SetIntData2DataBlock(data,gTheGame.chara[i].Mokuzai,&dataSize);
        //所持しているアイテム
        SetIntData2DataBlock(data,gTheGame.chara[i].i_num,&dataSize);
    }
    //アイテムの状態
    for(j=0;j<gTheGame.ItemNum;j++){
        if(gTheGame.item[j].stts==IS_Disable){
            SetIntData2DataBlock(data,0,&dataSize);
        }
        else if(gTheGame.item[j].stts==IS_Enable){
            SetIntData2DataBlock(data,1,&dataSize);
        }
        else if(gTheGame.item[j].stts==IS_Mine){
            SetIntData2DataBlock(data,2,&dataSize);
        }
        else if(gTheGame.item[j].stts==IS_Open){
            SetIntData2DataBlock(data,3,&dataSize);
        }
    }
    //秘密基地の状態
    for(b=0;b<gTheGame.BaseNum;b++){
        if(gTheGame.base[b].stts==BS_FLEE){
            SetIntData2DataBlock(data,2,&dataSize);
        }
        else if(gTheGame.base[b].stts==BS_Group1){
            SetIntData2DataBlock(data,3,&dataSize);
        }  
        else if(gTheGame.base[b].stts==BS_Group2){
            SetIntData2DataBlock(data,4,&dataSize);
        }
        else if(gTheGame.base[b].stts==BS_Group1_P){
            SetIntData2DataBlock(data,5,&dataSize);
        }
         else if(gTheGame.base[b].stts==BS_Group2_P){
            SetIntData2DataBlock(data,6,&dataSize);
        }

    }
    for(int m=0;m<gTheGame.MokuNum;m++){
        //木材のステータスの送信
        stts=gTheGame.mokuzai[m].stts;
        SetIntData2DataBlock(data,stts,&dataSize);
    }
    SetIntData2DataBlock(data,gTheGame.Group1_point,&dataSize);
    SetIntData2DataBlock(data,gTheGame.Group2_point,&dataSize);
    /* 全ユーザーに送る */
	SendData(ALL_CLIENTS,data,dataSize);
    
}


/*****************************************************************
関数名	: SendSttsO
機能	: 障害物の状態を送信する関数
引数	: なし 
出力	: なし
*****************************************************************/
void SendSttO(void)
{
    unsigned char	data[MAX_DATA];
    int			dataSize;
    int j;
    int stts;
     /* 引き数チェック */
    //assert(0<=pos && pos<MAX_CLIENTS);
    dataSize = 0;

     /* コマンドIのセット */
    SetCharData2DataBlock(data,SttsO_COMMAND,&dataSize);
    for(j=0;j<gTheGame.ObjeNum;j++){
        if(gTheGame.object[j].stts==OS_Disable){
            stts=0;
        }
        else if(gTheGame.object[j].stts==OS_Enable){
            stts=1;
        }
        else if(gTheGame.object[j].stts==OS_Die){
            stts=2;
        }
        else if(gTheGame.object[j].stts==OS_Shake){
            stts=3;
        }
        SetIntData2DataBlock(data,stts,&dataSize); 
    }
    /* 全ユーザーに送る */
	SendData(ALL_CLIENTS,data,dataSize);
    
}

//(2018/12/20)
/*****************************************************************
関数名	: SendTime
機能	: 時間を送信する送信する関数
引数	: なし 
出力	: なし
*****************************************************************/
void SendTime(unsigned int severtime)
{
    unsigned char	data[MAX_DATA];
    int			dataSize;
    int j;

    unsigned int limit = 180000;
    unsigned int current;
    dataSize = 0;

    current += SDL_GetTicks();
    gTheGame.Limit = limit - current+severtime;
    //fprintf(stderr,"last%ds,elapsed%ds\n",gTheGame.Limit/1000,current/1000);
     /* コマンドMのセット */
    SetCharData2DataBlock(data,Time_COMMAND,&dataSize);
    /*残り時間のセット*/
    SetIntData2DataBlock(data,gTheGame.Limit,&dataSize);
    /* 全ユーザーに送る */
	SendData(ALL_CLIENTS,data,dataSize);
    
}



/******************************************************************************************
条件を満たしたときに送られる送信部分
******************************************************************************************/


/*****************************************************************
関数名	: Send_BCol_Command
機能	: クライアントにキャラと拠点の当たり判定を描画させるために，
		サーバーにデータを送る
引数	: int		chara	    : キャラ番号
引数2 : int       base      　: 拠点の番号
引数3 : int       Flag      　: 衝突しているかしていないかを判断するフラグ
引数4 : int       Build       : 拠点を立てるか建てないかを判断するフラグ
出力	: なし
*****************************************************************/
void Send_BCol_Command(int chara,int base,int Flag,int Build)
{
    unsigned char	data[MAX_DATA];
    int			dataSize;

    dataSize = 0;
    /* コマンドBのセット */
    SetCharData2DataBlock(data,BCol_COMMAND,&dataSize);
    /* キャラ番号のセット */
    SetIntData2DataBlock(data,chara,&dataSize);
    /* 拠点のセット */
    SetIntData2DataBlock(data,base,&dataSize);
    /* フラグのセット */
    SetIntData2DataBlock(data,Flag,&dataSize);
    /* buildのセット */
    SetIntData2DataBlock(data,Build,&dataSize);
    /* 全ユーザーに送る */
	SendData(ALL_CLIENTS,data,dataSize);
}




/*****************************************************************
関数名	: Send_Rizult_Command
機能	: 勝敗判定の結果をクライアントに送信する
引数	: なし
出力	: なし
*****************************************************************/
void Send_Rizult_Command(void)
{
    unsigned char	data[MAX_DATA];
    int			dataSize;
    int         i;
    int         Win=0;
    int         Lose=0;

    //(追加部分・・・勝敗判定の判定処理を追加     2018/12/20)////
    if(gTheGame.Group1_point>gTheGame.Group2_point){
        Win=0;
        Lose=1;
    }
    else if(gTheGame.Group1_point<gTheGame.Group2_point){
        Win=1;
        Lose=0;
    }
    else{
        Win=2;
        Lose=2;       
    }
    ////ここまで/////////////////////////////////
    dataSize = 0;
    for(i=0;i<MAX_CLIENTS;i++){
        dataSize = 0;
        if(gTheGame.chara[i].group==Win){
            /* コマンドWのセット */
            SetCharData2DataBlock(data,Win_COMMAND,&dataSize);
            /* 勝利したユーザに送る */
	        SendData(i,data,dataSize);
        }
        else if(gTheGame.chara[i].group==Lose){
            /* コマンドWのセット */
            SetCharData2DataBlock(data,Los_COMMAND,&dataSize);
            /* 敗北したユーザに送る */
	        SendData(i,data,dataSize);
        }
        else{
            /* コマンドWのセット */
            SetCharData2DataBlock(data,Dra_COMMAND,&dataSize);
            /* 引き分けしたユーザに送る */
	        SendData(i,data,dataSize);
        }
    }
}


/*****************************************************************
関数名	:Recv_Mask
機能	: キャラクター、アイテム、拠点のマスクを受信する
引数	: int		pos	    : クライアント番号
出力	: なし
*****************************************************************/
void Recv_Mask(int pos){
    unsigned char	data[MAX_DATA];
    int			dataSize;

     /* 引き数チェック */
    assert(0<=pos && pos<MAX_CLIENTS);  

    //キャラのマスク
    for(int i=0;i<MAX_CLIENTS;i++){
        RecvIntData(pos,&gTheGame.chara[i].mask.h);
        RecvIntData(pos,&gTheGame.chara[i].mask.w);
        RecvIntData(pos,&gTheGame.chara[i].mask.x);
        RecvIntData(pos,&gTheGame.chara[i].mask.y);
        //fprintf(stderr,"chara[%d]:mask.h=%d,mask.w=%d.\n",i,gTheGame.chara[i].mask.h,gTheGame.chara[i].mask.w);
        //fprintf(stderr,"chara[%d]:mask.x=%d,mask.y=%d.\n",i,gTheGame.chara[i].mask.x,gTheGame.chara[i].mask.y);
    }
     //アイテムのマスク
    for(int j=0;j<gTheGame.ItemNum;j++){
        RecvIntData(pos,&gTheGame.item[j].mask.h);
        RecvIntData(pos,&gTheGame.item[j].mask.w);
        RecvIntData(pos,&gTheGame.item[j].mask.x);
        RecvIntData(pos,&gTheGame.item[j].mask.y);
        /*if(j==4){
            fprintf(stderr,"item[%d]:mask.h=%d,mask.w=%d.\n",4,gTheGame.item[4].mask.h,gTheGame.item[4].mask.w);
            fprintf(stderr,"item[%d]:mask.x=%d,mask.y=%d.\n",4,gTheGame.item[4].mask.x,gTheGame.item[4].mask.y);
        }*/
      
    }
     //拠点のマスク
    for(int k=0;k<gTheGame.BaseNum;k++){
       RecvIntData(pos,&gTheGame.base[k].mask.h);
       RecvIntData(pos,&gTheGame.base[k].mask.w);
       RecvIntData(pos,&gTheGame.base[k].mask.x);
       RecvIntData(pos,&gTheGame.base[k].mask.y);
       RecvIntData(pos,&gTheGame.base[k].mask2.h);
       RecvIntData(pos,&gTheGame.base[k].mask2.w);
       RecvIntData(pos,&gTheGame.base[k].mask2.x);
       RecvIntData(pos,&gTheGame.base[k].mask2.y);
       RecvIntData(pos,&gTheGame.base[k].mask3.h);
       RecvIntData(pos,&gTheGame.base[k].mask3.w);
       RecvIntData(pos,&gTheGame.base[k].mask3.x);
       RecvIntData(pos,&gTheGame.base[k].mask3.y);
       RecvIntData(pos,&gTheGame.base[k].mask4.h);
       RecvIntData(pos,&gTheGame.base[k].mask4.w);
       RecvIntData(pos,&gTheGame.base[k].mask4.x);
       RecvIntData(pos,&gTheGame.base[k].mask4.y);
       /*if(k==0){
            fprintf(stderr,"base[%d]:mask.h=%d,mask.w=%d.\n",0,gTheGame.base[0].mask.h,gTheGame.base[0].mask.w);
            fprintf(stderr,"base[%d]:mask.x=%d,mask.y=%d.\n",0,gTheGame.base[0].mask.x,gTheGame.base[0].mask.y);
       }*/
    }
    //落とし穴のマスク
    for(int f=0;f<gTheGame.FallNum;f++){
        //fprintf(stderr,"->Player[%d]=(%d,%d)",i,x,y);
       RecvIntData(pos,&gTheGame.fall[f].mask.h);
       RecvIntData(pos,&gTheGame.fall[f].mask.w);
       RecvIntData(pos,&gTheGame.fall[f].mask.x);
       RecvIntData(pos,&gTheGame.fall[f].mask.y);
    }  
     
}




/*****************************************************************
関数名	: SendGhost
機能	: CS_GhostかCS_Enableの状態変化を送る
引数	: なし 
出力	: なし
*****************************************************************/
void SendGhost(int pos,int sttsG)
{
    unsigned char	data[MAX_DATA];
    int			dataSize;
    
     /* 引き数チェック */
    assert(0<=pos && pos<MAX_CLIENTS);
    dataSize = 0;
    /* コマンドCのセット */
    SetCharData2DataBlock(data,Ghost_COMMAND,&dataSize);
    for(int i=0;i<MAX_CLIENTS;i++){
        if(sttsG>0){
            SetIntData2DataBlock(data,pos,&dataSize);
            SetIntData2DataBlock(data,sttsG,&dataSize);
        }
    }
    /* 全ユーザーに送る */
	SendData(ALL_CLIENTS,data,dataSize);
    
}


/*****************************************************************
関数名	: Send_Fall_Command
機能	: 落とし穴を作成するためにクライアントにデータを送る
引数	: なし 
出力	: なし
*****************************************************************/
void Send_Fall_Command(int chara,int fall)
{
    unsigned char	data[MAX_DATA];
    int			dataSize;


    dataSize = 0;
    /* コマンドLのセット */
    SetCharData2DataBlock(data,Fall_COMMAND,&dataSize);
    /* キャラ番号のセット */
    SetIntData2DataBlock(data,chara,&dataSize);
    /* アイテムのセット */
    SetIntData2DataBlock(data,fall,&dataSize);
    /* 全ユーザーに送る */
	SendData(ALL_CLIENTS,data,dataSize);
}


/*****************************************************************
関数名	: Send_Catch_Command
機能	: クライアントに落とし穴の当たり判定を描画させるために，
		クライアントにデータを送る
引数	: int		chara	    : キャラ番号
引数2 : int       fall      　: 落とし穴の番号
引数3 : int       item      　: アイテムの番号
出力	: なし
*****************************************************************/
void Send_Catch_Command(int chara,int fall)
{
    unsigned char	data[MAX_DATA];
    int			dataSize;


    dataSize = 0;
    /* コマンドLのセット */
    SetCharData2DataBlock(data,Catch_COMMAND,&dataSize);
    /* キャラ番号のセット */
    SetIntData2DataBlock(data,chara,&dataSize);
    /* 落とし穴のセット */
    SetIntData2DataBlock(data,fall,&dataSize);
    /* 全ユーザーに送る */
	SendData(ALL_CLIENTS,data,dataSize);
}




void Send_Messege_Command(int group,int change)
{
    unsigned char	data[MAX_DATA];
    int			dataSize;
    int i;
    dataSize = 0;
    for(i=0;i<MAX_CLIENTS;i++){
        dataSize = 0;
        if(gTheGame.chara[i].group==group){
            /* コマンドMのセット */
            SetCharData2DataBlock(data,Msg_COMMAND,&dataSize);
            /*攻撃されているかされていないか*/
             SetIntData2DataBlock(data,change,&dataSize);
            /* 攻撃されているグループに送る */
	        SendData(i,data,dataSize);
        }
    }
}