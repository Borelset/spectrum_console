#include<iostream> 
#include <opencv2/core/core.hpp> 
#include <opencv2/highgui/highgui.hpp> 
#include "opencv2/core/utility.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include <vector>
#include <stdio.h>
using namespace cv;
using namespace std;

void show(Mat img);
Point centerpoint(vector<Point> input);
void pointinsert(vector<Point>* result, Point input, int ref_lenth);
int linetest(Point a, Point b, Mat source);
void lrlu(Point a, Point b, Point c, Point d, Point* r);
Point closeto(vector<Point>index, Point a);
vector<Point> sortpoint(vector<Point> input);
vector<Point2f> topoint2f(vector<Point> input);
vector<Point> topoint(vector<Point2f> input);
vector<vector<Point> > findsymbal(Mat source);
Mat picreduce(Mat source, int width, int height, int type);
Mat color_balance(Mat input);

Mat kernal = getStructuringElement(MORPH_RECT, Size(2, 2));
int tag = 0;

int locate(char* path) {
	Mat source = imread(path);

	if ((source.size()).width > 1000)
	{
		source = picreduce(source, (source.size()).width, (source.size()).height, 1);
	}
	if ((source.size()).height > 1000)
	{
		source = picreduce(source, (source.size()).width, (source.size()).height, 0);
	}

	if (tag)
	{
		system("rm temp.jpg");
	}
	
	int ref_lenth = (source.size()).width * (source.size()).height *0.005;

	Mat temp;
	cvtColor(source, temp, CV_BGR2GRAY);
	GaussianBlur(temp, temp, Size(3, 3), 3, 3);
	morphologyEx(temp, temp, MORPH_CLOSE, kernal);
	morphologyEx(temp, temp, MORPH_OPEN, kernal);
	Canny(temp, temp, 30, 40);

	Scalar color(0, 255, 0);
	vector<vector<Point> >result = findsymbal(temp);

	Mat cts = source.clone();
	vector<Point> centerpoints;
	for (int i = 0; i < result.size(); i++)
	{
		drawContours(cts, result, i, color, 1);
		cts = source.clone();
		pointinsert(&centerpoints, centerpoint(result[i]), ref_lenth);
	}

	int plenth = centerpoints.size();
	Scalar color2(0, 0, 255);
	for (int i = 0; i < plenth; i++)
	{
		for (int j = i; j < plenth; j++)
		{
			line(cts, centerpoints[i], centerpoints[j], color2, 3);
		}
	}

	cts = source.clone();

	if (centerpoints.size() != 4)
	{
		cout << "something wrong!";
		return -1;
	}
	vector<Point> receivepoints = sortpoint(centerpoints);
	Point2f found_corners[4];
	for (int i = 0; i < 4; i++)
	{
		found_corners[i] = receivepoints[i];
	}
	Point2f stand_corners[4];
	stand_corners[0] = Point(0, 0);
	stand_corners[1] = Point(0, 560);
	stand_corners[2] = Point(560, 0);
	stand_corners[3] = Point(560, 560);
	Mat transform = getPerspectiveTransform(found_corners, stand_corners);
	Mat converted;

	Size targetsize(560, 560);
	warpPerspective(cts, converted, transform, targetsize);
	vector<Point2f> converttemp;
	vector<Point2f> temppoints = topoint2f(centerpoints);
	perspectiveTransform(temppoints, converttemp, transform);
	centerpoints = topoint(converttemp);

	cts = color_balance(converted.clone());

	vector<vector<Point> > targetset;
	vector<Point> temp0;
	for (int i = 0; i < plenth; i++)
	{
		for (int j = i + 1; j < plenth; j++)
		{
			Point vector = centerpoints[i] - centerpoints[j];
			int temp = vector.x;
			vector.x = vector.y;
			vector.y = -temp;
			Point newpoint[4];
			newpoint[0] = centerpoints[i] + vector * 3 / 28;
			newpoint[1] = centerpoints[i] - vector * 3 / 28;
			newpoint[2] = centerpoints[j] + vector * 3 / 28;
			newpoint[3] = centerpoints[j] - vector * 3 / 28;
			if (linetest(newpoint[0], newpoint[2], cts) == 1)
			{
				temp0.insert(temp0.end(), newpoint[0]);
				temp0.insert(temp0.end(), newpoint[2]);
				targetset.insert(targetset.end(), temp0);
				temp0.clear();
			}
			if (linetest(newpoint[1], newpoint[3], cts) == 1)
			{
				temp0.insert(temp0.end(), newpoint[1]);
				temp0.insert(temp0.end(), newpoint[3]);
				targetset.insert(targetset.end(), temp0);
				temp0.clear();
			}
		}
	}

	if (targetset.size() < 2)
	{
		cout << "error: not enough tags\n";
		return -1;
	}

	Point lru[3];
	lrlu(closeto(centerpoints, targetset[0][0]), closeto(centerpoints, targetset[0][1]), closeto(centerpoints, targetset[1][0]), closeto(centerpoints, targetset[1][1]), lru);
	Point h_axis = lru[0] - lru[2];
	Point v_axis = lru[0] - lru[1];
	Point frame[4];
	frame[0] = lru[0];
	frame[1] = lru[2];
	frame[2] = lru[1];
	frame[3] = frame[1] - v_axis;
	cts = converted.clone();
	Point2f rotate[4];
	for (int i = 0; i < 4; i++)
	{
		rotate[i] = frame[i];
	}
	Mat transform2 = getPerspectiveTransform(rotate, stand_corners);
	warpPerspective(cts, cts, transform2, targetsize);
	imwrite("locate.jpg", cts);

	return 0;
}


void show(Mat img)
{
	imshow("demo", img);
	waitKey(0);
}

Point centerpoint(vector<Point> input)
{
	Moments temp;
	temp = moments(input);
	int x_center = temp.m10 / temp.m00;
	int y_center = temp.m01 / temp.m00;
	Point result(x_center, y_center);
	return result;
}

int pointcmp(Point a, Point b, int ref_lenth)
{
	Point c = a - b;
	int result = c.dot(c);
	if (result < ref_lenth)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

void pointinsert(vector<Point>* result, Point input, int ref_lenth)
{
	int lenth = result->size();
	for (int i = 0; i < lenth; i++)
	{
		if (pointcmp((*result)[i], input, ref_lenth))
		{
			return;
		}
	}
	result->insert(result->end(), input);
}

int linetest(Point a, Point b, Mat source)
{
	if (a.x < 0 || a.x>560 || a.y < 0 || a.y>560 || b.x < 0 || b.x>560 || b.y < 0 || b.y>560)
	{
		return 0;
	}
	Point a_ori = a;
	Mat cts = source.clone();
	Scalar color2(0, 255, 255);
	line(cts, a, b, color2, 3);
	Point vector = b - a;
	int result[600] = { 0 };
	int lenth = 0;
	for (int i = 0; (b-a).dot(vector)>0 && i<600; i++)
	{
		a = a + vector / 200;
		lenth++;
		uchar test = source.at<uchar>(a);
		int j = int(test);
		if (j > 200)
		{
			result[i] = 1;
		}
		else
		{
			result[i] = 0;
		}
	}

	int count = 0;
	int times = 0;
	for (int i = 0; i < 600 - 1; i++)
	{
		if (result[i] == result[i + 1])
		{
			count++;
		}
		else
		{
			if (count >= lenth/28 - 3 && count <= lenth / 28 + 3)
			{
				times++;
			}
			count = 0;
		}
	}
	if (times >= 15)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

void lrlu(Point a, Point b, Point c, Point d, Point* r)
{
	Point lu;
	Point ll;
	Point ru;
	if (a == c)
	{

		Point j1 = a - b;
		Point j2 = a - d;
		lu = a;
		if (j1.cross(j2) <0)
		{
			ru = b;
			ll = d;
		}
		else
		{
			ll = b;
			ru = d;
		}
	}
	else if (a == d)
	{

		Point j1 = a - b;
		Point j2 = a - c;
		lu = a;
		if (j1.cross(j2) < 0)
		{
			ru = b;
			ll = c;
		}
		else
		{
			ll = b;
			ru = c;
		}
	}
	else if (b == c)
	{

		Point j1 = b - a;
		Point j2 = b - d;
		lu = b;
		if (j1.cross(j2) < 0)
		{
			ru = a;
			ll = d;
		}
		else
		{
			ll = a;
			ru = d;
		}
	}
	else if(b == d)
	{

		Point j1 = b - a;
		Point j2 = b - c;
		lu = b;
		if (j1.cross(j2) < 0)
		{
			ru = a;
			ll = c;
		}
		else
		{
			ll = a;
			ru = c;
		}
	}
	r[0] = lu;
	r[1] = ll;
	r[2] = ru;
}

Point closeto(vector<Point>index, Point a)
{
	int lenth = index.size();
	Point result;
	int dis = INT_MAX;
	for (int i = 0; i < lenth; i++)
	{
		int temp = (a - index[i]).dot(a - index[i]);
		if (temp < dis)
		{
			result = index[i];
			dis = temp;
		}
	}
	return result;
}

vector<Point> sortpoint(vector<Point> input)
{
	vector<Point> tempinput = input;
	vector<Point> result;
	int minlenth = 0;
	int order = 0;
	int lenth = 0;
	for (int i = 0; i < 4; i++)
	{
		minlenth = INT_MAX;
		order = 0;
		for (int j = 0; j < tempinput.size(); j++)
		{
			lenth = tempinput[j].x * tempinput[j].x + tempinput[j].y * tempinput[j].y;
			if (lenth < minlenth)
			{
				minlenth = lenth;
				order = j;
			}
		}
		result.insert(result.end(), tempinput[order]);
		vector<Point>::iterator k = tempinput.begin() + order;
		tempinput.erase(k);
	}
	Point vector1 = result[1] - result[0];
	Point vector2 = result[2] - result[0];
	if (vector1.cross(vector2) > 0)
	{
		Point temp = result[1];
		result[1] = result[2];
		result[2] = temp;
	}
	return result;
}

vector<Point2f> topoint2f(vector<Point> input)
{
	vector<Point2f> result;
	for (int i = 0; i < 4; i++)
	{
		result.insert(result.end(), (Point2f)input[i]);
	}
	return result;
}

vector<Point> topoint(vector<Point2f> input)
{
	vector<Point> result;
	for (int i = 0; i < 4; i++)
	{
		result.insert(result.end(), (Point)input[i]);
	}
	return result;
}

vector<vector<Point> > findsymbal(Mat source)
{
	vector<vector<Point> > contours(2500);
	vector<Vec4i> hierarchy(2500);

	findContours(source, contours, hierarchy, RETR_TREE, CHAIN_APPROX_TC89_KCOS);

	int lenth = contours.size();

	Scalar color(0, 255, 0);

	vector<vector<Point> > result;
	for (int i = 0; i < lenth; i++)
	{
		int level = 0;
		int tempptr = i;
		while (hierarchy[tempptr][3] != -1)
		{
			tempptr = hierarchy[tempptr][3];
			level++;
			if (level == 4)
			{
				break;
			}
		}
		if (level >= 4)
		{
			result.insert(result.end(), contours[tempptr]);
		}
	}
	return result;
}

Mat picreduce(Mat source, int width, int height, int type)
{
	Mat result;
	Point2f origin[4];
	Point2f aim[4];

	int multi = type ? width / 1000 : height / 1000;

	Size targetsize(int(width / multi), int(height / multi));

	resize(source, result, targetsize, 0, 0, INTER_AREA);
	imwrite("temp.jpg", result);
	result = imread("temp.jpg");
	tag = 1;

	return result;
}

Mat color_balance(Mat input)
{
	Mat result;
	GaussianBlur(input, result, Size(3, 3), 11, 11);
	cvtColor(result, result, CV_BGR2GRAY);
	adaptiveThreshold(result, result, 255, CV_ADAPTIVE_THRESH_MEAN_C, CV_THRESH_BINARY, 31, 0);
	morphologyEx(result, result, MORPH_CLOSE, kernal);
	return result;
}
