#ifndef	___EFFECT_H___
#define	___EFFECT_H___

#include "common.h"


/********************
    ���Z�G�t�F�N�g
 ********************/
typedef struct
{
	int16_t		x;			// �\�����W
	int16_t		y;
	uint16_t	pat0;		// �X�v���C�g
	uint16_t	pat1;
	short		cnt;		// �J�E���^
} Effect;


void	Effect_init(Effect*);									// ������
void	Effect_set(Effect*, int, int16_t, int16_t, short);		// �ݒ�
void	Effect_update(Effect*);									// �ғ�

#endif
