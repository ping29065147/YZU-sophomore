#include <iostream>
#include <opencv2/opencv.hpp>
#define _USE_MATH_DEFINES
#include <math.h>

using namespace std;
using namespace cv;

Vec4i get_line(vector<Vec4i> lines, double& slope)
{
	double maxval = 0, temp;
    	Vec4i reline;

	for (size_t i = 0; i < lines.size(); i++) {

        	temp = (lines[i][2] - lines[i][2]) * (lines[i][2] - lines[i][0]);
        	temp += (lines[i][1] - lines[i][3]) * (lines[i][1] - lines[i][3]);
        	temp = sqrt(temp);
        	if (temp >= maxval) maxval = temp, reline = lines[i];
    	}

    	slope = (double)(reline[3] - reline[1]) / (double)(reline[2] - reline[0]);

	return reline;
}

int main(int argc, char* argv[])
{
	Mat img = imread(argv[1], -1);
	Mat CannyImg, DstImg, Imgsss;
    	vector <Vec4i> Lines;

	cvtColor(img, Imgsss, 0);
    	Canny(Imgsss, CannyImg, 500, 100);

    	cvtColor(img, DstImg, 0);
    	HoughLinesP(CannyImg, Lines, 1,CV_PI/180,250,200,10);

	double slope;
    	Vec4i reline = get_line(Lines, slope);

	line(DstImg, Point(reline[0], reline[1]), Point(reline[2], reline[3]), Scalar(0, 0, 255), 2, 8);

	slope = atan(slope) * 180 / M_PI + 90;

	Mat dst;
    	Size img_sz = img.size();
    	Point2f center( img.rows/2,img.cols/2);
    	Mat rot_mat = getRotationMatrix2D(center, slope, 1.0);

    	warpAffine(img, dst, rot_mat, img_sz);
    	imwrite(argv[2], dst);

   	 return 0;
}
