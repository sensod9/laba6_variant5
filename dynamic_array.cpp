#include <initializer_list> // мог бы и через va_list
#include <cstdlib> // malloc
#include <cstring> // memcpy (strcpy)
#include <iostream>

#define DEFAULT_CAPACITY 4

// из-за нетривиальности используемого в лабе типа испытал огромную боль настоящего ООП. оно того стоило, многое стало проясняться

template<typename T>
class dynamic_array {
public:
	dynamic_array<T>() : size(0), capacity(DEFAULT_CAPACITY), inner(reinterpret_cast<T*>(calloc(capacity, sizeof(T)))) {};
	dynamic_array<T>(const dynamic_array<T>& other) {
		size = other.size;
		capacity = other.capacity;
		inner = reinterpret_cast<T*>(malloc(capacity * sizeof(T)));
		for (unsigned long long i = 0; i < size; ++i) {
			new (&inner[i]) T(other.inner[i]);
			// inner[i] = other.inner[i];
		} // нетривиальный тип dynamic_array<long> через memcpy не идёт
	}
	explicit dynamic_array<T>(unsigned long long size) : size(size), capacity(size + DEFAULT_CAPACITY), inner(reinterpret_cast<T*>(malloc(capacity * sizeof(T)))) {
		for (unsigned long long i = 0; i < size; ++i) {
			new (&inner[i]) T();
		}
	};
	explicit dynamic_array<T>(unsigned long long size, const T& value) : size(size), capacity(size + DEFAULT_CAPACITY), inner(reinterpret_cast<T*>(malloc(capacity * sizeof(T)))) {
		for (unsigned long long i = 0; i < size; ++i) {
			new (&inner[i]) T(value);
			// memcpy(&inner[i], &value, sizeof(T));
		}
	};
	dynamic_array<T>(std::initializer_list<T> init) : size(init.size()), capacity(size + DEFAULT_CAPACITY), inner(reinterpret_cast<T*>(malloc(capacity * sizeof(T)))) {
		unsigned long long i = 0;
		for (auto& e : init) {
			new(inner[i++]) T(e);
		}		
	}

	~dynamic_array<T>() {
		freeInner();
	}
	
	dynamic_array<T>& operator=(const dynamic_array<T>& other) {
		freeInner();
		size = other.size;
		capacity = other.capacity;
		inner = reinterpret_cast<T*>(malloc(capacity * sizeof(T)));
		for (unsigned long long i = 0; i < size; ++i) {
			inner[i] = other.inner[i];
		}
		return *this;
	}
	
	T& operator[](unsigned long long index) {
		if (index >= size) throw "Выход за границу массива";
		return inner[index];
	};
	unsigned long long size;
	unsigned long long capacity;
	dynamic_array<T>& insert(unsigned long long index, T e) { // сегодня без итераторов
		++size;
		if (index >= size) throw "Выход за границу массива";
		if (capacity < size) {
			capacity += DEFAULT_CAPACITY;
			T* new_inner = reinterpret_cast<T*>(malloc(capacity * sizeof(T))); // new T[capacity];

			for (unsigned long long i = 0; i < index; ++i) {
				new (&new_inner[i]) T(inner[i]);
			}
			new (&new_inner[index]) T(e);
			if (index != size - 1) {
				for (unsigned long long i = index; i < size - 1; ++i) {
					new (&new_inner[i]) T(inner[i]);
				}
			}
			freeInner();
			inner = new_inner;
		}
		else {
			for (unsigned long long i = size - 1; i > index; ++i) {
				new (&inner[i]) T(inner[i-1]);
				inner[i-1].~T();
			}
			new (&inner[index]) T(e);
		}
		return *this;
	}

	dynamic_array<T>& push_back(T e) {
		insert(size, e);
		return *this;
	}

	// erase для лабы не нужен
	T* inner;
private:
	void freeInner()
	{
		for (unsigned long long i = 0; i < size; ++i) {
			inner[i].~T();
		}
		free(inner);
	}
};