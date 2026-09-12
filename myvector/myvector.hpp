#pragma once

#include <iostream>
#include <memory>
#include <cassert>
#include <stdexcept>


template <typename T>
class MyVector {
private:
    std::allocator<T> m_alloc;
    T*     m_data;
    size_t m_capacity;
    size_t m_size;

public:
    /* Default constructor: creates an empty vector with no allocation. */
    MyVector() : m_data(nullptr), m_capacity(0), m_size(0) {}

    /* Copy constructor: deep-copies all elements from other. */
    MyVector(const MyVector<T>& other)
        : m_data(other.m_capacity? m_alloc.allocate(other.m_capacity): nullptr), m_capacity(other.m_capacity), m_size(0) {
        for (size_t i = 0; i < other.m_size; i++) {
            std::allocator_traits<decltype(m_alloc)>::construct(
                m_alloc,
                m_data + i,
                other.m_data[i]
            );
            m_size++;
        }
    }

    /* Move constructor: steals other's buffer, leaving it empty. */
    MyVector(MyVector<T>&& other) noexcept: m_data(other.m_data), m_capacity(other.m_capacity), m_size(other.m_size) {
        other.m_data = nullptr;
        other.m_capacity = 0;
        other.m_size = 0;
    }

    /* Copy operator: releases current storage, then deep-copies other. */
    MyVector<T>& operator=(const MyVector<T>& other) {
        if (this == &other) return *this;
        if (m_capacity) {
            (*this).clear();
            m_alloc.deallocate(m_data, m_capacity);
            m_capacity = 0;
            m_size = 0;
        }

        m_capacity = other.m_capacity;
        m_data = m_alloc.allocate(m_capacity);
        for (size_t i = 0; i < other.m_size; i++) {
            std::allocator_traits<decltype(m_alloc)>::construct(
                m_alloc,
                m_data + i,
                other.m_data[i]
            );
            m_size++;
        }

        return *this;
    }

    /* Move operator: releases current storage, then steals other's buffer. */
    MyVector<T>& operator=(MyVector<T>&& other) noexcept{
        if (this == &other) return *this;
        if (m_capacity) {
            clear();
            m_alloc.deallocate(m_data, m_capacity);
            m_capacity = 0;
        }

        m_capacity = other.m_capacity;
        other.m_capacity = 0;

        m_data = other.m_data;
        other.m_data = nullptr;

        m_size = other.m_size;
        other.m_size = 0;

        return *this;
    }

    /* Destructor: destroys all constructed elements and releases storage. */
    ~MyVector() {
        // Destroy elements from the vector.
        clear();
        // Deallocate.
        if (m_data != nullptr) {
            m_alloc.deallocate(m_data, m_capacity);
        }
    }   

    /* Grows capacity to at least new_capacity, preserving existing elements.
       No-op if new_capacity <= capacity(). */
    void reserve(size_t new_capacity) {
        if (new_capacity <= m_capacity) {return ; }

        T* new_data = m_alloc.allocate(new_capacity);

        for (size_t i = 0; i < m_size; i++) {
            std::allocator_traits<decltype(m_alloc)>::construct(
                m_alloc,
                new_data + i,
                m_data[i]
            );
            std::allocator_traits<decltype(m_alloc)>::destroy(
                m_alloc,
                m_data + i
            );
        }
        
        m_alloc.deallocate(m_data, m_capacity);
        m_data = new_data;
        m_capacity = new_capacity;
    }

    /* Appends a copy of value at the end, doubling capacity if full. */
    void push_back(const T& value) {
        // Step 1: Increase capacity if the vector is full.
        if (m_capacity == m_size) {
            size_t new_capacity = (m_capacity == 0)? 1: 2 * m_capacity;
            reserve(new_capacity);
        }

        // Step 2: Adding new elements to the vector.
        std::allocator_traits<decltype(m_alloc)>::construct(
            m_alloc,
            m_data + m_size++,
            value
        );
    }

    bool empty() const { return m_size == 0; }
    size_t size() const { return m_size; }
    size_t capacity() const { return m_capacity; }

    /* Non-const data access operator. */
    T& operator[](size_t i) {
        assert(i < m_size);
        return m_data[i];
    }
    /* Const data access operator. */
    const T& operator[](size_t i) const {
        assert(i < m_size);
        return m_data[i];
    }

    T& at(size_t i) {
        if (i >= m_size) {
            throw std::out_of_range("index out of bounds");
        }
        return m_data[i];
    }
    const T& at(size_t i) const {
        if (i >= m_size) {
            throw std::out_of_range("index out of bounds");
        }
        return m_data[i];
    }

    /* Iterators (contiguous storage, so a raw pointer suffices). */

    T* begin() { return m_data; }
    T* end() { return m_data + m_size; }
    const T* begin() const { return m_data; }
    const T* end() const { return m_data + m_size; }

    /* Destroys all elements, keeping allocated capacity. */
    void clear() {
        while (m_size > 0) {
            m_size--;
            std::allocator_traits<decltype(m_alloc)>::destroy(
                m_alloc,
                m_data + m_size
            );
        }
    }

    /* Removes the last element. Undefined behavior if the vector is empty. */
    void pop_back() {
        assert(m_size > 0);
        std::allocator_traits<decltype(m_alloc)>::destroy(
            m_alloc,
            m_data + --m_size
        );
    }

};
