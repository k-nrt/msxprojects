//-----------------------------------------------------------------------------
//. �������.
//-----------------------------------------------------------------------------
#include "msx-bios-wrapper.h"
#include "msx-sound.h"
#include "msx-sequence.h"

extern u8 bgm0_track0[];
extern u8 bgm0_track1[];
extern u8 bgm0_track2[];

static const s16 s_nTone[] =
{
	//.	1		2		3		4		5		6		7		8		9		10		11		12.
	//.	C		C#		D		D#		E		F		F#		G		G#		A		A#		B
		0xD5D,	0xC9C,	0xBE7,	0xB3C,	0xA9B,	0xA02,	0x973,	0x8EB,	0x86B,	0x7F2,	0x780,	0x714, 0x0, 0x0, 0x0, 0x0,
		0x6AF,	0x64E,	0x5F4,	0x59E,	0x54E,	0x501,	0x4BA,	0x476,	0x436,	0x3F9,	0x3C0,	0x38A, 0x0, 0x0, 0x0, 0x0,
		0x357,	0x327,	0x2FA,	0x2CF,	0x2A7,	0x281,	0x25D,	0x23B,	0x21B,	0x1FD,	0x1E0,	0x1C5, 0x0, 0x0, 0x0, 0x0,
		0x1AC,	0x194,	0x17D,	0x168,	0x153,	0x140,	0x12E,	0x11D,	0x10D,	0x0FE,	0x0F0,	0x0E3, 0x0, 0x0, 0x0, 0x0,
		0x0D6,	0x0CA,	0x0BE,	0x0B4,	0x0AA,	0x0A0,	0x097,	0x08F,	0x087,	0x07F,	0x078,	0x071, 0x0, 0x0, 0x0, 0x0,
		0x06B,	0x065,	0x05F,	0x05A,	0x055,	0x050,	0x04C,	0x047,	0x043,	0x040,	0x03C,	0x039, 0x0, 0x0, 0x0, 0x0,
		0x035,	0x032,	0x030,	0x02D,	0x02A,	0x028,	0x026,	0x024,	0x022,	0x020,	0x01E,	0x01C, 0x0, 0x0, 0x0, 0x0,
		0x01B,	0x019,	0x018,	0x016,	0x015,	0x014,	0x013,	0x012,	0x011,	0x010,	0x00F,	0x00E, 0x0, 0x0, 0x0, 0x0,
};


struct SSequence
{
	u16 m_nCount;
	u8 *m_pSeqStartPos;
	u8 *m_pSeqPos;
	u8 *m_pSeqMarkPos;
	u8 *m_pSeqReturnPos;
	s8 m_nOctave;
	u8 m_nQuantize;
	u8 m_nVolume;
	u8 m_nModeNext;

	u8 m_nMixer;

	s16 m_nTone;
	u8 m_nNoiseFreq;

	u8 m_nDetune;

	u8 m_nEnvVolumePos;
	s8* m_pEnvVolume;
	s8* m_pEnvVolumeNext;
	s8* m_pEnvVolumePattern[16];

	u8 m_nEnvTonePos;
	s8* m_pEnvTone;
	s8* m_pEnvToneNext;
	s8* m_pEnvTonePattern[16];

	u8* m_pEnvPitch;
	u8* m_pEnvPitchNext;
	u8* m_pEnvPitchPattern[16];

	u8* m_pEffect;
	u8* m_pEffectNext;
	u8* m_pEffectPattern[16];

	u8 m_nEnvNoisePos;
	s8* m_pEnvNoise;
	s8* m_pEnvNoisePattern[16];
};

static void _SequenceClean(  struct SSequence *pSeq );
static void _SequenceSetData( struct SSequence *pSeq, u8 *pData );
static void _SequenceExecute( u8 nChannel, struct SSequence *pSeq );

static const u8 kMixer_Tone = 1;
static const u8 kMixer_Noise  = (1 << 3);

//-----------------------------------------------------------------------------
//. �N���A.
//-----------------------------------------------------------------------------
void _SequenceClean( struct SSequence *pSeq )
{
	int i;
	pSeq->m_nCount = 0;
	pSeq->m_pSeqStartPos = NULL;
	pSeq->m_pSeqPos = NULL;
	pSeq->m_nOctave = 4;
	pSeq->m_nQuantize = 0;
	pSeq->m_nVolume = 15;
	pSeq->m_nDetune = 0;
	pSeq->m_nModeNext = 0;

	pSeq->m_nMixer = kMixer_Tone;

	pSeq->m_nTone = 0;
	pSeq->m_nNoiseFreq = 0;

	pSeq->m_pEnvVolume = NULL;
	pSeq->m_pEnvVolumeNext = NULL;

	pSeq->m_pEnvTone = NULL;
	pSeq->m_pEnvToneNext = NULL;

	pSeq->m_pEnvPitch = NULL;
	pSeq->m_pEnvPitchNext = NULL;

	pSeq->m_pEffect = NULL;
	pSeq->m_pEffectNext = NULL;

	pSeq->m_pEnvNoise = NULL;
	pSeq->m_nEnvNoisePos = 0;
	
	for( i = 0 ; i < 16; i++ )
	{
		pSeq->m_pEnvVolumePattern[i] = NULL;
		pSeq->m_pEnvTonePattern[i] = NULL;
		pSeq->m_pEnvPitchPattern[i] = NULL;
		pSeq->m_pEffectPattern[i] = NULL;
		pSeq->m_pEnvNoise[i] = NULL;
	}
}

//-----------------------------------------------------------------------------
//. �f�[�^�ݒ�.
//-----------------------------------------------------------------------------
void _SequenceSetData( struct SSequence *pSeq, u8* pData )
{
	pSeq->m_nCount = 0;
	pSeq->m_pSeqPos = pData;
	pSeq->m_pSeqStartPos = pData;
	pSeq->m_pSeqMarkPos = pData;
}

//-----------------------------------------------------------------------------
//. �V�[�P���X���s.
//-----------------------------------------------------------------------------
void _SequenceExecute( u8 nChannel, struct SSequence *pSeq )
{
	u8 nFreqLow = 0;
	u8 nFreqHigh = 0;
	s8 nVolume = 0;
	s16 nTone;
	u8 nOpCode;
	u8 nOpCodeHI;
	s8 nOctave;

	s8 nNoiseFreq = -1;

	if( pSeq->m_nCount > 0 )
	{
		if( pSeq->m_pEnvVolume && pSeq->m_nVolume != 0 )
		{
			s8 nVolumeDef = pSeq->m_pEnvVolume[pSeq->m_nEnvVolumePos];
			if( nVolumeDef != KENV_END )
			{
				nVolume = pSeq->m_nVolume + nVolumeDef;
				pSeq->m_nEnvVolumePos++;

				if( nVolume < 0 )
				{
					nVolume = 0;
				}

				if( nVolume > 15 )
				{
					nVolume = 15;
				}

				msxBiosWritePSG( 8 + nChannel, nVolume );

				if( pSeq->m_pEnvVolume[pSeq->m_nEnvVolumePos] == KENV_LOOP )
				{
					pSeq->m_nEnvVolumePos = pSeq->m_pEnvVolume[pSeq->m_nEnvVolumePos+1];
				}
			}
		}

		if( pSeq->m_pEnvTone != 0 && pSeq->m_nTone != 0 )
		{
			s8 nToneDef = pSeq->m_pEnvTone[pSeq->m_nEnvTonePos];
			if( nToneDef != KENV_END )
			{
				nTone = pSeq->m_nTone + (s16) nToneDef;
				pSeq->m_nEnvTonePos++;

				nTone += pSeq->m_nDetune;

				if( nTone < 0 )
				{
					nTone = 0;
				}

				if( nTone > 0xfff )
				{
					nTone = 0xfff;
				}

				msxBiosWritePSG( nChannel << 1, nTone & 0xff );
				msxBiosWritePSG( (nChannel << 1) + 1, nTone >> 8 );

				if( pSeq->m_pEnvTone[pSeq->m_nEnvTonePos] == KENV_LOOP )
				{
					pSeq->m_nEnvTonePos = pSeq->m_pEnvTone[pSeq->m_nEnvTonePos + 1];
				}
			}
		}

		if( pSeq->m_pEnvNoise && (pSeq->m_nMixer & kMixer_Noise ) )
		{
			s8 nNoise = pSeq->m_pEnvNoise[pSeq->m_nEnvNoisePos];

			if( nNoise == KENV_END )
			{
				nNoise = pSeq->m_pEnvNoise[pSeq->m_nEnvNoisePos-1];
			}
			else
			{
				pSeq->m_nEnvNoisePos++;
			}

			nNoiseFreq = nNoise;
			msxBiosWritePSG( 6, nNoise );
		}

		//		msxBiosWritePSG( 8 + nChannel, pSeq->m_nCount );
		pSeq->m_nCount--;
		return;
	}

	for(;;)
	{
		nOpCode = *(pSeq->m_pSeqPos);
		nOpCodeHI = (nOpCode & 0xf0) >> 4;

		//. �x�� �R�}���h.
		if( nOpCodeHI == 0 )
		{
			pSeq->m_nTone = 0;

			nFreqLow = 0;
			nFreqHigh = 0x0;

			pSeq->m_nCount = ((u16) nOpCode & 0x0f);
			pSeq->m_pSeqPos++;

			if( pSeq->m_nCount == 15 )
			{
				pSeq->m_nCount = *pSeq->m_pSeqPos;
				pSeq->m_pSeqPos++;
			}
			break;

		}

		//. �����R�}���h.
		if( nOpCodeHI <= 12 )
		{
			//. ���g��.
			nOctave = (pSeq->m_nOctave - 1);

			if( nOctave < 0 )
			{
				nOctave = 0;
			}

			if( nOctave > 7 )
			{
				nOctave = 7;
			}

			nOpCodeHI--;
			nTone = s_nTone[(nOctave<<4) + nOpCodeHI];
			pSeq->m_nTone = nTone;
			if( pSeq->m_pEnvTone )
			{
				nTone += (s16) pSeq->m_pEnvTone[0];
			}
			pSeq->m_nEnvTonePos = 1;

			nTone += pSeq->m_nDetune;

			if( nTone < 1 )
			{
				nTone = 1;
			}

			if( nTone > 0xfff )
			{
				nTone = 0xfff;
			}

			nFreqLow = nTone & 0x00ff;
			nFreqHigh = nTone >> 8;

			//. �m�C�Y.
			if( pSeq->m_pEnvNoise && (pSeq->m_nMixer & kMixer_Noise) )
			{
				nNoiseFreq = pSeq->m_pEnvNoise[0];
			}
			pSeq->m_nEnvNoisePos = 1;

			//. �{�����[��.
			nVolume = pSeq->m_nVolume;
			if( pSeq->m_pEnvVolume )
			{
				nVolume += pSeq->m_pEnvVolume[0];
			}
			pSeq->m_nEnvVolumePos = 1;

			pSeq->m_nCount = ((u16) nOpCode & 0x0f) ;
			pSeq->m_pSeqPos++;

			if( pSeq->m_nCount == 15 )
			{
				pSeq->m_nCount = *pSeq->m_pSeqPos;
				pSeq->m_pSeqPos++;
			}
			break;
		}

		//. ���̂ق��̃R�}���h.
		switch( nOpCode )
		{
		case KSQ_TIMEBASE: //. �^�C���x�[�X.
			{
				//. ����`.
				pSeq->m_pSeqPos += 2;
				break;
			}

		case KSQ_O1: //. ��΃I�N�^�[�u.
		case KSQ_O2:
		case KSQ_O3:
		case KSQ_O4:
		case KSQ_O5:
		case KSQ_O6:
		case KSQ_O7:
		case KSQ_O8:
			{
				pSeq->m_nOctave = nOpCode - KSQ_O1;
				pSeq->m_pSeqPos++;
				break;
			}

		case KSQ_O_UP: //. ���΃I�N�^�[�u.
			{
				pSeq->m_nOctave++;
				pSeq->m_pSeqPos++;
				break;
			}

		case KSQ_O_DOWN: //. ���΃I�N�^�[�u.
			{
				pSeq->m_nOctave--;
				pSeq->m_pSeqPos++;
				break;
			}

		case KSQ_TAI: //. �^�C.
			{
				//. ����`.
				pSeq->m_pSeqPos++;
				break;
			}

		case KSQ_TEMPO: //. �e���|.
			{
				//. ����`.
				pSeq->m_pSeqPos += 2;
				break;
			}

		case KSQ_QUANTIZE: //. �N�H���^�C�Y.
			{
				pSeq->m_nQuantize = pSeq->m_pSeqPos[1];
				pSeq->m_pSeqPos += 2;
				break;
			}

		case KSQ_EFFECT_ON: //. �G�t�F�N�g ON.
			{
				pSeq->m_nModeNext = 1;
				pSeq->m_pSeqPos++;
				break;
			}

		case KSQ_EFFECT_OFF: //. �G�t�F�N�g OFF.
			{
				pSeq->m_nModeNext = 0;
				pSeq->m_pSeqPos++;
				break;
			}

		case KSQ_VOLUME: //. ��΃{�����[��.
			{
				pSeq->m_nVolume = pSeq->m_pSeqPos[1];
				pSeq->m_pSeqPos += 2;
				break;
			}
		case KSQ_V_UP: //. ���΃{�����[��.
			{
				pSeq->m_nVolume++;
				pSeq->m_pSeqPos++;
				break;
			}
		case KSQ_V_DOWN: //. ���΃{�����[��.
			{
				pSeq->m_nVolume--;
				pSeq->m_pSeqPos++;
				break;
			}

		case KSQ_DETUNE: //. �f�`���[��.
			{
				pSeq->m_nDetune = (s8) pSeq->m_pSeqPos[1];
				pSeq->m_pSeqPos += 2;
				break;
			}

		case KSQ_EV: //. �G���x���[�v �{�����[��.
			{
				pSeq->m_pEnvVolume = pSeq->m_pEnvVolumePattern[pSeq->m_pSeqPos[1]];
				pSeq->m_pSeqPos += 2;
				break;
			}
			
		case KSQ_ET: //. �G���x���[�v �g�[��.
			{
				pSeq->m_pEnvTone = pSeq->m_pEnvTonePattern[pSeq->m_pSeqPos[1]];
				pSeq->m_pSeqPos += 2;
				break;
			}
		case KSQ_EP: //. �G���x���[�v �s�b�`.
			{
				pSeq->m_pEnvPitchNext = pSeq->m_pEnvPitchPattern[pSeq->m_pSeqPos[1]];
				pSeq->m_pSeqPos += 2;
				break;
			}

		case KSQ_EN: //. �G���x���[�v �m�C�Y.
			{
				pSeq->m_pEnvNoise = pSeq->m_pEnvNoisePattern[pSeq->m_pSeqPos[1]];
				pSeq->m_pSeqPos += 2;
				break;
			}

		case KSQ_NON:
			{
				pSeq->m_nMixer |= kMixer_Noise;
				pSeq->m_pSeqPos++;
				break;
			}

		case KSQ_NOFF:
			{
				pSeq->m_nMixer &= ~kMixer_Noise;
				pSeq->m_pSeqPos++;
				break;
			}

		case KSQ_TON:
			{
				pSeq->m_nMixer |= kMixer_Tone;
				pSeq->m_pSeqPos++;
				break;
			}

		case KSQ_TOFF:
			{
				pSeq->m_nMixer &= ~kMixer_Tone;
				pSeq->m_pSeqPos++;
				break;
			}

		case KSQ_EV_PATTERN: //. �G���x���[�v �{�����[�� �p�^�[��.
			{
				pSeq->m_pEnvVolumePattern[pSeq->m_pSeqPos[1]] = (u8*) (*(u16*) &pSeq->m_pSeqPos[2]);
				pSeq->m_pSeqPos += 4;
				break;
			}

		case KSQ_ET_PATTERN: //. �G���x���[�v �g�[�� �p�^�[��..
			{
				pSeq->m_pEnvTonePattern[pSeq->m_pSeqPos[1]] = (u8*) (*(u16*) &pSeq->m_pSeqPos[2]);
				pSeq->m_pSeqPos += 4;
				break;
			}
		case KSQ_EP_PATTERN: //. �G���x���[�v�s�b�` �p�^�[��..
			{
				pSeq->m_pEnvPitchPattern[pSeq->m_pSeqPos[1]] = (u8*) (*(u16*) &pSeq->m_pSeqPos[2]);
				pSeq->m_pSeqPos += 4;
				break;
			}

		case KSQ_EFFECT_PATTERN: //. �G�t�F�N�g �p�^�[��.
			{
				pSeq->m_pEffectPattern[pSeq->m_pSeqPos[1]] = (u8*) (*(u16*) &pSeq->m_pSeqPos[2]);
				pSeq->m_pSeqPos += 4;
				break;
			}

		case KSQ_EN_PATTERN: //. �m�C�Y �p�^�[��.
			{
				pSeq->m_pEnvNoisePattern[pSeq->m_pSeqPos[1]] = (u8*) (*(u16*) &pSeq->m_pSeqPos[2]);
				pSeq->m_pSeqPos += 4;
				break;
			}

		case KSQ_JUMP: //. �W�����v.
			{
				pSeq->m_pSeqPos = (u8*) (*(u16*)(pSeq->m_pSeqPos + 1));
				break;
			}
		case KSQ_CALL: //. �R�[��.
			{
				pSeq->m_pSeqReturnPos = pSeq->m_pSeqPos + 3;
				pSeq->m_pSeqPos = (u8*) (*(u16*)(pSeq->m_pSeqPos + 1));
				break;
			}
		case KSQ_RETURN: //. ���^�[��.
			{
				pSeq->m_pSeqPos = pSeq->m_pSeqReturnPos;
				break;
			}

		case KSQ_MARK:
			{
				pSeq->m_pSeqPos++;
				pSeq->m_pSeqMarkPos = pSeq->m_pSeqPos;
				break;
			}

		case KSQ_MARK_JUMP:
			{
				pSeq->m_pSeqPos = pSeq->m_pSeqMarkPos;
				break;
			}

		case KSQ_END: //. �I��.
			{
				pSeq->m_pSeqPos = pSeq->m_pSeqStartPos;
				break;
			}

		}
	}

	//. ���W�X�^�ݒ�.
	msxBiosWritePSG( (nChannel << 1),     nFreqLow );
	msxBiosWritePSG( (nChannel << 1) + 1, nFreqHigh );
	msxBiosWritePSG( (nChannel + 8), nVolume );

	if( nNoiseFreq >= 0 )
	{
		msxBiosWritePSG( 6, nNoiseFreq );
	}
//	msxBiosWritePSG( 0, nFreqLow );
//	msxBiosWritePSG( 1, nFreqHigh );
//	msxBiosWritePSG( 8, 15 );

//	msxBiosWriteVRAM( 0x1800 + 25, (u8) *("0123456789ABCDEF" + ((*(pSeq->m_pSeqPos) & 0xf0) >> 4)) );
//	msxBiosWriteVRAM( 0x1800 + 26, (u8) *("0123456789ABCDEF" + (*(pSeq->m_pSeqPos) & 0x0f)) );
}

static struct SSequence s_sequence[3];

//-----------------------------------------------------------------------------
//. ������.
//-----------------------------------------------------------------------------
void msxSoundInitialize()
{
//	msxBiosWritePSG( 7, 0x9C );

	_SequenceClean( &s_sequence[0] );
	_SequenceSetData( &s_sequence[0], bgm0_track0 );

//	s_sequence[0].m_pEnvVolumePattern[1] = s_nEnvVolume;
//	s_sequence[0].m_pEnvTonePattern[0] = s_nEnvTone0;
//	s_sequence[0].m_pEnvTonePattern[1] = s_nEnvTone;

	_SequenceClean( &s_sequence[1] );
	_SequenceSetData( &s_sequence[1], bgm0_track1 );
//	s_sequence[1].m_pEnvVolumePattern[1] = s_nEnvVolume2;
//	s_sequence[1].m_pEnvTonePattern[1] = s_nEnvTone2;

	_SequenceClean( &s_sequence[2] );
	_SequenceSetData( &s_sequence[2], bgm0_track2 );
//	s_sequence[2].m_pEnvVolumePattern[1] = s_nEnvVolume3;
//	s_sequence[2].m_pEnvTonePattern[1] = s_nEnvTone3;
//	s_sequence[2].m_pEnvVolumePattern[2] = s_nEnvVolume4;

}

//-----------------------------------------------------------------------------
//. ���s.
//-----------------------------------------------------------------------------
void msxSoundExecute() __critical
{
	u8 nMixer;

	_SequenceExecute( 0, &s_sequence[0] );
	_SequenceExecute( 1, &s_sequence[1] );
	_SequenceExecute( 2, &s_sequence[2] );

	nMixer = s_sequence[0].m_nMixer | (s_sequence[1].m_nMixer << 1) | (s_sequence[2].m_nMixer << 2);
	nMixer = 0x80 | ((~nMixer) & 0x3f);
	msxBiosWritePSG( 7, nMixer );
}



//------------------------------------------------------------------------[eof]
