#ifndef CONSOLE_MENU_H
#define CONSOLE_MENU_H
#define C_NEW(DataType)			(DataType*)malloc(sizeof(DataType))
#define C_DELETE(Mem)			free(Mem)

#define MAX_ACTION_COUNT		10			//�������
#define MAX_ACTION_NAME_LEN		30			//����������󳤶�
#define	DEFAULT_HL_COLOR		YELLOW		//Ĭ�ϸ�����ɫ
#define BACK_GROUND_COLOR		BLACK		//Ĭ�ϱ�����ɫ
#define DEFAULT_TEXT_COLOR		WHITE		//Ĭ��������ɫ

typedef enum ACTIONTYPE
{
	AT_Value = 0,				//�˵�����һ��ֵ
	AT_Function,				//�˵�����һ������
	AT_SubMenu,					//�˵�չʾһ���Ӳ˵�
}ActionType;

typedef enum COLOR
{
	BLACK = 0,					//��
	BLUE,						//��
	GREEN,						//��
	CYAN,						//��
	RED,						//��
	MAGENTA,					//���
	BROWN,						//��
	LIGHTGRAY,					//����
	DARKGRAY,					//���
	LIGHTBLUE,					//����
	LIGHTGREEN,					//����
	LIGHTCYAN,					//����
	LIGHTRED,					//����
	LIGHTMAGENTA,				//�����
	YELLOW,						//��
	WHITE,						//��
}Color;

typedef struct MENU
{
	int ActionCount;						//��������(����Ҫ�û�ָ��)
	struct ACTION *ActionList;				//�����б�(����Ҫ�û�ָ��)
	int ShowPosX;							//��ʾ��ʼλ��X
	int ShowPosY;							//��ʾ��ʼλ��Y
	Color HighLightColor;					//ѡ��ʱ�ĸ�����ɫ
	Color ComTextColor;						//δѡ��ʱ��Ĭ����ɫ
}Menu;

typedef struct ACTION
{
	int ActionType;							//ָ���˵�����(enum ActionType)
	char ShortCut;							//ָ����ݼ���һ����ĸ��
	char ActionName[MAX_ACTION_NAME_LEN];	//�˵���ʾ����
	/*AT_Value*/
	int Value;								//Ϊ����ֵ�Ĳ˵�ָ������ֵ
	/*AT_Function*/
	void(*Callback)(void);					//Ϊ���ú����Ĳ˵�ָ������ָ��
	/*AT_SubMenu*/
	struct MENU *SubMenu;					//Ϊ�Ӳ˵�ָ���˵�����
}Action;



/*�˵�����*/
int ExecMenu(Menu* specMenu, int* ret);
void InitMenu(Menu* specMenu);
static void ShowMenu(Menu* specMenu);
static void ClearMenu(Menu* specMenu);
static int RunChoose(Menu* specMenu, int* retDst);
static int GetInputKey();
/*��������*/
void InitAction(Action* action);
void SetActionName(Action* action, char* name);
void AddAction(Menu* specMenu, Action* action);

/*ͼ�κ���*/
void ClearScreen();
void SetTitle(const char* title);
void SetFontColor(Color color);
void MoveCursor(int startX, int startY);
void InitScreen();


#endif // !CONSOLE_MENU_H
