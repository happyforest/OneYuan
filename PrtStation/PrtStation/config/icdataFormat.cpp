
#include "StdAfx.h"

#include "prtCommon.h"
#include "PrtConfig.h"
#include "PrtGlobal.h"
#include "icdataFormat.h"


#import "msxml3.dll" named_guids
using namespace MSXML2;

static int g_icdataFmtElmCnt;
static PRT_DATA_FMT_T g_icdataFmt[DATA_ATTR_MAX_CNT];

PRT_DATA_FMT_T* icDataFmtInfoGet(int *count)
{
	*count = g_icdataFmtElmCnt;
	return &g_icdataFmt[0];
}

int icDataFmtInit(void)
{
	int iRet = OK;

	memset(&g_icdataFmt[0], 0 , sizeof(g_icdataFmt));
	g_icdataFmtElmCnt = 0;

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

	m_plDomDocument->load(g_configIcDataFmtDir);
	XMLROOT = m_plDomDocument->GetdocumentElement();//获得根节点;

	XMLROOT->get_childNodes(&XMLNODES);//获得根节点的所有子节点;
	long XMLNODESNUM, ATTSNUM;
	XMLNODES->get_length(&XMLNODESNUM);//获得所有子节点的个数;

	if (XMLNODESNUM > DATA_ATTR_MAX_CNT)
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

		for(int J=0;J<ATTSNUM;J++)
		{
			XMLNODEATTS->get_item(J,&XMLNODE);//获得某个属性;

			nodeNameStr = (char*)(_bstr_t)XMLNODE->nodeName;
			nodeValueStr = (char*)(_bstr_t)XMLNODE->text;


			if (strncmp((LPCTSTR)nodeNameStr, "name", strlen("name")) == 0)
			{
				strncpy(g_icdataFmt[I].attrName, (LPCTSTR)nodeValueStr, DATA_ATTR_NAME_LEN);
				g_icdataFmt[I].attrName[DATA_ATTR_NAME_LEN - 1] = 0;

				count++;
			}
			else if (strncmp((LPCTSTR)nodeNameStr, "positonInIc", strlen("positonInIc")) == 0)
			{
				strncpy(g_icdataFmt[I].positonInIc, (LPCTSTR)nodeValueStr, DATA_ATTR_ICPOS_LEN);
				count++;
			}
			else if (strncmp((LPCTSTR)nodeNameStr, "isSendToIc", strlen("isSendToIc")) == 0)
			{
				if (atoi((LPCTSTR)nodeValueStr) != 0)
				{
					g_icdataFmt[I].isSendToIc = TRUE;
				}
				else
				{
					g_icdataFmt[I].isSendToIc = FALSE;
				}
				count++;
			}
			else if (strncmp((LPCTSTR)nodeNameStr, "isPrint", strlen("isPrint")) == 0)
			{
				if (atoi((LPCTSTR)nodeValueStr) != 0)
				{
					g_icdataFmt[I].isPrint = TRUE;
				}
				else
				{
					g_icdataFmt[I].isPrint = FALSE;
				}
				count++;
			}
			else if (strncmp((LPCTSTR)nodeNameStr, "printName", strlen("printName")) == 0)
			{
				strncpy(g_icdataFmt[I].printName, (LPCTSTR)nodeValueStr, ATTR_NAME_MAX_LEN);
				count++;
			}
			else if (strncmp((LPCTSTR)nodeNameStr, "required", strlen("required")) == 0)
			{
				if (atoi((LPCTSTR)nodeValueStr) != 0)
				{
					g_icdataFmt[I].isRequired = TRUE;
				}
				else
				{
					g_icdataFmt[I].isRequired = FALSE;
				}
				count++;
			}
			else if (strncmp((LPCTSTR)nodeNameStr, "type", strlen("type")) == 0)
			{
				if (strncmp((LPCTSTR)nodeValueStr, "an", 2) == 0)
				{
					g_icdataFmt[I].codeType = CODE_AN;
				}
				else if (strncmp((LPCTSTR)nodeValueStr, "cn", 2) == 0)
				{
					g_icdataFmt[I].codeType = CODE_CN;
				}
				else
				{
					PRT_LOG_ERROR("code type invalid：%s", (LPCTSTR)nodeValueStr);
					break;
				}

				count++;
			}
			else if (strncmp((LPCTSTR)nodeNameStr, "minlen", strlen("minlen")) == 0)
			{
				g_icdataFmt[I].minLen = atoi((LPCTSTR)nodeValueStr);
				count++;
			}
			else if (strncmp((LPCTSTR)nodeNameStr, "maxlen", strlen("maxlen")) == 0)
			{
				g_icdataFmt[I].maxLen = atoi((LPCTSTR)nodeValueStr);
				count++;
			}
			else if (strncmp((LPCTSTR)nodeNameStr, "defaultVal", strlen("defaultVal")) == 0)
			{
				strncpy(g_icdataFmt[I].defaultVal, (LPCTSTR)nodeValueStr, DEF_VAL_MAX_LEN);
				g_icdataFmt[I].defaultVal[DEF_VAL_MAX_LEN-1] = 0;

				count++;
			}

			XMLNODE.Release();
		}

		XMLNODE_ELM.Release();
		XMLNODEATTS.Release();

		if (count != MAX_ATTR_CNT)
		{
			PRT_LOG_ERROR("解析失败,属性个数不够：%d", count);
			iRet = ET_ERROR;
			goto parseEnd;
		}
		else
		{
			if (g_icdataFmt[I].codeType == CODE_CN)
			{
				g_icdataFmt[I].maxLen *= 2;
				g_icdataFmt[I].minLen *= 2;
			}

			PRT_LOG_DEBUG("parse succ, index %d, name %s, positonInIc %s, isSendToIc %d, isPrint %d, printName %s, isrequired %d, type %d, minlen %d, maxlen %d, default %s.",
					I, g_icdataFmt[I].attrName, g_icdataFmt[I].positonInIc,
					g_icdataFmt[I].isSendToIc, g_icdataFmt[I].isPrint, g_icdataFmt[I].printName,
					g_icdataFmt[I].isRequired, g_icdataFmt[I].codeType, g_icdataFmt[I].minLen,
					g_icdataFmt[I].maxLen,
					g_icdataFmt[I].defaultVal);
		}
	}

	g_icdataFmtElmCnt = XMLNODESNUM;

parseEnd:
	XMLNODES.Release();
	XMLROOT.Release();

	m_plDomDocument.Release();

///TODO:
	CoUninitialize();
	return iRet;
}


int icDataFmtValidChk()
{
	BOOL nameOk = FALSE;
	BOOL sexOk = FALSE;
	BOOL cardNoOk = FALSE;
	BOOL personIdOk = FALSE;
	BOOL photoOk = FALSE;
	BOOL launchTimeOk = FALSE;
	BOOL launchPlaceOk = FALSE;

	PRT_TEMPLATE_T* printTemp = prtTemplateGet();

	if (!printTemp->ownerNameAttr.isPrint)
	{
		nameOk = TRUE;
	}

	if (!printTemp->ownerSexAttr.isPrint)
	{
		sexOk = TRUE;
	}

	if (!printTemp->ownerCardNumAttr.isPrint)
	{
		cardNoOk = TRUE;
	}

	if (!printTemp->ownerIdentityNumAttr.isPrint)
	{
		personIdOk = TRUE;
	}

	if (!printTemp->ownerPhotoAttr.isPrint)
	{
		photoOk = TRUE;
	}

	if (!printTemp->cardLaunchTimeAttr.isPrint)
	{
		launchTimeOk = TRUE;
	}

	if (!printTemp->cardLaunchPlaceAttr.isPrint)
	{
		launchPlaceOk = TRUE;
	}

	int i = 0;
	for (i = 0; i < g_icdataFmtElmCnt; i++)
	{
		if (!nameOk)
		{
			if (0 == strncmp(g_icdataFmt[i].attrName,
					printTemp->ownerNameAttr.attrName,
					ATTR_NAME_MAX_LEN)
					&& g_icdataFmt[i].isPrint)
			{
				nameOk = TRUE;
				continue;
			}
		}

		if (!sexOk)
		{
			if (0 == strncmp(g_icdataFmt[i].attrName,
					printTemp->ownerSexAttr.attrName,
					ATTR_NAME_MAX_LEN)
					&& g_icdataFmt[i].isPrint)
			{
				sexOk = TRUE;
				continue;
			}
		}

		if (!cardNoOk)
		{
			if (0 == strncmp(g_icdataFmt[i].attrName,
					printTemp->ownerCardNumAttr.attrName,
					ATTR_NAME_MAX_LEN)
					&& g_icdataFmt[i].isPrint)
			{
				cardNoOk = TRUE;
				continue;
			}
		}

		if (!personIdOk)
		{
			if (0 == strncmp(g_icdataFmt[i].attrName,
					printTemp->ownerIdentityNumAttr.attrName,
					ATTR_NAME_MAX_LEN)
					&& g_icdataFmt[i].isPrint)
			{
				personIdOk = TRUE;
				continue;
			}
		}

		if (!photoOk)
		{
			if (0 == strncmp(g_icdataFmt[i].attrName,
					printTemp->ownerPhotoAttr.attrName,
					ATTR_NAME_MAX_LEN)
					&& g_icdataFmt[i].isPrint)
			{
				photoOk = TRUE;
				continue;
			}
		}


		if (!launchTimeOk)
		{
			if (0 == strncmp(g_icdataFmt[i].attrName,
					printTemp->cardLaunchTimeAttr.attrName,
					ATTR_NAME_MAX_LEN)
					&& g_icdataFmt[i].isPrint)
			{
				launchTimeOk = TRUE;
				continue;
			}
		}

		if (!launchPlaceOk)
		{
			if (0 == strncmp(g_icdataFmt[i].attrName,
					printTemp->cardLaunchPlaceAttr.attrName,
					ATTR_NAME_MAX_LEN)
					&& g_icdataFmt[i].isPrint)
			{
				launchPlaceOk = TRUE;
				continue;
			}
		}
	}

	if (launchTimeOk && photoOk && personIdOk
			&& cardNoOk && sexOk && nameOk && launchPlaceOk)
	{
		return OK;
	}

	PRT_LOG_ERROR("launchTimeOk：%d,photoOk：%d,personIdOk：%d,cardNoOk：%d,sexOk：%d,nameOk：%d, launchPlaceOk:%d.",
			launchTimeOk, photoOk, personIdOk,
			cardNoOk, sexOk, nameOk, launchPlaceOk);
	return ET_ERROR;
}
