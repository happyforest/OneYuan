#ifndef _TEMP_CFG_H
#define _TEMP_CFG_H


#define  DEF_PRT_TEXT_FONT  "宋体"
#define  DEF_PRT_TEXT_SIZE   8

#define  DEF_PRT_PHOTO_W   20
#define  DEF_PRT_PHOTO_H   25

#define ATTR_NAME_MAX_LEN  32

#define OWNER_NAME_ATTR "姓名"
#define OWNER_IDENTIFY_ATTR "社会保障号码"
#define OWNER_CARDNUM_ATTR "卡号"
#define OWNER_SEX_ATTR "性别"
#define CARD_LAUNCHTIME_ATTR "发卡日期"
#define CARD_LAUNCHPLACE_ATTR "数据来源"
#define OWNER_PHOTO_ATTR "照片"

typedef struct
{
	char attrName[ATTR_NAME_MAX_LEN + 1];

	int isPrint;
	int isPrintStatic;

	float X;					//打印位置的横坐标
	float Y;					//打印位置的纵坐标
	char Font[32];				//使用的字体名称
	float Size;					//字号大小
	unsigned char Attribute;	//文字属性
	int Color;					//文字颜色（使用标准的Web颜色代码）
	BOOL  TransBkColor;
	int  Space;
}PRT_ATTR_TEXT_T;


typedef struct
{
	char attrName[ATTR_NAME_MAX_LEN + 1];
	int isPrint;

	float X;		//打印位置的横坐标
	float Y;		//打印位置的纵坐标
	float Weight;	//打印图片的宽度
	float Height;	//打印图片的高度
}PRT_ATTR_IMAGE_T;


typedef struct
{
	PRT_ATTR_TEXT_T ownerNameAttr;
	PRT_ATTR_TEXT_T ownerCardNumAttr;

	PRT_ATTR_TEXT_T ownerSexAttr;
	PRT_ATTR_TEXT_T cardLaunchTimeAttr;
	PRT_ATTR_TEXT_T cardLaunchPlaceAttr;

	PRT_ATTR_TEXT_T ownerIdentityNumAttr;
	PRT_ATTR_IMAGE_T ownerPhotoAttr;
}PRT_TEMPLATE_T;


typedef struct
{
	bool isPrint;
	bool isPrintStatic;
	double posX;
	double posY;
}ELM_POS_T;

typedef struct
{
	ELM_POS_T name;
	ELM_POS_T sex;
	ELM_POS_T cardNo;
	ELM_POS_T personId;
	ELM_POS_T photo;
	ELM_POS_T launchTime;
	ELM_POS_T launchPlace;

	char font[64];
	float fontSize;
	unsigned char Attribute;	//文字属性
}PRT_TEMP_CFG_T;


PRT_TEMPLATE_T* prtTemplateGet();

PRT_TEMP_CFG_T* templateCfgGet(void);
int templateCfgSave(PRT_TEMP_CFG_T* posInfo);
int templateCfgInit(char *stationName);

int templateCfgRead(char *stationName, PRT_TEMP_CFG_T *templateCfg);
#endif

