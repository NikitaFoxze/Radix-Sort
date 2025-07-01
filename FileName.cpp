#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <locale.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>
#include <limits.h>
#include <string.h>

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
		int digit = (abs(arr[i]) / exp) % 10;
		countDigits[digit]++;
	}

	// 2. Преобразование countDigits в позиции
	for (i = 1; i < 10; i++) {
		countDigits[i] += countDigits[i - 1];
	}

	// 3. Построение выходного массива
	for (i = n - 1; i >= 0; i--) {
		int digit = (abs(arr[i]) / exp) % 10;
		int pos = --countDigits[digit];

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
	int m = getMax(arr, n);
	for (int exp = 1; m / exp > 0; exp *= 10) {
		countSort(arr, n, exp, count);
	}
}

// Функция для записи массива в CSV файл
void writeCSV(const char* filename, int arr[], int n) {
	FILE* file = fopen(filename, "w");
	if (file == NULL) {
		printf("Ошибка открытия файла %s для записи!\n", filename);
		return;
	}

	for (int i = 0; i < n; i++) {
		fprintf(file, "%d", arr[i]);
		if (i < n - 1) fprintf(file, ",");
	}

	fclose(file);
}

// Функция для чтения массива из CSV файла
int* readCSV(const char* filename, int* size) {
	FILE* file = fopen(filename, "r");
	if (file == NULL) {
		printf("Ошибка открытия файла %s для чтения!\n", filename);
		return NULL;
	}

	// Подсчет количества чисел в файле
	int count = 0;
	char ch;
	while ((ch = fgetc(file)) != EOF) {
		if (ch == ',') count++;
	}
	count++; // Последнее число без запятой
	rewind(file);

	// Выделение памяти
	int* arr = (int*)malloc(count * sizeof(int));
	if (arr == NULL) {
		printf("Ошибка выделения памяти!\n");
		fclose(file);
		return NULL;
	}

	// Чтение чисел
	for (int i = 0; i < count; i++) {
		if (fscanf(file, "%d,", &arr[i]) != 1) {
			printf("Ошибка чтения данных из файла!\n");
			free(arr);
			fclose(file);
			return NULL;
		}
	}

	fclose(file);
	*size = count;
	return arr;
}

// Функция для ручного ввода массива
int* manualInput(int* size) {
	printf("Введите количество элементов массива: ");
	scanf_s("%d", size);
	if (*size <= 0) {
		printf("Неверный размер массива!\n");
		return NULL;
	}

	int* arr = (int*)malloc(*size * sizeof(int));
	if (arr == NULL) {
		printf("Ошибка выделения памяти!\n");
		return NULL;
	}

	printf("Введите %d элементов массива:\n", *size);
	for (int i = 0; i < *size; i++) {
		printf("Элемент %d: ", i + 1);
		scanf_s("%d", &arr[i]);
	}

	return arr;
}

// Функция для генерации случайного массива с диапазоном значений
int* generateRandomArray(int size, int minVal, int maxVal) {
	int* arr = (int*)malloc(size * sizeof(int));
	if (arr == NULL) {
		printf("Ошибка выделения памяти!\n");
		return NULL;
	}

	for (int i = 0; i < size; i++) {
		arr[i] = rand() % (maxVal - minVal + 1) + minVal;
	}

	return arr;
}

// Функция для вывода массива на экран (с ограничением до 100 элементов)
void printArray(int arr[], int n) {
	printf("Массив (%d элементов):\n", n);
	int limit = (n > 100) ? 100 : n;

	for (int i = 0; i < limit; i++) {
		printf("%d ", arr[i]);
		if ((i + 1) % 10 == 0) printf("\n");
	}

	if (n > 100) {
		printf("\n... (показаны первые 100 элементов из %d)\n", n);
	}
	printf("\n");
}

// Функция для вывода меню (с простыми символами)
void printMenu() {
	printf("\n===============================================\n");
	printf("|          МЕНЮ ПРОГРАММЫ                     |\n");
	printf("===============================================\n");
	printf("| 1 - Сортировка случайного массива           |\n");
	printf("| 2 - Сортировка вручную введенного           |\n");
	printf("| 3 - Сортировка данных из файла              |\n");
	printf("| 4 - Записать массив в CSV файл              |\n");
	printf("| ESC - Выход из программы                    |\n");
	printf("===============================================\n\n");
	printf("Выберите действие: ");
}

int main() {
	setlocale(LC_ALL, "Russian");
	srand((unsigned int)time(NULL));

	int* array = NULL;
	int size = 0;
	char ch;

	do {
		system("cls");
		printMenu();
		ch = _getch();

		clock_t start, stop;
		double time;
		long count = 0;

		switch (ch) {
		case '1': {
			system("cls");
			printf("================================================\n");
			printf("|     ГЕНЕРАЦИЯ СЛУЧАЙНОГО МАССИВА             |\n");
			printf("================================================\n");

			int minVal, maxVal;
			printf("Введите количество элементов: ");
			scanf_s("%d", &size);
			printf("Введите минимальное значение: ");
			scanf_s("%d", &minVal);
			printf("Введите максимальное значение: ");
			scanf_s("%d", &maxVal);

			if (size <= 0 || minVal > maxVal) {
				printf("Неверные параметры массива!\n");
				system("pause");
				break;
			}

			if (array != NULL) free(array);
			array = generateRandomArray(size, minVal, maxVal);
			if (array == NULL) {
				system("pause");
				break;
			}

			printf("\nСгенерированный массив:\n");
			printArray(array, size);

			printf("\nВыберите направление сортировки:\n");
			printf("1 - По возрастанию\n");
			printf("2 - По убыванию\n");
			char sortChoice = _getch();

			start = clock();
			count = 0;
			radixSort(array, size, &count);

			if (sortChoice == '2') {
				// Реверс массива для сортировки по убыванию
				for (int i = 0; i < size / 2; i++) {
					int temp = array[i];
					array[i] = array[size - i - 1];
					array[size - i - 1] = temp;
					count++;
				}
			}

			stop = clock();

			printf("\nОтсортированный массив:\n");
			printArray(array, size);

			time = (double)(stop - start) / CLOCKS_PER_SEC;
			printf("\nВремя выполнения сортировки: %.3lf сек\n", time);
			printf("Количество операций: %ld\n", count);

			writeCSV("output.csv", array, size);
			printf("Результат сохранен в output.csv\n");

			system("pause");
			break;
		}
		case '2': {
			system("cls");
			printf("================================================\n");
			printf("|        РУЧНОЙ ВВОД МАССИВА                   |\n");
			printf("================================================\n");

			if (array != NULL) free(array);
			array = manualInput(&size);
			if (array == NULL) {
				system("pause");
				break;
			}

			printf("\nВведенный массив:\n");
			printArray(array, size);

			printf("\nВыберите направление сортировки:\n");
			printf("1 - По возрастанию\n");
			printf("2 - По убыванию\n");
			char sortChoice = _getch();

			start = clock();
			count = 0;
			radixSort(array, size, &count);

			if (sortChoice == '2') {
				// Реверс массива для сортировки по убыванию
				for (int i = 0; i < size / 2; i++) {
					int temp = array[i];
					array[i] = array[size - i - 1];
					array[size - i - 1] = temp;
					count++;
				}
			}

			stop = clock();

			printf("\nОтсортированный массив:\n");
			printArray(array, size);

			time = (double)(stop - start) / CLOCKS_PER_SEC;
			printf("\nВремя выполнения сортировки: %.3lf сек\n", time);
			printf("Количество операций: %ld\n", count);

			writeCSV("output.csv", array, size);
			printf("Результат сохранен в output.csv\n");

			system("pause");
			break;
		}
		case '3': {
			system("cls");
			printf("================================================\n");
			printf("|        ЧТЕНИЕ МАССИВА ИЗ ФАЙЛА               |\n");
			printf("================================================\n");

			char filename[100];
			printf("Введите имя файла для чтения (например, input.csv): ");
			scanf("%s", filename);

			if (array != NULL) free(array);
			array = readCSV(filename, &size);
			if (array == NULL) {
				system("pause");
				break;
			}

			printf("\nПрочитанный массив:\n");
			printArray(array, size);

			printf("\nВыберите направление сортировки:\n");
			printf("1 - По возрастанию\n");
			printf("2 - По убыванию\n");
			char sortChoice = _getch();

			start = clock();
			count = 0;
			radixSort(array, size, &count);

			if (sortChoice == '2') {
				// Реверс массива для сортировки по убыванию
				for (int i = 0; i < size / 2; i++) {
					int temp = array[i];
					array[i] = array[size - i - 1];
					array[size - i - 1] = temp;
					count++;
				}
			}

			stop = clock();

			printf("\nОтсортированный массив:\n");
			printArray(array, size);

			time = (double)(stop - start) / CLOCKS_PER_SEC;
			printf("\nВремя выполнения сортировки: %.3lf сек\n", time);
			printf("Количество операций: %ld\n", count);

			writeCSV("output.csv", array, size);
			printf("Результат сохранен в output.csv\n");

			system("pause");
			break;
		}
		case '4': {
			system("cls");
			printf("================================================\n");
			printf("|        СОХРАНЕНИЕ МАССИВА В ФАЙЛ             |\n");
			printf("================================================\n");

			if (array == NULL || size == 0) {
				printf("Нет данных для сохранения!\n");
				system("pause");
				break;
			}

			char filename[100];
			printf("Введите имя файла для сохранения (например, data.csv): ");
			scanf("%s", filename);

			writeCSV(filename, array, size);
			printf("Массив успешно сохранен в файл %s\n", filename);

			system("pause");
			break;
		}
		case 27: // ESC
			break;
		default:
			printf("\nНеверный выбор! Попробуйте снова.\n");
			system("pause");
			break;
		}
	} while (ch != 27);

	if (array != NULL) free(array);
	return 0;
}