#include <iostream>
#include <opencv2/opencv.hpp>
#include <math.h>
#include <iomanip>

using namespace std;
using namespace cv;

int main(/*int argc, char* argv[]*/)
{
	//Mat img = imread(argv[1], 1);
	Mat img = imread("ex1.jpg", 1);
	Mat img2, img3, cannyimg;
	vector< vector<Point> > contours;
	vector<Point> poly;

	cvtColor(img, img2, COLOR_BGR2GRAY);
	GaussianBlur(img2, img3, Size(5, 5), 0);
	Canny(img3, cannyimg, 35, 125);

	findContours(cannyimg, contours, RETR_LIST, CHAIN_APPROX_SIMPLE);

	double max = 0;
	vector<Point> maxpoint;
	for (auto& i : contours)
	{
		double tmp = contourArea(i);
		if (tmp > max) max = tmp, maxpoint = i;
	}

	approxPolyDP(maxpoint, poly, 3, true);
	auto minAR = minAreaRect(poly);
	
	Point2f box[4];
	minAR.points(box);
	double len = sqrt(pow(box[0].x - box[3].x, 2.0) + pow(box[0].y - box[3].y, 2.0));
	
	double dis = 210 * 3752.24 / len;
	cout << dis;
	
	return 0;
}
/*
3752.24
*/
