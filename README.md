# C++ From Scratch

Reimplementing core C++ standard library components and mechanisms from
scratch, in order to deeply understand what happens under the hood — memory
management, move semantics, concurrency, templates.

Each project is self-contained, includes its own tests and README, and has
been verified leak-free with `valgrind`.

## Projects

| Project | Reimplements | Key concepts |
|---|---|---|
| [`myvector/`](./myvector) | `std::vector<T>` | Rule of five, move semantics, iterators, exception safety |
| [`myuniqueptr/`](./myuniqueptr) | `std::unique_ptr<T>` | RAII, deleted copy, move-only ownership, `explicit` |

More projects to come.

## Build & test

Each project can be compiled independently, e.g.:

```bash
cd myvector
g++ -std=c++17 -g myvector.cpp -o test
./test
```

To check for memory leaks:

```bash
valgrind --leak-check=full ./test
```

## License

MIT (or whatever you'd like — feel free to browse and reuse).