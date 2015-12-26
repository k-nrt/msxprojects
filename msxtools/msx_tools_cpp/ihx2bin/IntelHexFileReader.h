#pragma once

class CIntelHexFileReader
{
private:
	unsigned char m_nImage[65536];

public:
	CIntelHexFileReader(void);
	~CIntelHexFileReader(void);

	bool CreateImage( FILE *pFile );

	int GetHex( char h );
	int GetByte( char h, char l );
	int GetWord( char hh, char hl, char lh, char ll );

	unsigned char *GetImage() { return m_nImage; }
};
