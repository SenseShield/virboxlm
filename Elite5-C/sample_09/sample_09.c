/* ********************************************************
Copyright (C )2016 Beijing Senseshield Technology Co.,Ltd
Author : senseshield 2017-6
Description : 演示许可管理(删除许可(单条/所有) 锁定/解锁许可)。

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

#define LIC_DEL_ONE		0
#define LIC_DEL_ALL		1
#define LIC_LOCK_ALL	2
#define LIC_UNLOCK_ALL	3


//组建许可信息(删除许可(单条/所有) 锁定/解锁许可)
//hD2C:D2C句柄 iWhich:选择许可操作(上边宏定义)
SS_UINT32 build_license_d2c(D2C_HANDLE hD2C, SS_UINT32 iWhich);

//许可管理(删除许可(单条/所有) 锁定/解锁许可)
//iWhich:选择许可操作(上边宏定义)
SS_UINT32 manage_license(SS_UINT32 iWhich);

int main()
{
	manage_license(LIC_DEL_ONE);
	manage_license(LIC_DEL_ALL);
	manage_license(LIC_LOCK_ALL);
	manage_license(LIC_UNLOCK_ALL);

	printf("\npress any key exit process.\n");
	getchar();
	return 0;
}


//许可管理(删除许可(单条/所有) 锁定/解锁许可)
SS_UINT32 manage_license(SS_UINT32 iWhich)
{
	SS_UINT32 ret								= 0;
	SS_CHAR strDevSn[SLM_LOCK_SN_LENGTH*2+1]	= {0};
	SS_BYTE deviceSn[SLM_LOCK_SN_LENGTH]		= {0};
	
	SS_BYTE p7b_cert[CERT_SIZE*3]				= {0};
	SS_UINT32 p7b_size							= CERT_SIZE*3;

	SS_CHAR* d2c								= NULL;
	SS_UINT32 d2c_len							= 0;
	D2C_HANDLE hD2c						= NULL;
	SS_CHAR d2c_filename[256]                   = {0};
	MASTER_HANDLE hmaster						= NULL;
		
	//此示例只演示使用设备证书签发许可，使用云数据库信息获取p7b请参见在sample07示例

	/************************************************************************/
	/*                 1、获取p7b证书用于生成许可       */
	/************************************************************************/	

	ret = getUserDevP7b(p7b_cert, &p7b_size);
	if (ret != 0)
	{
		printf("get_p7b error :0x%08X \n", ret);
		return 1;
	}
	/************************************************************************/
	/*                 2、打开控制锁并创建D2C对象            */
	/************************************************************************/
	ret = master_open(&hmaster);
	if (ret != 0)
	{
		printf("master_open error :0x%08X \n", ret);
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
	ret = d2c_lic_new(hmaster, &hD2c, ACCOUNT_TYPE_NONE, deviceSn, sizeof(deviceSn), p7b_cert, p7b_size);
	if (ret != 0)
	{
		printf("d2c_lic_new error :0x%08X \n", ret);
		master_close(hmaster);
		return 1;
	}
	/************************************************************************/
	/*                 3、添加许可信息到d2c句柄并获取许可            */
	/************************************************************************/
	//添加许可信息到d2c句柄
	ret = build_license_d2c(hD2c, iWhich);
	if (ret != 0)
	{
		printf("build_license_d2c error :0x%08X \n", ret);
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

			if(iWhich == LIC_DEL_ONE){
				strcat(d2c_filename, "_license_del_one.d2c");
			}else if(iWhich == LIC_DEL_ALL){
				strcat(d2c_filename, "_license_del_all.d2c");
			}else if(iWhich == LIC_LOCK_ALL){
				strcat(d2c_filename, "_license_lock_all.d2c");
			}else if(iWhich == LIC_UNLOCK_ALL){
				strcat(d2c_filename, "_license_unlock_all.d2c");
			}else{
				//...
			}
			write_file(d2c_filename, d2c, d2c_len);
			printf("生成D2C升级包成功：\n\t%s\n" ,d2c_filename);
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

SS_UINT32 build_license_d2c(D2C_HANDLE hD2C, SS_UINT32 bWhich)
{
	SS_UINT32 ret		= 0;
	SS_UINT32 licID		= 1;
	cJSON *root			= NULL;
	SS_CHAR lic_guid[D2C_GUID_LENGTH] = {0};
	
    root = cJSON_CreateObject();    
	if( bWhich == LIC_DEL_ONE)
	{
		// 删除单条指定许可
		cJSON_AddStringToObject(root, "op", "dellic");
		// 许可ID
		cJSON_AddNumberToObject(root, "license_id", licID);
	}
	if(bWhich == LIC_DEL_ALL)
	{
		//全部删除，包括数据区
		cJSON_AddStringToObject(root, "op", "delalllic");
	}
	if(bWhich == LIC_LOCK_ALL)
	{
		// 锁定所有许可
		cJSON_AddStringToObject(root, "op", "lockalllic");
	}
	if(bWhich == LIC_UNLOCK_ALL)
	{
		// 解锁所有许可
		cJSON_AddStringToObject(root, "op", "unlockalllic");
	}

    printf("%s\n", cJSON_Print(root));
	ret = d2c_add_lic(hD2C, cJSON_PrintUnformatted(root), "manage license sample", lic_guid);
	if (ret != 0)
	{
		printf("d2c_add_lic error :0x%08X\n", ret);
		return 1;
	}
    if (root)
    {
        free(root);
        root = NULL;
    }

	return ret;
}