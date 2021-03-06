#include "StdAfx.h"

#include "prtCommon.h"
#include "PrtConfig.h"
#include "PrtGlobal.h"
#include "placeCode.h"

#import "msxml3.dll" named_guids
using namespace MSXML2;


CPlaceCode g_placeMgr;
CPlaceCode g_bankMgr;
CPlaceCode g_bankWangdianMgr;
CPlaceCode g_mingzuMgr;
CPlaceCode g_zengjianMgr;

CPlaceCode::CPlaceCode(void)
{
}


CPlaceCode::~CPlaceCode(void)
{
}


int CPlaceCode::placeInit(char *file)
{
	int iRet = OK;

	HRESULT hr = CoInitialize(NULL);

	//AfxOleInit();

	MSXML2::IXMLDOMDocument2Ptr m_plDomDocument;
	 hr = m_plDomDocument.CreateInstance(MSXML2::CLSID_DOMDocument);
	if(FAILED(hr))
	{
		_com_error er(hr);
		PRT_LOG_ERROR("初始化失败：%s", er.ErrorMessage());
		return ET_ERROR;
	}

	MSXML2::IXMLDOMElementPtr XMLROOT;
	MSXML2::IXMLDOMElementPtr XMLELEMENT;
	MSXML2::IXMLDOMNodeListPtr XMLNODES; //某个节点的所以字节点
	MSXML2::IXMLDOMNamedNodeMapPtr XMLNODEATTS;//某个节点的所有属性;
	MSXML2::IXMLDOMNodePtr XMLNODE_ELM;

	m_plDomDocument->load(file);
	XMLROOT = m_plDomDocument->GetdocumentElement();//获得根节点;

	XMLROOT->get_childNodes(&XMLNODES);//获得根节点的所有子节点;
	long XMLNODESNUM, ATTSNUM;
	XMLNODES->get_length(&XMLNODESNUM);//获得所有子节点的个数;

	if (XMLNODESNUM > MAX_PLACE_CNT)
	{
		PRT_LOG_ERROR("属性个数太多：%d", XMLNODESNUM);
		iRet = ET_ERROR;
		goto parseEnd;
	}

	for(int I = 0; I < XMLNODESNUM; I++)
	{
		XMLNODES->get_item(I,&XMLNODE_ELM);//获得某个子节点;
		XMLNODE_ELM->get_attributes(&XMLNODEATTS);//获得某个节点的所有属性;
		XMLNODEATTS->get_length(&ATTSNUM);//获得所有属性的个数;

		int count = 0;
		CString nodeNameStr;
		CString nodeValueStr;
		MSXML2::IXMLDOMNodePtr XMLNODE;

		PLACE_T place;
		memset(&place, 0, sizeof(place));

		for(int J=0;J<ATTSNUM;J++)
		{
			XMLNODEATTS->get_item(J,&XMLNODE);//获得某个属性;

			nodeNameStr = (char*)(_bstr_t)XMLNODE->nodeName;
			nodeValueStr = (char*)(_bstr_t)XMLNODE->text;

			if (strncmp((LPCTSTR)nodeNameStr, "code", strlen("code")) == 0)
			{
				strncpy(place.code, (LPCTSTR)nodeValueStr, CODE_MAX_LEN);
				place.code[CODE_MAX_LEN - 1] = 0;

				count++;
			}
			else if (strncmp((LPCTSTR)nodeNameStr, "name", strlen("name")) == 0)
			{
				strncpy(place.name, (LPCTSTR)nodeValueStr, NAME_MAX_LEN);
				place.name[NAME_MAX_LEN - 1] = 0;

				count++;
			}

			XMLNODE.Release();
		}

		XMLNODE_ELM.Release();
		XMLNODEATTS.Release();

		if (count != 2)
		{
			PRT_LOG_ERROR("element解析失败,属性个数不够：%d", count);
			iRet = ET_ERROR;
			goto parseEnd;
		}
		else
		{
			PLACE_T *newPlace = new PLACE_T;
			memcpy(newPlace, &place, sizeof(PLACE_T));

			m_places.insert(newPlace);
			PRT_LOG_DEBUG("add element succ, code %s, name %s.", place.code, place.name);
		}
	}

parseEnd:
	XMLNODES.Release();
	XMLROOT.Release();

	m_plDomDocument.Release();

	CoUninitialize();
	return iRet;
}

char* CPlaceCode::getNameByCode(char *code)
{
	PlacePoolItr itr;
	PLACE_T *place;

	for (itr = m_places.begin();
			itr != m_places.end();
			itr++)
	{
		place = *itr;
		if (strncmp(place->code, code, CODE_MAX_LEN) == 0)
		{
			return place->name;
		}
	}

	return NULL;
}

char* CPlaceCode::getCodeByName(char *name)
{
	PlacePoolItr itr;
	PLACE_T *place;

	for (itr = m_places.begin();
			itr != m_places.end();
			itr++)
	{
		place = *itr;
		if (strncmp(place->name, name, CODE_MAX_LEN) == 0)
		{
			return place->code;
		}
	}

	return NULL;
}

int placeCodeInit(void)
{
	if (OK != g_placeMgr.placeInit(g_configPlaceCodeDir))
	{
		MessageBox(NULL, "城市代码初始化失败，请联系管理员检查配置文件!", "错误", MB_OK);
		return ET_ERROR;
	}
	if (OK != g_bankMgr.placeInit(g_configBankCodeDir))
	{
		MessageBox(NULL, "城市代码初始化失败，请联系管理员检查配置文件!", "错误", MB_OK);
		return ET_ERROR;
	}
	if (OK != g_bankWangdianMgr.placeInit(g_configBankWangdianCodeDir))
	{
		MessageBox(NULL, "城市代码初始化失败，请联系管理员检查配置文件!", "错误", MB_OK);
		return ET_ERROR;
	}
	if (OK != g_mingzuMgr.placeInit(g_configMingzuCodeDir))
	{
		MessageBox(NULL, "城市代码初始化失败，请联系管理员检查配置文件!", "错误", MB_OK);
		return ET_ERROR;
	}
	if (OK != g_zengjianMgr.placeInit(g_configZengjianCodeDir))
	{
		MessageBox(NULL, "城市代码初始化失败，请联系管理员检查配置文件!", "错误", MB_OK);
		return ET_ERROR;
	}

	return OK;
}
