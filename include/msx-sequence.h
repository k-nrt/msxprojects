//-----------------------------------------------------------------------------
//. �V�[�P���X�̒�`..
//-----------------------------------------------------------------------------
#ifndef __MSX_SEQUENCE_H__
#define __MSX_SEQUENCE_H__

//. �x��.
#define KSQ_R		0x00 //. 1.

//. ����.
#define KSQ_C		0x10 //. 1.
#define KSQ_CS		0x20 //. 1.
#define KSQ_D		0x30 //. 1.
#define KSQ_DS		0x40 //. 1.
#define KSQ_E		0x50 //. 1.
#define KSQ_F		0x60 //. 1.
#define KSQ_FS		0x70 //. 1.
#define KSQ_G		0x80 //. 1.
#define KSQ_GS		0x90 //. 1.
#define KSQ_A		0xA0 //. 1.
#define KSQ_AS		0xB0 //. 1.
#define KSQ_B		0xC0 //. 1.

//. �^�C���x�[�X.
#define KSQ_TIMEBASE	0xD0 //. 2.

//. �I�N�^�[�u.
#define KSQ_O1		0xD1 //. 1.
#define KSQ_O2		0xD2 //. 1.
#define KSQ_O3		0xD3 //. 1.
#define KSQ_O4		0xD4 //. 1.
#define KSQ_O5		0xD5 //. 1.
#define KSQ_O6		0xD6 //. 1.
#define KSQ_O7		0xD7 //. 1.
#define KSQ_O8		0xD8 //. 1.

#define KSQ_O_UP		0xD9 //. 1. ���΃I�N�^�[�u
#define KSQ_O_DOWN		0xDA //. 1.

#define KSQ_TAI			0xDB //. 1. �^�C.

#define KSQ_TEMPO		0xDC //. 2. �e���|.
#define KSQ_QUANTIZE	0xDD //. 2. �N�H���^�C�Y.

#define KSQ_EFFECT_ON	0xDE //. 1. //. �G�t�F�N�g���[�h ON/OFF.
#define KSQ_EFFECT_OFF	0xDF //. 1.

#define KSQ_VOLUME		0xE0 //. 2. �{�����[��.
#define KSQ_V_UP        0xE1 //. 1. ���΃{�����[��.
#define KSQ_V_DOWN		0xE2 //. 1.

#define KSQ_DETUNE		0xE3	//. 2. �f�`���[��.

#define KSQ_EV			0xE4 //. 2. �{�����[���ݒ�.
#define KSQ_ET			0xE5 //. 2. ���g���ݒ�.
#define KSQ_EP			0xE6 //. 2. �m�[�g�ݒ�.
#define KSQ_EN			0xE7 //. 2. �m�C�Y�ݒ�.

#define KSQ_NON			0xE8 //. 1. �m�C�Y�I��.
#define KSQ_NOFF		0xE9 //. 1. �m�C�Y�I�t.
#define KSQ_TON			0xEA //. 1. �g�[���I��.
#define KSQ_TOFF		0xEB //. 1. �g�[���I�t.

#define KSQ_NOISE		0xEC //. 2. �m�C�Y���g���ύX.

#define KSQ_EV_PATTERN			0xF0 //. 4. �{�����[���p�^�[����`.
#define KSQ_ET_PATTERN			0xF1 //. 4. ���g���p�^�[����`.
#define KSQ_EP_PATTERN			0xF2 //. 4. �m�[�g�p�^�[����`.
#define KSQ_EN_PATTERN			0xF3 //. 4. �m�C�Y�p�^�[����`.
#define KSQ_EFFECT_PATTERN		0xF4 //. 4. �G�t�F�N�g��`.

#define KSQ_JUMP				0xF5 //. 3.
#define KSQ_CALL				0xF6 //. 3.
#define KSQ_RETURN				0xF7 //. 1.

#define KSQ_MARK				0xF8 //. 1. �}�[�N�Z�b�g.
#define KSQ_MARK_JUMP			0xF9 //. 1. �}�[�N�W�����v.

#define KSQ_EXT					0xFE //. 1. �g������.
#define KSQ_END					0xFF //. 1.

#define KENV_LOOP		126
#define KENV_END		127

#endif
//------------------------------------------------------------------------[eof]
