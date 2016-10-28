/*入力オブジェクトファイル
入力ファイルを増やすときはここに追加
INPUT(bootpack.o nasmfunc.o)
*/

/*出力オブジェクトファイル
OUTPUT(bootpack.bim)
*/

/*エントリーポイントファイル
これを設定するとstartupルーチンが.textセクションの先頭にくる
STARTUP(startup.o)
*/

/*出力フォーマット
"binary"でtext,data,bssだけのシンプルな構成になる。*/
OUTPUT_FORMAT(binary)

/*セクション*/
SECTIONS
{
  /*textセクションのメモリ上での開始位置*/
  . = 0x280000;
  .text : 
  {
    /*textセクションを全部*/
    *(.text);
    /*textのサイズをシンボルに代入*/
    _b_tsize = SIZEOF(.text);
  }

  /*dataセクションのメモリ上での開始位置*/
  .data 0x310000 :

  /*dataセクションのファイル上の開始位置*/
  AT ( ADDR(.text) + SIZEOF(.text))
  {
    /*.dataのスタートアドレスをシンボルに代入*/
    _b_dstart = SIZEOF(.text) ;
    /*.dataセクションを全部*/
    *(.data);
    /*.dataのサイズをシンボルに代入*/
    _b_dsize = SIZEOF(.data);
  }

  /*bssセクションのメモリ上での開始位置
  ディフォルトではdataの終端から*/
  .bss ( 0x310000 + SIZEOF (.data) ) :
  {
    *(.bss);
    _b_bsssize = SIZEOF(.bss);
  }
}

/*
  NOTE
  リンカスクリプト内のシンボル
  各シンボルはstartup.oに参照される。
    _b_tsize  .textセクションのサイズ
    _b_dstart .dataセクションの開始位置
    _b_dsize  .dataセクションのサイズ
    _b_bsssize    .bssセクションのサイズ
  *bimヘッダの定義ではbssセクションのサイズは64bitであるが、
  quad命令がいつもエラーるので32bitにしてむりやり64bit化している。
  もしもbssセクションのサイズが32bitを超える仕様のモノを作るとしたら
  再考の余地がある（そんなデカい必要は無いと思うが）
*/
