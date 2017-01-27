#include "code.h"
#include "rs.hpp"
#include <iostream>
#include <stdio.h>

#define ECC_LENGTH 60
#define MSGLEN 132

using namespace std;

RS::ReedSolomon<MSGLEN, ECC_LENGTH> rs;

void encode(char * input, char* output)
{
	char encoded[MSGLEN + ECC_LENGTH];
	rs.Encode(input, encoded);
	memcpy(output, encoded, MSGLEN + ECC_LENGTH);
	return;
}

void decode(char* input)
{
	char repaired[MSGLEN];
	rs.Decode(input, repaired);
	cout << repaired << endl;
	return;
}


