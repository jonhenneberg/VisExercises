#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;


bool generateHist(Mat * image,Mat & histImage, int &histSize,const float ** histRange)
{
	cv::Mat hist;

    cv::calcHist(image, 1, 0, cv::Mat() ,hist ,1 ,&histSize , histRange , true, false);

    int histHight = histImage.rows;
    int bin_w = cvRound( (double) histImage.cols / histSize);

    //Mat histImage(histHight, histWidth, uchar(255));

    normalize(hist, hist, 0, histImage.rows, NORM_MINMAX, -1, Mat() );
//
    for (int i = 0; i < histSize; i++)
    	line(histImage, Point(bin_w * (i-1),histHight - cvRound(hist.at<float>(i-1))),
    			        Point(bin_w * i ,histHight - cvRound(hist.at<float>(i))) ,
    			        Scalar(128,0,0) ,2 ,8 ,0) ;



	return true;
}

int main(int argc, char **argv) {
  system("cls");
  cout<<"Starting\n";

  // A sligthly gray image
  cv::Mat_<uchar> img = cv::imread("lena.bmp",0);

  cv::Mat_<uchar> nimg(img.cols, img.rows);

  cout<<"Img Size: " << img.rows << "," << img.cols << "\n";


  int i;
  for(i=0; i < img.cols-1;i++)
	for(int j = 0; j < img.rows-1;j++)
	{
		int newval = img.at<uchar>(i,j)+50;
		newval = newval < 0 ? 0 : newval;
		newval = newval > 255 ? 255 : newval;
		nimg.at<uchar>(i,j) = newval;
	}

  int histSize = 256;
  float range[] = {0, 256};
  const float* histRange = { range };
//  cv::Mat orgHist, modHist;

  //cv::calcHist(&img, 1, 0, cv::Mat() ,orgHist ,1 ,&histSize ,&histRange , true, false);
  //cv::calcHist(&nimg, 1, 0, cv::Mat() ,modHist ,1 ,&histSize ,&histRange , true, false);

  Mat orgHistImg(512, 512, CV_8UC3, Scalar( 255,255,255));
  generateHist(&img, orgHistImg, histSize, &histRange);

  Mat modHistImg(512, 512, CV_8UC3, Scalar( 255,255,255));
  generateHist(&nimg, modHistImg, histSize, &histRange);

  //  for (int x = 0; x < 256; x++)
//	  cout<<x<<":"<<(int)orgHist.at<uchar>(x)<<"\n";




  cout<<"(1,1):" << (int)img.at<uchar>(1,1) << "\n";

  cv::imshow("A Window", img);
  cv::imshow("B Window", nimg);
  cv::imshow("AH Window", orgHistImg);
  cv::imshow("BH Window", modHistImg);
  cv::waitKey();

  cout<<"Closing\n";

  return 0;
}
