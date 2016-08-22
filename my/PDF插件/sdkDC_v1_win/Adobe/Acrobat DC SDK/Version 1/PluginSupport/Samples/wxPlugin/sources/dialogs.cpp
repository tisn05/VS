/*********************************************************************

 ADOBE SYSTEMS INCORPORATED
 Copyright (C) 2008 Adobe Systems Incorporated
 All rights reserved.

 NOTICE: Adobe permits you to use, modify, and distribute this file
 in accordance with the terms of the Adobe license agreement
 accompanying it. If you have received this file from a source other
 than Adobe, then your use, modification, or distribution of it
 requires the prior written permission of Adobe.

 ---------------------------------------------------------------------

 dialogs.cpp

 - Simple dialog code for wxWidgets.

*********************************************************************/
#include "wx/app.h"
#include "wx/sizer.h"
#include "wx/checkbox.h"
#include "wx/msgdlg.h"
#include "wx/textdlg.h"
#include "wxInit.h"
#include "dialogs.h"


// My frame constructor
MyFrame::MyFrame(wxWindow *parent,
                 const wxString& title)
       : wxFrame(parent, wxID_ANY, title)
{
}

MyFrame::~MyFrame()
{

}


void MyFrame::OnClose(wxCloseEvent& event)
{
	Show(false);
	Destroy();	
}

BEGIN_EVENT_TABLE(MyFrame, wxFrame)
	EVT_CLOSE(MyFrame::OnClose)
	END_EVENT_TABLE()


// ----------------------------------------------------------------------------
// MyModelessDialog
// ----------------------------------------------------------------------------

MyModelessDialog::MyModelessDialog(wxWindow *parent)
                : wxDialog(parent, wxID_ANY, wxString(_T("Modeless dialog")))
{
    wxBoxSizer *sizerTop = new wxBoxSizer(wxVERTICAL);

    wxButton *btn = new wxButton(this, 3, _T("Press me"));
    wxCheckBox *check = new wxCheckBox(this, wxID_ANY, _T("Should be disabled"));
    check->Disable();

    sizerTop->Add(btn, 1, wxEXPAND | wxALL, 5);
    sizerTop->Add(check, 1, wxEXPAND | wxALL, 5);

    SetSizer(sizerTop);

    sizerTop->SetSizeHints(this);
    sizerTop->Fit(this);
}

void MyModelessDialog::OnButton(wxCommandEvent& WXUNUSED(event))
{
    wxMessageBox(_T("Button pressed in modeless dialog"), _T("Info"),
                 wxOK | wxICON_INFORMATION, this);
}

void MyModelessDialog::OnClose(wxCloseEvent& event)
{
    if ( event.CanVeto() )
    {
        wxMessageBox(_T("Use the menu item to close this dialog"),
                     _T("Modeless dialog"),
                     wxOK | wxICON_INFORMATION, this);

        event.Veto();
    }
}

// ----------------------------------------------------------------------------
// MyModalDialog
// ----------------------------------------------------------------------------
#ifdef WIN_PLATFORM
MyModalDialog::MyModalDialog(wxWindow *parent)
             : wxDialog(parent, wxID_ANY, wxString(_T("Modal dialog")))
#elif MAC_PLATFORM
//remove close box from modal dialog per standard
MyModalDialog::MyModalDialog(wxWindow *parent)
             : wxDialog(parent, wxID_ANY, wxString(_T("Modal dialog")),
			 wxDefaultPosition, wxDefaultSize, wxCAPTION | wxSYSTEM_MENU)
#endif
{
    wxBoxSizer *sizerTop = new wxBoxSizer(wxHORIZONTAL);

    m_btnModal = new wxButton(this, wxID_ANY, _T("&Modal dialog..."));
    m_btnDelete = new wxButton(this, wxID_ANY, _T("&Delete button"));

    wxButton *btnOk = new wxButton(this, wxID_CANCEL, _T("&Close"));
    sizerTop->Add(m_btnModal, 0, wxALIGN_CENTER | wxALL, 5);
    sizerTop->Add(m_btnDelete, 0, wxALIGN_CENTER | wxALL, 5);
    sizerTop->Add(btnOk, 0, wxALIGN_CENTER | wxALL, 5);

    SetSizer(sizerTop);

    sizerTop->SetSizeHints(this);
    sizerTop->Fit(this);

    m_btnModal->SetFocus();
    m_btnModal->SetDefault();
}

BEGIN_EVENT_TABLE(MyModelessDialog, wxDialog)
	EVT_BUTTON(wxID_ANY, MyModelessDialog::OnButton)
        EVT_CLOSE(MyModelessDialog::OnClose)
	END_EVENT_TABLE()
	
void MyModalDialog::OnButton(wxCommandEvent& event)
{
    if ( event.GetEventObject() == m_btnDelete )
    {
        delete m_btnModal;
        m_btnModal = NULL;

        m_btnDelete->Disable();
    }
    else if ( event.GetEventObject() == m_btnModal )
    {
        wxGetTextFromUser(_T("Dummy prompt"),
                          _T("Modal dialog called from dialog"),
                          wxEmptyString, this);
    }
    else
    {
        event.Skip();
    }
}
	
BEGIN_EVENT_TABLE(MyModalDialog, wxDialog)
	EVT_BUTTON(wxID_ANY, MyModalDialog::OnButton)
	END_EVENT_TABLE()