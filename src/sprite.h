#ifndef	___SPRITE_H___
#define	___SPRITE_H___

#include "common.h"


void	set_sprite(int16_t, int16_t, uint16_t, uint16_t);			// �X�v���C�g�ݒ�
void	set_sprite_4(int16_t, int16_t, uint16_t, uint16_t);
void	sprite_terminate(void);										// �I�[����
void	sprite_clear(void);											// �X�v���C�g�o�b�t�@�N���A
void	sprite_init(void);											// �X�v���C�g�Ǘ�������
void	sprite_quit(void);											// �X�v���C�g�Ǘ��I��

#endif
