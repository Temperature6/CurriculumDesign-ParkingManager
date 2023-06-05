#pragma warning(disable:4996)
#include "ConsoleMenu.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <conio.h>
#include <Windows.h>

typedef enum
{
	KEY_EMPTY = 0,
	KEY_UP,
	KEY_DOWN,
	KEY_LEFT,
	KEY_RIGHT,
}Key;


/*菜单函数*/
/*
* @brief 执行一个菜单
* @param specMenu 要执行的菜单
* @param ret 存储返回值的整型指针，不需要返回值可传入NULL
* @retval 返回确定的菜单索引
*/
int ExecMenu(Menu* specMenu, int* ret)
{
	ShowMenu(specMenu);
	return RunChoose(specMenu, ret);
}

/*
* @brief 初始化空菜单
* @param specMenu 要初始化的菜单
*/
void InitMenu(Menu* specMenu)
{
	specMenu->ActionCount = 0;
	specMenu->ActionList = (Action*)malloc(sizeof(Action) * MAX_ACTION_COUNT);
	memset(specMenu->ActionList, 0, sizeof(Action) * MAX_ACTION_COUNT);
	specMenu->HighLightColor = DEFAULT_HL_COLOR;
	specMenu->ComTextColor = DEFAULT_TEXT_COLOR;
	specMenu->ShowPosX = 0;
	specMenu->ShowPosY = 0;
}

/*
* @brief 显示菜单，这个函数通常不需要用户调用
* @param specMenu 要显示的菜单
*/
void ShowMenu(Menu* specMenu)
{
	SetFontColor(specMenu->ComTextColor == BACK_GROUND_COLOR ? DEFAULT_TEXT_COLOR : specMenu->ComTextColor);
	for (int i = 0; i < specMenu->ActionCount; i++)
	{
		MoveCursor(specMenu->ShowPosX, specMenu->ShowPosY + i);
		printf("%s%s",
			specMenu->ActionList[i].ActionName,
			specMenu->ActionList[i].ActionType == AT_SubMenu ? "..." : "");
		if (specMenu->ActionList[i].ShortCut != 0)
		{
			printf("(%c)", specMenu->ActionList[i].ShortCut);
		}
	}

	SetFontColor(specMenu->HighLightColor);
	MoveCursor(specMenu->ShowPosX, specMenu->ShowPosY);
	printf("%s%s",
		specMenu->ActionList[0].ActionName,
		specMenu->ActionList[0].ActionType == AT_SubMenu ? "..." : "");
}

/*
* @brief 清除显示出来的菜单
* @param specMenu 要清除的菜单
*/
void ClearMenu(Menu* specMenu)
{
	int maxLen = 0, len = 0;
	for (int i = 0; i < specMenu->ActionCount; i++)
	{
		len = strlen(specMenu->ActionList[i].ActionName) + 3;
		len += specMenu->ActionList[i].ActionType == AT_SubMenu ? 3 : 0;
		if (len > maxLen)
			maxLen = len;
	}
	SetFontColor(BACK_GROUND_COLOR);
	for (int i = specMenu->ShowPosX; i < specMenu->ShowPosX + maxLen; i++)
	{
		for (int j = specMenu->ShowPosY; j < specMenu->ShowPosY + specMenu->ActionCount; j++)
		{
			MoveCursor(i, j);
			printf(" ");
		}
	}
}

/*
* @brief 运行菜单选择
* @param specMenu 要运行的菜单
* @param retDst 存储返回值的整型指针，不需要返回值可传入NULL
* @retval 返回确定的菜单索引
*/
int RunChoose(Menu* specMenu, int* retDst)
{
	int conRun = 1;
	int currentIndex = 0;
	int dirt = 0;
	while (conRun)
	{
		dirt = GetInputKey();
		//printf("%d", dirt);
		switch (dirt)
		{
		case KEY_UP:
			MoveCursor(specMenu->ShowPosX, specMenu->ShowPosY + currentIndex);
			SetFontColor(specMenu->ComTextColor);
			printf("%s%s",
				specMenu->ActionList[currentIndex].ActionName,
				specMenu->ActionList[currentIndex].ActionType == AT_SubMenu ? "..." : "");
			currentIndex--;
			if (currentIndex < 0)
				currentIndex = specMenu->ActionCount - 1;
			break;
		case KEY_DOWN:
			MoveCursor(specMenu->ShowPosX, specMenu->ShowPosY + currentIndex);
			SetFontColor(specMenu->ComTextColor);
			printf("%s%s",
				specMenu->ActionList[currentIndex].ActionName,
				specMenu->ActionList[currentIndex].ActionType == AT_SubMenu ? "..." : "");
			currentIndex++;
			if (currentIndex >= specMenu->ActionCount)
				currentIndex = 0;
			break;
		case KEY_LEFT:
			printf("\a");
			break;
		case KEY_RIGHT:
			if (specMenu->ActionList[currentIndex].ActionType == AT_Value)
			{
				conRun = 0;
				*retDst = specMenu->ActionList[currentIndex].Value;
				ClearMenu(specMenu);
			}
			else if (specMenu->ActionList[currentIndex].ActionType == AT_Function)
			{
				conRun = 0;
				retDst = NULL;
				if (specMenu->ActionList[currentIndex].Callback != NULL)
				{
					specMenu->ActionList[currentIndex].Callback();
				}
				ClearMenu(specMenu);
			}
			else if (specMenu->ActionList[currentIndex].ActionType == AT_SubMenu)
			{
				specMenu->ActionList[currentIndex].SubMenu->ShowPosX = specMenu->ShowPosX + 
					strlen(specMenu->ActionList[currentIndex].ActionName) + 
					6 + 
					(specMenu->ActionList[currentIndex].ShortCut != 0 ? 3 : 0);

				specMenu->ActionList[currentIndex].SubMenu->ShowPosY = specMenu->ShowPosY + currentIndex;
				ExecMenu(specMenu->ActionList[currentIndex].SubMenu, retDst);
				ClearMenu(specMenu->ActionList[currentIndex].SubMenu);
				conRun = 0;
			}
			break;
		default:
			if (dirt != KEY_EMPTY)
			{
				for (int i = 0; i < specMenu->ActionCount; i++)
				{
					if (specMenu->ActionList[i].ShortCut == dirt)
					{
						if (specMenu->ActionList[i].ActionType == AT_Value)
						{
							conRun = 0;
							*retDst = specMenu->ActionList[i].Value;
							ClearMenu(specMenu);
						}
						else if (specMenu->ActionList[i].ActionType == AT_Function)
						{
							conRun = 0;
							retDst = NULL;
							if (specMenu->ActionList[i].Callback != NULL)
							{
								specMenu->ActionList[i].Callback();
							}
							ClearMenu(specMenu);
						}
						else if (specMenu->ActionList[i].ActionType == AT_SubMenu)
						{
							specMenu->ActionList[i].SubMenu->ShowPosX = specMenu->ShowPosX +
								strlen(specMenu->ActionList[i].ActionName) +
								6 +
								(specMenu->ActionList[i].ShortCut != 0 ? 3 : 0);

							specMenu->ActionList[i].SubMenu->ShowPosY = specMenu->ShowPosY + i;
							ExecMenu(specMenu->ActionList[i].SubMenu, retDst);
							ClearMenu(specMenu->ActionList[i].SubMenu);
							conRun = 0;
						}
					}
				}
			}
			break;
		}
		if (dirt != KEY_EMPTY && conRun)
		{
			MoveCursor(specMenu->ShowPosX, specMenu->ShowPosY + currentIndex);
			SetFontColor(specMenu->HighLightColor);
			printf("%s%s",
				specMenu->ActionList[currentIndex].ActionName,
				specMenu->ActionList[currentIndex].ActionType == AT_SubMenu ? "..." : "");
		}
		
	}
	return currentIndex;
}

int GetInputKey()
{
	if (GetAsyncKeyState(VK_UP))
	{
		while (GetAsyncKeyState(VK_UP));
		return KEY_UP;
	}
	else if (GetAsyncKeyState(VK_DOWN))
	{
		while (GetAsyncKeyState(VK_DOWN));
		return KEY_DOWN;
	}
	else if (GetAsyncKeyState(VK_LEFT))
	{
		while (GetAsyncKeyState(VK_LEFT));
		return KEY_LEFT;
	}
	else if (GetAsyncKeyState(VK_RIGHT))
	{
		while (GetAsyncKeyState(VK_RIGHT));
		return KEY_RIGHT;
	}
	else
	{
		return _getch();
	}
}

/*动作函数*/
/*
* @brief 初始化动作
* @param action 要初始化的动作
*/
void InitAction(Action* action)
{
	/*action->ActionType = AT_Value;
	action->Callback = NULL;
	action->SubMenu = NULL;
	action->Value = 0;
	memset(action->ActionName, 0, MAX_ACTION_NAME_LEN);*/
	memset(action, 0, sizeof(Action));
}

/*
* @brief 设置动作的名字
* @param action 要设置名字的动作指针
* @param name 名字
*/
void SetActionName(Action* action, char* name)
{
	if (strlen(name) > MAX_ACTION_NAME_LEN)
		return;
	memset(action->ActionName, 0, MAX_ACTION_NAME_LEN);
	memcpy(action->ActionName, name, strlen(name));
}

/*
* @brief 向菜单添加动作
* @param specMenu 向哪个菜单添加动作
* @param action 要添加的动作指针
*/
void AddAction(Menu* specMenu, Action* action)
{
	specMenu->ActionList[specMenu->ActionCount++] = *action;
}

void SetFontColor(Color color)
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

void MoveCursor(int startX, int startY)
{
	HANDLE hd;
	COORD pos;
	pos.X = startX;
	pos.Y = startY;
	hd = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorPosition(hd, pos);
}

/*
* @brief 初始化屏幕（清空屏幕，指针设置到 0，0）
*/
void InitScreen()
{
	ClearScreen();
	SetFontColor(WHITE);
	MoveCursor(0, 0);
}

/*
* @brief 清除控制台屏幕
*/
void ClearScreen()
{
	system("cls");
}

/*
* @brief 设置控制台窗口标题
* @param title 标题
*/
void SetTitle(const char* title)
{
	char* titleCmd = (char*)malloc(strlen(title) + 8);
	if (titleCmd != NULL)
	{
		memset(titleCmd, 0, strlen(title) + 8);
		sprintf(titleCmd, "title %s", title);

		system(titleCmd);
	}
	
	free(titleCmd);
}
