#include "dynamic_array.cpp"
#include <iostream>

#define VARIANT 5
#define ROWS 2
#define COLS 2

void transformMatrix2(dynamic_array<dynamic_array<long>>& arr, unsigned long* array_ptr) {
	unsigned long decrement = 0;

	for (unsigned long k = 1; k < *array_ptr + 1; ++k) {
		for (unsigned long long i = 0; i < arr.size; ++i) {
			arr[i].pop(*(array_ptr + k) - decrement);
		}
		decrement++;
	}
}

unsigned long* findZeros(dynamic_array<dynamic_array<long>>& arr)
{
	unsigned long* zeros_arr = reinterpret_cast<unsigned long*>(malloc((arr[0].size + 1) * sizeof(unsigned long))) + 1;
	unsigned long arr_size = 0;

	for (unsigned long long j = 0; j < arr[0].size; ++j) {
		for (unsigned long long i = 0; i < arr.size; ++i) {
			if (arr[i][j] == 0) {
				zeros_arr[arr_size++] = j;			
				break;
			}
		}
	}
	*(--zeros_arr) = arr_size; // возвращаем не вектор из функции
	return zeros_arr;
}

void transformMatrix(dynamic_array<dynamic_array<long>>& arr)
{
	long a = arr[0][0], b = arr[0][1], c = arr[1][0], d = arr[1][1];

	unsigned long long value;
	for (unsigned long long i = 0; i < ROWS; ++i) {
		for (unsigned long long j = 0; j < b; ++j) {
			arr[i].push_back((i - 1) * c + (j + 1) * d);
		}
	}
	for (unsigned long long i = ROWS; i < ROWS + a; ++i) {
		arr.push_back(dynamic_array<long>());
		for (unsigned long long j = 0; j < COLS + b; ++j) {
			arr[i].push_back((i - ROWS + 1) * c + (j - 1) * d);
		}
	}
}

void firstClause()
{
	dynamic_array<dynamic_array<long>> arr(ROWS, dynamic_array<long>(COLS)); // dynamic_array.cpp
	
	long temp;
	std::cout << "Введите A: ";
	for (;std::cin >> temp; std::cout << "Ошибка (A < 0)" << std::endl << "Введите A: ") {
		if (temp >= 0) break; 
	}
	arr[0][0] = temp;
	std::cout << "Введите B: ";
	for (;std::cin >> temp; std::cout << "Ошибка (B < 0)" << std::endl << "Введите B: ") {
		if (temp >= 0) break; 
	}
	arr[0][1] = temp;
	std::cout << "Введите C: ";
	std::cin >> arr[1][0];
	std::cout << "Введите D: ";
	std::cin >> arr[1][1];

	transformMatrix(arr);
	unsigned long* zeros_ptr = findZeros(arr);
	transformMatrix2(arr, zeros_ptr);

	for (unsigned long long i = 0; i < arr.size; ++i) {
		for (unsigned long long j = 0; j < arr[i].size; ++j)
			std::cout << arr[i][j] << " ";
		std::cout << std::endl;
	}
}

void secondClause()
{
	double a, b, *pa = &a, *pb = &b;
	std::cout << "Введите A и B: ";
	std::cin >> a >> b;
	*pa *= 3;
	pa = new double(a);
	pb = new double(b);
	a = *pb;
	b = *pa;
	delete pa;
	delete pb;
	std::cout << "A = " << a << ", B = " << b;
	pa = &a;
	pb = &b;
}

int main()
{
	setlocale(LC_ALL, "Russian");
	uint16_t flag;
	std::cout << "Пункт 1: 1" << std::endl << "Пункт 2: 2" << std::endl;
	std::cin >> flag;
	if (flag == 1) 
		firstClause();
	else if (flag == 2)
		secondClause();
	return 0;
}