/************************

		数値BG

 ************************/

#include "number.h"
#include "game.h"


/***********************************
    初期化
		引数	_keta  = 桁数
				_x, _y = 描画位置
 ***********************************/
void	Number_init(Number* this, short _keta, int16_t _x, int16_t _y)
{
	this->keta	= _keta;
	this->value	= 0;
	this->addr	= (uint16_t*)(0xebe000 + _x*2 + _y*0x80);		// 描画アドレス
	this->cnt	= 0;
}

/*****************************
    数値設定
		引数	_val = 数値
 *****************************/
void	Number_set(Number* this, uint32_t _val)
{
	this->value = _val;
}


/**********
    稼働
 **********/
void	Number_update(Number* this)
{
	uint16_t*	p = &this->buf[this->keta][0];
	uint32_t	t = this->value;
	short		k = this->keta*2;

	do {
		uint16_t	bg = PAL_NUMBER*0x100 + BG_NUMBER + (t % 10)*2;

		*--p = bg++;
		*--p = bg;
		k -= 2;
		t /= 10;
	} while ( t > 0 ) ;

	for (; k > 0; k--) {
		*--p = PAL_NUMBER*0x100 + BG_SPACE;
	}
}


/**********
    描画
 **********/
void	Number_draw(Number* this)
{
	uint16_t	*p0 = this->addr,
				*p1 = &this->buf[0][0];

	for (short j = this->keta; j > 0; j--) {
		*(p0 + 0x80/2) = *p1++;
		*p0++ = *p1++;
	}
}
