#include <stdio.h>
#include <unistd.h>

char readCase(char *grid, int x, int y)
{
	if (x < 0 || x > 15 || y < 0 || y > 10)
	{
		return 0;
	}
	return grid[y * 16 + x];
}

void writeCase(char newValue, char *grid, int x, int y)
{
	grid[y * 16 + x] = newValue;
}

void nextGrid(char *grid)
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
			for (int k = 0; sum < 4 && k < 8; k++)
			{
				sum += readCase(inGrid, j + xVar[k], i + yVar[k]);
			}
			if (value && (sum < 2 || sum > 3))
			{
				writeCase(0, grid, j, i);
			}
			else if (!value && sum == 3)
			{
				writeCase(1, grid, j, i);
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

int main(void)
{
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
		for (int j = 0; j < 4; j++) {
			grid[i * 4 + j] = (quartet >> (3 - j)) & 1;
		}
		i++;
	}

	printGrid(grid);
	for (int i = 0; i < 100; i++)
	{
		nextGrid(grid);
		printGrid(grid);
		sleep(1);
	}
}
