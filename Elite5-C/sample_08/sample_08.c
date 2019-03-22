/* ********************************************************
Copyright (C )2016 Beijing Senseshield Technology Co.,Ltd
Author : senseshield 2017-6
Description : ��ʾ�������������ݵ�ʹ�÷�����

History :
[���� ���� �޸�����]
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

//����Ҫ�����ļ���������(d2c�ļ�)ȫ·����д������ĺ궨�� UPDATE_D2C_FILENAME
//d2c�ļ�����ʹ��sample_07ʾ������

#define UPDATE_D2C_FILENAME ""

//����d2c����������
int update_d2c(const char *d2c_filename);

int main()
{
	if(!strlen(UPDATE_D2C_FILENAME))
	{
		printf("\n��DEMO��Ҫ��׼�������ļ���������(d2c�ļ�)ȫ·����дһ��!\n\n");
		printf("\npress any key exit process.\n");
		getchar();
		return 0;
	}

	update_d2c(UPDATE_D2C_FILENAME);
	printf("\npress any key exit process.\n");
	getchar();
	return 0;
}

//����d2c����������
int update_d2c(const char *d2c_filename)
{
	int ret = 0;
	SS_CHAR* d2c				= NULL;
	SS_UINT32 d2c_len			= 0;
	SS_CHAR* error_msg_json		= NULL;
	
	/************************************************************************/
	/*                 1����ȡd2c�ļ����ݣ��ַ�����             */
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
	/*                 2���������������ݵ���������            */
	/************************************************************************/
	// ǰ�᣺ִ�� slm_update ʱȷ����ǰ���������Ӳ���������ҿ������û���ɹ����п�����������Ϣ��
	// �����ǰ�����������Ӳ������slm_update �Ὣd2cд������һ���û����ڣ�
	// ���ָ����������d2c����ʹ�� slm_update_ex() �����ع��ܡ�
	ret = slm_update(d2c, &error_msg_json);
	printf("slm_update return message: \n%s\n",error_msg_json);
	// ��Ҫ�������ͷ��ڴ棬ʹ����ϲ��ͷŻ�����ڴ�й¶��
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
