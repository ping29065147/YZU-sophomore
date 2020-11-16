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

	cout << reline[3] - reline[1] << " " << reline[2] - reline[0] << "\n";
	slope = (double)(reline[3] - reline[1]) / (double)(reline[2] - reline[0]);

	return reline;
}

int main(int argc, char* argv[])
{
	Mat img = imread(argv[1]/*"C:\\Users\\rayeh\\Desktop\\程式設計\\線代\\5.jpg"*/, -1);
	if (img.empty()) return -1;

	Mat CannyImg, DstImg, Imgsss;
	vector <Vec4i> Lines;

	cvtColor(img, Imgsss, 0);//
	Canny(Imgsss, CannyImg, 120, 300, 3);
	//imshow("r", CannyImg);
	cvtColor(img, DstImg, 0);
	HoughLinesP(CannyImg, Lines, 1, CV_PI / 360, 170, 30, 15); 
	
	double slope;
	Vec4i reline = get_line(Lines, slope);

	line(DstImg, Point(reline[0], reline[1]), Point(reline[2], reline[3]), Scalar(0, 0, 255), 2, 8);
	//imshow("t2", DstImg);

	slope = atan(slope) * 180 / M_PI + 90;
	
	Mat dst;
	Size img_sz = img.size();
	Size dst_sz(img_sz.height, img_sz.width);
	int len = max(img.cols, img.rows);
	Point2f center(len / 2., len / 2.);
	Mat rot_mat = getRotationMatrix2D(center, slope, 1.0);
	warpAffine(img, dst, rot_mat, dst_sz);
	//imshow("result", dst);
	imwrite(argv[2], dst);

	//waitKey(0);
	return 0;
}