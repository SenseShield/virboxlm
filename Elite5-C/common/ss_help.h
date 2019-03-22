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
#define LOCAL_MAKE	1			//	本地有锁，使用加密锁序列号和证书合成证书链
#define REMOTE_MAKE 2			//	本地无锁，使用数据库中序列号和证书链

#ifdef __cplusplus
extern "C" {
#endif

/*
*	name:	initSlm				初始化slm库
*	return: error(1),ok(0)
*/
int initSlm();
/*
*	name:	cleanSlm			清理slm库
*/
void cleanSlm();


/*
*	name:	getUserDevSN	获取当前用户锁的序列号
*	param1:	[out]deviceSN	当前用户锁的序列号
*	return:	error(1),ok(0)
*/
int getUserDevSN(char* deviceSN);

/*
*	name:	getUserDevCert	获取当前用户锁的设备证书
*	param1:	[out]pCert		当前用户锁的设备证书
*	param2:	[out]pCertLen	当前用户锁的设备证书长度
*	return:	error(1),ok(0)
*/
int getUserDevCert(unsigned char* pCert, unsigned int* pCertLen);

/*
*	name:	getUserDevP7b	获取用户锁的p7b证书
*	param1:	[out]pCertP7b	p7b接收buffer
*	param2:	[out]pCertLen	p7b长度
*	return: error(1),ok(0)
*/
int getUserDevP7b(unsigned char *pCertP7b, unsigned int *pCertLen);

int masterPINVerify(MASTER_HANDLE hD2CHandle, char *pPIN, int nPinLen);
int masterPINDeauth(MASTER_HANDLE hD2CHandle);

#ifdef __cplusplus
}
#endif

#endif // _H_SS_HELP_H_

