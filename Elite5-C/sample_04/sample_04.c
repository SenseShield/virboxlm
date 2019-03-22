/* ********************************************************
Copyright (C )2016 Beijing Senseshield Technology Co.,Ltd
Author : senseshield 2017-6
Description : 演示调用RuntimeAPI设备操作的基本使用方法。

History :
[作者 日期 修改内容]
********************************************************* */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "common.h"
#include "ss_lm_runtime.h"  //runtime 库使用注意事项详情见"readme.txt"
#pragma warning(disable : 4996)
#include "cJSON/cJSON.h"



//使用单独子函数演示获取设备信息 获取外壳号
void getDevInfo();
//演示获取设备描述信息
void getDevDesc(cJSON *pDevs, int iDev);
//演示获取单个设备中所有许可的id值，返回许可的个数
int getDevLicenseIDs(cJSON *pDevs, int iDev, unsigned int* pOutIDs);
//演示获取单个设备的所有许可内容(根据许可ID进行获取)
void getDevLicenseinfo(cJSON *pDevs, unsigned int* plicenseIDs, int iLicenseCnt, int iDev);

int main()
{
	SS_UINT32 ret						= SS_OK;
	ST_INIT_PARAM st_init_param			= {0};	

	SS_CHAR *pchar_Devdescs				= NULL;
	cJSON *pjson_Devdescs				= NULL;

	SS_UINT32 index_device				= 0;
	SS_UINT32 dev_num					= 0;

	SS_UINT32 *pLicenseIDs				= NULL;
	SS_UINT32 uLicenseCnt				= 0;


	/************************************************************************/
	/*     单独使用子函数来演示 获取设备信息 获取外壳号                            */
	/************************************************************************/
	getDevInfo();

	// 初始化接口调用，参数初始化
	st_init_param.version = SLM_CALLBACK_VERSION02;
	st_init_param.pfn = NULL;
	st_init_param.flag = SLM_INIT_FLAG_NOTIFY;
	memcpy( st_init_param.password, api_password, sizeof(api_password) );
	ret = slm_init( &(st_init_param) );	
	if(SS_OK != ret)
	{
		printf("slm_init error : 0x%08X\n", ret);
		goto CLEAR;
	}
	else
	{
		printf("slm_init ok\n");
	}
	ret = slm_enum_device( &pchar_Devdescs );
	//将返回的设备描述解析成JSON
	pjson_Devdescs = cJSON_Parse(pchar_Devdescs);
	if (!pjson_Devdescs)
	{
		printf("cJSON_Parse error\n");
		goto CLEAR;
	}
	//获取设备数量
	dev_num	= cJSON_GetArraySize( pjson_Devdescs );
	printf("device count : %d\n", dev_num );
	
	/************************************************************************/
	/*      获取设备描述信息                            */
	/************************************************************************/		
	for( index_device = 0; index_device < dev_num; index_device++ )
	{
		printf("device %d[device-desc]:\n", index_device);
		getDevDesc(pjson_Devdescs, index_device);
	}
	/************************************************************************/
	/*      获取设备中所有许可的ID                           */
	/************************************************************************/		
	for( index_device = 0; index_device < dev_num; index_device++ )
	{
		printf("device %d[license-ids]:\n", index_device);
		getDevLicenseIDs(pjson_Devdescs, index_device,NULL);
	}
	/************************************************************************/
	/*      根据许可ID获取许可信息                           */
	/************************************************************************/		
	for( index_device = 0; index_device < dev_num; index_device++ )
	{
		uLicenseCnt = getDevLicenseIDs(pjson_Devdescs, index_device,NULL);
		if(uLicenseCnt)
		{
			pLicenseIDs = (SS_UINT32*)malloc(uLicenseCnt * sizeof(SS_UINT32));
			if( pLicenseIDs )
			{
				uLicenseCnt = getDevLicenseIDs(pjson_Devdescs, index_device,pLicenseIDs);
				//获取设备的所有许可内容
				getDevLicenseinfo(pjson_Devdescs, pLicenseIDs, uLicenseCnt, index_device);
				free(pLicenseIDs);
				pLicenseIDs = NULL;
			}
			else
			{
				printf("device %d : malloc error\n",index_device );
			}
		}
		else
		{
			printf("device %d : no licenses\n",index_device );
		}
	}

CLEAR:
	if(pjson_Devdescs)
		cJSON_Delete(pjson_Devdescs);
	if(pchar_Devdescs)
		slm_free(pchar_Devdescs);

	slm_cleanup();
	printf("\npress any key exit process.\n");
	getchar();
	return 0;
}

void getDevInfo()
{
	SS_UINT32 ret = SS_OK;
	ST_INIT_PARAM st_init_param			= {0};
	ST_LOGIN_PARAM login_param			= {0};
	SLM_HANDLE_INDEX slm_handle			= 0;
	SS_CHAR *lock_info					= NULL;
	cJSON   *pjson_DevInfo		= NULL;
	cJSON	*pjson_Item			= NULL;

	// 初始化接口调用，参数初始化
	st_init_param.version = SLM_CALLBACK_VERSION02;
	st_init_param.pfn = NULL;
	st_init_param.flag = SLM_INIT_FLAG_NOTIFY;
	memcpy( st_init_param.password, api_password, sizeof(api_password) );
	ret = slm_init( &(st_init_param) );
	if(SS_OK != ret)
	{
		printf("slm_init error : 0x%08X\n", ret);
		goto CLEAR;
	}
	// 安全登录许可，得到许可句柄(SLM_HANDLE_INDEX), 供其它接口使用
	login_param.license_id = 1;
	login_param.size = sizeof(ST_LOGIN_PARAM);
	login_param.timeout = 86400;
	ret = slm_login( &login_param, STRUCT, &(slm_handle), NULL );
	if(SS_OK != ret)
	{
		printf("slm_login error : 0x%08X\n", ret);
		goto CLEAR;
	}
	// 获取信息 根据枚举类型INFO_TYPE的值(指定想要获取的信息名称)
	// 调用成功后，需要对接收内存地址进行释放(slm_free)
	ret = slm_get_info( slm_handle, LOCK_INFO, JSON, &lock_info );
	if(SS_OK != ret)
	{
		printf("slm_get_info[LOCK_INFO] error : 0x%08X\n", ret);
		goto CLEAR;
	}
	else
	{
		printf( "slm_get_info(lock_info) : %s\n", lock_info );
	}

	pjson_DevInfo = cJSON_Parse(lock_info);
	if (!pjson_DevInfo){
		printf("cJSON_Parse error\n");
		goto CLEAR;
	}
	pjson_Item = cJSON_GetObjectItem( pjson_DevInfo, "shell_num" );
	if(pjson_Item)
		printf("	shell_num : %s\n",pjson_Item->valuestring);



	slm_free( lock_info );
	lock_info = NULL;


CLEAR:
	//  许可登出 & 反初始化函数(slm_cleanup)，与slm_init对应
	if ( 0 != slm_handle )
	{
		ret = slm_logout(slm_handle);
		if (SS_OK != ret) 
		{
			printf("slm_logout error : 0x%08X", ret);
		}
	}
	if(pjson_DevInfo)
		cJSON_Delete(pjson_DevInfo);
	slm_cleanup();
	return;

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
int getDevLicenseIDs(cJSON *pDevs, int iDev, unsigned int* pOutIDs)
{
	SS_UINT32 ret			= 0;
	cJSON *pjson_Array		= NULL;
	SS_CHAR *pchar_Array	= NULL;
	SS_CHAR *pLicenseIDs	= NULL;
	cJSON *pjsonLicenseIDs	= NULL;
	cJSON *pjsonLicenseID	= NULL;
	SS_UINT32 license_cnt	= 0;
	SS_UINT32 index_license	= 0;

	//获取一个设备描述结构
	pjson_Array = cJSON_GetArrayItem( pDevs, iDev );
	//json转为char* 需要释放返回的buffer地址
	pchar_Array = cJSON_PrintUnformatted(pjson_Array);
	
	ret = slm_enum_license_id(pchar_Array, &pLicenseIDs);
	if( 0 == ret )
	{
		printf("license ids(json) :\n%s\n", pLicenseIDs);

		pjsonLicenseIDs = cJSON_Parse(pLicenseIDs);
		license_cnt = cJSON_GetArraySize(pjsonLicenseIDs);

		printf("license ids(unsigned int) :\n");
		for( index_license =0 ; index_license < license_cnt; index_license++ )
		{
			pjsonLicenseID = cJSON_GetArrayItem(pjsonLicenseIDs, index_license);
			printf("%u,",(SS_UINT32)pjsonLicenseID->valuedouble);

			//使用 LicenseID 类型必须为 unsigned int
			if(pOutIDs)
				pOutIDs[index_license] = (SS_UINT32)pjsonLicenseID->valuedouble;

		}
		printf("\n");
		if(pjsonLicenseIDs)
			cJSON_Delete(pjsonLicenseIDs);
		if(pLicenseIDs)
			slm_free(pLicenseIDs);
	}
	else
	{
		printf("slm_enum_license_id error : 0x%08X\n", ret);
	}
	if(pchar_Array)
		free(pchar_Array);

	return license_cnt;
}
void getDevLicenseinfo(cJSON *pDevs, unsigned int* plicenseIDs, int iLicenseCnt, int iDev)
{
	SS_UINT32 ret			= 0;
	cJSON *pjson_Array		= NULL;
	SS_CHAR *pchar_Array	= NULL;
	SS_CHAR *pLicenseinfo	= NULL;
	SS_INT32 index_license	= 0;

	//获取一个设备描述结构
	pjson_Array = cJSON_GetArrayItem( pDevs, iDev );
	//json转为char* 需要释放返回的buffer地址
	pchar_Array = cJSON_PrintUnformatted(pjson_Array);

	for(index_license = 0; index_license < iLicenseCnt; index_license++ )
	{
		ret = slm_get_license_info(pchar_Array, plicenseIDs[index_license],&pLicenseinfo);
		if(0 == ret )
		{
			printf("license %u:\n%s\n", plicenseIDs[index_license], pLicenseinfo);
		}
		else
		{
			printf("slm_get_license_info error : 0x%08X\n", ret);
		}
		//释放内存
		if(pLicenseinfo)
			slm_free(pLicenseinfo);
	}

	if(pchar_Array)
		free(pchar_Array);
	return ;
}