#ifndef	___BALL_H___
#define	___BALL_H___

#include "common.h"


/********
    ��
 ********/
typedef struct
{
	uint16_t	color;			// �F
	int16_t		x;				// �\�����W
	int16_t		y;

	short	rot_cnt;			// ��]�J�E���^
	short	rot_num;			// ��]�ꏊ
	short	erase_cnt;			// �����J�E���^
	short	fall_cnt;			// �����J�E���^
	short	wait_cnt;			// ��������҂��J�E���^
} Ball;

void	Ball_init_position(void);							// ��]���W�쐬
void	Ball_init(Ball*, int16_t, int16_t, uint16_t);		// ������
Bool	Ball_update(Ball*);									// �ғ�

inline				// �F�ݒ�
void	Ball_set(Ball* this, uint16_t _color)
{
	this->color = _color;
}

inline				// ��]
void	Ball_rotate(Ball* this, short _num, uint16_t _color)
{
	this->color		= _color | 0x30;	// �F
	this->rot_num	= _num;				// ��]�ꏊ
	this->rot_cnt	= 8;				// ��]�p�J�E���^
}

inline				// ����
void	Ball_erase(Ball* this)
{
	if ( this->erase_cnt == 0 ) {
		this->erase_cnt	= 0xff;
		this->wait_cnt	= 0;
	}
}

inline				// ����
void	Ball_fall(Ball* this, uint16_t _color)
{
	this->color		= _color;
	this->fall_cnt	= 6;
	this->color |= 0x30;
}


/************
    ��]��
 ************/
typedef struct
{
	int16_t		x;				// �\�����W
	int16_t		y;
	short		cnt;			// �v�b�V���J�E���^
} Pivot;

void	Pivot_init(Pivot*, int16_t, int16_t);				// ������
void	Pivot_update(Pivot*);								// �ғ�

#endif
