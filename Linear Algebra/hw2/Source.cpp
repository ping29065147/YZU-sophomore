#include <iostream>
#include <opencv2/opencv.hpp>
#include <math.h>

using namespace std;
using namespace cv;

Vec4i get_line(vector<Vec4i> lines, double& slope)
{
	double maxval = 0, temp;
	Vec4i reline;

	for (auto& i : lines)
	{
		temp = sqrt(pow(i[2] - i[0], 2.0) + pow(i[3] - i[1], 2.0));
		if (temp >= maxval) maxval = temp, reline = i;
	}

	slope = (double)(reline[3] - reline[1]) / (double)(reline[2] - reline[0]);

	return reline;
}

int main(int argc, char* argv[])
{
	Mat img = imread(argv[1], 1);

	Mat CannyImg, Imgc, dst;
	vector<Vec4i> Lines;

	cvtColor(img, Imgc, COLOR_BGR2GRAY);
	Canny(Imgc, CannyImg, 500, 100);

	HoughLinesP(CannyImg, Lines, 1, CV_PI / 180, 250, 200, 10);

	double slope;
	Vec4i reline = get_line(Lines, slope);
	//line(Imgsss, Point(reline[0], reline[1]), Point(reline[2], reline[3]), Scalar(0, 0, 255), 2, 8);

	slope = atan(slope) * 180 / CV_PI;
	slope += (slope > 0 ? -90 : 90);

	Point2f center(img.cols / 2, img.rows / 2);
	Mat rot_mat = getRotationMatrix2D(center, slope, 1.0);
	warpAffine(img, dst, rot_mat, img.size());

	//imshow("a", dst);
	//waitKey(0);
	imwrite(argv[2], dst);

	return 0;
}