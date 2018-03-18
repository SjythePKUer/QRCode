#include <opencv2/core/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/opencv.hpp>
#include <iostream>
#include <string>
#include <vector>
using namespace std;
int main() 
{
    cv::Mat raw = cv::imread("data/qrcode.jpg");
    if (raw.empty())
    {
        cerr << endl << "Could not read image file" << endl;
        return 1;
    }
    //************************************************************
    // 1. Get the three position patterns
   //************************************************************
    cv::Mat image = raw;
    cv::cvtColor(raw, image, CV_BGR2GRAY);
    cv::threshold(image, image, 100, 255, cv::THRESH_BINARY);
    cv::medianBlur(image, image, 3);
    //imshow("new", image);
    cv::Mat edges = image;
    Canny(image, edges, 100, 255, 3);
    //imshow("edges", edges);

    vector<vector<cv::Point>> contours;
    vector<cv::Vec4i> hierarchy;
    vector<int> index;
    cv::findContours(edges, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE);
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
        cv::drawContours(raw, contours, index[i], cv::Scalar(0, 0, 255), 2, 8);
    }
    //cv::imshow("contours", raw);
    cv::imwrite("data/result.jpg", raw);
    cv::waitKey(0);
    return 0;
}