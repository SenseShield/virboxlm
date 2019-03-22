/* ********************************************************
Copyright (C )2016 Beijing Senseshield Technology Co.,Ltd
Author : senseshield 2017-6
Description : 演示调用RuntimeAPI数据区的基本使用方法。

History :
[作者 日期 修改内容]
********************************************************* */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "common.h"
#include "ss_lm_runtime.h"  //runtime 库使用注意事项详情见"readme.txt"

/** 用户数据区最大*/
#ifndef MAX_USER_DATA_SIZE
	#define MAX_USER_DATA_SIZE          32 * 1024
#endif // MAX_USER_DATA_SIZE

void pub_data_demo();

int main()
{
	SS_UINT32 ret = SS_OK;
	ST_INIT_PARAM st_init_param			= {0};
	ST_LOGIN_PARAM login_param			= {0};
	SLM_HANDLE_INDEX slm_handle			= 0;
	SS_UINT32 ulRAWLen					= 0;
	SS_UINT32 ulPUBLen					= 0;
	SS_UINT32 ulROWLen					= 0;  
	SS_CHAR *lic_info					= NULL;
	SS_CHAR *session_info				= NULL;
	SS_BYTE testdata[65535]				= {0};
	SS_UINT32 ulTestData				= 0;
	SS_BYTE *pData						= NULL;
	SS_UINT32 index						= 0;
	SS_UINT32 nCount					= 0;
	SS_UINT32 nRemainLen				= 0;
	SS_CHAR *testWrite					= "123456789";
	SS_UINT32 iOffset					= 0;

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

	// 安全登录许可
	login_param.license_id = 1;
	login_param.size = sizeof(ST_LOGIN_PARAM);
	login_param.timeout = 86400;
	ret = slm_login( &login_param, STRUCT, &(slm_handle), NULL );
	if(SS_OK != ret)
	{
		printf("slm_login error : 0x%08X\n", ret);
		goto CLEAR;
	}
	else
	{
		printf("slm_login ok\n");
	}

	/************************************************************************/
	/*  
		只读数据区(ROM)
		可以用此区作软件所需要的数据的存储，例如一些配置信息。
		只读区较读写区更为常用，原因是所存储的内容不会在未授权的情况下被任意更改
		更改只读区内容只有一种方法：远程的安全升级包(详见许可签发DEMO模块)                                                                   
	*/
	/************************************************************************/

	ret = slm_user_data_getsize(slm_handle, ROM, &ulROWLen);
	if(SS_OK != ret)
	{
		printf("slm_user_data_getsize[ROM] error : 0x%08X\n", ret);
		goto CLEAR;
	}
	else
	{
		printf( "slm_user_data_getsize[ROM] ok rom_size : %d\n", ulROWLen );
	} 
	if (ret == SS_OK && ulROWLen > 0)
	{        
		pData = (SS_BYTE *)calloc(sizeof(SS_BYTE), ulROWLen);        
		ret = slm_user_data_read(slm_handle, ROM, pData, 0, ulROWLen);        
		if(SS_OK != ret)
		{
			printf("slm_user_data_read[ROM] error : 0x%08X\n", ret);
			goto CLEAR;
		}
		else
		{
			printf( "slm_user_data_read[ROM] ok\n" );
		} 

		// 可在此处理获取到的数据  

		free(pData);        
		pData = NULL;    
	}
	/************************************************************************/
	/*  
		读写数据区(RAW)
		读写区可以让开发商把运行过程中需要保存的数据保存在锁内，下次启动的时候可以访问。 
		访问之前一定要先登录到许可，如果许可失效，则无法读取使用任何该内存                                                          
	*/
	/************************************************************************/
	// 1、RAW数据区写入(使用偏移进行写入演示)
	ret = slm_user_data_getsize(slm_handle, RAW, &ulRAWLen);
	if(SS_OK != ret)
	{
		printf("slm_user_data_getsize[RAW] error : 0x%08X\n", ret);
		goto CLEAR;
	}
	else
	{
		printf( "slm_user_data_getsize[RAW] ok ram_size : %d\n", ulRAWLen );
	} 

	iOffset = 0;
	ret = slm_user_data_write(slm_handle, (SS_BYTE*)testWrite, iOffset, 5); 
	if(SS_OK != ret)
	{
		printf("slm_user_data_write[RAW][OFFSET] offset=0 error : 0x%08X\n", ret);
		goto CLEAR;
	}
	iOffset += 5;
	ret = slm_user_data_write(slm_handle, (SS_BYTE*)testWrite+5, iOffset, 4); 
	if(SS_OK != ret)
	{
		printf("slm_user_data_write[RAW][OFFSET] offset=5 error : 0x%08X\n", ret);
		goto CLEAR;
	}
	printf( "slm_user_data_write[RAW][OFFSET]: %s\n", testWrite);
	ret = slm_user_data_read(slm_handle, RAW, testdata, 0, 9);        
	if(SS_OK != ret)
	{
		printf("slm_user_data_read[RAW][OFFSET] error : 0x%08X\n", ret);
		goto CLEAR;
	}
	else
	{
		printf("slm_user_data_read[RAW][OFFSET]: %s\n",testdata);
	}	
	// 2、RAW数据区写入(短数据、长数据写入演示)，每次最多可写入 SLM_MAX_WRITE_SIZE(1904)字节	
	//短数据写入
	memset(testdata, '1', sizeof(testdata) );
	ulTestData = 10;
	if( ulTestData <= SLM_MAX_WRITE_SIZE )
	{
		ret = slm_user_data_write(slm_handle, testdata, 0, ulTestData);        
		if(SS_OK != ret)
		{
			printf("slm_user_data_write[RAW][SHORT] error : 0x%08X\n", ret);
			goto CLEAR;
		}
		else
		{
			printf("slm_user_data_write[RAW][SHORT] ok\n");
		}  
	}
	//长数据写入
	ulTestData = ulRAWLen;
	if( ( ulTestData >= SLM_MAX_WRITE_SIZE ) && ( ulTestData <= ulRAWLen ) )
	{
		nCount = ulTestData / SLM_MAX_WRITE_SIZE;
		nRemainLen = ulTestData % SLM_MAX_WRITE_SIZE;
		for(index = 0; index < nCount; index++ )
		{
			ret = slm_user_data_write(slm_handle, &testdata[SLM_MAX_WRITE_SIZE*index], SLM_MAX_WRITE_SIZE * index, SLM_MAX_WRITE_SIZE);        
			if(SS_OK != ret)
			{
				printf("slm_user_data_write[RAW][LONG] %d error : 0x%08X\n", index, ret);
				goto CLEAR;
			}  
		}
		if( nRemainLen )
		{
			ret = slm_user_data_write(slm_handle, &testdata[SLM_MAX_WRITE_SIZE*index], SLM_MAX_WRITE_SIZE * index, nRemainLen);        
			if(SS_OK != ret)
			{
				printf("slm_user_data_write[RAW][LONG](remain) error : 0x%08X\n", index, ret);
				goto CLEAR;
			}  
		}
	}
	// 3、RAW数据区读取(短数据、长数据写读取演示)，每次最多可读取 MAX_USER_DATA_SIZE(32*1024) 字节
	//短数据读取
	memset(testdata, 0, sizeof(testdata) );
	ulTestData = 10;
	if( ulTestData <= MAX_USER_DATA_SIZE )
	{				  
		ret = slm_user_data_read(slm_handle, RAW, testdata, 0, ulTestData);        
		if(SS_OK != ret)
		{
			printf("slm_user_data_read[RAW][SHORT] error : 0x%08X\n", ret);
			goto CLEAR;
		}  
	}
	//长数据读取
	ulTestData = ulRAWLen;
	if( ( ulTestData >= MAX_USER_DATA_SIZE ) && ( ulTestData <= ulRAWLen ) )
	{
		nCount = ulTestData / MAX_USER_DATA_SIZE;
		nRemainLen = ulTestData % MAX_USER_DATA_SIZE;
		for(index = 0; index < nCount; index++ )
		{
			ret = slm_user_data_read(slm_handle, RAW, &testdata[MAX_USER_DATA_SIZE*index], MAX_USER_DATA_SIZE * index, MAX_USER_DATA_SIZE);        
			if(SS_OK != ret)
			{
				printf("slm_user_data_read[RAW][LONG] %d error : 0x%08X\n", index, ret);
				goto CLEAR;
			}  
		}
		if( nRemainLen )
		{
			ret = slm_user_data_read(slm_handle, RAW, &testdata[MAX_USER_DATA_SIZE*index], MAX_USER_DATA_SIZE * index, nRemainLen);        
			if(SS_OK != ret)
			{
				printf("slm_user_data_read[RAW][LONG](remain) error : 0x%08X\n", index, ret);
				goto CLEAR;
			}  
		}
	}
	/************************************************************************/
	/*  
		全局数据区(PUB)
		全局数据区和ROM区数据访问区别是:全局数据区数据是绑定在0号许可的
		当用户不希望通过登录相应的许可，从而获取到公开区的时候
		用户可用通过调用slm_pub_data_getsize和slm_pub_data_read进行访问，前提是需要登录0号许可,代码演示在下方子函数pub_data_demo
		更改全局数据区内容只有一种方法:远程的安全升级包(详见许可签发DEMO模块)
	*/
	/************************************************************************/

	ret = slm_user_data_getsize(slm_handle, PUB, &ulPUBLen);  
	if (ret == SS_OK && ulPUBLen > 0)
	{        
		pData = (SS_BYTE *)calloc(sizeof(SS_BYTE), ulPUBLen);        
		ret = slm_user_data_read(slm_handle, PUB, pData, 0, ulPUBLen);        
		if(SS_OK != ret)
		{
			printf("slm_user_data_read[PUB] error : 0x%08X\n", ret);
			goto CLEAR;
		} 

		// 可在此处理获取到的数据  

		free(pData);        
		pData = NULL;    
	}

	//  许可登出  
	ret = slm_logout(slm_handle);
	if (SS_OK == ret) 
	{
		printf("slm_logout ok.\n");
	}
	else
	{
		printf("slm_logout error : 0x%08X", ret);
	}	

	//pub_data_demo函数将演示：登录0号许可，访问其他许可PUB区的内容
	pub_data_demo();
CLEAR:
	if(pData)
	{		
		free(pData);        
		pData = NULL;  
	}
	//反初始化函数(slm_cleanup)，与slm_init对
	slm_cleanup();
	printf("\npress any key exit process.\n");
	getchar();
	return 0;
}

void pub_data_demo()
{
	/************************************************************************/
	/*  
		全局数据区(PUB)
		登录0号许可，传入其它许可ID号(DEMO演示1号ID)，使用slm_pub_data_getsize和slm_pub_data_read来获取全局数据区的大小的内容。
	*/
	/************************************************************************/
	SS_UINT32 ret = SS_OK;
	ST_INIT_PARAM st_init_param			= {0};
	ST_LOGIN_PARAM login_param			= {0};
	SLM_HANDLE_INDEX slm_handle			= 0;
	SS_BYTE *pData						= NULL;
	SS_UINT32 ulPUBLen					= 0;
	SS_UINT32 ulLicenseID				= 1;

	// 初始化接口调用，参数初始化
	st_init_param.version = SLM_CALLBACK_VERSION02;
	st_init_param.pfn = NULL;
	st_init_param.flag = SLM_INIT_FLAG_NOTIFY;
	memcpy( st_init_param.password, api_password, sizeof(api_password) );
	ret = slm_init( &(st_init_param) );	

	// 安全登录许可 0 
	login_param.license_id = 0;
	login_param.size = sizeof(ST_LOGIN_PARAM);
	login_param.timeout = 86400;
	ret = slm_login( &login_param, STRUCT, &(slm_handle), NULL );	

	ret = slm_pub_data_getsize(slm_handle, ulLicenseID, &ulPUBLen);
	if (ret == SS_OK && ulPUBLen > 0)
	{        
		pData = (SS_BYTE *)calloc(sizeof(SS_BYTE), ulPUBLen);        
		ret = slm_pub_data_read(slm_handle, 1, pData, 0, ulPUBLen);

		// 可在此处理获取到的数据  

		free(pData);        
		pData = NULL;    
	}
	if ( 0 != slm_handle )
	{
		slm_logout(slm_handle);
	}
	slm_cleanup();
	printf("slm_pub_data_getsize/read(PUB) ok\n");
	return ;
}