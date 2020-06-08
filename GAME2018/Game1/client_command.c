/*****************************************************************
�ե�����̾	: client_command.c
��ǽ		: ���饤����ȤΥ��ޥ�ɽ���
*****************************************************************/

#include"common.h"
#include"client_func.h"
#include "system.h"

static void SetIntData2DataBlock(void *data,int intData,int *dataSize);
static void SetCharData2DataBlock(void *data,char charData,int *dataSize);
static int SetfloatData(float floatData);
static float ChangefloatData(int intData);
static void Recv_Mov_Command(void);
//(�ɲôؿ������������Ԥ��ؿ�   2018/11/7)
static void Recv_Att_Command(void);
static void Recv_BCol_Command(void);
static void Recv_HP_Command(void);
static void Recv_Rizult_Command(int rizult);
static void Recv_Stts_Command(void);
static void Recv_Object_Command(void);
static void Recv_Ghost_Command(void);
static void Recv_SttsO_Command(void);
static void Recv_Fall_Command(void);
static void Recv_Catch_Command(void);
static void Recv_Start_Command (void);
static void Recv_Message_Command(void);

int flag_gbec[4];
/*****************************************************************
�ؿ�̾	: ExecuteCommand
��ǽ	: �����С����������Ƥ������ޥ�ɤ򸵤ˡ�
		����������������¹Ԥ���
����	: char	command		: ���ޥ��
����	: �ץ���ཪλ���ޥ�ɤ��������Ƥ������ˤ�0���֤���
		����ʳ���1���֤�
*****************************************************************/
int ExecuteCommand(char command)
{
    int	endFlag = 1;
#ifndef NDEBUG
    printf("#####\n");
    printf("ExecuteCommand()\n");
    printf("command = %c\n",command);
#endif
    switch(command){
        case END_COMMAND:
			endFlag = 0;
			break;
        case Mov_COMMAND:
			Recv_Mov_Command();
			break;
        case HP_COMMAND:
			Recv_HP_Command();
			break;
        case Win_COMMAND:
			Recv_Rizult_Command(0);
			break;
        case Los_COMMAND:
			Recv_Rizult_Command(1);
			break;
        case Dra_COMMAND:
			Recv_Rizult_Command(2);
			break;
        case Stts_COMMAND:
			Recv_Stts_Command();
			break;
        case SttsO_COMMAND:
			Recv_SttsO_Command();
			break;
        case Obje_COMMAND:
			Recv_Object_Command();
			break;
        case BCol_COMMAND:
			Recv_BCol_Command();
			break;
        case Ghost_COMMAND:
			Recv_Ghost_Command();
			break;  
        //(�ɲ���ʬ 2018/12.20)
        case Time_COMMAND:
            RecvIntData(&gTheGame.Limit);
			break;
        case Fall_COMMAND:
			Recv_Fall_Command();//�����Ф������
			break;
        case Catch_COMMAND:
			Recv_Catch_Command();//�����Ф������
			break;
        case Start_COMMAND:
			Recv_Start_Command();//�����Ф������
			break;
        case Msg_COMMAND:
			Recv_Message_Command();//�����Ф������
			break;
    }
    return endFlag;
}

/*******************************************************************************
static
********************************************************************************/



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
    int tmp;

    /* �����������å� */
    assert(data!=NULL);
    assert(0<=(*dataSize));

    tmp = htonl(intData);

    /* int ���Υǡ����������ѥǡ����κǸ�˥��ԡ����� */
    memcpy(data + (*dataSize),&tmp,sizeof(int));
    /* �ǡ��������������䤹 */
    (*dataSize) += sizeof(int);
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

    /* char ���Υǡ����������ѥǡ����κǸ�˥��ԡ����� */
    *(char *)(data + (*dataSize)) = charData;
    /* �ǡ��������������䤹 */
    (*dataSize) += sizeof(char);
}



/*****************************************************************
�ؿ�̾	: SetfloatData
��ǽ	: float ���Υǡ�����int�����ѹ�����ؿ�
����	:   float	  floatdata	  : ���åȤ���ǡ���
����	: �ʤ�
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



/*****************************************************************************
���饤����Ȥ��饵���С��������������
******************************************************************************/


/*****************************************************************
�ؿ�̾	: SendEndCommand
��ǽ	: �ץ����ν�λ���Τ餻�뤿��ˡ�
		  �����С��˥ǡ���������
����	: �ʤ�
����	: �ʤ�
*****************************************************************/
void SendEndCommand(void)
{
    unsigned char	data[MAX_DATA];
    int			dataSize;

#ifndef NDEBUG
    printf("#####\n");
    printf("SendEndCommand()\n");
#endif
    dataSize = 0;
    /* ���ޥ�ɤΥ��å� */
    SetCharData2DataBlock(data,END_COMMAND,&dataSize);

    /* �ǡ��������� */
    SendData(data,dataSize);
}


/*****************************************************************
�ؿ�̾	: Send_MOVE_Command
��ǽ	: ���饤����Ȥ˰�ư�����뤿��ˡ�
		�����С��˥ǡ���������
����	: int		pos	    : ���饤������ֹ�
����2 : float     rad    :�����Ϥ��������ֹ�
����	: �ʤ�
*****************************************************************/
void Send_MOVE_Command(int pos,float rad)
{
    unsigned char	data[MAX_DATA];
    int			dataSize;
    int rad_int;

    /* �����������å� */
    assert(0<=pos && pos<MAX_CLIENTS);

#ifndef NDEBUG
    printf("#####\n");
    printf("Send_CH_Command()\n");
    printf("Send Circle Command to %d\n",pos);
#endif
   // fprintf(stderr,"Putkey=%d\n",Putkey);
    dataSize = 0;
    /* ���ޥ��M�Υ��å� */
    SetCharData2DataBlock(data,Mov_COMMAND,&dataSize);

     /*int�����Ѵ�*/
    rad_int=SetfloatData(rad);

    /* ���饤������ֹ�Υ��å� */
    SetIntData2DataBlock(data,pos,&dataSize);
    /* �饸����Υ��å� */
    SetIntData2DataBlock(data,rad_int,&dataSize);
    /* �ǡ��������� */
    SendData(data,dataSize);
    //fprintf(stderr,"Mov_COMMAND\n");
}


/*****************************************************************
�ؿ�̾	: Send_ATT_Command
��ǽ	: ���饤����Ȥ˹��⤵���뤿��ˡ�
		�����С��˥ǡ���������
����	: int		pos	    : ���饤������ֹ�
����2 : int       Pattern  :������Υѥ�����
����	: �ʤ�
*****************************************************************/
void Send_ATT_Command(int pos,int Pattern)
{
    unsigned char	data[MAX_DATA];
    int			dataSize;

    /* �����������å� */
    assert(0<=pos && pos<MAX_CLIENTS);

#ifndef NDEBUG
    printf("#####\n");
    printf("Send_CH_Command()\n");
    printf("Send Circle Command to %d\n",pos);
#endif
    //fprintf(stderr,"->ATT_Pattern=%d\n",Pattern);
    dataSize = 0;
    /* ���ޥ��A�Υ��å� */
    SetCharData2DataBlock(data,Att_COMMAND,&dataSize);
    /* ���饤������ֹ�Υ��å� */
    SetIntData2DataBlock(data,pos,&dataSize);
    /* ���Ϥ��������Υ��å� */
    SetIntData2DataBlock(data,Pattern,&dataSize);
    /* �ǡ��������� */
    SendData(data,dataSize);
    //fprintf(stderr,"Send_COMMAND\n");
}

/*****************************************************************
�ؿ�̾	: Send_Chuze_Command
��ǽ	: ���饤����Ȥ˥����ƥ�����򤵤��뤿��ˡ�
		�����С��˥ǡ���������
����	: int		pos	    : ���饤������ֹ�
����	: �ʤ�
*****************************************************************/
void Send_Chuze_Command(int pos,int dir)
{
    unsigned char	data[MAX_DATA];
    int			dataSize;
    int number;

    /* �����������å� */
    assert(0<=pos && pos<MAX_CLIENTS);

#ifndef NDEBUG
    printf("#####\n");
    printf("Send_CH_Command()\n");
    printf("Send Circle Command to %d\n",pos);
#endif
    //fprintf(stderr,"->ATT_Pattern=%d\n",Pattern);
    dataSize = 0;
    if(dir==1){
        gTheGame.chuze++;
    }
    else if(dir==2){
        gTheGame.chuze--;
    }
    if(gTheGame.chuze<0){
        gTheGame.chuze=3;
    }
    else if(gTheGame.chuze>3){
        gTheGame.chuze=0;
    }
    gTheGame.message[8].src.y=200*gTheGame.chuze;
  
   
    /* ���ޥ��A�Υ��å� */
    SetCharData2DataBlock(data,Chu_COMMAND,&dataSize);
    /* ���饤������ֹ�Υ��å� */
    SetIntData2DataBlock(data,pos,&dataSize);
    SetIntData2DataBlock(data,gTheGame.chuze,&dataSize);
    /* �ǡ��������� */
    SendData(data,dataSize);
    //fprintf(stderr,"Send_Chuze_COMMAND\n");

}

/*****************************************************************
�ؿ�̾	: SendMask
��ǽ	: ����饯�����������ƥࡢ�����Υޥ����򥵡��С�������
����	: �ʤ�
����	: �ʤ�
*****************************************************************/
void SendMask(void)
{
    unsigned char	data[MAX_DATA];
    int			dataSize;

    /* �����������å� */
    //assert(0<=pos && pos<MAX_CLIENTS);


    dataSize = 0;
    /* ���ޥ��K�Υ��å� */
    SetCharData2DataBlock(data,Mask_COMMAND,&dataSize);

    //�����Υޥ���
    for(int i=0;i<MAX_CLIENTS;i++){
        SetIntData2DataBlock(data,gTheGame.chara[i].mask.h,&dataSize);
        SetIntData2DataBlock(data,gTheGame.chara[i].mask.w,&dataSize);
        SetIntData2DataBlock(data,gTheGame.chara[i].mask.x,&dataSize);
        SetIntData2DataBlock(data,gTheGame.chara[i].mask.y,&dataSize);
        //fprintf(stderr,"chara[%d]:mask.h=%d,mask.w=%d.\n",i,gTheGame.chara[i].mask.h,gTheGame.chara[i].mask.w);
        //fprintf(stderr,"chara[%d]:mask.x=%d,mask.y=%d.\n",i,gTheGame.chara[i].mask.x,gTheGame.chara[i].mask.y);
    }
    //�����ƥ�Υޥ���
    for(int j=0;j<gTheGame.ItemNum;j++){
        SetIntData2DataBlock(data,gTheGame.item[j].mask.h,&dataSize);
        SetIntData2DataBlock(data,gTheGame.item[j].mask.w,&dataSize);
        SetIntData2DataBlock(data,gTheGame.item[j].mask.x,&dataSize);
        SetIntData2DataBlock(data,gTheGame.item[j].mask.y,&dataSize);
        //fprintf(stderr,"item[%d]:mask.h=%d,mask.w=%d.\n",j,gTheGame.item[j].mask.h,gTheGame.item[j].mask.w);
        //fprintf(stderr,"item[%d]:mask.x=%d,mask.y=%d.\n",j,gTheGame.item[j].mask.x,gTheGame.item[j].mask.y);
    }
    //�����Υޥ���
    for(int k=0;k<gTheGame.baseNum;k++){
        SetIntData2DataBlock(data,gTheGame.base[k].mask.h,&dataSize);
        SetIntData2DataBlock(data,gTheGame.base[k].mask.w,&dataSize);
        SetIntData2DataBlock(data,gTheGame.base[k].mask.x,&dataSize);
        SetIntData2DataBlock(data,gTheGame.base[k].mask.y,&dataSize);
        SetIntData2DataBlock(data,gTheGame.base[k].mask2.h,&dataSize);
        SetIntData2DataBlock(data,gTheGame.base[k].mask2.w,&dataSize);
        SetIntData2DataBlock(data,gTheGame.base[k].mask2.x,&dataSize);
        SetIntData2DataBlock(data,gTheGame.base[k].mask2.y,&dataSize);
        SetIntData2DataBlock(data,gTheGame.base[k].mask3.h,&dataSize);
        SetIntData2DataBlock(data,gTheGame.base[k].mask3.w,&dataSize);
        SetIntData2DataBlock(data,gTheGame.base[k].mask3.x,&dataSize);
        SetIntData2DataBlock(data,gTheGame.base[k].mask3.y,&dataSize);
        SetIntData2DataBlock(data,gTheGame.base[k].mask4.h,&dataSize);
        SetIntData2DataBlock(data,gTheGame.base[k].mask4.w,&dataSize);
        SetIntData2DataBlock(data,gTheGame.base[k].mask4.x,&dataSize);
        SetIntData2DataBlock(data,gTheGame.base[k].mask4.y,&dataSize);
        //fprintf(stderr,"base[%d]:mask.h=%d,mask.w=%d.\n",k,gTheGame.base[k].mask.h,gTheGame.base[k].mask.w);
        //fprintf(stderr,"base[%d]:mask.x=%d,mask.y=%d.\n",k,gTheGame.base[k].mask.x,gTheGame.base[k].mask.y);
    }
     //��Ȥ���Υޥ���
    for(int f=0;f<gTheGame.FallNum;f++){
        SetIntData2DataBlock(data,gTheGame.fall[f].mask.h,&dataSize);
        SetIntData2DataBlock(data,gTheGame.fall[f].mask.w,&dataSize);
        SetIntData2DataBlock(data,gTheGame.fall[f].mask.x,&dataSize);
        SetIntData2DataBlock(data,gTheGame.fall[f].mask.y,&dataSize);
    }
    /* �ǡ��������� */
    SendData(data,dataSize);
    fprintf(stderr,"Send_Mask\n");

}


/*****************************************************************************
�����С����饯�饤����Ȥؼ����������
******************************************************************************/




/*****************************************************************
�ؿ�̾	: Recv_Mov_Command
��ǽ	: ��ư��ɽ�����뤿��Υǡ������������ɽ�����롣
����	: �ʤ�
����	: �ʤ�
*****************************************************************/
static void Recv_Mov_Command(void){
    int i,j,k,n,m;
    int n1,n2;//x,y�ι��
    int x[MAX_CLIENTS],y[MAX_CLIENTS];
    int Ix[gTheGame.ItemNum],Iy[gTheGame.ItemNum];
    int Bx[gTheGame.baseNum],By[gTheGame.baseNum];
    int Fx[gTheGame.FallNum],Fy[gTheGame.FallNum];
    int Mx[gTheGame.MokuNum],My[gTheGame.MokuNum];

    //x��y���ͤ򤽤줾��������int����float��
    for(i=0;i<MAX_CLIENTS;i++){
        RecvIntData(&x[i]);//x��ɸ
        RecvIntData(&y[i] );//y��ɸ
    }
    for(j=0;j<gTheGame.ItemNum;j++){
        RecvIntData(&Ix[j]);//x��ɸ
        RecvIntData(&Iy[j] );//y��ɸ
    }
    for(k=0;k<gTheGame.baseNum;k++){
        RecvIntData(&Bx[k]);//x��ɸ
        RecvIntData(&By[k] );//y��ɸ
    }
    for(n=0;n<gTheGame.FallNum;n++){
        RecvIntData(&Fx[n]);//x��ɸ
        RecvIntData(&Fy[n] );//y��ɸ
    }
    for(m=0;m<gTheGame.MokuNum;m++){
        RecvIntData(&Mx[m]);//x��ɸ
        RecvIntData(&My[m] );//y��ɸ
    }
    //x��y���ͤ򥭥���point������
    for(i=0;i<MAX_CLIENTS;i++){
        gTheGame.chara[i].point.x=ChangefloatData(x[i]);
        gTheGame.chara[i].point.y=ChangefloatData(y[i]);
    }
    for(j=0;j<gTheGame.ItemNum;j++){
        gTheGame.item[j].point.x=ChangefloatData(Ix[j]);
        gTheGame.item[j].point.y=ChangefloatData(Iy[j]);
        //fprintf(stderr,"Item[%d] is  (%f,%f).\n",i,gTheGame.item[j].point.x,gTheGame.item[j].point.y);
    }
    for(k=0;k<gTheGame.baseNum;k++){
        gTheGame.base[k].point.x = ChangefloatData(Bx[k]);
        gTheGame.base[k].point.y = ChangefloatData(By[k]);
        //fprintf(stderr,"Base[%d] is  (%f,%f).\n",i,gTheGame.base[k].point.x,gTheGame.base[k].point.y);
    }
    for(n=0;n<gTheGame.FallNum;n++){
        gTheGame.fall[n].point.x = ChangefloatData(Fx[n]);
        gTheGame.fall[n].point.y = ChangefloatData(Fy[n]);
        //fprintf(stderr,"Base[%d] is  (%f,%f).\n",i,gTheGame.base[k].point.x,gTheGame.base[k].point.y);
    }
    for(m=0;m<gTheGame.MokuNum;m++){
        gTheGame.mokuzai[m].point.x = ChangefloatData(Mx[m]);
        gTheGame.mokuzai[m].point.y = ChangefloatData(My[m]);
        //fprintf(stderr,"mokuzai[%d] is  (%f,%f).\n",m,gTheGame.mokuzai[m].point.x,gTheGame.mokuzai[m].point.y);
    }
    //fprintf(stderr,"OK_Recv_XY\n");
  
}


/*****************************************************************
�ؿ�̾	: Recv_Object_Command
��ǽ	: �㳲ʪ�κ�ɸ���������ؿ�
����	: �ʤ�
����	: �ʤ�
*****************************************************************/
static void Recv_Object_Command(void){
    int i;
    int x[gTheGame.ObjeNum],y[gTheGame.ObjeNum];
    for(i=0;i<gTheGame.ObjeNum;i++){
        RecvIntData(&x[i]);//x��ɸ
        RecvIntData(&y[i] );//y��ɸ
    }
    //x��y���ͤ򥭥���point������
    for(i=0;i<gTheGame.ObjeNum;i++){
        gTheGame.object[i].point.x=ChangefloatData(x[i]);
        gTheGame.object[i].point.y=ChangefloatData(y[i]);
    }
    //fprintf(stderr,"OK_Recv_Object\n");
}



/*****************************************************************
�ؿ�̾	: Recv_Stts_Command
��ǽ	: ����饯�����ξ��֤��������ؿ�
����	: �ʤ�
����	: �ʤ�
*****************************************************************/
static void Recv_Stts_Command(void){
    int i,j,b;
    int stts_num;
    int stts;
    int Mokuzai;
    for(i=0;i<MAX_CLIENTS;i++){
        /* �����θ����Υ��å� */
        RecvIntData(&stts_num);
        RecvIntData(&stts);
        RecvIntData(&Mokuzai);  //�ں�μ���(�ɲá�2018/12.19)
        RecvIntData(&gTheGame.chara[i].i_num);
        gTheGame.chara[i].Mokuzai = Mokuzai;
        gTheGame.chara[i].dir_flag=stts_num;
        if(gTheGame.chara[i].dir_flag>=8){
            gTheGame.chara[i].dir_flag-=8;
        }
        if( Mokuzai >= 50 ){
            Mokuzai = 50;
        }
        if(gTheGame.chara[i].stts == CS_Ghost){//8����(2019.1.5)
            if(stts_num>=8){
                stts_num -=8;
            }
            if(stts_num == 5){
                gTheGame.chara[i].src3.y = 144*(stts_num+1);
            }else if(stts_num == 6){
                gTheGame.chara[i].src3.y= 144*(stts_num-1);
            }
            else{
              gTheGame.chara[i].src3.y =144*stts_num;  
            }
        }
        else {
            flag_gbec[i] == 0;
            gTheGame.chara[i].src.y =144*stts_num;
            if(gTheGame.chara[i].f_die==0){
                gTheGame.chara[i].src2.y=144*gTheGame.chara[i].dir_flag;
                gTheGame.chara[i].f_die=1;
            }
        }
        //����饯�����ξ���
        if(stts!=3){
            gTheGame.chara[i].stts=stts;
        }
    }
    //�����ƥ�ξ���
    for(j=0;j<gTheGame.ItemNum;j++){
        RecvIntData(&stts_num);
        gTheGame.item[j].stts=stts_num;
        //fprintf(stderr,"item[%d].type%d.stts=%d\n",j,gTheGame.item[j].type,gTheGame.item[j].stts);
    }
    //��̩���Ϥξ���
    for(b=0;b<gTheGame.baseNum;b++){
        RecvIntData(&stts_num);
        gTheGame.base[b].stts=stts_num;
        if(stts_num==2){
            gTheGame.base[b].src.y=0;
        }
        else if(stts_num==3){
            gTheGame.base[b].src.y=320;
        }
        else if(stts_num==4){
            gTheGame.base[b].src.y=640;
        }
        else if(stts_num==5){
            gTheGame.base[b].src2.y=0;
        }
        else if(stts_num==6){
            gTheGame.base[b].src2.y=640;
        }
    }
    for(int m=0;m<gTheGame.MokuNum;m++){
        //�ں�Υ��ơ�����������
        RecvIntData(&stts_num);
        gTheGame.mokuzai[m].stts=stts_num;
        //fprintf(stderr,"mokuzai[%d].stts=%d\n",m,gTheGame.mokuzai[m].stts);
    }
    //���롼�פΥݥ���Ȥμ���
    RecvIntData(&gTheGame.Group1_point);
    RecvIntData(&gTheGame.Group2_point);
    //fprintf(stderr,"Group1_point=%d\n,Group2_point=%d\n",gTheGame.Group1_point,gTheGame.Group2_point);
}


/*****************************************************************
�ؿ�̾	: Recv_SttsO_Command
��ǽ	: �㳲ʪ��������Ƚ���ɽ�����뤿��Υǡ������������ɽ�����롣
����	: �ʤ�
����	: �ʤ�
*****************************************************************/
static void Recv_SttsO_Command(void){
    int j;
    int stts_num;
     //fprintf(stderr,"O\n");
     for(j=0;j<gTheGame.ObjeNum;j++){
        RecvIntData(&stts_num);
        gTheGame.object[j].stts=stts_num;
        //fprintf(stderr,"Object[%d].stts=%d\n",j,gTheGame.object[j].stts);
         if(gTheGame.object[j].stts==OS_Enable){
            gTheGame.object[j].src.y=0;
        }
        else if(gTheGame.object[j].stts==OS_Shake){
            gTheGame.object[j].src.y=308;
            //fprintf(stderr,"Object[%d].src.y=%d\n",j,gTheGame.object[j].src.y);
        }

    }
   
}


/*****************************************************************
�ؿ�̾	:Recv_BCol_Command
��ǽ	: �����ȵ�����������Ƚ���ɽ�����뤿��Υǡ������������ɽ�����롣
����	: �ʤ�
����	: �ʤ�
*****************************************************************/
static void Recv_BCol_Command(void){
    int chara;
    int base;
    int Flag;
    int Build;
    RecvIntData(&chara);
    RecvIntData(&base);
    RecvIntData(&gTheGame.chara[chara].SteyFlag);
    RecvIntData(&Build);

     gTheGame.chara[gTheGame.c_ID].basecollision = base;
    //1�ĤΥ��饤����Ȥˤ���ɽ��
    if(chara==gTheGame.c_ID){
        if(Build == 0){
            if(gTheGame.chara[chara].SteyFlag>0){
                gTheGame.message[MS_kazu].stts=MS_Enable;
            }
            else if(gTheGame.chara[chara].SteyFlag==0){
                gTheGame.message[MS_kazu].stts=MS_Disable;
            }
            gTheGame.message[MS_kazu].rect.y=gTheGame.base[base].rect.y-100;
            gTheGame.message[MS_kazu].rect.x=gTheGame.base[base].rect.x-10;
            gTheGame.chara[chara].HPFlag = 0;
        }
        else if( Build == 1 ){
            if( gTheGame.chara[chara].SteyFlag > 0 ){
                gTheGame.chara[chara].HPFlag = 1;
            }
            else if( gTheGame.chara[chara].SteyFlag == 0 ){
                gTheGame.chara[chara].HPFlag = 0;
            }
        }
       
    }
}


/*****************************************************************
�ؿ�̾	: Recv_HP_Command
��ǽ	: ������顢������HP���ͤΥǡ������������ɽ�����롣
����	: �ʤ�
����	: �ʤ�
*****************************************************************/
static void Recv_HP_Command(void){
    int i,j;
    for(i=0;i<MAX_CLIENTS;i++){
        /* ������HP�Υ��å� */
        RecvIntData(&gTheGame.chara[i].hp);
    }
    for(j=0;j<gTheGame.baseNum;j++){
        /* ������HP�Υ��å� */
        RecvIntData(&gTheGame.base[j].hp_A);
        RecvIntData(&gTheGame.base[j].hp_B);
    }
     
}


/*****************************************************************
�ؿ�̾	: Recv_Rizult_Command
��ǽ	: ���Է�̤��������ɽ������ؿ������
����	: int   rizult
����	: �ʤ�
*****************************************************************/
static void Recv_Rizult_Command(int rizult){
    if(gTheGame.stts==GS_Playing){
        gTheGame.stts=GS_End;
        //fprintf(stderr,"GS=%d\n",gTheGame.stts);
    }
    gTheGame.rizultFLag=rizult;
    /*switch(rizult){
        case 0:
            gTheGame.message[0].stts=MS_Enable;
            break;
        case 1:
            gTheGame.message[1].stts=MS_Enable;
            break;
        case 2:
            gTheGame.message[2].stts=MS_Enable;
            break;
    }*/
}

/*****************************************************************
�ؿ�̾	: Recv_Ghost_Command
��ǽ	: �������Ȥ��������ɽ������ؿ������
����	: �ʤ�
����	: �ʤ�
*****************************************************************/
static void Recv_Ghost_Command(void){//2018.12.9
    int pos;
    int chara_stts;
    for(int i=0;i<MAX_CLIENTS;i++){
        
        /* �����ξ��֤Υ��å� */
        RecvIntData(&pos);
        RecvIntData(&chara_stts);
        
        if(i == 0){
            if(chara_stts == 3){
                if(gTheGame.c_ID == pos){
                    gTheGame.chara[pos].stts = CS_Ghost;
                }else{
                    gTheGame.chara[pos].stts = CS_Die;
                    if(gTheGame.chara[pos].f_die==0){
                        gTheGame.chara[pos].src2.y=144*gTheGame.chara[pos].dir_flag;
                        gTheGame.chara[pos].f_die=1;
                    }
                }
            }else if(chara_stts == 1){
                gTheGame.chara[pos].rect = gTheGame.chara[pos].rect3;//(2018.12.29) 
            }else  if(chara_stts == 2){
                gTheGame.fall[gTheGame.chara[pos].fall_num].stts = FS_Disable;
                gTheGame.chara[pos].fall_num = 30;
            }
        }
    }
}


/*****************************************************************
�ؿ�̾	: Recv_Fall_Command
��ǽ	:����Ȥ�����������ɽ������ؿ������
����	: �ʤ�
����	: �ʤ�
*****************************************************************/
static void Recv_Fall_Command(void){//2018.12.17
    int pos;
    int fall;
    for(int i=0;i<MAX_CLIENTS;i++){
        RecvIntData(&pos);
        RecvIntData(&fall);
        if(gTheGame.c_ID == pos){
        /* �����ξ��֤Υ��å� */
            gTheGame.fall[fall].c_num = pos;
            gTheGame.fall[fall].stts = FS_Enable;
        }    
    }
}

//2018.12.23
/*****************************************************************
�ؿ�̾	: Recv_Catch_Command
��ǽ	: ��Ȥ����������Ƚ���ɽ�����뤿��Υǡ������������ɽ�����롣
����	: �ʤ�
����	: �ʤ�
*****************************************************************/
static void Recv_Catch_Command(void){
    int chara;
    int fall;
    for(int i=0;i<MAX_CLIENTS;i++){
    RecvIntData(&chara);
    RecvIntData(&fall);
        if(i == 0){
            gTheGame.fall[fall].stts = FS_Catch; 
            gTheGame.chara[chara].stts = CS_Fallout; 
            gTheGame.chara[chara].fall_num = fall;
            gTheGame.chara[chara].src4.x = 0;
            
            int bec = gTheGame.chara[chara].src.y;
                if(bec == 0 || bec == 144*4){//(2019.1.7)
                    gTheGame.chara[chara].src4.y = 144*0;//��
                }else if(bec == 144*1 || bec == 144*5){
                    gTheGame.chara[chara].src4.y = 144*1;//���
                }else if(bec == 144*2 || bec == 144*6){
                    gTheGame.chara[chara].src4.y = 144*2;//��
                }else if(bec == 144*3 || bec == 144*7){
                    gTheGame.chara[chara].src4.y = 144*3;//��
                }else if(bec == 144*8 || bec == 144*12){
                    gTheGame.chara[chara].src4.y = 144*4;//����
                }else if(bec == 144*9 || bec == 144*13){
                    gTheGame.chara[chara].src4.y = 144*5;//����
                }else if(bec == 144*10 || bec == 144*14){
                    gTheGame.chara[chara].src4.y = 144*6;//����
                }else if(bec == 144*11 || bec == 144*15){
                    gTheGame.chara[chara].src4.y = 144*7;//����
                }
        }
    }
}


//2019.1.7
/*****************************************************************
�ؿ�̾	: Send_Start
��ǽ	: �������Ȥ���ˡ�
		�����С��˥ǡ���������
����	: int		pos	    : ���饤������ֹ�
����2 : int       Pattern  :������Υѥ�����
����	: �ʤ�
*****************************************************************/
void Send_Start_Command(int pos)
{
    unsigned char	data[MAX_DATA];
    int			dataSize;

    /* �����������å� */
    assert(0<=pos && pos<MAX_CLIENTS);

#ifndef NDEBUG
    printf("#####\n");
    printf("Send_CH_Command()\n");
    printf("Send Circle Command to %d\n",pos);
#endif
    //fprintf(stderr,"->ATT_Pattern=%d\n",Pattern);
    dataSize = 0;
    /* ���ޥ��A�Υ��å� */
    SetCharData2DataBlock(data,Start_COMMAND,&dataSize);
    /* ���饤������ֹ�Υ��å� */
    SetIntData2DataBlock(data,pos,&dataSize);
    /* �ǡ��������� */
    SendData(data,dataSize);
    //fprintf(stderr,"RAITA\n");
}


//2019.1.11
/*****************************************************************
�ؿ�̾	: Recv_Start_Command
��ǽ	: �����Ф��饹�����Ȥ����ޤ�������
����	: �ʤ�
����	: �ʤ�
*****************************************************************/
static void Recv_Start_Command(void){
    gTheGame.input.start == SDL_TRUE;
    gTheGame.input.ready = SDL_TRUE;
    gTheGame.input.wait = SDL_TRUE;   
    
    //fprintf(stderr,"RAITA2\n");
}

static void Recv_Message_Command(void){
    int change;
    RecvIntData(&change);
    if(change==0){
        gTheGame.message[11].stts=MS_Enable;
        gTheGame.message[11].rect.x=WD_Width*MAP_ChipSize-200;
        gTheGame.message[11].rect.y=50;
    }
    else if(change==1){
        gTheGame.message[11].stts=MS_Disable;
    }
   
}