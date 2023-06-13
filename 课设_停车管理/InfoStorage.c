#pragma warning(disable:4996)
#include "InfoStorage.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char tempCarInfoStr[CAR_INFO_LEN];

/*
* @brief ���ļ��е����ݶ����洢������Ϣ��ָ����
* @param filePath Ҫ��ȡ���ļ���·��
* @param infoList ������Ϣָ��
* @retval ���ؽ��
*/
int ReadDataFile(const char* filePath, CarInfo* infoList)
{
	int fileLength = 0;
	FILE* fp = fopen(filePath, "r");
	int infoScannedCount = 0, indexScannedCount = 0;
	if (fp == NULL)							//��ʧ��
	{
		//�ļ���ʧ�ܣ��û��������
		return FAIL_OPEN_FILE;
	}
	else
	{
		fseek(fp, 0, SEEK_END);				//���ļ�ָ�����õ��ļ�β
		fileLength =  ftell(fp);			//��ȡ�ļ�����

		char* fileStr = (char*)malloc(fileLength);
		if (fileStr == NULL)
		{
			//�ڴ����ʧ�ܣ��û��������
			return UNKNOWN;
		}
		else
		{
			for (int i = 0; i < PARK_SPOT_COUNT; i++)
			{
				memset(tempCarInfoStr, 0, CAR_INFO_LEN);
				ReadLine(fp, tempCarInfoStr, CAR_INFO_LEN * i);
				//�Ȼ�ȡ��ǰ��������ͣ��λ
				int index = i;
				indexScannedCount = sscanf(tempCarInfoStr, "%d %*8s %*lld %*lld", &index);
				index--;	//ת��Ϊ����

				infoScannedCount = sscanf(tempCarInfoStr, "%d %8s %lld %lld",
										&infoList[index].parkNum,
										&infoList[index].carID,
										&infoList[index].entryTimeStamp,
										&infoList[index].leaveTimeStamp);

				if (infoScannedCount != 4 && indexScannedCount != EOF)
				{
					memset(&infoList[index], 0, sizeof(CarInfo));
					break;
				}
			}

		}
		free(fileStr);
		fclose(fp);
		return SUCCESS;
	}
}

/*
* @brief �Ѵ洢������Ϣ��ָ���е����ݴ浽�ļ���
* @param filePath Ҫ�洢���ļ���·��
* @param infoList �洢������Ϣ��ָ��
* @retval ���ؽ��
*/
int SaveDataFile(const char* filePath, CarInfo* infoList)
{
	FILE* fp = fopen(filePath, "w");
	if (fp == NULL)
	{
		//�û��������
		return FAIL_OPEN_FILE;
	}
	else
	{
		for (int i = 0;
			i < PARK_SPOT_COUNT;
			i++)
		{
			if (infoList[i].entryTimeStamp != -1 && infoList[i].entryTimeStamp != 0)	//�ó�λ�г���
			{
				memset(tempCarInfoStr, 0, CAR_INFO_LEN);
				sprintf(tempCarInfoStr, "%02d %s %10lld %10lld\n",
					infoList[i].parkNum,
					infoList[i].carID,
					infoList[i].entryTimeStamp,
					infoList[i].leaveTimeStamp);
				fwrite(tempCarInfoStr, 1, strlen(tempCarInfoStr), fp);
			}
		}
		fclose(fp);
		return SUCCESS;
	}
}

/*
* @brief ��ȡ�ļ��е�һ��
* @param fp �ļ�ָ��
* @param buffer �洢��ȡ���ݵ�ָ��
* @param offset ָ����������ļ���ʼλ�õ��ļ�ָ��ƫ�������Ƕ�ȡ��ʼ��λ��
* @retval ��ȡ�˶����ֽڵ�����
*/
int ReadLine(FILE* fp, char* buffer, int offset)
{
	int count = 0;
	fseek(fp, offset, SEEK_SET);
	do
	{
		count++;
		fread(buffer++, 1, 1, fp);
	} while (*(buffer - 1) != '\n');
	return count;
}

/*
* @brief ��ȡ�ܽ��
* @param filePath �ļ�·��
* @param val ��������ָ��
* @retval ���ؽ��
*/
int ReadMoney(const char* filePath, int* val)
{
	char str[11] = "";
	FILE* fp = fopen(filePath, "r");

	if (fp == NULL)
	{
		//���ļ�ʧ�ܣ��û��������
		return FAIL_OPEN_FILE;
	}
	else
	{
		fseek(fp, 0, SEEK_END);
		int length = ftell(fp);
		fseek(fp, 0, SEEK_SET);

		fread(str, 1, length, fp);
		fclose(fp);

		*val = atoi(str);
		return SUCCESS;
	}
}

/*
* @brief �����ܽ��
* @param filePath �ļ�·��
* @param val ���
* @retval ���ؽ��
*/
int SaveMoney(const char* filePath, int val)
{
	char str[11] = "";
	FILE* fp = fopen(filePath, "w");

	if (fp == NULL)
	{
		//���ļ�ʧ�ܣ��û��������
		return FAIL_OPEN_FILE;
	}
	else
	{
		sprintf(str, "%d", val);
		fwrite(str, 1, strlen(str), fp);
		fclose(fp);
		return SUCCESS;
	}
}
