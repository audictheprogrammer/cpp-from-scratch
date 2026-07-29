#include "myuniqueptr.hpp"
#include <iostream>
#include <string>

// Simple utility class to test operator-> and operator*.
class Point {
public:
    Point(int x, int y) : m_x(x), m_y(y) {
        std::cout << "Point(" << m_x << ", " << m_y << ") constructed" << std::endl;
    }
    ~Point() {
        std::cout << "Point(" << m_x << ", " << m_y << ") destroyed" << std::endl;
    }
    int getX() const { return m_x; }
    int getY() const { return m_y; }

private:
    int m_x, m_y;
};

// Utility function to force a move (return by value).
MyUniquePtr<Point> makePoint(int x, int y) {
    MyUniquePtr<Point> p(new Point(x, y));
    return p; // move constructor expected here (or RVO)
}

int main() {
    std::cout << "=== 1. Construction + operator*, operator->, get() ===" << std::endl;
    MyUniquePtr<Point> a(new Point(3, 4));
    std::cout << "a->getX(): " << a->getX() << std::endl;
    std::cout << "(*a).getY(): " << (*a).getY() << std::endl;
    std::cout << "a.get() is not null: " << std::boolalpha << (a.get() != nullptr) << std::endl;

    std::cout << "\n=== 2. Default constructor (empty pointer) ===" << std::endl;
    MyUniquePtr<Point> empty_ptr;
    std::cout << "empty_ptr.get() is null: " << (empty_ptr.get() == nullptr) << std::endl;

    std::cout << "\n=== 3. operator bool ===" << std::endl;
    std::cout << "a is truthy: " << static_cast<bool>(a) << std::endl;
    std::cout << "empty_ptr is truthy: " << static_cast<bool>(empty_ptr) << std::endl;
    if (a) {
        std::cout << "if (a) works as expected" << std::endl;
    }

    std::cout << "\n=== 4. Move constructor (via function return) ===" << std::endl;
    MyUniquePtr<Point> b = makePoint(1, 2);
    std::cout << "b->getX(): " << b->getX() << std::endl;

    std::cout << "\n=== 5. Move assignment ===" << std::endl;
    MyUniquePtr<Point> c(new Point(9, 9));
    c = makePoint(7, 8); // Point(9,9) should be destroyed here, before Point(7,8) is owned
    std::cout << "c->getX(): " << c->getX() << std::endl;

    std::cout << "\n=== 6. Self-assignment (move) ===" << std::endl;
    c = std::move(c); // forces operator=(&&) on itself
    std::cout << "c after self-move, still valid, c->getX(): " << c->getX() << std::endl;

    std::cout << "\n=== 7. reset() ===" << std::endl;
    MyUniquePtr<Point> d(new Point(5, 5));
    std::cout << "Before reset, d.get() is not null: " << (d.get() != nullptr) << std::endl;
    d.reset(); // deletes Point(5,5)
    std::cout << "After reset, d.get() is null: " << (d.get() == nullptr) << std::endl;
    d.reset(new Point(6, 6)); // takes ownership of a new object
    std::cout << "After reset(new Point), d->getX(): " << d->getX() << std::endl;

    std::cout << "\n=== 8. release() ===" << std::endl;
    Point* raw = d.release();
    std::cout << "d.get() is null after release: " << (d.get() == nullptr) << std::endl;
    std::cout << "raw->getX(): " << raw->getX() << std::endl;
    delete raw; // release() does not delete, caller is responsible

    std::cout << "\n=== 9. make_unique ===" << std::endl;
    auto e = make_unique<Point>(10, 20);
    std::cout << "e->getX(): " << e->getX() << ", e->getY(): " << e->getY() << std::endl;

    std::cout << "\n=== 10. Copy is forbidden (compile-time check) ===" << std::endl;
    std::cout << "Uncomment the lines below to verify they fail to compile:" << std::endl;
    // MyUniquePtr<Point> f = a;        // ERROR: copy constructor is deleted
    // MyUniquePtr<Point> g;
    // g = a;                           // ERROR: copy assignment is deleted

    std::cout << "\nAll tests passed if no crash occurred. Remaining objects will now "
                 "be destroyed as they go out of scope." << std::endl;
    return 0;
}