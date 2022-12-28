#ifndef	___BALL_H___
#define	___BALL_H___

#include "common.h"


/********
    球
 ********/
typedef struct
{
	uint16_t	color;			// 色
	int16_t		x;				// 表示座標
	int16_t		y;

	short	rot_cnt;			// 回転カウンタ
	short	rot_num;			// 回転場所
	short	erase_cnt;			// 消去カウンタ
	short	fall_cnt;			// 落下カウンタ
	short	wait_cnt;			// 消去判定待ちカウンタ
} Ball;

void	Ball_init_position(void);							// 回転座標作成
void	Ball_init(Ball*, int16_t, int16_t, uint16_t);		// 初期化
Bool	Ball_update(Ball*);									// 稼働

inline				// 色設定
void	Ball_set(Ball* this, uint16_t _color)
{
	this->color = _color;
}

inline				// 回転
void	Ball_rotate(Ball* this, short _num, uint16_t _color)
{
	this->color		= _color | 0x30;	// 色
	this->rot_num	= _num;				// 回転場所
	this->rot_cnt	= 8;				// 回転用カウンタ
}

inline				// 消去
void	Ball_erase(Ball* this)
{
	if ( this->erase_cnt == 0 ) {
		this->erase_cnt	= 0xff;
		this->wait_cnt	= 0;
	}
}

inline				// 落下
void	Ball_fall(Ball* this, uint16_t _color)
{
	this->color		= _color;
	this->fall_cnt	= 6;
	this->color |= 0x30;
}


/************
    回転軸
 ************/
typedef struct
{
	int16_t		x;				// 表示座標
	int16_t		y;
	short		cnt;			// プッシュカウンタ
} Pivot;

void	Pivot_init(Pivot*, int16_t, int16_t);				// 初期化
void	Pivot_update(Pivot*);								// 稼働

#endif
