��sqlite3�ɂ��ĊȒP����

[�T�v]

sqlite3�Ńf�[�^�x�[�X�Ǘ����邽�߂ɊJ�����ƊȒP�ȃv���W�F�N�g���쐬���܂����B

[�f�B���N�g��]

+ download/
|	+ sqlite-amalgamation-3310100.zip		//sqlite���C�u�����i�\�[�X�j
|	+ sqlite-dll-win32-x86-3310100.zip		//sqlite��windows�pDLL(win32)
|	+ sqlite-dll-win64-x64-3310100.zip		//sqlite��windows�pDLL(win64)
|	+ sqlite-tools-win32-x86-3310100.zip	//sqlite�̃c�[��
|
+ project/
|	+ simple/								//�ȒP�ȃv���W�F�N�gvs2017�p
|
+ readme.txt								//(���e�L�X�g)

[sqlite��g�ݍ��ލ�ƍH��]
�P�DSQLite�̃_�E�����[�h
�@�@��SQLite�I�t�B�V�����z�[���y�[�W���\�[�X,DLL,�c�[�����_�E�����[�h����B
�@�@�@�E�\�[�X�R�[�h
�@�@�@�@sqlite-amalgamation-3310100.zip
�@�@�@�EWindows�p�̃v���R���p�C���ς݃o�C�i��
�@�@�@�@sqlite-dll-win32-x86-3310100.zip
�@�@�@�@sqlite-dll-win64-x64-3310100.zip
�@�@�@�@sqlite-tools-win32-x86-3310100.zip

�Q�DSQLite���g�p����sqlite3.lib�t�@�C�����쐬
�@�@���usqlite-dll-****.zip�v�𓀂��āA�t�H���_���ɂ���sqlite3.dll��sqlite3.def�t�@�C������ÓI���C�u�����ł���sqlite3.lib���쐬����B
�@�@�P�DVisual Studio 2015�́u�J���҃R�}���h�v�����v�gfor2015�v�𗧂��グ��B
�@�@�Q�D���C�u�����쐬�ɂ�lib�R�}���h���g�p����B�R�}���h�v�����v�g��ŉ��L����́B
	
�@�@>lib /def:C:\Workspace\sqlite3\sqlite3.def /out:C:\Workspace\sqlite3\sqlite3.lib

�@�@�R�D���������삷��΁Asqlite3.lib���w��t�H���_�ɐ��������B

�R�D�V�K�v���W�F�N�g�̗����グ
�@�@Visual Studio 2015�𗧂��グ�āA��v���W�F�N�g���쐬����B

�S�D�쐬����sqlite3.lib���C�u�����t�@�C�����v���W�F�N�g�Ɋ֘A�t��
�@�@�P�D�v���W�F�N�g�̃v���p�e�B����[�\���v���p�e�B]-[�����J]-[�S��]�ɂ���[�ǉ��̃��C�u�����f�B���N�g��]��sqlite3.lib�̂���t�H���_�ւ̃p�X��ǉ��B
�@�@�Q�D�����ăv���p�e�B����[�\���v���p�e�B]-[�����J]-[����]�ɂ���[�ǉ��ˑ��̃t�@�C��]�Ɂusqlite3.lib�v��ǉ��B

�T�Dsqlite3.h�ւ̃p�X��ʂ�
�@�@�E�v���W�F�N�g�̃v���p�e�B��[�\���v���p�e�B]-[C/C++]-[�S��]�ɂ���[�ǉ��̃C���N���[�h�f�B���N�g��]��sqlite3.h������t�H���_�p�X���w��B
�@�@�@��F$(ProjectDir)source\sqlite\include

�U�Dsqlite3.dll�Ƀp�X��ʂ�
�@�@sqlite3.dll�𗧂��グ���v���W�F�N�g(exe�t�@�C���z���j�̃t�H���_���ɃR�s�[����B
�@�@��j�F�uproject\simple\Debug�v��uproject\simple\Release�v�̔z��

[�Q�l�T�C�g]

//SQLite�̃C���X�g�[��
http://marupeke296.com/SQLITE_No2_InstallSQLite.html
//SQLite Vesion 3 DLL�� ���Ăяo��C/C++�A�v���P�[�V�����̍\�z
http://hp.vector.co.jp/authors/VA002803/sqlite/capi3dll.htm
//C�����SQLite���g��
https://qiita.com/Ki4mTaria/items/bce1a6080e537fb1c949
//SQLite�̃C���X�g�[����SQL�����s����T���v��
https://itsakura.com/tool-sqlite-install

���ӁF2020/04/20���_�̏��ł��B�X�V����Ă��邱�Ƃ�����܂��̂Œ��ӂ��Ă��������B

---------------
2020/04/20 ��l
