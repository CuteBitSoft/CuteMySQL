#include "QListView.h"
#include "core/common/Lang.h"
#include "utils/Log.h"

QListView::QListView()
	: wxListView(), rowBkgColor1(30, 31, 34, 30), rowBkgColor2(46, 46, 46, 43), textColor(223, 225, 229, 213)
{
	 // Bind(wxEVT_LIST_CACHE_HINT, &QListView::OnListCacheHint, this);
	 Bind(wxEVT_LIST_ITEM_CHECKED, &QListView::OnListItemChecked, this);
	 Bind(wxEVT_LIST_ITEM_UNCHECKED, &QListView::OnListItemUnChecked, this);
}

void QListView::SetDataList(const DataList* dataList)
{
	cachData.clear();
	this->dataList = dataList;
	SetItemCount(dataList->size());
}

wxItemAttr* QListView::OnGetItemAttr(long item) const
{
	auto itemAttrPtr = wxListView::OnGetItemAttr(item);
	if (itemAttrPtr == nullptr) {
		static wxItemAttr rowItemAttr;
		rowItemAttr.SetBackgroundColour(item % 2 ? rowBkgColor1 : rowBkgColor2);
		rowItemAttr.SetTextColour(textColor);
		rowItemAttr.SetFont(FTBP("elem-size", true, 12));

		return &rowItemAttr;
	}

	itemAttrPtr->SetBackgroundColour(item % 2 ? rowBkgColor1 : rowBkgColor2);
	itemAttrPtr->SetTextColour(textColor);
	itemAttrPtr->SetFont(FTBP("elem-size", true, 12));

	return itemAttrPtr;
	
}

wxItemAttr* QListView::OnGetItemColumnAttr(long item, long column) const
{
	auto itemAttrPtr = wxListView::OnGetItemColumnAttr(item, column);
	if (itemAttrPtr == nullptr) {
		static wxItemAttr colItemAttr;

		colItemAttr.SetBackgroundColour(item % 2 ? rowBkgColor1 : rowBkgColor2);
		colItemAttr.SetTextColour(textColor);
		colItemAttr.SetFont(FTBP("elem-size", true, 12));

		return &colItemAttr;
	}
    
	itemAttrPtr->SetBackgroundColour(item % 2 ? rowBkgColor1 : rowBkgColor2);
	itemAttrPtr->SetTextColour(textColor);
	itemAttrPtr->SetFont(FTBP("elem-size", true, 12));

	return itemAttrPtr;
}

wxString QListView::OnGetItemText(long item, long column) const
{	
	if (cachData.empty()) {
		return wxEmptyString;
	}
	std::pair<uint32_t, uint32_t> pair(item, column);
	return cachData.at(pair);
}

bool QListView::OnGetItemIsChecked(long WXUNUSED(item)) const
{
	return false;
}

int QListView::OnGetItemImage(long WXUNUSED(item)) const
{
	return -1;
}

int QListView::OnGetItemColumnImage(long WXUNUSED(item), long WXUNUSED(column)) const
{
	return -1;
}

void QListView::OnListCacheHint(wxListEvent& event)
{
	Q_INFO("OnListCacheHint: cache items from {} to {}", event.GetCacheFrom(), event.GetCacheTo());
	
	if (dataList->empty()) {
		return ;
	}
	long nFrom = event.GetCacheFrom();
	long nTo = event.GetCacheTo();

	auto iter = dataList->begin();
	for (int i = 0; i < nFrom; ++i) {
		iter++;
	}

	size_t nCols = iter->size();
	for (long i = nFrom; i < nTo + 1; ++i) {
		const auto& rowItem = *iter;
		for (size_t j = 0; j < nCols; ++j) {
			std::pair<uint32_t, uint32_t> pair(i, j);
			cachData[pair] = rowItem.at(j).c_str();
		}
		iter++;
	}
}

void QListView::OnListItemChecked(wxListEvent& event)
{
	Select(event.GetIndex(), true);
}

void QListView::OnListItemUnChecked(wxListEvent& event)
{
	Select(event.GetIndex(), false);
}
