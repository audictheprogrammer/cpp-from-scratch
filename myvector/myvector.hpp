#include <iostream>

template <typename T>
class MyVector {
    
public:
    // Default constructor.
    MyVector() : m_data(nullptr), m_size(0), m_capacity(0) {}

    // Copy constructor.
    MyVector(const MyVector<T>& v) {
        m_data = new T[v.m_capacity];
        for (size_t i = 0; i < v.m_size; i++) {
            m_data[i] = v.m_data[i];
        }
        m_size = v.m_size;
        m_capacity = v.m_capacity;
    }

    // Move constructor.
    MyVector(MyVector<T>&& v) {
        m_data = v.m_data;
        m_size = v.m_size;
        m_capacity = v.m_capacity;

        v.m_data = nullptr;
        v.m_size = 0;
        v.m_capacity = 0;
    }

    // Copy operator.
    MyVector<T>& operator=(const MyVector<T>& v) {
        if (this == &v) {
            return *this;
        }
        delete[] m_data; // delete[] applied on nullptr make no-op.
        m_data = new T[v.m_capacity];
        for (size_t i = 0; i < v.m_size; i++) {
            m_data[i] = v.m_data[i];
        }
        m_size = v.m_size;
        m_capacity = v.m_capacity;

        return *this;
    }

    // Move operator.
    MyVector& operator=(MyVector<T>&& v) {
        if (this == &v) {
            return *this;
        }
        delete[] m_data;
        m_data = v.m_data;
        m_size = v.m_size;
        m_capacity = v.m_capacity;
        
        v.m_data = nullptr;
        v.m_size = 0;
        v.m_capacity = 0;
        return *this;
    }

    // Destructor.
    ~MyVector() { delete[] m_data; }

    // Getter empty(): true if the vector contains no element.
    bool empty() const { return m_size == 0; }

    // Getter size().
    size_t size() const { return m_size; }

    // Getter capacity().
    size_t capacity() const { return m_capacity; }

    // Method push_back(): add an element at the end, reallocating if needed.
    void push_back(const T& value) {
        if (m_size == m_capacity) {
            // Increase capacity by +1 or by *2.
            m_capacity == 0 ? m_capacity++ : m_capacity*=2;

            // Allocate new dynamic array and copy.
            T* new_data = new T[m_capacity];
            for (size_t i = 0; i < m_size; i++) {
                new_data[i] = m_data[i];
            }
            // Delete previous array and make it point to the new one.
            delete[] m_data;
            m_data = new_data;
        }

        m_data[m_size++] = value;

    }

    // Method pop_back(): remove and return the last element.
    T pop_back() {
        if (m_size == 0) {
            throw std::out_of_range("cannot pop, vector is empty.");
        }
        T res = m_data[--m_size];
        return res;
    }

    // Access operator.
    T& operator[](size_t index) {
        if (index >= m_size) {
            throw std::out_of_range("index out of bounds");
        }
        return m_data[index];
    }
    // Const access operator.
    const T& operator[](size_t index) const {
        if (index >= m_size) {
            throw std::out_of_range("index out of bounds");
        }
        return m_data[index];
    }


    // Iterators.
    T* begin() { return m_data; }
    T* end() { return m_data + m_size; }

    // Const iterators.
    const T* begin() const { return m_data; }
    const T* end() const { return m_data + m_size; }


private:
    T* m_data;
    size_t m_size;
    size_t m_capacity;

};