#include <iostream>

using namespace std;
#define ALPHA
//data
#ifdef ALPHA 
const int alpha[10][8][8] =
{
	{ //0
		{ 0, 0, 0, 0, 8, 8, 8, 8 },
		{ 0, 0, 0, 0, 8, 0, 0, 0 },
		{ 0, 0, 0, 0, 8, 0, 0, 0 },
		{ 0, 0, 0, 0, 8, 0, 0, 0 },
		{ 0, 0, 0, 0, 0, 0, 0, 0 },
		{ 0, 0, 0, 0, 0, 0, 0, 0 },
		{ 0, 0, 0, 0, 0, 0, 0, 0 },
		{ 0, 0, 0, 0, 0, 0, 0, 0 },
	},

	{ //1
		{ 0, 0, 0, 0, 8, 8, 8, 8 },
		{ 0, 0, 0, 0, 8, 0, 0, 0 },
		{ 0, 0, 0, 0, 8, 0, 0, 0 },
		{ 0, 0, 0, 0, 8, 0, 0, 0 },
		{ 0, 0, 0, 0, 8, 0, 0, 0 },
		{ 0, 0, 0, 0, 8, 0, 0, 0 },
		{ 0, 0, 0, 0, 8, 0, 0, 0 },
		{ 0, 0, 0, 0, 8, 0, 0, 0 },
	},

	{ //2
		{ 0, 0, 0, 0, 8, 8, 8, 8 },
		{ 0, 0, 0, 0, 8, 0, 0, 0 },
		{ 0, 0, 0, 0, 8, 0, 0, 0 },
		{ 0, 0, 0, 0, 8, 0, 0, 0 },
		{ 0, 0, 0, 0, 8, 0, 0, 0 },
		{ 0, 0, 0, 0, 8, 0, 0, 0 },
		{ 0, 0, 0, 0, 8, 0, 0, 0 },
		{ 0, 0, 0, 0, 8, 8, 8, 8 },
	},

	{ //3
		{ 0, 0, 0, 0, 0, 0, 0, 0 },
		{ 0, 0, 0, 0, 0, 0, 0, 0 },
		{ 0, 0, 0, 0, 0, 0, 0, 0 },
		{ 0, 0, 0, 0, 0, 0, 0, 0 },
		{ 0, 0, 0, 0, 8, 0, 0, 0 },
		{ 0, 0, 0, 0, 8, 0, 0, 0 },
		{ 0, 0, 0, 0, 8, 0, 0, 0 },
		{ 0, 0, 0, 0, 8, 8, 8, 8 },
	},

	{ //4
		{ 8, 8, 8, 8, 0, 0, 0, 0 },
		{ 0, 0, 0, 8, 0, 0, 0, 0 },
		{ 0, 0, 0, 8, 0, 0, 0, 0 },
		{ 0, 0, 0, 8, 0, 0, 0, 0 },
		{ 0, 0, 0, 0, 0, 0, 0, 0 },
		{ 0, 0, 0, 0, 0, 0, 0, 0 },
		{ 0, 0, 0, 0, 0, 0, 0, 0 },
		{ 0, 0, 0, 0, 0, 0, 0, 0 },
	},

	{ //5
		{ 8, 8, 8, 8, 0, 0, 0, 0 },
		{ 0, 0, 0, 8, 0, 0, 0, 0 },
		{ 0, 0, 0, 8, 0, 0, 0, 0 },
		{ 0, 0, 0, 8, 0, 0, 0, 0 },
		{ 0, 0, 0, 8, 0, 0, 0, 0 },
		{ 0, 0, 0, 8, 0, 0, 0, 0 },
		{ 0, 0, 0, 8, 0, 0, 0, 0 },
		{ 0, 0, 0, 8, 0, 0, 0, 0 },
	},

	{ //6
		{ 8, 8, 8, 8, 0, 0, 0, 0 },
		{ 0, 0, 0, 8, 0, 0, 0, 0 },
		{ 0, 0, 0, 8, 0, 0, 0, 0 },
		{ 0, 0, 0, 8, 0, 0, 0, 0 },
		{ 0, 0, 0, 8, 0, 0, 0, 0 },
		{ 0, 0, 0, 8, 0, 0, 0, 0 },
		{ 0, 0, 0, 8, 0, 0, 0, 0 },
		{ 8, 8, 8, 8, 0, 0, 0, 0 },
	},
	{ //7
		{ 0, 0, 0, 0, 0, 0, 0, 0 },
		{ 0, 0, 0, 0, 0, 0, 0, 0 },
		{ 0, 0, 0, 0, 0, 0, 0, 0 },
		{ 0, 0, 0, 0, 0, 0, 0, 0 },
		{ 0, 0, 0, 8, 0, 0, 0, 0 },
		{ 0, 0, 0, 8, 0, 0, 0, 0 },
		{ 0, 0, 0, 8, 0, 0, 0, 0 },
		{ 8, 8, 8, 8, 0, 0, 0, 0 },
	},
	{ //8
		{ 0, 0, 0, 0, 0, 0, 0, 0 },
		{ 0, 0, 0, 0, 0, 0, 0, 0 },
		{ 0, 0, 8, 8, 8, 8, 0, 0 },
		{ 0, 0, 8, 0, 0, 8, 0, 0 },
		{ 0, 0, 8, 0, 0, 8, 0, 0 },
		{ 0, 0, 8, 0, 0, 8, 0, 0 },
		{ 0, 0, 0, 0, 0, 0, 0, 0 },
		{ 0, 0, 0, 0, 0, 0, 0, 0 },
	},
	{ //9
		{ 0, 0, 0, 0, 0, 0, 0, 0 },
		{ 0, 0, 0, 0, 0, 0, 0, 0 },
		{ 0, 0, 8, 0, 0, 8, 0, 0 },
		{ 0, 0, 8, 0, 0, 8, 0, 0 },
		{ 0, 0, 8, 0, 0, 8, 0, 0 },
		{ 0, 0, 8, 8, 8, 8, 0, 0 },
		{ 0, 0, 0, 0, 0, 0, 0, 0 },
		{ 0, 0, 0, 0, 0, 0, 0, 0 },
	},

};
#endif

int capture[16][64];

extern int userCode(int arr[16][64]);
void assign(int digit, int bx, int by)
{
	for (int y = 0; y < 8; y++)
	for (int x = 0; x < 8; x++)
	{
		capture[by + y][bx + x] = alpha[digit][y][x];
	}
}
int main()
{
	int score = 0;
	for (int i = 0; i < 2000; i++)
	{
		int digit = rand() % 10000;
		for (int y = 0; y < 16; y++)
		for (int x = 0; x < 64; x++)
			capture[y][x] = 0;

		int number = rand() % 10000;
		int number_origin = number;
		int devide = 1000;
		int by = 0;
		int bx = 0;
		for (int c = 0; c < 4; c++)
		{
			int digit = number / devide;
			bx += rand() % 9;
			by = rand() % 9;
			assign(digit, bx, by);

			number -= digit*devide;
			devide /= 10;
			bx += 8;
		}

		if (userCode(capture) == number_origin)
			score++;
	}
	printf("Score:%d\n");
}