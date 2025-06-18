# RandomWalker

**RandomWalker** — это десктопное приложение на C++ с использованием **Qt 6.8.1**, **Eigen** и системой сборки **CMake + Conan**. Проект автоматически подтягивает зависимости и может быть собран на любой машине с минимальной подготовкой.

---

## Требования

| Компонент                        | Версия / Уточнение                 |
| -------------------------------- | ---------------------------------- |
| [CMake](https://cmake.org/)      | 3.21 или выше                      |
| [Conan](https://conan.io/)       | 2.0 или выше (`pip install conan`) |
| [Qt](https://www.qt.io/download) | 6.8.1 (локальная установка)        |
| Компилятор                       | MSVC 2022 (Visual Studio 17)       |
| Python                           | 3.8 или выше (требуется для Conan) |

---

## Установка зависимостей (однократно)

1. Установите **Conan**:
```
pip install conan
```
2. Сгенерируйте профили сборки:
```
conan profile detect --name=msvc-debug
conan profile detect --name=msvc-release
```
3. Убедитесь, что переменная среды `CMAKE_PREFIX_PATH` указывает на путь установки Qt:
```
set CMAKE_PREFIX_PATH=C:\Qt\6.8.1\msvc2022_64
```