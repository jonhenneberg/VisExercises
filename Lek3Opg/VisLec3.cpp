#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

// Correction of quadrant positions of a spectrum
void dftshift(cv::Mat_<float>& magnitude) {
   const int cx = magnitude.cols/2;
   const int cy = magnitude.rows/2;

   cv::Mat_<float> tmp;
   cv::Mat_<float> topLeft(magnitude, cv::Rect(0, 0, cx, cy));
   cv::Mat_<float> topRight(magnitude, cv::Rect(cx, 0, cx, cy));
   cv::Mat_<float> bottomLeft(magnitude, cv::Rect(0, cy, cx, cy));
   cv::Mat_<float> bottomRight(magnitude, cv::Rect(cx, cy, cx, cy));

   topLeft.copyTo(tmp);
   bottomRight.copyTo(topLeft);
   tmp.copyTo(bottomRight);

   topRight.copyTo(tmp);
   bottomLeft.copyTo(topRight);
   tmp.copyTo(bottomLeft);
}

void logtransform(Mat_<float>& image, int c)
{
	  int i;
	  for(i=0; i < image.cols;i++)
		for(int j = 0; j < image.rows;j++)
		{
			float newval = c * log10f(1 + image.at<float>(i,j));
			image.at<float>(i,j) = newval;
		}
}

bool Exercise1(string filename, Mat_<float>& returnmag, Mat_<float>& returnphase) {
	Mat_<float> image = imread(filename,CV_LOAD_IMAGE_GRAYSCALE);

	imshow(filename, (Mat_<uchar>)image);

	Mat_<Vec2f> imagedata;
	Mat planes[] = {Mat_<float>(image), Mat::zeros(image.size(), CV_32F)};

	merge(planes,2,imagedata);
	dft(imagedata,imagedata);

	Mat_<float> mag, pha;

	split(imagedata,planes);

//	magnitude(planes[0],planes[1],mag);
//	phase(planes[0],planes[1],pha,false);
	cartToPolar(planes[0],planes[1],mag,pha,false);


	dftshift(mag);

	returnmag = mag.clone();
	returnphase = pha.clone();
	//imshow("Debug Mag", mag);


	logtransform(mag, 100);
	normalize(mag, mag, 0, 1, CV_MINMAX);
	normalize(pha, pha, 0, 1, CV_MINMAX);

	imshow(filename + " Mag", mag);

	//dft(imagedata, testdata, DFT_INVERSE & DFT_SCALE & DFT_REAL_OUTPUT);
	//imshow(filename + " Test", testdata);
	return true;
}

bool Exercise1(string filename) {
	Mat_<float> magdata , phasedata;
	return Exercise1(filename, magdata, phasedata);
}


bool Exercise2(Mat_<float>& magnitude, Mat_<float>& phase) {
	magnitude(cv::Rect(magnitude.cols/2 - 25, magnitude.rows/2 -25, 50, 50)) = 0.0f;
	//magnitude(cv::Rect(0,0,magnitude.cols, magnitude.rows)) = 1.0f;

	dftshift(magnitude);

	Mat_<float> planes[2];
	Mat_<Vec2f> imagedata;

	polarToCart(magnitude,phase,planes[0],planes[1],false);
	merge(planes,2,imagedata);
	dft(imagedata, imagedata, DFT_INVERSE + DFT_SCALE);
	//dft(imagedata, planes[0], DFT_INVERSE + DFT_SCALE + DFT_REAL_OUTPUT);
	//cout<<"213\n";
	split(imagedata,planes);
	normalize(planes[0], planes[0], 0, 1, CV_MINMAX);

	imshow("Exercise2", planes[0]);

	return true;
}

void Lec3() {
	Exercise1("lena_hair.bmp");
	Exercise1("lena_hat.bmp");
	Exercise1("lena_face.bmp");

	Mat_<float> magdata , phasedata;
	Exercise1("lena.bmp", magdata, phasedata);
	Exercise2(magdata, phasedata);
}



int main(int argc, char **argv) {
	system("cls");
	cout<<"Starting...\n";

	try
	{
	  Lec3();
	  cv::waitKey();
	} catch (Exception & e) {
		cout<<"Msg:"<<e.msg;
	}



	cout<<"Closing\n";

	return 0;
}
