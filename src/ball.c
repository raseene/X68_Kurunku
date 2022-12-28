/***************************

		球・回転軸

 ***************************/

#include "ball.h"
#include "game.h"
#include "sprite.h"
#include <math.h>


static int16_t	rot_position[6][8][XY];				// 回転座標

/******************
    回転座標作成
 ******************/
void	Ball_init_position(void)
{
	double	_w = BALL_W*1.7320508/(1.0 + 1.7320508),
			_h = BALL_H/1.7320508;

	for (int i = 0; i < 6; i++) {
		double	_cs = cos(i*4*(PI/12.0)), _sn = sin(i*4*(PI/12.0));

		_w = -_w;
		rot_position[i][0][X] = 0;
		rot_position[i][0][Y] = 0;
		for (int j = 1; j < 8; j++) {
			rot_position[i][j][X] = (int16_t)(_w*(cos((i*4 - j)*(PI/12.0)) - _cs));
			rot_position[i][j][Y] = (int16_t)(_h*(sin((i*4 - j)*(PI/12.0)) - _sn));
		}
	}
}


/*******************************
    初期化
		引数	_x, _y = 位置
				_color = 色
 *******************************/
void	Ball_init(Ball* this, int16_t _x, int16_t _y, uint16_t _color)
{
	this->x = _x;
	this->y = _y;
	this->color = _color;

	this->rot_cnt	= 0;				// 回転カウンタ
	this->erase_cnt	= 0;				// 消去カウンタ
	this->fall_cnt	= 0;				// 落下カウンタ
	this->wait_cnt	= 0;				// 消去判定待ちカウンタ
}

/********************************
    稼働
		戻り値	稼働しているか
 ********************************/
Bool	Ball_update(Ball* this)
{
	int16_t		_x = this->x,
				_y = this->y;
	uint16_t	_pat = PAL_BALL*0x100 + SPR_BALL + (this->color & 0x0f)*0x100,
				_prw = 3*4;
	Bool		_ret = TRUE;

	if ( this->rot_cnt > 0 ) {						// 回転中
		if ( --this->rot_cnt == 0 ) {
			this->color &= 0x0f;
			this->wait_cnt = 10;
		}
		else {
			_x += rot_position[this->rot_num][this->rot_cnt][X];
			_y += rot_position[this->rot_num][this->rot_cnt][Y];
			_prw = 2*4;
		}
		_ret = FALSE;
	}
	else if ( this->erase_cnt > 0 ) {				// 消去中
		if ( this->erase_cnt == 0xff ) {
			this->erase_cnt = 24;
		}
		else if ( --this->erase_cnt == 0 ) {
			this->color = 0x08;
			return	TRUE;
		}
		else if ( this->erase_cnt % 6 > 2 ) {
			return	TRUE;
		}
		_pat += COLOR_MAX*0x100;
	}
	else if ( this->fall_cnt > 0 ) {				// 落下中
		if ( --this->fall_cnt == 0 ) {
			this->color &= 0x0f;
			this->wait_cnt = 14;
		}
		else {
			_y -= this->fall_cnt*BALL_H/6;
		}
	}
	else if ( this->wait_cnt > 0 ) {				// 消去判定待ち（落下）
		this->wait_cnt--;
	}
	else {
		_ret = FALSE;
	}

	if ( !(this->color & 0x08) ) {
		set_sprite_4(_x, _y, _pat, _prw);			// スプライト描画
	}
	return	_ret;
}



/*******************************
    初期化
		引数	_x, _y = 位置
 *******************************/
void	Pivot_init(Pivot* this, int16_t _x, int16_t _y)
{
	this->x		= _x;
	this->y		= _y;
	this->cnt	= 0;
}

/**********
    稼働
 **********/
void	Pivot_update(Pivot* this)
{
	if ( this->cnt > 0 ) {
		this->cnt--;
	}
	set_sprite(this->x, this->y, PAL_PIVOT*0x100 + SPR_PIVOT + (this->cnt + 1)/2, 1*4);			// スプライト描画
}
