/*******************************

		加算エフェクト

 *******************************/

#include "effect.h"
#include "game.h"
#include "sprite.h"


/************
    初期化
 ************/
void	Effect_init(Effect* this)
{
	this->cnt = 0;
}

/***********************************
    設定
		引数	_value = 加算点
				_x, _y = 表示位置
				_d     = ディレイ
 ***********************************/
void	Effect_set(Effect* this, int _value, int16_t _x, int16_t _y, short _d)
{
	this->x = _x;
	this->y = _y;
	this->pat0 = PAL_EFFECT*0x100 + SPR_EFFECT + (_value % 10);
	this->pat1 = _value/10;
	if ( this->pat1 > 0 ) {
		this->pat1 += PAL_EFFECT*0x100 + SPR_EFFECT + 10;
		this->x += 4;
	}
	this->cnt = 50 + 8 + _d*8;
}

/**********
    稼働
 **********/
void	Effect_update(Effect* this)
{
	if ( (this->cnt > 0) && (--this->cnt < 50) ) {
		this->y -= 1;

		set_sprite(this->x, this->y, this->pat0, 0*4);			// スプライト描画
		if ( this->pat1 > 0 ) {
			set_sprite(this->x - 16, this->y, this->pat1, 0*4);
		}
	}
}
