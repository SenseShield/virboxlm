/* ********************************************************
Copyright (C )2016 Beijing Senseshield Technology Co.,Ltd
Author : senseshield 2017-6
Description : ��ʾ����RuntimeAPI�Ļ���ʹ�÷�����

History :
[���� ���� �޸�����]
********************************************************* */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <process.h>
#include "common.h"
#include "ss_lm_runtime.h"  //runtime ��ʹ��ע�����������"readme.txt"

//keepalive�̺߳���
unsigned int _stdcall threadproc_keep_alive(void* pParam);

//��ʼ��slm_init���������õĻص�����
SS_UINT32 SSAPI handle_service_msg( SS_UINT32 message, void* wparam, void* lparam );

int main()
{
	SS_UINT32 ret = SS_OK;
	ST_INIT_PARAM st_init_param			= {0};
	ST_LOGIN_PARAM login_param			= {0};
	SLM_HANDLE_INDEX slm_handle			= 0;

	SS_CHAR *lic_info					= NULL;
	SS_CHAR *lock_info					= NULL;
	SS_CHAR *session_info				= NULL;

	SS_BYTE original_data[32]			= {"test_data1234567890"};
	SS_BYTE encrypted_data[32]			= {0};
	SS_BYTE decrypted_data[32]			= {0};

	SS_UINT32 rom_size					= 0;
	SS_BYTE readbuffer[1024]			= {0};	

	SS_UINT32 mem_id					= 0;
	SS_UINT32 retsize					= 0;
	SS_BYTE mm_buffer[SLM_MEM_MAX_SIZE] = {"testmm/�����ڴ��й�"};

	SS_BYTE sign_data[32]				= {"test_data1234567890"};
	SS_BYTE signature[2048]				= {0};
	SS_CHAR *sign_info					= NULL;
	SS_UINT32 signature_length			= 0;

	// ��ʼ���ӿڵ��ã�������ʼ��
	st_init_param.version = SLM_CALLBACK_VERSION02;
	st_init_param.pfn = handle_service_msg;
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
	
	// ��ȫ��¼��ɣ��õ���ɾ��(SLM_HANDLE_INDEX), �������ӿ�ʹ��
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
	
	//����keepalive�߳�
	_beginthreadex(NULL, 0, threadproc_keep_alive, &slm_handle, 0, NULL);	

	// ��ȡ��Ϣ ����ö������INFO_TYPE��ֵ(ָ����Ҫ��ȡ����Ϣ����)
	// ���óɹ�����Ҫ�Խ����ڴ��ַ�����ͷ�(slm_free)
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
	slm_free( lock_info );
	lock_info = NULL;

	ret = slm_get_info( slm_handle, SESSION_INFO, JSON, &session_info );
	if(SS_OK != ret)
	{
		printf("slm_get_info[SESSION_INFO] error : 0x%08X\n", ret);
		goto CLEAR;
	}
	else
	{
		printf( "slm_get_info(SESSION_INFO) : %s\n", session_info );
	}
	slm_free( session_info );
	session_info = NULL;

	ret = slm_get_info( slm_handle, LICENSE_INFO, JSON, &lic_info );
	if(SS_OK != ret)
	{
		printf("slm_get_info[LICENSE_INFO] error : 0x%08X\n", ret);
		goto CLEAR;
	}
	else
	{
		printf( "slm_get_info(LICENSE_INFO) : %s\n", lic_info );
	}
	slm_free(lic_info);
	lic_info = NULL;

	// ��ɼ��� & ��ɽ���
	ret = slm_encrypt(slm_handle, original_data, encrypted_data, 32); 
	if(SS_OK != ret)
	{
		printf("slm_encrypt error : 0x%08X\n", ret);
		goto CLEAR;
	}
	else
	{
		printf( "slm_encrypt ok\n" );
	}
	ret = slm_decrypt(slm_handle, encrypted_data, decrypted_data, 32); 
	if(SS_OK != ret)
	{
		printf("slm_decrypt error : 0x%08X\n", ret);
		goto CLEAR;
	}
	else
	{
		printf( "slm_decrypt ok\n" );
	}
	
	// �����ɵ���������С�Ͷ�ȡ(��ָ��PUB��RAW��ROM)
	ret = slm_user_data_getsize(slm_handle, ROM, &rom_size);
	if(SS_OK != ret)
	{
		printf("slm_user_data_getsize error : 0x%08X\n", ret);
		goto CLEAR;
	}
	else
	{
		printf( "slm_user_data_getsize ok rom_size : %d\n", rom_size );
	}
	if (rom_size != 0)
	{	
		ret = slm_user_data_read(slm_handle, ROM, readbuffer, 0, rom_size);
		if(SS_OK != ret)
		{
			printf("slm_user_data_read error : 0x%08X\n", ret);
			goto CLEAR;
		}
		else
		{
			printf( "slm_user_data_read ok\n" );
		}
	}	
	
	// SS�ڴ��й��ڴ����롢д�롢��ȡ���ͷ�
	ret = slm_mem_alloc(slm_handle, SLM_MEM_MAX_SIZE, &(mem_id));
	if(SS_OK != ret)
	{
		printf("slm_mem_alloc error : 0x%08X\n", ret);
		goto CLEAR;
	}
	else
	{
		printf( "slm_mem_alloc ok\n" );
	}
	ret = slm_mem_write(slm_handle, mem_id, 0, SLM_MEM_MAX_SIZE, mm_buffer, &(retsize));
	if(SS_OK != ret)
	{
		printf("slm_mem_write error : 0x%08X\n", ret);
		goto CLEAR;
	}
	else
	{
		printf( "slm_mem_write ok\n" );
	}
	ret = slm_mem_read(slm_handle, mem_id, 0, SLM_MEM_MAX_SIZE, mm_buffer, &(retsize));
	if(SS_OK != ret)
	{
		printf("slm_mem_read error : 0x%08X\n", ret);
		goto CLEAR;
	}
	else
	{
		printf( "slm_mem_read ok\n" );
	}
	ret = slm_mem_free(slm_handle, mem_id);
	if(SS_OK != ret)
	{
		printf("slm_mem_free error : 0x%08X\n", ret);
		goto CLEAR;
	}
	else
	{
		printf( "slm_mem_free ok\n" );
	}
CLEAR:
	//  ��ɵǳ� & ����ʼ������(slm_cleanup)����slm_init��Ӧ
	if ( 0 != slm_handle )
	{
		ret = slm_logout(slm_handle);
		if (SS_OK == ret) 
		{
			printf("slm_logout ok.\n");
		}
		else
		{
			printf("slm_logout error : 0x%08X", ret);
		}
	}
	slm_cleanup();
	printf("\npress any key exit process.\n");
	getchar();
	return 0;
}

unsigned int _stdcall threadproc_keep_alive(void* pParam)
{
	int ret = 0;
	while (1)
	{
		ret = slm_keep_alive(*(SLM_HANDLE_INDEX*)(pParam)); 
		if (SS_OK != ret)
		{
			printf("slm_keep_alive error = 0x%08X\n", ret);
			break;
		}

		Sleep(1000 * 60);
	}
	_endthreadex(0);
	return 0;
}

/**
������Ϣ����ص�����

�ܹ����յ�Ӳ�����β���Ϣ��������ͣ����Ϣ��
 */
#define DEVICE_SN_LENGTH 16
SS_UINT32 SSAPI handle_service_msg( SS_UINT32 message, void* wparam, void* lparam )
{
    SS_UINT32 ret = SS_OK;
    SS_UINT32 result = 0;

    char szmsg[1024]={0};
    char lock_sn[DEVICE_SN_LENGTH] = {0};
    char szlock_sn[DEVICE_SN_LENGTH*2 + 1] = {0};

    switch(message)
    {
    case SS_ANTI_INFORMATION:   // ��Ϣ��ʾ
        sprintf(szmsg, "SS_ANTI_INFORMATION is  0x%08X wparam is %p", message, wparam);
        break;
    case SS_ANTI_WARNING:       // ����
        // �����Լ�顣һ������������Ϣ����������ֹͣ��������ҵ�񣬷�ֹ���򱻺ڿ͵��ԡ�
        switch ((unsigned int)(wparam))
        {
        case SS_ANTI_PATCH_INJECT: 
            sprintf(szmsg, "%s, ��Ϣ����=0x%08x, ���������=0x%08x","ע��", message, wparam);
            break;
        case SS_ANTI_MODULE_INVALID:
            sprintf(szmsg, "%s, ��Ϣ����=0x%08x, ���������=0x%08x","�Ƿ�ģ��DLL", message, wparam);
            break;
        case SS_ANTI_ATTACH_FOUND:
            sprintf(szmsg, "%s, ��Ϣ����=0x%08x, ���������=0x%08x","���ӵ���", message, wparam);
            break;
        case SS_ANTI_THREAD_INVALID:
            sprintf(szmsg, "%s, ��Ϣ����=0x%08x, ���������=0x%08x","�̷߳Ƿ�", message, wparam);
            break;
        case SS_ANTI_THREAD_ERROR:
            sprintf(szmsg, "%s, ��Ϣ����=0x%08x, ���������=0x%08x","�̴߳���", message, wparam);
            break;
        case SS_ANTI_CRC_ERROR:
            sprintf(szmsg, "%s, ��Ϣ����=0x%08x, ���������=0x%08x","�ڴ�ģ��CRCУ��", message, wparam);
            break;
        case SS_ANTI_DEBUGGER_FOUND:
            sprintf(szmsg, "%s, ��Ϣ����=0x%08x, ���������=0x%08x","���ֵ�����", message, wparam);
            break;
        default:
            sprintf(szmsg, "%s, ��Ϣ����=0x%08x, ���������=0x%08x","����δ֪����", message, wparam);
            break;
        }
        break;
    case SS_ANTI_EXCEPTION:         // �쳣
        sprintf(szmsg, "SS_ANTI_EXCEPTION is  0x%08X wparam is %p", message, wparam);
        break;
    case SS_ANTI_IDLE:              // �ݱ���
        sprintf(szmsg, "SS_ANTI_IDLE is  0x%08X wparam is %p", message, wparam);
        break;
    case SS_MSG_SERVICE_START:      // ��������
        sprintf(szmsg, "SS_MSG_SERVICE_START is  0x%08X wparam is %p", message, wparam);
        break;
    case SS_MSG_SERVICE_STOP:       // ����ֹͣ
        sprintf(szmsg, "SS_MSG_SERVICE_STOP is  0x%08X wparam is %p", message, wparam);
        break;
    case SS_MSG_LOCK_AVAILABLE:     // �����ã���������SS����ʱ���ѳ�ʼ����ɣ���wparam ��������
        // ��������Ϣ�����Ը������Ų�ѯ���������Ϣ��ʵ���Զ���¼����ȹ��ܡ�

        // �������Ƽ�¼������ת����ʮ�������ַ���
        memcpy(lock_sn, wparam, DEVICE_SN_LENGTH);
        bytes_to_hexstr((unsigned char*)lock_sn, DEVICE_SN_LENGTH, szlock_sn);

        sprintf(szmsg, "time is %08u, SS_MSG_LOCK_AVAILABLE is 0x%08X wparam is locksn -memory address %p, lock-sn is %s", 
            GetTickCount(), message, wparam, szlock_sn);
        break;
    case SS_MSG_LOCK_UNAVAILABLE:   // ����Ч�����Ѱγ�����wparam ��������
        // ���γ���Ϣ������ֻʹ������Ӧ�ó���һ���������γ�������޷�����ʹ�ã����鷢�ִ���Ϣ��ʾ�û��������ݣ������������Ȳ�����

        // �������Ƽ�¼������ת����ʮ�������ַ���
        memcpy(lock_sn, wparam, DEVICE_SN_LENGTH);
        bytes_to_hexstr((unsigned char*)lock_sn, DEVICE_SN_LENGTH, szlock_sn);

        sprintf(szmsg, "time is %08u SS_MSG_LOCK_UNAVAILABLE is 0x%08X wparam is locksn -memory address %p, lock-sn is %s", 
            GetTickCount(), message, wparam, szlock_sn);
        break;
    }    
    // �����ʽ�������Ϣ����
    printf("%s\n", szmsg);
    return ret;
}