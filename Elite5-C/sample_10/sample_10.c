/* ********************************************************
Copyright (C )2016 Beijing Senseshield Technology Co.,Ltd
Author : senseshield 2017-6
Description : 演示种子码签发文件的使用方法。

History :
[作者 日期 修改内容]
********************************************************* */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "d2c.h"
#include "ss_error.h"
#include "ss_lm_runtime.h"
#include "common.h"
#include "cJSON/cJSON.h"
#include "ss_help.h"

#pragma warning(disable : 4996)
#define TEST_FILENAME "hello.evx"

//种子码签发文件
int seed_file_d2c(char *filepath);

int main()
{
	seed_file_d2c(TEST_FILENAME);
	printf("\npress any key exit process.\n");
	getchar();
	return 0;
}

//种子码签发文件
//种子码签发文件生成的锁内升级文件（D2C），可用于任意加密锁升级，不限定某一把锁。
//证书签发文件生成的锁内升级文件（D2C），只能用于特定加密锁升级。
int seed_file_d2c(char *filepath)
{
    int ret = 0;
    MASTER_HANDLE hmaster		= NULL;
    D2C_HANDLE hD2c		= NULL;
	SS_CHAR fileBufferBin[2048] = {0};
	SS_CHAR fileBufferHex[2048*2] = {0};
	SS_UINT32 fileSize			= 2048;

	SS_CHAR* d2c				= NULL;
	SS_UINT32 d2c_len			= 0;
	SS_CHAR d2c_filename[256]	= {0};
    cJSON *root					= NULL;
	int bind_lic_array[2]		= {1, 2};

    SS_BYTE root_ca_cert[CERT_SIZE] = {0};
    SS_UINT32 root_ca_cert_len = CERT_SIZE;

	/************************************************************************/
	/*                1、读取文件(升级到锁内的文件),转换成十六进制字符串            */
	/************************************************************************/
	ret = read_file(filepath, fileBufferBin, sizeof(fileBufferBin), &fileSize);
	if (0 != ret)
	{
		printf("读取：\n\t%s 失败!\n", filepath);
        return 1;
	}
	bytes_to_hexstr(fileBufferBin, fileSize, fileBufferHex);
	
	/************************************************************************/
	/*                2、打开控制锁 创建D2C对象         */
	/************************************************************************/
	ret = master_open(&hmaster);
	if ( ret != SS_OK)
	{
		printf("打开控制锁失败，请插入控制锁\n");
        return 1;
	}

    // 新开发锁必须验证PIN码，否则无法使用
	// 详情见 ../common/common.h 文件中对 PIN_DEFAULT_PWD 的注释。
    ret = masterPINVerify(hmaster, PIN_DEFAULT_PWD, strlen(PIN_DEFAULT_PWD));
    if (ret != 0)
    {
        printf("masterPINVerify error :0x%08X \n", ret);
        return 1;
    }

    // master_get_ca_cert_ex 支持获取开发锁内多个Root CA证书
    // 根证书索引序号0，获取根证书用于合成旧用户锁证书链，对应的开发锁不支持PIN码功能。
    // 根证书索引序号1，获取根证书用于合成新用户锁证书链，使用支持PIN开发锁的开发者 master_get_ca_cert_ex 参数 3 默认设置为1。
    ret = master_get_ca_cert_ex(hmaster, PKI_CA_TYPE_ROOT, 1, root_ca_cert, root_ca_cert_len, &root_ca_cert_len);
    if (ret != 0)
    {
        printf("master_get_root_ca error :0x%08X \n", ret);
        return 1;
    }

	ret = d2c_file_new(hmaster, &hD2c, SIGN_TYPE_SEED, root_ca_cert, root_ca_cert_len);
	if (ret != SS_OK)
	{
		printf("创建D2C句柄失败：[Error] 0x%08X\n", ret);
		master_close(hmaster);
		return 1;
	}

	/************************************************************************/
	/*                3、 设置升级包内容并添加到D2C         */
	/************************************************************************/
    
	//种子码签发文件json格式参数如下
	//文件操作：
	//{
	//"op":           "addfile" | "updatefile" | "delfile",   添加、更新或删除文件
	//"filename":     "file_name.xxx",            文件名，长度需小于16字节
	//"filetype":     "evx" | "evd" | "key"         文件类型，依次为可执行文件，数据文件，密钥文件（删除文件时不需要）
	//"access":       number                      文件的访问权限（添加，更新，删除文件时都可以设置，默认为0x0F（开发商所有权限，Entry不可访问）
	//"timestamp":    number(0 ~ 0xFFFFFFFF)      文件的版本（用于抗重放），如果JSON中没有此项，则使用当前时间戳生成一个版本号
	//"filebuffer":   "0123456789ABCDEF"          文件内容HEX16字符串（删除文件时不需要）
	//"fileoffset":   number,                     （可选，默认为0）文件偏移（删除文件时不需要）
	//"bind_lic":     [1,2,3,4]                   可执行文件绑定的许可（删除文件时不需要）
	//}	

	root = cJSON_CreateObject();
	cJSON_AddStringToObject(root, "op", "addfile");
	cJSON_AddStringToObject(root, "filename", TEST_FILENAME);
	cJSON_AddStringToObject(root, "filetype", "evx");
	cJSON_AddStringToObject(root, "filebuffer", (char*)fileBufferHex);
	cJSON_AddNumberToObject(root, "fileoffset", 0);
	cJSON_AddItemToObject(root, "bind_lic", cJSON_CreateIntArray(bind_lic_array, 2));

	printf("%s\n", cJSON_Print(root));

    ret = d2c_add_pkg(hD2c, cJSON_PrintUnformatted(root), "seed_file_hello_sample");
    if (ret != SS_OK)
    {
        printf("添加升级参数失败：[Error] 0x%08X\n", ret);
		master_close(hmaster);
		d2c_delete(hD2c);
		if (root)
		{
			free(root);
			root = NULL;
		}
		return 1;
    }
	/************************************************************************/
	/*                 4、 获取D2C         */
	/************************************************************************/

	//获取签发完的许可，获取buffer传入空，可获取到许可大小
	ret = d2c_get(hD2c, NULL, 0, &d2c_len);
	if (ret != 0 && d2c_len == 0)
	{
		printf("获取D2C长度失败 :%d \n", ret);
	}
	else
	{
		d2c = (SS_CHAR*)malloc(d2c_len);
		ret = d2c_get(hD2c, d2c, d2c_len, &d2c_len);
		if (ret == SS_OK)
		{
			// 写入本地文件，升级至锁内需要通过 slm_update() API接口，或者使用用户许可工具的加密锁数据升级工具。
			strcpy(d2c_filename, TEST_FILENAME);
			strcat(d2c_filename, "_package.d2c");
			write_file(d2c_filename, d2c, d2c_len);
			printf("种子码签发文件的D2C升级包制作成功：\n\t%s\n" ,d2c_filename);
		}
		else
		{
			printf("种子码签发文件的D2C升级包制作失败：[Error] 0x%08X\n", ret);
		}
		free(d2c);
	}

    if (hD2c != NULL)
    {
	    d2c_delete(hD2c);
    }
    masterPINDeauth(hmaster);
    master_close(hmaster);
	if (root)
	{
		free(root);
		root = NULL;
	}
	return 0;
}
