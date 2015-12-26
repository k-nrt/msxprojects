// ihx2bin.cpp : コンソール アプリケーションのエントリ ポイントを定義します。
//
#include "stdafx.h"
#include "IntelHexFileReader.h"

//. intelhex2bin input.ihx output.bin [-offset 0] [-size 65536]
int _tmain(int argc, _TCHAR* argv[])
{
	_TCHAR *pszInputFileName = argv[1];
	_TCHAR *pszOutputFileName = argv[2];

	int nOffset = 0;
	for( int i = 0 ; i < argc - 1 ; i++ )
	{
		if( _tcscmp( argv[i], L"-offset" ) == 0 )
		{
			nOffset = _tstoi( argv[i+1] );
			break;
		}
	}

	int nSize = 65536;
	for( int i = 0 ; i < argc - 1 ; i++ )
	{
		if( _tcscmp( argv[i], L"-size" ) == 0 )
		{
			nSize = _tstoi( argv[i+1] );
			break;
		}
	}

	//, 読み込み.
	FILE *pFile;
	_tfopen_s( &pFile, pszInputFileName, L"r" );

	if( pFile == NULL )
		return -1;

	CIntelHexFileReader reader;
	reader.CreateImage( pFile );

	fclose( pFile );

	//. 書き出し.
	_tfopen_s( &pFile, pszOutputFileName, L"wb" );

	fwrite( reader.GetImage() + nOffset, 1, nSize, pFile );

	fclose( pFile );

	return 0;
}

