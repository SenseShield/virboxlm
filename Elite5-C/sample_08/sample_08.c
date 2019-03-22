/* ********************************************************
Copyright (C )2016 Beijing Senseshield Technology Co.,Ltd
Author : senseshield 2017-6
Description : 演示升级加密锁数据的使用方法。

History :
[作者 日期 修改内容]
********************************************************* */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "ss_error.h"
#include "common.h"
#include "cJSON/cJSON.h"

#include "ss_lm_runtime.h"
#include "ss_lm_update.h"

//将需要升级的加密锁数据(d2c文件)全路径填写到下面的宏定义 UPDATE_D2C_FILENAME
//d2c文件可以使用sample_07示例生成

#define UPDATE_D2C_FILENAME ""

//升级d2c到加密锁内
int update_d2c(const char *d2c_filename);

int main()
{
	if(!strlen(UPDATE_D2C_FILENAME))
	{
		printf("\n此DEMO需要将准备升级的加密锁数据(d2c文件)全路径填写一下!\n\n");
		printf("\npress any key exit process.\n");
		getchar();
		return 0;
	}

	update_d2c(UPDATE_D2C_FILENAME);
	printf("\npress any key exit process.\n");
	getchar();
	return 0;
}

//升级d2c到加密锁内
int update_d2c(const char *d2c_filename)
{
	int ret = 0;
	SS_CHAR* d2c				= NULL;
	SS_UINT32 d2c_len			= 0;
	SS_CHAR* error_msg_json		= NULL;
	
	/************************************************************************/
	/*                 1、读取d2c文件内容（字符串）             */
	/************************************************************************/
	
	ret = read_file_text(d2c_filename, d2c, d2c_len, &d2c_len);
	if (0 == d2c_len)
	{
		printf("get file size failed. %d\n", ret);
		return 1;
	}

	d2c = (SS_CHAR*)malloc(d2c_len + 1);
	if (d2c == NULL)
	{
		printf("malloc %d failed.\n", d2c);
		return 2;
	}

	memset(d2c, 0, d2c_len + 1);
	ret = read_file_text(d2c_filename, d2c, d2c_len, &d2c_len);
	if (ret)
	{
		free(d2c);
		d2c = NULL;

		printf("get d2c file(%s) content failed. %d\n", d2c_filename, ret);
		return 3;
	}

	/************************************************************************/
	/*                 2、升级加密锁数据到加密锁内            */
	/************************************************************************/
	// 前提：执行 slm_update 时确保当前计算机插入硬件锁，并且可以在用户许可工具中看到加密锁信息。
	// 如果当前计算机插入多把硬件锁，slm_update 会将d2c写入任意一把用户锁内，
	// 如果指定锁号升级d2c，请使用 slm_update_ex() 完成相关功能。
	ret = slm_update(d2c, &error_msg_json);
	printf("slm_update return message: \n%s\n",error_msg_json);
	// 需要调用者释放内存，使用完毕不释放会产生内存泄露。
	slm_free(error_msg_json);

	if (ret)
	{
		free(d2c);
		d2c = NULL;

		printf("slm_update failed %d\n",ret);
		return 4;
	}
	
	printf("update d2c success.\n");

	if (d2c)
	{
		free(d2c);
		d2c = NULL;
	}
	return 0;
}
