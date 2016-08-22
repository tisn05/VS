/*
	scspPinDialog.cpp

	This file was created by Adobe engineer.
	This is intended to be made publicly available after legal review.
	Copyright (C) Adobe Systems 2006.
*/

#include "stdafx.h"
#include "scspResources.h"
#include "scspPinDialog.h"

using namespace scsp;
using namespace sample;

// global buffer for PIN data - NOT THREADSAFE!
std::auto_ptr<sample::Buffer> gPinBuffer( new Buffer );

// Message handler for PIN dialog
static LRESULT CALLBACK PinDlgProc( HWND hDlg, UINT message, WPARAM wParam, LPARAM /*lParam*/ )
{
	WORD pinLen = 0;
	BYTE* pin = NULL;
	unsigned int count = 0;

	switch (message)
	{
	case WM_INITDIALOG:
		{
			SendMessage( hDlg, DM_SETDEFID, (WPARAM) IDCANCEL, (LPARAM) 0 );
			SetFocus( GetDlgItem( hDlg, IDC_PIN ) );
			HWND desktop = GetParent( hDlg );
			RECT desktopRect;
			GetWindowRect( desktop, &desktopRect );
			RECT dialogRect;
			GetWindowRect( hDlg, &dialogRect );
			MoveWindow( hDlg, 
				(desktopRect.right + desktopRect.left - dialogRect.right + dialogRect.left) / 2,
				(desktopRect.bottom + desktopRect.top - dialogRect.bottom + dialogRect.top) / 2,
				dialogRect.right - dialogRect.left,
				dialogRect.bottom - dialogRect.top,
				false );
		}
		return TRUE;

	case WM_COMMAND:
		if( HIWORD (wParam) == EN_CHANGE && LOWORD(wParam) == IDC_PIN ) {
			SendMessage( hDlg, DM_SETDEFID, (WPARAM) IDOK, (LPARAM) 0 ); 
		}
		switch( wParam ) 
		{ 
		case IDOK: 
			// get size of the pin
			pinLen = (WORD) SendDlgItemMessage( hDlg, IDC_PIN, EM_LINELENGTH, (WPARAM) 0, (LPARAM) 0 ); 
			if( MAX_PIN_CHARS <= pinLen ) { 
				MessageBox( NULL, L"Too many characters.", L"Error", MB_OK ); 
				return FALSE; 
			} 

			gPinBuffer->resize( ( pinLen + 1 ) * sizeof(TCHAR) );

			// get the pin
			pin = gPinBuffer->begin();
			count = GetDlgItemText(hDlg, IDC_PIN, reinterpret_cast<LPWSTR>(pin), pinLen + 1);
			pin[ count * sizeof(TCHAR) ] = '\0'; 

			// close the dialog
			EndDialog( hDlg, TRUE ); 
			return TRUE; 

		case IDCANCEL: 
			EndDialog( hDlg, TRUE ); 
			return TRUE; 
		} 
		break;
	}

	return FALSE;
}

bool scsp::getPinFromUser( Buffer& outPin, HWND inHWND )
{
	HWND windowToUse = inHWND ? inHWND : GetForegroundWindow();
	if( IDOK == DialogBox( scsp::getGlobalData().m_Instance, MAKEINTRESOURCE(IDD_PIN_DIALOG), windowToUse, (DLGPROC)PinDlgProc ) ) {
		Buffer pin;
		const size_t actualPinLen = gPinBuffer->size();
		pin.resize( actualPinLen );
		BYTE* actualPin = gPinBuffer->begin();
		memmove( pin.begin(), actualPin, actualPinLen );
#if UNICODE
		outPin.swap( pin );
#else
#error XXX implement conversion to wide chars
#endif
		return true;
	}
	
	return false;
}

