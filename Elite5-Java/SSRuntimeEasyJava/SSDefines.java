package com.senseyun.openapi.SSRuntimeEasyJava;

public interface SSDefines {
    int LED_COLOR_BLUE = 0;         //������ɫ����ɫ
    int LED_COLOR_RED = 1;          //������ɫ����ɫ

    int LED_STATE_CLOSE = 0;        //���ƿ��ƣ��ر�
    int LED_STATE_OPEN = 1;         //���ƿ��ƣ���
    int LED_STATE_SHRINK = 2;       //���ƿ��ƣ���˸

    //============================================================
    //              �ص���Ϣ message ����
    //============================================================
    int SS_ANTI_INFORMATION		    = 0x0101;  //  ��Ϣ���ͣ���Ϣ��ʾ
    int SS_ANTI_WARNING			    = 0x0102;  //  ��Ϣ���ͣ�����
    int SS_ANTI_EXCEPTION		    = 0x0103;  //  ��Ϣ���ͣ��쳣
    int SS_ANTI_IDLE			    = 0x0104;  //  ��Ϣ���ͣ��ݱ���
    
    
    int SS_MSG_SERVICE_START        = 0x0200;  //  ��������
    int SS_MSG_SERVICE_STOP         = 0x0201;  //  ����ֹͣ
    int SS_MSG_LOCK_AVAILABLE       = 0x0202;  //  �����ã���������SS����ʱ���ѳ�ʼ����ɣ��ص�����wparam ��������
    int SS_MSG_LOCK_UNAVAILABLE     = 0x0203;  //  ����Ч�����Ѱγ����ص�����wparam ��������
    
    
    //============================================================
    //              �ص���Ϣ wparam ����
    //============================================================
    int SS_ANTI_PATCH_INJECT		= 0x0201;  //  ����ע��
    int SS_ANTI_MODULE_INVALID		= 0x0202;  //  ģ����ʧ��
    int SS_ANTI_ATTACH_FOUND		= 0x0203;  //  ���ֱ�����������
    int SS_ANTI_THREAD_INVALID		= 0x0204;  //  ��Ч���߳�
    int SS_ANTI_THREAD_ERROR		= 0x0205;  //  �̼߳��ʧ��
    int SS_ANTI_CRC_ERROR			= 0x0206;  //  CRC����ʧ��
    int SS_ANTI_DEBUGGER_FOUND		= 0x0207;  //  ���ֵ�����
    
    
    int SLM_FIXTIME_RAND_LENGTH     = 0x08;    //  ʱ��У׼��������ӳ���
    
    int SLM_CALLBACK_VERSION02      = 0x02;    //  SS_CALL_BACK�İ汾 ��֧�ֿ�����API����İ汾��
    
    
    int  SLM_MEM_MAX_SIZE           = 2048;  //  �ڴ��й�����ֽ�
    int SLM_MAX_INPUT_SIZE          = 1758;  //  ����ִ�У�������뻺������С���ֽڣ�
    int SLM_MAX_OUTPUT_SIZE         = 1758;  //  ����ִ�У���������������С���ֽڣ�
    int SLM_MAX_USER_CRYPT_SIZE     = 1520;  //  �ӽ�����󻺳�����С���ֽڣ�
                                                    
    int SLM_MAX_USER_DATA_SIZE      = 2048;  //  �û���������󳤶ȣ��ֽڣ�
    int SLM_MAX_WRITE_SIZE          = 1904;  //  �û�������д����󳤶ȣ��ֽڣ�

    String SLM_VERIFY_DEVICE_PREFIX   = "SENSELOCK";  //  ����ǩ��������ǰ׺

    int SLM_VERIFY_DATA_SIZE        = 41 ;  //  ����ǩ�������ݴ�С����slm_verify_device
    int SLM_LOCK_SN_LENGTH          = 16 ;  //  ���ŵĳ���
    int SLM_DEVELOPER_ID_SIZE       = 8  ;  //  ������ID����
    int SLM_MAX_SERVER_NAME         = 32 ;  //  ������������󳤶�
    int SLM_MAX_ACCESS_TOKEN_LENGTH = 64 ;  //  �����û�token��󳤶�
    int SLM_MAX_CLOUD_SERVER_LENGTH = 100;  //  ������������ַ��󳤶�
    int SLM_SNIPPET_SEED_LENGTH     = 32 ;  //  ��Ƭ�������ӳ���
    int SLM_DEV_PASSWORD_LENGTH     = 16 ;  //  ���������볤��

    int SLM_CLOUD_MAX_USER_GUID_SIZE 	       = 128;   //  ����û�GUID���� 
                                                               
    int SLM_FILE_TYPE_BINARY                   = 0;     //  �ļ����ͣ������ļ�
    int SLM_FILE_TYPE_EXECUTIVE                = 1;     //  �ļ����ͣ���ִ���ļ��ļ�
    int SLM_FILE_TYPE_KEY                      = 2;     //  �ļ����ͣ���Կ�ļ� 
                                                              
    int SLM_FILE_PRIVILEGE_FLAG_READ           = 0x01;  //  �ɶ�
    int SLM_FILE_PRIVILEGE_FLAG_WRITE          = 0x02;  //  ��д
    int SLM_FILE_PRIVILEGE_FLAG_USE            = 0x04;  //  ����Կ�ļ�����ʹ��
    int SLM_FILE_PRIVILEGE_FLAG_UPDATE         = 0x08;  //  ��Զ������

    int SLM_FILE_PRIVILEGE_FLAG_ENTRY_READ     = 0x10;  //  �ɶ�
    int SLM_FILE_PRIVILEGE_FLAG_ENTRY_WRITE    = 0x20;  //  ��д
    int SLM_FILE_PRIVILEGE_FLAG_ENTRY_USE      = 0x40;  //  ����Կ�ļ�����ʹ��
    int SLM_FILE_PRIVILEGE_FLAG_ENTRY_UPDATE   = 0x80;  //  ��Զ������

    int SLM_LOGIN_MODE_AUTO             = 0x0000;  //  �Զ���¼ģʽ
    int SLM_LOGIN_MODE_LOCAL            = 0x0001;  //  ָ����¼����USE��
    int SLM_LOGIN_MODE_REMOTE           = 0x0002;  //  ָ����¼Զ��USE�� 
    int SLM_LOGIN_MODE_CLOUD            = 0x0004;  //  ָ����¼����  
                                                        
    int SLM_LOGIN_FLAG_FIND_ALL         = 0x0001;  //  �������е�����������ֶ����������򲻵�¼���ṩѡ�񣬷����ҵ�������������ֱ�ӵ�¼
    int SLM_LOGIN_FLAG_VERSION          = 0x0004;  //  ָ����ɰ汾
    int SLM_LOGIN_FLAG_LOCKSN           = 0x0008;  //  ָ�����ţ�USB��
    int SLM_LOGIN_FLAG_SERVER           = 0x0010;  //  ָ��������
    int SLM_LOGIN_FLAG_SNIPPET          = 0x0020;  //  ָ����Ƭ����

    int LANGUAGE_CHINESE_ASCII              = 0x0001;  //  ����ID����������
    int LANGUAGE_ENGLISH_ASCII              = 0x0002;  //  ����ID��Ӣ��
    int LANGUAGE_TRADITIONAL_CHINESE_ASCII  = 0x0003;  //  ����ID����������

    int SLM_INIT_FLAG_NOTIFY          = 0x01;  //  ��ʾ���յ�SS����Ϣ֪ͨ
}
