/* ********************************************************
Copyright (C )2016 Beijing Senseshield Technology Co.,Ltd
Author : senseshield 2017-6
Description : 演示调用ControlAPI的基本使用方法。

History :
[作者 日期 修改内容]
********************************************************* */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "common.h"
#include "ss_error.h"
#include "ss_lm_control.h"
#include "cJSON/cJSON.h"

/*
枚举加密锁
读取加密锁信息
枚举加密锁许可信息
枚举加密锁会话信息
*/

//获取设备描述信息
void getDevDesc(cJSON *pDevs, int iDev);
//获取设备信息信息 获取外壳号
void getDevInfo(void *ipc, cJSON *pDevs, int iDev);
//获取单个设备下的所有许可信息
void getDevLicense(void *ipc, cJSON *pDevs, int iDev);
//获取单个锁的所有session信息
void getDevSession(void *ipc, cJSON *pDevs, int iDev);

int main()
{
	SS_UINT32 ret				= 0;
	void *ipc					= NULL;
	SS_CHAR *pchar_Devdescs		= NULL;
	cJSON *pjson_Devdescs		= NULL;
	SS_UINT32 index				= 0;
	SS_UINT32 dev_num			= 0;

	ret = slm_client_open(&ipc);
	if(SS_OK != ret)
	{
		printf("slm_client_open error : 0x%08X\n", ret);
		goto CLEAR;
	}
	else
	{
		printf("slm_client_open ok\n");
	}
	/************************************************************************/
	/*      枚举加密锁                             */
	/************************************************************************/
	//获取所有设备描述，使用者需要过滤所需设备id(developer_id)，进行后续操作。
	//出参 pchar_Devdescs 需要slm_free释放
	ret = slm_get_all_description(ipc, JSON, &pchar_Devdescs);
	if(SS_OK != ret)
	{
		printf("slm_get_all_description error : 0x%08X\n", ret);
		goto CLEAR;
	}
	else
	{
		printf("slm_get_all_description ok\n");
	}

	//将返回的设备描述解析成JSON,返回值需要cJSON_Delete释放
	pjson_Devdescs = cJSON_Parse(pchar_Devdescs);
	if (!pjson_Devdescs){
		printf("cJSON_Parse error\n");
		goto CLEAR;
	}
	//获取设备数量
	dev_num	= cJSON_GetArraySize( pjson_Devdescs );
	printf("device count : %d\n", dev_num );
	/************************************************************************/
	/*      获取设备信息                            */
	/************************************************************************/		
	for( index = 0; index < dev_num; index++ )
	{
		printf("device_info : %d\n", index);
		getDevInfo(ipc, pjson_Devdescs, index);
	}
	/************************************************************************/
	/*      获取设备描述信息                            */
	/************************************************************************/		
	for( index = 0; index < dev_num; index++ )
	{
		printf("device_desc : %d\n", index);
		getDevDesc(pjson_Devdescs, index);
	}
	/************************************************************************/
	/*      获取设备许可信息                            */
	/************************************************************************/		
	for( index = 0; index < dev_num; index++ )
	{
		printf("licenses:device : %d\n", index);
		getDevLicense(ipc, pjson_Devdescs, index);
	}
	/************************************************************************/
	/*      获取设备会话信息                            */
	/************************************************************************/		
	for( index = 0; index < dev_num; index++ )
	{
		printf("sessions:device : %d\n", index);
		getDevSession(ipc, pjson_Devdescs, index);
	}
CLEAR:
	if(ipc)
		slm_client_close(ipc);	
	if(pjson_Devdescs)
		cJSON_Delete(pjson_Devdescs);
	if(pchar_Devdescs)
		slm_free(pchar_Devdescs);
	printf("\npress any key exit process.\n");
	getchar();
	return 0;
}

void getDevInfo(void *ipc, cJSON *pDevs, int iDev)
{
	cJSON	*pjson_Array		= NULL;
	cJSON	*pjson_Item			= NULL;
	SS_CHAR *pchar_Array		= NULL;
	SS_CHAR *pDevInfo			= NULL;
	cJSON   *pjson_DevInfo		= NULL;

	//获取一个设备描述结构
	pjson_Array = cJSON_GetArrayItem( pDevs, iDev );
	pchar_Array = cJSON_PrintUnformatted(pjson_Array);

	slm_get_device_info(ipc, pchar_Array, &pDevInfo);

	//printf("device_info:\n%s\n", pDevInfo);
	/*
	"clock":        1500552195,
	"available_space":      517888,
	"total_space":  524288,
	"communication_protocol":       2,
	"lock_firmware_version":        "3.1.14",
	"lm_firmware_version":  "2.10.2.1",
	"h5_device_type":       0,
	"device_model": "e5",
	"hardware_version":     "0.0.2.0",
	"manufacture_date":     "2017-05-26 09:33:02",
	"lock_sn":      "9733c801000702054756001100050023",
	"slave_addr":   238,
	"shell_num":    "099900000648",
	"user_info":    "3039393930303030303634380000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000",
	"inner_info":   "00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000
	*/
	//演示获取外壳号信息

	pjson_DevInfo = cJSON_Parse(pDevInfo);
	if (!pjson_DevInfo){
		printf("cJSON_Parse error\n");
		goto CLEAR;
	}
	pjson_Item = cJSON_GetObjectItem( pjson_DevInfo, "shell_num" );
	if(pjson_Item)
		printf("	shell_num : %s\n",pjson_Item->valuestring);

CLEAR:
	if(pchar_Array)
		free(pchar_Array);
	if(pjson_DevInfo)
		cJSON_Delete(pjson_DevInfo);
	return ;

}

void getDevDesc(cJSON *pDevs, int iDev)
{
	cJSON *pjson_Array		= NULL;
	cJSON *pjson_Item		= NULL;

	//获取一个设备描述结构
	pjson_Array = cJSON_GetArrayItem( pDevs, iDev );		

	//获取设备描述中的每个成员变量(host_name、type、sn、developer_id等,根据设备类型成员参数可能不同,例如本地锁没有ip和port字段，而网络锁是有的)
	pjson_Item = cJSON_GetObjectItem( pjson_Array, "host_name" );
	if(pjson_Item)
		printf("	host_name : %s\n",pjson_Item->valuestring);

	pjson_Item = cJSON_GetObjectItem( pjson_Array, "type" );
	if(pjson_Item)
		printf("	type : %s\n",pjson_Item->valuestring);

	pjson_Item = cJSON_GetObjectItem( pjson_Array, "sn" );
	if(pjson_Item)
		printf("	sn : %s\n",pjson_Item->valuestring);

	pjson_Item = cJSON_GetObjectItem( pjson_Array, "developer_id" );
	if(pjson_Item)
		printf("	developer_id : %s\n",pjson_Item->valuestring);

	pjson_Item = cJSON_GetObjectItem( pjson_Array, "ip" );
	if(pjson_Item)
		printf("	ip : %s\n",pjson_Item->valuestring);

	pjson_Item = cJSON_GetObjectItem( pjson_Array, "port" );
	if(pjson_Item)
		printf("	port : %d\n",pjson_Item->valueint);
	return ;
}
void getDevLicense(void *ipc, cJSON *pDevs, int iDev)
{
	SS_UINT32 ret			= 0;
	cJSON *pjson_Array		= NULL;
	SS_CHAR *pchar_Array	= NULL;
	SS_CHAR *lic_context	= NULL;

	//获取一个设备描述结构
	pjson_Array = cJSON_GetArrayItem( pDevs, iDev );
	//json转为char* 需要释放返回的buffer地址
	pchar_Array = cJSON_PrintUnformatted(pjson_Array);
	//使用api获取设备中的许可信息，传入对应的设备描述(char*类型)(出参lic_context需要使用slm_free函数进行释放)
	ret = slm_read_brief_license_context(ipc, JSON, pchar_Array, &lic_context); 
	if(0 == ret )
	{
		printf(lic_context);
		printf("\n");
	}
	else
	{
		printf("slm_read_brief_license_context err : 0x%08X\n", ret);
	}

	if(lic_context)
		slm_free(lic_context);
	if(pchar_Array)
		free(pchar_Array);

	return ;
}
void getDevSession(void *ipc, cJSON *pDevs, int iDev)
{
	SS_UINT32 ret				= 0;
	cJSON *pjson_Array			= NULL;
	SS_CHAR *pchar_Array		= NULL;
	SS_CHAR *session_context	= NULL;

	//获取一个设备描述结构
	pjson_Array = cJSON_GetArrayItem( pDevs, iDev );
	//json转为char* 需要释放返回的buffer地址
	pchar_Array = cJSON_PrintUnformatted(pjson_Array);
	//使用api获取设备中的会话信息，传入对应的设备描述(char*类型)(出参lic_context需要使用slm_free函数进行释放)
	ret = slm_read_session_context(ipc, JSON, pchar_Array, &session_context);
	if(0 == ret )
	{
		printf(session_context);
		printf("\n");
	}
	else
	{
		printf("slm_read_session_context err : 0x%08X\n", ret);
	}

	if(session_context)
		slm_free(session_context);
	if(pchar_Array)
		free(pchar_Array);

	return ;
}