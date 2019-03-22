
/* ********************************************************
Copyright (C )2016 Beijing Senseshield Technology Co.,Ltd
Author : senseshield 2017-6
Description : 演示keepalive的基本使用方法。

History :
[作者 日期 修改内容]
********************************************************* */


#include <stdio.h>
#include <memory.h>
#include <windows.h>
#include <process.h>

#include "common.h"
#include "ss_lm_runtime.h"
#include "ss_user_login.h"

//对于不是频繁直接或间接调用runtime接口进行操作的客户程序
//可通过keepalive接口(循环调用)保持与服务端的通信，以表示此客户端还在存活

//此示例演示使用keepalive保持云锁登录状态,需要使用者填写: 开发商ID、云账户的用户名和密码、登录到云锁的许可ID
//开发商ID查看:登录开发者平台(https://developer.senseyun.com)，查看开发商信息中的开发商编号。

//填写示例
//#define DEVELOPER_ID		"0300000000000006"
//#define CLOUD_USERNAME	"yaowb@sense.com.cn"
//#define CLOUD_PASSWORD	"sense12345"
//#define CLOUD_LICENSE_ID	198

#define DEVELOPER_ID		""
#define CLOUD_USERNAME		""
#define CLOUD_PASSWORD		""
#define CLOUD_LICENSE_ID	1


//演示步骤如下

//1 登录云锁
//2 使用runtime接口访问云锁，并设置超时时间(slm_login接口参数)为30秒
//3 创建keepalive线程，循环30秒调用一次
//4 主线程(云锁登录线程)Sleep 35 秒
//5 调用runtime接口slm_get_info获取一下SESSION_INFO信息进行测试
//6 登出云锁

// 结果	如上操作，slm_get_info成功.
//		如果不进行步骤3，即不创建keepalive线程，则slm_get_info失败


//云锁guid
SS_CHAR user_guid_cloud[SS_CLOUD_MAX_USER_GUID_SIZE] = {0};

SS_UINT32 logincloud();		// 登录云锁
SS_UINT32 logoutcloud();	// 登出云锁
SS_UINT32 keep_alive_demo(SS_BOOL bUseKeep);				//demo 主函数
unsigned int _stdcall threadproc_keep_alive(void* pParam);	//keep alive 线程函数


int main()
{
	SS_UINT32 ret = 0;
	printf("\n\n");

	if( !strlen(DEVELOPER_ID) || !strlen(CLOUD_USERNAME) || !strlen(CLOUD_PASSWORD) )
	{
		printf("\n此DEMO需要使用者将 开发商ID、云锁账号密码、登录到云锁的证书ID 填写一下!\n\n");
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

	// 设置API密码，示例统一使用DEMO开发商密码，可访问DEMO用户锁。
	// 访问正式开发商用户锁必须修改此密码，开发商密码获取方法见文档说明。
	// 如果API密码与引用库不匹配，返回错误。
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
	// 2.登录许可(演示keepalive维持云锁登录状态)
	// 初始化结构体大小（必须）
	login_param.size = sizeof(ST_LOGIN_PARAM);

	// 登录访问许可ID
	login_param.license_id = CLOUD_LICENSE_ID;

	// 设置云锁类型
	login_param.login_mode = SLM_LOGIN_MODE_CLOUD;

	// 设置许可会话超时时间（单位：秒），设定时间内如果未进行任何许可相关操作，超时后访问会返回许可过期错误，最大不可超过12小时
	// 必须需要重新登录才能继续使用许可。
	// 许可会话，许可登录到许可登出期间表示一次会话。
	// 许可会话超时，许可登录后没有进行任何与许可相关的操作，没有操作时间超过设置超时时间被视为程序当前会话失效，需要重新登录许可。
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
	//35秒不去操作runtime接口，大于登录时候设置的30秒，之后再进行runtime接口的调用是否成功，演示slm_keep_alive的作用
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

	// 许可使用完毕，登出许可，登出后不可执行与许可相关的操作。
	if ( 0 != slm_handle )
	{
		ret = slm_logout(slm_handle);
		if (SS_OK == ret){
			printf("	slm_logout ok.\n");
		}else{
			printf("	slm_logout error : 0x%08X", ret);
		}
	}
	// 清空初始化申请资源（必须），全局调用一次即可。
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

			//以下提供常见错误码及处理建议，其它错误码请根据业务需要酌情处理。
			switch (ret)
			{
			/* 网络通用错误码 */
			case SS_ERROR_TIMEOUT:
				// 操作超时
				// 请重试。
				break;
			case SS_ERROR_NET_ERROR:
				// 网络错误
				// 检查网络是否连通后重试。
				break;
			/* 云许可返回场景错误 */
			case SS_ERROR_CLOUD_INVALID_TOKEN:
				// 不合法的token
				// 重新登录云账号，刷新账号授权。
				break;
			case SS_ERROR_CLOUD_SESSION_KICKED:
				// 不同IP使用相同账号同一许可，当第二个IP登录并使用许可时，服务会踢出第一个IP会话信息，
				// 此后第一个IP所有许可相关接口都返回此错误码，无法使用许可功能。
				// 建议处理方式：若继续使用许可，需重新登录许可。否则可提示用户异地登录。
				break;
			case SS_ERROR_CLOUD_SESSION_TIMEOUT:
				// 会话超时
				// 登录云账号，刷新账号授权。
				break;
				// 许可到期错误码处理都属于许可不可用。处理方式详情见 slm_login() 登录失败错误处理部分。
			case SS_ERROR_CLOUD_LICENSE_NOT_EXISTS:         // 许可不存在
			case SS_ERROR_CLOUD_LICENSE_EXPIRED2:           // 许可已过期
			case SS_ERROR_CLOUD_LICENSE_COUNTER_IS_ZERO:    // 许可无使用次数
			case SS_ERROR_CLOUD_LICENSE_RUNTIME_TIME_OUT:   // 许可无使用时间
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