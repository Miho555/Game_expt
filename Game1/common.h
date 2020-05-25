/*****************************************************************
�ե�����̾	: common.h
��ǽ		: �����С��ȥ��饤����Ȥǻ��Ѥ�������������Ԥ�
*****************************************************************/

#ifndef _COMMON_H_
#define _COMMON_H_

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>
#include<assert.h>
#include<math.h>

#define PORT			(u_short)8888	/* �ݡ����ֹ� */

#define MAX_CLIENTS		4				/* ���饤����ȿ��κ����� */
#define MAX_NAME_SIZE	10 				/* �桼����̾�κ�����*/

#define MAX_DATA		1000				/* ����������ǡ����κ����� */


#define Win_COMMAND	'A'				/* �������ޥ�� */
#define Los_COMMAND 'B'				/* ���̥��ޥ�� */
#define Dra_COMMAND 'C'				/* ����ʬ�����ޥ�� */
#define Mov_COMMAND	'D'				/* ��ư���ޥ�� */
#define Ite_COMMAND	'E'				/* �����ƥॳ�ޥ�� */
#define Att_COMMAND	'F'				/* ���⥳�ޥ�� */
#define END_COMMAND 'G'             /* ��λ���ޥ��*/
#define BCol_COMMAND 'H'            /* ����������Ƚ�ꥳ�ޥ��*/
#define Msg_COMMAND 'I'            /*��å��������ޥ��*/
#define HP_COMMAND 'J'              /* HP���ޥ��*/
#define Stts_COMMAND 'K'            /* ���֤Υ��ޥ��*/
#define SttsO_COMMAND 'L'            /* ���֤Υ��ޥ��*/
#define Mask_COMMAND 'M'            /* Mask�Υ��ޥ��*/
#define Obje_COMMAND 'N'            /* �㳲ʪ�Υ��ޥ��*/
#define Ghost_COMMAND 'O'           /* ͩ����֤Υ��ޥ��*/
#define Time_COMMAND  'P'           /*���ߤλ��֤�������*/
#define Chu_COMMAND   'Q'
#define Catch_COMMAND 'R'           /* ��Ȥ���Υ��ޥ��*/
#define Fall_COMMAND 'S'           /* ��Ȥ���Υ��ޥ��*/ 
#define Start_COMMAND 'T'            /* �������ȤΥ��ޥ��*/          

#endif
