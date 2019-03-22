
/* ********************************************************
Copyright (C )2016 Beijing Senseshield Technology Co.,Ltd
Author : senseshield 2017-6
Description : ��ʾkeepalive�Ļ���ʹ�÷�����

History :
[���� ���� �޸�����]
********************************************************* */


#include <stdio.h>
#include <memory.h>
#include <windows.h>
#include <process.h>

#include "common.h"
#include "ss_lm_runtime.h"
#include "ss_user_login.h"

//���ڲ���Ƶ��ֱ�ӻ��ӵ���runtime�ӿڽ��в����Ŀͻ�����
//��ͨ��keepalive�ӿ�(ѭ������)���������˵�ͨ�ţ��Ա�ʾ�˿ͻ��˻��ڴ��

//��ʾ����ʾʹ��keepalive����������¼״̬,��Ҫʹ������д: ������ID�����˻����û��������롢��¼�����������ID
//������ID�鿴:��¼������ƽ̨(https://developer.senseyun.com)���鿴��������Ϣ�еĿ����̱�š�

//��дʾ��
//#define DEVELOPER_ID		"0300000000000006"
//#define CLOUD_USERNAME	"yaowb@sense.com.cn"
//#define CLOUD_PASSWORD	"sense12345"
//#define CLOUD_LICENSE_ID	198

#define DEVELOPER_ID		""
#define CLOUD_USERNAME		""
#define CLOUD_PASSWORD		""
#define CLOUD_LICENSE_ID	1


//��ʾ��������

//1 ��¼����
//2 ʹ��runtime�ӿڷ��������������ó�ʱʱ��(slm_login�ӿڲ���)Ϊ30��
//3 ����keepalive�̣߳�ѭ��30�����һ��
//4 ���߳�(������¼�߳�)Sleep 35 ��
//5 ����runtime�ӿ�slm_get_info��ȡһ��SESSION_INFO��Ϣ���в���
//6 �ǳ�����

// ���	���ϲ�����slm_get_info�ɹ�.
//		��������в���3����������keepalive�̣߳���slm_get_infoʧ��


//����guid
SS_CHAR user_guid_cloud[SS_CLOUD_MAX_USER_GUID_SIZE] = {0};

SS_UINT32 logincloud();		// ��¼����
SS_UINT32 logoutcloud();	// �ǳ�����
SS_UINT32 keep_alive_demo(SS_BOOL bUseKeep);				//demo ������
unsigned int _stdcall threadproc_keep_alive(void* pParam);	//keep alive �̺߳���


int main()
{
	SS_UINT32 ret = 0;
	printf("\n\n");

	if( !strlen(DEVELOPER_ID) || !strlen(CLOUD_USERNAME) || !strlen(CLOUD_PASSWORD) )
	{
		printf("\n��DEMO��Ҫʹ���߽� ������ID�������˺����롢��¼��������֤��ID ��дһ��!\n\n");
		printf("\npress any key exit process.\n");
		getchar();
		return 0;
	}

	ret = logincloud();
	if(ret){
		printf("\npress any key exit process.\n");
		getchar();
		return 1;
	}
	printf("\nKeep alive(no use):\n");
	keep_alive_demo(0);

	printf("\nKeep alive(use):\n");
	keep_alive_demo(1);

	logoutcloud();
	
	printf("\npress any key exit process.\n");
	getchar();
    return 0;
}
SS_UINT32 logincloud()
{
	SS_UINT32 ret = 0;

	ret = ss_cloud_user_login(DEVELOPER_ID, CLOUD_USERNAME, CLOUD_PASSWORD, user_guid_cloud);
	if(SS_OK != ret)
	{
		printf("	ss_cloud_user_login error : 0x%08X\n", ret);
		return 1;
	}
	printf("	ss_cloud_user_login ok\n");

	return 0;
}
SS_UINT32 logoutcloud()
{
	SS_UINT32 ret = 0;
	ret = ss_cloud_user_logout(user_guid_cloud);
	if(SS_OK != ret)
	{
		printf("\n	ss_cloud_user_logout error : 0x%08X\n", ret);
		return 1;
	}
	printf("\n	ss_cloud_user_logout ok\n");
	return 0;
}
SS_UINT32 keep_alive_demo(SS_BOOL bUseKeep)
{

	SS_UINT32 ret	= SS_OK;
	ST_INIT_PARAM init_param	= {0};
	ST_LOGIN_PARAM login_param	= {0};
	SLM_HANDLE_INDEX slm_handle = 0;
	SS_CHAR *session_info		= NULL;

	init_param.version = SLM_CALLBACK_VERSION02;
	init_param.pfn = NULL;
	init_param.timeout = 0;

	// ����API���룬ʾ��ͳһʹ��DEMO���������룬�ɷ���DEMO�û�����
	// ������ʽ�������û��������޸Ĵ����룬�����������ȡ�������ĵ�˵����
	// ���API���������ÿⲻƥ�䣬���ش���
	memcpy(init_param.password, api_password, sizeof(api_password));
	ret = slm_init(&(init_param));
	if(SS_OK != ret)
	{
		printf("	slm_init error : 0x%08X\n", ret);
		return 1;
	}
	else
	{
		printf("	slm_init ok\n");
	}
	// 2.��¼���(��ʾkeepaliveά��������¼״̬)
	// ��ʼ���ṹ���С�����룩
	login_param.size = sizeof(ST_LOGIN_PARAM);

	// ��¼�������ID
	login_param.license_id = CLOUD_LICENSE_ID;

	// ������������
	login_param.login_mode = SLM_LOGIN_MODE_CLOUD;

	// ������ɻỰ��ʱʱ�䣨��λ���룩���趨ʱ�������δ�����κ������ز�������ʱ����ʻ᷵����ɹ��ڴ�����󲻿ɳ���12Сʱ
	// ������Ҫ���µ�¼���ܼ���ʹ����ɡ�
	// ��ɻỰ����ɵ�¼����ɵǳ��ڼ��ʾһ�λỰ��
	// ��ɻỰ��ʱ����ɵ�¼��û�н����κ��������صĲ�����û�в���ʱ�䳬�����ó�ʱʱ�䱻��Ϊ����ǰ�ỰʧЧ����Ҫ���µ�¼��ɡ�
	login_param.timeout = 30;

	ret = slm_login(&login_param, STRUCT, &(slm_handle), NULL);
	if(SS_OK != ret)
	{
		printf("	slm_login error : 0x%08X\n", ret);
		return 1;
	}
	else
	{
		printf("	slm_login ok\n");
	}

	if(bUseKeep)
		_beginthreadex(NULL, 0, threadproc_keep_alive, &slm_handle, 0, NULL);	

	printf("\n	Sleep 35s ...\n\n");
	//35�벻ȥ����runtime�ӿڣ����ڵ�¼ʱ�����õ�30�룬֮���ٽ���runtime�ӿڵĵ����Ƿ�ɹ�����ʾslm_keep_alive������
	Sleep(1000 * 35);

	ret = slm_get_info( slm_handle, SESSION_INFO, JSON, &session_info );
	if(SS_OK != ret)
	{
		printf("	slm_get_info[SESSION_INFO] error : 0x%08X\n", ret);
		slm_logout(slm_handle);
		slm_cleanup();
		printf("	slm_logout ok.\n");
		return 1;
	}
	else
	{
		printf( "	slm_get_info(SESSION_INFO) ok : %s\n", session_info );
	}
	slm_free( session_info );
	session_info = NULL;

	// ���ʹ����ϣ��ǳ���ɣ��ǳ��󲻿�ִ���������صĲ�����
	if ( 0 != slm_handle )
	{
		ret = slm_logout(slm_handle);
		if (SS_OK == ret){
			printf("	slm_logout ok.\n");
		}else{
			printf("	slm_logout error : 0x%08X", ret);
		}
	}
	// ��ճ�ʼ��������Դ�����룩��ȫ�ֵ���һ�μ��ɡ�
	slm_cleanup();
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
			printf("	slm_keep_alive error = 0x%08X\n", ret);

			//�����ṩ���������뼰�����飬���������������ҵ����Ҫ���鴦��
			switch (ret)
			{
			/* ����ͨ�ô����� */
			case SS_ERROR_TIMEOUT:
				// ������ʱ
				// �����ԡ�
				break;
			case SS_ERROR_NET_ERROR:
				// �������
				// ��������Ƿ���ͨ�����ԡ�
				break;
			/* ����ɷ��س������� */
			case SS_ERROR_CLOUD_INVALID_TOKEN:
				// ���Ϸ���token
				// ���µ�¼���˺ţ�ˢ���˺���Ȩ��
				break;
			case SS_ERROR_CLOUD_SESSION_KICKED:
				// ��ͬIPʹ����ͬ�˺�ͬһ��ɣ����ڶ���IP��¼��ʹ�����ʱ��������߳���һ��IP�Ự��Ϣ��
				// �˺��һ��IP���������ؽӿڶ����ش˴����룬�޷�ʹ����ɹ��ܡ�
				// ���鴦��ʽ��������ʹ����ɣ������µ�¼��ɡ��������ʾ�û���ص�¼��
				break;
			case SS_ERROR_CLOUD_SESSION_TIMEOUT:
				// �Ự��ʱ
				// ��¼���˺ţ�ˢ���˺���Ȩ��
				break;
				// ��ɵ��ڴ����봦��������ɲ����á�����ʽ����� slm_login() ��¼ʧ�ܴ������֡�
			case SS_ERROR_CLOUD_LICENSE_NOT_EXISTS:         // ��ɲ�����
			case SS_ERROR_CLOUD_LICENSE_EXPIRED2:           // ����ѹ���
			case SS_ERROR_CLOUD_LICENSE_COUNTER_IS_ZERO:    // �����ʹ�ô���
			case SS_ERROR_CLOUD_LICENSE_RUNTIME_TIME_OUT:   // �����ʹ��ʱ��
				break;
			default:
				break;
			}
		}
		else
		{
			printf("	slm_keep_alive ...\n");
			Sleep(1000 * 20);
		}
	}
	_endthreadex(0);
	return 0;
}