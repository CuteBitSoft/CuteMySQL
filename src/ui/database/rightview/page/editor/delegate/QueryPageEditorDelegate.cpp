#include "QueryPageEditorDelegate.h"
#include "utils/StringUtil.h"
#include "utils/SqlUtil.h"

QueryPageEditorDelegate::QueryPageEditorDelegate(wxWindow* editor, QueryPageSupplier* supplier)
{
	this->view = editor;
	this->mysupplier = supplier;
}

/**
 * Get tags for different SQL statement such as select sql statement or delete sql statement or update sql statement.
 * 
 * @param line - the current line string
 * @param preline - the string before current position 
 * @param word - current word
 * @param curPosInLine - The current position in the line
 * @return 
 */
std::vector<std::string> QueryPageEditorDelegate::getTags(const std::string & line, const std::string & preline, const std::string & word, size_t curPosInLine)
{
	if (word.empty() || line.empty() || preline.empty()) {
		return std::vector<std::string>();
	}

	auto sqlTags = mysupplier->sqlTags;
	auto functions = databaseService->getSysFunctionStrings(mysupplier->getRuntimeUserConnectId(), mysupplier->getRuntimeSchema(), true);
	sqlTags.insert(sqlTags.end(), functions.begin(), functions.end());
	std::string upline = StringUtil::toupper(line);
	std::string upPreline = StringUtil::toupper(preline);
	std::string upword = StringUtil::toupper(word);

	std::vector<std::string> selTags;
	if (upPreline.find("SELECT") != std::string::npos 
		|| upPreline.find("DELETE") != std::string::npos) {
		selTags = getSelectTags(upline, upPreline, upword, curPosInLine);
		if (!selTags.empty()) {
			return selTags;
		}
	} else if (upPreline.find("UPDATE") != std::string::npos) {
		selTags = getUpdateTags(upline, upPreline, upword, curPosInLine);
		if (!selTags.empty()) {
			return selTags;
		}
	}
	if (word == " ") {
		return sqlTags;
	}
	
	for (auto & str : sqlTags) {
		if (str == upword || str.find(upword) != 0) {
			continue;
		}

		selTags.push_back(str);
	}
	return selTags;
}

std::vector<std::string>& QueryPageEditorDelegate::getCacheUserTableStrings(uint64_t connectId, const std::string & schema)
{
	auto & tblStrs = mysupplier->getCacheUserTableStrings(connectId, schema);
	if (tblStrs.empty()) {
		// Cache to mysupplier
		tblStrs = metadataService->getUserTableStrings(connectId, schema);
		// mysupplier->setCacheUserTableStrings(userDbId, tblStrs);
	}
	return tblStrs;
}


std::vector<std::string> QueryPageEditorDelegate::getSelectTags(const std::string& upline, const std::string& upPreline, const std::string& upword, size_t curPosInLine)
{
	ATLASSERT(!upline.empty() && !upPreline.empty());
	std::vector<std::string> tags, selTags;
	auto words = StringUtil::splitByBlank(upPreline);
	if (words.empty()) {
		return selTags;
	}
	int n = static_cast<int>(words.size());
	std::string& lastWord = words.back();
	wchar_t lastCharOfLastWord = upword.back();
	wchar_t lastCharOfPrevWord = 0;
	std::string prevWord;
	if (n > 1) {
		prevWord = words.at(n - 2);
		lastCharOfPrevWord = prevWord.back();
	}
	
	size_t fromPos, wherePos, groupPos, orderPos, limitPos, npos;
	npos = std::string::npos;
	fromPos = upline.find(" FROM ");
	wherePos = upline.find(" WHERE ");
	groupPos = upline.find(" GROUP ");
	orderPos = upline.find(" ORDER ");
	limitPos = upline.find(" LIMIT ");
	if (lastWord == "FROM" || prevWord == "FROM"
		|| (wherePos != npos && curPosInLine < wherePos)
		|| (wherePos == npos && groupPos != npos && curPosInLine < groupPos)
		|| (wherePos == npos && groupPos == npos
			&& orderPos != npos && curPosInLine < orderPos)
		|| (wherePos == npos && groupPos == npos && orderPos == npos
			&& limitPos != npos && curPosInLine < limitPos)
		|| (wherePos == npos && groupPos == npos && orderPos == npos && limitPos == npos)) { // columns
		tags = getCacheUserTableStrings(mysupplier->getRuntimeUserConnectId(), mysupplier->getRuntimeSchema());
	}
	else if (lastWord == "ON" || prevWord == "ON"
		|| lastWord == "WHERE" || prevWord == "WHERE"
		|| lastWord == "BY" || prevWord == "BY"
		|| lastWord == "AND" || prevWord == "AND"
		|| lastWord == "OR" || prevWord == "OR"
		|| lastCharOfLastWord == '.' || lastCharOfPrevWord == '.'
		|| lastCharOfLastWord == '>' || lastCharOfPrevWord == '>'
		|| lastCharOfLastWord == '=' || lastCharOfPrevWord == '='
		|| lastCharOfLastWord == '<' || lastCharOfPrevWord == '<'
		|| lastWord == ">=" || prevWord == ">="
		|| lastWord == "<=" || prevWord == "<=") { // fields
		TableAliasVector tblAliasVec = SqlUtil::parseTableClauseFromSelectSql(upline);

		if (tblAliasVec.empty()) {
			return selTags;
		}
		size_t dotPos = lastWord.find_last_of(".");
		if (lastCharOfLastWord == '.' || lastCharOfPrevWord == '.'
			|| dotPos != std::string::npos) {
			std::string tblAlias = lastWord.substr(0, dotPos);
			for (TableAlias& item : tblAliasVec) {
				if (item.tbl == tblAlias || item.alias == tblAlias) {
					tags = getCacheTableColumns(
						mysupplier->getRuntimeUserConnectId(), 
						mysupplier->getRuntimeSchema(),
						mysupplier->getRuntimeTblName());
				}
			};
		}
		else {
			tags = getCacheTableColumns(
				mysupplier->getRuntimeUserConnectId(),
				mysupplier->getRuntimeSchema(), 
				tblAliasVec.at(0).tbl);
		}
	}

	for (auto& str : tags) {
		std::string upstr = StringUtil::toupper(str);
		if (upstr == upword || upstr.find(upword) != 0) {
			continue;
		}

		selTags.push_back(str);
	}
	return selTags;
}

std::vector<std::string> QueryPageEditorDelegate::getUpdateTags(const std::string& upline, const std::string& upPreline, const std::string& upword, size_t curPosInLine)
{
	ATLASSERT(!upline.empty() && !upPreline.empty());
	std::vector<std::string> tags, selTags;
	auto words = StringUtil::splitByBlank(upPreline);
	if (words.empty()) {
		return selTags;
	}

	int n = static_cast<int>(words.size());
	uint64_t connectId = mysupplier->getRuntimeUserConnectId();
	auto schema = mysupplier->getRuntimeSchema();
	std::string & lastWord = words.back();
	wchar_t lastCharOfLastWord = upword.back();
	
	std::string prevWord;
	wchar_t lastCharOfPrevWord = 0;
	if (n > 1) {
		prevWord = words.at(n - 2);
		lastCharOfPrevWord = prevWord.back();
	}

	
	size_t npos = std::string::npos;
	size_t setPos = upline.find(" SET ");
	size_t wherePos = upline.find(" WHERE ");
	if (lastWord == "UPDATE" || prevWord == "UPDATE"  || 
		(setPos != npos && curPosInLine < setPos) || 
		(setPos == npos && wherePos == npos)) {
		tags = getCacheUserTableStrings(connectId, schema);
	} else if (lastWord == "ON" || prevWord == "ON"
		|| lastWord == "WHERE" || prevWord == "WHERE"
		|| lastWord == "BY" || prevWord == "BY"
		|| lastWord == "AND" || prevWord == "AND"
		|| lastWord == "OR" || prevWord == "OR"
		|| lastWord == "SET" || prevWord == "SET"
		|| lastCharOfLastWord == ',' || lastCharOfPrevWord == ','
		|| lastCharOfLastWord == '.' || lastCharOfPrevWord == '.'
		|| lastCharOfLastWord == '>' || lastCharOfPrevWord == '>'
		|| lastCharOfLastWord == '=' || lastCharOfPrevWord == '='  
		|| lastCharOfLastWord == '<' || lastCharOfPrevWord == '<'
		|| lastWord == ">=" || prevWord == ">="
		|| lastWord == "<=" || prevWord == "<=") { // fields

		TableAliasVector tblAliasVec = SqlUtil::parseTableClauseFromUpdateSql(upline);
		if (tblAliasVec.empty()) {
			return selTags;
		}
		size_t dotPos = lastWord.find_last_of(".") ;
		if (lastCharOfLastWord == '.' || lastCharOfPrevWord == '.'
			|| dotPos != std::string::npos) {			
			std::string tblAlias = lastWord.substr(0, dotPos);
			for(TableAlias & item : tblAliasVec) {
				if (item.tbl == tblAlias || item.alias == tblAlias) {
					tags = getCacheTableColumns(connectId, schema, item.tbl);
				}
			};
		}else {
			tags = getCacheTableColumns(connectId, schema, tblAliasVec.at(0).tbl);
		}
	}

	for (auto & str : tags) {
		std::string upstr = StringUtil::toupper(str);
		if (upstr == upword || upstr.find(upword) != 0) {
			continue;
		}

		selTags.push_back(str);
	}
	return selTags;
}

Columns& QueryPageEditorDelegate::getCacheTableColumns(uint64_t connectId, const std::string& schema , const std::string& tblName)
{
	auto & columns = mysupplier->getCacheTableColumns(connectId, schema, tblName);
	if (columns.empty()) {
		columns = metadataService->getUserColumnStrings(connectId, schema, tblName);
		// mysupplier->setCacheTableColumns(userDbId, tblName, tags);
	}
	return columns;
}
