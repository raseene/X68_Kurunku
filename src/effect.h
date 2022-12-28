#ifndef	___EFFECT_H___
#define	___EFFECT_H___

#include "common.h"


/********************
    加算エフェクト
 ********************/
typedef struct
{
	int16_t		x;			// 表示座標
	int16_t		y;
	uint16_t	pat0;		// スプライト
	uint16_t	pat1;
	short		cnt;		// カウンタ
} Effect;


void	Effect_init(Effect*);									// 初期化
void	Effect_set(Effect*, int, int16_t, int16_t, short);		// 設定
void	Effect_update(Effect*);									// 稼働

#endif
