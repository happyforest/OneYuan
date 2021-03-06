#include "StdAfx.h"

#include "prtCommon.h"
#include "prtGlobal.h"
#include "base64.h"

static unsigned char* base64=(unsigned char *)"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/=";

char * strtok_my (char *s,const char *delim)
{
    static char *olds;

    if (s == NULL)
    {
        s = olds;
    }
    else
    {
        olds = s;

    }

    if ((olds == NULL)
            || ((olds != NULL) && (*olds == '\0')))
    {
        return  NULL;
    }

    int len = strlen(s);
    char *tmpPos = s;

    int i = 0;
    while(i < len)
    {
        if (tmpPos[i] == *delim)
        {
            tmpPos[i] = '\0';
            olds = &tmpPos[i + 1];
            break;
        }

        if ((tmpPos[i] & 0x80) != 0)
        {
            i += 2;
        }
        else
        {
            i += 1;
        }
    }

    if (i >= len)
    {
        olds = NULL;
    }

    return s;
}

char* base64encode(char *src,unsigned int srclen, unsigned int *dstlen)
{
     unsigned int n,buflen,i,j;
     char *dst;

     buflen=n=srclen;

     *dstlen = buflen/3*4 + 4;

     dst=(char*)malloc(*dstlen);
     if (NULL == dst)
     {
    	 return NULL;
     }

     memset(dst,0,*dstlen);

     for(i=0,j=0;i<=srclen-3;i+=3,j+=4) {
         dst[j]=(src[i]&0xFC)>>2;
         dst[j+1]=((src[i]&0x03)<<4) + ((src[i+1]&0xF0)>>4);
         dst[j+2]=((src[i+1]&0x0F)<<2) + ((src[i+2]&0xC0)>>6);
         dst[j+3]=src[i+2]&0x3F;
     }
     if(n%3==1) {
         dst[j]=(src[i]&0xFC)>>2;
         dst[j+1]=((src[i]&0x03)<<4);
         dst[j+2]= 64;
         dst[j+3]= 64;
         j+=4;
     }
     else if(n%3==2) {
         dst[j]=(src[i]&0xFC)>>2;
         dst[j+1]=((src[i]&0x03)<<4)+((src[i+1]&0xF0)>>4);
         dst[j+2]=((src[i+1]&0x0F)<<2);
         dst[j+3]= 64;
         j+=4;
     }

     for(i=0;i<j;i++) /* map 6 bit value to base64 ASCII character */
         dst[i]=base64[(int)dst[i]];

     return dst;
}

char* base64decode(char *src, unsigned int srclen,
		char *dst, unsigned int dstMaxLen,
		unsigned int* dstlen)
{
     unsigned int n,i,j,pad;
     unsigned char *p;

     *dstlen=0;
     pad=0;
     n = srclen;

     while(n>0&&src[n-1]== '=') {
         src[n-1]=0;
         pad++;
         n--;
     }

#if 0
     for(i=0;i<srclen;i++)   { /* map base64 ASCII character to 6 bit value */
         p=(unsigned char *)strchr((const char *)base64,(int)src[i]);
         if(!p)
              break;
         src[i] = p-(unsigned char *)base64;
     }
#endif

     unsigned int needLen = srclen*3/4+1;
     if (dstMaxLen < needLen)
     {
    	 return NULL;
     }

     memset(dst,0,srclen*3/4+1);
     for(i=0,j=0; i<srclen; ) {
		 p=(unsigned char *)strchr((const char *)base64,(int)src[i]);
		 if (!p)
		 {
			 i++;
			 continue;
		 }
		 src[i] = p-(unsigned char *)base64;
		
		 p=(unsigned char *)strchr((const char *)base64,(int)src[i+1]);
		 if (!p)
		 {
			 PRT_LOG_INFO("position %d isn't base64.", i+1);
			 return NULL;
		 }
		 src[i+1] = p-(unsigned char *)base64;

		 p=(unsigned char *)strchr((const char *)base64,(int)src[i+2]);
		 if (!p)
		 {
			 PRT_LOG_INFO("position %d isn't base64.", i+2);
			 return NULL;
		 }
		 src[i+2] = p-(unsigned char *)base64;

		p=(unsigned char *)strchr((const char *)base64,(int)src[i+3]);
		if (!p)
		{
			PRT_LOG_INFO("position %d isn't base64.", i+3);
			return NULL;
		}
		src[i+3] = p-(unsigned char *)base64;

         dst[j]=(src[i]<<2) + ((src[i+1]&0x30)>>4);
         dst[j+1]=((src[i+1]&0x0F)<<4) + ((src[i+2]&0x3C)>>2);
         dst[j+2]=((src[i+2]&0x03)<<6) + src[i+3];
         *dstlen += 3;

		 j+=3;
		 i+=4;
     }
     *dstlen -= pad;

     return dst;
}

