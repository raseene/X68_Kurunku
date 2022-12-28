/*****************************

		�T�E���h�Ǘ�

 *****************************/

#include "sound.h"
#include "app.h"
#include <io.h>


int		zm_ver(void);
void	m_init(void);
int		m_stop(int, int, int, int, int, int, int, int, int, int);
int		m_fadeout(int);
int		m_adpcm_block(char const*);


/*** �T�E���h�f�[�^ *******/
typedef struct
{
	uint8_t*	data;			// �f�[�^�A�h���X
	uint8_t*	bgm;			// BGM�A�h���X
	uint8_t*	se;				// SE�A�h���X
} SoundData;

static SoundData*	sound_data = NULL;			// �T�E���h�f�[�^
static Bool			init_flag = FALSE;			// �������t���O


/************
    ������
 ************/
void	sound_init(void)
{
	init_flag = FALSE;
	sound_data = NULL;
	if ( zm_ver() & 0xffff ) {
		init_flag = TRUE;
		m_init();							// ����������
	}
}

/**********
    �I��
 **********/
void	sound_quit(void)
{
	if ( init_flag ) {
		m_stop(0, 0, 0, 0, 0, 0, 0, 0, 0, 0);

		if ( sound_data ) {					// �T�E���h�f�[�^���
			SoundData*	p = sound_data;
			while ( p->data ) {
				free(p->data);
				p++;
			}
			free(sound_data);
		}
	}
}


/*************************************
    ADPCM�f�[�^�ǂݍ���
		����	_file = ZPD�t�@�C��
 *************************************/
Bool	load_adpcm(char const* _file)
{
	if ( init_flag ) {
		if ( m_adpcm_block(_file) ) {
			set_error("�t�@�C�� \"%s\" ���J���܂���\n", _file);
			return	FALSE;
		}
	}
	return	TRUE;
}

/*****************************************
    �T�E���h�f�[�^�ǂݍ���
			����	_file = ZMD�t�@�C��
			�߂�l	�ǂݍ��ݐ�����
 *****************************************/
Bool	load_sound(char const** _file)
{
	if ( !init_flag ) {
		return	TRUE;
	}

	int		n = 0;
	{
		char const**	p = _file;

		while ( *p++ ) {
			n++;							// �f�[�^��
		}
		sound_data = malloc(sizeof(SoundData)*(n + 1));
	}
	for (int i = 0; i < n + 1; i++) {
		sound_data[i].data = NULL;
	}

	for (int i = 0; i < n; i++) {
		FILE*	fp;
		long	size;

		if ( (fp = fopen(_file[i], "rb")) == NULL ) {
			set_error("�T�E���h�t�@�C�� \"%s\" ���J���܂���\n", _file[i]);
			return	FALSE;
		}
		size = filelength(fileno(fp));					// �t�@�C���̑傫��
		if ( (sound_data[i].data = malloc(size)) == NULL ) {
			set_error("������������܂���\n");
			fclose(fp);
			return	FALSE;
		}
		fread(sound_data[i].data, 1, size, fp);			// �ǂݍ���
		fclose(fp);

		sound_data[i].bgm = sound_data[i].data + 7;		// BGM
		uint8_t*	p = sound_data[i].bgm;

		while ( *p++ != 0xff ) ;
		if ( *p == 0xff ) {
			p++;
		}
		sound_data[i].se = p;							// SE
	}
	return	TRUE;
}

/**********************************
    BGM�Đ�
		����	n = �T�E���h�ԍ�
 **********************************/
void	play_bgm(int n)
{
	if ( init_flag ) {
		asm volatile (
			"	movea.l	%0,a1\n"
			"	clr.l	d2\n"
			"	moveq.l	#$11,d1\n"
			"	trap	#3\n"
		:	/* �o�� */
		:	/* ���� */	"irm" (sound_data[n].bgm)	// ���� %0
		:	/* �j�� */	"d0", "d1", "d2", "a1"
		);
	}
}

/************
   BGM��~
 ************/
void	stop_bgm(void)
{
	if ( init_flag ) {
		m_stop(0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
	}
}

/***********************************
   BGM�t�F�[�h�A�E�g
		����	_speed = �X�s�[�h
 ***********************************/
void	fadeout_bgm(int _speed)
{
	if ( init_flag ) {
		m_fadeout(_speed);
	}
}

/**********************************
    SE�Đ�
		����	n = �T�E���h�ԍ�
 **********************************/
void	play_se(int n)
{
	if ( init_flag ) {
		static int	track = 5;

		asm volatile (
			"	movea.l	%0,a1\n"
			"	move.l	%1,d2\n"
			"	moveq.l	#$12,d1\n"
			"	trap	#3\n"
		:	/* �o�� */
		:	/* ���� */	"irm" (sound_data[n].se),	// ���� %0
						"irm" (track)				// ���� %1
		:	/* �j�� */	"d0", "d1", "d2", "a1"
		);

		track = (track % 4) + 5;
	}
}
