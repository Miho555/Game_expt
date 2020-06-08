/*****************************************************************
ファイル名	: client_win.c
機能		: クライアントのユーザーインターフェース処理
*****************************************************************/

//#include<SDL/SDL.h>
#include"common.h"
#include"client_func.h"
#include "system.h"
#include<SDL2/SDL_image.h>
#include<SDL2/SDL2_gfxPrimitives.h>
#include <SDL2/SDL_ttf.h>
/* データファイルパス */
static char gMapDataFile[] = "map2.data";

/* 画像ファイルパス */
//背景
static char gBgImgFile[]           = "yosumi.png";//背景

static char gMapImgFile[] = "map1.png";

/* フォント関連 */
static char *gFontFile[] = {
    "/usr/share/fonts/truetype/freefont/FreeMonoBold.ttf",
    "Cherrybomb.ttf"
};

static char *gMsgStr[8] = { " ", "0123456789", "-", "point", "0123456789", "×", "0123456789", "-" };

static const SDL_Color gWhite = { 255, 255, 255, 255 };
static const SDL_Color gBlack = { 0, 0, 0, 255 };

/*キャラ*/
static char *gCharaImgFile[CT_NUM] = {"girl2walk.png","girl1walk.png","girl4walk.png","girl3walk.png"};
/*キャラの動作*/
static char *gCharaitemImgFile[CT_NUM] = {"pico_walk2.png","pico_walk.png","pico_walk6.png","pico_walk5.png"};
static char *gCharaitem2ImgFile[CT_NUM] = {"ono_walk2.png","ono_walk.png","ono_walk4.png","ono_walk3.png"};
static char *gCharaitem3ImgFile[CT_NUM] = {"kasa_walk2.png","kasa_walk1.png","kasa_walk4.png","kasa_walk3.png"};
static char *gCharaitem4ImgFile[CT_NUM] = {"scop2.png","scop1.png","scop4.png","scop3.png"};
static char *gCharaattackImgFile[CT_NUM] =  {"pico_atta2.png","pico_atta.png","pico_atta6.png","pico_atta5.png"};
static char *gCharaattack2ImgFile[CT_NUM] = {"ono_atta2.png","ono_atta.png","ono_atta4.png","ono_atta3.png"};
static char *gCharaattack3ImgFile[CT_NUM] = {"kasa_atta2.png","kasa_atta1.png","kasa_atta4.png","kasa_atta3.png"};
static char *gCharaDieImgFile[CT_NUM] = {"sleep2.png","sleep.png","sleep4.png","sleep3.png"};
static char *gGFallImgFile[CT_NUM] = {"fallperson2.png","fallperson.png","fallperson4.png","fallperson3.png"};
static char *gShakeImgFile[CT_NUM] = {"shake2.png","shake1.png","shake4.png","shake3.png"};//(2019.1.12)揺らす
static char *gRizaltImgFile[CT_NUM] = {"rizult2.png","rizult1.png","rizult4.png","rizult3.png"};//(2019.1.12)揺らす

/*アイテム*/
static char *gItemImgFile[IT_NUM] = {"pico_mask1.png","pico_mask2.png","pico_mask2.png","pico_mask2.png","mokuzai1.png"};

//拠点
static char *gBaseImgFile[2] ={"base.png","base2.png"};
//落とし穴
static char *gItemImgFile2[1] = {"fall.png"};
//ゴースト
static char *gGhostImgFile[1] = {"ghosts.png"};
//木
static char *gObjectImgFile[OT_NUM] = {"ki1.png"};
//木材
static char *gMokuzaiImgFile[1] = {"mokuzai1.png"};

//メッセージ
static char *gMessageImgFile[MS_NUM] = {"win1.png","lose1.png","draw.png","chara_head2.png","kazu1.png",
"kan.png","chara_head.png","Fuchi2.png","itemSelect.png","heart.png","heart2.png","pinti.png","Kanban.png"
,"ki.png","winback.png","load1.png","mini1.png","mini2.png","mini3.png","mini4.png","mini5.png" ,"sun.png" ,"sun2.png"};


//タイトル、ロード
static char TitleImgTitleFile[]   = "title1.png";//(2019.1.7)
static char WaitImgTitleFile[]    = "load.png";//(2019.1.11)
//static char WINImgTitleFile[]    = "winback.png";//(2019.1.11)
//関数
static void PlaceFall(void);
static void PlaceItem(void);
static void Place_Move(void);
static void PlaceMokuzai(void);
static int MakeMap(void);
static void PlaceMassege(void);
static void makeMiniMap(void);
static void Display_HP(void);
static void Display_Mokuzai(void);
static void Display_ItemSelect(void);
static void Display_point(void);
int MakeMessage(void);
static void Display_BaseHP(void);
static void Display_Limit(int Limit_Time);
static void Display_charahead(void);
static void RezultWindow(void);


//map関数
long map(long x, long in_min, long in_max, long out_min, long out_max)
{
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

/*****************************************************************
関数名	: InitWindows
機能	: メインウインドウの表示、設定
引数	: int	clientID		: クライアント番号
		int	num				: 全クライアント数
出力	: 正常に設定できたとき0，失敗したとき-1
*****************************************************************/
int InitWindows(int clientID,int num,char name[][MAX_NAME_SIZE])
{
	int i;
    int f,k,v;
    int n=0,nb=0;
    /* 引き数チェック */
    assert(0<num && num<=MAX_CLIENTS);
	
	/*image初期化*/
	if (IMG_INIT_PNG != IMG_Init(IMG_INIT_PNG)) {
        fprintf(stderr,"failed to initialize SDL_Image\n");
		return -1;
    }
	
	/* メインのウインドウを作成する */
	gTheGame.window=SDL_CreateWindow("GAME", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
    WD_Width * MAP_ChipSize, WD_Height * MAP_ChipSize, 0);
	if(gTheGame.window == NULL){
		fprintf(stderr,"failed to initialize Window\n");
		return -1;
	}
    gTheGame.render = SDL_CreateRenderer(gTheGame.window, -1, 0);
    if(gTheGame.render == NULL){
		fprintf(stderr,"failed to initialize render\n");
		return -1;
	}
	fprintf(stderr,"OK1\n");


    /*タイトル作成*/
    SDL_Surface *sb = IMG_Load(TitleImgTitleFile);
    if (sb == NULL){
        fprintf(stderr,"NO title\n");
		return -1;
    }
    gTheGame.title       = SDL_CreateTextureFromSurface(gTheGame.render, sb);
    SDL_FreeSurface(sb);
    if (gTheGame.title == NULL) {
        fprintf(stderr,"NO title\n");
		return -1;
    }



    /*ロード作成2019.1.11*/
    SDL_Surface *sb2 = IMG_Load(WaitImgTitleFile);
    if (sb2 == NULL){
        fprintf(stderr,"NO Wait\n");
		return -1;
    }
    gTheGame.waitwin       = SDL_CreateTextureFromSurface(gTheGame.render, sb2);
    SDL_FreeSurface(sb2);
    if (gTheGame.waitwin == NULL) {
        fprintf(stderr,"NO Wait\n");
		return -1;
    }


	/*キャラクターの作成*/
	SDL_Surface *s;
   
	s= IMG_Load(gBgImgFile);
	//画像が読み込まなければ、エラー表示
	 if (s == NULL){
		  fprintf(stderr,"failed to open background image.\n");
		  return -1;
	 }
	 //背景の貼り付け
    gTheGame.bg       = SDL_CreateTextureFromSurface(gTheGame.render, s);
    SDL_FreeSurface(s);
    if(gTheGame.bg == NULL){
        fprintf(stderr,"Not Bg");
    }


	//キャラ/////////////////////////////////////////////////////////////////////////////////////
    
    /*      通常時     */
    for(i = 0; i < gTheGame.charaNum; i++){
		if(NULL == (s = IMG_Load(gCharaImgFile[i]))){
			fprintf(stderr,"failed to open character[%d] image.\n",i);
            return -1;
		}
        if(NULL == (gTheGame.chara[i].s = IMG_Load(gCharaImgFile[i]))){
			fprintf(stderr,"failed to open character image.\n");
            return -1;
		}
		gTheGame.chara[i].image = SDL_CreateTextureFromSurface(gTheGame.render, s);
		if (gTheGame.chara[i].image == NULL) {
            SDL_FreeSurface(s);
            fprintf(stderr,"failed to initialize chara_image.\n");
			return -1;
        }
		//キャラのサイズ
        gTheGame.chara[i].src.w=s->w/gTheGame.anipatNum[gTheGame.chara[i].type];
        gTheGame.chara[i].src.h=s->h/16;
        gTheGame.chara[i].rect.h  = gTheGame.chara[i].src.h;
        gTheGame.chara[i].rect.w  = gTheGame.chara[i].src.w;
        gTheGame.chara[i].src.y=0;
       
        /* 画像のマスクから当たり判定用矩形の算出 */
        SDL_Rect r             = { 0,0 , gTheGame.chara[i].rect.w, gTheGame.chara[i].rect.h };
        gTheGame.chara[i].mask = MeasureMask(s, r);

    	SDL_FreeSurface(s);
	} 

    /*      ピコピコハンマーを装備しているとき        */
    for(i = 0; i < gTheGame.charaNum; i++){
		if(NULL == (s = IMG_Load(gCharaitemImgFile[i]))){
			fprintf(stderr,"failed to open character image.\n");
            return -1;
		}
		gTheGame.chara[i].image6 = SDL_CreateTextureFromSurface(gTheGame.render, s);
		if (gTheGame.chara[i].image6 == NULL) {
            SDL_FreeSurface(s);
            fprintf(stderr,"failed to initialize chara_image.\n");
			return -1;
        }
        gTheGame.chara[i].src6.w=216;
        gTheGame.chara[i].src6.h=216;
        gTheGame.chara[i].rect6.h  = gTheGame.chara[i].src6.h;
        gTheGame.chara[i].rect6.w  = gTheGame.chara[i].src6.w;
        
    	SDL_FreeSurface(s);
	} 


    /*      ピコピコハンマーでの攻撃        */
   for(i = 0; i < gTheGame.charaNum; i++){
		if(NULL == (s = IMG_Load(gCharaattackImgFile[i]))){
			fprintf(stderr,"failed to open character image.\n");
            return -1;
		}
		gTheGame.chara[i].image7 = SDL_CreateTextureFromSurface(gTheGame.render, s);
		if (gTheGame.chara[i].image7 == NULL) {
            SDL_FreeSurface(s);
            fprintf(stderr,"failed to initialize chara_image.\n");
			return -1;
        }
        gTheGame.chara[i].src7.w=216;
        gTheGame.chara[i].src7.h=216;
        gTheGame.chara[i].rect7.h  = gTheGame.chara[i].src7.h;
        gTheGame.chara[i].rect7.w  = gTheGame.chara[i].src7.w;

    	SDL_FreeSurface(s);
	}
      /*      傘を装備しているとき        */
    for(i = 0; i < gTheGame.charaNum; i++){
		if(NULL == (s = IMG_Load(gCharaitem3ImgFile[i]))){
			fprintf(stderr,"failed to open character image.\n");
            return -1;
		}
		gTheGame.chara[i].image10 = SDL_CreateTextureFromSurface(gTheGame.render, s);
		if (gTheGame.chara[i].image10 == NULL) {
            SDL_FreeSurface(s);
            fprintf(stderr,"failed to initialize chara_image.\n");
			return -1;
        }
        gTheGame.chara[i].src10.w=216;
        gTheGame.chara[i].src10.h=216;
        gTheGame.chara[i].rect10.h  = gTheGame.chara[i].src10.h;
        gTheGame.chara[i].rect10.w  = gTheGame.chara[i].src10.w;
        
    	SDL_FreeSurface(s);
	} 

    /*      傘での攻撃        */
   for(i = 0; i < gTheGame.charaNum; i++){
		if(NULL == (s = IMG_Load(gCharaattack3ImgFile[i]))){
			fprintf(stderr,"failed to open character image.\n");
            return -1;
		}
		gTheGame.chara[i].image11 = SDL_CreateTextureFromSurface(gTheGame.render, s);
		if (gTheGame.chara[i].image11 == NULL) {
            SDL_FreeSurface(s);
            fprintf(stderr,"failed to initialize chara_image.\n");
			return -1;
        }
        gTheGame.chara[i].src11.w=216;
        gTheGame.chara[i].src11.h=216;
        gTheGame.chara[i].rect11.h  = gTheGame.chara[i].src11.h;
        gTheGame.chara[i].rect11.w  = gTheGame.chara[i].src11.w;

    	SDL_FreeSurface(s);
	}

    /*      斧を装備しているとき    */
    for(i = 0; i < gTheGame.charaNum; i++){
        
		if(NULL == (s = IMG_Load(gCharaitem2ImgFile[i]))){
			fprintf(stderr,"failed to open character image.\n");
            return -1;
		}
		gTheGame.chara[i].image8 = SDL_CreateTextureFromSurface(gTheGame.render, s);
		if (gTheGame.chara[i].image8 == NULL) {
            SDL_FreeSurface(s);
            fprintf(stderr,"failed to initialize chara_image.\n");
			return -1;
        }

        gTheGame.chara[i].src8.w=216;
        gTheGame.chara[i].src8.h=216;
        gTheGame.chara[i].rect8.h  = gTheGame.chara[i].src8.h;
        gTheGame.chara[i].rect8.w  = gTheGame.chara[i].src8.w;
        

    	SDL_FreeSurface(s);
	}

    /*      斧での攻撃     */
   for(i = 0; i < gTheGame.charaNum; i++){
		if(NULL == (s = IMG_Load(gCharaattack2ImgFile[i]))){
			fprintf(stderr,"failed to open character image.\n");
            return -1;
		}
		gTheGame.chara[i].image9 = SDL_CreateTextureFromSurface(gTheGame.render, s);
		if (gTheGame.chara[i].image9 == NULL) {
            SDL_FreeSurface(s);
            fprintf(stderr,"failed to initialize chara_image.\n");
			return -1;
        }
        gTheGame.chara[i].src9.w=216;
        gTheGame.chara[i].src9.h=216;
        gTheGame.chara[i].rect9.h  = gTheGame.chara[i].src9.h;
        gTheGame.chara[i].rect9.w  = gTheGame.chara[i].src9.w;
    	SDL_FreeSurface(s);
	} 
        /*      スコップを装備しているとき    */
    for(i = 0; i < gTheGame.charaNum; i++){
        
		if(NULL == (s = IMG_Load(gCharaitem4ImgFile[i]))){
			fprintf(stderr,"failed to open character image.\n");
            return -1;
		}
		gTheGame.chara[i].image12 = SDL_CreateTextureFromSurface(gTheGame.render, s);
		if (gTheGame.chara[i].image12 == NULL) {
            SDL_FreeSurface(s);
            fprintf(stderr,"failed to initialize chara_image.\n");
			return -1;
        }

        gTheGame.chara[i].src12.w=216;
        gTheGame.chara[i].src12.h=216;
        gTheGame.chara[i].rect12.h  = gTheGame.chara[i].src12.h;
        gTheGame.chara[i].rect12.w  = gTheGame.chara[i].src12.w;
        

    	SDL_FreeSurface(s);
	}


    /*      キャラが木を揺らす       */
    for(int ki = 0; ki < gTheGame.charaNum; ki++){
		if(NULL == (s = IMG_Load(gShakeImgFile[ki]))){
			fprintf(stderr,"failed to open character_ki[%d] image.\n",k);
            return -1;
		}
		gTheGame.chara[ki].image5 = SDL_CreateTextureFromSurface(gTheGame.render, s);
		if (gTheGame.chara[ki].image5 == NULL) {
            SDL_FreeSurface(s);
            fprintf(stderr,"failed to initialize chara_image5.\n");
			return -1;
        }
        gTheGame.chara[ki].src5.w=s->w/4;
        gTheGame.chara[ki].src5.h=s->h/8;
        gTheGame.chara[ki].rect5.h  = gTheGame.chara[ki].src5.h;
        gTheGame.chara[ki].rect5.w  = gTheGame.chara[ki].src5.w;
        gTheGame.chara[ki].src5.y=0;
       
        SDL_Rect kii             = { 0,0, gTheGame.chara[ki].rect5.w, gTheGame.chara[ki].rect5.h };
    	SDL_FreeSurface(s);
	} 

    /*      キャラクターが死んだとき      */
    for(int j = 0; j < gTheGame.charaNum; j++){
        if(NULL == (s = IMG_Load(gCharaDieImgFile[j]))){
			fprintf(stderr,"failed to open character_Die[%d] image.\n",j);
            return -1;
		}
        gTheGame.chara[j].image2 = SDL_CreateTextureFromSurface(gTheGame.render, s);
		if (gTheGame.chara[j].image2 == NULL) {
            SDL_FreeSurface(s);
            fprintf(stderr,"failed to initialize chara_image.\n");
			return -1;
        }

        gTheGame.chara[j].src2.w=162;
        gTheGame.chara[j].src2.h=144;
        gTheGame.chara[j].rect2.h  = gTheGame.chara[j].src2.h;
        gTheGame.chara[j].rect2.w  = gTheGame.chara[j].src2.w;
       

        SDL_Rect r_die             = { 0,0 , gTheGame.chara[j].rect2.w, gTheGame.chara[j].rect2.h };
        gTheGame.chara[j].mask2 = MeasureMask(s, r_die);

    	SDL_FreeSurface(s);
    }


    /*      幽霊用       */
    for(int ii = 0; ii < gTheGame.charaNum; ii++){
    
		if(NULL == (s = IMG_Load(gGhostImgFile[0]))){
			fprintf(stderr,"failed to open character_ghost[%d] image.\n",ii);
            return -1;
		}
		gTheGame.chara[ii].image3 = SDL_CreateTextureFromSurface(gTheGame.render, s);
     
		if (gTheGame.chara[ii].image3 == NULL) {
            SDL_FreeSurface(s);
            fprintf(stderr,"failed to initialize chara_image3.\n");
			return -1;
        }
        gTheGame.chara[ii].src3.w=144;
        gTheGame.chara[ii].src3.h=s->h/8;
        gTheGame.chara[ii].rect3.h  = gTheGame.chara[ii].src3.h;
        gTheGame.chara[ii].rect3.w  = gTheGame.chara[ii].src3.w;
    	SDL_FreeSurface(s);
    }

    /*      落とし穴用       */
    for(int ff = 0; ff < gTheGame.charaNum; ff++){
        
		if(NULL == (s = IMG_Load(gGFallImgFile[ff]))){
			fprintf(stderr,"failed to open character_Fall[%d] image.\n",ff);
            return -1;
		}
		gTheGame.chara[ff].image4 = SDL_CreateTextureFromSurface(gTheGame.render, s);
    
		if (gTheGame.chara[ff].image4 == NULL) {
            SDL_FreeSurface(s);
            fprintf(stderr,"failed to initialize chara_image3.\n");
			return -1;
        }
        gTheGame.chara[ff].src4.w=144;
        gTheGame.chara[ff].src4.h=s->h/8;
        gTheGame.chara[ff].rect4.h  = gTheGame.chara[ff].src4.h;
        gTheGame.chara[ff].rect4.w  = gTheGame.chara[ff].src4.w;
    	SDL_FreeSurface(s);
    }
     /*      勝敗判定用       */
    for(int ff = 0; ff < gTheGame.charaNum; ff++){
        
		if(NULL == (s = IMG_Load(gRizaltImgFile[ff]))){
			fprintf(stderr,"failed to open character_Fall[%d] image.\n",ff);
            return -1;
		}
		gTheGame.chara[ff].image13 = SDL_CreateTextureFromSurface(gTheGame.render, s);
    
		if (gTheGame.chara[ff].image13 == NULL) {
            SDL_FreeSurface(s);
            fprintf(stderr,"failed to initialize chara_image3.\n");
			return -1;
        }
        gTheGame.chara[ff].src13.w=216;
        gTheGame.chara[ff].src13.h=s->h/2;
        gTheGame.chara[ff].rect13.h  = gTheGame.chara[ff].src13.h;
        gTheGame.chara[ff].rect13.w  = gTheGame.chara[ff].src13.w;
    	SDL_FreeSurface(s);
    }



    //アイテム///////////////////////////////////////////////////////////////////////////////////
    for(f = 0; f < gTheGame.ItemNum; f++){
        if(f>=IT_NUM){
            if(f>=(IT_NUM)+n){
                n+=5;
            }
            if( f <= 19 ){
                gTheGame.item[f] = gTheGame.item[f-n];
            }
        }
        else{
		    if(NULL == (s = IMG_Load(gItemImgFile[f]))){
			    fprintf(stderr,"failed to open Item[%d] image.\n",f);
                return -1;
		    }
            if(NULL == (gTheGame.item[f].sd= IMG_Load(gItemImgFile[f]))){
			    fprintf(stderr,"failed to open Item[%d] image.\n",f);
                return -1;
		    }
		    gTheGame.item[f].image = SDL_CreateTextureFromSurface(gTheGame.render, s);
		    if (gTheGame.item[f].image == NULL) {
                SDL_FreeSurface(s);
                fprintf(stderr,"failed to initialize item_image.\n");
			    return -1;
            }
            if(f == 0 || f==1 || f==2 || f==3){//アイテムなら
                gTheGame.item[f].src.w=216;//(2019/1/11);
                gTheGame.item[f].src.h=216;
                gTheGame.item[f].rect.h  = gTheGame.item[f].src.h;
                gTheGame.item[f].rect.w  = gTheGame.item[f].src.w;
                gTheGame.item[f].src.y=0;
                SDL_Rect rc             = { 0, 0, gTheGame.item[f].rect.w, gTheGame.item[f].rect.h };
                gTheGame.item[f].mask = MeasureMask(s, rc);
            }
            else{
                //アイテムのサイズ
                gTheGame.item[f].src.w=s->w/gTheGame.anipatNum_Item[gTheGame.item[f].type];//(2019/1/11)
		        gTheGame.item[f].src.h=s->h/2;
                gTheGame.item[f].rect.h  = gTheGame.item[f].src.h;
                gTheGame.item[f].rect.w  = gTheGame.item[f].src.w;
                //アイテムのマスク
                SDL_Rect rc             = { 0, 96, gTheGame.item[f].rect.w, gTheGame.item[f].rect.h };
                gTheGame.item[f].mask = MeasureMask(s, rc);
            }
		
    	    SDL_FreeSurface(s);
        }
		
    }
    //木材/////////////////////////////////////////////////////////////////////////////////////
   for(int m=0;m<gTheGame.MokuNum;m++){
        if(m==0){
            if(NULL == (s = IMG_Load(gMokuzaiImgFile[m]))){
		        fprintf(stderr,"failed to open base[%d] image.\n",k);
                return -1;
	        }
            gTheGame.mokuzai[m].image = SDL_CreateTextureFromSurface(gTheGame.render, s);
            if (gTheGame.mokuzai[m].image == NULL) {
                SDL_FreeSurface(s);
                fprintf(stderr,"failed to initialize chara_image.\n");
		    	return -1;
            }
            gTheGame.mokuzai[m].src.w=s->w;
            gTheGame.mokuzai[m].src.h=s->h/2;
            gTheGame.mokuzai[m].rect.h =gTheGame.mokuzai[m].src.h;
            gTheGame.mokuzai[m].rect.w =gTheGame.mokuzai[m].src.w;

            SDL_Rect r    = { 0, 96, gTheGame.mokuzai[m].rect.w, gTheGame.mokuzai[m].rect.h};
            gTheGame.mokuzai[m].mask = MeasureMask(s, r);
            SDL_FreeSurface(s);
        }
        else{
            gTheGame.mokuzai[m] = gTheGame.mokuzai[0];
        }
    }
    //拠点//////////////////////////////////////////////////////////////////////////////////////
    for(k = 0; k < gTheGame.baseNum; k++){    
        if(k==0){
		    if(NULL == (s = IMG_Load(gBaseImgFile[0]))){
			    fprintf(stderr,"failed to open base[%d] image.\n",k);
                return -1;
		    }
		    gTheGame.base[k].image = SDL_CreateTextureFromSurface(gTheGame.render, s);
         
		    if (gTheGame.base[k].image == NULL) {
                SDL_FreeSurface(s);
                fprintf(stderr,"failed to initialize chara_image.\n");
		    	return -1;
            }
		    //拠点のサイズ
            gTheGame.base[k].src.w=s->w;
		    gTheGame.base[k].src.h=s->h/7;
            gTheGame.base[k].rect.h  = gTheGame.base[k].src.h;
            gTheGame.base[k].rect.w  = gTheGame.base[k].src.w;
        
    
            /* 拠点のマスク*/
            SDL_Rect bs            = { 0, 960, gTheGame.base[k].rect.w, gTheGame.base[k].rect.h};
            gTheGame.base[k].mask = MeasureMask(s, bs);
            
            /*以下、柵のマスク*/
            SDL_Rect bo            = { 0, 1300, gTheGame.base[k].rect.w, gTheGame.base[k].rect.h};
            gTheGame.base[k].mask2 = MeasureMask(s, bo);

            SDL_Rect b1            = { 0, 1600, gTheGame.base[k].rect.w, gTheGame.base[k].rect.h};
            gTheGame.base[k].mask3 = MeasureMask(s, b1);

            SDL_Rect b2            = { 0, 1920, gTheGame.base[k].rect.w, gTheGame.base[k].rect.h};
            gTheGame.base[k].mask4 = MeasureMask(s, b2);
        
    	    SDL_FreeSurface(s);

            if(NULL == (s = IMG_Load(gBaseImgFile[1]))){
			    fprintf(stderr,"failed to open base[%d] image.\n",k);
                return -1;
		    }
		    gTheGame.base[k].image2 = SDL_CreateTextureFromSurface(gTheGame.render, s);
         
		    if (gTheGame.base[k].image2 == NULL) {
                SDL_FreeSurface(s);
                fprintf(stderr,"failed to initialize chara_image.\n");
		    	return -1;
            }
		    //拠点のサイズ
            gTheGame.base[k].src2.w=s->w;
		    gTheGame.base[k].src2.h=s->h/2;
            gTheGame.base[k].rect2.h  = gTheGame.base[k].src2.h;
            gTheGame.base[k].rect2.w  = gTheGame.base[k].src2.w;
    	    SDL_FreeSurface(s);
        }
        else{
            gTheGame.base[k] = gTheGame.base[0];
        }
		
	} 
     //障害物///////////////////////////////////////////////////////////////////////////////////
    for(int o = 0; o < gTheGame.ObjeNum; o++){  
        if(o>=OT_NUM){
            if(o>=(OT_NUM)+nb){
                nb+=1;
            }
            gTheGame.object[o] = gTheGame.object[o-nb];
        } 
        else{
		    if(NULL == (s = IMG_Load(gObjectImgFile[o]))){
			    fprintf(stderr,"failed to open object[%d] image.\n",o);
                return -1;
		    }
		    gTheGame.object[o].image = SDL_CreateTextureFromSurface(gTheGame.render, s);
		    if (gTheGame.object[o].image == NULL) {
                SDL_FreeSurface(s);
                fprintf(stderr,"failed to initialize chara_image.\n");
			    return -1;
            }
		    //障害物のサイズ
            gTheGame.object[o].src.w=s->w/4;
		    gTheGame.object[o].src.h=s->h/3;
            gTheGame.object[o].rect.h  = gTheGame.object[o].src.h;
            gTheGame.object[o].rect.w  = gTheGame.object[o].src.w;
       
            /* 障害物のマスク */
            SDL_Rect os             = { 0, 0, gTheGame.object[o].rect.w, gTheGame.object[o].rect.h };
            gTheGame.object[o].mask = MeasureMask(s, os);
         
    	    SDL_FreeSurface(s); 
        }
	
	} 
       
  
     //落とし穴/////////////////////////////////////////////////////////////////
    for(int b = 0; b < gTheGame.FallNum; b++){
    
		if(NULL == (s = IMG_Load(gItemImgFile2[0]))){
			fprintf(stderr,"failed to open Fall[%d] image.\n",b);
            return -1;
		}
		gTheGame.fall[b].image = SDL_CreateTextureFromSurface(gTheGame.render, s);
		if (gTheGame.fall[b].image == NULL) {
            SDL_FreeSurface(s);
            fprintf(stderr,"failed to initialize chara_image.\n");
			return -1;
        }
		//落とし穴のサイズ
        gTheGame.fall[b].src.w=144;
        gTheGame.fall[b].src.h=144;
        gTheGame.fall[b].rect.h  = gTheGame.fall[b].src.h;
        gTheGame.fall[b].rect.w  = gTheGame.fall[b].src.w;
        /* 落とし穴のマスク */
        SDL_Rect fs             = { 0, 0, gTheGame.fall[b].rect.w, gTheGame.fall[b].rect.h };
        gTheGame.fall[b].mask = MeasureMask(s, fs);
    	SDL_FreeSurface(s);
    }


    //メッセージ//////////////////////////////////////////////////////////////////////////////////
    for(v = 0; v < gTheGame.MsgNum; v++){    
		if(NULL == (s = IMG_Load(gMessageImgFile[v]))){
			fprintf(stderr,"failed to open Messege[%d] image.\n",v);
            return -1;
		}
        gTheGame.message[v].image= SDL_CreateTextureFromSurface(gTheGame.render, s);
		if (gTheGame.message[v].image == NULL) {
            SDL_FreeSurface(s);
            fprintf(stderr,"failed to initialize Messege_image.\n");
			return -1;
        }
		//メッセージのサイズ
        if(v==7){
            gTheGame.message[v].src.w=s->w;
		    gTheGame.message[v].src.h=s->h/2;
            gTheGame.message[v].src.y=200;
        }
        else if(v==8){
            gTheGame.message[v].src.w=s->w;
		    gTheGame.message[v].src.h=s->h/4;
        }
        else if(v==11 || v==MS_Load){
            gTheGame.message[v].src.w=s->w/4;
		    gTheGame.message[v].src.h=s->h;
        }
        else{
            gTheGame.message[v].src.w=s->w;
		    gTheGame.message[v].src.h=s->h;
        }
        gTheGame.message[v].rect.h  = gTheGame.message[v].src.h;
        gTheGame.message[v].rect.w  = gTheGame.message[v].src.w;
    
    	SDL_FreeSurface(s);
    }
    /* メッセージ作成 */
    if (MakeMessage())
        return -1;
     /* マップの作成 */
    if (MakeMap())
        return -1;
    /* image利用終了 */
	IMG_Quit();
    fprintf(stderr,"OK_image\n");
	return 0;
}

/*****************************************************************
関数名	: DestroyWindow
機能	: SDLを終了する
引数	: なし
出力	: なし
*****************************************************************/
void DestroyWindow(void)
{
	int i;
    for (i = 0; i < gTheGame.charaNum; i++)
        SDL_DestroyTexture(gTheGame.chara[i].image);
    for (i = 0; i < gTheGame.ItemNum; i++)
        SDL_DestroyTexture(gTheGame.item[i].image);
    for (i = 0; i < gTheGame.baseNum; i++)
        SDL_DestroyTexture(gTheGame.base[i].image); 
    for (i = 0; i < gTheGame.ObjeNum; i++)
        SDL_DestroyTexture(gTheGame.object[i].image); 
    for (i = 0; i < gTheGame.FallNum; i++)
        SDL_DestroyTexture(gTheGame.fall[i].image);
    for (i = 0; i < gTheGame.MsgNum; i++)
        SDL_DestroyTexture(gTheGame.message[i].image);   
    for (i = 0; i < gTheGame.MokuNum; i++)
        SDL_DestroyTexture(gTheGame.mokuzai[i].image);   
	SDL_DestroyTexture(gTheGame.map);
    SDL_DestroyTexture(gTheGame.bg);
    SDL_FreeSurface(gTheGame.mapMask);
	SDL_DestroyRenderer(gTheGame.render);
    SDL_DestroyWindow(gTheGame.window);
	SDL_Quit();
}

/*****************************************************************
関数名	:RenderWindow
機能	: メインウインドウを描画する
引数	: なし
出力	: なし
*****************************************************************/
int RenderWindow(void)
{
    unsigned int Limit = 180000;
    unsigned int currentTime;

	//背景のコピー
	int ret = SDL_RenderCopy(gTheGame.render, gTheGame.bg, &(gTheGame.rectBg), NULL);
    if (ret < 0) {
        fprintf(stderr,"Not Copy Bg.\n");
    }
     /* マップ */
    ret = SDL_RenderCopy(gTheGame.render, gTheGame.map, &(gTheGame.rectMap), NULL);
    if (ret < 0) {
         fprintf(stderr,"Not Copy Map.\n");
    }
    /*落とし穴*/
    PlaceFall();
    /*木材*/
    PlaceMokuzai();
     /*アイテム*/
    PlaceItem();
    /*キャラクター、拠点、アイテム、障害物の描画*/
    Place_Move();
    /*キャラクターの頭の上に矢印表示(1/26)変更*/
    Display_charahead();
    /*メッセージ　*/
    PlaceMassege();
    /*ミニマップ*/
    makeMiniMap();
    /*HP*/
    Display_HP();
    /*時間*/
    Display_Limit(gTheGame.Limit);
     /*ボイント*/
    Display_point();
    /*拠点のHP*/
    Display_BaseHP();
    /*木材の所有数*/
    Display_Mokuzai();
     /*アイテムセレクト*/
    Display_ItemSelect();
    /*勝敗判定*/
    RezultWindow();
	 /* 描画更新 */
	SDL_RenderPresent(gTheGame.render);

	return ret;
}


//2018.12.22
/*****************************************************************
関数名	:PlaceFall
機能	: 落とし穴のコピー
引数	: なし
出力	: なし
*****************************************************************/
static void PlaceFall(void)
{
    /* 落とし穴のコピー */
    int i;
	//落とし穴の数分、落とし穴の位置をコピー
        for (i = 0; i < gTheGame.FallNum; i++) {
		//表示されていたら、落とし穴の位置を移動する。
            if (gTheGame.fall[i].stts == FS_Enable) {
			    SDL_Rect dst = gTheGame.fall[i].rect;
                dst.y -= gTheGame.rectMap.y;
                dst.x -= gTheGame.rectMap.x;
                if (0 > SDL_RenderCopy(gTheGame.render, gTheGame.fall[i].image, &(gTheGame.fall[i].src), &dst)) {
                    fprintf(stderr,"Not Copy item[%d].\n",i);
                }
            }
        }
}

static void PlaceMokuzai(void)
{
    /* 木材のコピー */
    int i;
	//木材の数分、落とし穴の位置をコピー
        for (i = 0; i < gTheGame.MokuNum; i++) {
		//表示されていたら、木材の位置を移動する。
            if (gTheGame.mokuzai[i].stts == IS_Enable) {
			    SDL_Rect dst = gTheGame.mokuzai[i].rect;
                dst.y -= gTheGame.rectMap.y;
                dst.x -= gTheGame.rectMap.x;
                if (0 > SDL_RenderCopy(gTheGame.render, gTheGame.mokuzai[i].image, &(gTheGame.mokuzai[i].src), &dst)) {
                    fprintf(stderr,"Not Copy mokuzai[%d].\n",i);
                }
            }
        }
}


/*****************************************************************
関数名	:PlaceItem
機能	: アイテムのコピー
引数	: なし
出力	: なし
*****************************************************************/
static void PlaceItem(void)
{
    /* アイテムのコピー */
    int i;
	//アイテムの数分、アイテムの位置をコピー
    for (i = 0; i < gTheGame.ItemNum; i++) {
		//表示されていたら、アイテムの位置を移動する。
        if(i == 4 || i == 9 || i == 14 || i >= 19 ){
            if (gTheGame.item[i].stts == IS_Enable  || gTheGame.item[i].stts == IS_Mine) {
			    SDL_Rect dst = gTheGame.item[i].rect;
                dst.y -= gTheGame.rectMap.y;
                dst.x -= gTheGame.rectMap.x;
                if (0 > SDL_RenderCopy(gTheGame.render, gTheGame.item[i].image, &(gTheGame.item[i].src), &dst)) {
                    fprintf(stderr,"Not Copy item[%d].\n",i);
                }
           }
        }
    }
}


/*****************************************************************
関数名	:PlaceMassege
機能	: メッセージのコピー
引数	: なし
出力	: なし
*****************************************************************/
static void PlaceMassege(void)
{
    /* メッセージのコピー */
    int i;
	//メッセージの数分、メッセージの位置をコピー
    for (i = 0; i <gTheGame.MsgNum; i++) {
		//表示されていたら、メッセージの位置を移動する。
        if (gTheGame.message[i].stts != MS_Disable) {
			SDL_Rect dst = gTheGame.message[i].rect;
            if(gTheGame.message[i].type==MS_kazu){
                dst.y -= gTheGame.rectMap.y;
                dst.x -= gTheGame.rectMap.x;
            }
            if (0 > SDL_RenderCopy(gTheGame.render, gTheGame.message[i].image, &(gTheGame.message[i].src), &dst)) {
                fprintf(stderr,"Not Copy Massage[%d].\n",i);
            }
        }
    }
}


/*****************************************************************
関数名	:Place_Move
機能	: キャラ、拠点、障害物のコピー
引数	: なし
出力	: なし
*****************************************************************/
static void Place_Move(void){
    int o,i,j,tmp1,tmp2,tmp3;
    int k,n,g;
    //比較するために、一旦数値を別の配列に格納
    for(k=0;k<gTheGame.charaNum;k++){
        gTheGame.sort[k].rect=gTheGame.chara[k].rect.y+gTheGame.chara[k].rect.h;
        gTheGame.sort[k].number=k;
        gTheGame.sort[k].type=1;
    }
    for(n=0;n<gTheGame.baseNum;n++){
        gTheGame.sort[k+n].rect=gTheGame.base[n].rect.y+gTheGame.base[n].rect.h;
        gTheGame.sort[k+n].number=n;
        gTheGame.sort[k+n].type=2;
    }
    for(g=0;g<gTheGame.ObjeNum;g++){
        gTheGame.sort[k+n+g].rect=gTheGame.object[g].rect.y+gTheGame.object[g].rect.h;
        gTheGame.sort[k+n+g].number=g;
        gTheGame.sort[k+n+g].type=3;
    }
    //ヒープソートにより、描画の順番を決定
    for(o=0;o<gTheGame.Number_Total;o++){
        for(j=o+1;j<gTheGame.Number_Total;j++){
            if(gTheGame.sort[o].rect>gTheGame.sort[j].rect){
                tmp1=gTheGame.sort[o].rect;
                gTheGame.sort[o].rect=gTheGame.sort[j].rect;
                gTheGame.sort[j].rect=tmp1;
                tmp2=gTheGame.sort[o].number;
                gTheGame.sort[o].number=gTheGame.sort[j].number;
                gTheGame.sort[j].number=tmp2;
                tmp3=gTheGame.sort[o].type;
                gTheGame.sort[o].type=gTheGame.sort[j].type;
                gTheGame.sort[j].type=tmp3;
            }
        } 
    }
    //描画部分
    for(i=0;i<gTheGame.Number_Total;i++){
        /*キャラクター*/
        if(gTheGame.sort[i].type==1){

            /*  通常時   */
            if (gTheGame.chara[gTheGame.sort[i].number].stts ==CS_Enable || 
            gTheGame.chara[gTheGame.sort[i].number].stts ==CS_Act ||
            gTheGame.chara[gTheGame.sort[i].number].stts ==CS_Chuze) {
			    SDL_Rect dst = gTheGame.chara[gTheGame.sort[i].number].rect;
                dst.y -= gTheGame.rectMap.y;
                dst.x -= gTheGame.rectMap.x;
                if (0 > SDL_RenderCopy(gTheGame.render, gTheGame.chara[gTheGame.sort[i].number].image, 
                &(gTheGame.chara[gTheGame.sort[i].number].src), &dst)) {
                    fprintf(stderr,"Not Copy chara[%d].^\n",i);
                }
            }
            /*  アイテムを持った */
            else if(gTheGame.chara[gTheGame.sort[i].number].stts == CS_Item){

                int anipa_y = gTheGame.chara[gTheGame.sort[i].number].src.y;
                int anipa_y2 = anipa_y / 144;
                int item_ti = gTheGame.chara[gTheGame.sort[i].number].i_num;

                //ピコピコハンマーの場合
                if(item_ti == 0 || item_ti == 5 || item_ti == 10 || item_ti == 15 ){
                    gTheGame.chara[gTheGame.sort[i].number].rect6.x = gTheGame.chara[gTheGame.sort[i].number].rect.x - 36;
                    gTheGame.chara[gTheGame.sort[i].number].rect6.y = gTheGame.chara[gTheGame.sort[i].number].rect.y - 36;
                    gTheGame.chara[gTheGame.sort[i].number].src6.y = 216 * anipa_y2;
                
                    SDL_Rect dst = gTheGame.chara[gTheGame.sort[i].number].rect6;
                    dst.y -= gTheGame.rectMap.y;
                    dst.x -= gTheGame.rectMap.x;
                    if (0 > SDL_RenderCopy(gTheGame.render, gTheGame.chara[gTheGame.sort[i].number].image6,
                    &(gTheGame.chara[gTheGame.sort[i].number].src6), &dst)) {
                        fprintf(stderr,"Not Copy chara[%d].^\n",i);
                    }
                }
                //斧の場合
                else if(item_ti == 2 || item_ti == 7 || item_ti == 12 || item_ti == 17 ){
                    gTheGame.chara[gTheGame.sort[i].number].rect8.x = gTheGame.chara[gTheGame.sort[i].number].rect.x - 36;
                    gTheGame.chara[gTheGame.sort[i].number].rect8.y = gTheGame.chara[gTheGame.sort[i].number].rect.y - 36;
                    gTheGame.chara[gTheGame.sort[i].number].src8.y = 216 * anipa_y2;
                
                    SDL_Rect dst = gTheGame.chara[gTheGame.sort[i].number].rect8;
                    dst.y -= gTheGame.rectMap.y;
                    dst.x -= gTheGame.rectMap.x;
                    if (0 > SDL_RenderCopy(gTheGame.render, gTheGame.chara[gTheGame.sort[i].number].image8,&
                    (gTheGame.chara[gTheGame.sort[i].number].src8), &dst)) {
                        fprintf(stderr,"Not Copy chara[%d].^\n",i);
                    }
                }
                 //傘の場合
                else if(item_ti == 1 || item_ti == 6 || item_ti == 11 || item_ti == 16 ){
                    gTheGame.chara[gTheGame.sort[i].number].rect10.x = gTheGame.chara[gTheGame.sort[i].number].rect.x - 36;
                    gTheGame.chara[gTheGame.sort[i].number].rect10.y = gTheGame.chara[gTheGame.sort[i].number].rect.y - 36;
                    gTheGame.chara[gTheGame.sort[i].number].src10.y = 216 * anipa_y2;
                
                    SDL_Rect dst = gTheGame.chara[gTheGame.sort[i].number].rect10;
                    dst.y -= gTheGame.rectMap.y;
                    dst.x -= gTheGame.rectMap.x;
                    if (0 > SDL_RenderCopy(gTheGame.render, gTheGame.chara[gTheGame.sort[i].number].image10,&
                    (gTheGame.chara[gTheGame.sort[i].number].src10), &dst)) {
                        fprintf(stderr,"Not Copy chara[%d].^\n",i);
                    }
                }
                   //傘の場合
                else if(item_ti == 3 || item_ti == 8 || item_ti == 13 || item_ti == 18 ){
                    gTheGame.chara[gTheGame.sort[i].number].rect12.x = gTheGame.chara[gTheGame.sort[i].number].rect.x - 36;
                    gTheGame.chara[gTheGame.sort[i].number].rect12.y = gTheGame.chara[gTheGame.sort[i].number].rect.y - 36;
                    gTheGame.chara[gTheGame.sort[i].number].src12.y = 216 * anipa_y2;
                
                    SDL_Rect dst = gTheGame.chara[gTheGame.sort[i].number].rect12;
                    dst.y -= gTheGame.rectMap.y;
                    dst.x -= gTheGame.rectMap.x;
                    if (0 > SDL_RenderCopy(gTheGame.render, gTheGame.chara[gTheGame.sort[i].number].image12,&
                    (gTheGame.chara[gTheGame.sort[i].number].src12), &dst)) {
                        fprintf(stderr,"Not Copy chara[%d].^\n",i);
                    }
                }
                
            }
            /*  攻撃している場合  */
            else if(gTheGame.chara[gTheGame.sort[i].number].stts == CS_Attack1){
                int anipa_y3 = gTheGame.chara[gTheGame.sort[i].number].src.y;
                int anipa_y4 = anipa_y3 / 144;
                int item_ti2 = gTheGame.chara[gTheGame.sort[i].number].i_num;
                //fprintf(stderr,"inum = %d\n",item_ti2);
                //ピコピコハンマーの場合
                if(item_ti2 == 0 || item_ti2 == 5 || item_ti2 == 10 || item_ti2 == 15 ){
                    gTheGame.chara[gTheGame.sort[i].number].rect7.x = gTheGame.chara[gTheGame.sort[i].number].rect.x - 36;
                    gTheGame.chara[gTheGame.sort[i].number].rect7.y = gTheGame.chara[gTheGame.sort[i].number].rect.y - 36;
                    gTheGame.chara[gTheGame.sort[i].number].src7.y = 216 * anipa_y4;
                
                    SDL_Rect dst = gTheGame.chara[gTheGame.sort[i].number].rect7;
                    dst.y -= gTheGame.rectMap.y;
                    dst.x -= gTheGame.rectMap.x;
                    if (0 > SDL_RenderCopy(gTheGame.render, gTheGame.chara[gTheGame.sort[i].number].image7, 
                    &(gTheGame.chara[gTheGame.sort[i].number].src7), &dst)) {
                        fprintf(stderr,"Not Copy chara[%d].^\n",i);
                    }
                }
                //斧の場合
                else if(item_ti2 == 2 || item_ti2 == 7 || item_ti2 == 12 || item_ti2 == 17 ){
                    gTheGame.chara[gTheGame.sort[i].number].rect9.x = gTheGame.chara[gTheGame.sort[i].number].rect.x - 36;
                    gTheGame.chara[gTheGame.sort[i].number].rect9.y = gTheGame.chara[gTheGame.sort[i].number].rect.y - 36;
                    gTheGame.chara[gTheGame.sort[i].number].src9.y = 216 * anipa_y4;
                
                    SDL_Rect dst = gTheGame.chara[gTheGame.sort[i].number].rect9;
                    dst.y -= gTheGame.rectMap.y;
                    dst.x -= gTheGame.rectMap.x;
                    if (0 > SDL_RenderCopy(gTheGame.render, gTheGame.chara[gTheGame.sort[i].number].image9, 
                    &(gTheGame.chara[gTheGame.sort[i].number].src9), &dst)) {
                        fprintf(stderr,"Not Copy chara[%d].^\n",i);
                    }
                }
                   //斧の場合
                else if(item_ti2 == 1 || item_ti2 == 6 || item_ti2 == 11 || item_ti2 == 16 ){
                    gTheGame.chara[gTheGame.sort[i].number].rect11.x = gTheGame.chara[gTheGame.sort[i].number].rect.x - 36;
                    gTheGame.chara[gTheGame.sort[i].number].rect11.y = gTheGame.chara[gTheGame.sort[i].number].rect.y - 36;
                    gTheGame.chara[gTheGame.sort[i].number].src11.y = 216 * anipa_y4;
                
                    SDL_Rect dst = gTheGame.chara[gTheGame.sort[i].number].rect11;
                    dst.y -= gTheGame.rectMap.y;
                    dst.x -= gTheGame.rectMap.x;
                    if (0 > SDL_RenderCopy(gTheGame.render, gTheGame.chara[gTheGame.sort[i].number].image11, 
                    &(gTheGame.chara[gTheGame.sort[i].number].src11), &dst)) {
                        fprintf(stderr,"Not Copy chara[%d].^\n",i);
                    }
                }
                //斧の場合
                else if(item_ti2 == 3 || item_ti2 == 8 || item_ti2 == 13 || item_ti2 == 18 ){
                    gTheGame.chara[gTheGame.sort[i].number].rect12.x = gTheGame.chara[gTheGame.sort[i].number].rect.x - 36;
                    gTheGame.chara[gTheGame.sort[i].number].rect12.y = gTheGame.chara[gTheGame.sort[i].number].rect.y - 36;
                    gTheGame.chara[gTheGame.sort[i].number].src12.y = 216 * anipa_y4;
                
                    SDL_Rect dst = gTheGame.chara[gTheGame.sort[i].number].rect12;
                    dst.y -= gTheGame.rectMap.y;
                    dst.x -= gTheGame.rectMap.x;
                    if (0 > SDL_RenderCopy(gTheGame.render, gTheGame.chara[gTheGame.sort[i].number].image12, 
                    &(gTheGame.chara[gTheGame.sort[i].number].src12), &dst)) {
                        fprintf(stderr,"Not Copy chara[%d].^\n",i);
                    }
                }
                //アイテムのマスクのｙ座標変換
                if(item_ti2 != 4 && item_ti2 != 9 && item_ti2 != 14 && item_ti2 < 19 ){//木材以外の場合
                    gTheGame.item[item_ti2].src.y = gTheGame.chara[gTheGame.sort[i].number].dir_flag * 216;
                }
            }
            /*  木を揺らす   */
            else if(gTheGame.chara[gTheGame.sort[i].number].stts == CS_Wood){
                gTheGame.chara[gTheGame.sort[i].number].rect5 = gTheGame.chara[gTheGame.sort[i].number].rect;

                if(gTheGame.chara[gTheGame.sort[i].number].wood==0){
                    gTheGame.chara[gTheGame.sort[i].number].src5.y=144* gTheGame.chara[gTheGame.sort[i].number].dir_flag;
                    gTheGame.chara[gTheGame.sort[i].number].wood=1;
                }

                SDL_Rect dst = gTheGame.chara[gTheGame.sort[i].number].rect5;
                dst.y -= gTheGame.rectMap.y;
                dst.x -= gTheGame.rectMap.x;
                if (0 > SDL_RenderCopy(gTheGame.render, gTheGame.chara[gTheGame.sort[i].number].image5, 
                &(gTheGame.chara[gTheGame.sort[i].number].src5), &dst)) {
                    fprintf(stderr,"Not Copy chara[%d].^\n",i);
                }
            }
            /*  キャラが倒れた   */
            else if(gTheGame.chara[gTheGame.sort[i].number].stts == CS_Die){
                
                SDL_Rect dst = gTheGame.chara[gTheGame.sort[i].number].rect;
                dst.y -= gTheGame.rectMap.y;
                dst.x -= gTheGame.rectMap.x;
                if (0 > SDL_RenderCopy(gTheGame.render, gTheGame.chara[gTheGame.sort[i].number].image2, 
                &(gTheGame.chara[gTheGame.sort[i].number].src2), &dst)) {
                    fprintf(stderr,"Not Copy chara[%d].^\n",i);
                }
            }
            /*  落とし穴に落ちた  */
            else if(gTheGame.chara[gTheGame.sort[i].number].stts == CS_Fallout){//(2018.12.23)
                gTheGame.chara[gTheGame.sort[i].number].rect4.x = gTheGame.chara[gTheGame.sort[i].number].rect.x;
                gTheGame.chara[gTheGame.sort[i].number].rect4.y = gTheGame.chara[gTheGame.sort[i].number].rect.y;
                SDL_Rect dst = gTheGame.chara[gTheGame.sort[i].number].rect4;
                dst.y -= gTheGame.rectMap.y;
                dst.x -= gTheGame.rectMap.x;
                if (0 > SDL_RenderCopy(gTheGame.render, gTheGame.chara[gTheGame.sort[i].number].image4, 
                &(gTheGame.chara[gTheGame.sort[i].number].src4), &dst)) {
                    fprintf(stderr,"Not Copy chara[%d].^\n",i);
                }
            }
            /*  ゴーストになった  */
            else if(gTheGame.sort[i].number== gTheGame.c_ID){
                if(gTheGame.chara[gTheGame.sort[i].number].stts == CS_Ghost){
                    SDL_Rect dst = gTheGame.chara[gTheGame.sort[i].number].rect3;
                    dst.y -= gTheGame.rectMap.y;
                    dst.x -= gTheGame.rectMap.x;
                    if (0 > SDL_RenderCopy(gTheGame.render, gTheGame.chara[gTheGame.sort[i].number].image3,
                     &(gTheGame.chara[gTheGame.sort[i].number].src3), &dst)) {
                        fprintf(stderr,"Not Copy ghost[%d].^\n",i);
                    }
                 }
            }
        }

        /*拠点*/
        else if(gTheGame.sort[i].type==2){
            //表示されていたら、拠点の位置を移動する。
            if (gTheGame.base[gTheGame.sort[i].number].stts != BS_Disable && 
            gTheGame.base[gTheGame.sort[i].number].stts != BS_Group1_P &&
            gTheGame.base[gTheGame.sort[i].number].stts != BS_Group2_P) {
			    SDL_Rect dst = gTheGame.base[gTheGame.sort[i].number].rect;
                dst.y -= gTheGame.rectMap.y;
                dst.x -= gTheGame.rectMap.x;
                if (0 > SDL_RenderCopy(gTheGame.render, gTheGame.base[gTheGame.sort[i].number].image,
                 &(gTheGame.base[gTheGame.sort[i].number].src), &dst)) {
                    fprintf(stderr,"Not Copy base[%d].\n",i);
                }
             }
             else if(gTheGame.base[gTheGame.sort[i].number].stts == BS_Group1_P ||
             gTheGame.base[gTheGame.sort[i].number].stts == BS_Group2_P){
                gTheGame.base[gTheGame.sort[i].number].rect2.x=gTheGame.base[gTheGame.sort[i].number].rect.x;
                gTheGame.base[gTheGame.sort[i].number].rect2.y=gTheGame.base[gTheGame.sort[i].number].rect.y-320;
                SDL_Rect dst = gTheGame.base[gTheGame.sort[i].number].rect2;
                dst.y -= gTheGame.rectMap.y;
                dst.x -= gTheGame.rectMap.x;
                if (0 > SDL_RenderCopy(gTheGame.render, gTheGame.base[gTheGame.sort[i].number].image2,
                 &(gTheGame.base[gTheGame.sort[i].number].src2), &dst)) {
                    fprintf(stderr,"Not Copy base[%d].\n",i);
                }
             }
        }

        /*障害物*/
        else if(gTheGame.sort[i].type==3){
            //表示されていたら、障害物の位置を移動する。
            if (gTheGame.object[gTheGame.sort[i].number].stts != OS_Die) {
			    SDL_Rect dst = gTheGame.object[gTheGame.sort[i].number].rect;
                dst.y -= gTheGame.rectMap.y;
                dst.x -= gTheGame.rectMap.x;
                if (0 > SDL_RenderCopy(gTheGame.render, gTheGame.object[gTheGame.sort[i].number].image,
                 &(gTheGame.object[gTheGame.sort[i].number].src), &dst)) {
                    fprintf(stderr,"Not Copy base[%d].\n",i);
                 }
             }
        }

    }
}

/*****************************************************************
関数名	: MakeMap
機能	: マップの作成
引数	: なし
出力	: なし
*****************************************************************/
int MakeMap(void)
{
    int ret = 0;
     //画像読み込み 
    SDL_Surface *img = IMG_Load(gMapImgFile);
    if (img == NULL)
        fprintf(stderr,"Error\n");

    /* 画像フォーマットを元にサーフェイス生成 */
    SDL_Surface *map = SDL_CreateRGBSurfaceWithFormat(0, MAP_Width * MAP_ChipSizeZ, MAP_Height * MAP_ChipSizeZ, 32, img->format->format);
    if (map == NULL) {
        fprintf(stderr,"Error\n");
    }

    gTheGame.mapMask = SDL_CreateRGBSurfaceWithFormat(0, MAP_Width * MAP_ChipSizeZ, MAP_Height * MAP_ChipSizeZ, 32, img->format->format);
    if (gTheGame.mapMask == NULL) {
        fprintf(stderr,"Error\n");
    }

    /* マップの読み込みと配置 */
    FILE *fp = fopen(gMapDataFile, "r");
    if (fp == NULL) {
         fprintf(stderr,"failed to open map data.\n");
    } else if (map && gTheGame.mapMask) {
        int i, j;
        MapType mt   = MT_None;
        SDL_Rect src = { 0, 0, MAP_ChipSizeZ, MAP_ChipSizeZ };
        SDL_Rect dst = { 0 };
        for (j = 0; j < MAP_Height; j++, dst.y += MAP_ChipSizeZ) {
            dst.x = 0;
            for (i = 0; i < MAP_Width; i++, dst.x += MAP_ChipSizeZ) {
                if (fscanf(fp, "%u", &mt) != 1) {
                   fprintf(stderr,"failed to load map data.\n");
                }

                src.x = mt * MAP_ChipSizeZ;
                src.y = 0;
                if (0 > SDL_BlitSurface(img, &src, map, &dst)) {
                     fprintf(stderr,"failed to load BiltSurface x.\n");
                }

                src.y = MAP_ChipSizeZ;
                if (0 > SDL_BlitSurface(img, &src, gTheGame.mapMask, &dst)) {
                   fprintf(stderr,"failed to load BiltSurface x\n");
                }
            }
        }
        /* マップはテクスチャに */
        if (NULL == (gTheGame.map = SDL_CreateTextureFromSurface(gTheGame.render, map))) {
               fprintf(stderr,"Not Map Texture.\n");
        }
    }
    /* ファイルを閉じる*/ 
    fclose(fp);
    /* サーフェイス解放 */
    SDL_FreeSurface(map);
    SDL_FreeSurface(img);

    return ret;
}

/*****************************************************************
関数名	: MakeMessage
機能	: メッセージの作成
引数	: なし
出力	: なし
*****************************************************************/
int MakeMessage(void)
{
    int ret = 0;
    /* フォントからメッセージテクスチャ作成 */
    /* 初期化 */
    if (TTF_Init() < 0) {
        fprintf(stderr,"not_mess1\n");
    }
    /* フォントを開く */
    TTF_Font *ttf[2];
    if (NULL == (ttf[0] = TTF_OpenFont(gFontFile[1], 88))) {
        fprintf(stderr,"not_mess2\n");
    }
    if (NULL == (ttf[1] = TTF_OpenFont(gFontFile[1], 28))) {
       fprintf(stderr,"not_mess3\n");
    }
    /* メッセージ作成 */
    int nos[8] = { 0, 0, 0, 1, 1, 1, 1, 1 };
    SDL_Color cols[8] = { gWhite, gBlack, gBlack, gWhite, gBlack, gWhite, gWhite, gBlack };
    for (int i = 0; i < 8 && ttf[nos[i]]; i++) {
        SDL_Surface *sf;
        if (NULL == (sf = TTF_RenderUTF8_Blended(ttf[nos[i]], gMsgStr[i], cols[i]))) {
             fprintf(stderr,"not_mess4\n");
        } else {
            
            /* テクスチャへ */
            if (NULL == (gTheGame.msgTexts[i] = SDL_CreateTextureFromSurface(gTheGame.render, sf))) {
                fprintf(stderr,"not_mess7\n");
            }
            SDL_FreeSurface(sf);
        }
    }

    /* フォントを閉じる */
    TTF_CloseFont(ttf[1]);
    TTF_CloseFont(ttf[0]);
    /* フォント利用終了 */
    TTF_Quit();

    return ret;
}

/*****************************************************************
関数名	: MakeMiniMap
機能	: ミニマップの作成
引数	: なし
出力	: なし
*****************************************************************/
static void makeMiniMap(void){
       int i,b,t;
    //ミニマップの画面サイズの設定
    SDL_Rect kanban = { 600, 380, 200, 200 };
    if (0 > SDL_RenderCopy(gTheGame.render, gTheGame.message[MS_Kn].image, &(gTheGame.message[MS_Kn].src), &kanban)) {
                    fprintf(stderr,"Not Copy Tokuten\n");
    }
    SDL_Rect white = { 623, 415, 160, 130 };
    SDL_SetRenderDrawColor(gTheGame.render, 255, 255, 255, 255);
    //SDL_RenderFillRect(gTheGame.render,&white);
    SDL_Rect mini = { 623, 415, 160, 130 };//150.150
    SDL_SetRenderDrawColor(gTheGame.render, 0, 0, 0, 255);
    SDL_RenderDrawRect(gTheGame.render,&mini);
    //所有している拠点の位置の表示
    for( b = 0; b < gTheGame.baseNum; b++ ){
        //拠点の位置確認用 空き地を白色で表示
        //偶数チームが所有している拠点は赤色で表示
        if( gTheGame.base[b].stts == BS_Group1  ||gTheGame.base[b].stts == BS_Group1_P){
            if( gTheGame.c_ID == 0 || gTheGame.c_ID == 2 ){
                int x_base =  map(gTheGame.base[b].point.x,10, MAP_Width*MAP_ChipSizeZ - mini.h, mini.x, mini.x + mini.h + 20);  
                int y_base = map(gTheGame.base[b].point.y,10, MAP_Height*MAP_ChipSizeZ - mini.h, mini.y, mini.y + mini.h - 25);
                SDL_Rect rect_base = { x_base, y_base, 25, 25 };
                if (0 > SDL_RenderCopy(gTheGame.render, gTheGame.message[MS_Mini1].image, &(gTheGame.message[MS_Mini1].src), &rect_base)) {
                    fprintf(stderr,"Not Copy Tokuten\n");
                }
                //filledTrigonColor(gTheGame.render,x_base,y_base,x_base-10,y_base+10,x_base+10,y_base+10,0xff0000ff);
                //fprintf(stderr,"base%d(%d,%d)\n",b,x_base,y_base);
            }
        }
        //奇数チームが所有している拠点は青色で表示
        else if( gTheGame.base[b].stts == BS_Group2 || gTheGame.base[b].stts == BS_Group2_P){
            if( gTheGame.c_ID == 1 || gTheGame.c_ID == 3 ){
                int x_base =  map(gTheGame.base[b].point.x,10, MAP_Width*MAP_ChipSizeZ - mini.h, mini.x, mini.x + mini.h + 20);  
                int y_base = map(gTheGame.base[b].point.y,10, MAP_Height*MAP_ChipSizeZ - mini.h, mini.y, mini.y + mini.h - 25);
                SDL_Rect rect_base = { x_base, y_base, 25, 25 };
                if (0 > SDL_RenderCopy(gTheGame.render, gTheGame.message[MS_Mini2].image, &(gTheGame.message[MS_Mini2].src), &rect_base)) {
                    fprintf(stderr,"Not Copy Tokuten\n");
                }
                //filledTrigonColor(gTheGame.render,x_base,y_base,x_base-10,y_base+10,x_base+10,y_base+10,0xffff0000);
                //fprintf(stderr,"base[%d].stts%d\n",b,gTheGame.base[b].stts);
                //拠点が攻撃されている場合には注意喚起を表示
            }
        }
    }   
    //キャラクターの現在位置の表示
    for( i = 0; i < gTheGame.charaNum; i++ ){
        int x = map(gTheGame.chara[i].rect.x,10, MAP_Width*MAP_ChipSizeZ - mini.h, mini.x, mini.x + mini.h + 20 );  
        int y = map(gTheGame.chara[i].rect.y,10, MAP_Height*MAP_ChipSizeZ - mini.h, mini.y, mini.y + mini.h - 25 );
        SDL_Rect rect = {x, y, 25, 25 };
        //SDL_RenderDrawRect(gTheGame.render,&rect);
        //偶数チームは赤色、奇数チームは青色、幽霊状態は白色
        if( gTheGame.chara[i].stts != CS_Ghost ){
            if( i == 0 || i == 2 ){
                if (0 > SDL_RenderCopy(gTheGame.render, gTheGame.message[MS_Mini3].image, &(gTheGame.message[MS_Mini3].src), &rect)) {
                    fprintf(stderr,"Not Copy Tokuten\n");
                }
                //SDL_SetRenderDrawColor(gTheGame.render, 255, 0, 0, 255);
                //SDL_RenderFillRect(gTheGame.render, &rect);
            }else if( i == 1 || i == 3 ){
                if (0 > SDL_RenderCopy(gTheGame.render, gTheGame.message[MS_Mini4].image, &(gTheGame.message[MS_Mini4].src), &rect)) {
                    fprintf(stderr,"Not Copy Tokuten\n");
                }
                //SDL_SetRenderDrawColor(gTheGame.render, 0, 0, 255, 255);
                //SDL_RenderFillRect(gTheGame.render, &rect);
            }
        }else{
                if (0 > SDL_RenderCopy(gTheGame.render, gTheGame.message[MS_Mini5].image, &(gTheGame.message[18].src), &rect)) {
                    fprintf(stderr,"Not Copy Tokuten\n");
                }
                //SDL_SetRenderDrawColor(gTheGame.render, 255, 255, 255, 255);
                //SDL_RenderFillRect(gTheGame.render, &rect);
        }
    }
    SDL_SetRenderDrawColor(gTheGame.render, 0, 0, 0, 255);
}


/*****************************************************************
関数名	: Display_HP
機能	: HPの表示
引数	: なし
出力	: なし
*****************************************************************/
static void Display_HP(void){
    if(gTheGame.hideFlag==0){
        //HPをハートマークで表示
    SDL_Rect HP = { 150, 30, 60, 60 };
    SDL_Rect HP2 = { 200, 30, 60, 60 };
    SDL_Rect HP3 = { 250, 30, 60, 60 };
    switch(gTheGame.chara[gTheGame.c_ID].hp){
        case 3:
            if (0 > SDL_RenderCopy(gTheGame.render, gTheGame.message[9].image, &(gTheGame.message[9].src), &HP)) {
                    fprintf(stderr,"Not Copy Tokuten\n");
            }
            if (0 > SDL_RenderCopy(gTheGame.render, gTheGame.message[9].image, &(gTheGame.message[9].src), &HP2)) {
                    fprintf(stderr,"Not Copy Tokuten\n");
            }
            if (0 > SDL_RenderCopy(gTheGame.render, gTheGame.message[9].image, &(gTheGame.message[9].src), &HP3)) {
                    fprintf(stderr,"Not Copy Tokuten\n");
            }
            break;
        case 2:
            if (0 > SDL_RenderCopy(gTheGame.render, gTheGame.message[9].image, &(gTheGame.message[9].src), &HP)) {
                    fprintf(stderr,"Not Copy Tokuten\n");
            }
            if (0 > SDL_RenderCopy(gTheGame.render, gTheGame.message[9].image, &(gTheGame.message[9].src), &HP2)) {
                    fprintf(stderr,"Not Copy Tokuten\n");
            }
            if (0 > SDL_RenderCopy(gTheGame.render, gTheGame.message[10].image, &(gTheGame.message[10].src), &HP3)) {
                    fprintf(stderr,"Not Copy Tokuten\n");
            }
            break;
        case 1:
            if (0 > SDL_RenderCopy(gTheGame.render, gTheGame.message[9].image, &(gTheGame.message[9].src), &HP)) {
                    fprintf(stderr,"Not Copy Tokuten\n");
            }
            if (0 > SDL_RenderCopy(gTheGame.render, gTheGame.message[10].image, &(gTheGame.message[10].src), &HP2)) {
                    fprintf(stderr,"Not Copy Tokuten\n");
            }
            if (0 > SDL_RenderCopy(gTheGame.render, gTheGame.message[10].image, &(gTheGame.message[10].src), &HP3)) {
                    fprintf(stderr,"Not Copy Tokuten\n");
            }
            break;
        case 0:
            if (0 > SDL_RenderCopy(gTheGame.render, gTheGame.message[10].image, &(gTheGame.message[10].src), &HP)) {
                    fprintf(stderr,"Not Copy Tokuten\n");
            }
            if (0 > SDL_RenderCopy(gTheGame.render, gTheGame.message[10].image, &(gTheGame.message[10].src), &HP2)) {
                    fprintf(stderr,"Not Copy Tokuten\n");
            }
            if (0 > SDL_RenderCopy(gTheGame.render, gTheGame.message[10].image, &(gTheGame.message[10].src), &HP3)) {
                    fprintf(stderr,"Not Copy Tokuten\n");
            }
            break;
        default:
            break;
        }
    }
    
}

/*****************************************************************
関数名	: Display_Limit
機能	: 経過時間の表示
引数	: int Limit         :制限時間
        int current       :経過時間
出力	: Limit_Time
*****************************************************************/
static void Display_Limit(int Limit_Time){
    
    if(gTheGame.hideFlag==0){
        Limit_Time = map(Limit_Time,0,180,0,180);
        //filledPieColor(gTheGame.render,100,80,50,0,-(Limit_Time/1000),0xffffffff);
        //pieColor(gTheGame.render,90,100,50,180,360,0xffffffff);
        SDL_Rect Sem = {18,18,145,145 };
        if (0 > SDL_RenderCopy(gTheGame.render, gTheGame.message[MS_Sun2].image, &(gTheGame.message[MS_Sun2].src), &Sem)) {
                fprintf(stderr,"Not Copy Massage\n");
        }
        SDL_SetRenderDrawColor(gTheGame.render,0,0,0,255);
        float deg = -(Limit_Time/1000 );
        float rad = deg *(M_PI / 180.0 );
        SDL_Rect test = { 70, 85, 40, 40 };
        test.x = 70+50*cos(rad); test.y = 85+50*sin(rad);
        //SDL_RenderFillRect(gTheGame.render, &test);
        if (0 > SDL_RenderCopy(gTheGame.render, gTheGame.message[MS_Sun].image, &(gTheGame.message[MS_Sun].src), &test)) {
                fprintf(stderr,"Not Copy Massage\n");
        }
        if( gTheGame.c_ID == 0 ){
        //fprintf(stderr,"%d,last%d\n",-(Limit_Time/1000 ) +180,gTheGame.Limit/1000);
        //fprintf(stderr,"%d,%f,%f\n",-(Limit_Time/1000)+270,sin(-(Limit_Time/1000)+270),cos(-(Limit_Time/1000)+270) );
        }
    }

}

/*****************************************************************
関数名	: Display_Mokuzai
機能	: 所有している木材の数を表示
引数	: なし
出力	: なし
*****************************************************************/
static void Display_Mokuzai(void){
    SDL_Rect dst2 = { 150, 500, 150, 100 };
    if (0 > SDL_RenderCopy(gTheGame.render, gTheGame.message[MS_Ki].image, &(gTheGame.message[MS_Ki].src), &dst2)) {
                fprintf(stderr,"Not Copy Massage\n");
    }
    
    SDL_Rect dst = { 200, 500, 50, 50 };
    if (0 > SDL_RenderCopy(gTheGame.render, gTheGame.item[4].image, &(gTheGame.item[4].src), &dst)) {
                fprintf(stderr,"Not Copy Tokuten\n");
    }
    SDL_Rect src = { 0 };
    /* 得点（一桁ずつ） */
    if (0 > SDL_QueryTexture(gTheGame.msgTexts[4], NULL, NULL, &(src.w), &(src.h))) {
          fprintf(stderr,"not3");
    }
    src.w /= 10;

    dst.y += dst.h - 15;
    dst.h     = src.h;
    dst.w     = src.w;
    int pos = gTheGame.chara[gTheGame.c_ID].Mokuzai;
    if(pos==0){
        dst.x+=src.w+35;
        pos/=10;
        dst.x -= src.w;
        src.x = (pos % 10) * src.w;
        
        if (0 > SDL_RenderCopy(gTheGame.render, gTheGame.msgTexts[4], &src, &dst)) {
                fprintf(stderr,"not4");
        }
    }
    for (dst.x += src.w +20; pos; pos /= 10, dst.x -= src.w) {
        src.x = (pos % 10) * src.w;
        if (0 > SDL_RenderCopy(gTheGame.render, gTheGame.msgTexts[4], &src, &dst)) {
              fprintf(stderr,"not4");
        }
    }
}



/*****************************************************************
関数名	: Display_ItemSelect
機能	: アイテムの選択画面を表示
引数	: なし
出力	: なし
*****************************************************************/
static void Display_ItemSelect(void){
    int number;
    //SDL_Rect dst = { 15, 405, 155, 115 };
    SDL_Rect dst = { 15, 445, 155, 115 };
    if (0 > SDL_RenderCopy(gTheGame.render, gTheGame.message[8].image, &(gTheGame.message[8].src), &dst)) {
                fprintf(stderr,"Not Copy Massage\n");
    }
    number=gTheGame.c_ID*5+gTheGame.chuze;

    if(gTheGame.item[number].stts==IS_Open || gTheGame.item[number].stts==IS_Mine){
        gTheGame.message[7].src.y=0;
    }
    else if(gTheGame.item[number].stts==IS_Disable){
        gTheGame.message[7].src.y=200;        
    }
    SDL_Rect dst2 = { 5, 420, 170, 170 };
    if (0 > SDL_RenderCopy(gTheGame.render, gTheGame.message[7].image, &(gTheGame.message[7].src), &dst2)) {
                fprintf(stderr,"Not Copy Massage\n");
    }
    
    
}


/*****************************************************************
関数名	: Display_point
機能	: 獲得しているポイントの値を表示
引数	: なし
出力	: なし
*****************************************************************/
static void Display_point(void){
    if(gTheGame.hideFlag==0){
    SDL_Rect src = { 0 };
    SDL_Rect dst3={195, 105, 20, 20};
    if (0 > SDL_QueryTexture(gTheGame.msgTexts[7], NULL, NULL, &(src.w), &(src.h))) {
        fprintf(stderr,"not4");
    }
    if (0 > SDL_RenderCopy(gTheGame.render, gTheGame.msgTexts[7], &src, &dst3)) {
        fprintf(stderr,"not4");
    }
    if (0 > SDL_QueryTexture(gTheGame.msgTexts[4], NULL, NULL, &(src.w), &(src.h))) {
        fprintf(stderr,"not3");
    }
    SDL_Rect dst2 ={180, 70, 50, 50};
    SDL_Rect dst = {140, 70, 50, 50};
    
    src.w /= 10;

    dst.y += dst.h - 15;
    dst.h     = src.h;
    dst.w     = src.w;

    dst2.y += dst2.h - 15;
    dst2.h     = src.h;
    dst2.w     = src.w;

    int pos = gTheGame.Group1_point;
    int pos2 = gTheGame.Group2_point;

    if(pos==0){
        dst.x+=src.w+35;
        pos/=10;
        dst.x -= src.w;
        src.x = (pos % 10) * src.w;
        if (0 > SDL_RenderCopy(gTheGame.render, gTheGame.msgTexts[4], &src, &dst)) {
                fprintf(stderr,"not4");
        }
    }
    else if(pos>0){
        for (dst.x += src.w+20; pos; pos /= 10, dst.x -= src.w) {
        src.x = (pos % 10) * src.w;
            if (0 > SDL_RenderCopy(gTheGame.render, gTheGame.msgTexts[4], &src, &dst)) {
                fprintf(stderr,"not4");
            }
        }
    }
    if(pos2==0){
        dst2.x+=src.w+35;
        pos2/=10;
        dst2.x -= src.w;
        src.x = (pos2 % 10) * src.w;
        if (0 > SDL_RenderCopy(gTheGame.render, gTheGame.msgTexts[4], &src, &dst2)) {
                fprintf(stderr,"not4");
        }
    }
    else if(pos2>0){
        for (dst2.x += src.w+20; pos2; pos2 /= 10, dst2.x -= src.w) {
        src.x = (pos2 % 10) * src.w;
            if (0 > SDL_RenderCopy(gTheGame.render, gTheGame.msgTexts[4], &src, &dst2)) {
                fprintf(stderr,"not4");
            }
          }
        }
    }
 
  
}



/*****************************************************************
関数名	: TitleWindow
機能	: タイトル画面の表示
引数	: なし
出力	: なし
*****************************************************************/
int TitleWindow(void)
{
   int ret = SDL_RenderCopy(gTheGame.render, gTheGame.title, &(gTheGame.rectBg2), NULL);
    if (ret < 0) {
        fprintf(stderr,"NO title\n");
		return -1;
    }
    SDL_RenderPresent(gTheGame.render);

    return ret;
}

/*****************************************************************
関数名	: WaitWindow
機能	: ロード画面の表示
引数	: なし
出力	: なし
*****************************************************************/
int WaitWindow(void)
{
   int ret = SDL_RenderCopy(gTheGame.render, gTheGame.waitwin, &(gTheGame.rectBg2), NULL);
    if (ret < 0) {
        fprintf(stderr,"NO Wait\n");
		return -1;
    }
    SDL_RenderPresent(gTheGame.render);

    SDL_Rect dst = {0,0,215,215};
    if (0 > SDL_RenderCopy(gTheGame.render, gTheGame.message[15].image, &(gTheGame.message[15].src), &dst)) {
        fprintf(stderr,"Not Copy Massage[%d].\n",16);
    }
    
    return ret;
}


/*****************************************************************
関数名	: RezultWindow
機能	: 勝敗判定画面の表示
引数	: なし
出力	: なし
*****************************************************************/
static void RezultWindow(void)
{
    if(gTheGame.stts==GS_End){
        SDL_Rect dst1_1 = {0, 0, 816,576};
        if (0 > SDL_RenderCopy(gTheGame.render, gTheGame.message[14].image, &(gTheGame.message[14].src), &dst1_1)) {
                fprintf(stderr,"Not Copy Massage\n");
        }
        SDL_Rect dst1_2={250,100,320,200};
        int ij=gTheGame.rizultFLag;
        if (0 > SDL_RenderCopy(gTheGame.render, gTheGame.message[ij].image, &(gTheGame.message[ij].src), &dst1_2)) {
                fprintf(stderr,"Not Copy Massage\n");
        }
        SDL_Rect src = { 0 };
        SDL_Rect dst3={380, 280, 50, 50};
        if (0 > SDL_QueryTexture(gTheGame.msgTexts[2], NULL, NULL, &(src.w), &(src.h))) {
            fprintf(stderr,"not4");
        }
        if (0 > SDL_RenderCopy(gTheGame.render, gTheGame.msgTexts[2], &src, &dst3)) {
            fprintf(stderr,"not4");
        }
        if (0 > SDL_QueryTexture(gTheGame.msgTexts[1], NULL, NULL, &(src.w), &(src.h))) {
            fprintf(stderr,"not3");
        }
        SDL_Rect dst2 ={420, 220, 50, 50};
        SDL_Rect dst = {220, 220, 50, 50};
    
        src.w /= 10;

        dst.y += dst.h - 15;
        dst.h     = src.h;
        dst.w     = src.w;

        dst2.y += dst2.h - 15;
        dst2.h     = src.h;
        dst2.w     = src.w;

        int pos = gTheGame.Group1_point;
        int pos2 = gTheGame.Group2_point;

        if(pos==0){
            dst.x+=src.w+35;
            pos/=10;
            dst.x -= src.w;
            src.x = (pos % 10) * src.w;
            if (0 > SDL_RenderCopy(gTheGame.render, gTheGame.msgTexts[1], &src, &dst)) {
                fprintf(stderr,"not4");
            }
        }
        else if(pos>0){
            for (dst.x += src.w+20; pos; pos /= 10, dst.x -= src.w) {
                src.x = (pos % 10) * src.w;
                if (0 > SDL_RenderCopy(gTheGame.render, gTheGame.msgTexts[1], &src, &dst)) {
                    fprintf(stderr,"not4");
                 }
            }
        }
        if(pos2==0){
            dst2.x+=src.w+35;
            pos2/=10;
            dst2.x -= src.w;
            src.x = (pos2 % 10) * src.w;
            if (0 > SDL_RenderCopy(gTheGame.render, gTheGame.msgTexts[1], &src, &dst2)) {
                fprintf(stderr,"not4");
            }
        }
        else if(pos2>0){
            for (dst2.x += src.w+20; pos2; pos2 /= 10, dst2.x -= src.w) {
            src.x = (pos2 % 10) * src.w;
                if (0 > SDL_RenderCopy(gTheGame.render, gTheGame.msgTexts[1], &src, &dst2)) {
                    fprintf(stderr,"not4");
                }
             }
        }
        if(gTheGame.c_ID==0 || gTheGame.c_ID==2){
            for(int i=0;i<gTheGame.charaNum;i+=2){
                SDL_Rect dst = gTheGame.chara[i].rect13;
                if(i==0){
                    dst.x=400;
                    dst.y=400;
                }
                else if(i==2){
                    dst.x=100;
                    dst.y=400;
                }
                if(gTheGame.rizultFLag==0){
                    gTheGame.chara[i].src13.y=0;
                }
                else if(gTheGame.rizultFLag==1){
                    gTheGame.chara[i].src13.y=216;
                }
                else if(gTheGame.rizultFLag==2){
                    gTheGame.chara[i].src13.y=0;
                }
                if (0 > SDL_RenderCopy(gTheGame.render, gTheGame.chara[i].image13, 
                &(gTheGame.chara[i].src13), &dst)) {
                    fprintf(stderr,"Not Copy chara[%d].^\n",i);
                }
            }
               
        }
        else if(gTheGame.c_ID==1 || gTheGame.c_ID==3){
            for(int i=1;i<gTheGame.charaNum;i+=2){
                SDL_Rect dst = gTheGame.chara[i].rect13;
                if(i==1){
                    dst.x=400;
                    dst.y=400;
                }
                else if(i==3){
                    dst.x=100;
                    dst.y=400;
                }
                if(gTheGame.rizultFLag==0){
                    gTheGame.chara[i].src13.y=0;
                }
                else if(gTheGame.rizultFLag==1){
                    gTheGame.chara[i].src13.y=216;
                }
                else if(gTheGame.rizultFLag==2){
                    gTheGame.chara[i].src13.y=0;
                }
                if (0 > SDL_RenderCopy(gTheGame.render, gTheGame.chara[i].image13, 
                &(gTheGame.chara[i].src13), &dst)) {
                    fprintf(stderr,"Not Copy chara[%d].^\n",i);
                }
            }
               
        }
  
    }
    
}



/*****************************************************************
関数名	: Display_BaseHP
機能	: キャラクターと拠点が接したとき拠点のHPを表示
引数	: なし
出力	: なし
*****************************************************************/
static void Display_BaseHP(void){
    for( int c = 0; c < gTheGame.charaNum; c++ ){
        for( int i = 0; i < gTheGame.baseNum; i++ ){
            if( gTheGame.chara[c].HPFlag == 1 ){
                if( gTheGame.base[i].stts == BS_Group1 && i == gTheGame.chara[c].basecollision ){
                    if( gTheGame.chara[c].basecollision == 0 ){
                        int x = gTheGame.base[0].hp_A * 5;
                        int x2 = 0;
                        //外側の白色の枠を表示
                        SDL_Rect rect = { gTheGame.base[0].rect.x - gTheGame.rectMap.x + 112-(x2/2), gTheGame.base[0].rect.y - gTheGame.rectMap.y -52 ,77+x2,24 };
                        SDL_SetRenderDrawColor(gTheGame.render, 255, 255, 255, 255);
                        SDL_RenderFillRect(gTheGame.render,&rect);
                        //HPが減少している分を黒色で表示
                        SDL_Rect hp_black = { gTheGame.base[0].rect.x - gTheGame.rectMap.x + 113-(x2/2),gTheGame.base[0].rect.y - gTheGame.rectMap.y -50, 75+x2, 20 };  
                        SDL_SetRenderDrawColor(gTheGame.render, 0, 0, 0, 255);
                        SDL_RenderFillRect(gTheGame.render, &hp_black);
                        //現在のHPを表示
                        SDL_Rect hp = { gTheGame.base[0].rect.x - gTheGame.rectMap.x + 113-(x2/2),gTheGame.base[0].rect.y - gTheGame.rectMap.y -50, x, 20 };  
                        SDL_SetRenderDrawColor(gTheGame.render, 255, 0, 0, 255);
                        SDL_RenderFillRect(gTheGame.render, &hp);
                        //fprintf(stderr,"baseA0_rect(%d,%d)upgrade[%d]x2(%d)\n",gTheGame.base[0].rect.x,gTheGame.base[0].rect.y,gTheGame.base[0].upgradeFlag,x2);
                    }else if( gTheGame.chara[c].basecollision == 1){
                        int x = gTheGame.base[1].hp_A * 5;
                        int x2 = 0;
                        //外側の白色の枠を表示
                        SDL_Rect rect = { gTheGame.base[1].rect.x - gTheGame.rectMap.x + 112-(x2/2), gTheGame.base[1].rect.y - gTheGame.rectMap.y -52 ,77+x2,24 };
                        SDL_SetRenderDrawColor(gTheGame.render, 255, 255, 255, 255);
                        SDL_RenderFillRect(gTheGame.render,&rect);
                        //HPが減少している分を黒色で表示
                        SDL_Rect hp_black = { gTheGame.base[1].rect.x - gTheGame.rectMap.x + 113-(x2/2),gTheGame.base[1].rect.y - gTheGame.rectMap.y -50, 75+x2, 20 };  
                        SDL_SetRenderDrawColor(gTheGame.render, 0, 0, 0, 255);
                        SDL_RenderFillRect(gTheGame.render, &hp_black);
                        //現在のHPを表示
                        SDL_Rect hp = { gTheGame.base[1].rect.x - gTheGame.rectMap.x + 113-(x2/2),gTheGame.base[1].rect.y - gTheGame.rectMap.y-50, x, 20 };  
                        SDL_SetRenderDrawColor(gTheGame.render, 255, 0, 0, 255);
                        SDL_RenderFillRect(gTheGame.render, &hp);
                        //fprintf(stderr,"1_rect(%d,%d)upgrade[%d]x2(%d)\n",gTheGame.base[1].rect.x,gTheGame.base[1].rect.y,gTheGame.base[1].upgradeFlag,x2);
                    } else if( gTheGame.chara[c].basecollision == 2 ){
                        int x = gTheGame.base[2].hp_A * 5;
                        int x2 = 0;
                        //外側の白色の枠を表示
                        SDL_Rect rect = { gTheGame.base[2].rect.x - gTheGame.rectMap.x + 112-(x2/2), gTheGame.base[2].rect.y - gTheGame.rectMap.y -52 ,77+x2,24 };
                        SDL_SetRenderDrawColor(gTheGame.render, 255, 255, 255, 255);
                        SDL_RenderFillRect(gTheGame.render,&rect);
                        //HPが減少している分を黒色で表示
                        SDL_Rect hp_black = { gTheGame.base[2].rect.x - gTheGame.rectMap.x + 113-(x2/2),gTheGame.base[2].rect.y - gTheGame.rectMap.y -50, 75+x2, 20 };  
                        SDL_SetRenderDrawColor(gTheGame.render, 0, 0, 0, 255);
                        SDL_RenderFillRect(gTheGame.render, &hp_black);
                        //現在のHPを表示
                        SDL_Rect hp = { gTheGame.base[2].rect.x - gTheGame.rectMap.x + 113-(x2/2),gTheGame.base[2].rect.y - gTheGame.rectMap.y-50, x, 20 };  
                        SDL_SetRenderDrawColor(gTheGame.render, 255, 0, 0, 255);
                        SDL_RenderFillRect(gTheGame.render, &hp);
                        //fprintf(stderr,"baseA2_rect(%d,%d)upgrade[%d]x2(%d)\n",gTheGame.base[2].rect.x,gTheGame.base[2].rect.y,gTheGame.base[2].upgradeFlag,x2);
                    }
                }
                else if( gTheGame.base[i].stts == BS_Group1_P && i == gTheGame.chara[c].basecollision ){
                    if( gTheGame.chara[c].basecollision == 0 ){
                        int x = gTheGame.base[0].hp_A * 5;
                        int x2 = 15* 5;
                        //外側の白色の枠を表示
                        SDL_Rect rect = { gTheGame.base[0].rect.x - gTheGame.rectMap.x + 112-(x2/2), gTheGame.base[0].rect.y - gTheGame.rectMap.y -52 ,77+x2,24 };
                        SDL_SetRenderDrawColor(gTheGame.render, 255, 255, 255, 255);
                        SDL_RenderFillRect(gTheGame.render,&rect);
                        //HPが減少している分を黒色で表示
                        SDL_Rect hp_black = { gTheGame.base[0].rect.x - gTheGame.rectMap.x + 113-(x2/2),gTheGame.base[0].rect.y - gTheGame.rectMap.y -50, 75+x2, 20 };  
                        SDL_SetRenderDrawColor(gTheGame.render, 0, 0, 0, 255);
                        SDL_RenderFillRect(gTheGame.render, &hp_black);
                        //現在のHPを表示
                        SDL_Rect hp = { gTheGame.base[0].rect.x - gTheGame.rectMap.x + 113-(x2/2),gTheGame.base[0].rect.y - gTheGame.rectMap.y -50, x, 20 };  
                        SDL_SetRenderDrawColor(gTheGame.render, 255, 0, 0, 255);
                        SDL_RenderFillRect(gTheGame.render, &hp);
                        //fprintf(stderr,"baseA0_rect(%d,%d)upgrade[%d]x2(%d)\n",gTheGame.base[0].rect.x,gTheGame.base[0].rect.y,gTheGame.base[0].upgradeFlag,x2);
                    }else if( gTheGame.chara[c].basecollision == 1){
                        int x = gTheGame.base[1].hp_A * 5;
                        int x2 = 15*5;
                        //外側の白色の枠を表示
                        SDL_Rect rect = { gTheGame.base[1].rect.x - gTheGame.rectMap.x + 112-(x2/2), gTheGame.base[1].rect.y - gTheGame.rectMap.y -52 ,77+x2,24 };
                        SDL_SetRenderDrawColor(gTheGame.render, 255, 255, 255, 255);
                        SDL_RenderFillRect(gTheGame.render,&rect);
                        //HPが減少している分を黒色で表示
                        SDL_Rect hp_black = { gTheGame.base[1].rect.x - gTheGame.rectMap.x + 113-(x2/2),gTheGame.base[1].rect.y - gTheGame.rectMap.y -50, 75+x2, 20 };  
                        SDL_SetRenderDrawColor(gTheGame.render, 0, 0, 0, 255);
                        SDL_RenderFillRect(gTheGame.render, &hp_black);
                        //現在のHPを表示
                        SDL_Rect hp = { gTheGame.base[1].rect.x - gTheGame.rectMap.x + 113-(x2/2),gTheGame.base[1].rect.y - gTheGame.rectMap.y-50, x, 20 };  
                        SDL_SetRenderDrawColor(gTheGame.render, 255, 0, 0, 255);
                        SDL_RenderFillRect(gTheGame.render, &hp);
                        //fprintf(stderr,"1_rect(%d,%d)upgrade[%d]x2(%d)\n",gTheGame.base[1].rect.x,gTheGame.base[1].rect.y,gTheGame.base[1].upgradeFlag,x2);
                    } else if( gTheGame.chara[c].basecollision == 2 ){
                        int x = gTheGame.base[2].hp_A * 5;
                        int x2 = 15*5;
                        //外側の白色の枠を表示
                        SDL_Rect rect = { gTheGame.base[2].rect.x - gTheGame.rectMap.x + 112-(x2/2), gTheGame.base[2].rect.y - gTheGame.rectMap.y -52 ,77+x2,24 };
                        SDL_SetRenderDrawColor(gTheGame.render, 255, 255, 255, 255);
                        SDL_RenderFillRect(gTheGame.render,&rect);
                        //HPが減少している分を黒色で表示
                        SDL_Rect hp_black = { gTheGame.base[2].rect.x - gTheGame.rectMap.x + 113-(x2/2),gTheGame.base[2].rect.y - gTheGame.rectMap.y -50, 75+x2, 20 };  
                        SDL_SetRenderDrawColor(gTheGame.render, 0, 0, 0, 255);
                        SDL_RenderFillRect(gTheGame.render, &hp_black);
                        //現在のHPを表示
                        SDL_Rect hp = { gTheGame.base[2].rect.x - gTheGame.rectMap.x + 113-(x2/2),gTheGame.base[2].rect.y - gTheGame.rectMap.y-50, x, 20 };  
                        SDL_SetRenderDrawColor(gTheGame.render, 255, 0, 0, 255);
                        SDL_RenderFillRect(gTheGame.render, &hp);
                        //fprintf(stderr,"baseA2_rect(%d,%d)upgrade[%d]x2(%d)\n",gTheGame.base[2].rect.x,gTheGame.base[2].rect.y,gTheGame.base[2].upgradeFlag,x2);
                    }
                }
                else if( gTheGame.base[i].stts == BS_Group2 && i == gTheGame.chara[c].basecollision ){
                    if( gTheGame.chara[c].basecollision == 0 ){
                        int xb = gTheGame.base[0].hp_B * 5;
                        int xb2 = 0;
                        //外側の白色の枠を表示
                        SDL_Rect rect = { gTheGame.base[0].rect.x - gTheGame.rectMap.x + 112-(xb2/2), gTheGame.base[0].rect.y - gTheGame.rectMap.y -52 ,77+xb2,24 };
                        SDL_SetRenderDrawColor(gTheGame.render, 255, 255, 255, 255);
                        SDL_RenderFillRect(gTheGame.render,&rect);
                        //HPが減少している分を黒色で表示
                        SDL_Rect hp_black = { gTheGame.base[0].rect.x - gTheGame.rectMap.x + 113-(xb2/2),gTheGame.base[0].rect.y - gTheGame.rectMap.y -50, 75+xb2, 20 };  
                        SDL_SetRenderDrawColor(gTheGame.render, 0, 0, 0, 255);
                        SDL_RenderFillRect(gTheGame.render, &hp_black);
                        //現在のHPを表示
                        SDL_Rect hp = { gTheGame.base[0].rect.x - gTheGame.rectMap.x + 113-(xb2/2),gTheGame.base[0].rect.y - gTheGame.rectMap.y -50, xb, 20 };  
                        SDL_SetRenderDrawColor(gTheGame.render, 0, 0, 255, 255);
                        SDL_RenderFillRect(gTheGame.render, &hp);
                        //fprintf(stderr,"baseB0_rect(%d,%d)upgrade[%d]xb2(%d)\n",gTheGame.base[0].rect.x,gTheGame.base[0].rect.y,gTheGame.base[0].upgradeFlag,xb2);
                    }else if( gTheGame.chara[c].basecollision == 1 ){
                        int xb = gTheGame.base[1].hp_B * 5;
                        int xb2 = 0;
                        //外側の白色の枠を表示
                        SDL_Rect rect = { gTheGame.base[1].rect.x - gTheGame.rectMap.x + 112-(xb2/2), gTheGame.base[1].rect.y - gTheGame.rectMap.y -52 ,77+xb2,24 };
                        SDL_SetRenderDrawColor(gTheGame.render, 255, 255, 255, 255);
                        SDL_RenderFillRect(gTheGame.render,&rect);
                        //HPが減少している分を黒色で表示
                        SDL_Rect hp_black = { gTheGame.base[1].rect.x - gTheGame.rectMap.x + 113-(xb2/2),gTheGame.base[1].rect.y - gTheGame.rectMap.y -50, 75+xb2, 20 };  
                        SDL_SetRenderDrawColor(gTheGame.render, 0, 0, 0, 255);
                        SDL_RenderFillRect(gTheGame.render, &hp_black);
                        //現在のHPを表示
                        SDL_Rect hp = { gTheGame.base[1].rect.x - gTheGame.rectMap.x + 113-(xb2/2),gTheGame.base[1].rect.y - gTheGame.rectMap.y-50, xb, 20 };  
                        SDL_SetRenderDrawColor(gTheGame.render, 0, 0, 255, 255);
                        SDL_RenderFillRect(gTheGame.render, &hp);
                        //fprintf(stderr,"baseB1_rect(%d,%d)upgrade[%d]xb2(%d)\n",gTheGame.base[1].rect.x,gTheGame.base[1].rect.y,gTheGame.base[1].upgradeFlag,xb2);
                    } else if( gTheGame.chara[c].basecollision == 2 ){
                        int xb = gTheGame.base[2].hp_B * 5;
                        int xb2 = 0;
                        //外側の白色の枠を表示
                        SDL_Rect rect = { gTheGame.base[2].rect.x - gTheGame.rectMap.x + 112-(xb2/2), gTheGame.base[2].rect.y - gTheGame.rectMap.y -52 ,77+xb2,24 };
                        SDL_SetRenderDrawColor(gTheGame.render, 255, 255, 255, 255);
                        SDL_RenderFillRect(gTheGame.render,&rect);
                        //HPが減少している分を黒色で表示
                        SDL_Rect hp_black = { gTheGame.base[2].rect.x - gTheGame.rectMap.x + 113-(xb2/2),gTheGame.base[2].rect.y - gTheGame.rectMap.y -50, 75+xb2, 20 };  
                        SDL_SetRenderDrawColor(gTheGame.render, 0, 0, 0, 255);
                        SDL_RenderFillRect(gTheGame.render, &hp_black);
                        //現在のHPを表示
                        SDL_Rect hp = { gTheGame.base[2].rect.x - gTheGame.rectMap.x + 113-(xb2/2),gTheGame.base[2].rect.y - gTheGame.rectMap.y-50, xb, 20 };  
                        SDL_SetRenderDrawColor(gTheGame.render, 0, 0, 255, 255);
                        SDL_RenderFillRect(gTheGame.render, &hp);
                        //fprintf(stderr,"baseB2_rect(%d,%d)upgrade[%d]xb2(%d)\n",gTheGame.base[2].rect.x,gTheGame.base[2].rect.y,gTheGame.base[2].upgradeFlag,xb2);
                    }
            }
            else if( gTheGame.base[i].stts == BS_Group2_P && i == gTheGame.chara[c].basecollision ){
                   if( gTheGame.chara[c].basecollision == 0 ){
                        int xb = gTheGame.base[0].hp_B * 5;
                        int xb2 = 15*5;
                        //外側の白色の枠を表示
                        SDL_Rect rect = { gTheGame.base[0].rect.x - gTheGame.rectMap.x + 112-(xb2/2), gTheGame.base[0].rect.y - gTheGame.rectMap.y -52 ,77+xb2,24 };
                        SDL_SetRenderDrawColor(gTheGame.render, 255, 255, 255, 255);
                        SDL_RenderFillRect(gTheGame.render,&rect);
                        //HPが減少している分を黒色で表示
                        SDL_Rect hp_black = { gTheGame.base[0].rect.x - gTheGame.rectMap.x + 113-(xb2/2),gTheGame.base[0].rect.y - gTheGame.rectMap.y -50, 75+xb2, 20 };  
                        SDL_SetRenderDrawColor(gTheGame.render, 0, 0, 0, 255);
                        SDL_RenderFillRect(gTheGame.render, &hp_black);
                        //現在のHPを表示
                        SDL_Rect hp = { gTheGame.base[0].rect.x - gTheGame.rectMap.x + 113-(xb2/2),gTheGame.base[0].rect.y - gTheGame.rectMap.y -50, xb, 20 };  
                        SDL_SetRenderDrawColor(gTheGame.render, 0, 0, 255, 255);
                        SDL_RenderFillRect(gTheGame.render, &hp);
                        //fprintf(stderr,"baseB0_rect(%d,%d)upgrade[%d]xb2(%d)\n",gTheGame.base[0].rect.x,gTheGame.base[0].rect.y,gTheGame.base[0].upgradeFlag,xb2);
                    }else if( gTheGame.chara[c].basecollision == 1 ){
                        int xb = gTheGame.base[1].hp_B * 5;
                        int xb2 = 15*5;
                        //外側の白色の枠を表示
                        SDL_Rect rect = { gTheGame.base[1].rect.x - gTheGame.rectMap.x + 112-(xb2/2), gTheGame.base[1].rect.y - gTheGame.rectMap.y -52 ,77+xb2,24 };
                        SDL_SetRenderDrawColor(gTheGame.render, 255, 255, 255, 255);
                        SDL_RenderFillRect(gTheGame.render,&rect);
                        //HPが減少している分を黒色で表示
                        SDL_Rect hp_black = { gTheGame.base[1].rect.x - gTheGame.rectMap.x + 113-(xb2/2),gTheGame.base[1].rect.y - gTheGame.rectMap.y -50, 75+xb2, 20 };  
                        SDL_SetRenderDrawColor(gTheGame.render, 0, 0, 0, 255);
                        SDL_RenderFillRect(gTheGame.render, &hp_black);
                        //現在のHPを表示
                        SDL_Rect hp = { gTheGame.base[1].rect.x - gTheGame.rectMap.x + 113-(xb2/2),gTheGame.base[1].rect.y - gTheGame.rectMap.y-50, xb, 20 };  
                        SDL_SetRenderDrawColor(gTheGame.render, 0, 0, 255, 255);
                        SDL_RenderFillRect(gTheGame.render, &hp);
                        //fprintf(stderr,"baseB1_rect(%d,%d)upgrade[%d]xb2(%d)\n",gTheGame.base[1].rect.x,gTheGame.base[1].rect.y,gTheGame.base[1].upgradeFlag,xb2);
                    } else if( gTheGame.chara[c].basecollision == 2 ){
                        int xb = gTheGame.base[2].hp_B * 5;
                        int xb2 = 15*5;
                        //外側の白色の枠を表示
                        SDL_Rect rect = { gTheGame.base[2].rect.x - gTheGame.rectMap.x + 112-(xb2/2), gTheGame.base[2].rect.y - gTheGame.rectMap.y -52 ,77+xb2,24 };
                        SDL_SetRenderDrawColor(gTheGame.render, 255, 255, 255, 255);
                        SDL_RenderFillRect(gTheGame.render,&rect);
                        //HPが減少している分を黒色で表示
                        SDL_Rect hp_black = { gTheGame.base[2].rect.x - gTheGame.rectMap.x + 113-(xb2/2),gTheGame.base[2].rect.y - gTheGame.rectMap.y -50, 75+xb2, 20 };  
                        SDL_SetRenderDrawColor(gTheGame.render, 0, 0, 0, 255);
                        SDL_RenderFillRect(gTheGame.render, &hp_black);
                        //現在のHPを表示
                        SDL_Rect hp = { gTheGame.base[2].rect.x - gTheGame.rectMap.x + 113-(xb2/2),gTheGame.base[2].rect.y - gTheGame.rectMap.y-50, xb, 20 };  
                        SDL_SetRenderDrawColor(gTheGame.render, 0, 0, 255, 255);
                        SDL_RenderFillRect(gTheGame.render, &hp);
                        //fprintf(stderr,"baseB2_rect(%d,%d)upgrade[%d]xb2(%d)\n",gTheGame.base[2].rect.x,gTheGame.base[2].rect.y,gTheGame.base[2].upgradeFlag,xb2);
                    }
                }
            }
            else{
                //fprintf(stderr,"ok2\n");
            }
             /*fprintf(stderr,"base%d_stts(%d)upgrade[%d],chara%d_basecollision(%d)HPFlag(%d)\n",i,gTheGame.base[i].stts,gTheGame.base[i].upgradeFlag,
            c,gTheGame.chara[c].basecollision,gTheGame.chara[c].HPFlag);*/
        }
    }
    SDL_SetRenderDrawColor(gTheGame.render,0,0,0,255);
}



/*****************************************************************
関数名	: Display_charahead
機能	: キャラクターの頭の上に矢印を表示
引数	: なし
出力	: なし
*****************************************************************/
static void Display_charahead(void){ 
    int i;
    int x,y;
    for( i = 0; i < gTheGame.charaNum; i ++ ){
        if(gTheGame.chara[i].stts==CS_Ghost){
            x=gTheGame.chara[i].rect3.x- gTheGame.rectMap.x +25;
            y=gTheGame.chara[i].rect3.y- gTheGame.rectMap.y -52;
        }
        else{
            x=gTheGame.chara[i].rect.x- gTheGame.rectMap.x +25;
            y=gTheGame.chara[i].rect.y- gTheGame.rectMap.y -52;
        }
        //SDL_Rect dst = {gTheGame.chara[i].rect.x- gTheGame.rectMap.x +25 , gTheGame.chara[i].rect.y- gTheGame.rectMap.y -52, 100, 100};
        SDL_Rect dst = {x,y, 100, 100};
        if( i == 0 || i == 2 ){
            if (0 > SDL_RenderCopy(gTheGame.render, gTheGame.message[6].image, &(gTheGame.message[6].src), &dst)) {
                fprintf(stderr,"Not Copy head\n");
            }
        }else if( i == 1 || i == 3 ){
            if (0 > SDL_RenderCopy(gTheGame.render, gTheGame.message[3].image, &(gTheGame.message[3].src), &dst)) {
                fprintf(stderr,"Not Copy head\n");
            }
        }
    }
}