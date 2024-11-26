#pragma once
#include "wx/wx.h"
#include "wx/panel.h"
#include <wx/colour.h>
#include "ui/common/supplier/EmptySupplier.h"

template <typename S = EmptySupplier>
class QPanel :  public wxPanel
{
public:
	QPanel();
	~QPanel();

	QPanel(wxWindow* parent,
		wxWindowID id = 0,
		const wxPoint& pos = wxDefaultPosition,
		const wxSize& size = wxDefaultSize,
		long style = wxCAPTION | wxCLOSE_BOX);

	// Creation
	bool Create(wxWindow* parent,
		wxWindowID id = 0,
		const wxPoint& pos = wxDefaultPosition,
		const wxSize& size = wxDefaultSize,
		long style = wxCAPTION | wxCLOSE_BOX);

	// load data for controls, It must be called after createControls 
	virtual void loadControls();
	// clear data for controls, It must be called after createControls 
	virtual void clearControls();
	// exchange data from controls to supplier runtime data member variable of XXXSupplier
	virtual void exchangeData();

	void setRuntimeDataDirty(bool dirty = true);
	bool getRuntimeDataDirty();
protected:
	S* supplier;
	wxColour bkgColor;
	wxColour textColor;

	wxBoxSizer* topSizer;

	virtual void init();
	// create controls for UI
	virtual void createControls();	

	wxTextCtrl* createInputControl(wxWindowID editId, const wxString& labelText, wxWindowID labelId = wxID_STATIC, const wxSize& editSize = wxSize(300, -1), long editStyle = 0);
	wxTextCtrl* createInputFileControl(wxWindowID editId, wxWindowID buttonId, const wxString& labelText, wxWindowID labelId = wxID_STATIC, const wxSize& editSize = wxSize(260, -1), long editStyle = 0);
	// when the runtime data has change in the subclass of controls ,will be binded the event function
	void OnRuntimeDataHasChanged(wxCommandEvent& event);
};


template <typename S>
QPanel<S>::QPanel() : bkgColor(43, 45, 48, 43), textColor(223, 225, 229, 213)
{
	supplier = S::getInstance();
	// 注意:基类构造函数，只调用基类对象的init函数(QPanel::init)，而不是子类的init函数, by <<Effective C++>>
	// Note: The base class constructor only calls the init function of the base class object (QPanel::init), not the init function of the subclass, by <<Effective C++>>
	init();	
}

template <typename S>
QPanel<S>::QPanel(wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style)
	: bkgColor(43, 45, 48, 43), textColor(223, 225, 229, 213)
{
	supplier = S::getInstance();
	init();
	Create(parent, id, pos, size, style);
}

template <typename S>
QPanel<S>::~QPanel()
{
	S::destroyInstance();
	supplier = nullptr;
}
template <typename S>
bool QPanel<S>::Create(wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style)
{
	if (!wxPanel::Create(parent, id, pos, size, style))
		return false;

	topSizer = new wxBoxSizer(wxVERTICAL);
	createControls();
	SetSizer(topSizer, true);

	// This fits the dialog to the minimum size dictated by
	// the sizers

	GetSizer()->Fit(this);

	// This ensures that the dialog cannot be sized smaller
	// than the minimum size

	GetSizer()->SetSizeHints(this);

	loadControls();
	return true;
}

template <typename S>
void QPanel<S>::init()
{
	supplier = S::getInstance();
}

template <typename S>
void QPanel<S>::createControls()
{
	SetBackgroundColour(bkgColor);
	SetForegroundColour(textColor);
}

template <typename S>
void QPanel<S>::loadControls()
{
	
}


template <typename S /*= EmptySupplier*/>
void QPanel<S>::clearControls()
{
	auto childControls = this->GetChildren();
	for (const auto& win : childControls) {
		if (win->IsKindOf(CLASSINFO(wxTextCtrl))) {
			((wxTextCtrl*)win)->Clear();
		}
		else if (win->IsKindOf(CLASSINFO(wxCheckBox))) {
			((wxCheckBox*)win)->SetValue(false);
		}
	}
}

template <typename S /*= EmptySupplier*/>
void QPanel<S>::exchangeData()
{

}

template <typename S /*= EmptySupplier*/>
void QPanel<S>::setRuntimeDataDirty(bool dirty)
{
	supplier->setIsDirty(dirty);
}

template <typename S /*= EmptySupplier*/>
bool QPanel<S>::getRuntimeDataDirty()
{
	return supplier->getIsDirty();
}

template <typename S>
wxTextCtrl* QPanel<S>::createInputControl(wxWindowID editId,  const wxString& labelText, wxWindowID labelId, const wxSize& editSize /*= wxSize(300, -1)*/, long editStyle /*= 0*/)
{
	topSizer->Add(3, 6, 0, wxALIGN_CENTRE_HORIZONTAL | wxALL, 5);

	wxBoxSizer* horizontalSizer = new wxBoxSizer(wxHORIZONTAL);
	topSizer->Add(horizontalSizer, 0, wxALIGN_TOP, 3);

	horizontalSizer->AddSpacer(10);

	wxStaticText* label = new wxStaticText(this, labelId, labelText, wxDefaultPosition, { 130, -1 }, wxALIGN_LEFT);
	horizontalSizer->Add(label, 0, wxALIGN_CENTER_VERTICAL | wxALIGN_LEFT, 3);
	horizontalSizer->AddSpacer(5);

	wxBoxSizer* rightSizer = new wxBoxSizer(wxHORIZONTAL);
	horizontalSizer->Add(rightSizer, 1, wxALIGN_CENTER_VERTICAL | wxALIGN_LEFT);

	wxTextCtrl* text = new wxTextCtrl(this, editId, wxEmptyString, wxDefaultPosition, editSize, editStyle);
	rightSizer->Add(text, 0, wxALIGN_CENTER_VERTICAL | wxALIGN_LEFT, 3);

	return text;
}

template <typename S>
wxTextCtrl* QPanel<S>::createInputFileControl(wxWindowID editId, wxWindowID buttonId, const wxString& labelText, wxWindowID labelId, const wxSize& editSize /*= wxSize(300, -1)*/, long editStyle /*= 0*/)
{
	topSizer->Add(3, 6, 0, wxALIGN_CENTRE_HORIZONTAL | wxALL, 5);

	wxBoxSizer* horizontalSizer = new wxBoxSizer(wxHORIZONTAL);
	topSizer->Add(horizontalSizer, 0, wxALIGN_TOP, 3);

	horizontalSizer->AddSpacer(10);

	wxStaticText* label = new wxStaticText(this, labelId, labelText, wxDefaultPosition, { 130, -1 }, wxALIGN_LEFT);
	horizontalSizer->Add(label, 0, wxALIGN_CENTER_VERTICAL | wxALIGN_LEFT, 3);
	horizontalSizer->AddSpacer(5);

	wxBoxSizer* rightSizer = new wxBoxSizer(wxHORIZONTAL);
	horizontalSizer->Add(rightSizer, 1, wxALIGN_CENTER_VERTICAL | wxALIGN_LEFT);

	wxTextCtrl* text = new wxTextCtrl(this, editId, wxEmptyString, wxDefaultPosition, editSize, editStyle);
	rightSizer->Add(text, 0, wxALIGN_CENTER_VERTICAL | wxALIGN_LEFT, 3);

	rightSizer->AddSpacer(10);
	wxButton* button = new wxButton(this, buttonId, _("..."), wxDefaultPosition, { 30, 20 });
	rightSizer->Add(button, 0, wxALIGN_CENTER_VERTICAL | wxALIGN_LEFT, 3);

	return text;
}

template <typename S>
void QPanel<S>::OnRuntimeDataHasChanged(wxCommandEvent& event)
{
	setRuntimeDataDirty(true);
}
