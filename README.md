# Cmptool

## Development

Build application: 

```
make all # for build cmptool
./build/cmptool -h 

make tests # for run tests
```


## Схема работы приложения:
1) Отrкрытие файла;
2) Прохождение по файлу;
3) Закрытие файла;
4) Создание нового файла;
5) *Показывать насколько сжался файл*; 

Синтаксис: наимнеовапние утилиты, каккой файл создается, какой пкуть до файла котрый надо архивировать. 

## План

- прописать инструкцию по сборке
- приложение получает на вход 2 аргумента `cpmtool ./text.txt ./text.cmp`. 1 - название файла который будем сжимать, 2 - название файла архива. + одельная функция для сжатия которая ничего не делает
- добавить тесты. 
- доработка функции сжатия
- добавление распаковки архива -u (`cmptool -u ./text.cmp ./text2.txt`)
- настроить CI на базе github
- сделать поддержку директорий, при сжатии первый аргумент может быть директория *
