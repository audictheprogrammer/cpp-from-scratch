#include "myvector.hpp"
#include <iostream>
#include <string>

// Utility function to force a move (return by value).
MyVector<int> makeVector(int a, int b, int c) {
    MyVector<int> temp;
    temp.push_back(a);
    temp.push_back(b);
    temp.push_back(c);
    return temp; // move constructor expected here (or RVO)
}

void printVector(const std::string& label, MyVector<int>& v) {
    std::cout << label << " (size=" << v.size() << "): ";
    for (size_t i = 0; i < v.size(); i++) {
        std::cout << v[i] << " ";
    }
    std::cout << std::endl;
}

// Takes a const reference to test const operator[] and const iterators.
void printConstVector(const std::string& label, const MyVector<int>& v) {
    std::cout << label << " (size=" << v.size() << "): ";
    for (const auto& x : v) {
        std::cout << x << " ";
    }
    std::cout << std::endl;
}

int main() {
    std::cout << "=== 1. Default constructor + push_back ===" << std::endl;
    MyVector<int> a;
    a.push_back(1);
    a.push_back(2);
    a.push_back(3);
    printVector("a", a);

    std::cout << "\n=== 2. Growth beyond initial capacity ===" << std::endl;
    for (int i = 4; i <= 10; i++) {
        a.push_back(i);
    }
    printVector("a after 10 push_back", a);
    std::cout << "a capacity: " << a.capacity() << std::endl;

    std::cout << "\n=== 3. Copy constructor ===" << std::endl;
    MyVector<int> b = a; // copy constructor
    a.push_back(999);
    printVector("a (modified after copy)", a);
    printVector("b (copy, should stay independent)", b);

    std::cout << "\n=== 4. Copy assignment operator ===" << std::endl;
    MyVector<int> c;
    c.push_back(42);
    c = a; // copy assignment
    printVector("c (after c = a)", c);
    a.push_back(1234);
    printVector("a (modified after)", a);
    printVector("c (should stay independent)", c);

    std::cout << "\n=== 5. Self-assignment (copy) ===" << std::endl;
    c = c;
    printVector("c after c = c", c);

    std::cout << "\n=== 6. Move constructor (via function return) ===" << std::endl;
    MyVector<int> d = makeVector(100, 200, 300);
    printVector("d (built by move)", d);

    std::cout << "\n=== 7. Move assignment ===" << std::endl;
    MyVector<int> e;
    e.push_back(-1);
    e = makeVector(7, 8, 9); // move assignment
    printVector("e (after move assignment)", e);

    std::cout << "\n=== 8. at() out of bounds (exception handled) ===" << std::endl;
    try {
        std::cout << e.at(100) << std::endl;
    } catch (const std::out_of_range& ex) {
        std::cout << "Exception caught: " << ex.what() << std::endl;
    }

    std::cout << "\n=== 9. Empty vector (edge case) ===" << std::endl;
    MyVector<int> empty_vec;
    std::cout << "empty_vec.empty(): " << std::boolalpha << empty_vec.empty() << std::endl;
    std::cout << "empty size: " << empty_vec.size() << std::endl;
    try {
        std::cout << empty_vec.at(0) << std::endl;
    } catch (const std::out_of_range& ex) {
        std::cout << "Exception caught: " << ex.what() << std::endl;
    }

    std::cout << "\n=== 10. Copy/assignment of an empty vector ===" << std::endl;
    MyVector<int> f = empty_vec; // copy constructor on an empty vector
    MyVector<int> g;
    g.push_back(1);
    g = empty_vec; // copy assignment with an empty vector
    std::cout << "f size: " << f.size() << ", g size: " << g.size() << std::endl;

    std::cout << "\n=== 11. pop_back ===" << std::endl;
    e = makeVector(7, 8, 9); // move assignment
    printVector("e before pop_back", e);
    e.pop_back();
    printVector("e after pop_back (last element removed)", e);

    std::cout << "\n=== 12. Empty pop_back (precondition, not exception) ===" << std::endl;
    MyVector<int> empty_pop;
    std::cout << "empty_pop.size() before pop_back attempt: " << empty_pop.size()
              << " (skipping actual call: pop_back() on empty vector is UB by design, like std::vector)"
              << std::endl;

    std::cout << "\n=== 13. Iterators (non-const) ===" << std::endl;
    MyVector<int> h;
    h.push_back(1);
    h.push_back(2);
    h.push_back(3);
    printVector("h (via printVector)", h);
    std::cout << "h (via iterator): ";
    for (auto& x : h) {
        std::cout << x << " ";
    }
    std::cout << std::endl;

    std::cout << "\n=== 14. Const iterators + const operator[] ===" << std::endl;
    printConstVector("h (via const iterator)", h);

    std::cout << "\n=== 15. reserve() ===" << std::endl;
    MyVector<int> r;
    r.reserve(50);
    std::cout << "r.size() after reserve(50): " << r.size()
              << ", r.capacity(): " << r.capacity() << std::endl;
    r.push_back(1);
    r.push_back(2);
    std::cout << "r.capacity() should stay 50 (no reallocation triggered): "
              << r.capacity() << std::endl;

    std::cout << "\nAll tests passed if no crash occurred." << std::endl;
    return 0;

    /*
    $ valgrind --leak-check=full ./test
    (expected: 0 errors, all heap blocks freed)
    */
}