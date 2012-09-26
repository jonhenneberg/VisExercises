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

void dftToMagnitudeAndPhase(Mat_<float> & image, Mat_<float>& returnmag, Mat_<float>& returnphase) {
	Mat_<Vec2f> imagedata;
	Mat planes[] = {Mat_<float>(image), Mat::zeros(image.size(), CV_32F)};

	merge(planes,2,imagedata);
	dft(imagedata,imagedata);

	split(imagedata,planes);
	cartToPolar(planes[0],planes[1],returnmag,returnphase,false);

	dftshift(returnmag);
}

bool ButterworthHPF(Mat_<float> & filter, int P, int Q, int Diamater, int order) {
	filter=Mat::zeros(P, Q, CV_32F);
	for(int u = 0; u < P;u++)
		for(int v = 0; v < Q;v++)
		{
			float D = sqrtf(powf(u - (P/2),2.0) + powf(v - (Q/2),2.0));  //page 291
			float H = 1 / (1 + powf(Diamater/D,2*order));  //page 306
			//float H = 1 / (1 + powf(sqrtf(powf(v,2.0) + powf(u,2.0))/Diamater,2*order));  //slide
			filter.at<float>(u,v) = H;
		}


	return true;
}

bool Exercise1(string filename) {
	Mat_<float> image2;
	Mat_<float> image = imread(filename,IMREAD_UNCHANGED);
	imshow(filename + " gray", (Mat_<uchar>)image);
	copyMakeBorder(image,image2,0,image.rows,0,image.cols,BORDER_CONSTANT,0.0);
	imshow(filename + " x2", (Mat_<uchar>)image2);

	Mat_<float> mag,mag1, pha;
	Mat_<float> filter;
	dftToMagnitudeAndPhase(image2,mag,pha);
	ButterworthHPF(filter, mag.rows, mag.cols,250,2);

	imshow(filename + " filter", filter);

	cout<<filter.cols<<"\n";
	cout<<mag.cols<<"\n";
	//mag1 = mag * filter;

	mulSpectrums(mag, filter, mag, DFT_ROWS, true);
//	mag1 = mag + 1.0f;
//    cv::log(mag1, mag1);
//	normalize(mag1, mag1, 0, 1, CV_MINMAX);
//	imshow(filename + " Mag", mag1);

	Mat_<float> planes[2];
	Mat_<Vec2f> imagedata;

	dftshift(mag);
	polarToCart(mag,pha,planes[0],planes[1],false);
	merge(planes,2,imagedata);
	dft(imagedata, imagedata, DFT_INVERSE + DFT_SCALE);
	//dft(imagedata, planes[0], DFT_INVERSE + DFT_SCALE + DFT_REAL_OUTPUT);
	//cout<<"213\n";
	split(imagedata,planes);
	//normalize(planes[0], planes[0], 0, 1, CV_MINMAX);

	Mat_<float> image3(planes[0], cv::Rect(0, 0, image.cols, image.rows));
	Mat_<float> image4;
	imshow("Exercise2", image3);

	image4 = image - image3;


	imshow("Exercise3", (Mat_<uchar>)image4);

	add(image,image3,image4);
	imshow("Exercise4", (Mat_<uchar>)image4);
	return true;
}

void Lec4() {
	//Code goes here
	Exercise1("moon_gray.png");

	//Mat_<float> filter;
	//ButterworthHPF(filter, 200, 200,50,2);
	//cv::log(filter, filter);
	//normalize(filter, filter, 0, 1, CV_MINMAX);
	//imshow("filter", filter);
}



int main(int argc, char **argv) {
	system("cls");
	cout<<"Starting...\n";

	try
	{
	  Lec4();
	  cv::waitKey();
	} catch (Exception & e) {
		cout<<"Msg:"<<e.msg;
	}



	cout<<"Closing\n";

	return 0;
}
