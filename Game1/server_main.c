/*****************************************************************
�ե�����̾	: server_main.c
��ǽ		: �����С��Υᥤ��롼����
*****************************************************************/
//(�ѹ���ʬ���������ޥ�ɤ��������ؿ�SendRecvManager�򥹥�åɤȤ��Ƶ�ư����褦���ѹ�)
//(2018/11/17)
#include"server_common.h"
#include"server_system.h"
#include"server_func.h"

static Uint32 SignalHandler(Uint32 interval, void *param);
static Uint32 Timer1(Uint32 interval, void *param);
static Uint32 Timer2(Uint32 interval, void *param);

static int NetworkEvent(void *data);

//CS_Die��CS_Ghost�ΤȤ���5s��˾����Ѳ����뤿��λ��ַ�¬��(2018.12.9)
unsigned int previous_time[4];
unsigned int current_time[4];
int flag_ghost[4];//����previous_time�ι����ѤΥե饰(2018.12.9)
unsigned int severtime;

int main(int argc,char *argv[])
{
	int	num;
	int	endFlag = 1;
	SDL_Thread	*thr;	
	/* �����������å� */
	if(argc != 2){
		fprintf(stderr,"Usage: number of clients\n");
		exit(-1);
	}
	if((num = atoi(argv[1])) < 0 ||  num > MAX_CLIENTS){
		fprintf(stderr,"clients limit = %d \n",MAX_CLIENTS);
		exit(-1);
	}
	gTheGame.numnum = num;
	/* SDL�ν���� */
	if(SDL_Init(SDL_INIT_TIMER) < 0) {
		printf("failed to initialize SDL.\n");
		exit(-1);
	}

	/* ���饤����ȤȤ���³ */
	if(SetUpServer(num) == -1){
		fprintf(stderr,"Cannot setup server\n");
		exit(-1);
	}
	/*�����ƥ�ν����*/
    if (InitSystemS() < 0) {
        fprintf(stderr,"failed to initialize System\n");
        return -1;
    }
	 /* �����ޡ� */
    SDL_TimerID timer1 = SDL_AddTimer(200, Timer1, NULL);
    if(timer1==0){
        fprintf(stderr,"Not timer1\n");
        return -1;
    }
	/* �����ޡ�:CS_Wood���ڤ��ؤ� (2019.1.12)*/
    SDL_TimerID timer2 = SDL_AddTimer(400, Timer2, NULL);
    if(timer2==0){
        fprintf(stderr,"Not timer2\n");
        return -1;
    }
	
	/* �ͥåȥ�����٥�Ƚ�������åɤκ���(�ɲ���ʬ 2018/11/17)*/
	thr = SDL_CreateThread(NetworkEvent,"NetworkThread", &endFlag);
	
	while(gTheGame.love.start == SDL_FALSE){//(2019.1.12)
		if(endFlag == 0)break;
	}
	Set_Object();
	SendObjectXY();
	
	
	severtime=SDL_GetTicks();
	/* �ᥤ�󥤥٥�ȥ롼�� */
	while(endFlag){
	if(gTheGame.stts==GS_Playing){
		for(int i = 0; i < gTheGame.charaNum; i++){
            UpdateCharaStatus(&(gTheGame.chara[i]),0);
			//�����ΰ�ư�׻�
            MoveCharaS(&(gTheGame.chara[i]));
			ChuzeItem(&(gTheGame.chara[i]),i);
        }
		for(int f = 0; f < gTheGame.ItemNum; f++){
            //�����ƥ�ΰ�ư�׻�
            MoveItem(&(gTheGame.item[f]));
        }
		for(int m = 0; m < gTheGame.MokuNum; m++){
            //�����ƥ�ΰ�ư�׻�
            MoveMokuzai(&(gTheGame.mokuzai[m]));
        }
		//�����ΰ�ư�׻�(�ɲ���ʬ��2018/11.26)
		for(int s = 0; s < gTheGame.BaseNum; s++){
			MoveBase(&(gTheGame.base[s]));
		}
		//�㳲ʪ�ΰ�ư�׻�(�ɲ���ʬ��2018/12.8)
		for(int o = 0; o < gTheGame.ObjeNum; o++){
			MoveObject(&(gTheGame.object[o]));
		}
		for(int k = 0; k < gTheGame.FallNum; k++){
            //��Ȥ���ΰ�ư�׻�
            MoveFall(&(gTheGame.fall[k]));
        }
		for (int i = 0; i < gTheGame.charaNum; i++) {
				//�����ȥ����ƥ��������Ƚ��
				for (int g = 0; g < gTheGame.ItemNum; g++) {
                    CollisionItem(&(gTheGame.chara[i]), &(gTheGame.item[g]),g,i);
                }
				//�����ȵ�����������Ƚ��
				for (int c = 0; c < gTheGame.BaseNum; c++) {
                    CollisionCharaBase(&(gTheGame.chara[i]), &(gTheGame.base[c]));
                }
				//��������Ȥ����������Ƚ��(2018.12.21)
				for (int m = 0; m < gTheGame.FallNum; m++) {
                   CollisionFall(&(gTheGame.chara[i]), &(gTheGame.fall[m]),i,m);
                }
				//�������ں��������Ƚ��(2018.12.21)
				for (int m = 0; m < gTheGame.MokuNum; m++) {
                   CollisionMokuzai(&(gTheGame.mokuzai[m]),&(gTheGame.chara[i]));
                }
        }
		for (int b = 0; b < gTheGame.ItemNum; b++) {
				//�����ƥ�ȵ�����������Ƚ��
                for (int r = 0; r < gTheGame.BaseNum; r++) {
                    CollisionBase(&(gTheGame.item[b]), &(gTheGame.base[r]),r,b);
                }
				//�����ƥ�Ⱦ㳲ʪ��������Ƚ��
				for (int s = 0; s < gTheGame.ObjeNum; s++) {
                    CollisionWood(&(gTheGame.item[b]), &(gTheGame.object[s]));
                }
		}
		//CD_Ghost��CS_Enable�ؤλ��֤ˤ���Ѳ�(2018.12.9)
		for (int i = 0; i < gTheGame.charaNum; i++) {
			if(gTheGame.chara[i].stts == CS_Die){
				if(flag_ghost[i] == 0){//���Τ߹���
					previous_time[i] = SDL_GetTicks();
					flag_ghost[i] = 1;
				}
				
				current_time[i] = SDL_GetTicks(); //�в���֤��������ѿ��˳�Ǽ
				if (current_time[i] > previous_time[i] + 5000){ //���Υ����ߥ󥰤���1�÷в�
					previous_time[i] = current_time[i]; //���Υ����ߥ󥰤򸽺ߤǹ���
					
					Ghost(&(gTheGame.chara[i]), i);
				}
			}else if(gTheGame.chara[i].stts == CS_Ghost){
				current_time[i] = SDL_GetTicks(); //�в���֤��������ѿ��˳�Ǽ
				if (current_time[i] > previous_time[i] + 10000){ //���Υ����ߥ󥰤���1�÷в�
					previous_time[i] = current_time[i]; //���Υ����ߥ󥰤򸽺ߤǹ���
					Ghost(&(gTheGame.chara[i]), i);
					//UpdateCharaStatus(&(gTheGame.chara[i]));
					flag_ghost[i] = 0;
				}
			}else if(gTheGame.chara[i].stts == CS_Fallout){
				if(flag_ghost[i] == 0){//���Τ߹���
					previous_time[i] = SDL_GetTicks();
					flag_ghost[i] = 1;
				}
				
				current_time[i] = SDL_GetTicks(); //�в���֤��������ѿ��˳�Ǽ
				if (current_time[i] > previous_time[i] + 5000){ //���Υ����ߥ󥰤���1�÷в�
					previous_time[i] = current_time[i]; //���Υ����ߥ󥰤򸽺ߤǹ���
					
					Ghost(&(gTheGame.chara[i]), i);
					flag_ghost[i] = 0;
				}
			}
			
		}
		//�����å�(2018.12.22)
		for (int b = 0; b < gTheGame.charaNum; b++) {
			if(gTheGame.chara[b].stts == CS_Attack1){
				int bi = gTheGame.chara[b].i_num;
				if(gTheGame.item[bi].type == IT_Scop){
					MakePitfall(&(gTheGame.chara[b]),&(gTheGame.item[bi]),b);
				}
			}
		}
		//�������κ�ɸ������
		SendHP();
		//��������HP������(�ɲ���ʬ 2018/11/26)
		SendXY();
		SendStts();
		SendSttO();
		
		//(�ɲ���ʬ	2018/12/20)//////
		SendTime(severtime);
		if(gTheGame.Limit<=0 && gTheGame.stts==GS_Playing){
			Send_Rizult_Command();
			gTheGame.stts=GS_End;
		}
		}
		
		//�����ޤ�////////////////
		SDL_Delay(10);
	}
	SDL_RemoveTimer(timer1);
	SDL_RemoveTimer(timer2);
	DestroySystemS();
	SDL_WaitThread(thr,NULL);
	/* ��λ���� */
	Ending();

	return 0;
}
//(�ɲ���ʬ�����ͥåȥ���Υ��٥�ȥ롼�פδؿ����ɲ�)
/*****************************************************************
�ؿ�̾  : NetworkEvent
��ǽ    : �ͥåȥ���Υ��٥�ȥ롼��
����    : void		*data		: ��λ�ե饰
����    : ����åɽ�λ����0���֤�
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

//���ɲôؿ�  2018/12/8��
/*****************************************************************
�ؿ�̾  : Timer1
��ǽ    :�����ޡ�����1(����饯������̵Ũ���֤ι���)
����    : interval    ���֤Υ����ޡ�
����2   : parm    ���֤Υѥ�᡼����
����    : ����åɽ�λ����0���֤�
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

/*CS_Wood���ڤ��ؤ�(2019.1.12)*/
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