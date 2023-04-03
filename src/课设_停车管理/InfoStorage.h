#ifndef INFO_STORAGE_H_
#define INFO_STORAGE_H_
#include <stdio.h>

#define PARK_SPOT_COUNT		20	//停车位个数
#define CAR_ID_LEN			9	//车牌号字符串长度
#define CAR_INFO_LEN		35	//单个车辆信息存储字符串长度

typedef struct
{
	int parkNum;				//车位号
	char carID[9];				//车牌号
	long long entryTimeStamp;	//车辆进入时的时间戳，没有车辆时，该值为-1
	long long leaveTimeStamp;	//车辆离开时的时间戳，车辆未离开时，该值为-1
}CarInfo;

typedef enum
{
	SUCCESS = 0,
	FAIL_OPEN_FILE,
	FAIL_WRITE_DATA,
	NOTHING_TO_READ,
	UNKNOWN,
}RetVal;

int ReadFile(const char* filePath, CarInfo* infoList);
int SaveFile(const char* filePath, CarInfo* infoList);
int ReadMoney(const char* filePath, int* val);
int SaveMoney(const char* filePath, int val);

static int ReadLine(FILE* fp, char* buffer, int offset);

#endif //INFO_STORAGE_H_
