/* Coded By    : Daniel Herman Shmulyan
   Version     : 0.5
   Description : Auxillary file with computer vision different
                 usefull methods. 
*/

/****************************************************************************/
/***********************    sfCVextension.cpp    *****************************/
/****************************************************************************/

/****************************************************************************/
// Includes
#include "stdafx.h"
#include "sfCVextension.h"
/****************************************************************************/
/*********************** Image iterator implementation **********************/
/****************************************************************************/
// Constructor. 
// Get ready to iterate from (startX,startY) up to (startX+dX,startY+dY)
// Coordinates are relative to cv ROI for openCV compatibility.
/*template <class PEL>
IplImageIterator<PEL>::IplImageIterator(IplImage* image, int startX= 0, int startY= 0, 
	                              int dX    = 0, int dY    = 0){
	initialize(image, startX, startY, dX, dY);
}

/****************************************************************************/
// Reinitialize on new image
/*template <class PEL>
void IplImageIterator<PEL>::initialize(IplImage* image, int startX= 0, int startY= 0, 
	                                                    int dX    = 0, int dY    = 0){ 

	// Read image information
	data        = reinterpret_cast<PEL*>(image->imageData); 
	step        = image->widthStep / sizeof(PEL);
	nChannels   = image->nChannels;

	// Retrieve infromation from image ROI
	CvRect rect = cvGetImageROI(image);
	nRows = rect.height;
	nCols = rect.width;

	// Move (startX,startY) to be relative to ROI upper left pixel
	startX+= rect.x;
	startY+= rect.y;

	// Rows to proccess is min(nRows,startY+dY)
	if ((startY+dY)>0 && (startY+dY)<nRows){
  		nRows= startY+dY;
	}

	// Check validity:  0 <= startY < nRows
	if (startY<0 || startY>=nRows){ 
		startY=0;
	}
	j = startY;
	
	// Skip rows if j > 0.
	data+= step*j;
 
	// Initialize values
	i = startX;
	
	// Cols to proccess is min(nCols,startX+dX)
	if ((startX+dX)>0 && (startX+dX)<nCols){ 
	   nCols= startX+dX;
	}

	// Check validity:  0 <= startX < nCols			
	if (startX<0 || startX>=nCols) {
	   startX = 0;
	}
	nCols *= nChannels;
	i = start_i = startX*nChannels;
}

/****************************************************************************/
// Get pointer to current pixel
template <class PEL>
PEL* IplImageIterator<PEL>::operator&() const { 
	return data+i; 
}
/****************************************************************************/
// Get current pixel coordinates

template <class PEL>
int IplImageIterator<PEL>::col() const{ 
	return i/nChannels; 
}

template <class PEL>
int IplImageIterator<PEL>::row() const{ 
	return j; 
}
/****************************************************************************/
// Access pixel neighbour

/* template <class PEL>
const PEL IplImageIterator<PEL>::neighbor(int dx, int dy) const { 
	return *( data + (dy*step) + i + dx*nChannels ); 
}

/****************************************************************************/
// Advance to next pixel or next color component 
/*
template <class PEL>
IplImageIterator<PEL>& IplImageIterator<PEL>::operator++(){
	i++; 

	// Check if end of row is reached
	if (i >= nCols){ 
		i = start_i; 

		// Go to next line
		j++; 
		data+= step; 
	}
	return *this;
}

/****************************************************************************/
// Advance to next pixel or next color component, but store copy before ++
/*template <class PEL>
const IplImageIterator<PEL> IplImageIterator<PEL>::operator++(int){
	IplImageIterator<PEL> prev(*this);
	++(*this);
	return prev;
}

/****************************************************************************/
// Jump few pixels (advanced step must be less then image width).
// For example, use this method when you want to proccess only even pixels 
// in each line. Note when end of line is reached iterator goes to beggining of 
// new line disregarding size of the step.
template <class PEL>
IplImageIterator<PEL>& IplImageIterator<PEL>::operator+=(int s) {
	i+= s; 

	// Check if end of row is reached
	if (i >= nCols) { 
		i=start_i; 

		// Go to next line
		j++; 
		data+= step; 
	}
	return *this;
}

/****************************************************************************/
/***************************** Image Operations *****************************/
/****************************************************************************/

/****************************************************************************/
// Morphological noise removal of objects and holes of size not greater then
// minObjSize. Applies opening and then closing

void cvExtMorphologicalNoiseRemove( IplImage* img, int minObjSize, 
								    int cvShape){

	IplConvKernel* element;
	minObjSize = minObjSize/2;
    element = cvCreateStructuringElementEx( minObjSize*2+1, minObjSize*2+1, 
		                                    minObjSize,     minObjSize, 
											cvShape, NULL );

    cvErode (img,img,element,1);
    cvDilate(img,img,element,2);
    cvErode (img,img,element,1);
	cvReleaseStructuringElement(&element);
}

/****************************************************************************/
// Morphological skeleton transform. Just a sketch

void cvExtSkeletonTransform(IplImage* srcImg, IplImage* dstImg){

	//cvCanny(fgImage8U,mask8U,10,50,3);
	//cvConvertScale(mask8U,mask8U,-1,255);
	//cvAnd(fgImage8U,mask8U,fgImage8U);
	////cvConvert(mask8U,fgImage8U);

	//cvDistTransform( mask8U, tmpImg32F, CV_DIST_L1, CV_DIST_MASK_3, NULL, NULL );
	//cvSobel(tmpImg32F,tmpImg32F,2,2,3);
	////cvLaplace(tmpImg32F,tmpImg32F,3);
	//double max;
	//cvMinMaxLoc(tmpImg32F,NULL,&max);
	//cvConvertScale( tmpImg32F, tmpImg32F, 1./max, 0);
}

/****************************************************************************/
// Invert black/white colors of binary image {0,255}

void cvExtInvertBinaryImage(IplImage* img){	
	cvConvertScale( img,img, -255, 255 );
}

/****************************************************************************/
// Find minimal and maximal values in the image. Used for debugging

/*void cvExtImgMinMax(IplImage* img, cvExt8U*min, cvExt8U* max){
	
	cvExt8U_ImageIterator it(img);
	cvExt8U curMin = 0, curMax = 0;

	if (!it){
	   curMin = curMax = *it;
	}

	while (!it) {
	   curMin = curMin<*it?curMin:*it;
	   curMax = curMax>*it?curMax:*it;
	   ++it;
	}

	if (min)*min = curMin;
	if (max)*max = curMax;
}*/

/*void cvExtImgMinMax(IplImage* img, cvExt32U*min, cvExt32U* max){
	
	cvExt32UImageIterator it(img);
	cvExt32U curMin = 0, curMax = 0;

	if (!it){
	   curMin = curMax = *it;
	}

	while (!it) {
	   curMin = curMin<*it?curMin:*it;
	   curMax = curMax>*it?curMax:*it;
	   ++it;
	}

	if (min)*min = curMin;
	if (max)*max = curMax;
}

/****************************************************************************/
// Strech intensity range of the image to [0..maxVal]
void cvExtIntensStrech(IplImage* img, int maxVal){
	double min, max, newScale;
	cvMinMaxLoc(img,&min,&max);
	newScale = 1.0*maxVal/(max-min);
	cvConvertScale(img,img,newScale,-min*newScale);
}

/****************************************************************************/
// Crops intensity to range [min..max]. 
// For each pixel, if (pix<min) it becomes min. Same for max
void cvExtIntensCrop(IplImage* img, int minVal, int maxVal){
	cvMinS(img,maxVal,img);
	cvMaxS(img,minVal,img);
}

/****************************************************************************/
// Convert mask with values in {0,1} to binary image with values {0,255}.
// Used for debugging
void cvExtConvertMask2Binary(IplImage* img){
	cvConvertScale(img,img, 0, 255 );
}

/****************************************************************************/
// Convert hue value to RGB scalar
CvScalar cvExtHue2rgb(float hue){

    int rgb[3], p, sector;
    static const int sector_data[][3]=
        {{0,2,1}, {1,2,0}, {1,0,2}, {2,0,1}, {2,1,0}, {0,1,2}};

    hue *= 0.033333333333333333333333333333333f;
    sector = cvFloor(hue);
    p = cvRound(255*(hue - sector));
    p ^= (sector&1)?255:0;

    rgb[sector_data[sector][0]] = 255;
    rgb[sector_data[sector][1]] = 0;
    rgb[sector_data[sector][2]] = p;

    return cvScalar(rgb[2], rgb[1], rgb[0],0);
}

/****************************************************************************/
// If Null then does nothing. Else acts like cvReleaseImage() and sets the 
// input image to NULL
void cvExtReleaseImageCheckNULL(IplImage** image){

	if (!*image)
	   return;

	IplImage* img = *image;
    *image = NULL;
    
    cvReleaseData(img);
    cvReleaseImageHeader(&img);
}

/****************************************************************************/
/*************************** Rectangle Operations ***************************/
/****************************************************************************/

/****************************************************************************/
// Returns 0  if two input rectangles are equal
// Returns +1 if jRect is inside iRect
// Returns -1 if iRect is inside jRect
// If none of this then returns -2;
int cvExtIsRectInside(CvRect iRect, CvRect jRect){
	
	if ((iRect.x == jRect.x)&&
		(iRect.y == jRect.y)&&
		(iRect.height == jRect.height)&&
		(iRect.width  == jRect.width)){
	   return 0;
	}

	// Check if 'j' is in 'i'
	if ((iRect.x <= jRect.x)&&
		(iRect.y <= jRect.y)&&
		(iRect.y + iRect.height >= jRect.y + jRect.height)&&
		(iRect.x + iRect.width  >= jRect.x + jRect.width)){
	   return +1;
	}

	// Check if 'i' is in 'j'
	if ((iRect.x >= jRect.x)&&
		(iRect.y >= jRect.y)&&
		(iRect.y + iRect.height <= jRect.y + jRect.height)&&
		(iRect.x + iRect.width  <= jRect.x + jRect.width)){
	   return -1;
	}

	return -2;
}

/****************************************************************************/
// Returns 0  if small intersection or not at all
// Returns 1  if large intersection

int cvExtIsRectLargeIntersection(CvRect iRect, CvRect jRect){
	
	double fraction = 0.15;

	iRect.x		 += cvRound(iRect.width  *fraction  );
	iRect.y		 += cvRound(iRect.height *fraction  );
	iRect.width  -= cvRound(iRect.width  *fraction*2);
	iRect.height -= cvRound(iRect.height *fraction*2);

	jRect.x		 += cvRound(jRect.width  *fraction  );
	jRect.y		 += cvRound(jRect.height *fraction  );
	jRect.width  -= cvRound(jRect.width  *fraction*2);
	jRect.height -= cvRound(jRect.height *fraction*2);

	int xIntr = ((iRect.x <= jRect.x)&&( jRect.x <= iRect.x+iRect.width ))||
				((jRect.x <= iRect.x)&&( iRect.x <= jRect.x+jRect.width ));

	int yIntr = ((iRect.y <= jRect.y)&&( jRect.y <= iRect.y+iRect.height))||
				((jRect.y <= iRect.y)&&( iRect.y <= jRect.y+jRect.height));

    return (xIntr&&yIntr);
}

/****************************************************************************/
// Returns the angle of vector from center of iRect to center of jRect
// Returns also the magnitude of the vector in last parameter
double cvExtAngBetweenRectCenters(CvRect iRect, CvRect jRect, double* vecMagnitude){

	double dX = jRect.x - iRect.x + (jRect.width  - iRect.width )/2.0;
	double dY = jRect.y - iRect.y + (jRect.height - iRect.height)/2.0;

	if (vecMagnitude) *vecMagnitude = sqrt(dX*dX+dY*dY);
			  
	return atan2(dY,dX)*180/CV_PI;
}

/****************************************************************************/
// Add two 2d vectors. Calculates 'vec' + 'd' into 'vec' arguments.
// Angles are given in degrees
void cvExtAddVector(double* vecMag, double* vecAng, double dMag, double dAng){

	*vecAng *= CV_PI/180.0;
	dAng    *= CV_PI/180.0;

	double X = (*vecMag)*cos(*vecAng) + dMag*cos(dAng);
	double Y = (*vecMag)*sin(*vecAng) + dMag*sin(dAng);
	
	*vecMag = sqrt(X*X+Y*Y)/2;			  
	*vecAng = atan2(Y,X)*180/CV_PI;
}

/****************************************************************************/
// Returns absolute difference between two angles (in degrees)

double cvExtAngAbsDiff(double ang1, double ang2 ){
	double absDiff = abs(ang1-ang2);
	return (absDiff<180)?absDiff:360-absDiff;
}

/****************************************************************************/
/*************************** Drawing on images ******************************/
/****************************************************************************/

/****************************************************************************/
// Draw arrow on the image (img) in a given location (start) in direction (angDeg),
// with length (length). Color and thikcness are also parameters.

void cvExtDrawArrow(IplImage *img, CvPoint start, double angDeg, double length,
					               CvScalar color, int thickness){

	CvPoint endPoint, arrowPoint;

    // Convert to radians
	double angle = angDeg*CV_PI/180.0;

	// Find end point of the arrows body.
	endPoint.x = (int)(start.x + length*cos(angle));
	endPoint.y = (int)(start.y + length*sin(angle));

	// Draw the body (main line) of the arrow.
	cvLine(img, start, endPoint, color, thickness, CV_AA, 0);

	// Draw the tips of the arrow, scaled to the size of the main part
	double tipsLength = 0.4*length;
	arrowPoint.x = (int)(endPoint.x + tipsLength*cos(angle + 3*CV_PI/4));
	arrowPoint.y = (int)(endPoint.y + tipsLength*sin(angle + 3*CV_PI/4));
	cvLine(img, arrowPoint, endPoint, color, thickness, CV_AA, 0);

	arrowPoint.x = (int) (endPoint.x + tipsLength*cos(angle - 3*CV_PI/4));
	arrowPoint.y = (int) (endPoint.y + tipsLength*sin(angle - 3*CV_PI/4));
	cvLine(img, arrowPoint, endPoint, color, thickness, CV_AA, 0);

	// Draw circle around the arrow.
	// cvCircle(img, start, cvRound(length *1.2), color, thickness, CV_AA, 0 );
}

/****************************************************************************/
// Draw Object rectangle with an arrow from his center pointing in defined 
// direction. Used for drawing global motion direction in swordfish debug mode.
void cvExtDrawObj(IplImage *img, CvRect objRect, double angDeg, double length, 
				  CvScalar color){

        // draw rectangle in the image
        cvRectangle(img, cvPoint(objRect.x, objRect.y),
                    cvPoint(objRect.x+objRect.width, objRect.y+objRect.height),
                    color, 2+0*CV_FILLED, 8);
    
        // Draw a clock with arrow indicating the direction
        CvPoint center = cvPoint((objRect.x + objRect.width/2), 
			                     (objRect.y + objRect.height/2));

		cvExtDrawArrow(img, center, angDeg, length, color);
}

/****************************************************************************/
// Visualizes a histogram as a bar graph into an output image.
// Uses different Hue for each bar.
void cvExtVisualizeHueHistogram(const CvHistogram *hist, int hdims, IplImage *img){

    // Temporary color
	CvScalar color = cvScalar(0,0,0,0);
	
	cvZero(img);

	// Calculate width of a bin
    int bin_w = img->width/hdims;

	// Iterate over all bins
    for( int i = 0; i < hdims; i++ ){
        int val = cvRound( cvGetReal1D(hist->bins,i)*(img->height)/255 );

		// Attach new hue for each bin
        color = cvExtHue2rgb((i*180.f)/hdims);

		// Draw the bin
        cvRectangle( img, cvPoint(i*bin_w,img->height),
                     cvPoint((i+1)*bin_w,img->height - val),
                     color, CV_FILLED , 8, 0 );
    }
}

/****************************************************************************/
// Convert image to pencil sketch
void cvExtPhoto2Pencil(const IplImage* srcImage, IplImage* dstImage, 
					   double intensNormal, int colorNormal,
					   int looseDitails,
					   int pencilLines, double lightPencil,
					   int colorBits){

	// -----------------------------------------
	// Auxillary images
	IplImage* gryImage  = cvCreateImage( cvGetSize(srcImage), IPL_DEPTH_8U , 1 );
	IplImage* edgImage  = cvCreateImage( cvGetSize(srcImage), IPL_DEPTH_32F, 1 );
	IplImage* tmp1Image = cvCreateImage( cvGetSize(srcImage), IPL_DEPTH_32F, 1 );
	IplImage* tmp2Image = cvCreateImage( cvGetSize(srcImage), IPL_DEPTH_32F, 1 );

	// -----------------------------------------
	// Light pencil stroke matrix. (Sum of the matrix must be 1)!
	double _pencilStroke[] = {1,0,0,0,0,
							  0,2,0,0,0,
							  0,0,5,0,0,
							  0,0,0,2,0,
							  0,0,0,0,1};

	CvMat pencilStroke = cvMat( 5, 5, CV_64F, _pencilStroke );
	cvConvertScale(&pencilStroke,&pencilStroke,1/11.0,0);
	
	// Will make the strokes random
	static CvRNG rng = cvRNG(-1);

	// Start Algorithm
	cvCopy(srcImage,dstImage);

	// Loose ditails in the image
	for (int i=0; i<looseDitails;i++){
        cvSmooth(dstImage,dstImage,CV_MEDIAN,3, 0, 0 );
	}

	// -----------------------------------------
	// Turn image to HSV

	cvCvtColor( dstImage, dstImage, CV_BGR2HSV );
	cvSplit( dstImage, NULL, NULL, gryImage, NULL);

	// -----------------------------------------
    // Put black lines on edges

	// Calculate dX and dY
	cvConvertScale(gryImage,tmp1Image,1.0/255,0);
	cvCopy(tmp1Image,tmp2Image);
	cvSobel(tmp1Image,tmp1Image,1,0,3);
	cvSobel(tmp2Image,tmp2Image,0,1,3);

	// Calculate absolute edge strength: sqrt(dX^2+dY^2). 
	cvMul(tmp1Image,tmp1Image,tmp1Image);
	cvMul(tmp2Image,tmp2Image,tmp2Image);
	cvAdd(tmp1Image,tmp2Image,edgImage);	
	cvExtIntensStrech(edgImage,1);         // We dont do sqrt() on edge strength 
										   // since we will include it in gamma correction
	
	// Invert the edge (in order to turn it black), and change its gamma
	int gamma = 10*pencilLines;
	cvExt32UImageIterator edge(edgImage);
	while (!edge){
		*edge = pow((1-(*edge)),gamma);
		++edge;
	}

	// -----------------------------------------
	// Add random pencil strokes
	cvRandArr( &rng, tmp1Image, CV_RAND_UNI, cvScalarAll(-1), cvScalarAll(1));
	cvFilter2D(tmp1Image,tmp1Image,&pencilStroke);
	cvSmooth(tmp1Image,tmp1Image,CV_GAUSSIAN,3,3);

	cvAddWeighted(edgImage,1,tmp1Image,lightPencil,0,edgImage);
	cvExtIntensCrop(edgImage);

	// -----------------------------------------
	// Normalize intensity variations in the image 
	// by subtracting from the image its blurred background.
	// Formulae: (a)*Img + (1-a)*(Inverted bloored image)

	cvConvertScale(gryImage,tmp1Image,1/255.0,0);
	cvSmooth(gryImage,gryImage,CV_BLUR,5,5);
	cvConvertScale(gryImage,tmp2Image,-1/255.0,1);

	double iN = 0.5 + 0.5*intensNormal;
	cvAddWeighted(tmp1Image,iN,tmp2Image,1-iN,0,tmp1Image);

	// -----------------------------------------
	// Combine all information (edges and normalized image)

	cvMul(tmp1Image,edgImage,tmp1Image);
	cvExtIntensStrech(tmp1Image,1);
	cvConvertScale(tmp1Image,gryImage,255.0,0);

	// Incorporate back into image
    cvMerge(NULL, NULL, gryImage, NULL, dstImage );

	// -----------------------------------------
	// Reduce The hue in dark areas of original image for removing artifacts
	// Do this by: Hue = Hue * (Gamma corrected Blurred Background) * colorNormal
	// By the way, Inverted Blurred background is stored in tmp2Image

	cvSplit( dstImage, NULL, gryImage, NULL, NULL);
	cvConvertScale(gryImage,tmp1Image,1/255.0,0);

	// Correct Gamma of blurred background (use gamma > 1 since it is inverted)
	// Also do 1-result to invert the background back
	int gamma2 = colorNormal;
	cvExt32UImageIterator back(tmp2Image);
	while (!back){
		*back = 1-pow(*back,gamma2);
		++back;
	}

	double colorMultiply = (colorNormal<5)?(0.2+colorNormal/5.):(1+colorNormal/20.);
	cvMul(tmp1Image,tmp2Image,tmp1Image,colorMultiply);
	cvExtIntensCrop(tmp1Image,0,1);

	// Incorporate back into image
	cvConvertScale(tmp1Image,gryImage,255.0,0);
    cvMerge(NULL, gryImage, NULL, NULL, dstImage );

	// -----------------------------------------
	// Return to RGB plane
	cvCvtColor(dstImage, dstImage, CV_HSV2BGR);

	// -----------------------------------------
	// Reduce Bit depth. Renormalize values to [0..255].
	
	colorBits = (colorBits>8)?8:colorBits;

	if (colorBits<8){
	   int depthRatio = cvRound(256/pow(2.0,colorBits));
	   int multFactor = (depthRatio!=256)?255/(256/depthRatio -1):0;
	   cvExtImageIterator it(dstImage);
	   while (!it){
			*it = cvRound(*it/depthRatio)*multFactor;
			++it;
	   }
	}

	// -----------------------------------------
	// Free allocated images
    cvExtReleaseImageCheckNULL(&gryImage);
    cvExtReleaseImageCheckNULL(&edgImage);
    cvExtReleaseImageCheckNULL(&tmp1Image);
    cvExtReleaseImageCheckNULL(&tmp2Image);
}

/****************************************************************************/
// EOF.
