#include "recognize.h"
#include "locate.h"
#include "code.h"
#include <iostream>
#include <opencv2/core/core.hpp> 
#include <opencv2/highgui/highgui.hpp> 
#include "opencv2/core/utility.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include <stdio.h>

using namespace std;
using namespace cv;

Point getref_r(int order);
//int analycolor_r(Vec3b input);
int analycolor_r(Vec3b input, Scalar record_color[]);
void inttochar(vector<int> input, char* buf);

int recognize(char* path)
{

	if (locate(path) != 0)
	{
		cout << "Some troubles happened in locate!\nStop to recognize.\n";
		return -1;
	}

	Mat target = imread("locate.jpg");

	Scalar stand_color[8] = { Scalar(0,0,0), Scalar(0,0,255), Scalar(0,255,0), Scalar(0,255,255), Scalar(255,0,0), Scalar(255,0,255), Scalar(255,255,0), Scalar(255,255,255) };
	Scalar record_color[8] = { Scalar(255,255,255), Scalar(255,255,0), Scalar(255,0,255), Scalar(255,0,0), Scalar(0,255,255), Scalar(0,255,0), Scalar(0,0,255), Scalar(0,0,0) };
	int mindis[8] = { 195075, 195075, 195075, 195075, 195075, 195075, 195075, 195075 };
	for (int i = 0; i < 512; i++)
	{
		Scalar temp = target.at<Vec3b>(getref_r(i).y, getref_r(i).x);
		for (int j = 0; j < 8; j++)
		{
			int dis = (temp - stand_color[j]).dot(temp - stand_color[j]);
			if (dis < mindis[j])
			{
				mindis[j] = dis;
				record_color[j] = temp;
			}
			
		}
	}

	vector<int> receive_colors;
	for (int i = 0; i < 512; i++)
	{
		receive_colors.insert(receive_colors.end(), analycolor_r(target.at<Vec3b>(getref_r(i).y, getref_r(i).x), record_color));
	}
	char string[192];
	inttochar(receive_colors, string);

	decode(string);
	system("del locate.jpg");
	return 0;
}

Point getref_r(int order)
{
	Point ref;
	if (order < 19)
	{
		ref.x = 20 * ((order + 1) % 21) + 80;
		ref.y = 20 * ((order + 1) / 21) + 80;
	}
	else if (order < 418)
	{
		ref.x = 20 * ((order + 2) % 21) + 80;
		ref.y = 20 * ((order + 2) / 21) + 80;
	}
	else if (order < 437)
	{
		ref.x = 20 * ((order + 3) % 21) + 80;
		ref.y = 20 * ((order + 3) / 21) + 80;
	}
	else if (order < 456)
	{
		ref.x = 20 * ((order + 5) % 21) + 80;
		ref.y = 20 * ((order + 5) / 21) + 80;
	}
	else if (order < 475)
	{
		ref.x = 20 * ((order + 7) % 21) + 80;
		ref.y = 20 * ((order + 7) / 21) + 80;
	}
	else if (order < 494)
	{
		ref.x = 20 * ((order + 9) / 21) + 40;
		//ref.x = 20 * ((order - 21 * 2) / 21) + 80;
		ref.y = 20 * (((order + 9) - 483) % 21) + 80;
	}
	else
	{
		ref.x = 20 * ((order + 11) / 21) + 40;
		//ref.x = 20 * ((order - 21 * 2) / 21) + 80;
		ref.y = 20 * (((order + 11) - 483) % 21) + 80;
	}
	return ref;
}

int analycolor_r(Vec3b input, Scalar record_color[])
{
	Scalar temp = input;
	int result = 0;
	int dis = 195075;
	for (int i = 0; i < 8; i++)
	{
		if ((temp - record_color[i]).dot((temp - record_color[i])) < dis)
		{
			dis = (temp - record_color[i]).dot((temp - record_color[i]));
			result = i;
		}
	}
	/*
	int b = input[0] > 127 ? 1 : 0;
	int g = input[1] > 127 ? 1 : 0;
	int r = input[2] > 127 ? 1 : 0;
	int result = b * 4 + g * 2 + r;
	printf("%d %d %d\n", r, g, b);
	*/
	return result;
}

vector<char> getchar_r(int input)
{
	vector<char> result(3);
	result[0] = input >> 16;
	result[1] = (input >> 8) - (input >> 16) * 256;
	result[2] = input - (input >> 8) * 256;
	return result;
}

int involution_r(int a, int n)
{
	int result = 1;
	for (int i = 0; i < n; i++)
	{
		result *= a;
	}
	return result;
}

void inttochar(vector<int> input, char* buf)
{
	int temp = 0;
	int convert[64] = { 0 };
	//decross
	for (int i = 0; i < 16; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			convert[i * 4] += input[i * 4 * 8 + j * 8] * involution_r(2, 21 - j * 6) + input[i * 4 * 8 + j * 8 + 1] * involution_r(2, 18 - j * 6);
		}
		for (int j = 0; j < 4; j++)
		{
			convert[i * 4 + 1] += input[i * 4 * 8 + j * 8 + 2] * involution_r(2, 21 - j * 6) + input[i * 4 * 8 + j * 8 + 3] * involution_r(2, 18 - j * 6);
		}
		for (int j = 0; j < 4; j++)
		{
			convert[i * 4 + 2] += input[i * 4 * 8 + j * 8 + 4] * involution_r(2, 21 - j * 6) + input[i * 4 * 8 + j * 8 + 5] * involution_r(2, 18 - j * 6);
		}
		for (int j = 0; j < 4; j++)
		{
			convert[i * 4 + 3] += input[i * 4 * 8 + j * 8 + 6] * involution_r(2, 21 - j * 6) + input[i * 4 * 8 + j * 8 + 7] * involution_r(2, 18 - j * 6);
		}
	}
	//int to char
	vector<char> charbuff;
	int ptr = 0;
	for (int i = 0; i < 64; i++)
	{
		charbuff = getchar_r(convert[i]);
		for (int j = 0; j < 3; j++)
		{
			buf[ptr] = charbuff[j];
			ptr++;
		}
	}
}

