/*****************************************************************
�ե�����̾	: server_command.c
��ǽ		: �����С��Υ��ޥ�ɽ���
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
�ؿ�̾	: ExecuteCommand
��ǽ	: ���饤����Ȥ��������Ƥ������ޥ�ɤ򸵤ˡ�
		����������������¹Ԥ���
����	: char	command	  : ���ޥ��
		int	pos			: ���ޥ�ɤ����ä����饤������ֹ�
����	: �ץ���ཪλ���ޥ�ɤ������Ƥ������ˤ�0���֤���
		����ʳ���1���֤�
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
    /* �����������å� */
    assert(0<=pos && pos<MAX_CLIENTS);

#ifndef NDEBUG
    printf("#####\n");
    printf("ExecuteCommand()\n");
    printf("Get command %c\n",command);
#endif
    switch(command){
        case END_COMMAND:
			dataSize = 0;
			/* ���ޥ�ɤΥ��å� */
			SetCharData2DataBlock(data,command,&dataSize);
			
			/* ���桼���������� */
			SendData(ALL_CLIENTS,data,dataSize);

			endFlag = 0;
			break;
	    case Mov_COMMAND:
            //���饤����Ȥ��顢���饤������ֹ�����
		    RecvIntData(pos,&intData);
            //���饤����Ȥ��顢���饤����Ȥ����������������
            RecvIntData(pos,&intData2);

            rad = ChangefloatData(intData2);
           
            DirS(&(gTheGame.chara[pos]),rad);
			break;
       
        case Att_COMMAND:
            //fprintf(stderr,"mask_att[%d]\n",pos);
            //���饤����Ȥ��顢���饤������ֹ�����
		    RecvIntData(pos,&intData);

            //���饤����Ȥ��顢����ѥ���������
            RecvIntData(pos,&intData2);
            dataSize = 0;
            //a�ܥ���򲡤����Ȥ�
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
            //b�ܥ���򲡤���
            else if(intData2 == 1){
                if(gTheGame.chara[pos].stts==CS_Enable){
                    gTheGame.chara[intData].stts=CS_Chuze;
                }
            }
            //a��b�ܥ����Υ����
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
            //���饤����Ȥ��顢���饤������ֹ�����
		    RecvIntData(pos,&intData);
            gTheGame.c_start++;
            fprintf(stderr,"c_start = %d\n",gTheGame.c_start);
            fprintf(stderr,"numnum = %d\n",gTheGame.numnum);
            dataSize = 0;

            if(gTheGame.c_start == gTheGame.numnum){//�Ϳ����Ѥ���
                /* ���ޥ�ɤΥ��å� */
			    SetCharData2DataBlock(data,Start_COMMAND,&dataSize);
                 /* ���桼���������� */
			    SendData(ALL_CLIENTS,data,dataSize);
                gTheGame.c_start = 10;
                gTheGame.love.start = SDL_TRUE;
            }
            break;
	    default:
			/* ̤�ΤΥ��ޥ�ɤ������Ƥ��� */
			fprintf(stderr,"0x%02x is not command!\n",command);
    }
    return endFlag;
}

/************************************************************************************
static
************************************************************************************/



/*****************************************************************
�ؿ�̾	: SetIntData2DataBlock
��ǽ	: int ���Υǡ����������ѥǡ����κǸ�˥��åȤ���
����	: void		*data		: �����ѥǡ���
		  int		intData		: ���åȤ���ǡ���
		  int		*dataSize	: �����ѥǡ����θ��ߤΥ�����
����	: �ʤ�
*****************************************************************/
static void SetIntData2DataBlock(void *data,int intData,int *dataSize)
{
    int tmp2;

    /* �����������å� */
    assert(data!=NULL);
    assert(0<=(*dataSize));

    tmp2 = htonl(intData);

    /* int ���Υǡ����������ѥǡ����κǸ�˥��ԡ����� */
    memcpy(data + (*dataSize),&tmp2,sizeof(int));
    /* �ǡ��������������䤹 */
    (*dataSize) += sizeof(int);
}


/*****************************************************************
�ؿ�̾	: SetfloatData
��ǽ	 : float ���Υǡ�����int�����ѹ�����ؿ�
����	 :  float	  floatdata	  : ���åȤ���ǡ���
����	 : �ʤ�
*****************************************************************/
static int SetfloatData(float floatData)
{
    union IntAndFloat target;
    target.fval=floatData;
    return target.ival;
}




/*****************************************************************
�ؿ�̾	: ChangefloatData
��ǽ	: int���Υǡ�����float ���Υǡ������ѹ�����ؿ�
����	: int 	  intData	  : ���åȤ���ǡ���
����	: �ʤ�
*****************************************************************/
static float ChangefloatData(int intData){
     union newFloat newtarget;
     newtarget.ival=intData;
     return newtarget.fval;
}


/*****************************************************************
�ؿ�̾	: SetCharData2DataBlock
��ǽ	: char ���Υǡ����������ѥǡ����κǸ�˥��åȤ���
����	: void		*data		: �����ѥǡ���
		  int		intData		: ���åȤ���ǡ���
		  int		*dataSize	: �����ѥǡ����θ��ߤΥ�����
����	: �ʤ�
*****************************************************************/
static void SetCharData2DataBlock(void *data,char charData,int *dataSize)
{
    /* �����������å� */
    assert(data!=NULL);
    assert(0<=(*dataSize));

    /* int ���Υǡ����������ѥǡ����κǸ�˥��ԡ����� */
    *(char *)(data + (*dataSize)) = charData;
    /* �ǡ��������������䤹 */
    (*dataSize) += sizeof(char);
}

/******************************************************************************************
��ưŪ��������������ʬ
******************************************************************************************/


/*****************************************************************
�ؿ�̾	: SendXY
��ǽ	: ��������point�������ƥ��point���ͤ������饤����Ȥ���������ؿ�
����	: �ʤ� 
����	: �ʤ�
*****************************************************************/
void SendXY(void){
    unsigned char	data[MAX_DATA];
    int			dataSize;
    int         i,j,k,n,m;
    int         x,y; 
    dataSize=0;
     /* ���ޥ�ɤΥ��å� */
	SetCharData2DataBlock(data,Mov_COMMAND,&dataSize);
    for(i=0;i<MAX_CLIENTS;i++){
        /*�ץ쥤�䡼��x��ɸ��1�Х��Ȥ��Ĥ��Ѵ�*/
        x=SetfloatData(gTheGame.chara[i].point.x);
        /*�ץ쥤�䡼��y��ɸ��1�Х��Ȥ��Ĥ��Ѵ�*/
        y=SetfloatData(gTheGame.chara[i].point.y);
        SetIntData2DataBlock(data,x,&dataSize);
        SetIntData2DataBlock(data,y,&dataSize);
    }
    for(j=0;j<gTheGame.ItemNum;j++){
        /*�ץ쥤�䡼��x��ɸ��1�Х��Ȥ��Ĥ��Ѵ�*/
        x=SetfloatData(gTheGame.item[j].point.x);
        /*�ץ쥤�䡼��y��ɸ��1�Х��Ȥ��Ĥ��Ѵ�*/
        y=SetfloatData(gTheGame.item[j].point.y);
        SetIntData2DataBlock(data,x,&dataSize);
        SetIntData2DataBlock(data,y,&dataSize);
    }
    for(k=0;k<gTheGame.BaseNum;k++){
        /*�ץ쥤�䡼��x��ɸ��1�Х��Ȥ��Ĥ��Ѵ�*/
        x=SetfloatData(gTheGame.base[k].point.x);
        /*�ץ쥤�䡼��y��ɸ��1�Х��Ȥ��Ĥ��Ѵ�*/
        y=SetfloatData(gTheGame.base[k].point.y);
        SetIntData2DataBlock(data,x,&dataSize);
        SetIntData2DataBlock(data,y,&dataSize);
    }
    for(n=0;n<gTheGame.FallNum;n++){
        /*�ץ쥤�䡼��x��ɸ��1�Х��Ȥ��Ĥ��Ѵ�*/
        x=SetfloatData(gTheGame.fall[n].point.x);
        /*�ץ쥤�䡼��y��ɸ��1�Х��Ȥ��Ĥ��Ѵ�*/
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
    /* ���桼���������� */
	SendData(ALL_CLIENTS,data,dataSize);
}


/*****************************************************************
�ؿ�̾	: SendObjectXY
��ǽ	: �㳲ʪ��point���ͤ������饤����Ȥ���������ؿ�
����	: �ʤ� 
����	: �ʤ�
*****************************************************************/
void SendObjectXY(void){
    unsigned char	data[MAX_DATA];
    int			dataSize;
    int         i;
    int         x,y;
    dataSize=0;
    /* ���ޥ�ɤΥ��å� */
	SetCharData2DataBlock(data,Obje_COMMAND,&dataSize);
    for(i=0;i<gTheGame.ObjeNum;i++){
        /*�ץ쥤�䡼��x��ɸ��1�Х��Ȥ��Ĥ��Ѵ�*/
        x=SetfloatData(gTheGame.object[i].point.x);
        /*�ץ쥤�䡼��y��ɸ��1�Х��Ȥ��Ĥ��Ѵ�*/
        y=SetfloatData(gTheGame.object[i].point.y);
        //fprintf(stderr,"->Player[%d]=(%d,%d)",i,x,y);
        SetIntData2DataBlock(data,x,&dataSize);
        SetIntData2DataBlock(data,y,&dataSize);
    }
    /* ���桼���������� */
	SendData(ALL_CLIENTS,data,dataSize);

}

/*****************************************************************
�ؿ�̾	: SendHP
��ǽ	: ������顢������HP���ͤ������饤����Ȥ���������ؿ�
����	: �ʤ� 
����	: �ʤ�
*****************************************************************/
void SendHP(void)
{
    unsigned char	data[MAX_DATA];
    int			dataSize;
    int i,j;

    dataSize = 0;
    /* ���ޥ��C�Υ��å� */
    SetCharData2DataBlock(data,HP_COMMAND,&dataSize);
    for(i=0;i<MAX_CLIENTS;i++){
        /* ������HP�Υ��å� */
        SetIntData2DataBlock(data,gTheGame.chara[i].hp,&dataSize);
        //fprintf(stderr,"chara[%d].HP=%d\n",i,gTheGame.chara[i].hp);
    }
    for(j=0;j<gTheGame.BaseNum;j++){
        /* ������HP�Υ��å� */
        SetIntData2DataBlock(data,gTheGame.base[j].hp_A,&dataSize);
        SetIntData2DataBlock(data,gTheGame.base[j].hp_B,&dataSize);
        //fprintf(stderr,"base[%d],HP(A)=%d,HP(B)=%d\n",j,gTheGame.base[j].hp_A,gTheGame.base[j].hp_B);
    }
     /* ���桼���������� */
	SendData(ALL_CLIENTS,data,dataSize);
}


/*****************************************************************
�ؿ�̾	: SendStts
��ǽ	: �������ξ��֤���������ؿ�
����	: �ʤ� 
����	: �ʤ�
*****************************************************************/
void SendStts(void)
{
    unsigned char	data[MAX_DATA];
    int			dataSize;
    int i,j,b;
    int stts;

    dataSize = 0;

     /* ���ޥ��C�Υ��å� */
    SetCharData2DataBlock(data,Stts_COMMAND,&dataSize);

    //����饯�����ξ���
    for(i=0;i<MAX_CLIENTS;i++){
        //��ư���Ƥ���Ȥ��ξ���
        if(gTheGame.chara[i].input.up==SDL_TRUE || gTheGame.chara[i].input.down==SDL_TRUE || 
            gTheGame.chara[i].input.left==SDL_TRUE || gTheGame.chara[i].input.right==SDL_TRUE){
                SetIntData2DataBlock(data,gTheGame.chara[i].Dir_Flag+8,&dataSize);//8����(2019.1.5)
        }
        //�ߤޤäƤ���Ȥ��ξ���
        else{
            SetIntData2DataBlock(data,gTheGame.chara[i].Dir_Flag,&dataSize);
        }
        //�����Υ��ơ�����������
        stts=gTheGame.chara[i].stts;
        SetIntData2DataBlock(data,stts,&dataSize);
        //�ں�ο�
        SetIntData2DataBlock(data,gTheGame.chara[i].Mokuzai,&dataSize);
        //������Ƥ��륢���ƥ�
        SetIntData2DataBlock(data,gTheGame.chara[i].i_num,&dataSize);
    }
    //�����ƥ�ξ���
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
    //��̩���Ϥξ���
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
        //�ں�Υ��ơ�����������
        stts=gTheGame.mokuzai[m].stts;
        SetIntData2DataBlock(data,stts,&dataSize);
    }
    SetIntData2DataBlock(data,gTheGame.Group1_point,&dataSize);
    SetIntData2DataBlock(data,gTheGame.Group2_point,&dataSize);
    /* ���桼���������� */
	SendData(ALL_CLIENTS,data,dataSize);
    
}


/*****************************************************************
�ؿ�̾	: SendSttsO
��ǽ	: �㳲ʪ�ξ��֤���������ؿ�
����	: �ʤ� 
����	: �ʤ�
*****************************************************************/
void SendSttO(void)
{
    unsigned char	data[MAX_DATA];
    int			dataSize;
    int j;
    int stts;
     /* �����������å� */
    //assert(0<=pos && pos<MAX_CLIENTS);
    dataSize = 0;

     /* ���ޥ��I�Υ��å� */
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
    /* ���桼���������� */
	SendData(ALL_CLIENTS,data,dataSize);
    
}

//(2018/12/20)
/*****************************************************************
�ؿ�̾	: SendTime
��ǽ	: ���֤�����������������ؿ�
����	: �ʤ� 
����	: �ʤ�
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
     /* ���ޥ��M�Υ��å� */
    SetCharData2DataBlock(data,Time_COMMAND,&dataSize);
    /*�Ĥ���֤Υ��å�*/
    SetIntData2DataBlock(data,gTheGame.Limit,&dataSize);
    /* ���桼���������� */
	SendData(ALL_CLIENTS,data,dataSize);
    
}



/******************************************************************************************
�������������Ȥ���������������ʬ
******************************************************************************************/


/*****************************************************************
�ؿ�̾	: Send_BCol_Command
��ǽ	: ���饤����Ȥ˥����ȵ�����������Ƚ������褵���뤿��ˡ�
		�����С��˥ǡ���������
����	: int		chara	    : ������ֹ�
����2 : int       base      ��: �������ֹ�
����3 : int       Flag      ��: ���ͤ��Ƥ��뤫���Ƥ��ʤ�����Ƚ�Ǥ���ե饰
����4 : int       Build       : ������Ω�Ƥ뤫���Ƥʤ�����Ƚ�Ǥ���ե饰
����	: �ʤ�
*****************************************************************/
void Send_BCol_Command(int chara,int base,int Flag,int Build)
{
    unsigned char	data[MAX_DATA];
    int			dataSize;

    dataSize = 0;
    /* ���ޥ��B�Υ��å� */
    SetCharData2DataBlock(data,BCol_COMMAND,&dataSize);
    /* ������ֹ�Υ��å� */
    SetIntData2DataBlock(data,chara,&dataSize);
    /* �����Υ��å� */
    SetIntData2DataBlock(data,base,&dataSize);
    /* �ե饰�Υ��å� */
    SetIntData2DataBlock(data,Flag,&dataSize);
    /* build�Υ��å� */
    SetIntData2DataBlock(data,Build,&dataSize);
    /* ���桼���������� */
	SendData(ALL_CLIENTS,data,dataSize);
}




/*****************************************************************
�ؿ�̾	: Send_Rizult_Command
��ǽ	: ����Ƚ��η�̤򥯥饤����Ȥ���������
����	: �ʤ�
����	: �ʤ�
*****************************************************************/
void Send_Rizult_Command(void)
{
    unsigned char	data[MAX_DATA];
    int			dataSize;
    int         i;
    int         Win=0;
    int         Lose=0;

    //(�ɲ���ʬ����������Ƚ���Ƚ��������ɲ�     2018/12/20)////
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
    ////�����ޤ�/////////////////////////////////
    dataSize = 0;
    for(i=0;i<MAX_CLIENTS;i++){
        dataSize = 0;
        if(gTheGame.chara[i].group==Win){
            /* ���ޥ��W�Υ��å� */
            SetCharData2DataBlock(data,Win_COMMAND,&dataSize);
            /* ���������桼�������� */
	        SendData(i,data,dataSize);
        }
        else if(gTheGame.chara[i].group==Lose){
            /* ���ޥ��W�Υ��å� */
            SetCharData2DataBlock(data,Los_COMMAND,&dataSize);
            /* ���̤����桼�������� */
	        SendData(i,data,dataSize);
        }
        else{
            /* ���ޥ��W�Υ��å� */
            SetCharData2DataBlock(data,Dra_COMMAND,&dataSize);
            /* ����ʬ�������桼�������� */
	        SendData(i,data,dataSize);
        }
    }
}


/*****************************************************************
�ؿ�̾	:Recv_Mask
��ǽ	: ����饯�����������ƥࡢ�����Υޥ������������
����	: int		pos	    : ���饤������ֹ�
����	: �ʤ�
*****************************************************************/
void Recv_Mask(int pos){
    unsigned char	data[MAX_DATA];
    int			dataSize;

     /* �����������å� */
    assert(0<=pos && pos<MAX_CLIENTS);  

    //�����Υޥ���
    for(int i=0;i<MAX_CLIENTS;i++){
        RecvIntData(pos,&gTheGame.chara[i].mask.h);
        RecvIntData(pos,&gTheGame.chara[i].mask.w);
        RecvIntData(pos,&gTheGame.chara[i].mask.x);
        RecvIntData(pos,&gTheGame.chara[i].mask.y);
        //fprintf(stderr,"chara[%d]:mask.h=%d,mask.w=%d.\n",i,gTheGame.chara[i].mask.h,gTheGame.chara[i].mask.w);
        //fprintf(stderr,"chara[%d]:mask.x=%d,mask.y=%d.\n",i,gTheGame.chara[i].mask.x,gTheGame.chara[i].mask.y);
    }
     //�����ƥ�Υޥ���
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
     //�����Υޥ���
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
    //��Ȥ���Υޥ���
    for(int f=0;f<gTheGame.FallNum;f++){
        //fprintf(stderr,"->Player[%d]=(%d,%d)",i,x,y);
       RecvIntData(pos,&gTheGame.fall[f].mask.h);
       RecvIntData(pos,&gTheGame.fall[f].mask.w);
       RecvIntData(pos,&gTheGame.fall[f].mask.x);
       RecvIntData(pos,&gTheGame.fall[f].mask.y);
    }  
     
}




/*****************************************************************
�ؿ�̾	: SendGhost
��ǽ	: CS_Ghost��CS_Enable�ξ����Ѳ�������
����	: �ʤ� 
����	: �ʤ�
*****************************************************************/
void SendGhost(int pos,int sttsG)
{
    unsigned char	data[MAX_DATA];
    int			dataSize;
    
     /* �����������å� */
    assert(0<=pos && pos<MAX_CLIENTS);
    dataSize = 0;
    /* ���ޥ��C�Υ��å� */
    SetCharData2DataBlock(data,Ghost_COMMAND,&dataSize);
    for(int i=0;i<MAX_CLIENTS;i++){
        if(sttsG>0){
            SetIntData2DataBlock(data,pos,&dataSize);
            SetIntData2DataBlock(data,sttsG,&dataSize);
        }
    }
    /* ���桼���������� */
	SendData(ALL_CLIENTS,data,dataSize);
    
}


/*****************************************************************
�ؿ�̾	: Send_Fall_Command
��ǽ	: ��Ȥ����������뤿��˥��饤����Ȥ˥ǡ���������
����	: �ʤ� 
����	: �ʤ�
*****************************************************************/
void Send_Fall_Command(int chara,int fall)
{
    unsigned char	data[MAX_DATA];
    int			dataSize;


    dataSize = 0;
    /* ���ޥ��L�Υ��å� */
    SetCharData2DataBlock(data,Fall_COMMAND,&dataSize);
    /* ������ֹ�Υ��å� */
    SetIntData2DataBlock(data,chara,&dataSize);
    /* �����ƥ�Υ��å� */
    SetIntData2DataBlock(data,fall,&dataSize);
    /* ���桼���������� */
	SendData(ALL_CLIENTS,data,dataSize);
}


/*****************************************************************
�ؿ�̾	: Send_Catch_Command
��ǽ	: ���饤����Ȥ���Ȥ����������Ƚ������褵���뤿��ˡ�
		���饤����Ȥ˥ǡ���������
����	: int		chara	    : ������ֹ�
����2 : int       fall      ��: ��Ȥ�����ֹ�
����3 : int       item      ��: �����ƥ���ֹ�
����	: �ʤ�
*****************************************************************/
void Send_Catch_Command(int chara,int fall)
{
    unsigned char	data[MAX_DATA];
    int			dataSize;


    dataSize = 0;
    /* ���ޥ��L�Υ��å� */
    SetCharData2DataBlock(data,Catch_COMMAND,&dataSize);
    /* ������ֹ�Υ��å� */
    SetIntData2DataBlock(data,chara,&dataSize);
    /* ��Ȥ���Υ��å� */
    SetIntData2DataBlock(data,fall,&dataSize);
    /* ���桼���������� */
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
            /* ���ޥ��M�Υ��å� */
            SetCharData2DataBlock(data,Msg_COMMAND,&dataSize);
            /*���⤵��Ƥ��뤫����Ƥ��ʤ���*/
             SetIntData2DataBlock(data,change,&dataSize);
            /* ���⤵��Ƥ��륰�롼�פ����� */
	        SendData(i,data,dataSize);
        }
    }
}