// makedsk.cpp : �R���\�[�� �A�v���P�[�V�����̃G���g�� �|�C���g���`���܂��B
//

#include "stdafx.h"

//. makedsk output.dsk
int _tmain(int argc, _TCHAR* argv[])
{
	_TCHAR *pszOutputFileName = argv[1];

	unsigned char *pDiskImage = new unsigned char[737280];

	memset( pDiskImage, 0, 737280 );

	
	//. �����o��.
	FILE *pFile;
	_tfopen_s( &pFile, pszOutputFileName, L"wb" );

	fwrite( pDiskImage, 1, 737280, pFile );

	fclose( pFile );


	return 0;
}

