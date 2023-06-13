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
#define PRICE				10					//10Ԫ/Сʱ

Menu* mainMenu;
Action* carEntry, * carLeave, * showEmpty, * findCar, * showCar, * showMoney, * exitSys;
Menu* subMenu;
Action* findByID, * findByNum;

int sumMoney = 0;

CarInfo carInfoList[PARK_SPOT_COUNT] = { 0 };

/*���ݲ���*/
int GetEmptySpot();
long long GetCurTimeStamp();
void GetTimeStr(char* buffer, int len, long long timeStamp);
int isCarExist(const char* carID);
void DeleteCar(int index);
void AddHistory(CarInfo* info, int parkNum, int price);
/*�˵��ص�*/
void ExitProgram(void);						//�˳�����
void CarEntry(void);						//��������
void CarLeave(void);						//�����뿪
void ShowAllSpot(void);						//��ʾ���г�λ
void FindByID(void);						//ͨ�����Ʋ���
void FindByNum(void);						//ͨ����λ�Ų���
void ShowAllCar(void);						//��ʾ���г�λ
void ShowMoney(void);						//��ʾ������

int main()
{
	/*�˵����ݳ�ʼ��*/
	mainMenu = C_NEW(Menu);
	InitMenu(mainMenu);
	mainMenu->ComTextColor = WHITE;
	mainMenu->HighLightColor = RED;
	mainMenu->ShowPosX = 4;
	mainMenu->ShowPosY = 1;

	carEntry = C_NEW(Action);
	InitAction(carEntry);
	SetActionName(carEntry, "��������");
	carEntry->ActionType = AT_Function;
	carEntry->ShortCut = 'e';
	carEntry->Callback = CarEntry;

	carLeave = C_NEW(Action);
	InitAction(carLeave);
	SetActionName(carLeave, "�����뿪");
	carLeave->ActionType = AT_Function;
	carLeave->ShortCut = 'l';
	carLeave->Callback = CarLeave;

	showEmpty = C_NEW(Action);
	InitAction(showEmpty);
	SetActionName(showEmpty, "��ʾ�ճ�λ");
	showEmpty->ActionType = AT_Function;
	showEmpty->ShortCut = 'a';
	showEmpty->Callback = ShowAllSpot;

	
	subMenu = C_NEW(Menu);
	InitMenu(subMenu);
	subMenu->ComTextColor = DARKGRAY;
	subMenu->HighLightColor = BLUE;

	findByID = C_NEW(Action);
	InitAction(findByID);
	SetActionName(findByID, "ͨ�����ƺŲ��ҳ���");
	findByID->ActionType = AT_Function;
	findByID->ShortCut = 'i';
	findByID->Callback = FindByID;

	findByNum = C_NEW(Action);
	InitAction(findByNum);
	SetActionName(findByNum, "��ѯָ����λ�Ƿ��г���");
	findByNum->ActionType = AT_Function;
	findByNum->ShortCut = 'n';
	findByNum->Callback = FindByNum;

	AddAction(subMenu, findByID);
	AddAction(subMenu, findByNum);

	findCar = C_NEW(Action);
	InitAction(findCar);
	SetActionName(findCar, "��ѯ������Ϣ");
	findCar->ActionType = AT_SubMenu;
	findCar->ShortCut = 'i';
	findCar->SubMenu = subMenu;

	showCar = C_NEW(Action);
	InitAction(showCar);
	SetActionName(showCar, "��ʾͣ�����ڳ���");
	showCar->ActionType = AT_Function;
	showCar->ShortCut = 'c';
	showCar->Callback = ShowAllCar;

	showMoney = C_NEW(Action);
	InitAction(showMoney);
	SetActionName(showMoney, "��ʾ�շ��ܽ��");
	showMoney->ActionType = AT_Function;
	showMoney->ShortCut = 'm';
	showMoney->Callback = ShowMoney;

	exitSys = C_NEW(Action);
	InitAction(exitSys);
	SetActionName(exitSys, "�˳�ϵͳ");
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
		SetTitle("��ӭʹ��ͣ��������ϵͳ");
		MoveCursor(0, 0);
		SetFontColor(WHITE);
		printf("ʹ�á���������������Ҫѡ��Ĳ˵�,ʹ�á�����ѡ��ò˵�,���֡��������ʱ���Ƽ�ʹ�ÿո��س�");

		ExecMenu(mainMenu, &userChoose);
	}
}

/*
* @brief ��ȡ�ǰ�ĳ�λ�ĳ�λ��(����)
* @retval �����ǰ�ĳ�λ�ĳ�λ��(����)����û�п��ó�λ��ʱ�򣬷���-1
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
* @brief ��ȡ��ǰʱ�����10λ��
* @retval ��ǰʱ�����10λ��
*/
long long GetCurTimeStamp()
{
	return time(NULL);
}

/*
* @brief �˵��ص� - �˳�����
*/
void ExitProgram(void)
{
	SaveDataFile(DATA_FILE_PATH, carInfoList);
	SaveMoney(MONEY_FILE_PATH, sumMoney);
	exit(0);
}

/*
* @brief �˵��ص� - ��������ͣ����
*/
void CarEntry(void)
{
	InitScreen();
	//�ж��Ƿ��пճ�λ
	int empty = GetEmptySpot();

	if (empty == -1)
	{
		printf("�Բ����Ѿ�û�пճ�λ�ˣ�\n");
		PAUSE;
	}
	else
	{
		char tempCarID[CAR_ID_LEN] = "";
		printf("���복�ƺ�(������A520AQ)��ϵͳΪ�����ų�λ��(ֻ����q���˳�)\n");
		scanf("%s", tempCarID);
		if (strlen(tempCarID) < 8)
			return;
		else if (tempCarID[0] == 'q')
			return;

		if (isCarExist(tempCarID) != -1)
		{
			printf("�������룡�����Ѿ���ͣ�������ˣ�\n");
			PAUSE;
		}
		else
		{
			char timeStr[50] = "";
			GetTimeStr(timeStr, 50, 0);

			memcpy(carInfoList[empty].carID, tempCarID, strlen(tempCarID));	//��ʼ�����ƺ�
			carInfoList[empty].parkNum = empty + 1;							//ͣ��λID
			carInfoList[empty].entryTimeStamp = GetCurTimeStamp();			//����ʱ��

			printf("\n");
			printf("������\033[;31m%s\033[0m �� \033[;34m%s\033[0m ����ͣ��������λ�ţ�\033[;32m%02d\033[0m \n",
				carInfoList[empty].carID,
				timeStr,
				empty + 1);
			PAUSE;
		}
		
	}
	
}

/*
* @brief �жϳ����Ƿ����
* @param carID ���ƺ�
* @retval ���������ڣ����س�������ͣ��λ���������������ڣ�����-1
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
* @brief �˵��ص� - �����뿪
*/
void CarLeave()
{
	char tempCarID[CAR_ID_LEN] = "";
	InitScreen();
	printf("�������ĳ��ƺţ�����Ϊ���Զ��Ʒѣ�10Ԫ/Сʱ��\n");
	memset(tempCarID, 0, CAR_ID_LEN);
	scanf("%s", tempCarID);

	int index = isCarExist(tempCarID);
	if (index == -1)
	{
		printf("\n���������ڣ������뿪���������룡\n");
		PAUSE;
	}
	else
	{
		char timeStr[50] = "";
		GetTimeStr(timeStr, 50, 0);
		int curTime = GetCurTimeStamp();
		int durTime = curTime - carInfoList[index].entryTimeStamp;
		int hours = (int)(durTime / 3600 + 1);
		
		printf("\n���� \033[;31m%s\033[0m �� \033[;34m%s\033[0m �뿪ͣ����\nͣ��ʱ�� \033[;33m%d\033[0m Сʱ��ȡ���� \033[;33m%d\033[0m Ԫ\n��ӭ�´�ʹ�ã�һ·˳�磡\n",
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
* @brief �ӳ�λɾ��һ��������Ϣ
* @param Ҫɾ���ĳ�λ����
*/
void DeleteCar(int index)
{
	memset(&carInfoList[index], 0, sizeof(CarInfo));
}

/*
* @brief ��ȡָ��ʱ�����ʽ������ַ���
* @param buffer ����ַ���������
* @param len �ַ�������ĳ���
* @param timeStamp ָ��Ҫ��ʽ����ʱ���������ֵΪ0ʱ�������ַ���Ϊ��ǰʱ��
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
* @brief �˵��ص� - ��ʾ���пճ�λ
*/
void ShowAllSpot()
{
	InitScreen();
	if (GetEmptySpot() == -1)
	{
		printf("�Բ����Ѿ�û�п��ó�λ�ˣ�");
		PAUSE;
	}
	else
	{
		for (int i = 0; i < PARK_SPOT_COUNT; i++)
		{
			if (carInfoList[i].entryTimeStamp == 0 || carInfoList[i].entryTimeStamp == -1)
			{
				printf("��λ \033[;32m%02d\033[0m ��ǰΪ��\n", i + 1);
			}
		}
		printf("\n");
		PAUSE;
	}
	
}

/*
* @brief �˵��ص� - ͨ�����ƺŲ��ҳ���
*/
void FindByID()
{
	InitScreen();
	printf("���복�ƺţ�����Ϊ����ѯ������Ϣ\n");
	char tempCarID[CAR_ID_LEN] = "";
	scanf("%s", tempCarID);

	int index = isCarExist(tempCarID);
	if (index == -1)
	{
		printf("\n�Բ�����Ҫ���ҵĳ�����Ϣ������!\n");
		PAUSE;
	}
	else
	{
		char entryTimeStr[50] = "";
		GetTimeStr(entryTimeStr, 50, carInfoList[index].entryTimeStamp);
		printf("���� \033[;31m%s\033[0m λ�ڳ�λ \033[;32m%02d\033[0m���� \033[;34m%s\033[0m ����ͣ����\n",
			tempCarID,
			index + 1,
			entryTimeStr);
		PAUSE;
	}
}

/*
* @brief �˵��ص� - ͨ����λ�Ų��ҳ���
*/
void FindByNum()
{
	InitScreen();
	printf("����ͣ��λ�ţ�����Ϊ����ѯ��ͣ��λ������Ϣ\n");
	int num = 0;
	scanf("%d", &num);

	if (carInfoList[num].entryTimeStamp == 0 || carInfoList[num].entryTimeStamp == -1)
	{
		printf("�ó�λû�г�����\n");
		PAUSE;
	}
	else
	{
		char entryTimeStr[50] = "";
		GetTimeStr(entryTimeStr, 50, carInfoList[num - 1].entryTimeStamp);
		printf("���� \033[;31m%s\033[0m λ�ڳ�λ \033[;32m%02d\033[0m���� \033[;34m%s\033[0m ����ͣ����\n",
			carInfoList[num - 1].carID,
			num,
			entryTimeStr);
		PAUSE;
	}
}

/*
* @brief �˵��ص� - ��ʾͣ���������г���
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
			printf("��λ \033[;32m%02d\033[0m ���� \033[;31m%s\033[0m �� \033[;34m%s\033[0m ����ͣ����\n",
				i + 1,
				carInfoList[i].carID,
				timeStr);
		}
		
	}
	printf("��ɴ�ӡ\n");
	PAUSE;
}

/*
* @brief �˵��ص� - ��ʾͣ����������
*/
void ShowMoney()
{
	InitScreen();
	char timeStr[50] = "";
	GetTimeStr(timeStr, 50, 0);
	printf("��ֹ�� %s ��ͣ���������� %d Ԫ\n���������ο� \"%s\"�е�����\n",
		timeStr,
		sumMoney,
		HISTORY_FILE_PATH);
	PAUSE;
}

/*
* @brief ��ӳ�����ʷ
* @param info ������Ϣ�ṹ��ָ��
* @param parkNum ��λ��
* @param price ͣ����
*/
void AddHistory(CarInfo* info, int parkNum, int price)
{
	FILE* fp = fopen(HISTORY_FILE_PATH, "a+");

	char entryTime[50] = "";
	char leaveTime[50] = "";

	GetTimeStr(entryTime, 50, info->entryTimeStamp);
	GetTimeStr(leaveTime, 50, info->leaveTimeStamp);

	fprintf(fp, "���� %s �� %s ����ͣ��λ %02d ���� %s �뿪ͣ�������շ� %d Ԫ\n",
		info->carID,
		entryTime,
		parkNum,
		leaveTime,
		price);

	fclose(fp);
}
