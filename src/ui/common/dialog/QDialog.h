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
 * @file   QDialog.h
 * @brief  include the component object :
 *          supplier - subclass object of QSupplier
 *          delegate - subclass object of QDelegate 
 * 
 * @author Xuehan Qin (qinxuehan2018@gmail.com) 
 * @date   2024-11-14
 *********************************************************************/
#pragma once
#include "wx/wx.h"
#include <wx/dialog.h>
#include <wx/colour.h>
#include "ui/common/supplier/EmptySupplier.h"

// template parameter:T - subclass of QDelegate  S - subclass of QSupplier
template <typename T, typename S = EmptySupplier>
class QDialog :  public wxDialog
{
public:
	QDialog();
	~QDialog();

	QDialog(wxWindow* parent,
		wxWindowID id = 0,
		const wxString& caption = _("QDialog"),
		const wxPoint& pos = wxDefaultPosition,
		const wxSize& size = wxDefaultSize,
		long style = wxCAPTION | wxCLOSE_BOX);

	/// Creation
	bool Create(wxWindow* parent,
		wxWindowID id = 0,
		const wxString& caption = _("QDialog"),
		const wxPoint& pos = wxDefaultPosition,
		const wxSize& size = wxDefaultSize,
		long style = wxCAPTION | wxCLOSE_BOX);
protected:
	T* delegate; // the delegate for Dialog 
	S* supplier; // the supplier for Dialog 

	wxColour bkgColor;
	wxColour textColor;

	virtual void init();
	virtual void createControls();
};

template <typename T, typename S>
QDialog<T, S>::QDialog() : wxDialog()
{
	init();
}

template <typename T, typename S>
QDialog<T, S>::~QDialog()
{
	T::destroyInstance();
	delegate = nullptr;

	S::destroyInstance();
	supplier = nullptr;
	
}

template <typename T, typename S>
QDialog<T, S>::QDialog(wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style)
{
	init();
	Create(parent, id, caption, pos, size, style);
}

template <typename T, typename S>
bool QDialog<T, S>::Create(wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style)
{
	// dialog
	// SetExtraStyle(wxWS_EX_BLOCK_EVENTS|wxDIALOG_EX_CONTEXTHELP);

	if (!wxDialog::Create(parent, id, caption, pos, size, style))
		return false;

	createControls();

	// This fits the dialog to the minimum size dictated by
	// the sizers

	GetSizer()->Fit(this);

	// This ensures that the dialog cannot be sized smaller
	// than the minimum size

	GetSizer()->SetSizeHints(this);

	// Centre the dialog on the parent or (if none) screen

	Centre();

	return true;
}

template <typename T, typename S>
void QDialog<T, S>::init()
{
	bkgColor = { 43, 45, 48, 43 };
	textColor = { 223, 225, 229, 213 };

	// singleton supplier
	supplier = S::getInstance();
	// singleton delegate
	delegate = T::getInstance(this, supplier);
	
}

template <typename T, typename S>
void QDialog<T, S>::createControls()
{
	SetBackgroundColour(bkgColor);
}



