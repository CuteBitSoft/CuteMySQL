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
// 忽略CTRL+[key]...的功能
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
	// - lex setup (lex语法解释器配置)
	SetLexer(wxSTC_LEX_SQL); // 选择SQL解释器
	// Divide each styling byte into lexical class bits (default: 5) and indicator
	// bits (default: 3). If a lexer requires more than 32 lexical states, then this
	// is used to expand the possible states.
	// 将每个样式字节划分为词法类位（默认值：5）和指示符位（默认值：3）。
	// 如果词法分析器需要超过 32 个词法状态，则用于扩展可能的状态。
	// SetStyleBitsEx(5);
	StyleSetForeground(wxSTC_STYLE_DEFAULT, wxColour(255, 0, 0)); // 编辑器的文本默认的前景色（文本默认的颜色）
	StyleSetBackground(wxSTC_STYLE_DEFAULT, bkgColor); // 编辑器默认的背景色
	StyleClearAll(); // 清理编辑器所有的样式


	// - Other
	SetIndent(4); // 缩进4字符
	SetIndentationGuides(wxSTC_IV_LOOKBOTH); // 显示或隐藏缩进参考线
	UsePopUpEx(true); // 设置当用户在某些区域上按错鼠标按钮时是否自动显示弹出菜单
	
	// Error marker
	MarkerDefine(0, wxSTC_MARK_ARROW); // 设置用于箭头标记编号的符号，以及（可选）前景色(第三参数)和背景色（第四参数）
	MarkerSetBackground(0, wxColour(255, 255, 255)); // 设置第0个Marker的背景色
	MarkerSetForeground(0, wxColour(0, 0, 0)); // 设置第0个Marker的前景色

	// Font And Size 编辑器的字体和大小
	wxFont font(wxSize(0, nSize), wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, face);
	StyleSetFont(wxSTC_STYLE_DEFAULT, font);
	StyleSetSize(wxSTC_STYLE_DEFAULT, nSize);

	// - Margins
    // number margin 
	SetMarginType(0, wxSTC_MARGIN_NUMBER); // 行号边距，将边距设置为数字。此处设置第0个边距是行号数字
	SetMarginWidth(0, 37); // 设置边距宽度，第0个边距边宽
	// SetMarginBackground(0, bkgColor); // 注意，这里不生效的原因是SetMarginBackground只对SC_MARGIN_COLOUR类型的margin生效，这里第0边距是wxSTC_MARGIN_NUMBER
	

	// folding margin 折叠线边距
	SetMarginMask(1, wxSTC_MASK_FOLDERS);  // 设置第2个边距为折叠标记
	SetMarginWidth(1, 12); // 设置边距宽度，第2个边距边宽
	SetMarginSensitive(1, true); // 使第2个边距对鼠标单击敏感或不敏感。

	// - Choose folding icons 选择折叠的小图标
	MarkerDefine(wxSTC_MARKNUM_FOLDEROPEN, wxSTC_MARK_BOXMINUS); // 定义折叠线打开的图标:方框减号
	MarkerDefine(wxSTC_MARKNUM_FOLDER, wxSTC_MARK_BOXPLUS); // 定义折叠线收缩的图标:方框加号
	MarkerDefine(wxSTC_MARKNUM_FOLDERSUB, wxSTC_MARK_VLINE); // 定义子折叠线:VLINE
	MarkerDefine(wxSTC_MARKNUM_FOLDERTAIL, wxSTC_MARK_LCORNER); // 定义折叠线结束的图标:L型拐角
	MarkerDefine(wxSTC_MARKNUM_FOLDEREND, wxSTC_MARK_BOXPLUSCONNECTED); // 定义折叠线结束的图标:方框加号连接
	MarkerDefine(wxSTC_MARKNUM_FOLDEROPENMID, wxSTC_MARK_BOXMINUSCONNECTED); // 定义折叠线打开中间的图标:方框减号连接
	MarkerDefine(wxSTC_MARKNUM_FOLDERMIDTAIL, wxSTC_MARK_LCORNERCURVE); // 定义折叠线中间收尾的图标:T型拐角曲线

	// - Choose folding icon colours 选择折叠的小图标颜色
	MarkerSetForeground(wxSTC_MARKNUM_FOLDEROPEN, textColor); // 设置折叠线打开小图标的前景色
	MarkerSetBackground(wxSTC_MARKNUM_FOLDEROPEN, bkgColor); // 设置折叠线打开小图标的背景色
	MarkerSetForeground(wxSTC_MARKNUM_FOLDER, textColor); // 设置折叠线收缩小图标的前景色
	MarkerSetBackground(wxSTC_MARKNUM_FOLDER, bkgColor); // 设置折叠线收缩小图标的背景色
	MarkerSetForeground(wxSTC_MARKNUM_FOLDERSUB, textColor); // 设置子折叠线的背景色
	MarkerSetBackground(wxSTC_MARKNUM_FOLDERSUB, textColor); // 设置子折叠线的背景色
	MarkerSetForeground(wxSTC_MARKNUM_FOLDERTAIL, textColor); // 设置折叠线收缩结束小图标的背景色
	MarkerSetBackground(wxSTC_MARKNUM_FOLDERTAIL, textColor); // 设置折叠线收缩结束小图标的背景色
	MarkerSetForeground(wxSTC_MARKNUM_FOLDEREND, textColor); // 设置折叠线结束小图标的前景色
	MarkerSetBackground(wxSTC_MARKNUM_FOLDEREND, bkgColor); // 设置折叠线结束小图标的背景色
	MarkerSetForeground(wxSTC_MARKNUM_FOLDEROPENMID, textColor); // 设置折叠线打开中间小图标的前景色
	MarkerSetBackground(wxSTC_MARKNUM_FOLDEROPENMID, bkgColor); // 设置折叠线打开中间小图标的背景色
	MarkerSetForeground(wxSTC_MARKNUM_FOLDERMIDTAIL, textColor); // 设置折叠线中间收尾的小图标的前景色
	MarkerSetBackground(wxSTC_MARKNUM_FOLDERMIDTAIL, textColor); // 设置折叠线中间收尾的小图标的背景色

	// 行号文本颜色，仅仅对SetMarginType(0, wxSTC_MARGIN_NUMBER);起作用
	StyleSetForeground(wxSTC_STYLE_LINENUMBER, textColor);
	// 行号背景颜色	
	StyleSetBackground(wxSTC_STYLE_LINENUMBER, bkgColor2);
	// 折叠边背景色
	SetFoldMarginHiColour(true, bkgColor2);
	SetFoldMarginColour(true, bkgColor2);

	// - Set carlet 光标
	// Color of Carlet 光标的颜色
	SetCaretForeground(wxColour(0x00CEC0D6));
	// Width of Carlet 光标大小
	SetCaretWidth(2);
	// set the caret blinking time to 400 milliseconds 光标闪烁间隔
	SetCaretPeriod(400);

	// - Set caret line colour (设置光标所处行的颜色）
	SetCaretLineBackground(wxColour(38, 40, 46)); // 光标所处行的背景色
	SetCaretLineVisible(true); // 显示光标所处行

	// - Comment block （注释块）
	StyleSetForeground(wxSTC_SQL_DEFAULT, wxColour(0, 0, 0)); // 默认SQL的前景色
	StyleSetForeground(wxSTC_SQL_COMMENT, wxColour(32768)); // SQL注释的前景色

	// - Single comment line (注释行)
	StyleSetForeground(wxSTC_SQL_COMMENTLINE, wxColour(32768)); // 默认SQL注释的前景色

	// - SQL number 数字
	StyleSetForeground(wxSTC_SQL_NUMBER, wxColour(0x002AACB8)); // SQL数字的前景色
	StyleSetBold(wxSTC_SQL_NUMBER, true); // SQL数字加粗

	// - SQL string/operator/identifier (字符串/操作符/标识符)
	StyleSetForeground(wxSTC_SQL_STRING, wxColour(0x00cc99ff)); // SQL字符串的前景色
	StyleSetForeground(wxSTC_SQL_CHARACTER, wxColour(0x00cc99ff)); // SQL字符的前景色
	StyleSetForeground(wxSTC_SQL_OPERATOR, wxColour(0x00BCBEC4)); // SQL操作符的前景色
	StyleSetBold(wxSTC_SQL_OPERATOR, true); // SQL操作符加粗
	StyleSetForeground(wxSTC_SQL_IDENTIFIER, wxColour(0x00BCBEC4));// SQL标识符的前景色

	// - Color Of Selection (选中的颜色)
	SetSelBackground(true, wxColour(49, 106, 197)); // 选中项启用并设置背景色
	SetSelForeground(true, wxColour(255, 255, 255)); // 选中项启用并设置前景色

	// Set Keywords
	wxString keywords(sqlKeyWords);
	SetKeyWords(0, keywords);
	// Color Of Keyword
	StyleSetForeground(wxSTC_SQL_WORD, wxColour(0x00ff9966)); // 0x00CF8E6D
	StyleSetForeground(wxSTC_SQL_WORD2, wxColour(0x00ff9966));
	StyleSetForeground(wxSTC_SQL_USER1, wxColour(0x00ff9966));

	// 自动停顿的字符
	AutoCompStops(autoStopChars);

	// ignore the cmd key for CTRL+[key] 忽略CTRL+[key]
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
	//start 显示行号
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
	//end of 显示行号
}