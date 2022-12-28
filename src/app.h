#ifndef	___APP_H___
#define	___APP_H___


void	fade_in(void);								// フェードイン
void	fade_out(void);								// フェードアウト
void	clear_text(void);							// テキスト画面クリア
Bool	load_sprite(const char*, const char*);		// スプライト読み込み
void	set_error(const char*, ...);				// エラー設定

#endif
