#ifndef _H_SS_HELP_H_
#define _H_SS_HELP_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "d2c.h"
#include "ss_error.h"
#include "ss_define.h"
#include "ss_lm_runtime.h"
#include "ss_lm_control.h"

#include "common.h"
#include "cJSON/cJSON.h"
#include "ss_x509.h"

#pragma warning(disable : 4996)


#define CERT_SIZE 2048
#define LOCAL_MAKE	1			//	����������ʹ�ü��������кź�֤��ϳ�֤����
#define REMOTE_MAKE 2			//	����������ʹ�����ݿ������кź�֤����

#ifdef __cplusplus
extern "C" {
#endif

/*
*	name:	initSlm				��ʼ��slm��
*	return: error(1),ok(0)
*/
int initSlm();
/*
*	name:	cleanSlm			����slm��
*/
void cleanSlm();


/*
*	name:	getUserDevSN	��ȡ��ǰ�û��������к�
*	param1:	[out]deviceSN	��ǰ�û��������к�
*	return:	error(1),ok(0)
*/
int getUserDevSN(char* deviceSN);

/*
*	name:	getUserDevCert	��ȡ��ǰ�û������豸֤��
*	param1:	[out]pCert		��ǰ�û������豸֤��
*	param2:	[out]pCertLen	��ǰ�û������豸֤�鳤��
*	return:	error(1),ok(0)
*/
int getUserDevCert(unsigned char* pCert, unsigned int* pCertLen);

/*
*	name:	getUserDevP7b	��ȡ�û�����p7b֤��
*	param1:	[out]pCertP7b	p7b����buffer
*	param2:	[out]pCertLen	p7b����
*	return: error(1),ok(0)
*/
int getUserDevP7b(unsigned char *pCertP7b, unsigned int *pCertLen);

int masterPINVerify(MASTER_HANDLE hD2CHandle, char *pPIN, int nPinLen);
int masterPINDeauth(MASTER_HANDLE hD2CHandle);

#ifdef __cplusplus
}
#endif

#endif // _H_SS_HELP_H_

