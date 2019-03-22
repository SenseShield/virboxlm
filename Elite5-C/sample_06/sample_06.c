/* ********************************************************
Copyright (C )2016 Beijing Senseshield Technology Co.,Ltd
Author : senseshield 2017-6
Description : ��ʾ����ControlAPI�Ļ���ʹ�÷�����

History :
[���� ���� �޸�����]
********************************************************* */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "common.h"
#include "ss_error.h"
#include "ss_lm_control.h"
#include "cJSON/cJSON.h"

/*
ö�ټ�����
��ȡ��������Ϣ
ö�ټ����������Ϣ
ö�ټ������Ự��Ϣ
*/

//��ȡ�豸������Ϣ
void getDevDesc(cJSON *pDevs, int iDev);
//��ȡ�豸��Ϣ��Ϣ ��ȡ��Ǻ�
void getDevInfo(void *ipc, cJSON *pDevs, int iDev);
//��ȡ�����豸�µ����������Ϣ
void getDevLicense(void *ipc, cJSON *pDevs, int iDev);
//��ȡ������������session��Ϣ
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
	/*      ö�ټ�����                             */
	/************************************************************************/
	//��ȡ�����豸������ʹ������Ҫ���������豸id(developer_id)�����к���������
	//���� pchar_Devdescs ��Ҫslm_free�ͷ�
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

	//�����ص��豸����������JSON,����ֵ��ҪcJSON_Delete�ͷ�
	pjson_Devdescs = cJSON_Parse(pchar_Devdescs);
	if (!pjson_Devdescs){
		printf("cJSON_Parse error\n");
		goto CLEAR;
	}
	//��ȡ�豸����
	dev_num	= cJSON_GetArraySize( pjson_Devdescs );
	printf("device count : %d\n", dev_num );
	/************************************************************************/
	/*      ��ȡ�豸��Ϣ                            */
	/************************************************************************/		
	for( index = 0; index < dev_num; index++ )
	{
		printf("device_info : %d\n", index);
		getDevInfo(ipc, pjson_Devdescs, index);
	}
	/************************************************************************/
	/*      ��ȡ�豸������Ϣ                            */
	/************************************************************************/		
	for( index = 0; index < dev_num; index++ )
	{
		printf("device_desc : %d\n", index);
		getDevDesc(pjson_Devdescs, index);
	}
	/************************************************************************/
	/*      ��ȡ�豸�����Ϣ                            */
	/************************************************************************/		
	for( index = 0; index < dev_num; index++ )
	{
		printf("licenses:device : %d\n", index);
		getDevLicense(ipc, pjson_Devdescs, index);
	}
	/************************************************************************/
	/*      ��ȡ�豸�Ự��Ϣ                            */
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

	//��ȡһ���豸�����ṹ
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
	//��ʾ��ȡ��Ǻ���Ϣ

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

	//��ȡһ���豸�����ṹ
	pjson_Array = cJSON_GetArrayItem( pDevs, iDev );		

	//��ȡ�豸�����е�ÿ����Ա����(host_name��type��sn��developer_id��,�����豸���ͳ�Ա�������ܲ�ͬ,���籾����û��ip��port�ֶΣ������������е�)
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

	//��ȡһ���豸�����ṹ
	pjson_Array = cJSON_GetArrayItem( pDevs, iDev );
	//jsonתΪchar* ��Ҫ�ͷŷ��ص�buffer��ַ
	pchar_Array = cJSON_PrintUnformatted(pjson_Array);
	//ʹ��api��ȡ�豸�е������Ϣ�������Ӧ���豸����(char*����)(����lic_context��Ҫʹ��slm_free���������ͷ�)
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

	//��ȡһ���豸�����ṹ
	pjson_Array = cJSON_GetArrayItem( pDevs, iDev );
	//jsonתΪchar* ��Ҫ�ͷŷ��ص�buffer��ַ
	pchar_Array = cJSON_PrintUnformatted(pjson_Array);
	//ʹ��api��ȡ�豸�еĻỰ��Ϣ�������Ӧ���豸����(char*����)(����lic_context��Ҫʹ��slm_free���������ͷ�)
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