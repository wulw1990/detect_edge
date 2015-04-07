#ifndef _SUB_DETECTOR_FACET_MODEL_MODEL_H_
#define _SUB_DETECTOR_FACET_MODEL_MODEL_H_

#include <vector>
std::vector<std::vector<float> > g_GetMasks()
{
	std::vector<std::vector<float> > masks(10);
	const int n = 25;

	{
		int i = 0;
		float numerator = 1;
		float denominator = 175;
		float A[] =
		{
			-13, 2, 7, 2, -13,
			2, 17, 22, 17, 2,
			7, 22, 27, 22, 7,
			2, 17, 22, 17, 2,
			-13, 2, 7, 2, -13
		};
		masks[i].resize(n);
		for (int j = 0; j < n; ++j)
			masks[i][j] = A[j];
	}
	{
		int i = 1;
		float numerator = 1;
		float denominator = 420;
		float A[] =
		{
			31, -44, 0, 44, -31,
			-5, -62, 0, 62, 5,
			-17, -68, 0, 68, 17,
			-5, -62, 0, 62, 5,
			31, -44, 0, 44, -31
		};
		masks[i].resize(n);
		for (int j = 0; j < n; ++j)
			masks[i][j] = A[j];
	}
	{
		int i = 2;
		float numerator = 1;
		float denominator = 420;
		float A[] =
		{
			-31,	5,	17,	5, -31,
			44,	62,	68,	62,	44,
			0,	0,	0,	0, 0,
			-44, -62, -68, -62, -44,
			31, -5, -17, -5,	31
		};
		masks[i].resize(n);
		for (int j = 0; j < n; ++j)
			masks[i][j] = A[j];
	}
	{
		int i = 3;
		float numerator = 1;
		float denominator = 70;
		float A[] =
		{
			2, -1, -2, -1,	2,
			2, -1, -2, -1,	2,
			2, -1, -2, -1,	2,
			2, -1, -2, -1,	2,
			2, -1, -2, -1,	2
		};
		masks[i].resize(n);
		for (int j = 0; j < n; ++j)
			masks[i][j] = A[j];
	}
	{
		int i = 4;
		float numerator = 1;
		float denominator = 100;
		float A[] =
		{
			-4, -2,	0,	2,	4,
			-2, -1,	0,	1,	2,
			0,	0,  0,	0,	0,
			2,	1,	0, -1, -2,
			4,	2,	0, -2, -4
		};
		masks[i].resize(n);
		for (int j = 0; j < n; ++j)
			masks[i][j] = A[j];
	}
	{
		int i = 5;
		float numerator = 1;
		float denominator = 70;
		float A[] =
		{
			2,	2,	2,	2,	2,
			-1, -1, -1, -1, -1,
			-2, -2, -2, -2, -2,
			-1, -1, -1, -1, -1,
			2,	2,	2,	2,	2
		};
		masks[i].resize(n);
		for (int j = 0; j < n; ++j)
			masks[i][j] = A[j];
	}
	{
		int i = 6;
		float numerator = 1;
		float denominator = 60;
		float A[] =
		{
			-1,	2,	0, -2,	1,
			-1,	2,	0, -2,	1,
			-1,	2,	0, -2,	1,
			-1,	2,	0, -2,	1,
			-1,	2,	0, -2,	1
		};
		masks[i].resize(n);
		for (int j = 0; j < n; ++j)
			masks[i][j] = A[j];
	}
	{
		int i = 7;
		float numerator = 1;
		float denominator = 140;
		float A[] =
		{
			4, -2, -4, -2,	4,
			2, -1, -2, -1,	2,
			0,	0,	0,	0,	0,
			-2,	1,	2,	1, -2,
			-4,	2,  4,	2, -4
		};
		masks[i].resize(n);
		for (int j = 0; j < n; ++j)
			masks[i][j] = A[j];
	}
	{
		int i = 8;
		float numerator = 1;
		float denominator = 140;
		float A[] =
		{
			-4, -2,	0,	2,	4,
			2,	1,	0, -1, -2,
			4,	2,	0, -2, -4,
			2,	1,	0, -1, -2,
			-4, -2,	0,	2,	4
		};
		masks[i].resize(n);
		for (int j = 0; j < n; ++j)
			masks[i][j] = A[j];
	}
	{
		int i = 9;
		float numerator = 1;
		float denominator = 60;
		float A[] =
		{
			1,	1,	1,	1,	1,
			-2, -2, -2, -2, -2,
			0,	0,	0,	0,	0,
			2,	2,	2,	2,	2,
			-1, -1, -1, -1, -1
		};
		masks[i].resize(n);
		for (int j = 0; j < n; ++j)
			masks[i][j] = A[j];
	}
	return masks;
}


/*
1	1	175
- 13	2	7	2 - 13
2	17	22	17	2
7	22	27	22	7
2	17	22	17	2
- 13	2	7	2 - 13

2	1	420
31 - 44	0	44 - 31
- 5 - 62	0	62	5
- 17 - 68	0	68	17
- 5 - 62	0	62	5
31 - 44	0	44 - 31

3	1	420
- 31	5	17	5 - 31
44	62	68	62	44
0	0	0	0	0
- 44 - 62 - 68 - 62 - 44
31 - 5 - 17 - 5	31

4	1	70
2 - 1 - 2 - 1	2
2 - 1 - 2 - 1	2
2 - 1 - 2 - 1	2
2 - 1 - 2 - 1	2
2 - 1 - 2 - 1	2

5	1	100
- 4 - 2	0	2	4
- 2 - 1	0	1	2
0	0	0	0	0
2	1	0 - 1 - 2
4	2	0 - 2 - 4

6	1	70
2	2	2	2	2
- 1 - 1 - 1 - 1 - 1
- 2 - 2 - 2 - 2 - 2
- 1 - 1 - 1 - 1 - 1
2	2	2	2	2


7	1	60
- 1	2	0 - 2	1
- 1	2	0 - 2	1
- 1	2	0 - 2	1
- 1	2	0 - 2	1
- 1	2	0 - 2	1

8	1	140
4 - 2 - 4 - 2	4
2 - 1 - 2 - 1	2
0	0	0	0	0
- 2	1	2	1 - 2
- 4	2	4	2 - 4

9	1	140
- 4 - 2	0	2	4
2	1	0 - 1 - 2
4	2	0 - 2 - 4
2	1	0 - 1 - 2
- 4 - 2	0	2	4

10	1	60
1	1	1	1	1
- 2 - 2 - 2 - 2 - 2
0	0	0	0	0
2	2	2	2	2
- 1 - 1 - 1 - 1 - 1
*/
#endif