/* ********************************************************
Copyright (C )2016 Beijing Senseshield Technology Co.,Ltd
Author : senseshield 2017-6
Description : ��ʾ֤��ǩ����ɵĻ���ʹ�÷�����

History :
[���� ���� �޸�����]
********************************************************* */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <time.h>

#include "d2c.h"
#include "ss_error.h"
#include "ss_lm_runtime.h"

#include "common.h"
#include "cJSON/cJSON.h"
#include "base64/base64.h"
#include "ss_help.h"

//	������Ӳ�����к�
//  ��������ǰ���޸�����SN
//	ע��������˼���������������Ϣ�����ݿ��������
#define LOCK_SN "9733c8010007020547560014003a0032"

//	�������豸֤��
//  ��������ǰ���޸������豸֤��
//	ע��������˼���������������Ϣ�����ݿ⣬��������
#define P7BDATA "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"

//ǩ�����
SS_UINT32 make_license(SS_UINT32 choose);
//��֯���json����
SS_UINT32 add_license_d2c(D2C_HANDLE hD2c,SS_UINT32 licID);
//��ü����������кźͼ�����P7B֤��
SS_UINT32 getP7bAndSNFromDB(SS_CHAR *pSN, SS_BYTE *pCertP7b, SS_UINT32 *pCertLen);

int main()
{
	make_license(LOCAL_MAKE);
	make_license(REMOTE_MAKE);

	printf("\npress any key exit process.\n");
	getchar();
	return 0;
}
//�������
SS_UINT32 make_license(SS_UINT32 choose)
{
	SS_UINT32 i = 0;
	SS_UINT32 ret = 0;
	SS_CHAR strDevSn[SLM_LOCK_SN_LENGTH*2+1]	= {0};
	SS_BYTE deviceSn[SLM_LOCK_SN_LENGTH]		= {0};
	
	SS_BYTE p7b_cert[CERT_SIZE*3]				= {0};
	SS_UINT32 p7b_size							= CERT_SIZE*3;

	MASTER_HANDLE hmaster						= NULL;
	SS_CHAR* d2c								= NULL;
	SS_UINT32 d2c_len							= 0;
	D2C_HANDLE hD2c						= NULL;
    SS_CHAR d2c_filename[256]                   = {0};
		
	/************************************************************************/
	/*                 1����ȡp7b֤������ǩ�����(���ַ�ʽ����p7b)             */
	/************************************************************************/
	if( choose == LOCAL_MAKE )
	{
		ret = getUserDevP7b(p7b_cert, &p7b_size);
		getUserDevSN(strDevSn);
		hexstr_to_bytes(strDevSn, SLM_LOCK_SN_LENGTH, deviceSn);
	}
	else
	{
		ret = getP7bAndSNFromDB(strDevSn, p7b_cert, &p7b_size);
		hexstr_to_bytes(strDevSn, SLM_LOCK_SN_LENGTH, deviceSn);
	}	
	if (ret != 0)
	{
		printf("get_p7b error :0x%08X \n", ret);
		return 1;
	}
	/************************************************************************/
	/*                 2���򿪿�����������D2C����            */
	/************************************************************************/
	ret = master_open(&hmaster);
	if (ret != 0)
	{
		printf("master_open error :0x%08X \n", ret);
		return 1;
    }

    // �¿�����������֤PIN�룬�����޷�ʹ��
	// ����� ../common/common.h �ļ��ж� PIN_DEFAULT_PWD ��ע�͡�
    ret = masterPINVerify(hmaster, PIN_DEFAULT_PWD, strlen(PIN_DEFAULT_PWD));
    if (ret != 0)
    {
        printf("masterPINVerify error :0x%08X \n", ret);
        return 1;
    }

	ret = d2c_lic_new(hmaster, &hD2c, ACCOUNT_TYPE_NONE, deviceSn, sizeof(deviceSn), p7b_cert, p7b_size);
	if (ret != 0)
	{
		printf("d2c_lic_new error :0x%08X \n", ret);
		master_close(hmaster);
		return 1;
	}
	/************************************************************************/
	/*                 3����������Ϣ��d2c�������ȡǩ����ɵ����  (֧����Ӷ��������Ϣ)          */
	/************************************************************************/
	//��������Ϣ��d2c��� ��DEMO��ʾ���������ɣ����ID 1��2
	for(i = 0; i < 2; i++ )
	{
		ret = add_license_d2c(hD2c, i+1);
		if (ret != 0)
		{
			printf("add_license %d error :0x%08X \n", i+1, ret);
			master_close(hmaster);
			return 1;
		}
	}

	//��ȡǩ�������ɣ���ȡbuffer����գ��ɻ�ȡ����ɴ�С
	ret = d2c_get(hD2c, NULL, 0, &d2c_len);
	if (ret != 0 && d2c_len == 0)
	{
		printf("d2c_get error :0x%08X \n", ret);
	}
	else
	{
		d2c = (SS_CHAR*)malloc(d2c_len);
		ret = d2c_get(hD2c, d2c, d2c_len, &d2c_len);
		if (ret == SS_OK)
		{
            // д�뱾���ļ���������������Ҫͨ�� slm_update() API�ӿڣ�����ʹ���û���ɹ��ߵļ����������������ߡ�
            strcpy(d2c_filename, strDevSn);
			if( choose == LOCAL_MAKE )
				strcat(d2c_filename, "_local_license.d2c");
			else
				strcat(d2c_filename, "_remote_license.d2c");
			write_file(d2c_filename, d2c, d2c_len);
			printf("����D2C�������ɹ���\n\t%s\n" ,d2c_filename);
		}
		else
		{
			printf("d2c_get buffer error :0x%08X\n", ret);
		}
		free(d2c);
	}
    masterPINDeauth(hmaster);
	master_close(hmaster);
	d2c_delete(hD2c);
	return ret;
}

SS_UINT32 add_license_d2c(D2C_HANDLE hD2c,SS_UINT32 licID)
{
	SS_UINT32 ret = 0;
    SS_CHAR lic_guid[D2C_GUID_LENGTH] = {0};

    cJSON *root = NULL;
    cJSON *data = NULL;
    SS_CHAR temp[128] = {0};
    SS_CHAR *pub_data = "hello senseshield.";
    int module_array[4] = {1, 2, 63, 64};

    root = cJSON_CreateObject();
    
    // ������
    cJSON_AddStringToObject(root, "op", "addlic");
    // ǿ��д�루��ʶ��
    cJSON_AddBoolToObject(root, "force", cJSON_True);
    // ���ID
    cJSON_AddNumberToObject(root, "license_id", licID);

    /************************************************************************/
    /*                  �����������                                        */
    /************************************************************************/
    // һ����Ч�ڣ���ǰ��һ���
	sprintf(temp, "=%d", (int)time(NULL));
    cJSON_AddStringToObject(root, "start_time", temp);
	sprintf(temp, "=%d", (int)time(NULL) + 365 * 24 * 3600);
    cJSON_AddStringToObject(root, "end_time", temp);
    // ʱ���ȣ�1���£�
	sprintf(temp, "=%d", 30 * 24 * 3600);
    cJSON_AddStringToObject(root, "span", temp);
    // ʹ�ô�����1000�Σ�
    cJSON_AddStringToObject(root, "counter", "=1000");

    /************************************************************************/
    /*                      ��������                                        */
    /************************************************************************/
    // ��������������ʣ������������� concurrent_type ����Ϊ "process"��concurrent����Ϊ "=0"��
    // ���Ʋ������ͣ��Ự��������ͬʱ������ɵĵ�����
    // ���Ҫ���Ʒ�����ɵĳ����������� win_user_session ��Ϊ process��
    cJSON_AddStringToObject(root, "concurrent_type", "win_user_session");
    // ͬʱ���ʵ���������
    cJSON_AddStringToObject(root, "concurrent", "=3");

    /************************************************************************/
    /*                        ģ����                                        */
    /************************************************************************/
    // ���� module_array �����е�ģ����ã�ģ������ 1~64���ã�
    cJSON_AddItemToObject(root, "module", cJSON_CreateIntArray(module_array, 4));

    /************************************************************************/
    /*                        ������                                        */
    /************************************************************************/
    // ������
    data = cJSON_CreateObject();
    bytes_to_hexstr(pub_data, strlen(pub_data), temp);
    cJSON_AddStringToObject(data, "data", temp);
    cJSON_AddNumberToObject(data, "offset", 0);
    cJSON_AddNumberToObject(data, "resize", strlen(pub_data));
    cJSON_AddItemToObject(root, "pub", data);

    // ֻ����
    data = cJSON_CreateObject();
    cJSON_AddStringToObject(data, "data", "31323334353637383132333435363738");
    cJSON_AddNumberToObject(data, "offset", 0);
    cJSON_AddNumberToObject(data, "resize", 256);
    cJSON_AddItemToObject(root, "rom", data);

    // ��д��
    data = cJSON_CreateObject();
    cJSON_AddStringToObject(data, "data", "31323334353637383132333435363738");
    cJSON_AddNumberToObject(data, "offset", 0);
    cJSON_AddNumberToObject(data, "resize", 256);
    cJSON_AddItemToObject(root, "raw", data);
    
    printf("%s\n", cJSON_Print(root));
	ret = d2c_add_lic(hD2c, cJSON_PrintUnformatted(root), "add license sample", lic_guid);
    if (root)
    {
        free(root);
        root = NULL;
    }
	return ret;
}
SS_UINT32 getP7bAndSNFromDB(SS_CHAR *pSN, SS_BYTE *pCertP7b, SS_UINT32 *pCertLen)
{
	SS_UINT32 ret = 0;
	SS_CHAR SNTem[56] = {0};
	SS_BYTE *pDecodeP7bTemp = NULL;
	//��ȡ��������Ϣ�����ݿ��е���Ϣ��������˼��������������Ϣ(LOCK_SN �� P7BDATA)
	pDecodeP7bTemp = base64Decode(P7BDATA, pCertLen, 1);
	memcpy(pCertP7b, pDecodeP7bTemp, *pCertLen) ;
	base64Free(pDecodeP7bTemp);

    ss_x509_init();
	ret = ss_verify_p7b(pCertP7b, *pCertLen);
    ss_x509_cleanup();
	if (ret)
	{
		printf("ss_verify_p7b error :0x%08X\n", ret);
		return 1;
	}
	strcpy(SNTem, LOCK_SN);
	memcpy(pSN, SNTem, strlen(SNTem));	
	return 0;
}