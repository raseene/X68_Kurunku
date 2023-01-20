/*******************************

		���Z�G�t�F�N�g

 *******************************/

#include "effect.h"
#include "game.h"
#include "sprite.h"


/************
    ������
 ************/
void	Effect_init(Effect* this)
{
	this->cnt = 0;
}

/***********************************
    �ݒ�
		����	_value = ���Z�_
				_x, _y = �\���ʒu
				_d     = �f�B���C
 ***********************************/
void	Effect_set(Effect* this, int _value, int16_t _x, int16_t _y, short _d)
{
	this->x = _x;
	this->y = _y;
	this->pat0 = PAL_EFFECT*0x100 + SPR_EFFECT + modu(_value, 10);			// DIVU�ň�x�Ɍv�Z����������������
	this->pat1 = divu(_value, 10);											// ���Ղ��D��
	if ( this->pat1 > 0 ) {
		this->pat1 += PAL_EFFECT*0x100 + SPR_EFFECT + 10;
		this->x += 4;
	}
	this->cnt = 50 + 8 + _d*8;
}

/**********
    �ғ�
 **********/
void	Effect_update(Effect* this)
{
	if ( (this->cnt > 0) && (--this->cnt < 50) ) {
		this->y -= 1;

		set_sprite(this->x, this->y, this->pat0, 0*4);			// �X�v���C�g�`��
		if ( this->pat1 > 0 ) {
			set_sprite(this->x - 16, this->y, this->pat1, 0*4);
		}
	}
}
