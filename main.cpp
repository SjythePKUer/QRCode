#include <opencv2/core/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <string>
#include <vector>
using namespace cv;
using namespace std;
int main() 
{
	const char* str = "qrcode.jpg";
	Mat raw = imread(str);
	Mat image = raw;
	if (raw.empty())
	{
		cout << "Could not read image file" << endl;
		return -1;
	}
	//************************************************************
	//1. Get the three position patterns
	//************************************************************
	cvtColor(raw, image, CV_BGR2GRAY);
	threshold(image, image, 100, 255, THRESH_BINARY);
	medianBlur(image, image, 3);
	//imshow("new", image);
	Mat edges = image;
	Canny(image, edges, 100, 255, 3);
	//imshow("edges", edges);

	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;
	vector<int> index;
	findContours(edges, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE);
	for (int i = 0; i < contours.size(); i++)
	{
		int k = i;
		int c = 0;
		while (hierarchy[k][2] != -1)
		{
			k = hierarchy[k][2];
			c++;
		}
		if (c >= 5)
		{
			index.push_back(i);
		}
	}
	if (index.size() < 3)
	{
		cout << "No valid QRcode in the picture. Check if part of the QRcode is missing..." << endl;
	}
	if (index.size() > 3)
	{
		cout << "There is more than one QRcode in the picture. " << endl;
	}
	for (int i = 0; i < index.size(); i++)
	{
		drawContours(raw, contours, index[i], Scalar(0, 0, 255), 2, 8);
	}
	imshow("contours", raw);
	imwrite("result.jpg", raw);
	waitKey(0);
	return 0;
}