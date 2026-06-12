
//=======================================================//
//　Z80 Single Board Computer <<<< PC-84C0SD 20MHz >>>>　//
//　 Implemented by Shisuibi --Grand Master Sorcerian--　//
//=======================================================//


★関連動画

　Crystal Palace（PC-84C0SD 20MHz 紹介サイト）
　　https://crystalpalace.web.fc2.com/

　祈宮翠微 YouTube チャンネル
　　https://www.youtube.com/channel/UCD1ukbXROQYn7PtXcsfblfQ

　Homebrew Z80 Single Board Computer on 10cm square PCB designed with KiCad【ESP32】
　　https://youtu.be/RSeWfix4Fm4


★概要

　A Z80-SBC based on a 10cm square PCB designed with KiCad.
　The CPU operates at a Variable clock speed of 4Hz to 20MHz.
　Input/output are handled by Dual ESP32-S3 modules.
　It has a USB wired connection to a PC and a BLE wireless connection to an iPhone.
　It features an IPS-LCD panel, microSD card slot, and an 88-note Melody buzzer, and runs on CP/M 2.2

　入出力にArduino Microを採用した2代目Z80シングル・ボード・コンピュータが、
　ESP32-S3-MINI-1と3.5インチ液晶ディスプレイを搭載して生まれ変わりましたｗ

　2代目同様、KiCadで設計＆PCBWayさん製作の本格的なプリント基板が美しいです
　4代目ではESP32を複数搭載して回路設計の簡略化と実行速度向上を目指しました

　SRAMのSとDualのDでPC-84C0SDと命名しました
　愛称は菜摘（なつみ）Gen4ちゃんです（紫と金の基板が菜の花に見えたから）


★歴代Z80-SBC

　1代目　…　PC-84C0SF 20MHz（FT245RL USBパラレル変換）ブレッドボード
　2代目　…　PC-84C0SA 20MHz（Arduino Micro）プリント基板
　3代目　…　PC-84C0SE 20MHz（ESP32-S3-MINI-1）プリント基板
　4代目　…　PC-84C0SD 20MHz（Dual ESP32-S3-MINI-1）プリント基板


★PC-84C0SD菜摘Gen4

　2代目同様、USBシリアル×2・BLEシリアルによる
　マルチUARTで、PC有線接続×2・iPhone無線接続の
　複製3画面を表示する事が出来ます

　ESP32のPWM機能による4[Hz]～20[MHz]までの
　可変クロックで、発振回路が不要になりました

　2代目で搭載した10セグメントLEDは
　液晶ディスプレイに変更、SDカードと一体化しています


★ダウンロード

　【PC-84C0SD 20MHz　菜摘（なつみ）Gen4】
　　https://github.com/Shisuibi/Z80-SBC_PC-84C0SD_20MHz


★ファイル一覧

　＋―　PC-84C0SD 　　　　　　　　　　　…　PC-84C0SD.zip
　　＋―　Extra 　　　　　　　　　　　　…　番外フォルダ（おまけ）
　　　｜―　DownMaze88.txt　　　　　　　…　DownMazeのPC-8801MAエミュレータ版
　　　｜―　natsumi_gen4.h　　　　　　　…　RunCPMのボード設定ヘッダファイル
　　　｜―　RomDumpList.txt 　　　　　　…　ROMのダンプリスト（IPL・BIOS）
　　＋―　Gerber　　　　　　　　　　　　…　プリント基板の製造データ
　　　｜―　PC-84C0SD-B_Cu.gbr　　　　　…　導体層（裏面・第4層・+5[V]）
　　　｜―　PC-84C0SD-B_Mask.gbr　　　　…　ソルダーマスク（裏面・紫）
　　　｜―　PC-84C0SD-B_Silkscreen.gbr　…　シルクスクリーン（裏面・白）
　　　｜―　PC-84C0SD-Edge_Cuts.gbr 　　…　基板外形（10[cm]×10[cm]）
　　　｜―　PC-84C0SD-F_Cu.gbr　　　　　…　導体層（表面・第1層・GND）
　　　｜―　PC-84C0SD-F_Mask.gbr　　　　…　ソルダーマスク（表面・紫）
　　　｜―　PC-84C0SD-F_Silkscreen.gbr　…　シルクスクリーン（表面・白）
　　　｜―　PC-84C0SD-In1_Cu.gbr　　　　…　導体層（内部・第2層・縦配線）
　　　｜―　PC-84C0SD-In2_Cu.gbr　　　　…　導体層（内部・第3層・横配線）
　　　｜―　PC-84C0SD-NPTH.drl　　　　　…　ドリル（ノンスルーホール）
　　　｜―　PC-84C0SD-PTH.drl 　　　　　…　ドリル（メッキスルーホール）
　　＋―　Hex2Rom 　　　　　　　　　　　…　HEXテキスト→RomImageヘッダ変換ツール
　　　｜―　Hex2Rom.c 　　　　　　　　　…　Hex2Romソースファイル
　　　｜―　Hex2Rom.h 　　　　　　　　　…　Hex2Romヘッダファイル
　　　｜―　Makefile　　　　　　　　　　…　構築ルール記述テキスト
　　＋―　RomImage　　　　　　　　　　　…　ROMのバイナリデータ
　　　｜―　BootRom.ASZ 　　　　　　　　…　ブートローダーのASZソース
　　　｜―　BootRom.HEX 　　　　　　　　…　ブートローダーのHEXテキスト
　　　｜―　BootRom.LST 　　　　　　　　…　ブートローダーのLSTテキスト
　　　｜―　BootRom.txt 　　　　　　　　…　ブートローダーのRomImageヘッダ
　　　｜―　Hex2Rom.exe 　　　　　　　　…　HEXテキスト→RomImageヘッダ変換ツール
　　　｜―　MakeBoot.txt　　　　　　　　…　ブートローダー構築コマンド
　　＋―　SDcard　　　　　　　　　　　　…　SDカードのルートフォルダ
　　　｜―　\A\1　　　　　　　　　　　　…　\A\1フォルダ（MAKEDISKコマンド）
　　　｜―　\B\0　　　　　　　　　　　　…　\B\0フォルダ（BASICファイル）
　　　｜―　BDOS-DR.BIN 　　　　　　　　…　BDOS（DRI）のバイナリファイル
　　　｜―　CCP-Z80.BIN 　　　　　　　　…　CCP（Z80CCP）のバイナリファイル
　　　｜―　CPMDISK.DAT 　　　　　　　　…　CP/M DISKのイメージファイル
　　　｜―　LCDSLOGO.BMP　　　　　　　　…　起動ロゴのビットマップ画像
　　＋―　Sketch　　　　　　　　　　　　…　Arduino IDE開発環境
　　　｜―　Clock.h 　　　　　　　　　　…　定義ファイル（クロック）
　　　｜―　CpmEmu.h　　　　　　　　　　…　定義ファイル（CP/M模倣）
　　　｜―　Function.h　　　　　　　　　…　定義ファイル（基本機能）
　　　｜―　Global.h　　　　　　　　　　…　定義ファイル（広域）
　　　｜―　LgfxUser.hpp　　　　　　　　…　LovyanGFXライブラリの設定ファイル
　　　｜―　Matrix.h　　　　　　　　　　…　定義ファイル（幾何学演算）
　　　｜―　Multi.h 　　　　　　　　　　…　定義ファイル（M/S相互通信）
　　　｜―　PC-84C0SD_Master.c　　　　　…　ESP32スケッチ（拡張子を.inoに変更して下さい）
　　　｜―　PC-84C0SD_Slave.c 　　　　　…　ESP32スケッチ（拡張子を.inoに変更して下さい）
　　　｜―　Reset.h 　　　　　　　　　　…　定義ファイル（リセット）
　　　｜―　RomImage.h　　　　　　　　　…　ROMのバイナリデータ
　　　｜―　SpiLcd.h　　　　　　　　　　…　定義ファイル（液晶表示）
　　　｜―　Trans.h 　　　　　　　　　　…　定義ファイル（シリアル通信）
　　　｜―　Vport.h 　　　　　　　　　　…　定義ファイル（仮想入出力）
　　｜―　LICENSE 　　　　　　　　　　　…　利用許諾書
　　｜―　README.htm　　　　　　　　　　…　説明画像
　　｜―　ReadMe.txt　　　　　　　　　　…　説明書


★プリント基板発注

　PCBWayさんにプリント基板の製造を依頼する際の発注手順です
　予めGerberフォルダ（製造データ）のzipファイルを作成して下さい

　・寸法【100[mm]×100[mm]】を入力します
　　10[cm]×10[cm]（単位はミリメートルなので注意）

　・枚数【5枚】を入力します
　・積層【4層】を入力します

　・導体層レイヤーの並び順を入力します

　　L1【F_Cu】
　　L2【In1_Cu】
　　L3【In2_Cu】
　　L4【B_Cu】

　・レジスト【紫】を入力します
　・シルク【白】を入力します

　・表面処理【無電解金フラッシュ（ENIG）】を入力します
　・製造番号【印刷しない（有料オプション）】を入力します


★RomImage構築

　RomImage.h（ROMのバイナリデータ）を変更する際の構築手順です
　（ROMの内容を変更しない人は読み飛ばして下さい）

　Z80アセンブラZASMをダウンロードします
　（ZASMは64ビットバージョンのWindowsでは動作しません）

　MS-DOS Player for Win32-x64をダウンロードします
　　http://takeda-toshiya.my.coocan.jp/msdos/
　i86_x64フォルダのmsdos.exeをZASMフォルダにコピーします

　cmdを開いて、msdos -c ZASM.exeと打ち込みます
　new_exec_file.exeが作成されるので、名前をZASM64.exeに変更します

　MakeBoot.txtの拡張子をbatに変更します
　（ZASM64.exeが入っているフォルダにPATHを通しておいて下さい）

　MakeBootコマンドを実行すると、BootRom.ASZソースから
　BootRom.HEXテキストとBootRom.LSTテキストが作成されます

　更にHex2Rom（HEXテキスト→RomImageヘッダ変換ツール）が
　呼び出されて、BootRom.HEXからBootRom.txtが作成されます

　出来上がったBootRom.txtをテキストエディタ等で
　切り貼りしてRomImage.h（ROMのバイナリデータ）を作成します


★SDカード作成

　市販のSDカード（SDSC　FAT16　2[GB]推奨）をフォーマットして、
　（SDHC　FAT32　16[GB]は使用可、32[GB]は一応認識するが要注意）

　SDcardフォルダの内容をルートフォルダにコピーします
　\A\1と\B\0はフォルダ構造を変更せずにコピーして下さい

　RunCPMをダウンロードします
　　https://github.com/Shisuibi/RunCPM
　（技術資料 → ★RunCPM導入を参照）

　（PC-84C0SDとRunCPMでSDカードのファイルを共用します）

　DISKフォルダのA0.zipを解凍して生成される\A\0を
　フォルダ構造を変更せずにルートフォルダにコピーします

　CP/M DISKのイメージファイル（CPMDISK.DAT）の中には、
　AドライブにMAKEDISK.COMが最初から入っています

　これはMS-DOSのFAT ⇔ CP/M DISKイメージを相互変換する為の
　菜摘Gen4専用コマンドで、DISK内に必須です（格納場所は不問）

　CP/Mのコマンドラインから「MAKEDISK」「1→Z→Y」の順に操作して、
　RunCPMのフォルダ構造からCP/M DISKイメージを作成します


★Arduino IDE開発環境

　SketchフォルダのPC-84C0SD_Master.cの拡張子を.inoに変更して下さい
　SketchフォルダのPC-84C0SD_Slave.cの拡張子を.inoに変更して下さい

　コメント行の文字化けが気になる人は、原本ファイルの内容を
　メモ帳などで全て選択して、Arduino IDEのエディタ画面で
　各ファイルに貼り付けて下さい

　ライブラリ管理画面からSdFatをインストールします
　ライブラリ管理画面からLovyanGFXをインストールします


★設計仕様

　　　　　　名称　｜　PC-84C0SD 20MHz
　　　　　　愛称　｜　菜摘（なつみ）Gen4

　　中央処理装置　｜　ZiLOG Z80（CMOS版・Z84C00）
　　動作クロック　｜　可変クロック　4・32・256・2K・16K・128K・1M・2.5M・4M・6M・8M・10M・20M[Hz]

　　　主記憶装置　｜　SRAM 512[KB]（32[KB]下位バンク×16ページ）
　　　入出力装置　｜　Dual ESP32-S3-MINI-1（ESP32-S3-DevKitM-1）Master/Slave
　　　　通信装置　｜　USBシリアル×2・BLEシリアル（iPhone側アプリ:AirTerminal）
　　補助記憶装置　｜　SDカード　FAT16／FAT32　2[GB]／32[GB]メディア

　　　設計ソフト　｜　KiCad EDA 9.0.5
　　プリント基板　｜　10[cm]×10[cm]　4層基板（1層:GND 2層:縦配線 3層:横配線 4層:+5[V]）
　　　　表面処理　｜　ソルダーマスク（紫）　シルクスクリーン（白）
　　　銅箔仕上げ　｜　ENIG（無電解ニッケル金フラッシュ）

　ブートローダー　｜　0000H～00FFH（起動時にESP32が直接SRAMへDMA転送）
　 CP/M 2.2 BIOS　｜　F200H～FFFFH（起動時にZ80がESP32からSRAMへ転送）


★部品リスト

　BZ0 　　　　　　｜　電磁ブザー（スピーカー）
　C0　　　　　　　｜　アルミ電解コンデンサ（10[uF]）
　C1・C2　　　　　｜　セラミックコンデンサ（100[nF]）
　H0・H1・H2・H3　｜　PCBスペーサー（M2）2.2[mm]

　J0・J1・J2・J3　｜　ピンソケット（1×22）1×10・1×12を繋いで代用（J2・J3はESP32を基板に直付け）
　　　　　　　　　｜　Espressif Systems　ESP32-S3-MINI-1（ESP32-S3-DevKitM-1）
　J4・J5　　　　　｜　ピンソケット（1×4）　ピンソケット（1×14）1×6・1×8を繋いで代用
　　　　　　　　　｜　ST7796U 3.5インチ 480×320 IPS液晶　FT6336U タッチパネル　SDカード

　M0　　　　　　　｜　ZiLOG　Z84C0020PEG　マイクロプロセッサ　20[MHz]
　M1　　　　　　　｜　Alliance Memory　AS6C4008-55PIN　SRAM　4Mビット（512K×8ビット）

　R0・R1　　　　　｜　金属被膜抵抗器（10[KΩ]）
　RN0 　　　　　　｜　集合抵抗器（10[KΩ]）4素子
　RN1・RN3・RN4 　｜　Bourns Inc　4116R-1-103LF　分離抵抗器（10[KΩ]）8素子
　RN2 　　　　　　｜　Bourns Inc　4116R-1-502LF　分離抵抗器（ 5[KΩ]）8素子
　U0・U1　　　　　｜　Texas Instruments　SN74AHCT32N　論理ORゲート　2入力1出力×4

　C0のアルミ電解コンデンサ（10[uF]）は、液晶画面の中央を支える設計になっています
　（電解コンデンサとピンソケットの長さを慎重に測る必要があります）
　液晶画面の実装部品が干渉する場合は、電解コンデンサを横倒しで取り付けて下さい

　RN2の分離抵抗器（ 5[KΩ]）8素子は分圧の関係で、DMA転送を正常動作させる為に
　RN0の集合抵抗器（10[KΩ]）4素子の半分以下（2～5[KΩ]）の抵抗値を選択して下さい

　実機では4116R-1-202LF　分離抵抗器（2[KΩ]）8素子を採用しています（実機内観の画像を参照）
　これは2[KΩ]しか手に入らなかった為で、5[KΩ]でも正常に動作します（金属被膜抵抗器×8で確認済）


★制御回路

　SRAM_WE　＝　Z84C_WR　＋　Z84C_MREQ
　SRAM_OE　＝　Z84C_RD　＋　Z84C_MREQ

　Z84C_WAIT　　＝　Z84C_IORQ　＋　（ESP32_PioWait　＋　ESP32_WitEnbl）

　入出力命令が実行されると、ESP32側から
　解除されるまでCPUはWAIT状態になります

　SRAM_A15　＝　Z84C_A15　＋　LowBank_P00
　SRAM_A16　＝　Z84C_A15　＋　LowBank_P01
　SRAM_A17　＝　Z84C_A15　＋　LowBank_P02
　SRAM_A18　＝　Z84C_A15　＋　LowBank_P03

　＋　…　OR（論理和）


★入出力ピン配置

　【Master】
　　ENBL　　｜　→　Master/Slave Reset　　GPIO19　｜　→　U1RTS（Arduino IDEからのプログラム転送に使用）
　　GPIO00　｜　→　Z80 CLOCK（PWM）　　　GPIO20　｜　←　U1CTS（Arduino IDEからのプログラム転送に使用）
　　GPIO01　｜　←　Z80 RD　　　　　　　　GPIO21　｜　→　Z80 INT
　　GPIO02　｜　←　Z80 WR　　　　　　　　GPIO26　｜　→　Z80 NMI
　　GPIO03　｜　←　Z80 MREQ　　　　　　　GPIO33　｜　→　PIO Wait Enable
　　GPIO04　｜　←　Z80 IORQ　　　　　　　GPIO34　｜　→　RAM Chip Enable
　　GPIO05　｜　←　Z80 M1　　　　　　　　GPIO35　｜　←　Adrs Bus Bit07
　　GPIO06　｜　←　Z80 RFSH　　　　　　　GPIO36　｜　←　Adrs Bus Bit06
　　GPIO07　｜　←　Z80 BUSACK　　　　　　GPIO37　｜　←　Adrs Bus Bit05
　　GPIO08　｜　←　Z80 HALT　　　　　　　GPIO38　｜　←　Adrs Bus Bit04
　　GPIO09　｜　←　Data Bus Bit00　　　　GPIO39　｜　←　Adrs Bus Bit03
　　GPIO10　｜　←　Data Bus Bit01　　　　GPIO40　｜　←　Adrs Bus Bit02
　　GPIO11　｜　←　Data Bus Bit02　　　　GPIO41　｜　←　Adrs Bus Bit01
　　GPIO12　｜　←　Data Bus Bit03　　　　GPIO42　｜　←　Adrs Bus Bit00
　　GPIO13　｜　←　Data Bus Bit04　　　　GPIO43　｜　→　U0TXD（Tera TermからのPC有線接続に使用）
　　GPIO14　｜　←　Data Bus Bit05　　　　GPIO44　｜　←　U0RXD（Tera TermからのPC有線接続に使用）
　　GPIO15　｜　←　Data Bus Bit06　　　　GPIO45　｜　→　Z80 WAIT
　　GPIO16　｜　←　Data Bus Bit07　　　　GPIO46　｜　→　Z80 RESET
　　GPIO17　｜　→　U1TXD（Serial1 TXD）　GPIO47　｜　→　Z80 BUSREQ
　　GPIO18　｜　←　U1RXD（Serial1 RXD）　GPIO48　｜　→　RGB LED（NeoPixel）

　【Slave】
　　ENBL　　｜　→　Master/Slave Reset　　GPIO19　｜　→　U1RTS（Arduino IDEからのプログラム転送に使用）
　　GPIO00　｜　→　LCD LED（PWM）　　　　GPIO20　｜　←　U1CTS（Arduino IDEからのプログラム転送に使用）
　　GPIO01　｜　←　Z80 RD　　　　　　　　GPIO21　｜　→　Low Bank Page00
　　GPIO02　｜　←　Z80 WR　　　　　　　　GPIO26　｜　→　Low Bank Page01
　　GPIO03　｜　←　Z80 MREQ　　　　　　　GPIO33　｜　→　Low Bank Page02
　　GPIO04　｜　←　Z80 IORQ　　　　　　　GPIO34　｜　→　Low Bank Page03
　　GPIO05　｜　←　Z80 M1　　　　　　　　GPIO35　｜　←　Adrs Bus Bit15
　　GPIO06　｜　←　Z80 RFSH　　　　　　　GPIO36　｜　←　Adrs Bus Bit14
　　GPIO07　｜　←　Z80 BUSACK　　　　　　GPIO37　｜　←　Adrs Bus Bit13
　　GPIO08　｜　←　Z80 HALT　　　　　　　GPIO38　｜　←　Adrs Bus Bit12
　　GPIO09　｜　→　SDC SCS 　　　　　　　GPIO39　｜　←　Adrs Bus Bit11
　　GPIO10　｜　→　LCD SCS 　　　　　　　GPIO40　｜　←　Adrs Bus Bit10
　　GPIO11　｜　→　LCD SDI 　　　　　　　GPIO41　｜　←　Adrs Bus Bit09
　　GPIO12　｜　→　LCD SCK 　　　　　　　GPIO42　｜　←　Adrs Bus Bit08
　　GPIO13　｜　←　LCD SDO 　　　　　　　GPIO43　｜　→　U0TXD（Tera TermからのPC有線接続に使用）
　　GPIO14　｜　→　LCD DCR 　　　　　　　GPIO44　｜　←　U0RXD（Tera TermからのPC有線接続に使用）
　　GPIO15　｜　→　CTP SCL 　　　　　　　GPIO45　｜　←　CTP INT
　　GPIO16　｜　→　CTP SDA 　　　　　　　GPIO46　｜　→　LCD RST・CTP RST
　　GPIO17　｜　→　U1TXD（Serial1 TXD）　GPIO47　｜　→　Buzzer Tone（PWM）
　　GPIO18　｜　←　U1RXD（Serial1 RXD）　GPIO48　｜　→　RGB LED（NeoPixel）


　→　出力　3.3[V]出力から5.0[V]入力へ直結しています
　←　入力　5.0[V]出力から3.3[V]入力へ電圧変換しています　4116R-1-103LF　分離抵抗器（10[KΩ]）8素子


★セグメント表示

　システム情報ビット（SysInfoBit）　　　　　　システム状態ビット（SysModeBit）　　　　　　シリアル送信データ（UartTxData）
　　H C W N I B B B B B 　　　　　　　　　　　　M E C C C C C P M L 　　　　　　　　　　　　S T T T T T T T T B
　　L E E R R 3 2 1 0 A 　　　　　　　　　　　　S C F N S K M H H C 　　　　　　　　　　　　D 7 6 5 4 3 2 1 0 L
　　赤橙黄黄黄緑緑緑空青　　　　　　　　　　　　赤橙黄黄黄緑緑緑空青　　　　　　　　　　　　赤橙黄黄黄緑緑緑空青
　　｜｜｜｜｜｜｜｜｜＋―　CpuCtrBSAK　　　　　｜｜｜｜｜｜｜｜｜＋―　SysLcdMode　　　　　｜｜｜｜｜｜｜｜｜＋―　SysBleUart
　　｜｜｜｜｜｜｜｜＋――　LowBnkPage00　　　　｜｜｜｜｜｜｜｜＋――　CpuPioHist（MREQ）　｜｜｜｜｜｜｜｜＋――　UartTxData00
　　｜｜｜｜｜｜｜＋―――　LowBnkPage01　　　　｜｜｜｜｜｜｜＋―――　CpuPioHist（IORQ）　｜｜｜｜｜｜｜＋―――　UartTxData01
　　｜｜｜｜｜｜＋――――　LowBnkPage02　　　　｜｜｜｜｜｜＋――――　CpuClkMode（_MHz）　｜｜｜｜｜｜＋――――　UartTxData02
　　｜｜｜｜｜＋―――――　LowBnkPage03　　　　｜｜｜｜｜＋―――――　CpuClkMode（_KHz）　｜｜｜｜｜＋―――――　UartTxData03
　　｜｜｜｜＋――――――　CpuIntRqst　　　　　｜｜｜｜＋――――――　CpuClkMode（低速）　｜｜｜｜＋――――――　UartTxData04
　　｜｜｜＋―――――――　CpuNmiRqst　　　　　｜｜｜＋―――――――　CpuClkMode（中速）　｜｜｜＋―――――――　UartTxData05
　　｜｜＋――――――――　PioWitEnbl　　　　　｜｜＋――――――――　CpuClkMode（高速）　｜｜＋――――――――　UartTxData06
　　｜＋―――――――――　RamChpEnbl　　　　　｜＋―――――――――　SysExeCycl　　　　　｜＋―――――――――　UartTxData07
　　＋――――――――――　CpuCtrHALT　　　　　＋――――――――――　SysMsgDisp　　　　　＋――――――――――　SysSdcBusy

　ユーザー任意データ00（AnyNumDt00）　　　　　CPU入出力履歴データ01（PioHistD01） 　　　　CPU入出力履歴データ02（PioHistD02）
　　N N N N N N N N N N 　　　　　　　　　　　　N P P P P P P P P N 　　　　　　　　　　　　N D D D D D D D D N
　　D 7 6 5 4 3 2 1 0 C 　　　　　　　　　　　　B 7 6 5 4 3 2 1 0 A 　　　　　　　　　　　　9 7 6 5 4 3 2 1 0 8
　　赤橙黄黄黄緑緑緑空青　　　　　　　　　　　　赤橙黄黄黄緑緑緑空青　　　　　　　　　　　　赤橙黄黄黄緑緑緑空青
　　｜｜｜｜｜｜｜｜｜＋―　AnyNumDt3C　　　　　｜｜｜｜｜｜｜｜｜＋―　AnyNumDt3A　　　　　｜｜｜｜｜｜｜｜｜＋―　AnyNumDt38
　　｜｜｜｜｜｜｜｜＋――　AnyNumData00　　　　｜｜｜｜｜｜｜｜＋――　AnyNumData10　　　　｜｜｜｜｜｜｜｜＋――　AnyNumData20
　　｜｜｜｜｜｜｜＋―――　AnyNumData01　　　　｜｜｜｜｜｜｜＋―――　AnyNumData11　　　　｜｜｜｜｜｜｜＋―――　AnyNumData21
　　｜｜｜｜｜｜＋――――　AnyNumData02　　　　｜｜｜｜｜｜＋――――　AnyNumData12　　　　｜｜｜｜｜｜＋――――　AnyNumData22
　　｜｜｜｜｜＋―――――　AnyNumData03　　　　｜｜｜｜｜＋―――――　AnyNumData13　　　　｜｜｜｜｜＋―――――　AnyNumData23
　　｜｜｜｜＋――――――　AnyNumData04　　　　｜｜｜｜＋――――――　AnyNumData14　　　　｜｜｜｜＋――――――　AnyNumData24
　　｜｜｜＋―――――――　AnyNumData05　　　　｜｜｜＋―――――――　AnyNumData15　　　　｜｜｜＋―――――――　AnyNumData25
　　｜｜＋――――――――　AnyNumData06　　　　｜｜＋――――――――　AnyNumData16　　　　｜｜＋――――――――　AnyNumData26
　　｜＋―――――――――　AnyNumData07　　　　｜＋―――――――――　AnyNumData17　　　　｜＋―――――――――　AnyNumData27
　　＋――――――――――　AnyNumDt3D　　　　　＋――――――――――　AnyNumDt3B　　　　　＋――――――――――　AnyNumDt39

　Z84C0020 Adrsバス（CpuAdrsBus） 　　　　　　Z84C0020 Portバス（CpuPortBus） 　　　　　　Z84C0020 Dataバス（CpuDataBus）
　　R A A A A A A A A M 　　　　　　　　　　　　I A A A A A A A A M 　　　　　　　　　　　　W D D D D D D D D R
　　F F E D C B A 9 8 1 　　　　　　　　　　　　O 7 6 5 4 3 2 1 0 R 　　　　　　　　　　　　R 7 6 5 4 3 2 1 0 D
　　赤橙黄黄黄緑緑緑空青　　　　　　　　　　　　赤橙黄黄黄緑緑緑空青　　　　　　　　　　　　赤橙黄黄黄緑緑緑空青
　　｜｜｜｜｜｜｜｜｜＋―　CpuCtrM1　　　　　　｜｜｜｜｜｜｜｜｜＋―　CpuCtrMREQ　　　　　｜｜｜｜｜｜｜｜｜＋―　CpuCtrRD
　　｜｜｜｜｜｜｜｜＋――　CpuAdrsBus08　　　　｜｜｜｜｜｜｜｜＋――　CpuAdrsBus00　　　　｜｜｜｜｜｜｜｜＋――　CpuDataBus00
　　｜｜｜｜｜｜｜＋―――　CpuAdrsBus09　　　　｜｜｜｜｜｜｜＋―――　CpuAdrsBus01　　　　｜｜｜｜｜｜｜＋―――　CpuDataBus01
　　｜｜｜｜｜｜＋――――　CpuAdrsBus10　　　　｜｜｜｜｜｜＋――――　CpuAdrsBus02　　　　｜｜｜｜｜｜＋――――　CpuDataBus02
　　｜｜｜｜｜＋―――――　CpuAdrsBus11　　　　｜｜｜｜｜＋―――――　CpuAdrsBus03　　　　｜｜｜｜｜＋―――――　CpuDataBus03
　　｜｜｜｜＋――――――　CpuAdrsBus12　　　　｜｜｜｜＋――――――　CpuAdrsBus04　　　　｜｜｜｜＋――――――　CpuDataBus04
　　｜｜｜＋―――――――　CpuAdrsBus13　　　　｜｜｜＋―――――――　CpuAdrsBus05　　　　｜｜｜＋―――――――　CpuDataBus05
　　｜｜＋――――――――　CpuAdrsBus14　　　　｜｜＋――――――――　CpuAdrsBus06　　　　｜｜＋――――――――　CpuDataBus06
　　｜＋―――――――――　CpuAdrsBus15　　　　｜＋―――――――――　CpuAdrsBus07　　　　｜＋―――――――――　CpuDataBus07
　　＋――――――――――　CpuCtrRFSH　　　　　＋――――――――――　CpuCtrIORQ　　　　　＋――――――――――　CpuCtrWR


★技術資料

　【SRAM下位バンク】
　　512[KB]のSRAMを32[KB]×16ページに分割し、4ビットのページ番号とCPUアドレスビット15の
　　論理和をSRAMアドレスビット15～18とする事で、Z80が持つメモリ空間64[KB]の下位32[KB]を
　　16ページ分のバンクメモリから切り替えて使う事が出来ます（上位32[KB]は常にページ15）

　　CP/M 3.0のマルチタスク機能にも対応可能です

　【Dual ESP32-S3】
　　CPUのCtrlBit・AdrsBus・DataBusの合計32ビットを取り込む際のピン不足を解消する為に、
　　3代目（PC-84C0SE）ではHC157マルチプレクサとHC597シフトレジスタを搭載していました

　　4代目ではESP32を複数搭載して回路設計の簡略化と実行速度向上を実現しました
　　起動時にGPIO48をINPUTに切り替えてPD/PUを読み取る事でMaster/Slaveを判別しています

　【M/S相互通信】
　　2台のESP32はSerial1を介して相互通信しており、Serial0の入出力は全て複製されます
　　Master/Slaveどちらに繋いだターミナル画面からでも同じ様に操作する事が可能です

　　BLEシリアル（iPhone側アプリ:AirTerminal）と併せて、複製3画面を表示する事が出来ます

　【信号電圧変換】
　　ESP32の3.3[V]出力→Z84C・論理ICの5.0[V]入力は、直結しますｗ
　　Z84C・論理ICの5.0[V]出力→ESP32の3.3[V]入力は、抵抗（10[KΩ]）を直列接続します

　【シリアル緩衝】
　　操作端末とのデータ通信は、USBシリアル×2・BLEシリアルによるマルチUARTで、
　　受信・送信それぞれ1Kバイトの緩衝バッファを設けています

　　制御キーのASCIIコードは、緩衝バッファを介さず優先的に処理されます

　【SRAM直接転送】
　　System Resetが実行される際のIPL（0000H～00FFH）や
　　CP/M 2.2が動作中のディスクR/Wデータは、ESP32が直接SRAMへDMA転送します

　　Adrsバスの上位8ビットはSlaveに接続されている為、MasterがSRAMにアクセスする際の
　　ページアドレス切り替えは、M/S相互通信によって行われます

　【入出力履歴】
　　液晶表示セグメントの最下段でCPU実行サイクルを表示中に、セグメント中段の右2列
　　（任意データ01&02のビット）にMREQとIORQの履歴を選択的に表示する事が出来ます

　　オペランド確認・入出力トレース等、低周波数クロックでのデバッグ時に役立ちます


★RunCPM導入

　2台搭載しているESP32-S3-DevKitM-1の片方（Slave側）と、
　液晶パネルと一体化しているSDカードスロット「だけ」を使って、
　RunCPM（Z80 CP/M 2.2 エミュレータ）を動作させてみましたｗ

　RunCPMをダウンロードします
　　https://github.com/Shisuibi/RunCPM
　（これは原本RunCPMの複写Forkで、以下の変更は全て適用済です）

　RunCPMフォルダの中にあるRunCPM.inoから
　Arduino IDEプロジェクトフォルダを作成します

　#include "～.h"で参照している以下の14個のヘッダファイルを
　Arduino IDEプロジェクトフォルダにコピーします

　"abstraction_arduino.h"　"ccp.h"　"console.h"　"cpm.h"
　"cpu_mhz.h"　"cpu1.h"　"cpu2.h"　"cpu3.h"　"cpu4.h"
　"debug.h"　"disk.h"　"globals.h"　"host.h"　"ram.h"

　ボード設定のヘッダファイルはPC-84C0SD.zipから
　【Extraフォルダの"natsumi_gen4.h"】をコピーします（後述）

　RunCPM.inoにある#include "hardware/～"を"natsumi_gen4.h"に書き換えます
　RunCPM.inoにある#define SERIALSPD 9600を115200に書き換えます

　RunCPM.inoにあるSerial.beginの直後の待機処理を削除します
　（PCと有線接続せずにiPhoneと無線接続だけする場合）

　　Serial.begin(SERIALSPD);
　　/*----
　　while (!Serial) {　～　}　// Wait until serial is connected
　　----*/

　RunCPM.inoに複数あるdigitalWrite(LED, ～);を全て書き換えます
　（"abstraction_arduino.h"の中にもあるので注意）

　　digitalWrite(LED, LOW );　→　NeoPixWrite(LED, LOW );
　　digitalWrite(LED, HIGH);　→　NeoPixWrite(LED, HIGH);

　abstraction_arduino.hの最下部にある以下の5個の関数を削除します

　　/*----
　　int _kbhit(void) {　～ }　uint8 _getch(void) {　～　}
　　uint8 _getche(void) {　～　}　void _putch(uint8 ch) {　～　}
　　void _clrscr(void) {　～　}
　　----*/

　DISKフォルダのA0.zipを解凍して生成される\A\0を
　フォルダ構造を変更せずにルートフォルダにコピーします

　PC-84C0SD.zipをダウンロードします
　（ダウンロード → ★SDカード作成を参照）

　（PC-84C0SDとRunCPMでSDカードのファイルを共用します）

　PC-84C0SD.zipを解凍して生成されるSDcardフォルダの内容を
　フォルダ構造を変更せずにルートフォルダにコピーします

　（RunCPMのMBASICで動作するのは、入出力命令を使用していない
　DOWNMAZE・MANDEL(Text)・STARTREK・TREKINSTだけです）

　PC-84C0SD.zipを解凍して生成されるExtraフォルダの"natsumi_gen4.h"を
　Arduino IDEプロジェクトフォルダにコピーします


★超小型RunCPM

　Seeed StudioのXIAO ESP32C6とSDカードスロットを使って、
　超小型RunCPMモジュールを作成してみましたｗ

　前項のArduino IDEプロジェクトフォルダがそのまま利用出来ます
　（XIAO ESP32C6とESP32-S3-DevKitMのLED Pinは自動識別されます）

　これをiPhoneのUSB-Cコネクタ（電源）に挿して、更にBLE UARTで
　iPhoneと無線通信すれば、ドングル1個のZ80 CP/Mモバイル環境爆誕ｗ


★CP/M 2.2実装

　RunCPMはZ80 CPU自身やCCP・BDOS等、CP/Mの全てをESP32でエミュレートしています
　動作も超高速で申し分ないのですが、IPS液晶表示等の周辺機器も活用したいので、
　PC-84C0SDに載っているZ80 CPUやSRAMも含めた実機上でCP/Mを動作させてみました

　CCPとBDOSはDigital Researchのオリジナルコードを変更せず、
　BIOSコールから先をDual ESP32-S3-MINI-1が連携して処理します

　PC-84C0SDとRunCPMでSDカードのファイルを共用する為に、外部コマンドとして
　MS-DOSのFAT ⇔ CP/MのDISKイメージを相互変換する機能を追加しました

　今まで大変お世話になったGrant Searle氏のNascom ROM BASICは、
　トランジェントコマンドROMBASIC.COMとして残しておきましたｗ

　（ROMBASICはBIOS領域も上書きするので注意）
　CP/Mに戻るには「Ctrl＋A（システム再起動）→発振φ選択」の順に操作して下さい
　ROMBASIC起動後の「Memory top?」で61952（F200H）未満の値を入力しておけば、
　MONITORまたはRESETコマンドで正常にCP/Mへ戻る事が出来ます

　【メモリマップ】

　　0000H　＋――――＋
　　　　　 ｜　IPL 　｜　Initial Program Loader
　　0100H　＋――――＋
　　　　　 ｜　　　　｜
　　　　　 ｜　TPA 　｜　Transient Program Area
　　　　　 ｜　　　　｜
　　DC00H　＋――――＋
　　　　　 ｜　　　　｜
　　　　　 ｜　CCP 　｜　Console Command Processor
　　　　　 ｜　　　　｜
　　E400H　＋――――＋
　　　　　 ｜　　　　｜
　　　　　 ｜　BDOS　｜　Basic Disk Operating System
　　　　　 ｜　　　　｜
　　F200H　＋――――＋
　　　　　 ｜　　　　｜
　　　　　 ｜　BIOS　｜　Basic Input & Output System
　　　　　 ｜　　　　｜
　　F600H　＋――――＋
　　　　　 ｜　　　　｜
　　　　　 ｜　ALV 　｜　Allocation Vector
　　　　　 ｜　　　　｜
　　FE00H　＋――――＋
　　　　　 ｜　DPB 　｜　Disk Parameter Block
　　FF00H　＋――――＋
　　　　　 ｜　DPH 　｜　Disk Parameter Header
　　　　　 ＋――――＋


★Z3D API

　【機能概要】
　　Z3D APIは、入出力ポートを経由してグラフィック画面に
　　リアルタイム3D映像を描画する幾何学演算ライブラリです

　　3D映像の描画はZ80→ESP32Master→ESP32Slaveの順に
　　パイプラインで並列処理されます

　　座標や倍率は整数部8ビット・小数部8ビットの固定小数点数、
　　角度や係数は8ビットの整数でZ80から設定する事が出来ます

　　ライブラリの内部では全て浮動小数点数で演算しています
　　±128[ｍ]を超える空間が必要な場合は拡大縮小して下さい

　【実行手順】
　　最初にZ3D APIを初期化します（60H）
　　画面に表示する模型を追加します（70H～75H）
　　材質の反射係数と光源の入射方向を設定します（62H・63H）

　　座標変換に必要な行列を積み上げます（64H～6FH）
　　模型登録・模型陰影を一緒に実行します（76H・77H）
　　出来上がった3D映像で液晶画面を更新します（61H）

　【模型追加】
　　模型の属性・材質数・頂点数・多角数を座標XYZW下位で設定してから
　　（座標はポート78H～7FH）ポート71Hに模型番号を出力します

　　（模型番号は任意の8ビット値を使用する事が出来ます）
　　座標X下位…属性、座標Y下位…材質数、座標Z下位…頂点数、座標W下位…多角数

　　属性ビット0…裏面判定、属性ビット1…針金枠組
　　（裏面判定・針金枠組が共にONの時は隠線消去も実行されます）

　　属性ビット7…奥行整列
　　（奥行整列は模型内部の多角だけを並べ替える機能なので注意）

　　模型登録（後述）した順番で描画されるので、模型同士の
　　並べ替えは事前にアプリケーションで演算する必要があります

　　模型を追加する毎に格納領域が消費されます（70H入出力で全消去)
　　模型追加の成功／失敗はポート71H・72Hの入力値で調べられます

　　模型材質の色彩を座標XYZで設定してから
　　ポート73Hに材質番号を出力します（材質数分繰り返します）

　　座標X下位…色彩B、座標Y下位…色彩G、座標Z下位…色彩R
　　（色彩は24ビット色RGB888のリトルエンディアンなので注意）

　　模型頂点の構築座標を座標XYZWで設定してから
　　ポート74Hに頂点番号を出力します（頂点数分繰り返します）

　　模型多角の頂点番号×3と材質番号を座標XYZWで設定してから
　　ポート75Hに多角番号を出力します（多角数分繰り返します）

　　座標X下位…頂点0、座標Y下位…頂点1、座標Z下位…頂点2、座標W下位…材質
　　（頂点追加／変更は多角追加／変更によって表面法線に反映されます）

　　全ての模型頂点を追加してから模型多角を追加して下さい
　　（多角を追加する毎に構成頂点から表面法線が計算される為）

　【光源設定】
　　光源演算は環境反射と拡散反射、平行光源に対応しています
　　反射係数は0.0～1.0を8ビットで設定します
　　（環境反射＋拡散反射≦1.0になる様に係数が補正されます）

　　平行光源の入射方向を座標XYZWで設定してから
　　拡散反射係数を設定して下さい（光源方向は正規化されます）

　【行列階層】
　　モデリング座標系→ワールド座標系→視点（カメラ）座標系→
　　視野錘台（ビューボリューム）座標系→デバイス座標系への
　　座標変換をする為に、行列スタックを使用する事が出来ます

　　結合法則により複数の座標変換を1回で実行する事が出来ます
　　多関節で構成される模型（人体等）を演算する時も便利です

　　初期化した直後の行列階層は0番で何も積み重なっておらず、
　　現行行列には単位行列が入っています

　　全ての行列演算ポートは入力値に現行の行列階層を返します
　　これは行列乗算の出力値として使います（後述）

　　後で透視変換行列×表示機器行列の行列乗算を実行する為に、
　　ここで現行の行列階層（00H）を取得しておきます

　　　 　｜　　　　　：　　　　　｜
　　　　 ＋―――――――――――＋
　　05H　｜　　WL×VP×PT×DD　　｜　世界座標行列×視点座標行列×透視変換行列×表示機器行列
　　　　 ＋―――――――――――＋
　　04H　｜　　　　World 　　　　｜　世界座標行列
　　　　 ＋―――――――――――＋
　　03H　｜　Viewpoint×PT×DD 　｜　視点座標行列×透視変換行列×表示機器行列
　　　　 ＋―――――――――――＋
　　02H　｜　　　　PT×DD　　　　｜　透視変換行列×表示機器行列
　　　　 ＋―――――――――――＋
　　01H　｜　Perspective Trans 　｜　透視変換行列
　　　　 ＋―――――――――――＋
　　00H　｜　　Display Device　　｜　表示機器行列
　　　　 ＋―――――――――――＋

　　ポート6EH出力で現行行列（00H）が表示機器行列になります
　　ポート64H出力で行列を1段積み上げます

　　視野錘台の前方切取面・後方切取面を座標XYで設定してから
　　ポート6FHに画角（180度を8ビットで設定）を出力します

　　現行行列（01H）が透視変換行列になります
　　ポート64H出力で行列を1段積み上げます

　　ポート6BHに表示機器行列の行列階層（00H）を出力します
　　現行行列（02H）が透視変換行列×表示機器行列になります

　　ポート64H出力で行列を1段積み上げます
　　ポート67H出力で現行行列に最上行列（02H）を複写します

　　後で世界座標行列×視点(等)行列の行列乗算を実行する為に、
　　ここで現行の行列階層（03H）を取得しておきます

　　視点座標系への行列演算（回転・平行移動）を実行します
　　ポート64H出力で行列を1段積み上げます

　【登録陰影】
　　（模型登録と模型陰影は順不同で実行出来ます）
　　ポート66H出力で現行行列（04H）が単位行列になります

　　世界座標系への行列演算（回転・平行移動）を実行します
　　ポート64H出力で行列を1段積み上げます

　　ポート6BHに視点(等)行列の行列階層（03H）を出力します
　　現行行列（05H）が世界座標行列×視点(等)行列になります

　　ポート76Hに模型番号を出力します（模型登録）
　　ポート65H出力で行列を1段取り除きます

　　ポート77Hに模型番号を出力します（模型陰影）
　　ポート65H出力で行列を1段取り除きます

　　模型登録…頂点の構築座標と現行行列で座標変換されます
　　模型陰影…多角の表面法線と現行行列で光源演算されます

　　模型登録・模型陰影の両方が揃うと描画されます
　　（同じ模型番号を続けて実行しないと描画されないので注意）

　【画面更新】
　　ポート61H入出力で液晶画面を更新します
　　（ESP32Slaveと同期されます）


★操作説明

　電源投入またはESP32リセット時、SDカードのルートフォルダから
　"LCDSLOGO.BMP"を読み込んで液晶ディスプレイに表示します

　起動ロゴを表示してから2秒後にZ80 CPUと入出力がリセットされ、
　ターミナル画面に"CB[RST]"と表示されます

　IPL（0000H～00FFH）が直接SRAMへDMA転送され、
　CPUクロックを停止してスタンバイしている状態です

　Ctrlキー（後述）を押してCPUクロックを選択すると、
　ブートローダー＆CP/M 2.2の実行が開始されます

　AドライブにAUTOEXEC.SUBがあれば、起動時に自動実行します

　【手動＆低周波数クロックでZ80 CPUの動作を観察】
　リセット＆スタンバイの状態から、

　Ctrl＋D（CPU入出力履歴）を3回押して"PM[P&M]"を表示します
　（液晶セグメント上段中央の"PH"と"MH"が両方とも点灯します）

　Ctrl＋F（CPU実行サイクル）を1回押して"EC[ ON]"を表示します
　（液晶セグメント上段中央の"EC"が点灯します）

　Ctrl＋P（手動φ／停止）を1回押す毎に"　0[_n_]"と表示され、
　単発の手動クロックが発行されます

　最初の2Tサイクルが無効になった後、3クロック目からZ80 CPUの
　M1サイクルが始まります（Z80 CPUリセット時の動作仕様）

　0043Hには【LD A, n】命令があり、0044HのオペランドAAHが
　読み込まれると、液晶セグメント中段中央＆右側に表示されます

　0045Hの【OUT (n), A】命令も同様、オペランド20Hの読み込みと
　ポート(20H)への書き込みの両方で入出力履歴が表示されます


★キー操作＆システムメッセージ

　Ctrl＋A　｜　システム再起動　"CB[RST]"
　Ctrl＋S　｜　メッセージ表示　"MS[ ON]"　"MS[OFF]"
　Ctrl＋D　｜　CPU入出力履歴　"--[A/N]"　"-M[MEM]"　"P-[PIO]"　"PM[P&M]"
　Ctrl＋F　｜　CPU実行サイクル　"EC[ ON]"　"EC[OFF]"
　Ctrl＋G　｜　液晶表示モード　"LC[ ON]"　"LC[OFF]"
　Ctrl＋V　｜　制御キー操作の有効／無効（システム再起動は除く）
　Ctrl＋K　｜　ヘルプ画面表示（キー操作／入出力ポート）
　Ctrl＋L　｜　テキスト画面消去

　Ctrl＋Q　｜　発振φ（　4[ Hz]）　"　4[ Hz]"
　Ctrl＋W　｜　発振φ（ 32[ Hz]）　" 32[ Hz]"
　Ctrl＋E　｜　発振φ（256[ Hz]）　"256[ Hz]"
　Ctrl＋R　｜　発振φ（　2[KHz]）　"　2[KHz]"
　Ctrl＋T　｜　発振φ（ 16[KHz]）　" 16[KHz]"
　Ctrl＋Y　｜　発振φ（128[KHz]）　"128[KHz]"
　Ctrl＋U　｜　発振φ（　1[MHz]）　"　1[MHz]"
　Ctrl＋I　｜　発振φ（　8[MHz]）　"　8[MHz]"
　Ctrl＋O　｜　発振φ（ 20[MHz]）　" 20[MHz]"
　Ctrl＋P　｜　手動φ／停止　　　　"　0[_n_]"

　Ctrl＋3　｜　発振φ（2.5[MHz]）　"2.5[MHz]"
　Ctrl＋4　｜　発振φ（　4[MHz]）　"　4[MHz]"
　Ctrl＋5　｜　発振φ（ 10[MHz]）　" 10[MHz]"
　Ctrl＋6　｜　発振φ（　6[MHz]）　"　6[MHz]"

　BドライブにはBASIC言語のサンプルアプリが入っています
　ROMBASICでは入出力ポート36Hに呼出番号を出力して下さい
　CP/M版のMBASICではLOADまたはRUNコマンドも利用出来ます

　OUT &H36,0　｜　SDカード読込（スーパースタートレック）
　OUT &H36,1　｜　SDカード読込（スタートレック説明書）
　OUT &H36,2　｜　SDカード読込（ナイトライダー）
　OUT &H36,3　｜　SDカード読込（マンデルブロ集合）
　OUT &H36,4　｜　SDカード読込（ダウンメイズ）
　OUT &H36,5　｜　SDカード読込（ネオピクセルHSV）
　OUT &H36,6　｜　SDカード読込（ROMダンプリスト）
　OUT &H36,7　｜　SDカード読込（システム時刻）
　OUT &H36,8　｜　SDカード読込（旋律♪自動演奏）
　OUT &H36,9　｜　SDカード読込（幾何学演算ライブラリ）

　対象ファイル名が表示された後、プログラムテキストが読み込まれます
　SDカードが挿入されていて、SDカードの\B\0フォルダに
　以下のファイルが書き込まれている必要があります

　"STARTREK.BAS"　"TREKINST.BAS"　"KNIGHT2K.BAS"　"MANDEL.BAS"
　"DOWNMAZE.BAS"　"NEOPIXEL.BAS"　"ROMDUMP.BAS"　"CLKTIMER.BAS"
　"MELODY.BAS"　"Z3DAPI.BAS"


★BASICファイル紹介

　StarTrek
　　1970年代から1980年代にかけて、
　　殆どの8ビットパソコンに移植されました
　　TVシリーズや映画も大好きですｗ

　TrekInst
　　StarTrekの操作説明を表示するプログラムです

　KnightRider
　　液晶セグメント中段の任意データ表示エリアを
　　フル活用した、ナイト2000風味のデモですｗ

　Mandel
　　テキスト版またはグラフィック版を選択すると、
　　あの有名なフラクタル図形が描画されます
　　グラフィック版はCtrl＋G（液晶表示モード）を
　　1回押して"LC[OFF]"を表示してから実行して下さい

　DownMaze
　　マイコンBASICマガジン1983年06月号（通巻12号）に
　　東芝製8ビットパソコンPASOPIA用として掲載されました
　　作者は「くたらはしい」さんです
　　https://archive.org/details/micom-basic-magazine-issue-12-june-1983/Micom%20BASIC%20Magazine%20-%20Issue%2012%20-%20June%201983/
　　シンプルなゲームなのに、中毒性の高さは異常ｗ
　　街の電気屋さんのPASOPIAで1日中遊んでました

　DownMaze88
　　PC-8801MAエミュレータに移植してみました
　　INKEY$が使えるので、こちらの方が操作性は良いです
　　オリジナルのPASOPIA版同様、サクサク降りられます
　　先行入力機能を追加したので、着地後即4に注意ｗ

　NeoPixel
　　ESP32-S3-DevKitM-1に搭載されているRGB LEDを
　　HSV空間で制御するプログラムです
　　CPUクロックは4[MHz]または8[MHz]が
　　ムードライトっぽくて良いと思います

　RomDump
　　ROMのダンプリスト（IPL・BIOS）を表示します

　ClockTimer
　　時・分・秒を入力すると、デジタル時計を表示します
　　Ctrl＋G（液晶表示モード）を1回押して"LC[OFF]"を
　　表示してから実行すると、液晶グラフィック画面にも
　　デジタル時計が表示されます

　Melody
　　単音BGMを2曲、自動演奏します
　　　1. XEVIOUS
　　　2. Elevator Action

　Z3D API
　　グラフィック画面にリアルタイム3D映像を描画します


★入出力ポート

　00H　｜　[IN] ROM読込（読込アドレスは自動更新）　[OUT] MakeDisk呼出
　01H　｜　[IN/OUT] システム再起動（値は不問）

　02H　｜　[IN]BIOSパラメータ下位8ビット　[OUT]ROM読込アドレス下位8ビット
　03H　｜　[IN]BIOSパラメータ上位8ビット　[OUT]ROM読込アドレス上位8ビット

　04H　｜　[IN/OUT] ヘルプ画面表示（値は不問）
　05H　｜　[IN/OUT] テキスト画面消去（値は不問）

　06H　｜　メッセージ表示

　08H　｜　[IN/OUT] 液晶表示起動装飾（液晶表示モードがグラフィック時のみ）
　09H　｜　[IN/OUT] 液晶表示画面撮影（SDカードのルートフォルダに"LCDSSHOT.BMP"として保存されます）

　0AH　｜　制御キー操作の有効／無効（システム再起動は除く）
　0FH　｜　[IN/OUT] 予約済（Slave制御）

　10H　｜　旋律♪音符（上位4ビット:オクターブ00H～80H｜下位4ビット:音階00H～0BH）
　11H　｜　旋律♪音量（00H～FFH:PWM解像度11ビット、デューティ比0[%]～12.5[%]）

　12H　｜　RAM 32[KB]下位バンク切替ページ番号（現状は未使用、0以外にするとCP/M 2.2が暴走します）

　13H　｜　CPU INT要求（現状は未使用、HighにするとCP/M 2.2が暴走します）
　14H　｜　CPU NMI要求（現状は未使用、HighにするとCP/M 2.2が暴走します）

　15H　｜　CPU WAIT許可（現状は未使用、LowにするとCP/M 2.2が暴走します）
　16H　｜　RAM Chip許可（現状は未使用、LowにするとCP/M 2.2が暴走します）

　18H　｜　システム時刻（厘:電源投入またはESP32リセットからの経過時間を表示します）
　19H　｜　システム時刻（秒:電源投入またはESP32リセットからの経過時間を表示します）

　1AH　｜　システム時刻（分:電源投入またはESP32リセットからの経過時間を表示します）
　1BH　｜　システム時刻（時:電源投入またはESP32リセットからの経過時間を表示します）

　20H　｜　任意データ00（ユーザー任意の8ビットデータ）　Neo Pixel色彩（赤）
　21H　｜　任意データ01（ユーザー任意の8ビットデータ）　Neo Pixel色彩（緑）

　22H　｜　任意データ02（ユーザー任意の8ビットデータ）　Neo Pixel色彩（青）
　23H　｜　任意データ03（ユーザー任意の6ビットデータ）　Neo Pixel送信（40H:Master｜80H:Slave）

　24H　｜　CPU入出力履歴（00H:任意データ01&02｜01H:MREQ｜02H:IORQ｜03H:MREQ&IORQ）
　25H　｜　CPU実行サイクル

　26H　｜　発振φモード（手動／停止・4・32・256・2K・16K・128K・1M・2.5M・4M・6M・8M・10M・20M[Hz]）

　30H　｜　[IN] UARTデータ照会　[OUT] 緩衝バッファ（受信）へ書き込み
　31H　｜　[IN] UARTデータ受信　[OUT] UARTデータ送信

　32H　｜　[IN Only] BLEシリアル端末状態
　33H　｜　[IN Only] SDカード状態

　34H　｜　液晶表示モード（セグメント／グラフィック）
　35H　｜　液晶表示LED輝度

　36H　｜　[IN] BASICファイル数　[OUT] SDカード読込（BASICファイル）

　40H　｜　液晶表示回転方向（0～3）
　41H　｜　液晶表示画面消去（0ならC0、1ならC1で画面を消去します）

　42H　｜　液晶表示文章寸法（文字のサイズを設定します）
　43H　｜　液晶表示文字描画（座標X3Y3色彩C0C1で文字を描画した後、座標X3Y3を変位X2Y2で更新します）

　44H　｜　液晶表示色彩C0下位8ビット（RGB565形式の下位8ビットを設定します）
　45H　｜　液晶表示色彩C0上位8ビット（RGB565形式の上位8ビットを設定します）

　46H　｜　液晶表示色彩C1下位8ビット（RGB565形式の下位8ビットを設定します）
　47H　｜　液晶表示色彩C1上位8ビット（RGB565形式の上位8ビットを設定します）

　48H　｜　液晶表示単点描画（0なら座標X0Y0色彩C0、1なら座標X1Y1色彩C1で単点を描画します）
　49H　｜　液晶表示線分描画（0なら座標X0Y0X1Y1色彩C0、1なら座標X0Y0X1Y1色彩C1で線分を描画します）

　4AH　｜　液晶表示三角描画（0なら座標X0Y0X1Y1X2Y2色彩C0、1なら座標X0Y0X1Y1X2Y2色彩C1で三角を描画します）
　4BH　｜　液晶表示三角塗布（0なら座標X0Y0X1Y1X2Y2色彩C0、1なら座標X0Y0X1Y1X2Y2色彩C1で三角を塗布します）

　4CH　｜　液晶表示四角描画（0なら座標X0Y0変位X2Y2色彩C0、1なら座標X1Y1変位X2Y2色彩C1で四角を描画します）
　4DH　｜　液晶表示四角塗布（0なら座標X0Y0変位X2Y2色彩C0、1なら座標X1Y1変位X2Y2色彩C1で四角を塗布します）

　4EH　｜　液晶表示円形描画（0なら座標X0Y0半径X3Y3色彩C0、1なら座標X1Y1半径X3Y3色彩C1で円形を描画します）
　4FH　｜　液晶表示円形塗布（0なら座標X0Y0半径X3Y3色彩C0、1なら座標X1Y1半径X3Y3色彩C1で円形を塗布します）

　50H　｜　液晶表示座標X0下位8ビット
　51H　｜　液晶表示座標X0上位8ビット

　52H　｜　液晶表示座標Y0下位8ビット
　53H　｜　液晶表示座標Y0上位8ビット

　54H　｜　液晶表示座標X1下位8ビット
　55H　｜　液晶表示座標X1上位8ビット

　56H　｜　液晶表示座標Y1下位8ビット
　57H　｜　液晶表示座標Y1上位8ビット

　58H　｜　液晶表示変位X2下位8ビット（三角の座標にも使用します）
　59H　｜　液晶表示変位X2上位8ビット（三角の座標にも使用します）

　5AH　｜　液晶表示変位Y2下位8ビット（三角の座標にも使用します）
　5BH　｜　液晶表示変位Y2上位8ビット（三角の座標にも使用します）

　5CH　｜　液晶表示半径X3下位8ビット（文字の座標にも使用します）
　5DH　｜　液晶表示半径X3上位8ビット（文字の座標にも使用します）

　5EH　｜　液晶表示半径Y3下位8ビット（文字の座標にも使用します）
　5FH　｜　液晶表示半径Y3上位8ビット（文字の座標にも使用します）

　60H　｜　[IN/OUT] 幾何学演算初期化（値は不問）
　61H　｜　[IN/OUT] 幾何学演算画面更新（値は不問）

　62H　｜　幾何学演算環境反射係数（0.0～1.0を8ビットで設定します）
　63H　｜　幾何学演算拡散反射係数（0.0～1.0を8ビット、平行光源方向は座標XYZWで設定します）

　64H　｜　[IN] 行列階層　[OUT] 幾何学演算行列積重（値は不問）
　65H　｜　[IN] 行列階層　[OUT] 幾何学演算行列取除（値は不問）

　66H　｜　[IN] 行列階層　[OUT] 幾何学演算単位行列（値は不問）
　67H　｜　[IN] 行列階層　[OUT] 幾何学演算行列複写（値は不問）

　68H　｜　[IN] 行列階層　[OUT] 幾何学演算回転X軸（角度は－180度～＋180度を8ビットで設定します）
　69H　｜　[IN] 行列階層　[OUT] 幾何学演算回転Y軸（角度は－180度～＋180度を8ビットで設定します）

　6AH　｜　[IN] 行列階層　[OUT] 幾何学演算回転Z軸（角度は－180度～＋180度を8ビットで設定します）
　6BH　｜　[IN] 行列階層　[OUT] 幾何学演算行列乗算（行列階層、現行行列＝最上行列×階層行列）

　6CH　｜　[IN] 行列階層　[OUT] 幾何学演算平行移動（値は不問、平行移動位置は座標XYZWで設定します）
　6DH　｜　[IN] 行列階層　[OUT] 幾何学演算拡大縮小（値は不問、拡大縮小倍率は座標XYZWで設定します）

　6EH　｜　[IN] 行列階層　[OUT] 幾何学演算表示機器行列（値は不問）
　6FH　｜　[IN] 行列階層　[OUT] 幾何学演算透視変換行列（画角は180度を8ビット、距離は座標XYで設定します）

　70H　｜　[IN/OUT] 幾何学演算模型消去
　71H　｜　[IN] 現行模型の有無　[OUT] 幾何学演算模型追加（模型番号、内容は座標XYZWで設定します）

　72H　｜　[IN] 現行模型の有無　[OUT] 幾何学演算現行模型変更（模型番号）
　73H　｜　[IN] 現行模型の材質数　[OUT] 幾何学演算材質変更（材質番号、内容は座標XYZで設定します）

　74H　｜　[IN] 現行模型の頂点数　[OUT] 幾何学演算頂点変更（頂点番号、内容は座標XYZWで設定します）
　75H　｜　[IN] 現行模型の多角数　[OUT] 幾何学演算多角変更（多角番号、内容は座標XYZWで設定します）

　76H　｜　[IN] 現行模型の頂点数　[OUT] 幾何学演算模型登録（模型番号、現行行列で登録します）
　77H　｜　[IN] 現行模型の多角数　[OUT] 幾何学演算模型陰影（模型番号、現行行列で陰影します）

　78H　｜　幾何学演算座標X下位8ビット（整数部8ビット・小数部8ビットの固定小数点数です）
　79H　｜　幾何学演算座標X上位8ビット（整数部8ビット・小数部8ビットの固定小数点数です）

　7AH　｜　幾何学演算座標Y下位8ビット（整数部8ビット・小数部8ビットの固定小数点数です）
　7BH　｜　幾何学演算座標Y上位8ビット（整数部8ビット・小数部8ビットの固定小数点数です）

　7CH　｜　幾何学演算座標Z下位8ビット（整数部8ビット・小数部8ビットの固定小数点数です）
　7DH　｜　幾何学演算座標Z上位8ビット（整数部8ビット・小数部8ビットの固定小数点数です）

　7EH　｜　幾何学演算座標W下位8ビット（整数部8ビット・小数部8ビットの固定小数点数です）
　7FH　｜　幾何学演算座標W上位8ビット（整数部8ビット・小数部8ビットの固定小数点数です）

　80H　｜　CP/M BIOS（CP/M開始）
　81H　｜　CP/M BIOS（UART制御）

　82H　｜　CP/M BIOS（UART受信）
　83H　｜　CP/M BIOS（UART送信）

　84H　｜　CP/M BIOS（リスト出力）
　85H　｜　CP/M BIOS（パンチ出力）

　86H　｜　CP/M BIOS（リーダ入力）
　87H　｜　CP/M BIOS（ヘッド帰還）

　88H　｜　CP/M BIOS（ドライブ選択）
　89H　｜　CP/M BIOS（トラック選択）

　8AH　｜　CP/M BIOS（セクタNo選択）
　8BH　｜　CP/M BIOS（DMAアドレス）

　8CH　｜　CP/M BIOS（セクタ読込）
　8DH　｜　CP/M BIOS（セクタ書込）

　8EH　｜　CP/M BIOS（リスト制御）
　8FH　｜　CP/M BIOS（セクタNo変換）

