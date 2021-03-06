#include "StdAfx.h"

#include "prtCommon.h"
#include "PrtConfig.h"
#include "PrtGlobal.h"
#include "tempConfig.h"

static PRT_TEMP_CFG_T g_template_cfg;
static PRT_TEMPLATE_T g_prt_template;

static char g_configTemplateDir[MAX_PATH + 1] = {0};

PRT_TEMPLATE_T* prtTemplateGet()
{
	return &g_prt_template;
}

PRT_TEMP_CFG_T* templateCfgGet(void)
{
	return &g_template_cfg;
}

static void updatePrtTemplate(void)
{
	/*本可以不用m_posInfo,直接设置此模板数据*/
	memset(&g_prt_template, 0, sizeof(g_prt_template));

	strncpy(g_prt_template.ownerNameAttr.Font, g_template_cfg.font, sizeof(g_prt_template.ownerNameAttr.Font));
	g_prt_template.ownerNameAttr.Size = g_template_cfg.fontSize;
	g_prt_template.ownerNameAttr.Attribute = g_template_cfg.Attribute;
	g_prt_template.ownerNameAttr.Color = 0;
	g_prt_template.ownerNameAttr.TransBkColor = 1;
	g_prt_template.ownerNameAttr.Space = 0;

	g_prt_template.ownerNameAttr.isPrint = g_template_cfg.name.isPrint;
	g_prt_template.ownerNameAttr.isPrintStatic = g_template_cfg.name.isPrintStatic;
	g_prt_template.ownerNameAttr.X = (float)g_template_cfg.name.posX;
	g_prt_template.ownerNameAttr.Y = (float)g_template_cfg.name.posY;
	strncpy(g_prt_template.ownerNameAttr.attrName, OWNER_NAME_ATTR, ATTR_NAME_MAX_LEN);

	memcpy(&g_prt_template.ownerCardNumAttr, &g_prt_template.ownerNameAttr, sizeof(PRT_ATTR_TEXT_T));
	g_prt_template.ownerCardNumAttr.isPrint = g_template_cfg.cardNo.isPrint;
	g_prt_template.ownerCardNumAttr.isPrintStatic = g_template_cfg.cardNo.isPrintStatic;
	g_prt_template.ownerCardNumAttr.X = (float)g_template_cfg.cardNo.posX;
	g_prt_template.ownerCardNumAttr.Y = (float)g_template_cfg.cardNo.posY;
	strncpy(g_prt_template.ownerCardNumAttr.attrName, OWNER_CARDNUM_ATTR, ATTR_NAME_MAX_LEN);

	memcpy(&g_prt_template.ownerIdentityNumAttr, &g_prt_template.ownerNameAttr, sizeof(PRT_ATTR_TEXT_T));
	g_prt_template.ownerIdentityNumAttr.isPrint = g_template_cfg.personId.isPrint;
	g_prt_template.ownerIdentityNumAttr.isPrintStatic = g_template_cfg.personId.isPrintStatic;
	g_prt_template.ownerIdentityNumAttr.X = (float)g_template_cfg.personId.posX;
	g_prt_template.ownerIdentityNumAttr.Y = (float)g_template_cfg.personId.posY;
	strncpy(g_prt_template.ownerIdentityNumAttr.attrName, OWNER_IDENTIFY_ATTR, ATTR_NAME_MAX_LEN);

	memcpy(&g_prt_template.ownerSexAttr, &g_prt_template.ownerNameAttr, sizeof(PRT_ATTR_TEXT_T));
	g_prt_template.ownerSexAttr.isPrint = g_template_cfg.sex.isPrint;
	g_prt_template.ownerSexAttr.isPrintStatic = g_template_cfg.sex.isPrintStatic;
	g_prt_template.ownerSexAttr.X = (float)g_template_cfg.sex.posX;
	g_prt_template.ownerSexAttr.Y = (float)g_template_cfg.sex.posY;
	strncpy(g_prt_template.ownerSexAttr.attrName, OWNER_SEX_ATTR, ATTR_NAME_MAX_LEN);

	memcpy(&g_prt_template.cardLaunchTimeAttr, &g_prt_template.ownerNameAttr, sizeof(PRT_ATTR_TEXT_T));
	g_prt_template.cardLaunchTimeAttr.isPrint = g_template_cfg.launchTime.isPrint;
	g_prt_template.cardLaunchTimeAttr.isPrintStatic = g_template_cfg.launchTime.isPrintStatic;
	g_prt_template.cardLaunchTimeAttr.X = (float)g_template_cfg.launchTime.posX;
	g_prt_template.cardLaunchTimeAttr.Y = (float)g_template_cfg.launchTime.posY;
	strncpy(g_prt_template.cardLaunchTimeAttr.attrName, CARD_LAUNCHTIME_ATTR, ATTR_NAME_MAX_LEN);

	memcpy(&g_prt_template.cardLaunchPlaceAttr, &g_prt_template.ownerNameAttr, sizeof(PRT_ATTR_TEXT_T));
	g_prt_template.cardLaunchPlaceAttr.isPrint = g_template_cfg.launchPlace.isPrint;
	g_prt_template.cardLaunchPlaceAttr.isPrintStatic = g_template_cfg.launchPlace.isPrintStatic;
	g_prt_template.cardLaunchPlaceAttr.X = (float)g_template_cfg.launchPlace.posX;
	g_prt_template.cardLaunchPlaceAttr.Y = (float)g_template_cfg.launchPlace.posY;
	strncpy(g_prt_template.cardLaunchPlaceAttr.attrName, CARD_LAUNCHPLACE_ATTR, ATTR_NAME_MAX_LEN);

	g_prt_template.ownerPhotoAttr.isPrint = g_template_cfg.photo.isPrint;
	g_prt_template.ownerPhotoAttr.X = (float)g_template_cfg.photo.posX;
	g_prt_template.ownerPhotoAttr.Y = (float)g_template_cfg.photo.posY;
	g_prt_template.ownerPhotoAttr.Weight = DEF_PRT_PHOTO_W;
	g_prt_template.ownerPhotoAttr.Height = DEF_PRT_PHOTO_H;
	strncpy(g_prt_template.ownerPhotoAttr.attrName, OWNER_PHOTO_ATTR, ATTR_NAME_MAX_LEN);
}

int templateCfgSave(PRT_TEMP_CFG_T* posInfo)
{
	char strTemp[16] = {0};

	/*字体信息未对外公开,先备份*/
	strncpy(posInfo->font, g_template_cfg.font, sizeof(g_template_cfg.font));
	posInfo->fontSize = g_template_cfg.fontSize;
	posInfo->Attribute = g_template_cfg.Attribute;	//文字属性
	/*在拷贝到本地配置中*/
	memcpy(&g_template_cfg, posInfo, sizeof(PRT_TEMP_CFG_T));

	SNPRINTF(strTemp, sizeof(strTemp) - 1, "%d", g_template_cfg.name.isPrint);
	WritePrivateProfileString(_T("name"), _T("IsPrint"), strTemp, _T(g_configTemplateDir));
	SNPRINTF(strTemp, sizeof(strTemp) - 1, "%d", g_template_cfg.name.isPrintStatic);
	WritePrivateProfileString(_T("name"), _T("IsPrintStatic"), strTemp, _T(g_configTemplateDir));
	SNPRINTF(strTemp, sizeof(strTemp) - 1, "%f", g_template_cfg.name.posX);
	WritePrivateProfileString(_T("name"), _T("posX"), strTemp, _T(g_configTemplateDir));
	SNPRINTF(strTemp, sizeof(strTemp) - 1, "%f", g_template_cfg.name.posY);
	WritePrivateProfileString(_T("name"), _T("posY"), strTemp, _T(g_configTemplateDir));

	SNPRINTF(strTemp, sizeof(strTemp) - 1, "%d", g_template_cfg.sex.isPrint);
	WritePrivateProfileString(_T("sex"), _T("IsPrint"), strTemp, _T(g_configTemplateDir));
	SNPRINTF(strTemp, sizeof(strTemp) - 1, "%d", g_template_cfg.sex.isPrintStatic);
	WritePrivateProfileString(_T("sex"), _T("IsPrintStatic"), strTemp, _T(g_configTemplateDir));
	SNPRINTF(strTemp, sizeof(strTemp) - 1, "%f", g_template_cfg.sex.posX);
	WritePrivateProfileString(_T("sex"), _T("posX"), strTemp, _T(g_configTemplateDir));
	SNPRINTF(strTemp, sizeof(strTemp) - 1, "%f", g_template_cfg.sex.posY);
	WritePrivateProfileString(_T("sex"), _T("posY"), strTemp, _T(g_configTemplateDir));

	SNPRINTF(strTemp, sizeof(strTemp) - 1, "%d", g_template_cfg.cardNo.isPrint);
	WritePrivateProfileString(_T("cardNo"), _T("IsPrint"), strTemp, _T(g_configTemplateDir));
	SNPRINTF(strTemp, sizeof(strTemp) - 1, "%d", g_template_cfg.cardNo.isPrintStatic);
	WritePrivateProfileString(_T("cardNo"), _T("IsPrintStatic"), strTemp, _T(g_configTemplateDir));
	SNPRINTF(strTemp, sizeof(strTemp) - 1, "%f", g_template_cfg.cardNo.posX);
	WritePrivateProfileString(_T("cardNo"), _T("posX"), strTemp, _T(g_configTemplateDir));
	SNPRINTF(strTemp, sizeof(strTemp) - 1, "%f", g_template_cfg.cardNo.posY);
	WritePrivateProfileString(_T("cardNo"), _T("posY"), strTemp, _T(g_configTemplateDir));

	SNPRINTF(strTemp, sizeof(strTemp) - 1, "%d", g_template_cfg.personId.isPrint);
	WritePrivateProfileString(_T("personId"), _T("IsPrint"), strTemp, _T(g_configTemplateDir));
	SNPRINTF(strTemp, sizeof(strTemp) - 1, "%d", g_template_cfg.personId.isPrintStatic);
	WritePrivateProfileString(_T("personId"), _T("IsPrintStatic"), strTemp, _T(g_configTemplateDir));
	SNPRINTF(strTemp, sizeof(strTemp) - 1, "%f", g_template_cfg.personId.posX);
	WritePrivateProfileString(_T("personId"), _T("posX"), strTemp, _T(g_configTemplateDir));
	SNPRINTF(strTemp, sizeof(strTemp) - 1, "%f", g_template_cfg.personId.posY);
	WritePrivateProfileString(_T("personId"), _T("posY"), strTemp, _T(g_configTemplateDir));

	SNPRINTF(strTemp, sizeof(strTemp) - 1, "%d", g_template_cfg.photo.isPrint);
	WritePrivateProfileString(_T("photo"), _T("IsPrint"), strTemp, _T(g_configTemplateDir));
	SNPRINTF(strTemp, sizeof(strTemp) - 1, "%d", g_template_cfg.photo.isPrintStatic);
	WritePrivateProfileString(_T("photo"), _T("IsPrintStatic"), strTemp, _T(g_configTemplateDir));
	SNPRINTF(strTemp, sizeof(strTemp) - 1, "%f", g_template_cfg.photo.posX);
	WritePrivateProfileString(_T("photo"), _T("posX"), strTemp, _T(g_configTemplateDir));
	SNPRINTF(strTemp, sizeof(strTemp) - 1, "%f", g_template_cfg.photo.posY);
	WritePrivateProfileString(_T("photo"), _T("posY"), strTemp, _T(g_configTemplateDir));

	SNPRINTF(strTemp, sizeof(strTemp) - 1, "%d", g_template_cfg.launchTime.isPrint);
	WritePrivateProfileString(_T("launchTime"), _T("IsPrint"), strTemp, _T(g_configTemplateDir));
	SNPRINTF(strTemp, sizeof(strTemp) - 1, "%d", g_template_cfg.launchTime.isPrintStatic);
	WritePrivateProfileString(_T("launchTime"), _T("IsPrintStatic"), strTemp, _T(g_configTemplateDir));
	SNPRINTF(strTemp, sizeof(strTemp) - 1, "%f", g_template_cfg.launchTime.posX);
	WritePrivateProfileString(_T("launchTime"), _T("posX"), strTemp, _T(g_configTemplateDir));
	SNPRINTF(strTemp, sizeof(strTemp) - 1, "%f", g_template_cfg.launchTime.posY);
	WritePrivateProfileString(_T("launchTime"), _T("posY"), strTemp, _T(g_configTemplateDir));

	SNPRINTF(strTemp, sizeof(strTemp) - 1, "%d", g_template_cfg.launchPlace.isPrint);
	WritePrivateProfileString(_T("launchPlace"), _T("IsPrint"), strTemp, _T(g_configTemplateDir));
	SNPRINTF(strTemp, sizeof(strTemp) - 1, "%d", g_template_cfg.launchPlace.isPrintStatic);
	WritePrivateProfileString(_T("launchPlace"), _T("IsPrintStatic"), strTemp, _T(g_configTemplateDir));
	SNPRINTF(strTemp, sizeof(strTemp) - 1, "%f", g_template_cfg.launchPlace.posX);
	WritePrivateProfileString(_T("launchPlace"), _T("posX"), strTemp, _T(g_configTemplateDir));
	SNPRINTF(strTemp, sizeof(strTemp) - 1, "%f", g_template_cfg.launchPlace.posY);
	WritePrivateProfileString(_T("launchPlace"), _T("posY"), strTemp, _T(g_configTemplateDir));

	/*赋值给下发给打印机的结构体，本来可以合成一个*/
	updatePrtTemplate();

	PRT_LOG_INFO("write position template config file.");
	return OK;
}

int templateCfgRead(char *stationName, PRT_TEMP_CFG_T *templateCfg)
{
	unsigned int retInt = 0;
	char charTmp[32] = {0};
	
	char configTemplateDir[MAX_PATH + 1] = {0};
	SNPRINTF(configTemplateDir, MAX_PATH, "%sconfig\\station\\%s\\%s", 
								g_localModDir, stationName, PRT_CFG_TEMPLATE_FILE);

	if(0 == ::GetPrivateProfileString(_T("font"), _T("font"),
									_T(""), templateCfg->font,
									63, _T(configTemplateDir)))
	{
		CString str;
		str.Format(_T("读字体配置失败"));
		MessageBox(NULL, str, "加载配置错误", MB_OK);
		return ET_ERROR;
	}

	if(0 == ::GetPrivateProfileString(_T("font"), _T("fontSize"),
										_T(""), charTmp,
										31, _T(configTemplateDir)))
	{
		CString str;
		str.Format(_T("读字体大小配置失败"));
		MessageBox(NULL, str, "加载配置错误", MB_OK);
		return ET_ERROR;
	}
	templateCfg->fontSize = (float)_ttof(charTmp);

	templateCfg->Attribute = ::GetPrivateProfileInt(_T("font"), _T("attribute"), 0, _T(configTemplateDir));

	retInt = ::GetPrivateProfileInt(_T("name"), _T("IsPrint"), 0, _T(configTemplateDir));
	templateCfg->name.isPrint = retInt == 0 ? FALSE : TRUE;
	retInt = ::GetPrivateProfileInt(_T("name"), _T("IsPrintStatic"), 0, _T(configTemplateDir));
	templateCfg->name.isPrintStatic = retInt == 0 ? FALSE : TRUE;
	::GetPrivateProfileString(_T("name"), _T("posX"), _T("1.0"), charTmp, 31, _T(configTemplateDir));
	templateCfg->name.posX = _ttof(charTmp);
	::GetPrivateProfileString(_T("name"), _T("posY"), _T("1.0"), charTmp, 31, _T(configTemplateDir));
	templateCfg->name.posY = _ttof(charTmp);

	retInt = ::GetPrivateProfileInt(_T("sex"), _T("IsPrint"), 0, _T(configTemplateDir));
	templateCfg->sex.isPrint = retInt == 0 ? FALSE : TRUE;
	retInt = ::GetPrivateProfileInt(_T("sex"), _T("IsPrintStatic"), 0, _T(configTemplateDir));
	templateCfg->sex.isPrintStatic = retInt == 0 ? FALSE : TRUE;
	::GetPrivateProfileString(_T("sex"), _T("posX"), _T("1.0"), charTmp, 31, _T(configTemplateDir));
	templateCfg->sex.posX = _ttof(charTmp);
	::GetPrivateProfileString(_T("sex"), _T("posY"), _T("1.0"), charTmp, 31, _T(configTemplateDir));
	templateCfg->sex.posY = _ttof(charTmp);

	retInt = ::GetPrivateProfileInt(_T("cardNo"), _T("IsPrint"), 0, _T(configTemplateDir));
	templateCfg->cardNo.isPrint = retInt == 0 ? FALSE : TRUE;
	retInt = ::GetPrivateProfileInt(_T("cardNo"), _T("IsPrintStatic"), 0, _T(configTemplateDir));
	templateCfg->cardNo.isPrintStatic = retInt == 0 ? FALSE : TRUE;
	::GetPrivateProfileString(_T("cardNo"), _T("posX"), _T("1.0"), charTmp, 31, _T(configTemplateDir));
	templateCfg->cardNo.posX = _ttof(charTmp);
	::GetPrivateProfileString(_T("cardNo"), _T("posY"), _T("1.0"), charTmp, 31, _T(configTemplateDir));
	templateCfg->cardNo.posY = _ttof(charTmp);

	retInt = ::GetPrivateProfileInt(_T("personId"), _T("IsPrint"), 0, _T(configTemplateDir));
	templateCfg->personId.isPrint = retInt == 0 ? FALSE : TRUE;
	retInt = ::GetPrivateProfileInt(_T("personId"), _T("IsPrintStatic"), 0, _T(configTemplateDir));
	templateCfg->personId.isPrintStatic = retInt == 0 ? FALSE : TRUE;
	::GetPrivateProfileString(_T("personId"), _T("posX"), _T("1.0"), charTmp, 31, _T(configTemplateDir));
	templateCfg->personId.posX = _ttof(charTmp);
	::GetPrivateProfileString(_T("personId"), _T("posY"), _T("1.0"), charTmp, 31, _T(configTemplateDir));
	templateCfg->personId.posY = _ttof(charTmp);

	retInt = ::GetPrivateProfileInt(_T("photo"), _T("IsPrint"), 0, _T(configTemplateDir));
	templateCfg->photo.isPrint = retInt == 0 ? FALSE : TRUE;
	retInt = ::GetPrivateProfileInt(_T("photo"), _T("IsPrintStatic"), 0, _T(configTemplateDir));
	templateCfg->photo.isPrintStatic = retInt == 0 ? FALSE : TRUE;
	::GetPrivateProfileString(_T("photo"), _T("posX"), _T("1.0"), charTmp, 31, _T(configTemplateDir));
	templateCfg->photo.posX = _ttof(charTmp);
	::GetPrivateProfileString(_T("photo"), _T("posY"), _T("1.0"), charTmp, 31, _T(configTemplateDir));
	templateCfg->photo.posY = _ttof(charTmp);

	retInt = ::GetPrivateProfileInt(_T("launchTime"), _T("IsPrint"), 0, _T(configTemplateDir));
	templateCfg->launchTime.isPrint = retInt == 0 ? FALSE : TRUE;
	retInt = ::GetPrivateProfileInt(_T("launchTime"), _T("IsPrintStatic"), 0, _T(configTemplateDir));
	templateCfg->launchTime.isPrintStatic = retInt == 0 ? FALSE : TRUE;
	::GetPrivateProfileString(_T("launchTime"), _T("posX"), _T("1.0"), charTmp, 31, _T(configTemplateDir));
	templateCfg->launchTime.posX = _ttof(charTmp);
	::GetPrivateProfileString(_T("launchTime"), _T("posY"), _T("1.0"), charTmp, 31, _T(configTemplateDir));
	templateCfg->launchTime.posY = _ttof(charTmp);

	retInt = ::GetPrivateProfileInt(_T("launchPlace"), _T("IsPrint"), 0, _T(configTemplateDir));
	templateCfg->launchPlace.isPrint = retInt == 0 ? FALSE : TRUE;
	retInt = ::GetPrivateProfileInt(_T("launchPlace"), _T("IsPrintStatic"), 0, _T(configTemplateDir));
	templateCfg->launchPlace.isPrintStatic = retInt == 0 ? FALSE : TRUE;
	::GetPrivateProfileString(_T("launchPlace"), _T("posX"), _T("1.0"), charTmp, 31, _T(configTemplateDir));
	templateCfg->launchPlace.posX = _ttof(charTmp);
	::GetPrivateProfileString(_T("launchPlace"), _T("posY"), _T("1.0"), charTmp, 31, _T(configTemplateDir));
	templateCfg->launchPlace.posY = _ttof(charTmp);

	return OK;
}

int templateCfgInit(char *stationName)
{
	memset(&g_template_cfg, 0, sizeof(g_template_cfg));

	SNPRINTF(g_configTemplateDir, MAX_PATH, "%sconfig\\station\\%s\\%s", 
								g_localModDir, stationName, PRT_CFG_TEMPLATE_FILE);

	/*从文件中读配置*/
	templateCfgRead(stationName, &g_template_cfg);

	/*赋值给下发给打印机的结构体，本来可以合成一个*/
	updatePrtTemplate();
	return OK;
}
