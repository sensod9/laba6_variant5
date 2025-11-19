#include <initializer_list>
#include <cstdlib> // malloc

#define DEFAULT_CAPACITY 4

// из-за нетривиальности используемого в лабе типа было больно, но оно того стоило: многое стало проясняться

template<typename T>
class dynamic_array {
public:
	unsigned long long capacity;
	unsigned long long size;
	T* inner;

	dynamic_array<T>() : size(0), capacity(DEFAULT_CAPACITY) {
		inner = reinterpret_cast<T*>(calloc(capacity, sizeof(T)));
	} // default
	
	dynamic_array<T>(const dynamic_array<T>& other) : size(other.size), capacity(other.capacity) {
		inner = reinterpret_cast<T*>(malloc(capacity * sizeof(T)));
		for (unsigned long long i = 0; i < size; ++i) {
			// и тут я осознал, что не получится на сишных функциях вывезти (или я слаб)
			new (&inner[i]) T(other.inner[i]);
			// inner[i] = other.inner[i], но нетривиальный тип => placement new в выделенный inner + констр. копир.
		}
	} // copy

	dynamic_array<T>(unsigned long long size) : size(size), capacity(size + DEFAULT_CAPACITY) {
		inner = reinterpret_cast<T*>(malloc(capacity * sizeof(T)));
		for (unsigned long long i = 0; i < size; ++i) {
			new (&inner[i]) T();
		}
	} // by size

	dynamic_array<T>(unsigned long long size, const T& value) : size(size), capacity(size + DEFAULT_CAPACITY) {
		inner = reinterpret_cast<T*>(malloc(capacity * sizeof(T)));
		for (unsigned long long i = 0; i < size; ++i) {
			new (&inner[i]) T(value);
		}
	} // такая себе реализация многомерности
	// dynamic_array<dynamic_array<long>> arr(ROWS, dynamic_array<long>(COLS))

	dynamic_array<T>(std::initializer_list<T> init) : size(init.size()), capacity(size + DEFAULT_CAPACITY) {
		inner = reinterpret_cast<T*>(malloc(capacity * sizeof(T)));
		unsigned long long i = 0;
		for (auto& e : init) {
			new(&inner[i++]) T(e);
		}
	} // было интересно, как реализуется различие arr{3} и arr(3). всё равно магия какая-то

	~dynamic_array<T>() {
		freeInner();
	}

	T& operator[](unsigned long long index) {
		if (index >= size) throw "Выход за границу массива";
		return inner[index];
	} // сегодня без итераторов
	
	dynamic_array<T>& operator=(const dynamic_array<T>& other) {
		freeInner();
		size = other.size;
		capacity = other.capacity;
		inner = reinterpret_cast<T*>(malloc(capacity * sizeof(T)));
		for (unsigned long long i = 0; i < size; ++i) {
			new (&inner[i]) T(other.inner[i]);
		}
		return *this;
	}

	dynamic_array<T>& insert(unsigned long long index, T e) {
		++size;
		if (index >= size) throw "Выход за границу массива";
		if (capacity < size) {
			capacity += DEFAULT_CAPACITY;
			T* new_inner = reinterpret_cast<T*>(malloc(capacity * sizeof(T)));

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

	// erase писать уже скучно

private:
	void freeInner()
	{
		for (unsigned long long i = 0; i < size; ++i) {
			inner[i].~T();
		}
		free(inner);
	}
};