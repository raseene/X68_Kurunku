#ifndef	___NUMBER_H___
#define	___NUMBER_H___

#include "common.h"


/************
    ���lBG
 ************/
typedef struct
{
	uint32_t	value;			// ���l
	short		keta;			// ����
	uint16_t*	addr;			// �`��A�h���X
	uint16_t	buf[8][2];		// BG�o�b�t�@
	short		cnt;			// �ėp�J�E���^
} Number;


void	Number_init(Number*, short, int16_t, int16_t);		// ������
void	Number_set(Number*, uint32_t);						// ���l�ݒ�
void	Number_update(Number*);								// �ғ�
void	Number_draw(Number*);								// �`��

#endif
