#include "stdafx.h"
#include "MainDlg.h"

#include <Windows.h>
#include <stdio.h>
#include <iostream>
#include <atltypes.h>
#include "atlctrls.h"
#include <atlstr.h>
#include <winuser.h>

using namespace std;
#define TEXTEXIST(str)  str ? str : L""


BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam)
{
	if (IsWindowVisible(hwnd)) {
		unsigned long find_pid = 0;
		GetWindowThreadProcessId(hwnd, &find_pid);

		if (find_pid == lParam) {
		}
	}
	return TRUE;
}


CMainDlg::CMainDlg()
	: m_hProcess(NULL)
	, m_bTopMost(FALSE)
{
}

CMainDlg::~CMainDlg()
{
	if (m_hProcess)
	{
		CloseHandle(m_hProcess); 
		m_hProcess = NULL;
	}
}


LRESULT CMainDlg::OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	// center the dialog on the screen
	CenterWindow();

	// set icons
	HICON hIcon = AtlLoadIconImage(IDR_MAINFRAME, LR_DEFAULTCOLOR, ::GetSystemMetrics(SM_CXICON), ::GetSystemMetrics(SM_CYICON));
	SetIcon(hIcon, TRUE);
	HICON hIconSmall = AtlLoadIconImage(IDR_MAINFRAME, LR_DEFAULTCOLOR, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON));
	SetIcon(hIconSmall, FALSE);

	// set data 
	CEdit editClassName = (CEdit)GetDlgItem(IDC_EDIT_CLASS_NAME);
	editClassName.SetWindowTextW(L"Notepad");

	CEdit editVert = (CEdit)GetDlgItem(IDC_EDIT_VERT);
	editVert.SetWindowTextW(L"50");

	CEdit editHoriz = (CEdit)GetDlgItem(IDC_EDIT_HORIZ);
	editHoriz.SetWindowTextW(L"50");

	return TRUE;
}

LRESULT CMainDlg::OnOK(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	EndDialog(IDOK);
	return 0;
}

LRESULT CMainDlg::OnCancel(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	EndDialog(IDCANCEL);
	return 0;
}



int CMainDlg::UpdateWindowsHandle()
{
	m_hProcess = NULL;
	int ret = UpdateWindowsHandleByName();
	if (ret != 0)
	{
		UpdateWindowsHandleByPid();
	}
	if (!m_hProcess)
	{
		::MessageBox(NULL, L"查找不到进程句柄", L"提示", MB_OK);
		return -1;
	}

	return 0;
}

int CMainDlg::UpdateWindowsHandleByName()
{
	CString strClassName;
	CEdit editClassName = (CEdit)GetDlgItem(IDC_EDIT_CLASS_NAME);
	editClassName.GetWindowText(strClassName);
	CString strWindowsName;
	CEdit editWindowsName = (CEdit)GetDlgItem(IDC_EDIT_WINDOWS_NAME);
	editWindowsName.GetWindowText(strWindowsName);
	HWND hq = NULL;
	if (strClassName.GetLength() > 0 && strWindowsName.GetLength() == 0)
	{
		hq = FindWindow(strClassName, NULL);
	}
	else if (strClassName.GetLength() == 0 && strWindowsName.GetLength() > 0)
	{
		hq = FindWindow(NULL, strWindowsName);
	}
	else
	{
		hq = FindWindow(strClassName, strWindowsName);
	}

	if (!hq)
	{
		return -1;
	}
	m_hProcess = hq;
	return 0;
}

int CMainDlg::UpdateWindowsHandleByPid()
{
	HWND hq = NULL;
	CString strPid;
	CEdit editPid = (CEdit)GetDlgItem(IDC_EDIT_PID);
	editPid.GetWindowText(strPid);
	if (strPid.GetLength() <= 0)
	{
		return -1;
	}
	unsigned long edit_pid = _wtoi(strPid);

	unsigned long find_pid = 0;

	HWND hwnd = GetForegroundWindow();

	while (hwnd) {
		GetWindowThreadProcessId(hwnd, &find_pid);
		if (find_pid != 0) {
			if (find_pid == edit_pid) {
				hq = hwnd;
				break;
			}
		}
		hwnd = ::GetNextWindow(hwnd, GW_HWNDPREV);

	}

	if (!hq)
	{
		return -1;
	}
	m_hProcess = hq;
	return 0;
}


LRESULT CMainDlg::OnBnClickedPosVert(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	UpdateWindowsHandleByPid();
	CString strVertAdd;
	CEdit editVert = (CEdit)GetDlgItem(IDC_EDIT_VERT);
	editVert.GetWindowText(strVertAdd);
	int changepos = _wtoi(strVertAdd);
	RECT rect = { 0 };
	::GetWindowRect(m_hProcess, &rect);
	int w = rect.right - rect.left;
	int h = rect.bottom - rect.top;
	::MoveWindow(m_hProcess, rect.left, rect.top + changepos, w, h, false);
	return 0;
}

LRESULT CMainDlg::OnBnClickedPosHoriz(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	UpdateWindowsHandle();
	CString strVertAdd;
	CEdit editVert = (CEdit)GetDlgItem(IDC_EDIT_HORIZ);
	editVert.GetWindowText(strVertAdd);
	int changepos = _wtoi(strVertAdd);
	RECT rect = { 0 };
	::GetWindowRect(m_hProcess, &rect);
	int w = rect.right - rect.left;
	int h = rect.bottom - rect.top;
	::MoveWindow(m_hProcess, rect.left + changepos, rect.top , w, h, false);
	return 0;
}


LRESULT CMainDlg::OnBnClickedShow(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	UpdateWindowsHandle();
	if (!::IsWindowVisible(m_hProcess))
	{
		::ShowWindow(m_hProcess, SW_SHOW);
	}
	return 0;
}

LRESULT CMainDlg::OnBnClickedHide(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	UpdateWindowsHandle();
	if (::IsWindowVisible(m_hProcess))
	{
		::ShowWindow(m_hProcess, SW_HIDE);
	}
	return 0;
}


LRESULT CMainDlg::OnBnClickedMin(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	UpdateWindowsHandle();
	::ShowWindow(m_hProcess, SW_MINIMIZE);
	return 0;
}



LRESULT CMainDlg::OnBnClickedMax(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	UpdateWindowsHandle();
	::ShowWindow(m_hProcess, SW_MAXIMIZE);

	return 0;
}


LRESULT CMainDlg::OnBnClickedRestore(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	UpdateWindowsHandle();
 	::ShowWindow(m_hProcess, SW_RESTORE);

	
	return 0;
}


LRESULT CMainDlg::OnBnClickedTopmost(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	UpdateWindowsHandle();
	::SetWindowPos(m_hProcess, m_bTopMost ?  HWND_TOPMOST: HWND_NOTOPMOST,0 , 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE | SWP_NOSENDCHANGING);
	m_bTopMost = !m_bTopMost;
	return 0;
}

LRESULT CMainDlg::OnBnClickedChnageWindowsText(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	CString strWindowText;
	CEdit editVert = (CEdit)GetDlgItem(IDC_EDIT_WINDOWS_TEXT);
	editVert.GetWindowText(strWindowText);
	if (strWindowText.GetLength() > 0)
	{
		::SetWindowTextW(m_hProcess, strWindowText);
	}
	return 0;
}
