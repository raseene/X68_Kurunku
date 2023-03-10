# m68k-elf-gcc でビルドする Makefile
# run68.exe を実行する都合上、msys か cygwin か WSL 環境から実行すること。
# （msys 以外ではテストできていません）

# 必要な環境変数が定義されていることを確認する。
ifndef XDEV68K_DIR
	$(error ERROR : XDEV68K_DIR is not defined.)
endif

# デフォルトサフィックスを削除
.SUFFIXES:

# 各種コマンド短縮名
CXX = ${XDEV68K_DIR}/m68k-toolchain/bin/m68k-elf-g++
CC = ${XDEV68K_DIR}/m68k-toolchain/bin/m68k-elf-gcc
GAS2HAS = perl ${XDEV68K_DIR}/util/x68k_gas2has.pl -cpu 68000 -inc doscall.inc -inc iocscall.inc
RUN68 = ${XDEV68K_DIR}/run68/run68
HAS = $(RUN68) ${XDEV68K_DIR}/x68k_bin/HAS060.X
HLK = $(RUN68) ${XDEV68K_DIR}/x68k_bin/hlk301.x

# 実行ファイル名
TARGET_FILE = ../KURUNKU.X

# ヘッダ検索パス
INCLUDE_FLAGS = -I${XDEV68K_DIR}/include/xc -I${XDEV68K_DIR}/include/xdev68k


# コンパイルフラグ
COMMON_FLAGS = -m68000 -O3 $(INCLUDE_FLAGS)
CFLAGS = $(COMMON_FLAGS) -Wno-builtin-declaration-mismatch -fcall-used-d2 -fcall-used-a2 -finput-charset=cp932 -fexec-charset=cp932 -fverbose-asm

# *.c ソースファイル
C_SRCS = main.c game.c ball.c number.c effect.c sound.c

# *.has ソースファイル
ASM_SRCS = sprite.has graphics.has

# リンク対象のライブラリファイル
LIBS =\
	APIC_LIB.L \
	ZMUSIC.L \
	${XDEV68K_DIR}/lib/xc/BASLIB.L \
	${XDEV68K_DIR}/lib/xc/CLIB.L \
	${XDEV68K_DIR}/lib/xc/DOSLIB.L \
	${XDEV68K_DIR}/lib/xc/IOCSLIB.L \
	${XDEV68K_DIR}/lib/xc/FLOATFNC.L \
	${XDEV68K_DIR}/lib/m68k_elf/m68000/libgcc.a \

# 中間ファイル生成用ディレクトリ
INTERMEDIATE_DIR = _build

# オブジェクトファイル
OBJS =	$(addprefix $(INTERMEDIATE_DIR)/,$(patsubst %.c,%.o,$(C_SRCS))) \
		$(addprefix $(INTERMEDIATE_DIR)/,$(patsubst %.has,%.o,$(ASM_SRCS)))

# HLK に入力するリンクリスト
HLK_LINK_LIST = $(INTERMEDIATE_DIR)/_lk_list.tmp

# デフォルトのターゲット
all : $(TARGET_FILE)

# 中間生成物の削除
clean : 
	rm -f $(TARGET_FILE)
	rm -rf $(INTERMEDIATE_DIR)

# 実行ファイルの生成
#	HLK に長いパス文字を与えることは難しい。
#	回避策としてリンク対象ファイルを $(INTERMEDIATE_DIR) 以下にコピーし、
#	短い相対パスを用いてリンクを実行させる。
$(TARGET_FILE) : $(OBJS)
	mkdir -p $(INTERMEDIATE_DIR)
	rm -f $(HLK_LINK_LIST)
	@for FILENAME in $(OBJS); do\
		echo $$FILENAME >> $(HLK_LINK_LIST); \
        done
	@for FILENAME in $(LIBS); do\
		cp $$FILENAME $(INTERMEDIATE_DIR)/`basename $$FILENAME`; \
		echo $(INTERMEDIATE_DIR)/`basename $$FILENAME` >> $(HLK_LINK_LIST); \
        done
	$(HLK) -i $(HLK_LINK_LIST) -o $(TARGET_FILE)

# *.c ソースのコンパイル
$(INTERMEDIATE_DIR)/%.o : %.c makefile
	mkdir -p $(INTERMEDIATE_DIR)
	$(CC) -S $(CFLAGS) -o $(INTERMEDIATE_DIR)/$*.m68k-gas.s $<
	$(GAS2HAS) -i $(INTERMEDIATE_DIR)/$*.m68k-gas.s -o $(INTERMEDIATE_DIR)/$*.s
	rm -f $(INTERMEDIATE_DIR)/$*.m68k-gas.s
	$(HAS) -e -u -w0 $(INCLUDE_FLAGS) $(INTERMEDIATE_DIR)/$*.s -o $(INTERMEDIATE_DIR)/$*.o

# *.has ソースのコンパイル
$(INTERMEDIATE_DIR)/%.o : %.has makefile
	mkdir -p $(INTERMEDIATE_DIR)
	$(HAS) -e -u -w0 $(INCLUDE_FLAGS) $*.has -o $(INTERMEDIATE_DIR)/$*.o


# 依存関係
$(INTERMEDIATE_DIR)/main.o : main.c common.h app.h sprite.h sound.h game.h
$(INTERMEDIATE_DIR)/game.o : game.c common.h game.h app.h sprite.h ball.h number.h effect.h sound.h
$(INTERMEDIATE_DIR)/ball.o : ball.c common.h ball.h game.h sprite.h
$(INTERMEDIATE_DIR)/number.o : number.c common.h number.h game.h
$(INTERMEDIATE_DIR)/effect.o : effect.c common.h effect.h game.h sprite.h
$(INTERMEDIATE_DIR)/sound.o : sound.c common.h sound.h app.h
$(INTERMEDIATE_DIR)/sprite.o : sprite.has
$(INTERMEDIATE_DIR)/graphics.o : graphics.has

