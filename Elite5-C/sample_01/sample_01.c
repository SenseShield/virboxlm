
/* ********************************************************
Copyright (C )2016 Beijing Senseshield Technology Co.,Ltd
Author : senseshield 2017-6
Description : 演示加密锁许可的基本使用方法。

History :
[作者 日期 修改内容]
********************************************************* */


#include <stdio.h>
#include <memory.h>

#include "common.h"
#include "ss_lm_runtime.h"  //runtime 库使用注意事项详情见"readme.txt"

/*
* 解决提示： VS2015 及以上版本编译失败，提示无法解析的外部符号 __iob_func()。
*            由于编译器版本引起的问题。
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

    // 1.初始化（必须），全局调用一次即可，可重复初始化，但以第一次初始化为准。
    // 服务消息回调函数版本号。注意：固定参数，请勿随意变更修改。（必须）
    init_param.version = SLM_CALLBACK_VERSION02;

    // 服务消息接收（回调）函数，此处不设置，即不接收服务消息。
    init_param.pfn = NULL;

    // 设置应用程序与加密锁服务通讯超时（单位：毫秒）。如果填0，则使用默认超时时间7秒，建议使用默认超时。
    // 通讯超时指API执行超时，当调用API未在超时时间内执行完成，返回超时。
    init_param.timeout = 0;

    // 设置API密码，示例统一使用DEMO开发商密码，可访问DEMO用户锁。
    // 访问正式开发商用户锁必须修改此密码，开发商密码获取方法见文档说明。
    // 如果API密码与引用库不匹配，返回错误。
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
    // 2.登录许可
    // 初始化结构体大小（必须）
    login_param.size = sizeof(ST_LOGIN_PARAM);

    // 登录访问许可ID
    login_param.license_id = 1;

    // 设置自动类型。可根据当前计算机是否插锁、检测到网络锁或登录云账号等情况逐一尝试登录本地锁、
    // 网络锁、云锁、软锁许可，只要其中有一种符合要求，即可登录成功，否则返回许可登录失败错误码。
    // 指定登录类型后只能访问特定，登录类型可使用 | （或）进行组合。类型详情见文档说明。
    login_param.login_mode = SLM_LOGIN_MODE_AUTO;

    // 设置许可会话超时时间（单位：秒），设定时间内如果未进行任何许可相关操作，超时后访问会返回许可过期错误，最大不可超过12小时
    // 必须需要重新登录才能继续使用许可。
    // 许可会话，许可登录到许可登出期间表示一次会话。
    // 许可会话超时，许可登录后没有进行任何与许可相关的操作，没有操作时间超过设置超时时间被视为程序当前会话失效，需要重新登录许可。
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
    /*  此处编写业务代码，进行许可操作或访问许可信息               		*/
    
    // 3.获取许可信息（LICENSE_INFO），指定返回JSON格式数据。
    // 返回信息格式可设置，详情参照 INFO_FORMAT_TYPE 类型说明
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

    // 需要调用者释放内存，使用完毕不释放会产生内存泄露。
    slm_free(license_info_json);
    license_info_json = NULL;


    // 维持许可会话心跳。定时发送心跳包请求，维持当前会话可用，不会超时。
    // 如果会话超时（slm_login 时设置超时）范围内没有调用任何与许可相关的接口（包括slm_keep_alive），会造成会话超时，
    // 超时后需要重新登录才能继续使用许可。
    // 注意：许可使用过程中，建议使用独立线程自定义间隔调用slm_keep_alive（建议：60秒），如果在维持心跳过程中发现错误，需要及时处理。
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
    // 4.许可使用完毕，登出许可，登出后不可执行与许可相关的操作。
    // 如仍需使用许可功能，请重新登录许可。
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
    // 5.清空初始化申请资源（必须），全局调用一次即可。
    slm_cleanup();

    printf("\npress any key exit process.\n");
    getchar();

    return ret;
}