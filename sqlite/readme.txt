●sqlite3について簡単調査

[概要]

sqlite3でデータベース管理するために開発環境と簡単なプロジェクトを作成しました。

[ディレクトリ]

+ download/
|	+ sqlite-amalgamation-3310100.zip		//sqliteライブラリ（ソース）
|	+ sqlite-dll-win32-x86-3310100.zip		//sqliteのwindows用DLL(win32)
|	+ sqlite-dll-win64-x64-3310100.zip		//sqliteのwindows用DLL(win64)
|	+ sqlite-tools-win32-x86-3310100.zip	//sqliteのツール
|
+ project/
|	+ simple/								//簡単なプロジェクトvs2017用
|
+ readme.txt								//(自テキスト)

[sqliteを組み込む作業工程]
１．SQLiteのダウンロード
　　→SQLiteオフィシャルホームページよりソース,DLL,ツールをダウンロードする。
　　　・ソースコード
　　　　sqlite-amalgamation-3310100.zip
　　　・Windows用のプリコンパイル済みバイナリ
　　　　sqlite-dll-win32-x86-3310100.zip
　　　　sqlite-dll-win64-x64-3310100.zip
　　　　sqlite-tools-win32-x86-3310100.zip

２．SQLiteが使用するsqlite3.libファイルを作成
　　→「sqlite-dll-****.zip」解凍して、フォルダ内にあるsqlite3.dllとsqlite3.defファイルから静的ライブラリであるsqlite3.libを作成する。
　　１．Visual Studio 2015の「開発者コマンドプロンプトfor2015」を立ち上げる。
　　２．ライブラリ作成にはlibコマンドを使用する。コマンドプロンプト上で下記を入力。
	
　　>lib /def:C:\Workspace\sqlite3\sqlite3.def /out:C:\Workspace\sqlite3\sqlite3.lib

　　３．正しく動作すれば、sqlite3.libが指定フォルダに生成される。

３．新規プロジェクトの立ち上げ
　　Visual Studio 2015を立ち上げて、空プロジェクトを作成する。

４．作成したsqlite3.libライブラリファイルをプロジェクトに関連付け
　　１．プロジェクトのプロパティ内の[構成プロパティ]-[リンカ]-[全般]にある[追加のライブラリディレクトリ]にsqlite3.libのあるフォルダへのパスを追加。
　　２．続いてプロパティ内の[構成プロパティ]-[リンカ]-[入力]にある[追加依存のファイル]に「sqlite3.lib」を追加。

５．sqlite3.hへのパスを通す
　　・プロジェクトのプロパティの[構成プロパティ]-[C/C++]-[全般]にある[追加のインクルードディレクトリ]にsqlite3.hがあるフォルダパスを指定。
　　　例：$(ProjectDir)source\sqlite\include

６．sqlite3.dllにパスを通す
　　sqlite3.dllを立ち上げたプロジェクト(exeファイル配下）のフォルダ下にコピーする。
　　例）：「project\simple\Debug」や「project\simple\Release」の配下

[参考サイト]

//SQLiteのインストール
http://marupeke296.com/SQLITE_No2_InstallSQLite.html
//SQLite Vesion 3 DLL版 を呼び出すC/C++アプリケーションの構築
http://hp.vector.co.jp/authors/VA002803/sqlite/capi3dll.htm
//C言語でSQLiteを使う
https://qiita.com/Ki4mTaria/items/bce1a6080e537fb1c949
//SQLiteのインストールとSQLを実行するサンプル
https://itsakura.com/tool-sqlite-install

注意：2020/04/20時点の情報です。更新されていることがありますので注意してください。

---------------
2020/04/20 崎浜
