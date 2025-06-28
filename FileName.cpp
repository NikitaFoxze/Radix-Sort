#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <locale.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>
#include <limits.h>

// Функция для получения максимального элемента массива
int getMax(int arr[], int n) {
	int max = arr[0];
	for (int i = 1; i < n; i++) {
		if (arr[i] > max) {
			max = arr[i];
		}
	}
	return max;
}

// Функция для выполнения сортировки подсчетом по разряду exp
void countSort(int arr[], int n, int exp, long* count) {
	int* output = (int*)malloc(n * sizeof(int));
	if (output == NULL) {
		printf("Ошибка выделения памяти!\n");
		exit(1);
	}

	int countDigits[10] = { 0 };
	int i;

	// 1. Подсчет количества каждой цифры
	for (i = 0; i < n; i++) {
		int digit = (abs(arr[i]) / exp) % 10;  // Используем abs() для отрицательных чисел
		countDigits[digit]++;
	}

	// 2. Преобразование countDigits в позиции
	for (i = 1; i < 10; i++)
		countDigits[i] += countDigits[i - 1];

	// 3. Построение выходного массива
	for (i = n - 1; i >= 0; i--) {
		int digit = (abs(arr[i]) / exp) % 10;
		int pos = --countDigits[digit];

		// Проверка границ
		if (pos < 0 || pos >= n) {
			printf("Ошибка: выход за границы массива! pos=%d, n=%d\n", pos, n);
			free(output);
			exit(1);
		}

		output[pos] = arr[i];
		(*count)++;
	}

	// 4. Копирование обратно в исходный массив
	for (i = 0; i < n; i++) {
		arr[i] = output[i];
	}
	free(output);
}

// Основная функция для реализации поразрядной сортировки
void radixSort(int arr[], int n, long* count) {
	// Находим максимальное число, чтобы знать количество разрядов
	int m = getMax(arr, n);

	// Выполняем сортировку подсчетом для каждого разряда
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
		printf("МЕНЮ:\n");
		printf("1 - Сортировка случайных значений по возрастанию\n");
		printf("2 - Сортировка случайных значений по убыванию\n");
		printf("3 - Сортировка возрастающих значений по убыванию\n");
		printf("4 - Сортировка возрастающих значений по возрастанию\n");
		printf("ESC - выход\n");
		ch = _getch();

		clock_t start, stop;
		double time;
		long count;

		switch (ch) {
		case '1': {
			system("cls");
			printf("Введите количество элементов массива: ");
			if (scanf("%d", &size) != 1 || size <= 0) {
				printf("Неверный размер массива!\n");
				system("pause");
				break;
			}

			array = (int*)malloc(size * sizeof(int));
			if (array == NULL) {
				printf("Ошибка выделения памяти!\n");
				system("pause");
				break;
			}

			printf("\nМассив случайных чисел: 'input.txt'\n");
			f = fopen("input.txt", "w");
			if (f == NULL) {
				printf("Ошибка открытия файла!\n");
				free(array);
				system("pause");
				break;
			}

			for (int i = 0; i < size; i++) {
				array[i] = rand() - RAND_MAX / 2;
				fprintf(f, "%d ", array[i]);
			}
			fclose(f);

			printf("Отсортированный массив: 'output.txt'\n");
			f = fopen("output.txt", "w");
			if (f == NULL) {
				printf("Ошибка открытия файла!\n");
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
			printf("\nВремя выполнения сортировки: %.3lf сек\n", time);
			printf("Количество операций: %ld\n", count);

			free(array);
			system("pause");
			break;
		}
		case '2': {
			// Сортировка случайных значений по убыванию
			system("cls");
			printf("Введите количество элементов массива: ");
			if (scanf_s("%d", &size) != 1 || size <= 0) {
				printf("Неверный размер массива!\n");
				system("pause");
				break;
			}

			array = (int*)malloc(size * sizeof(int));
			if (array == NULL) {
				printf("Ошибка выделения памяти!\n");
				system("pause");
				break;
			}

			printf("\nМассив случайных чисел: 'input.txt'\n");
			if (fopen_s(&f, "input.txt", "w") != 0) {
				printf("Ошибка открытия файла!\n");
				free(array);
				system("pause");
				break;
			}

			for (int i = 0; i < size; i++) {
				array[i] = rand() - rand();
				fprintf(f, "%d ", array[i]);
			}
			fclose(f);

			printf("Отсортированный массив: 'output.txt'\n");
			if (fopen_s(&f, "output.txt", "w") != 0) {
				printf("Ошибка открытия файла!\n");
				free(array);
				system("pause");
				break;
			}

			start = clock();
			count = 0;
			radixSort(array, size, &count);

			// Для сортировки по убыванию просто перевернем массив
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
			printf("\nВремя выполнения сортировки: %.3lf сек\n", time);
			printf("Количество операций: %ld\n", count);

			free(array);
			system("pause");
			break;
		}
		case '3': {
			// Сортировка возрастающих значений по убыванию
			system("cls");
			printf("Введите количество элементов массива: ");
			if (scanf_s("%d", &size) != 1 || size <= 0) {
				printf("Неверный размер массива!\n");
				system("pause");
				break;
			}

			array = (int*)malloc(size * sizeof(int));
			if (array == NULL) {
				printf("Ошибка выделения памяти!\n");
				system("pause");
				break;
			}

			printf("\nМассив возрастающих чисел: 'input.txt'\n");
			if (fopen_s(&f, "input.txt", "w") != 0) {
				printf("Ошибка открытия файла!\n");
				free(array);
				system("pause");
				break;
			}

			// Генерация возрастающей последовательности
			array[0] = rand() % 10000;
			for (int i = 1; i < size; i++) {
				array[i] = array[i - 1] + (rand() % 100) + 100; // Минимальный шаг 100
			}

			// Запись в файл
			for (int i = 0; i < size; i++) {
				fprintf(f, "%d ", array[i]);
			}
			fclose(f);

			printf("Отсортированный массив: 'output.txt'\n");
			if (fopen_s(&f, "output.txt", "w") != 0) {
				printf("Ошибка открытия файла!\n");
				free(array);
				system("pause");
				break;
			}

			start = clock();
			count = 0;
			radixSort(array, size, &count);

			// Реверс массива для сортировки по убыванию
			for (int i = 0; i < size / 2; i++) {
				int temp = array[i];
				array[i] = array[size - i - 1];
				array[size - i - 1] = temp;
				count++;
			}

			stop = clock();

			// Запись отсортированных данных
			for (int i = 0; i < size; i++) {
				fprintf(f, "%d ", array[i]);
			}
			fclose(f);

			time = (double)(stop - start) / CLOCKS_PER_SEC;
			printf("\nВремя выполнения сортировки: %.3lf сек\n", time);
			printf("Количество операций: %ld\n", count);

			free(array);
			system("pause");
			break;
		}

		case '4': {
			// Сортировка возрастающих значений по возрастанию
			system("cls");
			printf("Введите количество элементов массива: ");
			if (scanf_s("%d", &size) != 1 || size <= 0) {
				printf("Неверный размер массива!\n");
				system("pause");
				break;
			}

			array = (int*)malloc(size * sizeof(int));
			if (array == NULL) {
				printf("Ошибка выделения памяти!\n");
				system("pause");
				break;
			}

			printf("\nМассив возрастающих чисел: 'input.txt'\n");
			if (fopen_s(&f, "input.txt", "w") != 0) {
				printf("Ошибка открытия файла!\n");
				free(array);
				system("pause");
				break;
			}

			// Генерация возрастающей последовательности
			array[0] = rand() % 10000;
			for (int i = 1; i < size; i++) {
				array[i] = array[i - 1] + (rand() % 100) + 100; // Минимальный шаг 100
			}

			// Запись в файл
			for (int i = 0; i < size; i++) {
				fprintf(f, "%d ", array[i]);
			}
			fclose(f);

			printf("Отсортированный массив: 'output.txt'\n");
			if (fopen_s(&f, "output.txt", "w") != 0) {
				printf("Ошибка открытия файла!\n");
				free(array);
				system("pause");
				break;
			}

			start = clock();
			count = 0;
			radixSort(array, size, &count);
			stop = clock();

			// Запись отсортированных данных
			for (int i = 0; i < size; i++) {
				fprintf(f, "%d ", array[i]);
			}
			fclose(f);

			time = (double)(stop - start) / CLOCKS_PER_SEC;
			printf("\nВремя выполнения сортировки: %.3lf сек\n", time);
			printf("Количество операций: %ld\n", count);

			free(array);
			system("pause");
			break;
		}
		}
	} while (ch != 27);

	return 0;
}