// makemsxrom.cpp : コンソール アプリケーションのエントリ ポイントを定義します。
//

#include "stdafx.h"


int GetHex( char h )
{
	switch( h )
	{
	case '0': return 0;
	case '1': return 1;
	case '2': return 2;
	case '3': return 3;
	case '4': return 4;
	case '5': return 5;
	case '6': return 6;
	case '7': return 7;
	case '8': return 8;
	case '9': return 9;
	case 'A': return 10;
	case 'B': return 11;
	case 'C': return 12;
	case 'D': return 13;
	case 'E': return 14;
	case 'F': return 15;
	}

	return 0;
}

int GetByte( char h, char l )
{
	return GetHex( h ) << 4 | GetHex( l );
}

int GetWord( char hh, char hl, char lh, char ll )
{
	return GetByte( hh, hl ) << 8 | GetByte( lh, ll );
}


int _tmain(int argc, _TCHAR* argv[])
{
	_TCHAR *pszInputFileName = argv[1];
	_TCHAR *pszOutputFileName = argv[2];

	unsigned char nImage[65536];
	memset( nImage, 0, 65536 );

	FILE *pFile;
	_tfopen_s( &pFile, pszInputFileName, L"r" );

	if( pFile == NULL )
		return -1;

	for(;;)
	{
		//. 1 行読む.
		char szBuf[4096];
		if( fgets( szBuf, 4096, pFile ) == NULL  )
			break;

		char *pPos = szBuf;
		int nLength = GetByte( pPos[1], pPos[2] );
		int nOffset = GetWord( pPos[3], pPos[4], pPos[5], pPos[6] );
		int nType   = GetByte( pPos[7], pPos[8] );

		if( nType == 0x00 )
		{
			for( int i = 0 ; i < nLength ; i++ )
			{
				nImage[nOffset+i] = (unsigned char) GetByte( pPos[9+i*2], pPos[10+i*2] );
			}
		}
		else if( nType == 0x01 )
		{
			break;
		}
	}

	fclose( pFile );

	_tfopen_s( &pFile, pszOutputFileName, L"wb" );
	fwrite( &(nImage[16384]), 1, 32768, pFile );
	fclose( pFile );

	return 0;
}

