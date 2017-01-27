#include "recognize.h"
#include "genarate.h"
#include "locate.h"
#include <vector>
#include <iostream>
#include <cstring>

using namespace std;

int main(int argc, char* argv[])
{
	if (argc < 3)
	{
		cout << "usage:\n - Type \"spectrum -r pic_path\" to recognize a code\n - Type \"spectrum -g info\" to genarate a code\n";
		return -1;
	}
	if (strcmp(argv[1], "-r") == 0)
	{
		if (recognize(argv[2]) != 0)
		{
			cout << "Something wrong..\n";
			return -2;
		}
	}
	if (strcmp(argv[1], "-g") == 0)
	{

		int info_lenth = strlen(argv[2]);
		if (genarate((unsigned char*)argv[2], info_lenth) != 0)
		{
			cout << "Something wrong..\n";
			return -3;
		}
	}
}
