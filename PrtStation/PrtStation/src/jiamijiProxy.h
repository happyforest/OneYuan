#ifndef _JIAMIJI_PROXY_H
#define _JIAMIJI_PROXY_H

#define  DEF_JIAMIJI_PROXY_ADDR  0x7F000001
#define  DEF_JIAMIJI_PROXY_PORT  6001

extern int g_jiamijiProxyAddr;
extern int g_jiamijiProxyPort;

extern int jiamijiProxyInit();
extern void jiamijiProxyFree();

#endif
