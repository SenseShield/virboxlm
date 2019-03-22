#ifndef _COMMON_H_
#define _COMMON_H_

 
#include <memory.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 开发商密码(slm_init参数使用)，置为全局，供客户修改。
// 示例固定使用DEMO开发商密码，仅能访问DEMO用户锁。
// 访问非DEMO开发商用户锁必须修改密码。
extern unsigned char api_password[16];

// 开发锁登录PIN索引（固定0）
#define PIN_DEFAULT_INDEX   0
// 开发锁登录PIN码
// 开发锁出厂默认不启用PIN码功能或已启用（初始密码需从深思开发者平台获取）
// 开发锁PIN码需使用开发商工具的PIN设置功能进行修改
// 开发者运行测试程序前需将以下参数改为设置后的PIN码，否则在开发锁启用PIN的情况下调用接口返回失败
#define PIN_DEFAULT_PWD     "12345678"

#ifdef __cplusplus
extern "C" {
#endif

	int	bytes_to_hexstr( const void* data, int count,  char* hex );
	int hexstr_to_bytes( const char* hex,  int count,  unsigned char* data);

    int read_file(const char *filename, void *buffer, unsigned int buffer_max_length, unsigned int *file_size);
    int write_file(const char *filename, void *data, int data_length);

	int read_file_text(const char *filename, void *buffer, unsigned int buffer_max_length, unsigned int *file_size);

	
#ifdef __cplusplus
}
#endif

#endif// _COMMON_H_