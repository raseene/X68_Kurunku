#ifndef	___SPRITE_H___
#define	___SPRITE_H___

#include "common.h"


void	set_sprite(int16_t, int16_t, uint16_t, uint16_t);			// スプライト設定
void	set_sprite_4(int16_t, int16_t, uint16_t, uint16_t);
void	sprite_terminate(void);										// 終端処理
void	sprite_clear(void);											// スプライトバッファクリア
void	sprite_init(void);											// スプライト管理初期化
void	sprite_quit(void);											// スプライト管理終了

#endif
