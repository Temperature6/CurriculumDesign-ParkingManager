#ifndef INFO_STORAGE_H_
#define INFO_STORAGE_H_
#include <stdio.h>

#define PARK_SPOT_COUNT		20	//ͣ��λ����
#define CAR_ID_LEN			9	//���ƺ��ַ�������
#define CAR_INFO_LEN		35	//����������Ϣ�洢�ַ�������

typedef struct
{
	int parkNum;				//��λ��
	char carID[10];				//���ƺ�
	long long entryTimeStamp;	//��������ʱ��ʱ�����û�г���ʱ����ֵΪ-1��0
	long long leaveTimeStamp;	//�����뿪ʱ��ʱ���������δ�뿪ʱ����ֵΪ-1��0
}CarInfo;

typedef enum
{
	SUCCESS = 0,
	FAIL_OPEN_FILE,
	FAIL_WRITE_DATA,
	NOTHING_TO_READ,
	UNKNOWN,
}RetVal;

int ReadDataFile(const char* filePath, CarInfo* infoList);
int SaveDataFile(const char* filePath, CarInfo* infoList);
int ReadMoney(const char* filePath, int* val);
int SaveMoney(const char* filePath, int val);

static int ReadLine(FILE* fp, char* buffer, int offset);

#endif //INFO_STORAGE_H_
