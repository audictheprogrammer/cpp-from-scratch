# myvector

A from-scratch reimplementation of `std::vector<T>`, built on top of
`std::allocator<T>`. This keeps raw memory allocation separate from
object construction/destruction, so it works correctly even for types
with no default constructor — something a naive `new[]`-based
implementation can't do.

## Build & run

```bash
g++ -std=c++17 -g myvector.cpp -o test
./test
valgrind --leak-check=full ./test   # check for leaks
```

## API

### Construction / destruction

| Signature | Description |
|---|---|
| `MyVector()` | Creates an empty vector. |
| `MyVector(const MyVector<T>& v)` | Copy constructor: deep copy of `v`. |
| `MyVector(MyVector<T>&& v)` | Move constructor: takes ownership of `v`'s buffer. |
| `~MyVector()` | Destructor: releases the internal buffer. |

### Assignment

| Signature | Description |
|---|---|
| `operator=(const MyVector<T>& v)` | Copy assignment: deep copy, safe on self-assignment. |
| `operator=(MyVector<T>&& v)` | Move assignment: takes ownership of `v`'s buffer, safe on self-assignment. |

### Capacity

| Signature | Description |
|---|---|
| `size_t size() const` | Number of elements currently stored. |
| `size_t capacity() const` | Number of elements the buffer can hold before reallocating. |
| `bool empty() const` | Returns `true` if the vector has no elements. |
| `void reserve(size_t n)` | Grows capacity to at least `n`, without changing `size()`. Does nothing if `n <= capacity()`. |

### Modifiers

| Signature | Description |
|---|---|
| `void push_back(const T& value)` | Appends `value` at the end, reallocating if needed. |
| `void pop_back()` | Removes the last element. Undefined behavior if the vector is empty (same as `std::vector`). |
| `void clear()` | Destroys all elements, but keeps the allocated capacity. |

### Element access

| Signature | Description |
|---|---|
| `T& operator[](size_t index)` | Fast, unchecked access (`assert` only in debug builds). |
| `const T& operator[](size_t index) const` | Same as above, read-only. |
| `T& at(size_t index)` | Checked access. Throws `std::out_of_range` if `index >= size()`. |
| `const T& at(size_t index) const` | Same bounds check, read-only. |

### Iterators

| Signature | Description |
|---|---|
| `T* begin()` / `T* end()` | Mutable iterators, enable range-based `for` loops. |
| `const T* begin() const` / `const T* end() const` | Const iterators, used when the vector is `const`. |

## Developer's notes

- **`std::allocator<T>` instead of `new T[]`**: separates raw memory
  allocation from object construction. This lets `reserve()` work
  without constructing "phantom" objects, and lets the vector work
  with types that have no default constructor.
- **Rule of five**: if you have to manually write one of the five special
  member functions (destructor, copy constructor, copy assignment, move
  constructor, move assignment), you probably need to write all five:
  the compiler can't safely guess how to copy/move a raw resource like
  `m_data`. (There's also the "Rule of Zero": if a class only has members
  that already manage their own resources: e.g. a `std::vector` or
  `std::unique_ptr` member instead of a raw pointer: the compiler can
  generate all five correctly on its own, so you don't need to write any
  of them.)
- **`operator[]` vs `at()`**: same as the real STL. `operator[]` is
  fast and unchecked; `at()` throws on out-of-bounds access.
- **`push_back()` growth factor**: capacity doubles (`×2`) when full.
  Some real-world implementations use `×1.5` instead (better memory
  reuse in some allocators): `×2` is simpler and what I went with here.
- **`size_t` is unsigned**: it can never be negative, which is why bounds
  checks only need `index >= size_` and not `index < 0`.
- **Iterators are just `T*`**: this lets both styles work: range-based
  loops (`for (auto& value : v)`) and manual pointer-style iteration
  (`for (auto it = v.begin(); it != v.end(); ++it)`): without writing a
  custom iterator class.
- **Not exception-safe yet**: if `T`'s copy constructor throws during a
  reallocation, the vector leaks memory instead of rolling back cleanly.