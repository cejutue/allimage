#include "pch.h"

/**
 * `encode.c' - b64
 *
 * copyright (c) 2014 joseph werle
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "b64.h"

size_t b64_decod_length(size_t srclen)
{
	return ((srclen+3)/4*3);
}
/**
 * Base64 index table.
 */

static const char b64_table[] = {
  'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H',
  'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
  'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X',
  'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f',
  'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n',
  'o', 'p', 'q', 'r', 's', 't', 'u', 'v',
  'w', 'x', 'y', 'z', '0', '1', '2', '3',
  '4', '5', '6', '7', '8', '9', '+', '/'
};
 void OnB64FunSS(void *pOpaque, const unsigned char b)
 {
	 std::stringstream &ss =*((std::stringstream*)pOpaque);
	 ss<<(const char)b;
 }

bool b64_encode (const unsigned char *pbSrcData, size_t len,std::stringstream &ss)
{
	return b64_encode(pbSrcData,len,OnB64FunSS,&ss);
}
bool b64_encode (const unsigned char *pbSrcData, size_t nSrcLen, B64Fun call,void *pOpaque)
{
	if (!pbSrcData) return false;
	if(nSrcLen <=0) return false;

	int nWritten( 0 );
	int nLen1( (nSrcLen/3)*4 );
	int nLen2( nLen1/76 );
	int nLen3( 19 );

	for (int i=0; i<=nLen2; i++)
	{
		if (i==nLen2)
			nLen3 = (nLen1%76)/4;

		for (int j=0; j<nLen3; j++)
		{
			unsigned int dwCurr(0);
			for (int n=0; n<3; n++)
			{
				dwCurr |= *pbSrcData++;
				dwCurr <<= 8;
			}
			for (int k=0; k<4; k++)
			{
				unsigned char b = (unsigned char)(dwCurr>>26);
				call(pOpaque, (const unsigned char)b64_table[b]);
				dwCurr <<= 6;
			}
		}
		nWritten+= nLen3*4;
		nWritten+= 2;
	}
	nLen2 = (nSrcLen%3) ? (nSrcLen%3 + 1) : 0;
	if (nLen2)
	{
		unsigned int dwCurr(0);
		for (int n=0; n<3; n++)
		{
			if (n<(nSrcLen%3))
				dwCurr |= *pbSrcData++;
			dwCurr <<= 8;
		}
		for (int k=0; k<nLen2; k++)
		{
			unsigned char b = (unsigned char)(dwCurr>>26);
			call(pOpaque, (const unsigned char)b64_table[b]);
			dwCurr <<= 6;
		}
		nWritten+= nLen2;
		
		nLen3 = nLen2 ? 4-nLen2 : 0;
		for (int j=0; j<nLen3; j++)
		{
			call(pOpaque, '=');
		}
		nWritten+= nLen3;
	}
	return true;
}

inline int DecodeBase64Char(unsigned int ch) throw()
{
	// returns -1 if the character is invalid
	// or should be skipped
	// otherwise, returns the 6-bit code for the character
	// from the encoding table
	if (ch >= 'A' && ch <= 'Z')
		return ch - 'A' + 0;	// 0 range starts at 'A'
	if (ch >= 'a' && ch <= 'z')
		return ch - 'a' + 26;	// 26 range starts at 'a'
	if (ch >= '0' && ch <= '9')
		return ch - '0' + 52;	// 52 range starts at '0'
	if (ch == '+')
		return 62;
	if (ch == '/')
		return 63;
	return -1;
}
bool b64_decode (const char *szSrc, size_t nSrcLen, B64Fun call,void *pOpaque)
{
	// walk the source buffer
	// each four character sequence is converted to 3 bytes
	// CRLFs and =, and any characters not in the encoding table
	// are skiped

	if (szSrc == NULL)
		return false;
	if(nSrcLen <=0)
		nSrcLen = strlen(szSrc);

	const char * szSrcEnd = szSrc + nSrcLen;
	int nWritten = 0;
	
	while (szSrc < szSrcEnd &&(*szSrc) != 0)
	{
		int dwCurr = 0;
		int i;
		int nBits = 0;
		for (i=0; i<4; i++)
		{
			if (szSrc >= szSrcEnd)
				break;
			int nCh = DecodeBase64Char(*szSrc);
			szSrc++;
			if (nCh == -1)
			{
				// skip this char
				i--;
				continue;
			}
			dwCurr <<= 6;
			dwCurr |= nCh;
			nBits += 6;
		}
		 

		// dwCurr has the 3 bytes to write to the output buffer
		// left to right
		dwCurr <<= 24-nBits;
		for (i=0; i<nBits/8; i++)
		{
			call(pOpaque,(unsigned char) ((dwCurr & 0x00ff0000) >> 16));
			dwCurr <<= 8;
			nWritten++;
		}

	}
	return true;
}