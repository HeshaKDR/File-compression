# Схема работы приложения:
1) Отrкрытие файла;
2) Прохождение по файлу;
3) Закрытие файла;
4) Создание нового файла;
5) *Показывать насколько сжался файл*; 

Синтаксис: наимнеовапние утилиты, каккой файл создается, какой пкуть до файла котрый надо архивировать. 

# План

- прописать инструкцию по сборке
- приложение получает на вход 2 аргумента `cpmtool ./text.txt ./text.cmp`. 1 - название файла который будем сжимать, 2 - название файла архива. + одельная функция для сжатия которая ничего не делает
- добавить тесты. 
- доработка функции сжатия
- добавление распаковки архива -u (`cmptool -u ./text.cmp ./text2.txt`)
- настроить CI на базе github
- сделать поддержку директорий, при сжатии первый аргумент может быть директория *


# Инструкция по сборке

тут будет набор команд которые надо выполнить, что бы получить исполняемый файл под linux

- to check for clang-format: ```clang-format -n file_name.c```
- to put file to clang-format: ```clang-format -i file_name.c```
- to compile an object file: ```gcc -Wall -Werror -Wextra file_name(s).c -c program_name```
- to compile an executable file: ```gcc -Wall -Werror -Wextra file_name(s).o -o program_name```
- to check for cpp: ```cppcheck file_name.c```
- to check for leaks: ```leaks -atExit -- ./file_name.exe```

