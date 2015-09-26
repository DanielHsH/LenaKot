// IPcourseExDlg.h : header file
//

#pragma once


// CIPcourseExDlg dialog
class CIPcourseExDlg : public CDialog
{
// Construction
public:
	CIPcourseExDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_IPCOURSEEX_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedHistButton();

private:
		// ----------------------------------------------------------
		// ----------------------- Variables ------------------------
		// ----------------------------------------------------------

		// Pointers on GUI components
		CButton *pBt;

		CString     commandText;
		int         useImg;
		CString     useImgPath;
		
		// Control Variables.
		CComboBox   m_PicList;

		unsigned int nOpenDemos;
		// ----------------------------------------------------------
		// ----------------------- Handlers -------------------------
		// ----------------------------------------------------------

		afx_msg void OnBnClickedDeriveButton2();
		afx_msg void OnBnClickedEdgeDetButton();
		afx_msg void OnBnClickedKmeansButton();
		afx_msg void OnBnClickedInpaintButton();
		afx_msg void OnStnClickedCommandText();
		afx_msg void OnBnClickedMorphoButton();
		afx_msg void OnBnClickedDistButton();
		afx_msg void OnBnClickedDenoisingButton();
		afx_msg void OnBnClickedHsvButton2();
		afx_msg void OnBnClickedResButton();
		afx_msg void OnBnClickedAffineButton2();
		afx_msg void OnBnClickedPerspectButton();
		afx_msg void OnBnClickedPyramidButton();
		afx_msg void OnBnClickedContourButton2();
		afx_msg void OnBnClickedKalmanButton();
		afx_msg void OnBnClickedFisheyeButton();
		afx_msg void OnBnClickedFftButton();
		afx_msg void OnBnClickedHoughButton();
		afx_msg void OnBnClickedPyrsegButton();
		afx_msg void OnBnClickedFusionButton3();
		afx_msg void OnBnClickedWaterButton();
		afx_msg void OnCbnSelchangePiccombo();
		afx_msg void OnBnClickedUseimCheck();
		afx_msg void OnNMCustomdrawProgress1(NMHDR *pNMHDR, LRESULT *pResult);
		afx_msg void OnBnClickedElipseButton();
		afx_msg void OnBnClickedFloodButton();
		afx_msg void OnBnClickedSiftButton();
		afx_msg void OnBnClickedMotdetButton();
		afx_msg void OnBnClickedPencil();
};
