# C++ Code Style Guide

This style guide defines conventions used in this repository for C++20 development with Qt 6 and Eigen. It emphasizes functional purity, immutability, composability, and clarity.

---

## 1. General Principles

- Favor **immutability** and **pure functions**.
- Prefer **expressions** over statements.
- Avoid **side effects** wherever possible.
- Use **C++20 features**: `ranges`, `concepts`, `constexpr`, `modules`.
- Use **Doxygen** format for public APIs.
- All numerical output must be printed with fixed-point precision (default: `4` digits).

---

## 2. Naming Conventions

| Element            | Style                                    | Example                        |
| ------------------ | ---------------------------------------- | ------------------------------ |
| Classes            | `PascalCase`                             | `ImageFilter`, `SeedSelector`  |
| Functions          | `snake_case`                             | `compute_laplacian`, `solve()` |
| Function Variables | `snake_case`                             | `normalize`, `create_mask`     |
| Constants          | `kPascalCase`                            | `kMaxIterations`               |
| Regular Variables  | `snake_case`                             | `label_matrix`, `input_image`  |
| Namespaces         | `snake_case`                             | `math_utils`, `label_ops`      |
| Template Types     | `PascalCase`                             | `Transformer`, `Mapper`        |
| Files              | Based on main class or namespace in file | `global.hpp`, `segmenter.cpp`  |
| Class Members      | `snake_case`, suffix `_` if mutable      | `buffer_`, `width`             |

---

## 3. Functions

- Functions must be **pure** by default.
- Use the `_pure_` macro::
  ```cpp
  _pure_(int add(int a, int b)) { return a + b; }
  ```
- Functional objects and lambdas are encouraged. Use the `Pure<>` wrapper for stateless lambdas:
  ```cpp
  const Pure<int(int)> square = [](int x) { return x * x; };
  ```

- Use `concepts` to constrain templates:
  ```cpp
  template <typename T>
  concept Arithmetic = std::is_arithmetic_v<T>;

  template <Arithmetic T>
  constexpr T double_value(T value) { return value * 2; }
  ```

---

## 4. Variables

- Use `const` by default.
- Avoid mutations; prefer value transformations.
- Use `auto` when the type is obvious or irrelevant semantically.

---

## 5. Composition and Ranges

Use `std::ranges` for functional-style transformations:

```cpp
auto result = input
            | std::views::filter(is_valid)
            | std::views::transform(normalize);
```

Compose custom utilities where needed:

```cpp
template <std::ranges::range R, typename Pred>
auto filter(R&& r, Pred&& pred) {
    return r | std::views::filter(std::forward<Pred>(pred));
}
```

---

## 6. Qt Integration

- Use `QStringView` instead of `QString` where possible.
- Keep Qt objects out of pure logic.
- Prefer `std::vector` over `QVector` unless Qt requires otherwise.
- Use signals/slots declaratively, avoid heavy logic inside slots.

---

## 7. Eigen Integration

- Treat Eigen expressions as lazy and pure.
- Avoid in-place modifications; return new expressions.
- Use `Map<const MatrixXd>` for efficient reference without copy.
- Combine with `std::ranges`:

```cpp
std::ranges::for_each(std::views::iota(0, vec.size()), [&](int i) {
    result[i] = transform(vec[i]);
});
```

---

## 8. Error Handling

- Use `std::optional`, `std::variant`, or `std::expected`.
- Avoid exceptions for control flow.

---

## 9. Meta Annotations

Use custom attributes for purity and intention:

```cpp
#define _pure_(decl)       [[nodiscard]] static constexpr decl noexcept
#define _impure_(decl)     [[nodiscard]] static decl noexcept
#define _no_copy_(Type)    Type(const Type&) = delete
#define _dbg_(msg)         do { std::cerr << "[DBG] " << msg << '\n'; } while (0)
// ...
```

---

## 10. Directory Structure

- `global.hpp` – general-purpose definitions and utilities.
- `MetaAnnotations.hpp` – portability macros and attribute aliases.
- Domain-specific files should reflect their core purpose.

---

## 11. Formatting

- Indent with **4 spaces**.
- Opening braces on the **same line**.
- Use **leading commas** in enums and initializer lists, especially in multiline format.

**Example:**

```cpp
enum class Mode {
    Fast
    , Safe
    , Debug
};
```

- Group includes in the order:
    
    - related header (`.hpp`),
        
    - standard headers,
        
    - 3rd-party libraries (e.g. Eigen, Qt),
        
    - project headers.
        
- Indent with **4 spaces**.
    
- Opening braces on the **same line**.
    
- Use **leading commas** in enums and initializer lists, especially in multiline format.
    
- Group includes in the order:
    
    - related header (`.hpp`),
        
    - standard headers,
        
    - 3rd-party libraries (e.g. Eigen, Qt),
        
    - project headers.
---

## 12. Precision and Output

- Use `std::ostringstream` with `std::fixed` and `std::setprecision(PRECISION)` for all numeric formatting.
- Default precision is defined as:

```cpp
constexpr auto PRECISION = 4;
```

---

## Example

```cpp
_pure_(std::string toString(const Eigen::MatrixBase<T>& obj, int precision = PRECISION)) {
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(precision);
    oss << obj;
    return oss.str();
}
```

---

