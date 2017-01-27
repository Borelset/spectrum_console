#include "fillin.h"
#include <opencv2/core/core.hpp> 
#include <opencv2/highgui/highgui.hpp> 
#include "opencv2/core/utility.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include <vector>
#include <iostream>

using namespace std;
using namespace cv;

Point getref(int order);
void fillcolor(Point ref, int color, Mat source);

void fillin(vector<int> color)
{
	Mat source = imread("template.jpg");

	if (source.data == NULL)
	{
		cout << "error!\n";
	}

	Point ref;
	for (int i = 0; i < 512; i++)
	{
		ref = getref(i);
		fillcolor(ref, color[i], source);
	}
	imwrite("code.jpg", source);
}

Point getref(int order)
{
	Point ref;
	if (order < 19)
	{
		ref.x = 20 * ((order + 1) % 21) + 145;
		ref.y = 20 * ((order + 1) / 21) + 145;
	}
	else if (order < 418)
	{
		ref.x = 20 * ((order + 2) % 21) + 145;
		ref.y = 20 * ((order + 2) / 21) + 145;
	}
	else if (order < 437)
	{
		ref.x = 20 * ((order + 3) % 21) + 145;
		ref.y = 20 * ((order + 3) / 21) + 145;
	}
	else if (order < 456)
	{
		ref.x = 20 * ((order + 5) % 21) + 145;
		ref.y = 20 * ((order + 5) / 21) + 145;
	}
	else if (order < 475)
	{
		ref.x = 20 * ((order + 7) % 21) + 145;
		ref.y = 20 * ((order + 7) / 21) + 145;
	}
	else if (order < 494)
	{
		ref.x = 20 * ((order+9) / 21) + 105;
		//ref.x = 20 * ((order - 21 * 2) / 21) + 145;
		ref.y = 20 * (((order + 9) - 483) % 21) + 145;
	}
	else
	{
		ref.x = 20 * ((order + 11) / 21) + 105;
		//ref.x = 20 * ((order - 21 * 2) / 21) + 145;
		ref.y = 20 * (((order + 11) - 483) % 21) + 145;
	}
	return ref;
}

vector<int> analycolor(int color)
{
	vector<int> result;
	int b = 0;
	int g = 0;
	int r = 0;
	b = color >> 2;
	g = (color >> 1) - b * 2;
	r = color % 2;
	result.insert(result.end(), b);
	result.insert(result.end(), g);
	result.insert(result.end(), r);
	return result;
}

void fillcolor(Point ref, int color, Mat source)
{
	vector<int> color_int = analycolor(color);
	Vec3b temp_color(255 * color_int[0], 255 * color_int[1], 255 * color_int[2]);
	for (int i = ref.x; i < 20 + ref.x; i++)
	{
		for (int j = ref.y; j < 20 + ref.y; j++)
		{
			source.at<Vec3b>(j, i) = temp_color;
		}
	}
}
