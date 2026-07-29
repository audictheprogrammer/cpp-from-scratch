# myuniqueptr

A from-scratch reimplementation of `std::unique_ptr<T>`.

## Build & run

```bash
g++ -std=c++17 -g myuniqueptr.cpp -o test
./test
valgrind --leak-check=full ./test   # check for leaks
```

## API

### Construction / destruction

| Signature | Description |
|---|---|
| `explicit MyUniquePtr(T* ptr = nullptr)` | Takes ownership of a raw pointer (or none). |
| `~MyUniquePtr()` | Deletes the owned object. |

### Copy: forbidden

| Signature | Description |
|---|---|
| `MyUniquePtr(const MyUniquePtr&) = delete` | Copying is disabled: see notes below. |
| `operator=(const MyUniquePtr&) = delete` | Same reason. |

### Move: the only way to transfer ownership

| Signature | Description |
|---|---|
| `MyUniquePtr(MyUniquePtr&& other) noexcept` | Steals `other`'s pointer, leaves `other` empty. |
| `operator=(MyUniquePtr&& other) noexcept` | Releases its own resource first, then steals `other`'s. Safe on self-assignment. |

### Access / conversion

| Signature | Description |
|---|---|
| `T& operator*() const` | Dereference. Access the pointed-to object. |
| `T* operator->() const` | Member access, behaves like a raw pointer. |
| `explicit operator bool() const` | Enables `if (ptr)` checks. |

### Resource management

| Signature | Description |
|---|---|
| `T* get() const` | Returns the raw pointer without releasing ownership. |
| `void reset(T* ptr = nullptr)` | Deletes the currently owned object, takes ownership of `ptr` (or none). |
| `T* release()` | Releases ownership without deleting. Returns the raw pointer, now the caller's responsibility. |

### Factory

| Signature | Description |
|---|---|
| `make_unique<T>(Args&&... args)` | Allocates a `T` and wraps it in a `MyUniquePtr<T>` in one step. Free function, not a member. |

## Developer's notes

- **Why `explicit` on the constructor**: without it, there are two ways to
  create a `MyUniquePtr` from a raw pointer: `MyUniquePtr<int> p(raw);`
  (direct call) and `MyUniquePtr<int> p = raw;` (implicit conversion).
  The second one is dangerous because it can happen silently, without
  looking like a constructor call at all. Example:
  ```cpp
  void process(MyUniquePtr<int> p) { /* ... */ } // takes ownership

  int* a = new int(5);
  process(a);                    // a is implicitly converted to a MyUniquePtr<int>
  std::cout << *a << std::endl;  // a is dangling: process() already deleted it
  ```
  `process(a)` looks like an ordinary function call, but it silently
  builds a temporary `MyUniquePtr<int>` from `a`, which takes ownership
  and deletes it at the end of `process()`. `explicit` disables this
  implicit conversion, forcing the transfer of ownership to be written
  out (`process(MyUniquePtr<int>(a));`), so it's visible in the code.
- **Why copy is forbidden and only move is allowed**: this is the whole
  point of `unique_ptr`: a resource should have exactly one owner at a
  time. Allowing copies would mean two `MyUniquePtr` could point to the
  same object, and both would try to delete it when destroyed (double
  free). Move is allowed because it transfers ownership instead of
  duplicating it: only one of the two objects ends up owning the
  resource, the other is left empty.
- **`operator*` and `operator->`**: these let a `MyUniquePtr<Point>`
  behave like a raw pointer, so both `(*ptr).getX()` and `ptr->getX()`
  work as expected.
- **`make_unique`**: implemented with a *variadic template*
  (`template <typename T, typename... Args>`), which accepts any number
  of constructor arguments and forwards them to `new T(...)` via
  `std::forward`. This avoids writing `new` explicitly at the call site.