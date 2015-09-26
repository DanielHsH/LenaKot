/* Coded By    : Daniel Herman Shmulyan
   Version     : 0.5
   Description : Auxillary file with computer vision different
                 usefull methods. 
*/
/****************************************************************************/
/***********************    sfCVextension.h    *****************************/
/****************************************************************************/
// Includes
#include "globalPreDefines.h"

#if !defined(SFCVEXTENSION_09FJ17FB3N59SGFOL6H7H7KJD3487_IINC)
#define SFCVEXTENSION_09FJ17FB3N59SGFOL6H7H7KJD3487_IINC

/******************************** Defines ***********************************/
// define image iterators. Use precompiler define for realtime considerations

# define cvExt1U_ImageIterator IplImageIterator<bool>
# define cvExt8U_ImageIterator IplImageIterator<unsigned char>
# define cvExt8U unsigned char
# define cvExt16UImageIterator IplImageIterator<unsigned short>
# define cvExt32UImageIterator IplImageIterator<float>
# define cvExt32U float

// Default iterator
# define cvExtImageIterator cvExt8U_ImageIterator

/****************************************************************************/
// Class Image iterator. Allows easy access to image pixels with iterator
// For example, for thresholding image use:
// cvExtImageIterator it(image);
// while (!it) {
//    *it = (*it < 30)?0:255;
//    ++it;
// }
// Image iterator runs relatively slow, unless setting compiler to optimize for speed.
// When doing so - image iterator becomes as fast as pointers manipulation

template <class PEL>
class IplImageIterator {

	  private:
			// -----------------------------------
			// Reinitialize on new image (used in constructor) 
			void initialize(IplImage* image, int startX= 0, int startY= 0, 
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

	  public:
			
			// -----------------------------------
			// Constructor. 
		    // Get ready to iterate from (startX,startY) up to (startX+dX,startY+dY)
		    // Coordinates are relative to cv ROI for openCV compatibility.
			IplImageIterator(IplImage* image, int startX= 0, int startY= 0, 
				                              int dX    = 0, int dY    = 0){
				initialize(image, startX, startY, dX, dY);
			}

	  public:

			// -----------------------------------
			// Access pixel

			PEL& operator*(){
				return data[i]; 
			}

			const PEL operator*() const{
				return data[i]; 
			}


			// Get pointer to current pixel
			PEL* operator&() const;

			// -----------------------------------
			// Get current pixel coordinates

			int col() const;
			int row() const;

			// -----------------------------------
			// Access pixel neighbour
			const PEL neighbor(int dx, int dy) const;

			// -----------------------------------
			// Advance to next pixel or next color component 
			IplImageIterator& operator++(){
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

			// Advance to next pixel or next color component, but store copy before ++
			const IplImageIterator operator++(int){
					IplImageIterator<PEL> prev(*this);
					++(*this);
					return prev;
			}

			// Jump few pixels (advanced step must be less then image width).
			// For example, use this method when you want to proccess only even pixels 
			// in each line. Note when end of line is reached iterator goes to beggining of 
			// new line disregarding size of the step.
			IplImageIterator& operator+=(int s);

			// -----------------------------------
			// Check if iterator has more data to proccess
			bool operator!() const{
					return j < nRows; 
			}

			// ---------------------------------------
			// Inner image variables

	  private:
			PEL* data;			// Image information. Pointer to current row.
			int i, j;			// (i,j) coordinates of current pixel

			int start_i;		// starting column
			int step;			// number of bytes in image row
			int nRows, nCols;	// Size of the area to iterate over
			int nChannels;		// Number of channels in the image 
}; 

template <class PEL>
const PEL IplImageIterator<PEL>::neighbor(int dx, int dy) const { 
	return *( data + (dy*step) + i + dx*nChannels ); 
}

/****************************************************************************/
/***************************** Image Operations *****************************/
/****************************************************************************/

/****************************************************************************/
// Morphological noise removal of objects and holes of size not greater then
// minObjSize. Applies opening and then closing

void cvExtMorphologicalNoiseRemove( IplImage* img, int minObjSize, 
						            int cvShape = CV_SHAPE_RECT);

/****************************************************************************/
// Morphological skeleton transform. Just a sketch
void cvExtSkeletonTransform(IplImage* srcImg, IplImage* dstImg);

/****************************************************************************/
// Invert black/white colors of binary image {0,255}
void cvExtInvertBinaryImage(IplImage* img);

/****************************************************************************/
// Convert mask with values in {0,1} to binary image with values {0,255}
// Used for debugging
void cvExtConvertMask2Binary(IplImage* img);

/****************************************************************************/
// Find minimal and maximal values in the image. Used for debugging 
//void cvExtImgMinMax(IplImage* img, cvExt8U*  min = NULL, cvExt8U*  max = NULL);
//void cvExtImgMinMax(IplImage* img, cvExt32U* min = NULL, cvExt32U* max = NULL);

/****************************************************************************/
// Strech intensity range of the image to [0..maxVal]
void cvExtIntensStrech(IplImage* img, int maxVal = 1);

/****************************************************************************/
// Crops intensity to range [min..max]. 
// For each pixel, if (pix<min) it becomes min. Same for max
void cvExtIntensCrop(IplImage* img, int minVal = 0, int maxVal = 1);

/****************************************************************************/
// Convert hue value to RGB scalar
CvScalar cvExtHue2rgb(float hue);

/****************************************************************************/
// If Null then does nothing. Else acts like cvReleaseImage() and sets the 
// input image to NULL
void cvExtReleaseImageCheckNULL(IplImage** image);

/****************************************************************************/
/*************************** Rectangle Operations ***************************/
/****************************************************************************/

/****************************************************************************/
// Returns 0  if two input rectangles are equal
// Returns +1 if jRect is inside iRect
// Returns -1 if iRect is inside jRect
int cvExtIsRectInside(CvRect iRect, CvRect jRect);

/****************************************************************************/
// Returns 0  if small intersection or not at all
// Returns 1  if large intersection
int cvExtIsRectLargeIntersection(CvRect iRect, CvRect jRect);
	
/****************************************************************************/
// Returns the angle of vector from center of iRect to center of jRect
// Returns also the magnitude of the vector in last parameter if it is not null
double cvExtAngBetweenRectCenters(CvRect iRect, CvRect jRect, double* vecMagnitude = NULL);

/****************************************************************************/
// Add two 2d vectors. Calculates 'vec' + 'd' into 'vec' arguments.
// Angles are given in degrees
void cvExtAddVector(double* vecMag, double* vecAng, double dMag, double dAng);

/****************************************************************************/
// Returns absolute difference between two angles (in degrees)
double cvExtAngAbsDiff(double ang1, double ang2 );

/****************************************************************************/
/*************************** Drawing on images ******************************/
/****************************************************************************/

/****************************************************************************/
// Draw arrow on the image (img) in a given location (start) in direction (angDeg),
// with length (length). Color and thikcness are also parameters.
void cvExtDrawArrow(IplImage *img, CvPoint start, double angDeg, double length,
					               CvScalar color, int thickness = 1);

/****************************************************************************/
// Draw Object rectangle with an arrow from his center pointing in defined 
// direction. Used for drawing global motion direction in swordfish debug mode.
void cvExtDrawObj(IplImage *img, CvRect objRect, double angDeg, double length,
				  CvScalar color);

/****************************************************************************/
// Visualizes a histogram as a bar graph into an output image.
// Uses different Hue for each bar.
void cvExtVisualizeHueHistogram(const CvHistogram *hist, int hdims, IplImage *img);

/****************************************************************************/
// Draw cross of size d in pixels on a specific place in the image
#define drawCrossOnImage(img, center, color, d )    \
        cvLine( img, cvPoint( center.x - d, center.y - d ),                \
                     cvPoint( center.x + d, center.y + d ), color, 1, CV_AA, 0); \
        cvLine( img, cvPoint( center.x + d, center.y - d ),                \
                     cvPoint( center.x - d, center.y + d ), color, 1, CV_AA, 0 )

/****************************************************************************/
// Convert angle to pixel at this angle around the center of the image,
// at distance of height/3
#define calcPointAroundImageCenter(img,angle)  \
        cvPoint( cvRound(img->width/2  + img->height/3*cos(angle)),  \
                 cvRound(img->height/2 - img->height/3*sin(angle))) 

/****************************************************************************/
// Convert image to pencil sketch
// Parameters: 
//        intensNormal is in [0..1]   - normalizes variations in image intensity
//        colorNormal  is 0 or grater - Gamma correction to color depth (Hue)
//        looseDitails is in [0..2]   - Defines how much detailes will be in the image
//                                      0 - Best quality. High value means less ditails
//        pencilLines  is 1 or grater - Defines the size of bold black pencil lines
//        lightPencil  is in [0..1]   - Defines the light pencil strokes
//        colorDepth   is in [1..8]   - Number of bits to represent color
void cvExtPhoto2Pencil(const IplImage* srcImage, IplImage* dstImage, 
					   double intensNormal = 0.7, int colorNormal  = 5,
					   int looseDitails = 1,
					   int pencilLines = 8, double lightPencil = 0.2,
					   int colorBits = 5);

/****************************************************************************/
/*********************************** Other **********************************/
/****************************************************************************/

/****************************************************************************/
// integer to boolean conversion
#define SF_INT_2_BOOL(a)  ((a>0)?true:false)

/****************************************************************************/
// String copy with protection against buffer overflow (by truncating suffix)
inline void safeStrCpy_(char* dst, const char* src, int dstSize){
	int i = 0;
	
	// Verify that source is not NULL
	if (src)
	   for (; (i < dstSize-1)&&(src[i]); *(dst++) = src[i++]);		
	*dst = '\0';
}
#define safeStrBufCpy(dst,src)  safeStrCpy_(dst,src,sizeof(dst)) 

/****************************************************************************/
#define verifyCharRange(v)  \
		if (v<0) v = 0; if (v>255) v = 255;

/****************************************************************************/

#endif // #define SFCVEXTENSION_09FJ17FB3N59SGFOL6H7H7KJD3487_IINC

/****************************************************************************/
// EOF.
