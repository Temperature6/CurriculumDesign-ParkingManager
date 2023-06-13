#pragma warning(disable:4996)
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "ConsoleMenu.h"
#include "InfoStorage.h"

#define DATA_FILE_PATH		"data.txt"
#define MONEY_FILE_PATH		"money.txt"
#define HISTORY_FILE_PATH	"history.txt"
#define PAUSE				system("pause")
#define PRICE				10					//10元/小时

Menu* mainMenu;
Action* carEntry, * carLeave, * showEmpty, * findCar, * showCar, * showMoney, * exitSys;
Menu* subMenu;
Action* findByID, * findByNum;

int sumMoney = 0;

CarInfo carInfoList[PARK_SPOT_COUNT] = { 0 };

/*数据操作*/
int GetEmptySpot();
long long GetCurTimeStamp();
void GetTimeStr(char* buffer, int len, long long timeStamp);
int isCarExist(const char* carID);
void DeleteCar(int index);
void AddHistory(CarInfo* info, int parkNum, int price);
/*菜单回调*/
void ExitProgram(void);						//退出程序
void CarEntry(void);						//车辆进入
void CarLeave(void);						//车辆离开
void ShowAllSpot(void);						//显示所有车位
void FindByID(void);						//通过车牌查找
void FindByNum(void);						//通过车位号查找
void ShowAllCar(void);						//显示所有车位
void ShowMoney(void);						//显示总收入

int main()
{
	/*菜单内容初始化*/
	mainMenu = C_NEW(Menu);
	InitMenu(mainMenu);
	mainMenu->ComTextColor = WHITE;
	mainMenu->HighLightColor = RED;
	mainMenu->ShowPosX = 4;
	mainMenu->ShowPosY = 1;

	carEntry = C_NEW(Action);
	InitAction(carEntry);
	SetActionName(carEntry, "车辆进入");
	carEntry->ActionType = AT_Function;
	carEntry->ShortCut = 'e';
	carEntry->Callback = CarEntry;

	carLeave = C_NEW(Action);
	InitAction(carLeave);
	SetActionName(carLeave, "车辆离开");
	carLeave->ActionType = AT_Function;
	carLeave->ShortCut = 'l';
	carLeave->Callback = CarLeave;

	showEmpty = C_NEW(Action);
	InitAction(showEmpty);
	SetActionName(showEmpty, "显示空车位");
	showEmpty->ActionType = AT_Function;
	showEmpty->ShortCut = 'a';
	showEmpty->Callback = ShowAllSpot;

	
	subMenu = C_NEW(Menu);
	InitMenu(subMenu);
	subMenu->ComTextColor = DARKGRAY;
	subMenu->HighLightColor = BLUE;

	findByID = C_NEW(Action);
	InitAction(findByID);
	SetActionName(findByID, "通过车牌号查找车辆");
	findByID->ActionType = AT_Function;
	findByID->ShortCut = 'i';
	findByID->Callback = FindByID;

	findByNum = C_NEW(Action);
	InitAction(findByNum);
	SetActionName(findByNum, "查询指定车位是否有车辆");
	findByNum->ActionType = AT_Function;
	findByNum->ShortCut = 'n';
	findByNum->Callback = FindByNum;

	AddAction(subMenu, findByID);
	AddAction(subMenu, findByNum);

	findCar = C_NEW(Action);
	InitAction(findCar);
	SetActionName(findCar, "查询车辆信息");
	findCar->ActionType = AT_SubMenu;
	findCar->ShortCut = 'i';
	findCar->SubMenu = subMenu;

	showCar = C_NEW(Action);
	InitAction(showCar);
	SetActionName(showCar, "显示停车场内车辆");
	showCar->ActionType = AT_Function;
	showCar->ShortCut = 'c';
	showCar->Callback = ShowAllCar;

	showMoney = C_NEW(Action);
	InitAction(showMoney);
	SetActionName(showMoney, "显示收费总金额");
	showMoney->ActionType = AT_Function;
	showMoney->ShortCut = 'm';
	showMoney->Callback = ShowMoney;

	exitSys = C_NEW(Action);
	InitAction(exitSys);
	SetActionName(exitSys, "退出系统");
	exitSys->ActionType = AT_Function;
	exitSys->ShortCut = 'E';
	exitSys->Callback = ExitProgram;

	AddAction(mainMenu, carEntry);
	AddAction(mainMenu, carLeave);
	AddAction(mainMenu, showEmpty);
	AddAction(mainMenu, findCar);
	AddAction(mainMenu, showCar);
	AddAction(mainMenu, showMoney);
	AddAction(mainMenu, exitSys);

	ReadDataFile(DATA_FILE_PATH, carInfoList);
	ReadMoney(MONEY_FILE_PATH, &sumMoney);
	int userChoose = 0;
	
	while (1)
	{
		InitScreen();
		SetTitle("欢迎使用停车场管理系统");
		MoveCursor(0, 0);
		SetFontColor(WHITE);
		printf("使用‘↑’‘↓’高亮要选择的菜单,使用‘→’选择该菜单,出现“任意键”时，推荐使用空格或回车");

		ExecMenu(mainMenu, &userChoose);
	}
}

/*
* @brief 获取最靠前的车位的车位号(索引)
* @retval 返回最靠前的车位的车位号(索引)，当没有可用车位的时候，返回-1
*/
int GetEmptySpot()
{
	int emptyIndex = -1;
	for (int i = 0; i < PARK_SPOT_COUNT; i++)
	{
		if (carInfoList[i].entryTimeStamp == 0 || carInfoList[i].entryTimeStamp == -1)
		{
			emptyIndex = i;
			break;
		}
	}
	return emptyIndex;
}

/*
* @brief 获取当前时间戳（10位）
* @retval 当前时间戳（10位）
*/
long long GetCurTimeStamp()
{
	return time(NULL);
}

/*
* @brief 菜单回调 - 退出程序
*/
void ExitProgram(void)
{
	SaveDataFile(DATA_FILE_PATH, carInfoList);
	SaveMoney(MONEY_FILE_PATH, sumMoney);
	exit(0);
}

/*
* @brief 菜单回调 - 车辆进入停车场
*/
void CarEntry(void)
{
	InitScreen();
	//判断是否还有空车位
	int empty = GetEmptySpot();

	if (empty == -1)
	{
		printf("对不起，已经没有空车位了！\n");
		PAUSE;
	}
	else
	{
		char tempCarID[CAR_ID_LEN] = "";
		printf("输入车牌号(例：冀A520AQ)，系统为您安排车位！(只输入q则退出)\n");
		scanf("%s", tempCarID);
		if (strlen(tempCarID) < 8)
			return;
		else if (tempCarID[0] == 'q')
			return;

		if (isCarExist(tempCarID) != -1)
		{
			printf("错误输入！车辆已经在停车场中了！\n");
			PAUSE;
		}
		else
		{
			char timeStr[50] = "";
			GetTimeStr(timeStr, 50, 0);

			memcpy(carInfoList[empty].carID, tempCarID, strlen(tempCarID));	//初始化车牌号
			carInfoList[empty].parkNum = empty + 1;							//停车位ID
			carInfoList[empty].entryTimeStamp = GetCurTimeStamp();			//进入时间

			printf("\n");
			printf("车辆：\033[;31m%s\033[0m 于 \033[;34m%s\033[0m 进入停车场，车位号：\033[;32m%02d\033[0m \n",
				carInfoList[empty].carID,
				timeStr,
				empty + 1);
			PAUSE;
		}
		
	}
	
}

/*
* @brief 判断车辆是否存在
* @param carID 车牌号
* @retval 若车辆存在，返回车辆所在停车位的索引；若不存在，返回-1
*/
int isCarExist(const char* carID)
{
	int ret = -1;
	for (int i = 0; i < PARK_SPOT_COUNT; i++)
	{
		if (!strcmp(carInfoList[i].carID, carID))
			return i;
	}
	return ret;
}

/*
* @brief 菜单回调 - 车辆离开
*/
void CarLeave()
{
	char tempCarID[CAR_ID_LEN] = "";
	InitScreen();
	printf("输入您的车牌号，程序为您自动计费（10元/小时）\n");
	memset(tempCarID, 0, CAR_ID_LEN);
	scanf("%s", tempCarID);

	int index = isCarExist(tempCarID);
	if (index == -1)
	{
		printf("\n车辆不存在，不能离开，请检查输入！\n");
		PAUSE;
	}
	else
	{
		char timeStr[50] = "";
		GetTimeStr(timeStr, 50, 0);
		int curTime = GetCurTimeStamp();
		int durTime = curTime - carInfoList[index].entryTimeStamp;
		int hours = (int)(durTime / 3600 + 1);
		
		printf("\n车辆 \033[;31m%s\033[0m 于 \033[;34m%s\033[0m 离开停车场\n停车时长 \033[;33m%d\033[0m 小时收取费用 \033[;33m%d\033[0m 元\n欢迎下次使用，一路顺风！\n",
			tempCarID,
			timeStr,
			hours,
			hours * PRICE);

		sumMoney += hours * PRICE;
		AddHistory(&carInfoList[index], index + 1, hours * PRICE);
		DeleteCar(index);
		PAUSE;
	}
}

/*
* @brief 从车位删除一辆车的信息
* @param 要删除的车位索引
*/
void DeleteCar(int index)
{
	memset(&carInfoList[index], 0, sizeof(CarInfo));
}

/*
* @brief 获取指定时间戳格式化后的字符串
* @param buffer 存放字符串的数组
* @param len 字符串数组的长度
* @param timeStamp 指定要格式化的时间戳，当该值为0时，返回字符串为当前时间
*/
void GetTimeStr(char* buffer, int len, long long timeStamp)
{
	struct tm* current;
	if (timeStamp == 0)
	{
		time_t current_sec = time(&timeStamp);
		current = localtime(&current_sec);
	}
	else
	{
		current = localtime(&timeStamp);
	}
	strftime(buffer, len, "[%Y/%m/%d %H:%M:%S]", current);
}

/*
* @brief 菜单回调 - 显示所有空车位
*/
void ShowAllSpot()
{
	InitScreen();
	if (GetEmptySpot() == -1)
	{
		printf("对不起！已经没有可用车位了！");
		PAUSE;
	}
	else
	{
		for (int i = 0; i < PARK_SPOT_COUNT; i++)
		{
			if (carInfoList[i].entryTimeStamp == 0 || carInfoList[i].entryTimeStamp == -1)
			{
				printf("车位 \033[;32m%02d\033[0m 当前为空\n", i + 1);
			}
		}
		printf("\n");
		PAUSE;
	}
	
}

/*
* @brief 菜单回调 - 通过车牌号查找车辆
*/
void FindByID()
{
	InitScreen();
	printf("输入车牌号，程序为您查询车辆信息\n");
	char tempCarID[CAR_ID_LEN] = "";
	scanf("%s", tempCarID);

	int index = isCarExist(tempCarID);
	if (index == -1)
	{
		printf("\n对不起！您要查找的车辆信息不存在!\n");
		PAUSE;
	}
	else
	{
		char entryTimeStr[50] = "";
		GetTimeStr(entryTimeStr, 50, carInfoList[index].entryTimeStamp);
		printf("车辆 \033[;31m%s\033[0m 位于车位 \033[;32m%02d\033[0m，于 \033[;34m%s\033[0m 进入停车场\n",
			tempCarID,
			index + 1,
			entryTimeStr);
		PAUSE;
	}
}

/*
* @brief 菜单回调 - 通过车位号查找车辆
*/
void FindByNum()
{
	InitScreen();
	printf("输入停车位号，程序为您查询该停车位车辆信息\n");
	int num = 0;
	scanf("%d", &num);

	if (carInfoList[num].entryTimeStamp == 0 || carInfoList[num].entryTimeStamp == -1)
	{
		printf("该车位没有车辆！\n");
		PAUSE;
	}
	else
	{
		char entryTimeStr[50] = "";
		GetTimeStr(entryTimeStr, 50, carInfoList[num - 1].entryTimeStamp);
		printf("车辆 \033[;31m%s\033[0m 位于车位 \033[;32m%02d\033[0m，于 \033[;34m%s\033[0m 进入停车场\n",
			carInfoList[num - 1].carID,
			num,
			entryTimeStr);
		PAUSE;
	}
}

/*
* @brief 菜单回调 - 显示停车场内所有车辆
*/
void ShowAllCar()
{
	InitScreen();
	
	for (int i = 0; i < PARK_SPOT_COUNT; i++)
	{
		if (carInfoList[i].entryTimeStamp != 0 && carInfoList[i].entryTimeStamp != -1)
		{
			char timeStr[50] = "";
			GetTimeStr(timeStr, 50, carInfoList[i].entryTimeStamp);
			printf("车位 \033[;32m%02d\033[0m 车辆 \033[;31m%s\033[0m 于 \033[;34m%s\033[0m 进入停车场\n",
				i + 1,
				carInfoList[i].carID,
				timeStr);
		}
		
	}
	printf("完成打印\n");
	PAUSE;
}

/*
* @brief 菜单回调 - 显示停车场总收入
*/
void ShowMoney()
{
	InitScreen();
	char timeStr[50] = "";
	GetTimeStr(timeStr, 50, 0);
	printf("截止至 %s ，停车场已收入 %d 元\n具体记账请参考 \"%s\"中的内容\n",
		timeStr,
		sumMoney,
		HISTORY_FILE_PATH);
	PAUSE;
}

/*
* @brief 添加车辆历史
* @param info 车辆信息结构体指针
* @param parkNum 车位号
* @param price 停车费
*/
void AddHistory(CarInfo* info, int parkNum, int price)
{
	FILE* fp = fopen(HISTORY_FILE_PATH, "a+");

	char entryTime[50] = "";
	char leaveTime[50] = "";

	GetTimeStr(entryTime, 50, info->entryTimeStamp);
	GetTimeStr(leaveTime, 50, info->leaveTimeStamp);

	fprintf(fp, "车辆 %s 于 %s 进入停车位 %02d ，于 %s 离开停车场，收费 %d 元\n",
		info->carID,
		entryTime,
		parkNum,
		leaveTime,
		price);

	fclose(fp);
}
