#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <locale.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>
#include <limits.h>

// ������� ��� ��������� ������������� �������� �������
int getMax(int arr[], int n) {
	int max = arr[0];
	for (int i = 1; i < n; i++) {
		if (arr[i] > max) {
			max = arr[i];
		}
	}
	return max;
}

// ������� ��� ���������� ���������� ��������� �� ������� exp
void countSort(int arr[], int n, int exp, long* count) {
	int* output = (int*)malloc(n * sizeof(int));
	if (output == NULL) {
		printf("������ ��������� ������!\n");
		exit(1);
	}

	int countDigits[10] = { 0 };
	int i;

	// 1. ������� ���������� ������ �����
	for (i = 0; i < n; i++) {
		int digit = (abs(arr[i]) / exp) % 10;  // ���������� abs() ��� ������������� �����
		countDigits[digit]++;
	}

	// 2. �������������� countDigits � �������
	for (i = 1; i < 10; i++)
		countDigits[i] += countDigits[i - 1];

	// 3. ���������� ��������� �������
	for (i = n - 1; i >= 0; i--) {
		int digit = (abs(arr[i]) / exp) % 10;
		int pos = --countDigits[digit];

		// �������� ������
		if (pos < 0 || pos >= n) {
			printf("������: ����� �� ������� �������! pos=%d, n=%d\n", pos, n);
			free(output);
			exit(1);
		}

		output[pos] = arr[i];
		(*count)++;
	}

	// 4. ����������� ������� � �������� ������
	for (i = 0; i < n; i++) {
		arr[i] = output[i];
	}
	free(output);
}

// �������� ������� ��� ���������� ����������� ����������
void radixSort(int arr[], int n, long* count) {
	// ������� ������������ �����, ����� ����� ���������� ��������
	int m = getMax(arr, n);

	// ��������� ���������� ��������� ��� ������� �������
	for (int exp = 1; m / exp > 0; exp *= 10) {
		countSort(arr, n, exp, count);
	}
}

int main() {
	setlocale(LC_ALL, "Russian");
	srand((unsigned int)time(NULL));
	FILE* f;
	int size;
	int* array = NULL;
	char ch;

	do {
		system("cls");
		printf("����:\n");
		printf("1 - ���������� ��������� �������� �� �����������\n");
		printf("2 - ���������� ��������� �������� �� ��������\n");
		printf("3 - ���������� ������������ �������� �� ��������\n");
		printf("4 - ���������� ������������ �������� �� �����������\n");
		printf("ESC - �����\n");
		ch = _getch();

		clock_t start, stop;
		double time;
		long count;

		switch (ch) {
		case '1': {
			system("cls");
			printf("������� ���������� ��������� �������: ");
			if (scanf("%d", &size) != 1 || size <= 0) {
				printf("�������� ������ �������!\n");
				system("pause");
				break;
			}

			array = (int*)malloc(size * sizeof(int));
			if (array == NULL) {
				printf("������ ��������� ������!\n");
				system("pause");
				break;
			}

			printf("\n������ ��������� �����: 'input.txt'\n");
			f = fopen("input.txt", "w");
			if (f == NULL) {
				printf("������ �������� �����!\n");
				free(array);
				system("pause");
				break;
			}

			for (int i = 0; i < size; i++) {
				array[i] = rand() - RAND_MAX / 2;
				fprintf(f, "%d ", array[i]);
			}
			fclose(f);

			printf("��������������� ������: 'output.txt'\n");
			f = fopen("output.txt", "w");
			if (f == NULL) {
				printf("������ �������� �����!\n");
				free(array);
				system("pause");
				break;
			}

			start = clock();
			count = 0;
			radixSort(array, size, &count);
			stop = clock();

			for (int i = 0; i < size; i++) {
				fprintf(f, "%d ", array[i]);
			}
			fclose(f);

			time = (double)(stop - start) / CLOCKS_PER_SEC;
			printf("\n����� ���������� ����������: %.3lf ���\n", time);
			printf("���������� ��������: %ld\n", count);

			free(array);
			system("pause");
			break;
		}
		case '2': {
			// ���������� ��������� �������� �� ��������
			system("cls");
			printf("������� ���������� ��������� �������: ");
			if (scanf_s("%d", &size) != 1 || size <= 0) {
				printf("�������� ������ �������!\n");
				system("pause");
				break;
			}

			array = (int*)malloc(size * sizeof(int));
			if (array == NULL) {
				printf("������ ��������� ������!\n");
				system("pause");
				break;
			}

			printf("\n������ ��������� �����: 'input.txt'\n");
			if (fopen_s(&f, "input.txt", "w") != 0) {
				printf("������ �������� �����!\n");
				free(array);
				system("pause");
				break;
			}

			for (int i = 0; i < size; i++) {
				array[i] = rand() - rand();
				fprintf(f, "%d ", array[i]);
			}
			fclose(f);

			printf("��������������� ������: 'output.txt'\n");
			if (fopen_s(&f, "output.txt", "w") != 0) {
				printf("������ �������� �����!\n");
				free(array);
				system("pause");
				break;
			}

			start = clock();
			count = 0;
			radixSort(array, size, &count);

			// ��� ���������� �� �������� ������ ���������� ������
			for (int i = 0; i < size / 2; i++) {
				int temp = array[i];
				array[i] = array[size - i - 1];
				array[size - i - 1] = temp;
				count++;
			}

			stop = clock();

			for (int i = 0; i < size; i++) {
				fprintf(f, "%d ", array[i]);
			}
			fclose(f);

			time = (double)(stop - start) / CLOCKS_PER_SEC;
			printf("\n����� ���������� ����������: %.3lf ���\n", time);
			printf("���������� ��������: %ld\n", count);

			free(array);
			system("pause");
			break;
		}
		case '3': {
			// ���������� ������������ �������� �� ��������
			system("cls");
			printf("������� ���������� ��������� �������: ");
			if (scanf_s("%d", &size) != 1 || size <= 0) {
				printf("�������� ������ �������!\n");
				system("pause");
				break;
			}

			array = (int*)malloc(size * sizeof(int));
			if (array == NULL) {
				printf("������ ��������� ������!\n");
				system("pause");
				break;
			}

			printf("\n������ ������������ �����: 'input.txt'\n");
			if (fopen_s(&f, "input.txt", "w") != 0) {
				printf("������ �������� �����!\n");
				free(array);
				system("pause");
				break;
			}

			// ��������� ������������ ������������������
			array[0] = rand() % 10000;
			for (int i = 1; i < size; i++) {
				array[i] = array[i - 1] + (rand() % 100) + 100; // ����������� ��� 100
			}

			// ������ � ����
			for (int i = 0; i < size; i++) {
				fprintf(f, "%d ", array[i]);
			}
			fclose(f);

			printf("��������������� ������: 'output.txt'\n");
			if (fopen_s(&f, "output.txt", "w") != 0) {
				printf("������ �������� �����!\n");
				free(array);
				system("pause");
				break;
			}

			start = clock();
			count = 0;
			radixSort(array, size, &count);

			// ������ ������� ��� ���������� �� ��������
			for (int i = 0; i < size / 2; i++) {
				int temp = array[i];
				array[i] = array[size - i - 1];
				array[size - i - 1] = temp;
				count++;
			}

			stop = clock();

			// ������ ��������������� ������
			for (int i = 0; i < size; i++) {
				fprintf(f, "%d ", array[i]);
			}
			fclose(f);

			time = (double)(stop - start) / CLOCKS_PER_SEC;
			printf("\n����� ���������� ����������: %.3lf ���\n", time);
			printf("���������� ��������: %ld\n", count);

			free(array);
			system("pause");
			break;
		}

		case '4': {
			// ���������� ������������ �������� �� �����������
			system("cls");
			printf("������� ���������� ��������� �������: ");
			if (scanf_s("%d", &size) != 1 || size <= 0) {
				printf("�������� ������ �������!\n");
				system("pause");
				break;
			}

			array = (int*)malloc(size * sizeof(int));
			if (array == NULL) {
				printf("������ ��������� ������!\n");
				system("pause");
				break;
			}

			printf("\n������ ������������ �����: 'input.txt'\n");
			if (fopen_s(&f, "input.txt", "w") != 0) {
				printf("������ �������� �����!\n");
				free(array);
				system("pause");
				break;
			}

			// ��������� ������������ ������������������
			array[0] = rand() % 10000;
			for (int i = 1; i < size; i++) {
				array[i] = array[i - 1] + (rand() % 100) + 100; // ����������� ��� 100
			}

			// ������ � ����
			for (int i = 0; i < size; i++) {
				fprintf(f, "%d ", array[i]);
			}
			fclose(f);

			printf("��������������� ������: 'output.txt'\n");
			if (fopen_s(&f, "output.txt", "w") != 0) {
				printf("������ �������� �����!\n");
				free(array);
				system("pause");
				break;
			}

			start = clock();
			count = 0;
			radixSort(array, size, &count);
			stop = clock();

			// ������ ��������������� ������
			for (int i = 0; i < size; i++) {
				fprintf(f, "%d ", array[i]);
			}
			fclose(f);

			time = (double)(stop - start) / CLOCKS_PER_SEC;
			printf("\n����� ���������� ����������: %.3lf ���\n", time);
			printf("���������� ��������: %ld\n", count);

			free(array);
			system("pause");
			break;
		}
		}
	} while (ch != 27);

	return 0;
}