// writedsk.cpp : コンソール アプリケーションのエントリ ポイントを定義します。
//

#include "stdafx.h"

//. writedsk target.dsk input.bin [-offset 0] [-size byte] [-sector 0] 
int _tmain(int argc, _TCHAR* argv[])
{
	FILE *pFile;

	_TCHAR *pszTargetFileName = argv[1];

	_tfopen_s( &pFile, pszTargetFileName, L"rb" );

	if( pFile == NULL )
	{
		return 1;
	}

	fseek( pFile, 0, SEEK_END );
	long nTargetSize = ftell( pFile );
	fseek( pFile, 0, SEEK_SET );

	unsigned char *pTargetFile = new unsigned char[nTargetSize];
	fread( pTargetFile, 1, nTargetSize, pFile );

	fclose( pFile );

	_TCHAR *pszInputFileName = argv[2];

	_tfopen_s( &pFile, pszInputFileName, L"rb" );

	if( pFile == NULL )
	{
		return 1;
	}

	fseek( pFile, 0, SEEK_END );
	long nInputSize = ftell( pFile );
	fseek( pFile, 0, SEEK_SET );

	unsigned char *pInputFile = new unsigned char[nInputSize];
	fread( pInputFile, 1, nInputSize, pFile );

	fclose( pFile );

	int nOffset = 0;
	for( int i = 0 ; i < argc - 1 ; i++ )
	{
		if( _tcscmp( argv[i], L"-offset" ) == 0 )
		{
			nOffset = _tstoi( argv[i+1] );
			break;
		}
	}

	int nSize = nInputSize;
	for( int i = 0 ; i < argc - 1 ; i++ )
	{
		if( _tcscmp( argv[i], L"-size" ) == 0 )
		{
			nSize = _tstoi( argv[i+1] );
			break;
		}
	}

	int nSector = 0;
	for( int i = 0 ; i < argc - 1 ; i++ )
	{
		if( _tcscmp( argv[i], L"-sector" ) == 0 )
		{
			nSector = _tstoi( argv[i+1] );
			break;
		}
	}

	if( nOffset + nSize > nInputSize )
	{
		return 1;
	}

	if( nSector*512 + nSize > nTargetSize )
	{
		return 1;
	}

	memcpy( pTargetFile + nSector*512, pInputFile + nOffset, nSize );

	_tfopen_s( &pFile, pszTargetFileName, L"wb" );
	fwrite( pTargetFile, 1, nTargetSize, pFile );
	fclose( pFile );

	return 0;
}

