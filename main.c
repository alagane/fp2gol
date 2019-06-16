#include <stdio.h>
#include <unistd.h>
#include <string.h>

char readCase(char *grid, int x, int y)
{
	if (x < 0 || x > 15 || y < 0 || y > 9)
	{
		return 0;
	}
	return grid[y * 16 + x];
}

void writeCase(char newValue, char *grid, int x, int y)
{
	grid[y * 16 + x] = newValue;
}

void nextGrid(char *grid, char *birth, char *survival)
{
	char xVar[8] = {-1, 0, 1, -1, 1, -1, 0, 1},
		 yVar[8] = {-1, -1, -1, 0, 0, 1, 1, 1};
	char inGrid[16 * 10];
	memcpy(inGrid, grid, 16 * 10);
	for (int i = 0; i < 10; i++)
	{
		for (int j = 0; j < 16; j++)
		{
			int sum = 0;
			char value = readCase(inGrid, j, i);
			for (int k = 0; k < 8; k++)
			{
				sum += readCase(inGrid, j + xVar[k], i + yVar[k]);
			}
			if (!value && birth[sum])
			{
				writeCase(1, grid, j, i);
			}
			else if (value && !survival[sum])
			{
				writeCase(0, grid, j, i);
			}
		}
	}
}

void printGrid(char *grid)
{
	printf("+----------------+\n");
	for (int i = 0; i < 10; i++)
	{
		printf("|");
		for (int j = 0; j < 16; j++)
		{
			printf("%c", readCase(grid, j, i) ? '#' : '.');
		}
		printf("|\n");
	}
	printf("+----------------+\n");
	printf("[12A[12D");
}

int readRulestring(char *rulestring, char *birth, char *survival)
{
	for (int i = 0; i < 9; i++)
	{
		birth[i] = 0;
		survival[i] = 0;
	}

	int i = 0;
	if (rulestring[i++] != 'B')
	{
		return 1;
	}

	char c = rulestring[i++];
	while (c != '/')
	{
		if (c < '0' || c > '9')
		{
			return 1;
		}
		birth[c - '0'] = 1;
		c = rulestring[i++];
	}

	if (rulestring[i++] != 'S')
	{
		return 1;
	}

	c = rulestring[i++];
	while (c)
	{
		if (c < '0' || c > '9')
		{
			return 1;
		}
		survival[c - '0'] = 1;
		c = rulestring[i++];
	};

	return 0;
}

int main(int argc, char *argv[])
{
	char birth[9] = {0, 0, 0, 1};
	char survival[9] = {0, 0, 1, 1};
	for (int i = 1; i < argc; i++)
	{
		char *arg = argv[i++];
		if (strcmp(arg, "--rulestring") || strcmp(arg, "-r"))
		{
			if (i >= argc)
			{
				fprintf(stderr, "\"%s\" option must be followed by the rulestring argument using B/S notation.", arg);
				return 1;
			}
			arg = argv[i++];
			if (readRulestring(arg, birth, survival))
			{
				fprintf(stderr, "Malformed rulestring argument \"%s\", it must use B/S notation", arg);
				return 1;
			}
		}
	}

	char read[256] = {};
	char grid[16 * 10] = {};
	fread(read, sizeof(char), 255, stdin);
	int i = 0;
	char next;
	while (next = read[i])
	{
		char quartet;
		if (next == ' ')
		{
			i++;
			continue;
		}
		if (next >= '0' && next <= '9')
		{
			quartet = next - '0';
		}
		else if (next >= 'A' && next <= 'F')
		{
			quartet = 10 + next - 'A';
		}
		else
		{
			break;
		}
		for (int j = 0; j < 4; j++)
		{
			grid[i * 4 + j] = (quartet >> (3 - j)) & 1;
		}
		i++;
	}

	printGrid(grid);
	for (int i = 0; i < 100; i++)
	{
		nextGrid(grid, birth, survival);
		printGrid(grid);
		sleep(1);
	}
}
