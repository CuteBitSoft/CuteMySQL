#include "ResultListPage.h"
#include "common/Config.h"
#include "utils/ResourceUtil.h"
#include "utils/PerformUtil.h"
#include "core/entity/Entity.h"
#include "core/common/Lang.h"
#include "ui/common/msgbox/QAnimateBox.h"
#include "core/common/exception/QSqlExecuteException.h"

BEGIN_EVENT_TABLE(ResultListPage, wxPanel)
	EVT_CHECKBOX(Config::SELECT_ALL_BUTTON_ID, OnClickCheckBox)
	EVT_BUTTON(Config::LISTVIEW_EXPORT_BUTTON_ID, OnClickExportButton)
	EVT_BUTTON(Config::LISTVIEW_COPY_BUTTON_ID, OnClickCopyButton)
END_EVENT_TABLE()

ResultListPage::ResultListPage(QueryPageSupplier* supplier, const std::string& sql) 
	: QPanel<DatabaseSupplier>(), rowCount(0)
{
	this->mysupplier = supplier;
	this->mysupplier->setCacheUseSql(sql);
	init();
}

ResultListPage::~ResultListPage()
{
	delete delegate;
	delegate = nullptr;
}

void ResultListPage::setup(QueryPageSupplier* supplier, const std::string& sql)
{
	this->mysupplier = supplier;
	this->mysupplier->setCacheUseSql(sql);
}

void ResultListPage::loadListView()
{
	uint64_t connectId = mysupplier->getRuntimeUserConnectId();
	std::string schema = mysupplier->getRuntimeSchema();
	if (!connectId) {
		QAnimateBox::warning(S("no-select-userdb"));
		return;
	}
	
	auto _begin = PerformUtil::begin();
	try {
		rowCount = delegate->loadListView(connectId, schema, mysupplier->getCacheUseSql());
	} catch (QSqlExecuteException &ex) {
		QAnimateBox::error(ex);
		rowCount = 0;
	}
	
	ResultInfo & resultInfo = delegate->getRuntimeResultInfo();
	resultInfo.totalTime = PerformUtil::end(_begin);

	// display status bar panels 
	displayStatusBarPanels(resultInfo);
	/*
	int checkedFormView = formViewCheckBox.GetCheck();
	if (checkedFormView) {
		formView.clearLabelsAndEdits();
	}*/
}

void ResultListPage::init()
{
	bkgColor = wxColour(30, 31, 34, 30);
	imgdir = ResourceUtil::getProductImagesDir() +"/database/list/button/";
}

void ResultListPage::createControls()
{
	createLayouts();
	createToolbarButtons();
	createListView();
	createStatusBar();

	delegate = new ResultListPageDelegate(this, listView, mysupplier, QUERY_SQL_RESULT);
}

void ResultListPage::createLayouts()
{
	topSizer->AddSpacer(5);

	toolbarHoriLayout = new wxBoxSizer(wxHORIZONTAL);
	topSizer->Add(toolbarHoriLayout, 0, wxALL | wxEXPAND);
	
	toolbarHoriLeftLayout = new wxBoxSizer(wxHORIZONTAL);
	toolbarHoriLayout->Add(toolbarHoriLeftLayout, 0);
	toolbarHoriLayout->AddStretchSpacer(1); // AddStretchSpacer(1), parent boxSizer must be with wxALL | wxEXPAND styles.
	toolbarHoriRightLayout = new wxBoxSizer(wxHORIZONTAL);
	toolbarHoriLayout->Add(toolbarHoriRightLayout, 0);

	topSizer->AddSpacer(5);

}

void ResultListPage::createToolbarButtons()
{
	createToobarLeftButtons(); // left toolbar
	createToolbarMidInputs(); // Middle toolbar
	createToobarRightButtons(); // right toolbar
}

void ResultListPage::createToobarLeftButtons()
{
	toolbarHoriLeftLayout->AddSpacer(5);
	selAllCheckBox = new wxCheckBox(this, Config::SELECT_ALL_BUTTON_ID, S("select-all"), wxDefaultPosition, {-1, 22}, wxCLIP_CHILDREN | wxCLIP_SIBLINGS | wxNO_BORDER);
	selAllCheckBox->SetBackgroundColour(bkgColor);
	selAllCheckBox->SetForegroundColour(textColor);
	toolbarHoriLeftLayout->Add(selAllCheckBox, 0, wxALIGN_TOP | wxALIGN_LEFT);

	auto splitButton = createToolSplit();
	toolbarHoriLeftLayout->Add(splitButton, 0, wxALIGN_TOP | wxALIGN_LEFT);

	wxBitmap normalBitmap(imgdir + "export-button-normal.png", wxBITMAP_TYPE_PNG);
	wxBitmap pressedBitmap(imgdir + "export-button-pressed.png", wxBITMAP_TYPE_PNG); 
	exportButton = new wxBitmapButton(this, Config::LISTVIEW_EXPORT_BUTTON_ID, wxBitmapBundle(normalBitmap), wxDefaultPosition, { 22, 22 }, wxCLIP_CHILDREN | wxNO_BORDER);
	exportButton->SetBackgroundColour(bkgColor);
	exportButton->SetBitmapPressed(pressedBitmap);
	exportButton->SetBitmapFocus(pressedBitmap);
	exportButton->SetToolTip(S("export-result-as"));
	toolbarHoriLeftLayout->Add(exportButton, 0, wxALIGN_TOP | wxALIGN_LEFT);
	
	normalBitmap = wxBitmap(imgdir + "copy-button-normal.png", wxBITMAP_TYPE_PNG);
	pressedBitmap = wxBitmap(imgdir + "copy-button-pressed.png", wxBITMAP_TYPE_PNG);
	copyButton = new wxBitmapButton(this, Config::LISTVIEW_COPY_BUTTON_ID, wxBitmapBundle(normalBitmap), wxDefaultPosition, { 22, 22 }, wxCLIP_CHILDREN | wxNO_BORDER);
	copyButton->SetBackgroundColour(bkgColor);
	copyButton->SetBitmapPressed(pressedBitmap);
	copyButton->SetBitmapFocus(pressedBitmap);
	copyButton->SetToolTip(S("copy-result-data"));
	toolbarHoriLeftLayout->Add(copyButton, 0, wxALIGN_TOP | wxALIGN_LEFT);

}

void ResultListPage::createToolbarMidInputs()
{
	auto splitButton = createToolSplit();
	toolbarHoriLeftLayout->Add(splitButton, 0, wxALIGN_TOP | wxALIGN_LEFT);

	formViewCheckBox = new wxCheckBox(this, Config::LISTVIEW_FORMVIEW_CHECKBOX_ID, S("show-form-view"), wxDefaultPosition, { -1, 22 }, wxCLIP_CHILDREN | wxCLIP_SIBLINGS | wxNO_BORDER);
	formViewCheckBox->SetBackgroundColour(bkgColor);
	formViewCheckBox->SetForegroundColour(textColor);
	toolbarHoriLeftLayout->Add(formViewCheckBox, 0, wxALIGN_TOP | wxALIGN_LEFT);
}

void ResultListPage::createToobarRightButtons()
{
	wxBitmap normalBitmap(imgdir + "filter-button-normal.png", wxBITMAP_TYPE_PNG);
	wxBitmap pressedBitmap(imgdir + "filter-button-pressed.png", wxBITMAP_TYPE_PNG);
	filterButton = new wxBitmapButton(this, Config::LISTVIEW_FILTER_BUTTON_ID, wxBitmapBundle(normalBitmap), wxDefaultPosition, { 22, 22 }, wxCLIP_CHILDREN | wxNO_BORDER);
	filterButton->SetBackgroundColour(bkgColor);
	filterButton->SetBitmapPressed(pressedBitmap);
	filterButton->SetBitmapFocus(pressedBitmap);
	filterButton->SetToolTip(S("filter"));
	toolbarHoriRightLayout->Add(filterButton, 0, wxALIGN_TOP | wxALIGN_LEFT);

	normalBitmap = wxBitmap(imgdir + "refresh-button-normal.png", wxBITMAP_TYPE_PNG);
	pressedBitmap = wxBitmap(imgdir + "refresh-button-pressed.png", wxBITMAP_TYPE_PNG);
	refreshButton = new wxBitmapButton(this, Config::LISTVIEW_REFRESH_BUTTON_ID, wxBitmapBundle(normalBitmap), wxDefaultPosition, { 22, 22 }, wxCLIP_CHILDREN | wxNO_BORDER);
	refreshButton->SetBackgroundColour(bkgColor);
	refreshButton->SetBitmapPressed(pressedBitmap);
	refreshButton->SetBitmapFocus(pressedBitmap);
	refreshButton->SetToolTip(S("refresh"));
	toolbarHoriRightLayout->Add(refreshButton, 0, wxALIGN_TOP | wxALIGN_LEFT);

	auto splitButton = createToolSplit();
	toolbarHoriRightLayout->Add(splitButton, 0, wxALIGN_TOP | wxALIGN_LEFT);

	limitCheckBox = new wxCheckBox(this, Config::LISTVIEW_LIMIT_CHECKBOX_ID, S("limit-rows"), wxDefaultPosition, { -1, 22 }, wxCLIP_CHILDREN | wxCLIP_SIBLINGS | wxNO_BORDER);
	limitCheckBox->SetBackgroundColour(bkgColor);
	limitCheckBox->SetForegroundColour(textColor);
	toolbarHoriRightLayout->Add(limitCheckBox, 0, wxALIGN_TOP | wxALIGN_LEFT);

	offsetLabel = new wxStaticText(this, wxID_ANY, S("offset").append(":"), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT | wxCLIP_CHILDREN | wxCLIP_SIBLINGS | wxNO_BORDER);
	offsetLabel->SetForegroundColour(textColor);
	toolbarHoriRightLayout->Add(offsetLabel, 0, wxALIGN_TOP | wxALIGN_LEFT);
	toolbarHoriRightLayout->AddSpacer(5);
	offsetEdit = new wxTextCtrl(this, Config::LISTVIEW_OFFSET_EDIT_ID, "0", wxDefaultPosition, {36, 20}, wxCLIP_CHILDREN | wxCLIP_SIBLINGS | wxNO_BORDER);
	toolbarHoriRightLayout->Add(offsetEdit, 0, wxALIGN_CENTER_VERTICAL | wxALIGN_LEFT);

	toolbarHoriRightLayout->AddSpacer(10);
	rowsLabel = new wxStaticText(this, wxID_ANY, S("rows").append(":"), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT | wxCLIP_CHILDREN | wxCLIP_SIBLINGS | wxNO_BORDER);
	rowsLabel->SetForegroundColour(textColor);
	toolbarHoriRightLayout->Add(rowsLabel, 0, wxALIGN_TOP | wxALIGN_LEFT);

	toolbarHoriRightLayout->AddSpacer(5);
	rowsEdit = new wxTextCtrl(this, Config::LISTVIEW_LIMIT_EDIT_ID, "1000", wxDefaultPosition, { 36, 20 }, wxCLIP_CHILDREN | wxCLIP_SIBLINGS | wxNO_BORDER);
	toolbarHoriRightLayout->Add(rowsEdit, 0, wxALIGN_CENTER_VERTICAL | wxALIGN_LEFT);

	toolbarHoriRightLayout->AddSpacer(5);
}

void ResultListPage::createListView()
{
	listView = new QListView();	

	// virtual style: wxLC_VIRTUAL
	listView->Create(this, Config::DATABASE_QUERY_LISTVIEW_ID, wxDefaultPosition, wxDefaultSize, 
		wxCLIP_CHILDREN | wxNO_BORDER | wxLC_REPORT | wxLC_ALIGN_LEFT );
	listView->SetBackgroundColour(bkgColor);
	listView->SetTextColour(textColor);
	listView->EnableCheckBoxes(true);
	
	topSizer->Add(listView, 1, wxEXPAND);

}

void ResultListPage::createStatusBar()
{
	statusBar = new wxStatusBar();
	statusBar->Create(this, Config::DATABASE_QUERY_STATUSBAR_ID, wxCLIP_CHILDREN | wxNO_BORDER | wxSTB_SIZEGRIP|wxSTB_SHOW_TIPS|wxSTB_ELLIPSIZE_END|wxFULL_REPAINT_ON_RESIZE);
	const int widths[4]{ -1, -1, -1, 100};
	statusBar->SetFieldsCount(4, widths);
	statusBar->SetStatusWidths(4, widths);
	//statusBar->SetBackgroundColour(bkgColor);
	//statusBar->SetForegroundColour(textColor);
	topSizer->Add(statusBar, 0, wxALIGN_TOP | wxEXPAND);
}

void ResultListPage::initControls()
{
}

// display the result rows and exec time in the status bar
void ResultListPage::displayStatusBarPanels(ResultInfo& resultInfo)
{
	delegate->sendExecSqlMessage(resultInfo);
	displayRuntimeSql();
	displayDatabase(); 
	displayResultRows();
	displayExecTime(resultInfo);
}

void ResultListPage::displayRuntimeSql()
{
	statusBar->SetStatusText(delegate->getRuntimeSql().c_str(), 0);
}

void ResultListPage::displayDatabase()
{
	statusBar->SetStatusText(mysupplier->getRuntimeSchema(), 1);
}

void ResultListPage::displayResultRows()
{
	wxString resultRows = wxString::Format("%d rows", rowCount);
	statusBar->SetStatusText(resultRows, 2);
}

void ResultListPage::displayExecTime(ResultInfo & resultInfo)
{
	wxString execTime = wxString::Format("Exec:%s Transfer:%s Total:%s", 
		resultInfo.execTime.c_str(), resultInfo.transferTime.c_str(), resultInfo.totalTime.c_str());
	statusBar->SetStatusText(execTime, 3);
}

void ResultListPage::OnClickCheckBox(wxCommandEvent& WXUNUSED(event))
{
	bool checked = selAllCheckBox->GetValue();
	int rows = listView->GetItemCount();
	for (int i = 0; i < rows; ++i) {
		listView->CheckItem(i, checked);
	}
}

void ResultListPage::OnClickExportButton(wxCommandEvent& event)
{
	event.Skip();
}

void ResultListPage::OnClickCopyButton(wxCommandEvent& event)
{
	event.Skip();
}
