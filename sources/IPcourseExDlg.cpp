
/****************************************************************************/
/**************************    IPcourseExDlg.cpp    *************************/
/****************************************************************************/
#include "stdafx.h"
#include "IPcourseEx.h"
#include "IPcourseExDlg.h"
#include "demHist.h"
#include "demDerive.h"
#include "demEdgeDet.h"
#include "demKMeans.h"
#include "demInpaint.h"
#include "demMorpho.h"
#include "demDistTr.h"
#include "demDenoise.h"
#include "demHSV.h"
#include "demRes.h"
#include "demAffine.h"
#include "demPerspect.h"
#include "demPyrCre.h"
#include "demContour.h"
#include "demKalman.h"
#include "demKalman.h"
#include "demFishEye.h"
#include "demFFT.h"
#include "demHough.h"
#include "demPyrSeg.h"
#include "demFusion.h"
#include "demWatershed.h"
#include "demFitEl.h"
#include "demFfill.h"
#include "demFindObj.h"
#include "demMotionDet.h"
#include "demPencil.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


/****************************************************************************/
/************************** About Dialog ************************************/
/****************************************************************************/

class CAboutDlg : public CDialog {
public:
	    CAboutDlg();

	    enum { IDD = IDD_ABOUTBOX };

	protected:
	    virtual void DoDataExchange(CDataExchange* pDX);
	    DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD){
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX){
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CIPcourseExDlg dialog


/****************************************************************************/
/******************************** Main Dialog *******************************/
/****************************************************************************/

// Initialize variables and load icon
CIPcourseExDlg::CIPcourseExDlg(CWnd* pParent)
	: CDialog(CIPcourseExDlg::IDD, pParent),commandText(""),useImg(0), 
	  useImgPath(IPE_DEFAULT_IMAGE_FILE),nOpenDemos(0){
	
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

/****************************************************************************/
// Define data exchange between variables and GUI components
void CIPcourseExDlg::DoDataExchange(CDataExchange* pDX){
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX,IDC_PICCOMBO     ,m_PicList);
	DDX_Check  (pDX,IDC_USEIM_CHECK ,useImg);
}

/****************************************************************************/
// Map callback functions to GUI components

BEGIN_MESSAGE_MAP(CIPcourseExDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_HIST_BUTTON, &CIPcourseExDlg::OnBnClickedHistButton)
	ON_BN_CLICKED(IDC_DERIVE_BUTTON2, &CIPcourseExDlg::OnBnClickedDeriveButton2)
	ON_BN_CLICKED(IDC_EDGEDET_BUTTON, &CIPcourseExDlg::OnBnClickedEdgeDetButton)
	ON_BN_CLICKED(IDC_KMEANS_BUTTON, &CIPcourseExDlg::OnBnClickedKmeansButton)
	ON_BN_CLICKED(IDC_INPAINT_BUTTON, &CIPcourseExDlg::OnBnClickedInpaintButton)
	ON_STN_CLICKED(IDC_COMMAND_TEXT, &CIPcourseExDlg::OnStnClickedCommandText)
	ON_BN_CLICKED(IDC_MORPHO_BUTTON, &CIPcourseExDlg::OnBnClickedMorphoButton)
	ON_BN_CLICKED(IDC_DIST_BUTTON, &CIPcourseExDlg::OnBnClickedDistButton)
	ON_BN_CLICKED(IDC_DENOISING_BUTTON, &CIPcourseExDlg::OnBnClickedDenoisingButton)
	ON_BN_CLICKED(IDC_HSV_BUTTON2, &CIPcourseExDlg::OnBnClickedHsvButton2)
	ON_BN_CLICKED(IDC_RES_BUTTON, &CIPcourseExDlg::OnBnClickedResButton)
	ON_BN_CLICKED(IDC_AFFINE_BUTTON2, &CIPcourseExDlg::OnBnClickedAffineButton2)
	ON_BN_CLICKED(IDC_PERSPECT_BUTTON, &CIPcourseExDlg::OnBnClickedPerspectButton)
	ON_BN_CLICKED(IDC_PYRAMID_BUTTON, &CIPcourseExDlg::OnBnClickedPyramidButton)
	ON_BN_CLICKED(IDC_CONTOUR_BUTTON2, &CIPcourseExDlg::OnBnClickedContourButton2)
	ON_BN_CLICKED(IDC_KALMAN_BUTTON, &CIPcourseExDlg::OnBnClickedKalmanButton)
	ON_BN_CLICKED(IDC_FISHEYE_BUTTON, &CIPcourseExDlg::OnBnClickedFisheyeButton)
	ON_BN_CLICKED(IDC_FFT_BUTTON, &CIPcourseExDlg::OnBnClickedFftButton)
	ON_BN_CLICKED(IDC_HOUGH_BUTTON, &CIPcourseExDlg::OnBnClickedHoughButton)
	ON_BN_CLICKED(IDC_PYRSEG_BUTTON, &CIPcourseExDlg::OnBnClickedPyrsegButton)
	ON_BN_CLICKED(IDC_FUSION_BUTTON3, &CIPcourseExDlg::OnBnClickedFusionButton3)
	ON_BN_CLICKED(IDC_WATER_BUTTON, &CIPcourseExDlg::OnBnClickedWaterButton)
	ON_CBN_SELCHANGE(IDC_PICCOMBO, &CIPcourseExDlg::OnCbnSelchangePiccombo)
	ON_BN_CLICKED(IDC_USEIM_CHECK, &CIPcourseExDlg::OnBnClickedUseimCheck)
	ON_BN_CLICKED(IDC_ELIPSE_BUTTON, &CIPcourseExDlg::OnBnClickedElipseButton)
	ON_BN_CLICKED(IDC_FLOOD_BUTTON, &CIPcourseExDlg::OnBnClickedFloodButton)
	ON_BN_CLICKED(IDC_SIFT_BUTTON, &CIPcourseExDlg::OnBnClickedSiftButton)
	ON_BN_CLICKED(IDC_MOTDET_BUTTON, &CIPcourseExDlg::OnBnClickedMotdetButton)
	ON_BN_CLICKED(IDC_PENCIL_BUTTON, &CIPcourseExDlg::OnBnClickedPencil)
END_MESSAGE_MAP()



/****************************************************************************/
// Dialog Initialization
BOOL CIPcourseExDlg::OnInitDialog(){
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	// Make changes in system menu
	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL){
	   CString strAboutMenu;
	   strAboutMenu.LoadString(IDS_ABOUTBOX);
	   if (!strAboutMenu.IsEmpty()){
		  pSysMenu->AppendMenu(MF_SEPARATOR);
		  pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
	   }

	   // Disable Windows maximize button
	   ModifyStyle(WS_MAXIMIZEBOX, 0);
	   // Disable Windows Close Button X
	   // VERIFY(pSysMenu->RemoveMenu(SC_CLOSE, MF_BYCOMMAND));
	   
	}

	// Set the icon for this dialog.  
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	m_PicList.AddString(IPE_DEFAULT_IMAGE_FILE);
	m_PicList.AddString("Fruits.jpg");
	m_PicList.AddString("Trees.jpg");
	m_PicList.AddString("Baboon.jpg");
	m_PicList.AddString("FishEye.jpg");
	m_PicList.AddString("Sun.jpg");
	m_PicList.AddString("Planet.jpg");
	m_PicList.AddString("Daniel.jpg");
	m_PicList.AddString(IPE_DEFAULT_ONLINE_CAMERA);	
	m_PicList.SetCurSel(0);


	// Update the GUI using the initialized values
	UpdateData(false);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

/****************************************************************************/
// System command (from menu) handler

void CIPcourseExDlg::OnSysCommand(UINT nID, LPARAM lParam){
	if ((nID & 0xFFF0) == IDM_ABOUTBOX){
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else if ((nID & 0xFFF0) == 61536){           // If Exit
		if (nOpenDemos){
		   // AfxMessageBox("Some demos are still open. Please close them before exiting", MB_ICONERROR|MB_OK);
		   int res = AfxMessageBox("Some demos are still working. Are you sure, you want to quit?", MB_ICONERROR|MB_YESNO);
		   if (res == IDYES){
			  cvDestroyAllWindows();
		      CDialog::OnSysCommand(nID, lParam);
		   }
		}
		else{
		   CDialog::OnSysCommand(nID, lParam);
		}
	}
	else{
		CDialog::OnSysCommand(nID, lParam);
	}
}

/****************************************************************************/
// Dialog paint
void CIPcourseExDlg::OnPaint(){
	if (IsIconic()){
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else{
		CDialog::OnPaint();
	}
}

/****************************************************************************/
// Dragging minimized window
// The system calls this function to obtain the cursor to display while the user drags
// the minimized window.
HCURSOR CIPcourseExDlg::OnQueryDragIcon(){
	return static_cast<HCURSOR>(m_hIcon);
}

/****************************************************************************/
/*************************** CallBack Handlers ******************************/
/****************************************************************************/
// GUI button macro
#define macroButtonDemo(BUTTON_CODE,IMAGE,CLASS)  \
			pBt = reinterpret_cast<CButton *>(GetDlgItem(BUTTON_CODE)); \
			pBt->EnableWindow(false); \
			CLASS* demo; \
			if ((useImg)&&(strcmp(useImgPath,IPE_DEFAULT_IMAGE_FILE))) demo = new CLASS((LPCTSTR)useImgPath); \
			else                                                       demo = new CLASS(IMAGE); \
			commandText = demo->getComDesc(); \
			pBt = reinterpret_cast<CButton *>(GetDlgItem(IDC_COMMAND_TEXT));\
			pBt->SetWindowText(commandText);\
			nOpenDemos++;   \
			int res = demo->runDemo(); \
			delete demo; \
			nOpenDemos--; \
			if (res < 0){ \
               AfxMessageBox("Could not load desired image", MB_ICONERROR|MB_OK); \
			} \
			pBt = reinterpret_cast<CButton *>(GetDlgItem(IDC_COMMAND_TEXT));\
			pBt->SetWindowText("");\
			pBt = reinterpret_cast<CButton *>(GetDlgItem(BUTTON_CODE)); \
			pBt->EnableWindow(true) 	

/****************************************************************************/
// Demo buttons

void CIPcourseExDlg::OnBnClickedHistButton(){
	macroButtonDemo(IDC_HIST_BUTTON,"baboon.jpg",demHist);
}

void CIPcourseExDlg::OnBnClickedDeriveButton2(){
	macroButtonDemo(IDC_DERIVE_BUTTON2,"trees.jpg",demDerive);
}

void CIPcourseExDlg::OnBnClickedEdgeDetButton(){
	macroButtonDemo(IDC_EDGEDET_BUTTON,"trees.jpg",demEdgeDet);
}

void CIPcourseExDlg::OnBnClickedKmeansButton(){
	macroButtonDemo(IDC_KMEANS_BUTTON,NULL,demKMeans);
}

void CIPcourseExDlg::OnBnClickedInpaintButton(){
	macroButtonDemo(IDC_INPAINT_BUTTON,"fruits.jpg",demInpaint);	
}

void CIPcourseExDlg::OnBnClickedMorphoButton(){
	macroButtonDemo(IDC_MORPHO_BUTTON,"baboon.jpg",demMorpho);	
}

void CIPcourseExDlg::OnBnClickedDistButton(){
	macroButtonDemo(IDC_DIST_BUTTON,"circles.jpg",demDistTr);	
}
void CIPcourseExDlg::OnBnClickedDenoisingButton(){
	macroButtonDemo(IDC_DENOISING_BUTTON,"trees.jpg",demDenoise);	
}

void CIPcourseExDlg::OnBnClickedHsvButton2(){
	macroButtonDemo(IDC_HSV_BUTTON2,"trees.jpg",demHSV);		
}

void CIPcourseExDlg::OnBnClickedResButton(){
	macroButtonDemo(IDC_RES_BUTTON,"trees.jpg",demRes);			
}

void CIPcourseExDlg::OnBnClickedAffineButton2(){
	macroButtonDemo(IDC_AFFINE_BUTTON2,"trees.jpg",demAffine);			
}

void CIPcourseExDlg::OnBnClickedPerspectButton(){
	macroButtonDemo(IDC_PERSPECT_BUTTON,"trees.jpg",demPerspect);			
}

void CIPcourseExDlg::OnBnClickedPyramidButton(){
	macroButtonDemo(IDC_PYRAMID_BUTTON,"trees.jpg",demPyrCre);			
}

void CIPcourseExDlg::OnBnClickedContourButton2(){
	macroButtonDemo(IDC_CONTOUR_BUTTON2,NULL,demContour);			
}

void CIPcourseExDlg::OnBnClickedKalmanButton(){
	macroButtonDemo(IDC_KALMAN_BUTTON,NULL,demKalman);			
}

void CIPcourseExDlg::OnBnClickedFisheyeButton(){
	macroButtonDemo(IDC_FISHEYE_BUTTON,"FishEye.jpg",demFishEye);			
}

void CIPcourseExDlg::OnBnClickedFftButton(){
	macroButtonDemo(IDC_FFT_BUTTON,"trees.jpg",demFFT);			
}

void CIPcourseExDlg::OnBnClickedHoughButton(){
	macroButtonDemo(IDC_HOUGH_BUTTON,"circles.jpg",demHough);			
}

void CIPcourseExDlg::OnBnClickedPyrsegButton(){
	macroButtonDemo(IDC_PYRSEG_BUTTON,"baboon.jpg",demPyrSeg);			
}

void CIPcourseExDlg::OnBnClickedFusionButton3(){
	macroButtonDemo(IDC_FUSION_BUTTON3,NULL,demFusion);			
}

void CIPcourseExDlg::OnBnClickedWaterButton(){
	macroButtonDemo(IDC_WATER_BUTTON,"fruits.jpg",demWatershed);			
}

void CIPcourseExDlg::OnBnClickedElipseButton(){
	macroButtonDemo(IDC_ELIPSE_BUTTON,"planet.jpg",demFitEl);			
}

void CIPcourseExDlg::OnBnClickedFloodButton(){
	macroButtonDemo(IDC_FLOOD_BUTTON,"FishEye.jpg",demFfill);			
}

void CIPcourseExDlg::OnBnClickedSiftButton(){
	macroButtonDemo(IDC_SIFT_BUTTON,NULL,demFindObj);			
}

void CIPcourseExDlg::OnBnClickedPencil(){
	macroButtonDemo(IDC_PENCIL_BUTTON,"Daniel.jpg",demPencil);			
}

void CIPcourseExDlg::OnBnClickedMotdetButton(){
	macroButtonDemo(IDC_MOTDET_BUTTON,NULL,demMotionDet);
}

/****************************************************************************/
// GUI auxilary components

void CIPcourseExDlg::OnStnClickedCommandText(){
	pBt = reinterpret_cast<CButton *>(GetDlgItem(IDC_COMMAND_TEXT));
	pBt->SetWindowText("Each demo displays here a manual of how to operate it");
}


void CIPcourseExDlg::OnBnClickedUseimCheck(){
	UpdateData(true);

	if (!useImg){
	   // Return to default image
  	   m_PicList.SetCurSel(0);
	   UpdateData(false);
	   OnCbnSelchangePiccombo();
	}	
	
	// Set Enable/Diasble property of the image source combo box
	pBt = reinterpret_cast<CButton *>(GetDlgItem(IDC_PICCOMBO));
	pBt->EnableWindow(useImg!=0);
}

void CIPcourseExDlg::OnCbnSelchangePiccombo(){
     // Get the path of the image
  	 m_PicList.GetLBText(m_PicList.GetCurSel(),useImgPath);	   
}

/****************************************************************************/
// EOF.

