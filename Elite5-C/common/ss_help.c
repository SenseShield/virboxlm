
#include "ss_help.h"


//��ʼ��slm_runtime�⺯��
int initSlm()
{	
	int ret = 0;
	ST_INIT_PARAM initParam = { 0 };

	initParam.flag = SLM_INIT_FLAG_NOTIFY;
	initParam.version = SLM_CALLBACK_VERSION02;
	initParam.timeout = 600;
	initParam.pfn = NULL;
	memcpy( initParam.password, api_password, sizeof(api_password) );

	ret = slm_init(&initParam);
	if (ret != SS_OK)
	{
		printf("slm_init error :0x%08X\n", ret);
	}
	return ret;
}
//����ʼ������(slm_cleanup)����slm_init��Ӧ
void cleanSlm()
{
	slm_cleanup();
}
//��ȡ�豸���к�
int getUserDevSN(char* deviceSN)
{
	int ret						= 0;
	SS_UINT32 dev_num			= 0;
	cJSON *pjson_Array			= NULL;
	cJSON *pjson_Item			= NULL;
	cJSON *pjson_Devdescs		= NULL;
	SS_CHAR *pchar_Devdescs		= NULL;
	SS_UINT32 deviceSNLen		= 0;

	ret = initSlm();
	if (ret != SS_OK)
	{
		printf("slm_init error :0x%08X\n", ret);
		return 1;
	}
	ret = slm_enum_device(&pchar_Devdescs);
	if (ret == SS_OK)
	{
		//�����ص��豸����������JSON
		pjson_Devdescs = cJSON_Parse(pchar_Devdescs);
		if (!pjson_Devdescs)
		{
			printf("cJSON_Parse error\n");
			ret = 1;
			goto CLEAR;
		}
		//��ȡ�豸�����ṹ���� ���豸����
		dev_num	= cJSON_GetArraySize( pjson_Devdescs );
		printf("device count : %d\n", dev_num );
		if ( dev_num == 0)
		{
			printf("find no device\n");
			ret = 1;
			goto CLEAR;
		}
		//��ȡ�豸�����ṹ�е�һ���ṹ����ȡ��sn�ֶ�
		pjson_Array = cJSON_GetArrayItem( pjson_Devdescs, 0 );
		pjson_Item = cJSON_GetObjectItem( pjson_Array, "sn" );

		strcpy(deviceSN ,pjson_Item->valuestring);

		printf("\nget sn : %s\n", pjson_Item->valuestring);
	}
	else
	{
		printf("slm_enum_device error :0x%08X\n", ret);
		ret = 1;
	}
CLEAR:
	if(pjson_Devdescs)
		cJSON_Delete(pjson_Devdescs);
	if(pchar_Devdescs)
		slm_free(pchar_Devdescs);

	cleanSlm();
	return ret;
}
//��ȡ��ǰ���������豸֤��
int getUserDevCert(unsigned char* pCert, unsigned int* pCertLen)
{
    int ret = 0;
    SS_CHAR pCertName[256]						= {0};
    SS_CHAR deviceSn[SLM_LOCK_SN_LENGTH*2+1]	= {0};
    ST_LOGIN_PARAM loginParam					= {0};
    SLM_HANDLE_INDEX handle						= 0;

    ret = initSlm();
    if (ret != SS_OK)
    {
        printf("slm_init error :0x%08X\n", ret);
        return 1;
    }
    //��½0�������Ȩ
    loginParam.size = sizeof(ST_LOGIN_PARAM);
    loginParam.license_id = 0;
    loginParam.login_mode = SLM_LOGIN_MODE_LOCAL;
    loginParam.timeout = 600;

    ret = slm_login(&loginParam, STRUCT, &handle, NULL);
    if (ret != SS_OK)
    {
        printf("slm_login error : 0x%08X\n", ret);
        cleanSlm();
        return 1;
    }
    //��ȡ�豸֤��	
    ret = slm_get_device_cert(handle, pCert, CERT_SIZE, pCertLen);
    if (ret != SS_OK)
    {
        printf("slm_get_device_cert error : 0x%08X\n", ret);
        cleanSlm();
        return ret;
    }
    cleanSlm();
    return ret;
}
int getUserDevP7b(unsigned char *pCertP7b, unsigned int *pCertLen)
{
	int ret = 0;
	cert_st pcerts_st[3] = {0};
    SS_CHAR pCertName[256]						= {0};
    SS_CHAR deviceSn[SLM_LOCK_SN_LENGTH*2+1]	= {0};
    ST_LOGIN_PARAM loginParam					= {0};
    SLM_HANDLE_INDEX handle						= 0;

    ret = initSlm();
    if (ret != SS_OK)
    {
        printf("slm_init error :0x%08X\n", ret);
        return 1;
    }
    //��½0�������Ȩ
    loginParam.size = sizeof(ST_LOGIN_PARAM);
    loginParam.license_id = 0;
    loginParam.login_mode = SLM_LOGIN_MODE_LOCAL;
    loginParam.timeout = 600;

    ret = slm_login(&loginParam, STRUCT, &handle, NULL);
    if (ret != SS_OK)
    {
        printf("slm_login error : 0x%08X\n", ret);
        cleanSlm();
        return 1;
    }

    //��ȡ��֤��
    pcerts_st[0].certlen = x509_CERT_SIZE;
    ret = slm_get_cert(handle, CERT_TYPE_ROOT_CA, pcerts_st[0].certbuf, pcerts_st[0].certlen, &pcerts_st[0].certlen);
    if (ret != SS_OK)
    {
        printf("slm_get_cert(CERT_TYPE_ROOT_CA) error : 0x%08X\n", ret);
        cleanSlm();
        return ret;
    }

    //��ȡ�豸��֤��
    pcerts_st[1].certlen = x509_CERT_SIZE;
    ret = slm_get_cert(handle, CERT_TYPE_DEVICE_CA, pcerts_st[1].certbuf, pcerts_st[1].certlen, &pcerts_st[1].certlen);
    if (ret != SS_OK)
    {
        printf("slm_get_cert(CERT_TYPE_DEVICE_CA) error : 0x%08X\n", ret);
        cleanSlm();
        return ret;
    }

    //��ȡ�豸֤��
    pcerts_st[2].certlen = x509_CERT_SIZE;
    ret = slm_get_cert(handle, CERT_TYPE_DEVICE_CERT, pcerts_st[2].certbuf, pcerts_st[2].certlen, &pcerts_st[2].certlen);
    if (ret != SS_OK)
    {
        printf("slm_get_cert(CERT_TYPE_DEVICE_CERT) error : 0x%08X\n", ret);
        cleanSlm();
        return ret;
    }
    cleanSlm();

	//����p7b֤��
	ret = ss_make_p7b(pcerts_st, sizeof(pcerts_st) / sizeof(cert_st), pCertP7b, pCertLen);
	if (ret)
	{
		printf("ss_make_p7b error :0x%08X\n", ret);
		return 1;
	}
	return 0;
}

static int masterEnablePIN(MASTER_HANDLE hD2CHandle)
{
#define MASTER_GET_INFO_TYPE_DEFAULT 1
    int ret = SS_OK;
    char *info_str_json = NULL;

    cJSON *root = NULL;
    cJSON *node_support_pin = NULL;
    cJSON *node_enable_pin = NULL;

    ret = master_get_info(hD2CHandle, MASTER_GET_INFO_TYPE_DEFAULT, &info_str_json);
    if (ret != SS_OK)
    {
        return 0;
    }

    root = cJSON_Parse(info_str_json);
    if (root == NULL)
    {
        ret = 0;
        goto END;
    }

    node_support_pin = cJSON_GetObjectItem(root, "support_pin");
    node_enable_pin = cJSON_GetObjectItem(root, "enable_pin");

    if (node_support_pin != NULL && node_enable_pin != NULL)
    {
        ret = node_support_pin->type == cJSON_True && node_enable_pin->type == cJSON_True;
    }
    else
    {
        ret = 0;
    }

END:
    if (root)
    {
        cJSON_Delete(root);
    }

    if (info_str_json)
    {
        master_heap_free(info_str_json);
    }

    return ret;
}

int masterPINVerify(MASTER_HANDLE hD2CHandle, char *pPIN, int nPinLen)
{
	// ����������PIN���飬������PIN��ʱִ��PIN����֤���������򷵻سɹ���
	// �ɰ汾�����������������̼��汾����3.1.20����֧��PIN��
    if (masterEnablePIN(hD2CHandle))
    {
        return master_pin_verify(hD2CHandle, PIN_DEFAULT_INDEX, (SS_BYTE*)PIN_DEFAULT_PWD, nPinLen);
    }

    return SS_OK;
}

int masterPINDeauth(MASTER_HANDLE hD2CHandle)
{
    master_pin_deauth(hD2CHandle);
    return SS_OK;
}
