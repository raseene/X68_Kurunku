/*****************************

		ゲームメイン

 *****************************/

#include "game.h"
#include "app.h"
#include "sprite.h"
#include "ball.h"
#include "number.h"
#include "effect.h"
#include "sound.h"
#include <math.h>


#define	FIELD_X		7			// フィールド位置
#define	FIELD_Y		1
#define	HI_SCORE_X	22			// ハイスコア表示位置
#define	HI_SCORE_Y	14
#define	SCORE_X		22			// スコア表示位置
#define	SCORE_Y		20
#define	COMBO_X		27			// コンボ表示位置
#define	COMBO_Y		24
#define	TIMER_X		136			// タイマーバー描画位置
#define	TIMER_Y		224
#define	TIMER_W		112
#define	TIMER_H		12
#define	GAUGE_X		151			// コンボゲージ描画位置
#define	GAUGE_Y		207
#define	GAUGE_W		64
#define	GAUGE_H		5

#define	EFFECT_MAX	8			// エフェクト数

#define	FPS			55			// 秒間フレーム数


/*** 状態 *******/
enum
{
	PHASE_INIT,			// 初期化
	PHASE_START,		// 開始待ち
	PHASE_GAME,			// ゲーム中
	PHASE_OVER,			// ゲームオーバー
};


static Ball		ball[FIELD_W][FIELD_H];					// 球
static Pivot	pivot[FIELD_W - 1][FIELD_H - 1];		// 回転軸

static int			score;					// スコア
static Number		bg_score;				// スコア表示
static int			hi_score;				// ハイスコア
static Number		bg_hi_score;			// ハイスコア表示
static int			combo;					// コンボ数
static int			dec_combo;
static Number		bg_combo;				// コンボ表示
static int			gauge_len;				// コンボゲージの長さ
static uint16_t		forbidden_color;		// 非選択色

static int			timer;					// 残り時間
static int			timer_len;				// 残り時間バーの長さ

static Effect		effect[EFFECT_MAX];		// 加算エフェクト
static int			effect_num;

static uint16_t		warning_color[FPS*2];	// 警告カラー
static int16_t		sin_table[128];			// サインテーブル

static uint16_t		ms_button;				// マウスボタンの状態
static int			push_x;					// 連続プッシュ判定用
static int			push_y;

static int			phase;					// 状態
static int			cnt;					// 汎用カウンタ
static int			exit_cnt;				// 終了カウンタ


/*** サウンドデータ *******/
enum
{
	SND_TONE,			// 音色
	BGM_TITLE,			// タイトルBGM
	BGM_GAME,			// ゲーム中BGM
	SE_ROTATE,			// 回転
	SE_ERASE,			// 消去
};

static const
char*	sound_name[] =
{
	"SOUND\\TONE.ZMD",
	"SOUND\\TITLE3.ZMD",
	"SOUND\\BGM9.ZMD",
	"SOUND\\SJOME007.ZMD",
	"SOUND\\SJOME009.ZMD",
	NULL
};

#define	ADPCM_DATA	"SOUND\\PRETTY.ZPD"


#define	SAVE_FILE	"KURUNKU.SAV"			// セーブファイル
static char		save_name[0x100];			// フルパスセーブファイル名



static Bool		draw_board(void);			// ボード背景描画
static Bool		init_sound(void);			// サウンド初期化
static void		load_play_data(char*);		// プレイデータ読み込み
static void		save_play_data(void);		// プレイデータ書き込み

/*******************************************
    初期化
		引数	_dir = セーブディレクトリ
		戻り値	初期化成功か
 *******************************************/
Bool	game_init(char* _dir)
{
int		apic_load(const char*, int, int);		// PICファイル読み込み

	clear_text();								// テキスト画面クリア
	B_LOCATE(9, 7);
	puts("Now Loading...");

	B_WPOKE((UWORD*)0xe82600, 0x0020);			// グラフィック画面OFF
	if ( apic_load("GRAPHIC\\FRAME.PIC", 0, 12) ) {					// PICファイル描画
		set_error("ファイル \"GRAPHIC\\FRAME.PIC\" が開けません\n");
		return	FALSE;
	}
	{											// 上下帯
		static
		struct FILLPTR	_crop_u = {0,   0, 256,  11, RGB(0, 0, 1)},
						_crop_d = {0, 244, 256, 255, RGB(0, 0, 1)};

		FILL(&_crop_u);
		FILL(&_crop_d);
	}
	if ( !load_sprite("GRAPHIC\\KURUNKU.SP", "GRAPHIC\\KURUNKU.PAL") ) {		// スプライト読み込み
		return	FALSE;
	}
	BGCTRLST(1, 1, 1);
	BGTEXTCL(1, PAL_NUMBER*0x100 + BG_SPACE);	// BG画面クリア
	if ( !init_sound() ) {						// サウンド初期化
		return	FALSE;
	}
	load_play_data(_dir);						// プレイデータ読み込み
	if ( !draw_board() ) {						// ボード背景描画
		return	FALSE;
	}
	{
		static
		struct TXFILLPTR	_field = {0,	8, 20, 120, 216,	0xffff};

		TXFILL(&_field);						// ゲームフィールド背景
	}

	MS_LIMIT(FIELD_X, 20, FIELD_X + FIELD_W*BALL_W - 5, 234);		// マウスの移動範囲


	Ball_init_position();											// 回転座標作成

	for (int i = 0; i < FPS*2; i++) {								// 警告カラー
		warning_color[i] = RGB(8.5 + 12.0*(1.0 - cos(i*PI/FPS))/2, 8, 8);
	}
	for (int i = 0; i < 128; i++) {									// サインテーブル
		sin_table[i] = (int16_t)(256.0*sin(i*PI/64) + 0.5);
	}

	Number_init(&bg_score, 7, SCORE_X, SCORE_Y);					// スコア
	Number_init(&bg_hi_score, 7, HI_SCORE_X, HI_SCORE_Y);			// ハイスコア
	Number_init(&bg_combo, 2, COMBO_X, COMBO_Y);					// コンボ数


	B_WPOKE((UWORD*)0xe82600, 0x006f);			// 画面ON

	phase = PHASE_INIT;
	return	TRUE;
}

#pragma GCC push_options
#pragma GCC optimize("-fno-defer-pop")			// B_SUPER() 対処

/********************************
    スコアボード背景描画
			戻り値	成功したか
 ********************************/
static
Bool	draw_board(void)
{
	uint8_t*	buf;

	if ( (buf = malloc(32 + 32*32/2)) == NULL ) {
		set_error("メモリが足りません\n");
		return	FALSE;
	}

	FILE*	fp;

	if ( (fp = fopen("GRAPHIC\\BOARD.PAT", "rb")) == NULL ) {
		set_error("ファイル \"GRAPHIC\\BOARD.PAT\" が開けません\n");
		free(buf);
		return	FALSE;
	}
	fread(buf, 1, 32 + 32*32/2, fp);
	fclose(fp);


	CONTRAST(0);
	B_WPOKE((UWORD*)0xe82600, 0x0000);			// テキスト画面OFF
	clear_text();								// テキスト画面クリア

	{											// パレット定義
		uint16_t*	p = (uint16_t*)buf;

		for (int i = 0; i < 16; i++) {
			TPALET2(i, *p++);
		}
	}

	int		ssp = B_SUPER(0);

	for (int y = 12; y < 12 + 224; y += 32) {	// スコアボード下地描画
		for (int x = 128; x < 128 + 128; x += 32) {
			uint8_t*	p0 = &buf[16*2];
			uint8_t*	p1 = (uint8_t*)(0xe00000 + x/8 + y*0x80);

			for (int i = 0; i < 4; i++) {
				for (int  k = 0; k < 32; k++) {
					*p1++ = *p0++;
					*p1++ = *p0++;
					*p1++ = *p0++;
					*p1++ = *p0++;
					p1 += 0x80 - 4;
				}
				p1 += 0x20000 - 0x80*32;
			}
		}
	}
	if ( ssp > 0 ) {
		B_SUPER(ssp);
	}

	free(buf);
	return	TRUE;
}

#pragma GCC pop_options

/********************
    サウンド初期化
 ********************/
static
Bool	init_sound(void)
{
	if ( load_adpcm(ADPCM_DATA) && load_sound(sound_name) ) {		// データ読み込み
		play_bgm(SND_TONE);											// 音色設定
		return	TRUE;
	}
	return	FALSE;
}


/***********************************************
    プレイデータ読み込み
			引数	_dir = セーブディレクトリ
 ***********************************************/
static
void	load_play_data(char* _dir)
{
	if ( _dir == NULL ) {
		strcpy(save_name, SAVE_FILE);
	}
	else if ( _dir[strlen(_dir)] == '\\' ) {
		sprintf(save_name, "%s"SAVE_FILE, _dir);
	}
	else {
		sprintf(save_name, "%s\\"SAVE_FILE, _dir);
	}

	FILE*	fp;

	hi_score = 0;
	if ( (fp = fopen(save_name, "rb")) ) {
		fread(&hi_score, sizeof(hi_score), 1, fp);			// プレイデータ読み込み
		fclose(fp);
	}
}

/**************************
    プレイデータ書き込み
 **************************/
static
void	save_play_data(void)
{
	FILE*	fp;

	if ( (fp = fopen(save_name, "wb")) ) {
		fwrite(&hi_score, sizeof(hi_score), 1, fp);			// プレイデータ書き込み
		fclose(fp);
	}
	else {
		set_error("ファイル \"%s\" が作成できません\n");	// メッセージだけで途中終了はしない
	}
}


/******************
    パズル初期化
 ******************/
static
void	init_puzzle(void)
{
	for (int i = 0; i < FIELD_W; i++) {			// 球
		for (int j = 0; j < FIELD_H; j++) {
			uint16_t	_t;

			while (1) {
				_t = (uint16_t)(rand() % COLOR_MAX);
				if ( i % 2 == 0 ) {
					if ( i == 0 ) {
						break;
					}
					if ( (_t != ball[i - 1][j].color) || (((j == 0) || (_t != ball[i][j - 1].color)) && ((j == FIELD_H - 1) || (_t != ball[i - 1][j + 1].color))) ) {
						break;
					}
				}
				else {
					if ( j == 0 ) {
						break;
					}
					if ( (_t != ball[i - 1][j - 1].color) || ((_t != ball[i][j - 1].color) && (_t != ball[i - 1][j].color)) ) {
						break;
					}
				}
			}
			Ball_init(&ball[i][j], FIELD_X + 16 + i*BALL_W, FIELD_Y + 16 + i*(BALL_H/2) + (FIELD_H - 1 - i/2 - j)*BALL_H, _t);
		}
	}

	for (int i = 0; i < FIELD_W - 1; i++) {		// 回転軸
		for (int j = 0; j < FIELD_H - 1; j++) {
			Pivot_init(&pivot[i][j], FIELD_X + 8 + 7 + 16 + i*BALL_W, FIELD_Y - BALL_H/2 + 8 + 16 + ((i + 1) & 1)*BALL_H/2 + (FIELD_H - 1 - j)*BALL_H);
		}
	}

	score		= 0;							// スコア
	Number_set(&bg_score, 0);
	Number_set(&bg_hi_score, hi_score);
	combo		= 0;							// コンボ数
	dec_combo	= 0;
	Number_set(&bg_combo, 0);
	gauge_len	= 0;							// コンボゲージの長さ
	push_x		= -1;							// 連続プッシュ判定用
	forbidden_color	= COLOR_MAX;				// 非選択色

	for (int i = 0; i < EFFECT_MAX; i++) {		// エフェクト
		Effect_init(&effect[i]);
	}
	effect_num = 0;

	timer		= 62*FPS;						// 残り時間
	timer_len	= TIMER_W;						// 残り時間バーの長さ
	{											// タイマーバー
		static
		struct FILLPTR	bar = {TIMER_X, TIMER_Y, TIMER_X + TIMER_W - 1, TIMER_Y, RGB(0, 0, 1)};

		for (int i = 0; i < TIMER_H; i++) {
			bar.y1 = bar.y2 = TIMER_Y + i;
			bar.color = RGB(8 - i*4/TIMER_H, 16 - i*8/TIMER_H, 31 - i*16/TIMER_H);
			FILL(&bar);
		}
	}

	ms_button	= 0;							// マウスボタンの状態
	cnt			= 0;							// アニメーションカウンタ
	exit_cnt	= -1;							// フェードイン予約

	play_bgm(BGM_TITLE);						// タイトルBGM
}


static void		control(void);				// 操作
static Bool		check_erase(void);			// 消去チェック
static void		check_fall(void);			// 落下チェック
static void		select_color();				// 非選択色
static void		draw_mes_start(void);		// スタートメッセージ描画
static void		draw_mes_over(void);		// ゲームオーバーメッセージ描画

/****************************
    稼働
		戻り値	実行継続か
 ****************************/
Bool	game_update(void)
{
	if ( phase == PHASE_INIT ) {
		init_puzzle();							// パズル初期化
		phase = PHASE_START;
	}


	if ( timer == 20*FPS ) {					// 場に最も少ない色の球が出なくなる
		select_color();
	}

	Bool	_f = FALSE;

	check_erase();								// 消去チェック
	check_fall();								// 落下チェック
	control();									// 操作
	{											// 球
		Ball*	_ball = &ball[0][0];

		for (int i = FIELD_W*FIELD_H; i > 0; i--) {
			if ( Ball_update(_ball++) ) {
				_f = TRUE;
			}
		}
	}
	{											// 回転軸
		Pivot*	_pivot = &pivot[0][0];

		for (int i = (FIELD_W - 1)*(FIELD_H - 1); i > 0; i--) {
			Pivot_update(_pivot++);
		}
	}
	{											// エフェクト
		Effect*		_effect = &effect[0];

		for (int i = EFFECT_MAX; i > 0; i--) {
			Effect_update(_effect++);
		}
	}

	if ( (timer == 0) && (phase == PHASE_GAME) && !_f ) {
		phase = PHASE_OVER;						// ゲームオーバー
		cnt = 0;
		exit_cnt = 0;

		if ( score > hi_score ) {				// ハイスコア更新
			hi_score = score;
			save_play_data();
		}
	}

	if ( !_f && (dec_combo > 0) ) {				// コンボゲージ減算
		dec_combo--;
	}
	if ( bg_combo.value < combo ) {				// コンボ表示
		if ( --bg_combo.cnt == 0 ) {
			bg_combo.value++;
			bg_combo.cnt = 5;
		}
	}
	else {
		bg_combo.value = combo;
	}
	Number_update(&bg_combo);					// コンボ数

	if ( bg_score.cnt > 0 ) {					// スコアアップ
		bg_score.value += (score - bg_score.value + bg_score.cnt - 1)/bg_score.cnt;
		bg_score.cnt--;
	}
	Number_update(&bg_score);					// スコア
	if ( bg_score.value > bg_hi_score.value ) {
		bg_hi_score.value = bg_score.value;		// ハイスコア更新
	}
	Number_update(&bg_hi_score);				// ハイスコア

	if ( (phase == PHASE_GAME) && (timer > 0) ) {			// 残り時間
		if ( --timer == 0 ) {
			fadeout_bgm(30);
		}
	}


	switch ( phase ) {
	  case PHASE_START :			// 開始待ち
		cnt++;
		{
			uint16_t	_btn = (uint16_t)MS_GETDT();		// マウスボタン

			if ( ms_button == 0 ) {
				draw_mes_start();				// スタートメッセージ描画
				if ( _btn ) {
					stop_bgm();
				}
			}
			else if ( _btn == 0 ) {
				play_bgm(BGM_GAME);
				phase = PHASE_GAME;				// ゲーム開始
			}
			if ( cnt > 30 ) {
				ms_button = _btn;
			}
		}
		break;

	  case PHASE_OVER :				// ゲームオーバー
		cnt++;
		draw_mes_over();
		if ( exit_cnt == 0 ) {
			uint16_t	_btn = (uint16_t)MS_GETDT();		// マウスボタン

			if ( (cnt > FPS*3) && (~_btn & ms_button) ) {
				stop_bgm();
				fade_out();
				exit_cnt = 30;
			}
			ms_button = _btn;
		}
		else if ( --exit_cnt == 0 ) {
			phase = PHASE_INIT;
		}
		break;
	}

	return	((BITSNS(0) & 0x02) == 0);			// ESCキー
}

/**********
    操作
 **********/
static
void	control(void)
{
	if ( (timer == 0) && (phase != PHASE_START) ) {
		return;
	}

	int		_x, _y;

	_y = MS_CURGT();							// マウスカーソル座標
	_x = _y >> 16;
	_y &= 0xffff;
	set_sprite(_x + 14, _y + 15, PAL_CURSOR*0x100 + SPR_CURSOR, 0*4);		// カーソル表示

	if ( phase != PHASE_GAME ) {
		return;
	}

	uint16_t	_btn = (uint16_t)MS_GETDT(),	// マウスボタン
				_tmp;

	_tmp = _btn & ~ms_button;
	ms_button = _btn;
	if ( _tmp ) {
		_x -= FIELD_X + 16 + 7 - BALL_W/2;
		if ( (_x >= 0) && (_x < BALL_W*(FIELD_W - 1)) ) {
			_x /= BALL_W;

			_y = FIELD_Y + 16 + (FIELD_H - 1)*BALL_H + ((_x + 1) & 1)*BALL_H/2 - _y;
			if ( (_y >= 0) && (_y < BALL_H*(FIELD_H - 1)) ) {
				_y /= BALL_H;

				Ball	*_b0 = &ball[_x][_y + (_x & 1)],
						*_b1 = &ball[_x + 1][_y];

				if ( _b0->erase_cnt | _b1->erase_cnt | (_b1 + 1)->erase_cnt ) {
					return;						// 回転できない球がある
				}

				play_se(SE_ROTATE);				// SE再生

				if ( _tmp & 0xff00 ) {			// 右回転
					_tmp = _b0->color;
					Ball_rotate(_b0, 3, _b1->color);
					Ball_rotate(_b1, 1, (_b1 + 1)->color);
					Ball_rotate((_b1 + 1), 5, _tmp);
				}
				else {							// 左回転
					_tmp = _b0->color;
					Ball_rotate(_b0, 0, (_b1 + 1)->color);
					Ball_rotate((_b1 + 1), 4, _b1->color);
					Ball_rotate(_b1, 2, _tmp);
				}

				if ( (_x != push_x) || (_y != push_y) ) {
					if ( dec_combo > 0 ) {		// コンボゲージ減算
						dec_combo -= 16;
						if ( dec_combo < 0 ) {
							dec_combo = 0;
						}
					}
					else if ( combo > 0 ) {		// コンボ減算
						combo--;
					}
					push_x = _x;
					push_y = _y;
				}
				pivot[_x][_y].cnt = 5;			// 回転軸
			}
		}
	}
}


/*********************************************
    スコア加算
		引数	_x, _y = エフェクト表示位置
				_t     = エフェクトディレイ
		戻り値	スコア加算値
 *********************************************/
static
uint32_t	add_score(int16_t _x, int16_t _y, short _t)
{
	if ( combo < 99 ) {
		combo++;
		bg_combo.cnt = 1;
	}
	dec_combo = dec_combo*4/5 + 36;

	uint32_t	_d = (uint32_t)(combo*10);						// 加算点

	Effect_set(&effect[effect_num], combo, _x, _y, _t);			// 加算エフェクト
	effect_num = ++effect_num % EFFECT_MAX;

	return	_d;
}

/************************************
    消去チェック
		戻り値	球の消去があったか
 ************************************/
static
Bool	check_erase(void)
{
	uint32_t	d_score = 0;
	short		d = 0;
	Ball*		b0 = &ball[FIELD_W - 1][FIELD_H - 2];

	for (short i = FIELD_W - 1; i >= 0; i--) {
		for (short j = FIELD_H - 2, k = j + 1 - (i & 1); j >= 0; j--, k--) {
			uint16_t	t = b0->color;

			if ( !(t & 0xe8) && (t == (b0 + 1)->color) ) {		// 空白・稼働中を除く
				if ( i > 0 ) {
					Ball*	b2 = &ball[i - 1][k];

					if ( (t == b2->color) && (((b0->wait_cnt | (b0 + 1)->wait_cnt | b2->wait_cnt) == 0) || ((b0->erase_cnt | (b0 + 1)->erase_cnt | b2->erase_cnt) > 8)) ) {
						Ball_erase(b0);
						Ball_erase(b0 + 1);
						Ball_erase(b2);
						if ( (b0->erase_cnt | (b0 + 1)->erase_cnt | b2->erase_cnt) == 0xff ) {
							d_score += add_score((int16_t)(FIELD_X - BALL_W*2/5 + 19 + i*BALL_W), (int16_t)(FIELD_Y - BALL_H/2 + 16 + (FIELD_H - 1)*BALL_H + i*(BALL_H/2) - (i/2 + j)*BALL_H), d++);
						}
					}
				}
				if ( i < FIELD_W - 1 ) {
					Ball*	b2 = &ball[i + 1][k];

					if ( (t == b2->color) && (((b0->wait_cnt | (b0 + 1)->wait_cnt | b2->wait_cnt) == 0) || ((b0->erase_cnt | (b0 + 1)->erase_cnt | b2->erase_cnt) > 8)) ) {
						Ball_erase(b0);
						Ball_erase(b0 + 1);
						Ball_erase(b2);
						if ( (b0->erase_cnt | (b0 + 1)->erase_cnt | b2->erase_cnt) == 0xff ) {
							d_score += add_score((int16_t)(FIELD_X + BALL_W*2/5 + 19 + i*BALL_W), (int16_t)(FIELD_Y - BALL_H/2 + 16 + (FIELD_H - 1)*BALL_H + i*(BALL_H/2) - (i/2 + j)*BALL_H), d++);
						}
					}
				}
			}
			b0--;
		}
		b0--;
	}
	if ( d_score > 0 ) {						// 消去があった
		score += d_score;
		bg_score.cnt = 20;
		play_se(SE_ERASE);						// SE再生
		return	TRUE;
	}
	return	FALSE;
}


/******************
    落下チェック
 ******************/
static
void	check_fall(void)
{
	Ball*	b = &ball[0][0];

	for (int i = FIELD_W; i > 0; i--) {
		for (int j = FIELD_H - 1; j > 0; j--) {
			if ( ((b->color & 0xe8) == 0x08) && (((b + 1)->color & 0xe8) == 0) && ((b + 1)->erase_cnt == 0) ) {
				Ball_fall(b, (b + 1)->color);
				Ball_set(b + 1, 0x08);
			}
			b++;
		}
		if ( b->color == 0x08 ) {
			uint16_t	t;
			do {
				t = (uint16_t)(rand() % COLOR_MAX);
			} while ( (t == forbidden_color) || (t == ((b - 1)->color & 0x0f)) );
			Ball_fall(b, t);
		}
		b++;
	}
}

/**************
    非選択色
 **************/
static
void	select_color()
{
	short	num[COLOR_MAX] = {0, 0, 0, 0, 0};
	Ball*	_ball = &ball[0][0];				// 球

	for (int i = FIELD_W*FIELD_H; i > 0; i--) {
		if ( (_ball->color & 0xf8) == 0 ) {
			num[_ball->color]++;
		}
		_ball++;
	}
	short	t = FIELD_W*FIELD_H;
	for (int i = COLOR_MAX - 1; i >= 0; i--) {		// 最も少ない色
		if ( num[i] < t ) {
			forbidden_color = (uint16_t)i;
			t = num[i];
		}
	}
}


/****************************
    スタートメッセージ描画
 ****************************/
static
void	draw_mes_start(void)
{
	int16_t		_y = 108 + 16 + sin_table[cnt % 128]/48;

	for (int i = 0; i < 6; i++) {
		set_sprite(20 + 16 + i*16, _y, PAL_START*0x100 + SPR_START + i, 1*4);
	}
}

/**********************************
    ゲームオーバーメッセージ描画
 **********************************/
static
void	draw_mes_over(void)
{
	static const
	int		pat[] =
			{
				0, 12,			// G
				1, 14,			// A
				2, 13,			// M
				3, 18,			// E
				4, 14,			// O
				5, 12,			// V
				3, 13,			// E
				6, 13,			// R
			};

	int const*	p = pat;
	int16_t		x = 12 + 16;

	for (int i = 0; i < 8; i++) {
		int		t = cnt - i*8 - 120;
		set_sprite(x, 116 + 16 - ((t < 0) ? t*t/8 : (256 + sin_table[(t + 96) % 128])/32), PAL_OVER*0x100 + SPR_OVER + *p++, 0*4);
		x += *p++;
	}
}


#pragma GCC push_options
#pragma GCC optimize("-fno-defer-pop")			// B_SUPER() 対処

/**********
    描画
 **********/
void	game_draw(void)
{
void	timer_bar(int, int);
void	combo_gauge(int, int, uint16_t);

	int		ssp = B_SUPER(0);
	if ( timer < 10*FPS ) {						// 警告
#if 0
		TPALET2(1, warning_color[timer % (2*FPS)]);
#else
		*((uint16_t*)0xe82202) = warning_color[timer % (2*FPS)];
#endif
	}
	{
		Number_draw(&bg_hi_score);				// ハイスコア
		Number_draw(&bg_score);					// スコア
		Number_draw(&bg_combo);					// コンボ
	}

	int		len = (timer*TIMER_W + 62*FPS - 1)/(62*FPS);		// タイマーバー

	if ( len < timer_len ) {
#if 0
		static
		struct FILLPTR	bar = {TIMER_X, TIMER_Y, TIMER_X, TIMER_Y + TIMER_H - 1, 0};

		bar.x1 = TIMER_X + len;
		bar.x2 = TIMER_X + timer_len - 1;
		FILL(&bar);
#else
		timer_bar(len, timer_len - len);
#endif
		timer_len = len;
	}

	len = (dec_combo*GAUGE_W + 180 - 1)/180;					// コンボゲージ

	if ( len > gauge_len ) {
#if 0
		static
		struct FILLPTR	bar = {GAUGE_X, GAUGE_Y, GAUGE_X, GAUGE_Y + GAUGE_H - 1, RGB(31, 24, 0)};

		bar.x1 = GAUGE_X + gauge_len;
		bar.x2 = GAUGE_X + len - 1;
		FILL(&bar);
#else
		combo_gauge(gauge_len, len - gauge_len, RGB(31, 24, 0));
#endif
	}
	else if ( len < gauge_len ) {
#if 0
		static
		struct FILLPTR	bar = {GAUGE_X, GAUGE_Y, GAUGE_X, GAUGE_Y + GAUGE_H - 1, 0};

		bar.x1 = GAUGE_X + len;
		bar.x2 = GAUGE_X + gauge_len - 1;
		FILL(&bar);
#else
		combo_gauge(len, gauge_len - len, 0);
#endif
	}
	gauge_len = len;
	if ( ssp > 0 ) {
		B_SUPER(ssp);
	}

	if ( exit_cnt < 0 ) {
		fade_in();								// フェードイン
		exit_cnt = 0;
	}
}

#pragma GCC pop_options
