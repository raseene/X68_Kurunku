/**************************

		���C��

 **************************/

#include "common.h"
#include "app.h"
#include "sprite.h"
#include "sound.h"
#include "game.h"


/******************
    �t�F�[�h�C��
 ******************/
void	fade_in(void)
{
	CONTRAST(-2);
}

/********************
    �t�F�[�h�A�E�g
 ********************/
void	fade_out(void)
{
	CONTRAST(0);
}


/***************************************************
    �X�v���C�g�ǂݍ���
			����	sp_name  = �X�v���C�g�t�@�C��
					pal_name = �p���b�g�t�@�C��
			�߂�l	����������
 ***************************************************/
Bool	load_sprite(const char* sp_name, const char* pal_name)
{
	uint8_t*	buf;

	if ( (buf = malloc(0x8000)) == NULL ) {
		set_error("������������܂���\n");
		return	FALSE;
	}

	FILE*	fp;
	size_t	size;

	if ( (fp = fopen(sp_name, "rb")) == NULL ) {
		set_error("�p�^�[���t�@�C�� \"%s\" ���J���܂���\n", sp_name);
		free(buf);
		return	FALSE;
	}
	size = fread(buf, 0x80, 0x100, fp);
	fclose(fp);

	for (int i = 0; i < size; i++) {
		SP_DEFCG(i, 1, buf + i*0x80);		// �X�v���C�g��`
	}

	if ( (fp = fopen(pal_name, "rb")) == NULL ) {
		set_error("�p���b�g�t�@�C�� \"%s\" ���J���܂���\n", pal_name);
		free(buf);
		return	FALSE;
	}
	size = fread(buf, sizeof(uint16_t)*0x10, 0x0f, fp);
	fclose(fp);

	for (int j = 0; j < size; j++) {
		for (int i = 0; i < 0x10; i++) {
			SPALET((1 << 31) + i, j + 1, ((uint16_t*)buf)[j*0x10 + i]);		// �p���b�g��`
		}
	}

	free(buf);
	return	TRUE;
}


static
char	error_message[128] = "\0";			// �G���[���b�Z�[�W

/****************************************
    �G���[�ݒ�
		����	err = �G���[���b�Z�[�W
 ****************************************/
void	set_error(const char* mes, ...)
{
	va_list		args;

	va_start(args, mes);
	vsprintf(error_message, mes, args);
	va_end(args);
}


volatile uint16_t	vsync_cnt = 0;			// ���������J�E���^

/************
    ���C��
 ************/
int		main(int argc, char* argv[])
{
	int		crt_mode;						// ���X�̉�ʃ��[�h

	crt_mode = CRTMOD(-1);					// ��ʃ��[�h�ۑ�
	CRTMOD(14);								// 256x256(65536�F)
	B_WPOKE((UWORD*)0xe82500, 0x18e4);		// �v���C�I���e�B�ݒ�
	TGUSEMD(0, 2);							// �O���t�B�b�N��ʎg�p
	TGUSEMD(1, 2);							// �e�L�X�g��ʎg�p
	C_CUROFF();								// �J�[�\����\��
	MS_INIT();								// �}�E�X������
	MS_CUROF();								// �}�E�X�J�[�\����\��
	G_CLR_ON();								// �O���t�B�b�N��ʕ\��

	SP_INIT();								// �X�v���C�g��ʏ�����
	SP_ON();								// �X�v���C�g��ʕ\��
	sprite_init();							// �X�v���C�g�Ǘ�������

	sound_init();							// �T�E���h�Ǘ�������

	srand((unsigned int)TIMEGET());			// ����������


	if ( game_init((argc < 2) ? NULL : argv[1]) ) {				// �Q�[��������
		while ( game_update() ) {			// �Q�[���ғ�
			sprite_terminate();				// �X�v���C�g�ݒ�I��

			rand();
			vsync_cnt = 0;
			while ( vsync_cnt < 1 ) ;		// ��������

			game_draw();					// �Q�[���`��
			sprite_clear();					// �X�v���C�g�o�b�t�@�N���A
		}
	}

	sound_quit();							// �T�E���h�Ǘ��I��
	sprite_quit();							// �X�v���C�g�Ǘ��I��
	clear_text();							// �e�L�X�g��ʃN���A
	TGUSEMD(0, 3);							// �O���t�B�b�N��ʎg�p�ς�
	TGUSEMD(1, 3);							// �e�L�X�g��ʎg�p�ς�
	CRTMOD(crt_mode);						// ��ʃ��[�h���A
	CONTRAST(-2);							// �R���g���X�g��߂�
	C_CURON();								// �J�[�\���\��
	KFLUSHIO(0xff);							// �L�[�o�b�t�@�N���A
	MS_INIT();								// �}�E�X������

	if ( error_message[0] ) {				// �G���[
		puts(error_message);
		return	1;
	}
	return	0;
}
