# BrickGame Тетрис

## Описание проекта

BrickGame Тетрис - это реализация классической игры Тетрис на языке программирования C. Проект разработан с использованием структурного подхода и состоит из двух основных частей:

1. Библиотека, реализующая логику игры;
2. Терминальный интерфейс, разработанный с использованием библиотеки `ncurses`.

## Особенности

- Полная реализация классического геймплея Тетрис;
- Использование `ncurses` для создания интерактивного терминального интерфейса;
- Система подсчета очков и уровней;
- Отображение следующей фигуры;
- Сохранение и загрузка рекордов;
- Полное покрытие unit-тестами с использованием библиотеки `Check`.

## Требования

- `gcc` компилятор;
- Библиотека `ncurses`;
- Библиотека `Check` (для запуска тестов).

## Сборка и запуск

Для сборки проекта используется Makefile со следующими целями:

- `make all`: Собрать проект;
- `make install`: Установить игру;
- `make uninstall`: Удалить установленную игру;
- `make clean`: Очистить файлы сборки;
- `make dvi`: Сгенерировать документацию;
- `make dist`: Создать архив с исходным кодом;
- `make test`: Запустить unit-тесты;
- `make gcov_report`: Сгенерировать отчет о покрытии кода тестами.

Для запуска игры после сборки выполните:

```
./build/tetris
```

## Лицензия

Этот проект является частью образовательной программы [School21](https://21-school.ru/) - образовательный проект от Сбер для разработчиков