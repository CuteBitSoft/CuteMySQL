/*****************************************************************//**
 * Copyright 2024 Xuehan Qin (qinxuehan2018@gmail.com) 
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *   http://www.apache.org/licenses/LICENSE-2.0
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 * 
 * @file   QSqlEditor.cpp
 * @brief  
 * 
 * @author Xuehan Qin (qinxuehan2018@gmail.com) 
 * @date   2024-12-17
 *********************************************************************/
#include "QSqlEditor.h"

const char sqlKeyWords[] =
"absolute action add admin after aggregate alias all allocate alter and any are array as asc assertion at authorization "
"before begin binary bit blob body boolean both breadth by "
"call cascade cascaded case cast catalog char character "
"check class clob close collate collation column commit "
"completion connect connection constraint constraints index autoincrement "
"constructor continue corresponding create cross cube current "
"current_date current_path current_role current_time current_timestamp "
"current_user cursor cycle "
"data date datetime day deallocate dec decimal declare default "
"deferrable deferred delete depth deref desc describe descriptor "
"destroy destructor deterministic dictionary diagnostics disconnect "
"distinct domain double drop dynamic "
"each else end end-exec equals escape every except "
"exception exec execute exists exit external "
"false fetch first float for foreign found from free full "
"function "
"general get global go goto grant group grouping "
"having host hour "
"identity if ignore immediate in indicator initialize initially "
"inner inout input insert int integer intersect interval "
"into is isolation iterate "
"join "
"key "
"language large last lateral leading left less level like "
"limit local localtime localtimestamp locator rowid query plan "
"map match merge minute modifies modify module month "
"names national natural nchar nclob new next no none "
"not null numeric pragma explain offset "
"object of off old on only open operation option "
"or order ordinality out outer output "
"package pad parameter parameters partial path postfix precision prefix "
"preorder prepare preserve primary "
"prior privileges procedure public "
"read reads real recursive ref references referencing relative "
"restrict result return returns revoke right "
"role rollback rollup routine row rows "
"savepoint schema scroll scope search second section select "
"sequence session session_user set sets size smallint some| space "
"specific specifictype sql sqlexception sqlstate sqlwarning start "
"state statement static structure system_user "
"table temporary terminate than then time timestamp "
"timezone_hour timezone_minute to trailing transaction translation "
"treat trigger true "
"under union unique unknown "
"unnest update usage user using "
"value values varchar variable varying view "
"when whenever where with without work write "
"year "
"zone";

const char separator = '\x1E';
const char autoStopChars[] = "[\x1E(\x1E>\x1E=\x1E+\x1E*\x1E/\x1E)\x1E]";
// ����CTRL+[key]...�Ĺ���
const char ignoreCtrlKey[] = "SFBNLKHGPOREWQ";

QSqlEditor::QSqlEditor() : 
	wxStyledTextCtrl(), 
	bkgColor(30, 31, 34, 30),
	bkgColor2(38, 40, 46, 40),
	textColor(188, 166, 128, 149)
{
	
}

QSqlEditor::QSqlEditor(wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style) :	
	wxStyledTextCtrl(parent, id, pos, size, style),
	bkgColor(30, 31, 34, 30),
	bkgColor2(38, 40, 46, 40),
	textColor(188, 166, 128, 149)
{
	
}

// setup after Create
void QSqlEditor::setup(int nSize, const char* face)
{
	setupSqlSyntax(nSize, face);
}


bool QSqlEditor::SetBackgroundColour(const wxColour & color)
{
	StyleSetBackground(wxSTC_STYLE_DEFAULT, color);
	return true;
}

void QSqlEditor::setupSqlSyntax(int nSize, const char* face)
{
	// - lex setup (lex�﷨����������)
	SetLexer(wxSTC_LEX_SQL); // ѡ��SQL������
	// Divide each styling byte into lexical class bits (default: 5) and indicator
	// bits (default: 3). If a lexer requires more than 32 lexical states, then this
	// is used to expand the possible states.
	// ��ÿ����ʽ�ֽڻ���Ϊ�ʷ���λ��Ĭ��ֵ��5����ָʾ��λ��Ĭ��ֵ��3����
	// ����ʷ���������Ҫ���� 32 ���ʷ�״̬����������չ���ܵ�״̬��
	// SetStyleBitsEx(5);
	StyleSetForeground(wxSTC_STYLE_DEFAULT, wxColour(255, 0, 0)); // �༭�����ı�Ĭ�ϵ�ǰ��ɫ���ı�Ĭ�ϵ���ɫ��
	StyleSetBackground(wxSTC_STYLE_DEFAULT, bkgColor); // �༭��Ĭ�ϵı���ɫ
	StyleClearAll(); // ����༭�����е���ʽ


	// - Other
	SetIndent(4); // ����4�ַ�
	SetIndentationGuides(wxSTC_IV_LOOKBOTH); // ��ʾ�����������ο���
	UsePopUpEx(true); // ���õ��û���ĳЩ�����ϰ�����갴ťʱ�Ƿ��Զ���ʾ�����˵�
	
	// Error marker
	MarkerDefine(0, wxSTC_MARK_ARROW); // �������ڼ�ͷ��Ǳ�ŵķ��ţ��Լ�����ѡ��ǰ��ɫ(��������)�ͱ���ɫ�����Ĳ�����
	MarkerSetBackground(0, wxColour(255, 255, 255)); // ���õ�0��Marker�ı���ɫ
	MarkerSetForeground(0, wxColour(0, 0, 0)); // ���õ�0��Marker��ǰ��ɫ

	// Font And Size �༭��������ʹ�С
	wxFont font(wxSize(0, nSize), wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, face);
	StyleSetFont(wxSTC_STYLE_DEFAULT, font);
	StyleSetSize(wxSTC_STYLE_DEFAULT, nSize);

	// - Margins
    // number margin 
	SetMarginType(0, wxSTC_MARGIN_NUMBER); // �кű߾࣬���߾�����Ϊ���֡��˴����õ�0���߾����к�����
	SetMarginWidth(0, 37); // ���ñ߾��ȣ���0���߾�߿�
	// SetMarginBackground(0, bkgColor); // ע�⣬���ﲻ��Ч��ԭ����SetMarginBackgroundֻ��SC_MARGIN_COLOUR���͵�margin��Ч�������0�߾���wxSTC_MARGIN_NUMBER
	

	// folding margin �۵��߱߾�
	SetMarginMask(1, wxSTC_MASK_FOLDERS);  // ���õ�2���߾�Ϊ�۵����
	SetMarginWidth(1, 12); // ���ñ߾��ȣ���2���߾�߿�
	SetMarginSensitive(1, true); // ʹ��2���߾����굥�����л����С�

	// - Choose folding icons ѡ���۵���Сͼ��
	MarkerDefine(wxSTC_MARKNUM_FOLDEROPEN, wxSTC_MARK_BOXMINUS); // �����۵��ߴ򿪵�ͼ��:�������
	MarkerDefine(wxSTC_MARKNUM_FOLDER, wxSTC_MARK_BOXPLUS); // �����۵���������ͼ��:����Ӻ�
	MarkerDefine(wxSTC_MARKNUM_FOLDERSUB, wxSTC_MARK_VLINE); // �������۵���:VLINE
	MarkerDefine(wxSTC_MARKNUM_FOLDERTAIL, wxSTC_MARK_LCORNER); // �����۵��߽�����ͼ��:L�͹ս�
	MarkerDefine(wxSTC_MARKNUM_FOLDEREND, wxSTC_MARK_BOXPLUSCONNECTED); // �����۵��߽�����ͼ��:����Ӻ�����
	MarkerDefine(wxSTC_MARKNUM_FOLDEROPENMID, wxSTC_MARK_BOXMINUSCONNECTED); // �����۵��ߴ��м��ͼ��:�����������
	MarkerDefine(wxSTC_MARKNUM_FOLDERMIDTAIL, wxSTC_MARK_LCORNERCURVE); // �����۵����м���β��ͼ��:T�͹ս�����

	// - Choose folding icon colours ѡ���۵���Сͼ����ɫ
	MarkerSetForeground(wxSTC_MARKNUM_FOLDEROPEN, textColor); // �����۵��ߴ�Сͼ���ǰ��ɫ
	MarkerSetBackground(wxSTC_MARKNUM_FOLDEROPEN, bkgColor); // �����۵��ߴ�Сͼ��ı���ɫ
	MarkerSetForeground(wxSTC_MARKNUM_FOLDER, textColor); // �����۵�������Сͼ���ǰ��ɫ
	MarkerSetBackground(wxSTC_MARKNUM_FOLDER, bkgColor); // �����۵�������Сͼ��ı���ɫ
	MarkerSetForeground(wxSTC_MARKNUM_FOLDERSUB, textColor); // �������۵��ߵı���ɫ
	MarkerSetBackground(wxSTC_MARKNUM_FOLDERSUB, textColor); // �������۵��ߵı���ɫ
	MarkerSetForeground(wxSTC_MARKNUM_FOLDERTAIL, textColor); // �����۵�����������Сͼ��ı���ɫ
	MarkerSetBackground(wxSTC_MARKNUM_FOLDERTAIL, textColor); // �����۵�����������Сͼ��ı���ɫ
	MarkerSetForeground(wxSTC_MARKNUM_FOLDEREND, textColor); // �����۵��߽���Сͼ���ǰ��ɫ
	MarkerSetBackground(wxSTC_MARKNUM_FOLDEREND, bkgColor); // �����۵��߽���Сͼ��ı���ɫ
	MarkerSetForeground(wxSTC_MARKNUM_FOLDEROPENMID, textColor); // �����۵��ߴ��м�Сͼ���ǰ��ɫ
	MarkerSetBackground(wxSTC_MARKNUM_FOLDEROPENMID, bkgColor); // �����۵��ߴ��м�Сͼ��ı���ɫ
	MarkerSetForeground(wxSTC_MARKNUM_FOLDERMIDTAIL, textColor); // �����۵����м���β��Сͼ���ǰ��ɫ
	MarkerSetBackground(wxSTC_MARKNUM_FOLDERMIDTAIL, textColor); // �����۵����м���β��Сͼ��ı���ɫ

	// �к��ı���ɫ��������SetMarginType(0, wxSTC_MARGIN_NUMBER);������
	StyleSetForeground(wxSTC_STYLE_LINENUMBER, textColor);
	// �кű�����ɫ	
	StyleSetBackground(wxSTC_STYLE_LINENUMBER, bkgColor2);
	// �۵��߱���ɫ
	SetFoldMarginHiColour(true, bkgColor2);
	SetFoldMarginColour(true, bkgColor2);

	// - Set carlet ���
	// Color of Carlet ������ɫ
	SetCaretForeground(wxColour(0x00CEC0D6));
	// Width of Carlet ����С
	SetCaretWidth(2);
	// set the caret blinking time to 400 milliseconds �����˸���
	SetCaretPeriod(400);

	// - Set caret line colour (���ù�������е���ɫ��
	SetCaretLineBackground(wxColour(38, 40, 46)); // ��������еı���ɫ
	SetCaretLineVisible(true); // ��ʾ���������

	// - Comment block ��ע�Ϳ飩
	StyleSetForeground(wxSTC_SQL_DEFAULT, wxColour(0, 0, 0)); // Ĭ��SQL��ǰ��ɫ
	StyleSetForeground(wxSTC_SQL_COMMENT, wxColour(32768)); // SQLע�͵�ǰ��ɫ

	// - Single comment line (ע����)
	StyleSetForeground(wxSTC_SQL_COMMENTLINE, wxColour(32768)); // Ĭ��SQLע�͵�ǰ��ɫ

	// - SQL number ����
	StyleSetForeground(wxSTC_SQL_NUMBER, wxColour(0x002AACB8)); // SQL���ֵ�ǰ��ɫ
	StyleSetBold(wxSTC_SQL_NUMBER, true); // SQL���ּӴ�

	// - SQL string/operator/identifier (�ַ���/������/��ʶ��)
	StyleSetForeground(wxSTC_SQL_STRING, wxColour(0x00cc99ff)); // SQL�ַ�����ǰ��ɫ
	StyleSetForeground(wxSTC_SQL_CHARACTER, wxColour(0x00cc99ff)); // SQL�ַ���ǰ��ɫ
	StyleSetForeground(wxSTC_SQL_OPERATOR, wxColour(0x00BCBEC4)); // SQL��������ǰ��ɫ
	StyleSetBold(wxSTC_SQL_OPERATOR, true); // SQL�������Ӵ�
	StyleSetForeground(wxSTC_SQL_IDENTIFIER, wxColour(0x00BCBEC4));// SQL��ʶ����ǰ��ɫ

	// - Color Of Selection (ѡ�е���ɫ)
	SetSelBackground(true, wxColour(49, 106, 197)); // ѡ�������ò����ñ���ɫ
	SetSelForeground(true, wxColour(255, 255, 255)); // ѡ�������ò�����ǰ��ɫ

	// Set Keywords
	wxString keywords(sqlKeyWords);
	SetKeyWords(0, keywords);
	// Color Of Keyword
	StyleSetForeground(wxSTC_SQL_WORD, wxColour(0x00ff9966)); // 0x00CF8E6D
	StyleSetForeground(wxSTC_SQL_WORD2, wxColour(0x00ff9966));
	StyleSetForeground(wxSTC_SQL_USER1, wxColour(0x00ff9966));

	// �Զ�ͣ�ٵ��ַ�
	AutoCompStops(autoStopChars);

	// ignore the cmd key for CTRL+[key] ����CTRL+[key]
	int  n = static_cast<int>(sizeof(ignoreCtrlKey));
	for (int i = 0; i < n; i++) {
		CmdKeyClear(ignoreCtrlKey[i], wxSTC_KEYMOD_CTRL);
	}
	
	// Working Fold 
	SetProperty("fold", "1");
	SetProperty("fold.compact", "1");
	SetProperty("fold.html", "1");
	SetProperty("fold.html.preprocessor", "1");
	SetProperty("fold.comment", "1");
	SetProperty("fold.at.else", "1");
	SetProperty("fold.flags", "1");
	SetProperty("fold.preprocessor", "1");
	SetProperty("styling.within.preprocessor", "1");

	// set tab width to 4
	SetTabWidth(4);
}

void QSqlEditor::setDefaultColorFont(int nSize, const char* face)
{
	StyleSetSize(wxSTC_STYLE_DEFAULT, nSize);
	wxFont textFont(wxSize(0, nSize), wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL,  wxFONTWEIGHT_NORMAL, false, face);
	StyleSetFont(wxSTC_STYLE_DEFAULT, textFont);
}

void QSqlEditor::SetStyleBitsEx(int bits)
{
	// #define SCI_SETSTYLEBITS 2090
	SendMsg(2090, bits, 0);
}

void QSqlEditor::UsePopUpEx(int popUpMode)
{
	// #define SCI_USEPOPUP 2371
    SendMsg(2371, popUpMode, 0);
}

void QSqlEditor::updateLineNumberWidth()
{
	//start ��ʾ�к�
	long  iLineMarginWidthNow;
	long  iLineMarginWidthFit;
	long iLineNum = static_cast<long>(GetLineCount());
	long iLineNumCount = 1;
	while (iLineNum != 0) {
		++iLineNumCount;
		iLineNum /= 10;
	}
	iLineMarginWidthNow = static_cast<long>(GetMarginWidth(0));
	long charWidth = static_cast<long>(TextWidth(wxSTC_STYLE_LINENUMBER, "9"));
	iLineMarginWidthFit = (charWidth + 1) * iLineNumCount;
	if (iLineMarginWidthNow != iLineMarginWidthFit) {
		SetMarginWidth(0, iLineMarginWidthFit);
	}
	//end of ��ʾ�к�
}