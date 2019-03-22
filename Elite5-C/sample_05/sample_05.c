/* ********************************************************
Copyright (C )2016 Beijing Senseshield Technology Co.,Ltd
Author : senseshield 2017-6
Description : ��ʾ����RuntimeAPI���豸��PC�ķ�����

History :
[���� ���� �޸�����]
********************************************************* */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "common.h"
#include "ss_lm_runtime.h"  //runtime ��ʹ��ע�����������"readme.txt"
#include "cJSON/cJSON.h"

int main()
{
	SS_UINT32 ret = SS_OK;
	ST_INIT_PARAM st_init_param			= {0};
	ST_LOGIN_PARAM login_param			= {0};
	SLM_HANDLE_INDEX slm_handle			= 0;
	SS_UINT32 ulRAWLen					= 0;
	SS_CHAR *session_info				= NULL;	
	SS_BYTE *pData						= NULL;
	cJSON *pjson_session				= NULL;
	cJSON *pjson_mac					= NULL;

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
		��д������(RAW)
		ʹ�ô�RAW������������д��PC��Ϣ���Ժ�ɶ�ȡ������ͨ���ж�������PC�ͼ�����֮��İ󶨣�ʹ���߿ɸ����Լ�ҵ��������ж��ơ�                                                       
	*/
	/************************************************************************/
	
	//1���������У���ȡ��PC-MAC��Ϣ(��DEMO��ʾʹ����Ϣ�а�����MAC��Ϣ��ʹ���߿����Զ���buffer����)��д�뵽RAW��
	ret = slm_get_info( slm_handle, SESSION_INFO, JSON, &session_info );
	if(SS_OK != ret)
	{
		printf("slm_get_info[SESSION_INFO] error : 0x%08X\n", ret);
		goto CLEAR;
	}
	pjson_session = cJSON_Parse(session_info);
	pjson_mac = cJSON_GetObjectItem( pjson_session, "MAC" );

	ret = slm_user_data_getsize(slm_handle, RAW, &ulRAWLen);
	if (ret == SS_OK && ulRAWLen > 0 && ulRAWLen >= strlen(pjson_mac->valuestring) )
	{
		ret = slm_user_data_write(slm_handle, (SS_BYTE*)pjson_mac->valuestring, 0, strlen(pjson_mac->valuestring) );
		printf( "slm_user_data_write(RAW) : %s\n\n", pjson_mac->valuestring);	
	}
	else
	{
		printf("RAW size is too small to write SESSION_INFO\n");
		goto CLEAR;
	}
	slm_free( session_info );
	session_info = NULL;
	if(pjson_session)
		cJSON_Delete(pjson_session);

	//2�������ٴ����У��ٴλ�ȡPC_MAC��Ϣ������ȡ��֮ǰд��RAW����PC_MAC��Ϣ
	ret = slm_get_info( slm_handle, SESSION_INFO, JSON, &session_info );
	pjson_session = cJSON_Parse(session_info);
	pjson_mac = cJSON_GetObjectItem( pjson_session, "MAC" );
	
	pData = (SS_BYTE *)calloc(sizeof(SS_BYTE), strlen(pjson_mac->valuestring) + 1); 
	ret = slm_user_data_read(slm_handle, RAW, pData, 0, strlen(pjson_mac->valuestring));   
	printf( "\nslm_user_data_read(RAW) : %s\n\n", pData);	


	//3���Ա���Ϣ������ͬ����֤����������Ӧ��PC������ʹ�ã���������ʹ��
	if( strcmp(pjson_mac->valuestring, (char*)pData) == 0 )
	{
		printf( "the sense device can use in this computer!\n");
	}
	else
	{
		printf( "the sense device can not use in this computer!\n");
	}

CLEAR:
	if(pData)
	{
		free(pData);        
		pData = NULL;
	}
	if(session_info)
	{
		slm_free( session_info );
		session_info = NULL;
	}
	if(pjson_session)
		cJSON_Delete(pjson_session);

	//  ��ɵǳ� & ����ʼ������(slm_cleanup)����slm_init��
	if ( 0 != slm_handle )
	{
		slm_logout(slm_handle);
	}
	slm_cleanup();
	printf("\npress any key exit process.\n");
	getchar();
	return 0;
}