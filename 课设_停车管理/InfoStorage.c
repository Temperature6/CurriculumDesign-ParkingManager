#pragma warning(disable:4996)
#include "InfoStorage.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char tempCarInfoStr[CAR_INFO_LEN];

/*
* @brief 把文件中的数据读到存储车辆信息的指针中
* @param filePath 要读取的文件的路径
* @param infoList 车辆信息指针
* @retval 返回结果
*/
int ReadDataFile(const char* filePath, CarInfo* infoList)
{
	int fileLength = 0;
	FILE* fp = fopen(filePath, "r");
	int infoScannedCount = 0, indexScannedCount = 0;
	if (fp == NULL)							//打开失败
	{
		//文件打开失败，用户处理程序
		return FAIL_OPEN_FILE;
	}
	else
	{
		fseek(fp, 0, SEEK_END);				//把文件指针设置到文件尾
		fileLength =  ftell(fp);			//获取文件长度

		char* fileStr = (char*)malloc(fileLength);
		if (fileStr == NULL)
		{
			//内存分配失败，用户处理程序
			return UNKNOWN;
		}
		else
		{
			for (int i = 0; i < PARK_SPOT_COUNT; i++)
			{
				memset(tempCarInfoStr, 0, CAR_INFO_LEN);
				ReadLine(fp, tempCarInfoStr, CAR_INFO_LEN * i);
				//先获取当前车辆所在停车位
				int index = i;
				indexScannedCount = sscanf(tempCarInfoStr, "%d %*8s %*lld %*lld", &index);
				index--;	//转换为索引

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
* @brief 把存储车辆信息的指针中的数据存到文件里
* @param filePath 要存储的文件的路径
* @param infoList 存储车辆信息的指针
* @retval 返回结果
*/
int SaveDataFile(const char* filePath, CarInfo* infoList)
{
	FILE* fp = fopen(filePath, "w");
	if (fp == NULL)
	{
		//用户处理程序
		return FAIL_OPEN_FILE;
	}
	else
	{
		for (int i = 0;
			i < PARK_SPOT_COUNT;
			i++)
		{
			if (infoList[i].entryTimeStamp != -1 && infoList[i].entryTimeStamp != 0)	//该车位有车辆
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
* @brief 读取文件中的一行
* @param fp 文件指针
* @param buffer 存储读取数据的指针
* @param offset 指定从相对于文件开始位置的文件指针偏移量，是读取开始的位置
* @retval 读取了多少字节的数据
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
* @brief 读取总金额
* @param filePath 文件路径
* @param val 存放数组的指针
* @retval 返回结果
*/
int ReadMoney(const char* filePath, int* val)
{
	char str[11] = "";
	FILE* fp = fopen(filePath, "r");

	if (fp == NULL)
	{
		//打开文件失败，用户处理程序
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
* @brief 保存总金额
* @param filePath 文件路径
* @param val 金额
* @retval 返回结果
*/
int SaveMoney(const char* filePath, int val)
{
	char str[11] = "";
	FILE* fp = fopen(filePath, "w");

	if (fp == NULL)
	{
		//打开文件失败，用户处理程序
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
