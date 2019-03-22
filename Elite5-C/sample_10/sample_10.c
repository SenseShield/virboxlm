/* ********************************************************
Copyright (C )2016 Beijing Senseshield Technology Co.,Ltd
Author : senseshield 2017-6
Description : ��ʾ������ǩ���ļ���ʹ�÷�����

History :
[���� ���� �޸�����]
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

//������ǩ���ļ�
int seed_file_d2c(char *filepath);

int main()
{
	seed_file_d2c(TEST_FILENAME);
	printf("\npress any key exit process.\n");
	getchar();
	return 0;
}

//������ǩ���ļ�
//������ǩ���ļ����ɵ����������ļ���D2C����������������������������޶�ĳһ������
//֤��ǩ���ļ����ɵ����������ļ���D2C����ֻ�������ض�������������
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
	/*                1����ȡ�ļ�(���������ڵ��ļ�),ת����ʮ�������ַ���            */
	/************************************************************************/
	ret = read_file(filepath, fileBufferBin, sizeof(fileBufferBin), &fileSize);
	if (0 != ret)
	{
		printf("��ȡ��\n\t%s ʧ��!\n", filepath);
        return 1;
	}
	bytes_to_hexstr(fileBufferBin, fileSize, fileBufferHex);
	
	/************************************************************************/
	/*                2���򿪿����� ����D2C����         */
	/************************************************************************/
	ret = master_open(&hmaster);
	if ( ret != SS_OK)
	{
		printf("�򿪿�����ʧ�ܣ�����������\n");
        return 1;
	}

    // �¿�����������֤PIN�룬�����޷�ʹ��
	// ����� ../common/common.h �ļ��ж� PIN_DEFAULT_PWD ��ע�͡�
    ret = masterPINVerify(hmaster, PIN_DEFAULT_PWD, strlen(PIN_DEFAULT_PWD));
    if (ret != 0)
    {
        printf("masterPINVerify error :0x%08X \n", ret);
        return 1;
    }

    // master_get_ca_cert_ex ֧�ֻ�ȡ�������ڶ��Root CA֤��
    // ��֤���������0����ȡ��֤�����ںϳɾ��û���֤��������Ӧ�Ŀ�������֧��PIN�빦�ܡ�
    // ��֤���������1����ȡ��֤�����ںϳ����û���֤������ʹ��֧��PIN�������Ŀ����� master_get_ca_cert_ex ���� 3 Ĭ������Ϊ1��
    ret = master_get_ca_cert_ex(hmaster, PKI_CA_TYPE_ROOT, 1, root_ca_cert, root_ca_cert_len, &root_ca_cert_len);
    if (ret != 0)
    {
        printf("master_get_root_ca error :0x%08X \n", ret);
        return 1;
    }

	ret = d2c_file_new(hmaster, &hD2c, SIGN_TYPE_SEED, root_ca_cert, root_ca_cert_len);
	if (ret != SS_OK)
	{
		printf("����D2C���ʧ�ܣ�[Error] 0x%08X\n", ret);
		master_close(hmaster);
		return 1;
	}

	/************************************************************************/
	/*                3�� �������������ݲ���ӵ�D2C         */
	/************************************************************************/
    
	//������ǩ���ļ�json��ʽ��������
	//�ļ�������
	//{
	//"op":           "addfile" | "updatefile" | "delfile",   ��ӡ����»�ɾ���ļ�
	//"filename":     "file_name.xxx",            �ļ�����������С��16�ֽ�
	//"filetype":     "evx" | "evd" | "key"         �ļ����ͣ�����Ϊ��ִ���ļ��������ļ�����Կ�ļ���ɾ���ļ�ʱ����Ҫ��
	//"access":       number                      �ļ��ķ���Ȩ�ޣ���ӣ����£�ɾ���ļ�ʱ���������ã�Ĭ��Ϊ0x0F������������Ȩ�ޣ�Entry���ɷ��ʣ�
	//"timestamp":    number(0 ~ 0xFFFFFFFF)      �ļ��İ汾�����ڿ��طţ������JSON��û�д����ʹ�õ�ǰʱ�������һ���汾��
	//"filebuffer":   "0123456789ABCDEF"          �ļ�����HEX16�ַ�����ɾ���ļ�ʱ����Ҫ��
	//"fileoffset":   number,                     ����ѡ��Ĭ��Ϊ0���ļ�ƫ�ƣ�ɾ���ļ�ʱ����Ҫ��
	//"bind_lic":     [1,2,3,4]                   ��ִ���ļ��󶨵���ɣ�ɾ���ļ�ʱ����Ҫ��
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
        printf("�����������ʧ�ܣ�[Error] 0x%08X\n", ret);
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
	/*                 4�� ��ȡD2C         */
	/************************************************************************/

	//��ȡǩ�������ɣ���ȡbuffer����գ��ɻ�ȡ����ɴ�С
	ret = d2c_get(hD2c, NULL, 0, &d2c_len);
	if (ret != 0 && d2c_len == 0)
	{
		printf("��ȡD2C����ʧ�� :%d \n", ret);
	}
	else
	{
		d2c = (SS_CHAR*)malloc(d2c_len);
		ret = d2c_get(hD2c, d2c, d2c_len, &d2c_len);
		if (ret == SS_OK)
		{
			// д�뱾���ļ���������������Ҫͨ�� slm_update() API�ӿڣ�����ʹ���û���ɹ��ߵļ����������������ߡ�
			strcpy(d2c_filename, TEST_FILENAME);
			strcat(d2c_filename, "_package.d2c");
			write_file(d2c_filename, d2c, d2c_len);
			printf("������ǩ���ļ���D2C�����������ɹ���\n\t%s\n" ,d2c_filename);
		}
		else
		{
			printf("������ǩ���ļ���D2C����������ʧ�ܣ�[Error] 0x%08X\n", ret);
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
