#include "code.h"
#include "genarate.h"
#include "fillin.h"
#include <ctime>
#include <stdlib.h>
#include <vector>
#include <iostream>
#include <cstring>

void crossword(int* input, int* output, int lenth);
std::vector<int> block(int* info, int* rec);
int getbits_6(int input, int position);
int involution(int a, int n);

int genarate(unsigned char * input, int lenth)
{
	srand(time(0));
	char message[132];
	memcpy(message, input, lenth);
	message[lenth] = '\0';
	for (int i = lenth + 1; i < 132; i++)
	{
		message[i] = rand()%255;
	}

	char receive[192];
	encode(message, receive);

	int imsg[192] = { 0 };
	for (int i = 0; i < 192; i++)
	{
		if (receive[i] >= 0)
		{
			imsg[i] = receive[i];
		}
		else
		{
			imsg[i] = receive[i] + 256;
		}
	}

	int* info_ptrs[44];
	int* rec_ptrs[20];
	for (int i = 0; i < 44; i++)
	{
		info_ptrs[i] = imsg + i * 3;
	}
	for (int i = 0; i < 20; i++)
	{
		rec_ptrs[i] = &imsg[132] + i * 3;
	}

	int converted1[44];
	for (int i = 0; i < 44; i++)
	{
		converted1[i] = *info_ptrs[i] * 256 * 256 + *(info_ptrs[i] + 1) * 256 + *(info_ptrs[i] + 2);
	}
	int converted2[20];
	for (int i = 0; i < 20; i++)
	{
		converted2[i] = *rec_ptrs[i] * 256 * 256 + *(rec_ptrs[i] + 1) * 256 + *(rec_ptrs[i] + 2);
	}

	int crossed1[44] = { 0 };
	int crossed2[20] = { 0 };
	crossword(converted1, crossed1, 11);
	crossword(converted2, crossed2, 5);

	std::vector<int> blocks = block(crossed1, crossed2);
	fillin(blocks);
	std::cout << "done.\ncode.jpg has been genarated\n";
	return 0;
}

int involution(int a, int n)
{
	int result = 1;
	for (int i = 0; i < n; i++)
	{
		result *= a;
	}
	return result;
}

int getbits_6(int input, int position)
{
	int p[4] = { 0 };
	for (int i = 0; i < 4; i++)
	{
		p[i] = input / involution(2, 18 - i * 6) - (input / involution(2, 18 - (i - 1) * 6)) * involution(2, 6);
	}
	return p[position];
}

int getbits_3(int input, int position)
{
	int p[8] = { 0 };
	for (int i = 0; i < 8; i++)
	{
		p[i] = input / involution(2, 21 - i * 3) - (input / involution(2, 21 - (i - 1) * 3)) * involution(2, 3);
	}
	return p[position];
}

void crossword(int* input, int* output, int lenth)
{
	for (int i = 0; i < lenth; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			output[i * 4 + j] = getbits_6(input[i * 4], j)*involution(2, 18) + getbits_6(input[i * 4 + 1], j)*involution(2, 12) + getbits_6(input[i * 4 + 2], j)*involution(2, 6) + getbits_6(input[i * 4 + 3], j)*involution(2, 0);
		}
	}
	return;
}

std::vector<int> block(int* info, int* rec)
{
	std::vector<int> result;
	for (int i = 0; i < 44; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			result.insert(result.end(), getbits_3(info[i], j));
		}
	}
	for (int i = 0; i < 20; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			result.insert(result.end(), getbits_3(rec[i], j));
		}
	}
	return result;
}

