#ifndef	___GAME_H___
#define	___GAME_H___

#include "common.h"


#define	FIELD_W		6				// �t�B�[���h�̑傫��
#define	FIELD_H		7
#define	BALL_W		20				// ���̑傫��
#define	BALL_H		34

#define	COLOR_MAX	5				// �F��


// BG�ԍ�
enum
{
	BG_SPACE	= 0,							// ��
	BG_NUMBER,									// ����
};

// �X�v���C�g�ԍ�
enum
{
	SPR_BALL	= (BG_NUMBER + 20 + 4 - 1)/4,	// ��
	SPR_PIVOT	= SPR_BALL + 4,					// ��]��
	SPR_CURSOR	= SPR_PIVOT + 3,				// �J�[�\��
	SPR_EFFECT,									// ���Z�G�t�F�N�g
	SPR_START	= SPR_EFFECT + 20,				// "CLICK TO START"
	SPR_OVER	= SPR_START + 6,				// "GAME OVER"
};

// �p���b�g�ԍ�
enum
{
	PAL_NUMBER	= 1,							// ����
	PAL_BALL,									// ��
	PAL_PIVOT	= PAL_BALL + COLOR_MAX*2,		// ��]��
	PAL_START,									// "CLICK TO START"
	PAL_OVER,									// "GAME OVER"
	PAL_CURSOR	= PAL_NUMBER,					// �J�[�\��
	PAL_EFFECT	= PAL_NUMBER,					// ���Z�G�t�F�N�g
};


Bool	game_init(char*);			// ������
Bool	game_update(void);			// �ғ�
void	game_draw(void);			// �`��

#endif