#ifndef	___SOUND_H___
#define	___SOUND_H___

#include "common.h"


void	sound_init(void);				// 初期化
void	sound_quit(void);				// 終了
Bool	load_adpcm(char const*);		// ADPCMデータ読み込み
Bool	load_sound(char const**);		// サウンドデータ読み込み
void	play_bgm(int);					// BGM再生
void	stop_bgm(void);					// BGM停止
void	fadeout_bgm(int);				// BGMフェードアウト
void	play_se(int);					// SE再生

#endif
