## Архитектура проекта

### Структура проекта

Проект организован следующим образом:

```
project_root/
│
├── CMakeLists.txt
│
├── src/
│   ├── main.c
│   ├── compress.c
│   └── decompress.c
│
├── include/
│   ├── compress.h
│   └── decompress.h
│
├── common/
│   ├── common.c
│   └── common.h
│
└── build/
```

### Описание файлов

- **`CMakeLists.txt`**: Основной файл конфигурации для сборки проекта с использованием CMake. Определяет параметры сборки, такие как исходные файлы, директории для поиска заголовков и параметры компиляции.

- **`src/main.c`**: Основной исходный файл, содержащий функцию `main()`, которая управляет интерфейсом командной строки и вызывает функции сжатия и декомпрессии.

- **`src/compress.c`**: Реализация функций для сжатия данных. Включает функции `Compress1`, `Compress2` и `Analysis`.

- **`src/decompress.c`**: Реализация функций для декомпрессии данных. Включает функции `Decompress1`, `Decompress2` и `Output`.

- **`src/common.c`**: Реализация вспомогательных функций и структур данных, таких как работа с памятью и операции над словарем. Включает функции `Find`, `CreateMask`, `InitializeDictionary`, `FreeDictionary`, `SafeMalloc`, `SafeCalloc` и `SafeRealloc`.

- **`include/compress.h`**: Заголовочный файл для функций сжатия. Определяет интерфейсы функций `Compress1`, `Compress2` и `Analysis`.

- **`include/decompress.h`**: Заголовочный файл для функций декомпрессии. Определяет интерфейсы функций `Decompress1`, `Decompress2` и `Output`.

- **`include/common.h`**: Заголовочный файл для общих функций и структур данных. Определяет структуру `Dictionary`, а также функции для работы с памятью и словарем.

- **`build/`**: Каталог, в котором CMake создает временные файлы и конечный исполняемый файл проекта.

### CMakeLists.txt

Файл `CMakeLists.txt` настраивает сборку проекта следующим образом:

```cmake
cmake_minimum_required(VERSION 3.10)

# Определяем проект и версию
project(CompressionProject C)

# Устанавливаем стандарт C
set(CMAKE_C_STANDARD 11)

# Указываем директории для поиска заголовочных файлов
include_directories(include common)

# Добавляем исполняемый файл
add_executable(CompressionProject
        src/main.c
        src/compress.c
        src/decompress.c
        common/common.c
)
```
