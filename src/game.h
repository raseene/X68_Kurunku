#ifndef	___GAME_H___
#define	___GAME_H___

#include "common.h"


#define	FIELD_W		6				// フィールドの大きさ
#define	FIELD_H		7
#define	BALL_W		20				// 球の大きさ
#define	BALL_H		34

#define	COLOR_MAX	5				// 色数


// BG番号
enum
{
	BG_SPACE	= 0,							// 空白
	BG_NUMBER,									// 数字
};

// スプライト番号
enum
{
	SPR_BALL	= (BG_NUMBER + 20 + 4 - 1)/4,	// 球
	SPR_PIVOT	= SPR_BALL + 4,					// 回転軸
	SPR_CURSOR	= SPR_PIVOT + 3,				// カーソル
	SPR_EFFECT,									// 加算エフェクト
	SPR_START	= SPR_EFFECT + 20,				// "CLICK TO START"
	SPR_OVER	= SPR_START + 6,				// "GAME OVER"
};

// パレット番号
enum
{
	PAL_NUMBER	= 1,							// 数字
	PAL_BALL,									// 球
	PAL_PIVOT	= PAL_BALL + COLOR_MAX*2,		// 回転軸
	PAL_START,									// "CLICK TO START"
	PAL_OVER,									// "GAME OVER"
	PAL_CURSOR	= PAL_NUMBER,					// カーソル
	PAL_EFFECT	= PAL_NUMBER,					// 加算エフェクト
};


Bool	game_init(char*);			// 初期化
Bool	game_update(void);			// 稼働
void	game_draw(void);			// 描画

#endif