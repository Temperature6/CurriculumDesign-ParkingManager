#ifndef CONSOLE_MENU_H
#define CONSOLE_MENU_H
#define C_NEW(DataType)			(DataType*)malloc(sizeof(DataType))
#define C_DELETE(Mem)			free(Mem)

#define MAX_ACTION_COUNT		10			//最大动作数
#define MAX_ACTION_NAME_LEN		30			//动作名称最大长度
#define	DEFAULT_HL_COLOR		YELLOW		//默认高亮颜色
#define BACK_GROUND_COLOR		BLACK		//默认背景颜色
#define DEFAULT_TEXT_COLOR		WHITE		//默认字体颜色

typedef enum ACTIONTYPE
{
	AT_Value = 0,				//菜单返回一个值
	AT_Function,				//菜单调用一个函数
	AT_SubMenu,					//菜单展示一个子菜单
}ActionType;

typedef enum COLOR
{
	BLACK = 0,					//黑
	BLUE,						//蓝
	GREEN,						//绿
	CYAN,						//青
	RED,						//红
	MAGENTA,					//洋红
	BROWN,						//棕
	LIGHTGRAY,					//淡灰
	DARKGRAY,					//深灰
	LIGHTBLUE,					//淡蓝
	LIGHTGREEN,					//淡绿
	LIGHTCYAN,					//淡青
	LIGHTRED,					//淡红
	LIGHTMAGENTA,				//淡洋红
	YELLOW,						//黄
	WHITE,						//白
}Color;

typedef struct MENU
{
	int ActionCount;						//动作个数(不需要用户指定)
	struct ACTION *ActionList;				//动作列表(不需要用户指定)
	int ShowPosX;							//显示开始位置X
	int ShowPosY;							//显示开始位置Y
	Color HighLightColor;					//选中时的高亮颜色
	Color ComTextColor;						//未选中时的默认颜色
}Menu;

typedef struct ACTION
{
	int ActionType;							//指定菜单类型(enum ActionType)
	char ShortCut;							//指定快捷键（一个字母）
	char ActionName[MAX_ACTION_NAME_LEN];	//菜单显示文字
	/*AT_Value*/
	int Value;								//为返回值的菜单指定返回值
	/*AT_Function*/
	void(*Callback)(void);					//为调用函数的菜单指定函数指针
	/*AT_SubMenu*/
	struct MENU *SubMenu;					//为子菜单指定菜单内容
}Action;



/*菜单函数*/
int ExecMenu(Menu* specMenu, int* ret);
void InitMenu(Menu* specMenu);
static void ShowMenu(Menu* specMenu);
static void ClearMenu(Menu* specMenu);
static int RunChoose(Menu* specMenu, int* retDst);
static int GetInputKey();
/*动作函数*/
void InitAction(Action* action);
void SetActionName(Action* action, char* name);
void AddAction(Menu* specMenu, Action* action);

/*图形函数*/
void ClearScreen();
void SetTitle(const char* title);
void SetFontColor(Color color);
void MoveCursor(int startX, int startY);
void InitScreen();


#endif // !CONSOLE_MENU_H
