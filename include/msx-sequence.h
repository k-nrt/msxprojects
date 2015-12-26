//-----------------------------------------------------------------------------
//. シーケンスの定義..
//-----------------------------------------------------------------------------
#ifndef __MSX_SEQUENCE_H__
#define __MSX_SEQUENCE_H__

//. 休符.
#define KSQ_R		0x00 //. 1.

//. 音符.
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

//. タイムベース.
#define KSQ_TIMEBASE	0xD0 //. 2.

//. オクターブ.
#define KSQ_O1		0xD1 //. 1.
#define KSQ_O2		0xD2 //. 1.
#define KSQ_O3		0xD3 //. 1.
#define KSQ_O4		0xD4 //. 1.
#define KSQ_O5		0xD5 //. 1.
#define KSQ_O6		0xD6 //. 1.
#define KSQ_O7		0xD7 //. 1.
#define KSQ_O8		0xD8 //. 1.

#define KSQ_O_UP		0xD9 //. 1. 相対オクターブ
#define KSQ_O_DOWN		0xDA //. 1.

#define KSQ_TAI			0xDB //. 1. タイ.

#define KSQ_TEMPO		0xDC //. 2. テンポ.
#define KSQ_QUANTIZE	0xDD //. 2. クォンタイズ.

#define KSQ_EFFECT_ON	0xDE //. 1. //. エフェクトモード ON/OFF.
#define KSQ_EFFECT_OFF	0xDF //. 1.

#define KSQ_VOLUME		0xE0 //. 2. ボリューム.
#define KSQ_V_UP        0xE1 //. 1. 相対ボリューム.
#define KSQ_V_DOWN		0xE2 //. 1.

#define KSQ_DETUNE		0xE3	//. 2. デチューン.

#define KSQ_EV			0xE4 //. 2. ボリューム設定.
#define KSQ_ET			0xE5 //. 2. 周波数設定.
#define KSQ_EP			0xE6 //. 2. ノート設定.
#define KSQ_EN			0xE7 //. 2. ノイズ設定.

#define KSQ_NON			0xE8 //. 1. ノイズオン.
#define KSQ_NOFF		0xE9 //. 1. ノイズオフ.
#define KSQ_TON			0xEA //. 1. トーンオン.
#define KSQ_TOFF		0xEB //. 1. トーンオフ.

#define KSQ_NOISE		0xEC //. 2. ノイズ周波数変更.

#define KSQ_EV_PATTERN			0xF0 //. 4. ボリュームパターン定義.
#define KSQ_ET_PATTERN			0xF1 //. 4. 周波数パターン定義.
#define KSQ_EP_PATTERN			0xF2 //. 4. ノートパターン定義.
#define KSQ_EN_PATTERN			0xF3 //. 4. ノイズパターン定義.
#define KSQ_EFFECT_PATTERN		0xF4 //. 4. エフェクト定義.

#define KSQ_JUMP				0xF5 //. 3.
#define KSQ_CALL				0xF6 //. 3.
#define KSQ_RETURN				0xF7 //. 1.

#define KSQ_MARK				0xF8 //. 1. マークセット.
#define KSQ_MARK_JUMP			0xF9 //. 1. マークジャンプ.

#define KSQ_EXT					0xFE //. 1. 拡張命令.
#define KSQ_END					0xFF //. 1.

#define KENV_LOOP		126
#define KENV_END		127

#endif
//------------------------------------------------------------------------[eof]
