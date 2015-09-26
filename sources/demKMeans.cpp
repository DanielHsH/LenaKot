/****************************************************************************/
/****************************    demKMeans.cpp    ***************************/
/****************************************************************************/

/****************************************************************************/
// Includes
#include "stdafx.h"
#include "demKMeans.h"
#include "sfCVextension.h"

/****************************************************************************/
demKMeans::demKMeans(const char* path){

	if (path){ 
	   safeStrBufCpy(imagePath,path);
	}
	else {
	   safeStrBufCpy(imagePath,IPE_DEFAULT_IMAGE_FILE);
	}

	// Define the command
	safeStrBufCpy(command,"Press 'Space' for random generation of points.\tK-means is applied with random K");


	// Initialize private variables
    srcImage = cvCreateImage( cvSize( 500, 500 ), 8, 3 );
    rng = cvRNG(-1);
    
	// Define colors
    color_tab[0] = CV_RGB(255,0,0);
    color_tab[1] = CV_RGB(0,255,0);
    color_tab[2] = CV_RGB(100,100,255);
    color_tab[3] = CV_RGB(255,0,255);
    color_tab[4] = CV_RGB(255,255,0);
}

/****************************************************************************/
// Destructor
demKMeans::~demKMeans(){
	cvDestroyWindow("Clusters");

    cvExtReleaseImageCheckNULL(&srcImage);
}

/****************************************************************************/
int demKMeans::runDemo(void){
	 	
    cvNamedWindow("Clusters",0);
        
	char key = 0;
    while((key != -1)&&(key != 27)&&(key != 'q')&&(key != 'Q')){
		
        int k, cluster_count = cvRandInt(&rng)%MAX_KMEANS_CLUSTERS + 1;
        int i, sample_count  = cvRandInt(&rng)%1000 + 1;
        CvMat* points        = cvCreateMat( sample_count, 1, CV_32FC2 );
        CvMat* clusters      = cvCreateMat( sample_count, 1, CV_32SC1 );
        
        // generate random sample from multigaussian distribution
        for ( k = 0; k < cluster_count; k++ ){
            CvPoint center;
            CvMat point_chunk;
            center.x = cvRandInt(&rng)%srcImage->width;
            center.y = cvRandInt(&rng)%srcImage->height;
            cvGetRows( points, &point_chunk, k*sample_count/cluster_count,
                       k == cluster_count - 1 ? sample_count :
                       (k+1)*sample_count/cluster_count, 1 );
                        
            cvRandArr( &rng, &point_chunk, CV_RAND_NORMAL,
                       cvScalar(center.x,center.y,0,0),
                       cvScalar(srcImage->width*0.1,srcImage->height*0.1,0,0));
        }

        // shuffle samples
        for( i = 0; i < sample_count/2; i++ ){
            CvPoint2D32f* pt1 = (CvPoint2D32f*)points->data.fl + cvRandInt(&rng)%sample_count;
            CvPoint2D32f* pt2 = (CvPoint2D32f*)points->data.fl + cvRandInt(&rng)%sample_count;
            CvPoint2D32f temp;
            CV_SWAP( *pt1, *pt2, temp );
        }

		// Run K-Means
        cvKMeans2( points, cluster_count, clusters,
                   cvTermCriteria( CV_TERMCRIT_EPS+CV_TERMCRIT_ITER, 10, 1.0 ));

		// Display results to window
        cvZero(srcImage);
        for( i = 0; i < sample_count; i++ ){
            int cluster_idx = clusters->data.i[i];
            ipt.x = (int)points->data.fl[i*2];
            ipt.y = (int)points->data.fl[i*2+1];
            cvCircle( srcImage, ipt, 2, color_tab[cluster_idx], CV_FILLED, CV_AA, 0 );
        }
        cvShowImage( "Clusters", srcImage );

        // Release tmp arrays
		cvReleaseMat( &points );
        cvReleaseMat( &clusters );

		// Get key for future action
        key = cvWaitKey(0);
    }
    return 0;
}

/****************************************************************************/
// Return textual explanation of the commands
const char* demKMeans::getComDesc(void){
	return command;
}

/****************************************************************************/
// EOF.



