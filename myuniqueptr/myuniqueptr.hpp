#include <utility>

template <typename T>
class MyUniquePtr {
public:
    // Takes ownership of a raw pointer (or none, defaults to `nullptr`). `explicit` forbids implicit conversion from `T*`, see README.md.
    explicit MyUniquePtr(T* ptr = nullptr): m_ptr(ptr) { }

    // Copy constructor forbidden.
    MyUniquePtr(const MyUniquePtr& other) = delete;
    
    /* Move constructor.
    Steals the `other`'s pointer, leaves `other` empty.
    */
    MyUniquePtr(MyUniquePtr&& other) noexcept {

        m_ptr = other.m_ptr;
        other.m_ptr = nullptr;
    }

    // Copy assignment forbidden.
    MyUniquePtr<T>& operator=(const MyUniquePtr<T>& other) = delete;

    /* Move assignment.
    Releases its own resource first, then steals `other`'s pointer.
    Self-assignment safe.
    */
    MyUniquePtr<T>& operator=(MyUniquePtr<T>&& other) noexcept{
        if (this == &other) {
            return *this;
        }
        delete m_ptr;
        m_ptr = other.m_ptr;
        other.m_ptr = nullptr;
        return *this;
    }

    // Destructor. Deletes the owned object.
    ~MyUniquePtr() {
        delete m_ptr;
        m_ptr = nullptr;
    }

    // Dereference. Access the pointed-to object.
    T& operator*() const {
        return *m_ptr;
    }

    // Member access. Behaves like a raw pointer.
    T* operator->() const {
        return m_ptr;
    }
    
    // Enables `if (ptr)` checks. Marked explicit to forbid implicit conversion.
    explicit operator bool() const {
        return m_ptr != nullptr;
    }

    // Returns the raw pointer without releasing ownership.
    T* get() const {
        return m_ptr;
    }

    // Deletes the currently owned object and sets the pointer to `nullptr`.
    void reset(T* ptr = nullptr) {
        delete m_ptr;
        m_ptr = ptr;
    }

    // Remove the currently owned object and returns it.
    T* release() {
        T* old = m_ptr;
        m_ptr = nullptr;
        return old;
    }

private:
    T* m_ptr;

};


template <typename T, typename... Args>
MyUniquePtr<T> make_unique(Args&&... args) {
    return MyUniquePtr<T>(new T(std::forward<Args>(args)...));
}

