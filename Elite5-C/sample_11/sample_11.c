/* ********************************************************
Copyright (C )2016 Beijing Senseshield Technology Co.,Ltd
Author : senseshield 2017-6
Description : 演示用户锁恢复出厂设置的使用方法。

History :
[作者 日期 修改内容]
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

//组织恢复出厂设置json
SS_UINT32 build_reset_d2c(D2C_HANDLE hD2c);
//生成恢复出厂设置D2C包
SS_UINT32 make_reset_package();


int main()
{
	make_reset_package();
	printf("\npress any key exit process.\n");
	getchar();
	return 0;
}


// 生成恢复出厂设置D2C包
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
		
	//此示例只演示使用设备证书签发许可，使用云数据库信息获取p7b请参见在sample07示例


	/************************************************************************/
	/*                 1、制作p7b证书用于签发许可             */
	/************************************************************************/
	ret = getUserDevP7b(p7b_cert, &p7b_size);
	if (ret != 0)
	{
		printf("get_p7b error :0x%08X \n" ,ret);
		return 1;
	}
	/************************************************************************/
	/*                 2、打开控制锁并创建D2C对象            */
	/************************************************************************/
	ret = master_open(&hmaster);
	if (ret != 0)
	{
		printf("master_open error :0x%08X \n" ,ret);
		return 1;
	}
    // 新开发锁必须验证PIN码，否则无法使用
	// 详情见 ../common/common.h 文件中对 PIN_DEFAULT_PWD 的注释。
    ret = masterPINVerify(hmaster, PIN_DEFAULT_PWD, strlen(PIN_DEFAULT_PWD));
    if (ret != 0)
    {
        printf("masterPINVerify error :0x%08X \n", ret);
        return 1;
    }

	getUserDevSN(strDevSn);
	hexstr_to_bytes(strDevSn, SLM_LOCK_SN_LENGTH, deviceSn);

    // (1)种子码签发
    ret = master_get_root_ca(hmaster, root_ca_cert, root_ca_cert_size, &root_ca_cert_size);
    if (ret != 0)
    {
        printf("master_get_root_ca error :0x%08X \n", ret);
        return 1;
    }
	ret = d2c_file_new(hmaster, &hD2c, SIGN_TYPE_SEED, root_ca_cert, root_ca_cert_size);

    // (2)设备证书链签发
	//ret = d2c_file_new(hmaster, &hD2c, SIGN_TYPE_CERT, p7b_cert, p7b_size);
	if (ret != 0)
	{
		printf("d2c_file_seed_new error :0x%08X \n", ret);
		master_close(hmaster);
		return 1;
	}

	/************************************************************************/
	/*                 3、添加升级内容包到d2c句柄并获取签发完成的许可            */
	/************************************************************************/
	//添加升级内容包到d2c句柄
	ret = build_reset_d2c(hD2c);
	if (ret != 0)
	{
		printf("add_pkg error :0x%08X \n", ret);
		return 1;
	}
	//获取签发完的许可，获取buffer传入空，可获取到许可大小
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
            // 写入本地文件，升级至锁内需要通过 slm_update() API接口，或者使用用户许可工具的加密锁数据升级工具。
            strcpy(d2c_filename, strDevSn);
            strcat(d2c_filename, "_reset_package.d2c");
			write_file(d2c_filename, d2c, d2c_len);
			printf("生成D2C升级包成功：\n\t%s\n" ,d2c_filename);
		}
		else
		{
			printf("生成D2C升级包失败：[Error] 0x%08X\n", ret);
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
    // 限制恢复出厂设置升级包在签发后10min内有效（再次期间可重复使用）
	const int usetime_sec = 10 * 60;

    SS_UINT32 ret = 0;
    SS_CHAR temp[128] = {0};

    cJSON *root = NULL;

    root = cJSON_CreateObject();
    // 设置d2c类型为“重置”
    cJSON_AddStringToObject(root, "op", "reset");
    // 设置d2c的有效时间范围
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
