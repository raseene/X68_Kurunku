# くるくるアクションパズル　くるんくる～ぱ

X68000用のお手軽アクションパズルゲームです。
動作確認はエミュレータ（XM6 TypeG）上で行っており、実機での動作は未確認です。

![Screenshot](https://raseene.asablo.jp/blog/img/2022/12/27/661034.png)

## 遊び方

球の間の回転軸をクリックすると、その周りの３つの球がに回転して位置を入れ替えます（左クリックで右回転、右クリックで左回転）。
このとき同じ色の球が三角に並ぶとそれらの球は消え、空いた所には上の球が落ちてきます。
そして、また三角に並べばそれらの球も消えます。

球を素早く、少ない回数の回転で続けて消すとCOMBOが上昇して、球を消したときに入る点数がアップします。

約60秒の制限時間が経過するとゲーム終了です。


## 起動方法

ZMUSIC、またはZMSCを多重割り込み対応モードで常駐させます。

```
ZMUSIC -M
 または
ZMSC -M
```

KURUNKU.Xを実行します。
必要なファイルは以下の通りです。

```
KURUNKU.X
GRAPHIC\KURUNKU.SP
GRAPHIC\KURUNKU.PAL
GRAPHIC\FRAME.PIC
GRAPHIC\BOARD.PAT
SOUND\PRETTY.ZPD
SOUND\TONE.ZMD
SOUND\BGM9.ZMD
SOUND\TITLE3.ZMD
SOUND\SJOME007.ZMD
SOUND\SJOME009.ZMD
```

ESCキーでゲームを終了します。


カレントディレクトリにセーブデータ"KURUNKU.SAV"を作成します。
実行時の引数でセーブディレクトリを指定することができます。

```
KURUNKU ディレクトリ名
```

## 開発環境

yosshinさんのxdev68k( https://github.com/yosshin4004/xdev68k )を使用させていただきました。
コンパイルする場合は上記環境を構築後、bashコンソール上の srcディレクトリで makeを実行してください。

```
cd src
make
```


## 謝辞

各種データに、月刊電脳倶楽部より素材を使用させていただきました。

・フォント
MSY.さん        第110号　第125号　第145号

・タイル、マウスカーソル
ポチ丸さん      第110号　第128号

・ＢＧＭ
松本健一さん    第111号

・効果音
ＳＪＯＭさん    第115号

どうもありがとうございます。

## ライセンス

このプロジェクトのソースコードは、以下のライセンスの下に公開されています。

* MIT License(http://opensource.org/licenses/MIT)

