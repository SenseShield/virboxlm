/* ********************************************************
Copyright (C )2016 Beijing Senseshield Technology Co.,Ltd
Author : senseshield 2017-6
Description : ��ʾ�û����ָ��������õ�ʹ�÷�����

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
#include "ss_help.h"

//��֯�ָ���������json
SS_UINT32 build_reset_d2c(D2C_HANDLE hD2c);
//���ɻָ���������D2C��
SS_UINT32 make_reset_package();


int main()
{
	make_reset_package();
	printf("\npress any key exit process.\n");
	getchar();
	return 0;
}


// ���ɻָ���������D2C��
SS_UINT32 make_reset_package()
{
	SS_UINT32 ret								= 0;
	SS_CHAR strDevSn[SLM_LOCK_SN_LENGTH*2+1]	= {0};
	SS_BYTE deviceSn[SLM_LOCK_SN_LENGTH]		= {0};
	
	SS_BYTE p7b_cert[CERT_SIZE*3]				= {0};
	SS_UINT32 p7b_size							= CERT_SIZE*3;

    SS_BYTE root_ca_cert[CERT_SIZE]				= {0};
    SS_UINT32 root_ca_cert_size					= CERT_SIZE;

	MASTER_HANDLE hmaster						= NULL;
	SS_CHAR* d2c								= NULL;
	SS_UINT32 d2c_len							= 0;
	D2C_HANDLE hD2c						= NULL;
    SS_CHAR d2c_filename[256]                   = {0};
		
	//��ʾ��ֻ��ʾʹ���豸֤��ǩ����ɣ�ʹ�������ݿ���Ϣ��ȡp7b��μ���sample07ʾ��


	/************************************************************************/
	/*                 1������p7b֤������ǩ�����             */
	/************************************************************************/
	ret = getUserDevP7b(p7b_cert, &p7b_size);
	if (ret != 0)
	{
		printf("get_p7b error :0x%08X \n" ,ret);
		return 1;
	}
	/************************************************************************/
	/*                 2���򿪿�����������D2C����            */
	/************************************************************************/
	ret = master_open(&hmaster);
	if (ret != 0)
	{
		printf("master_open error :0x%08X \n" ,ret);
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

	getUserDevSN(strDevSn);
	hexstr_to_bytes(strDevSn, SLM_LOCK_SN_LENGTH, deviceSn);

    // (1)������ǩ��
    ret = master_get_root_ca(hmaster, root_ca_cert, root_ca_cert_size, &root_ca_cert_size);
    if (ret != 0)
    {
        printf("master_get_root_ca error :0x%08X \n", ret);
        return 1;
    }
	ret = d2c_file_new(hmaster, &hD2c, SIGN_TYPE_SEED, root_ca_cert, root_ca_cert_size);

    // (2)�豸֤����ǩ��
	//ret = d2c_file_new(hmaster, &hD2c, SIGN_TYPE_CERT, p7b_cert, p7b_size);
	if (ret != 0)
	{
		printf("d2c_file_seed_new error :0x%08X \n", ret);
		master_close(hmaster);
		return 1;
	}

	/************************************************************************/
	/*                 3������������ݰ���d2c�������ȡǩ����ɵ����            */
	/************************************************************************/
	//����������ݰ���d2c���
	ret = build_reset_d2c(hD2c);
	if (ret != 0)
	{
		printf("add_pkg error :0x%08X \n", ret);
		return 1;
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
            strcat(d2c_filename, "_reset_package.d2c");
			write_file(d2c_filename, d2c, d2c_len);
			printf("����D2C�������ɹ���\n\t%s\n" ,d2c_filename);
		}
		else
		{
			printf("����D2C������ʧ�ܣ�[Error] 0x%08X\n", ret);
		}
		free(d2c);
	}
    masterPINDeauth(hmaster);
	master_close(hmaster);
	d2c_delete(hD2c);
	return ret;
}

SS_UINT32 build_reset_d2c(D2C_HANDLE hD2c)
{
    // ���ƻָ�����������������ǩ����10min����Ч���ٴ��ڼ���ظ�ʹ�ã�
	const int usetime_sec = 10 * 60;

    SS_UINT32 ret = 0;
    SS_CHAR temp[128] = {0};

    cJSON *root = NULL;

    root = cJSON_CreateObject();
    // ����d2c����Ϊ�����á�
    cJSON_AddStringToObject(root, "op", "reset");
    // ����d2c����Чʱ�䷶Χ
    cJSON_AddNumberToObject(root, "not_before", (double)time(NULL));
    cJSON_AddNumberToObject(root, "not_after", (double)time(NULL) + usetime_sec);

    printf("%s\n", cJSON_Print(root));
	ret = d2c_add_pkg(hD2c, cJSON_PrintUnformatted(root), "reset dongle");

    if (root)
    {
        free(root);
    }

	return ret;
}
