
/* ********************************************************
Copyright (C )2016 Beijing Senseshield Technology Co.,Ltd
Author : senseshield 2017-6
Description : ��ʾ��������ɵĻ���ʹ�÷�����

History :
[���� ���� �޸�����]
********************************************************* */


#include <stdio.h>
#include <memory.h>

#include "common.h"
#include "ss_lm_runtime.h"  //runtime ��ʹ��ע�����������"readme.txt"

/*
* �����ʾ�� VS2015 �����ϰ汾����ʧ�ܣ���ʾ�޷��������ⲿ���� __iob_func()��
*            ���ڱ������汾��������⡣
*/
#if _MSC_VER>=1900
#include "stdio.h" 
_ACRTIMP_ALT FILE* __cdecl __acrt_iob_func(unsigned);
#ifdef __cplusplus 
extern "C"
#endif


FILE* __cdecl __iob_func(unsigned i) {
	return __acrt_iob_func(i);
}
#endif /* _MSC_VER>=1900 */

int main()
{
    SS_UINT32 ret = SS_OK;
    ST_INIT_PARAM init_param	= {0};
    ST_LOGIN_PARAM login_param	= {0};
	SLM_HANDLE_INDEX slm_handle = 0;
	char *license_info_json		= NULL;

    // 1.��ʼ�������룩��ȫ�ֵ���һ�μ��ɣ����ظ���ʼ�������Ե�һ�γ�ʼ��Ϊ׼��
    // ������Ϣ�ص������汾�š�ע�⣺�̶������������������޸ġ������룩
    init_param.version = SLM_CALLBACK_VERSION02;

    // ������Ϣ���գ��ص����������˴������ã��������շ�����Ϣ��
    init_param.pfn = NULL;

    // ����Ӧ�ó��������������ͨѶ��ʱ����λ�����룩�������0����ʹ��Ĭ�ϳ�ʱʱ��7�룬����ʹ��Ĭ�ϳ�ʱ��
    // ͨѶ��ʱָAPIִ�г�ʱ��������APIδ�ڳ�ʱʱ����ִ����ɣ����س�ʱ��
    init_param.timeout = 0;

    // ����API���룬ʾ��ͳһʹ��DEMO���������룬�ɷ���DEMO�û�����
    // ������ʽ�������û��������޸Ĵ����룬�����������ȡ�������ĵ�˵����
    // ���API���������ÿⲻƥ�䣬���ش���
    memcpy(init_param.password, api_password, sizeof(api_password));
    ret = slm_init(&(init_param));
	if(SS_OK != ret)
	{
		printf("slm_init error : 0x%08X\n", ret);
		goto CLEAR;
	}
	else
	{
		printf("slm_init ok\n");
	}
    // 2.��¼���
    // ��ʼ���ṹ���С�����룩
    login_param.size = sizeof(ST_LOGIN_PARAM);

    // ��¼�������ID
    login_param.license_id = 1;

    // �����Զ����͡��ɸ��ݵ�ǰ������Ƿ��������⵽���������¼���˺ŵ������һ���Ե�¼��������
    // ��������������������ɣ�ֻҪ������һ�ַ���Ҫ�󣬼��ɵ�¼�ɹ������򷵻���ɵ�¼ʧ�ܴ����롣
    // ָ����¼���ͺ�ֻ�ܷ����ض�����¼���Ϳ�ʹ�� | ���򣩽�����ϡ�����������ĵ�˵����
    login_param.login_mode = SLM_LOGIN_MODE_AUTO;

    // ������ɻỰ��ʱʱ�䣨��λ���룩���趨ʱ�������δ�����κ������ز�������ʱ����ʻ᷵����ɹ��ڴ�����󲻿ɳ���12Сʱ
    // ������Ҫ���µ�¼���ܼ���ʹ����ɡ�
    // ��ɻỰ����ɵ�¼����ɵǳ��ڼ��ʾһ�λỰ��
    // ��ɻỰ��ʱ����ɵ�¼��û�н����κ��������صĲ�����û�в���ʱ�䳬�����ó�ʱʱ�䱻��Ϊ����ǰ�ỰʧЧ����Ҫ���µ�¼��ɡ�
    login_param.timeout = 86400;

    ret = slm_login(&login_param, STRUCT, &(slm_handle), NULL);
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
    /*  �˴���дҵ����룬������ɲ�������������Ϣ               		*/
    
    // 3.��ȡ�����Ϣ��LICENSE_INFO����ָ������JSON��ʽ���ݡ�
    // ������Ϣ��ʽ�����ã�������� INFO_FORMAT_TYPE ����˵��
    ret = slm_get_info(slm_handle, LICENSE_INFO, JSON, &license_info_json);
	if(SS_OK != ret)
	{
		printf("slm_get_info[LICENSE_INFO] error : 0x%08X\n", ret);
		goto CLEAR;
	}
	else
	{
		printf("[LICENSE_INFO]:\n%s\n", license_info_json);
	}

    // ��Ҫ�������ͷ��ڴ棬ʹ����ϲ��ͷŻ�����ڴ�й¶��
    slm_free(license_info_json);
    license_info_json = NULL;


    // ά����ɻỰ��������ʱ��������������ά�ֵ�ǰ�Ự���ã����ᳬʱ��
    // ����Ự��ʱ��slm_login ʱ���ó�ʱ����Χ��û�е����κ��������صĽӿڣ�����slm_keep_alive��������ɻỰ��ʱ��
    // ��ʱ����Ҫ���µ�¼���ܼ���ʹ����ɡ�
    // ע�⣺���ʹ�ù����У�����ʹ�ö����߳��Զ���������slm_keep_alive�����飺60�룩�������ά�����������з��ִ�����Ҫ��ʱ����
    ret = slm_keep_alive(slm_handle); 
	if(SS_OK != ret)
	{
		printf("slm_keep_alive error : 0x%08X\n", ret);
	}
	else
	{
		printf("slm_keep_alive ok\n");
	}

    /************************************************************************/

CLEAR:
    // 4.���ʹ����ϣ��ǳ���ɣ��ǳ��󲻿�ִ���������صĲ�����
    // ������ʹ����ɹ��ܣ������µ�¼��ɡ�
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
    // 5.��ճ�ʼ��������Դ�����룩��ȫ�ֵ���һ�μ��ɡ�
    slm_cleanup();

    printf("\npress any key exit process.\n");
    getchar();

    return ret;
}