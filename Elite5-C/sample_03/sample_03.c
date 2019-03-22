/* ********************************************************
Copyright (C )2016 Beijing Senseshield Technology Co.,Ltd
Author : senseshield 2017-6
Description : ��ʾ����RuntimeAPI�������Ļ���ʹ�÷�����

History :
[���� ���� �޸�����]
********************************************************* */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "common.h"
#include "ss_lm_runtime.h"  //runtime ��ʹ��ע�����������"readme.txt"

/** �û����������*/
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

	// ��ʼ���ӿڵ��ã�������ʼ��
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

	// ��ȫ��¼���
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
		ֻ��������(ROM)
		�����ô������������Ҫ�����ݵĴ洢������һЩ������Ϣ��
		ֻ�����϶�д����Ϊ���ã�ԭ�������洢�����ݲ�����δ��Ȩ������±��������
		����ֻ��������ֻ��һ�ַ�����Զ�̵İ�ȫ������(������ǩ��DEMOģ��)                                                                   
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

		// ���ڴ˴����ȡ��������  

		free(pData);        
		pData = NULL;    
	}
	/************************************************************************/
	/*  
		��д������(RAW)
		��д�������ÿ����̰����й�������Ҫ��������ݱ��������ڣ��´�������ʱ����Է��ʡ� 
		����֮ǰһ��Ҫ�ȵ�¼����ɣ�������ʧЧ�����޷���ȡʹ���κθ��ڴ�                                                          
	*/
	/************************************************************************/
	// 1��RAW������д��(ʹ��ƫ�ƽ���д����ʾ)
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
	// 2��RAW������д��(�����ݡ�������д����ʾ)��ÿ������д�� SLM_MAX_WRITE_SIZE(1904)�ֽ�	
	//������д��
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
	//������д��
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
	// 3��RAW��������ȡ(�����ݡ�������д��ȡ��ʾ)��ÿ�����ɶ�ȡ MAX_USER_DATA_SIZE(32*1024) �ֽ�
	//�����ݶ�ȡ
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
	//�����ݶ�ȡ
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
		ȫ��������(PUB)
		ȫ����������ROM�����ݷ���������:ȫ�������������ǰ���0����ɵ�
		���û���ϣ��ͨ����¼��Ӧ����ɣ��Ӷ���ȡ����������ʱ��
		�û�����ͨ������slm_pub_data_getsize��slm_pub_data_read���з��ʣ�ǰ������Ҫ��¼0�����,������ʾ���·��Ӻ���pub_data_demo
		����ȫ������������ֻ��һ�ַ���:Զ�̵İ�ȫ������(������ǩ��DEMOģ��)
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

		// ���ڴ˴����ȡ��������  

		free(pData);        
		pData = NULL;    
	}

	//  ��ɵǳ�  
	ret = slm_logout(slm_handle);
	if (SS_OK == ret) 
	{
		printf("slm_logout ok.\n");
	}
	else
	{
		printf("slm_logout error : 0x%08X", ret);
	}	

	//pub_data_demo��������ʾ����¼0����ɣ������������PUB��������
	pub_data_demo();
CLEAR:
	if(pData)
	{		
		free(pData);        
		pData = NULL;  
	}
	//����ʼ������(slm_cleanup)����slm_init��
	slm_cleanup();
	printf("\npress any key exit process.\n");
	getchar();
	return 0;
}

void pub_data_demo()
{
	/************************************************************************/
	/*  
		ȫ��������(PUB)
		��¼0����ɣ������������ID��(DEMO��ʾ1��ID)��ʹ��slm_pub_data_getsize��slm_pub_data_read����ȡȫ���������Ĵ�С�����ݡ�
	*/
	/************************************************************************/
	SS_UINT32 ret = SS_OK;
	ST_INIT_PARAM st_init_param			= {0};
	ST_LOGIN_PARAM login_param			= {0};
	SLM_HANDLE_INDEX slm_handle			= 0;
	SS_BYTE *pData						= NULL;
	SS_UINT32 ulPUBLen					= 0;
	SS_UINT32 ulLicenseID				= 1;

	// ��ʼ���ӿڵ��ã�������ʼ��
	st_init_param.version = SLM_CALLBACK_VERSION02;
	st_init_param.pfn = NULL;
	st_init_param.flag = SLM_INIT_FLAG_NOTIFY;
	memcpy( st_init_param.password, api_password, sizeof(api_password) );
	ret = slm_init( &(st_init_param) );	

	// ��ȫ��¼��� 0 
	login_param.license_id = 0;
	login_param.size = sizeof(ST_LOGIN_PARAM);
	login_param.timeout = 86400;
	ret = slm_login( &login_param, STRUCT, &(slm_handle), NULL );	

	ret = slm_pub_data_getsize(slm_handle, ulLicenseID, &ulPUBLen);
	if (ret == SS_OK && ulPUBLen > 0)
	{        
		pData = (SS_BYTE *)calloc(sizeof(SS_BYTE), ulPUBLen);        
		ret = slm_pub_data_read(slm_handle, 1, pData, 0, ulPUBLen);

		// ���ڴ˴����ȡ��������  

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