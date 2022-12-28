/**************************

		メイン

 **************************/

#include "common.h"
#include "app.h"
#include "sprite.h"
#include "sound.h"
#include "game.h"


/******************
    フェードイン
 ******************/
void	fade_in(void)
{
	CONTRAST(-2);
}

/********************
    フェードアウト
 ********************/
void	fade_out(void)
{
	CONTRAST(0);
}


/***************************************************
    スプライト読み込み
			引数	sp_name  = スプライトファイル
					pal_name = パレットファイル
			戻り値	成功したか
 ***************************************************/
Bool	load_sprite(const char* sp_name, const char* pal_name)
{
	uint8_t*	buf;

	if ( (buf = malloc(0x8000)) == NULL ) {
		set_error("メモリが足りません\n");
		return	FALSE;
	}

	FILE*	fp;
	size_t	size;

	if ( (fp = fopen(sp_name, "rb")) == NULL ) {
		set_error("パターンファイル \"%s\" が開けません\n", sp_name);
		free(buf);
		return	FALSE;
	}
	size = fread(buf, 0x80, 0x100, fp);
	fclose(fp);

	for (int i = 0; i < size; i++) {
		SP_DEFCG(i, 1, buf + i*0x80);		// スプライト定義
	}

	if ( (fp = fopen(pal_name, "rb")) == NULL ) {
		set_error("パレットファイル \"%s\" が開けません\n", pal_name);
		free(buf);
		return	FALSE;
	}
	size = fread(buf, sizeof(uint16_t)*0x10, 0x0f, fp);
	fclose(fp);

	for (int j = 0; j < size; j++) {
		for (int i = 0; i < 0x10; i++) {
			SPALET((1 << 31) + i, j + 1, ((uint16_t*)buf)[j*0x10 + i]);		// パレット定義
		}
	}

	free(buf);
	return	TRUE;
}


static
char	error_message[128] = "\0";			// エラーメッセージ

/****************************************
    エラー設定
		引数	err = エラーメッセージ
 ****************************************/
void	set_error(const char* mes, ...)
{
	va_list		args;

	va_start(args, mes);
	vsprintf(error_message, mes, args);
	va_end(args);
}


volatile uint16_t	vsync_cnt = 0;			// 垂直同期カウンタ

/************
    メイン
 ************/
int		main(int argc, char* argv[])
{
	int		crt_mode;						// 元々の画面モード

	crt_mode = CRTMOD(-1);					// 画面モード保存
	CRTMOD(14);								// 256x256(65536色)
	B_WPOKE((UWORD*)0xe82500, 0x18e4);		// プライオリティ設定
	TGUSEMD(0, 2);							// グラフィック画面使用
	TGUSEMD(1, 2);							// テキスト画面使用
	C_CUROFF();								// カーソル非表示
	MS_INIT();								// マウス初期化
	MS_CUROF();								// マウスカーソル非表示
	G_CLR_ON();								// グラフィック画面表示

	SP_INIT();								// スプライト画面初期化
	SP_ON();								// スプライト画面表示
	sprite_init();							// スプライト管理初期化

	sound_init();							// サウンド管理初期化

	srand((unsigned int)TIMEGET());			// 乱数初期化


	if ( game_init((argc < 2) ? NULL : argv[1]) ) {				// ゲーム初期化
		while ( game_update() ) {			// ゲーム稼働
			sprite_terminate();				// スプライト設定終了

			rand();
			vsync_cnt = 0;
			while ( vsync_cnt < 1 ) ;		// 垂直同期

			game_draw();					// ゲーム描画
			sprite_clear();					// スプライトバッファクリア
		}
	}

	sound_quit();							// サウンド管理終了
	sprite_quit();							// スプライト管理終了
	clear_text();							// テキスト画面クリア
	TGUSEMD(0, 3);							// グラフィック画面使用済み
	TGUSEMD(1, 3);							// テキスト画面使用済み
	CRTMOD(crt_mode);						// 画面モード復帰
	CONTRAST(-2);							// コントラストを戻す
	C_CURON();								// カーソル表示
	KFLUSHIO(0xff);							// キーバッファクリア
	MS_INIT();								// マウス初期化

	if ( error_message[0] ) {				// エラー
		puts(error_message);
		return	1;
	}
	return	0;
}
