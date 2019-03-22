/* ********************************************************
Copyright (C )2016 Beijing Senseshield Technology Co.,Ltd
Author : senseshield 2017-6
Description : 演示调用RuntimeAPI的基本使用方法。

History :
[作者 日期 修改内容]
********************************************************* */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <process.h>
#include "common.h"
#include "ss_lm_runtime.h"  //runtime 库使用注意事项详情见"readme.txt"

//keepalive线程函数
unsigned int _stdcall threadproc_keep_alive(void* pParam);

//初始化slm_init函数，设置的回调函数
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
	SS_BYTE mm_buffer[SLM_MEM_MAX_SIZE] = {"testmm/测试内存托管"};

	SS_BYTE sign_data[32]				= {"test_data1234567890"};
	SS_BYTE signature[2048]				= {0};
	SS_CHAR *sign_info					= NULL;
	SS_UINT32 signature_length			= 0;

	// 初始化接口调用，参数初始化
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
	else
	{
		printf("slm_login ok\n");
	}
	
	//创建keepalive线程
	_beginthreadex(NULL, 0, threadproc_keep_alive, &slm_handle, 0, NULL);	

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

	// 许可加密 & 许可解密
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
	
	// 获得许可的数据区大小和读取(可指定PUB、RAW和ROM)
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
	
	// SS内存托管内存申请、写入、读取、释放
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
	//  许可登出 & 反初始化函数(slm_cleanup)，与slm_init对应
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
服务消息处理回调函数

能够接收到硬件锁拔插消息，服务启停等消息。
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
    case SS_ANTI_INFORMATION:   // 信息提示
        sprintf(szmsg, "SS_ANTI_INFORMATION is  0x%08X wparam is %p", message, wparam);
        break;
    case SS_ANTI_WARNING:       // 警告
        // 反调试检查。一旦发现如下消息，建议立即停止程序正常业务，防止程序被黑客调试。
        switch ((unsigned int)(wparam))
        {
        case SS_ANTI_PATCH_INJECT: 
            sprintf(szmsg, "%s, 信息类型=0x%08x, 具体错误码=0x%08x","注入", message, wparam);
            break;
        case SS_ANTI_MODULE_INVALID:
            sprintf(szmsg, "%s, 信息类型=0x%08x, 具体错误码=0x%08x","非法模块DLL", message, wparam);
            break;
        case SS_ANTI_ATTACH_FOUND:
            sprintf(szmsg, "%s, 信息类型=0x%08x, 具体错误码=0x%08x","附加调试", message, wparam);
            break;
        case SS_ANTI_THREAD_INVALID:
            sprintf(szmsg, "%s, 信息类型=0x%08x, 具体错误码=0x%08x","线程非法", message, wparam);
            break;
        case SS_ANTI_THREAD_ERROR:
            sprintf(szmsg, "%s, 信息类型=0x%08x, 具体错误码=0x%08x","线程错误", message, wparam);
            break;
        case SS_ANTI_CRC_ERROR:
            sprintf(szmsg, "%s, 信息类型=0x%08x, 具体错误码=0x%08x","内存模块CRC校验", message, wparam);
            break;
        case SS_ANTI_DEBUGGER_FOUND:
            sprintf(szmsg, "%s, 信息类型=0x%08x, 具体错误码=0x%08x","发现调试器", message, wparam);
            break;
        default:
            sprintf(szmsg, "%s, 信息类型=0x%08x, 具体错误码=0x%08x","其它未知错误", message, wparam);
            break;
        }
        break;
    case SS_ANTI_EXCEPTION:         // 异常
        sprintf(szmsg, "SS_ANTI_EXCEPTION is  0x%08X wparam is %p", message, wparam);
        break;
    case SS_ANTI_IDLE:              // 暂保留
        sprintf(szmsg, "SS_ANTI_IDLE is  0x%08X wparam is %p", message, wparam);
        break;
    case SS_MSG_SERVICE_START:      // 服务启动
        sprintf(szmsg, "SS_MSG_SERVICE_START is  0x%08X wparam is %p", message, wparam);
        break;
    case SS_MSG_SERVICE_STOP:       // 服务停止
        sprintf(szmsg, "SS_MSG_SERVICE_STOP is  0x%08X wparam is %p", message, wparam);
        break;
    case SS_MSG_LOCK_AVAILABLE:     // 锁可用（插入锁或SS启动时锁已初始化完成），wparam 代表锁号
        // 锁插入消息，可以根据锁号查询锁内许可信息，实现自动登录软件等功能。

        // 将二进制记录的锁号转换成十六进制字符串
        memcpy(lock_sn, wparam, DEVICE_SN_LENGTH);
        bytes_to_hexstr((unsigned char*)lock_sn, DEVICE_SN_LENGTH, szlock_sn);

        sprintf(szmsg, "time is %08u, SS_MSG_LOCK_AVAILABLE is 0x%08X wparam is locksn -memory address %p, lock-sn is %s", 
            GetTickCount(), message, wparam, szlock_sn);
        break;
    case SS_MSG_LOCK_UNAVAILABLE:   // 锁无效（锁已拔出），wparam 代表锁号
        // 锁拔出消息，对于只使用锁的应用程序，一旦加密锁拔出软件将无法继续使用，建议发现此消息提示用户保存数据，程序功能锁定等操作。

        // 将二进制记录的锁号转换成十六进制字符串
        memcpy(lock_sn, wparam, DEVICE_SN_LENGTH);
        bytes_to_hexstr((unsigned char*)lock_sn, DEVICE_SN_LENGTH, szlock_sn);

        sprintf(szmsg, "time is %08u SS_MSG_LOCK_UNAVAILABLE is 0x%08X wparam is locksn -memory address %p, lock-sn is %s", 
            GetTickCount(), message, wparam, szlock_sn);
        break;
    }    
    // 输出格式化后的消息内容
    printf("%s\n", szmsg);
    return ret;
}