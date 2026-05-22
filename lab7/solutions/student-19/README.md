# Лабораторная работа №7: Многопоточность в C. Задача «обедающие философы»

## Описание

## Задача «обедающие философы»

Задача «обедающие философы» — это классическая задача синхронизации, предложенная Эдсгером Дейкстрой для демонстрации проблем совместного доступа к ресурсам и взаимных блокировок.

За круглым столом сидят 5 философов. Между каждыми двумя соседними философами лежит одна вилка, всего вилок тоже 5. Перед каждым философом стоит тарелка с блюдом, которое можно есть **только двумя вилками одновременно**. Поэтому для еды философ должен взять вилку слева от себя и вилку справа от себя.

Каждый философ циклически повторяет одно и то же поведение:

- думает;
- испытывает голод;
- пытается взять две вилки;
- ест;
- кладёт вилки обратно и снова начинает думать.

Проблема состоит в том, что вилки — общий ресурс. Если все философы одновременно возьмут по одной вилке и будут ждать вторую, возникнет взаимная блокировка (`deadlock`) — программа зависнет. Поэтому задача состоит не просто в создании потоков, а в корректной синхронизации доступа к вилкам так, чтобы философы могли есть и программа продолжала работать без deadlock.

---

## Структура проекта

```
lab7/
├── solutions/
│   └── student-19/
│       ├── philosophers.c
│       ├── README.md
│       └── philosophers.exe
├── README.md
└── check-list.md
```

## Сборка и запуск

```bash
cd lab7/solutions/student-19
```

```
gcc philosophers.c -Wall -Wextra -Werror -pthread -o philosophers

```

```
./philosophers
```

## Пример вывода програмы:
```
Philosopher 1 is THINKING
Philosopher 2 is THINKING
Philosopher 4 is THINKING
Philosopher 5 is THINKING
Philosopher 3 is THINKING
Philosopher 3 is HUNGRY
Philosopher 3 took right fork
Philosopher 3 took left fork
Philosopher 3 is EATING
Philosopher 1 is HUNGRY
Philosopher 1 took right fork
Philosopher 1 took left fork
Philosopher 1 is EATING
Philosopher 5 is HUNGRY
Philosopher 4 is HUNGRY
Philosopher 2 is HUNGRY
Philosopher 5 took right fork
Philosopher 5 took left fork
Philosopher 5 is EATING
Philosopher 4 took left fork
Philosopher 3 put forks
Philosopher 3 is THINKING
Philosopher 1 put forks
Philosopher 1 is THINKING
Philosopher 2 took left fork
Philosopher 2 took right fork
Philosopher 2 is EATING
Philosopher 5 put forks
Philosopher 5 is THINKING
Philosopher 4 took right fork
Philosopher 4 is EATING
Philosopher 2 put forks
Philosopher 2 is THINKING
Philosopher 3 is HUNGRY
Philosopher 1 is HUNGRY
Philosopher 1 took right fork
Philosopher 1 took left fork
Philosopher 1 is EATING
Philosopher 5 is HUNGRY
Philosopher 4 put forks
Philosopher 4 is THINKING
Philosopher 3 took right fork
Philosopher 3 took left fork
Philosopher 3 is EATING
Philosopher 5 took right fork
Philosopher 1 put forks
Philosopher 1 is THINKING
Philosopher 5 took left fork
Philosopher 5 is EATING
Philosopher 2 is HUNGRY
Philosopher 2 took left fork
Philosopher 2 took right fork
Philosopher 2 is EATING
Philosopher 4 is HUNGRY
Philosopher 4 took left fork
Philosopher 3 put forks
Philosopher 3 is THINKING
Philosopher 4 took right fork
Philosopher 4 is EATING
Philosopher 1 is HUNGRY
Philosopher 5 put forks
Philosopher 5 is THINKING
Philosopher 3 is HUNGRY
Philosopher 1 took right fork
Philosopher 1 took left fork
Philosopher 1 is EATING
Philosopher 2 put forks
Philosopher 2 is THINKING
Philosopher 5 is HUNGRY
Philosopher 3 took right fork
Philosopher 3 took left fork
Philosopher 3 is EATING
Philosopher 4 put forks
Philosopher 4 is THINKING
Philosopher 5 took right fork
Philosopher 5 took left fork
Philosopher 5 is EATING
Philosopher 1 put forks
Philosopher 2 is HUNGRY
Philosopher 2 took left fork
Philosopher 2 took right fork
Philosopher 2 is EATING
Philosopher 4 is HUNGRY
Philosopher 4 took left fork
Philosopher 3 put forks
Philosopher 4 took right fork
Philosopher 4 is EATING
Philosopher 5 put forks
Philosopher 2 put forks
Philosopher 4 put forks
```

## Требования

- [+] Программа успешно компилируется командой gcc philosophers.c -Wall -Wextra -Werror -pthread -o philosophers
- [+] Используются потоки pthread_create и pthread_join
- [+] Используются 5 мьютексов для реализации вилок
- [+] Реализован обычный enum для состояния философа
- [+] Реализован флаговый enum для отслеживания вилок
- [+] Корректно реализованы состояния THINKING, HUNGRY, EATING
- [+] Флаги вилок корректно устанавливаются и сбрасываются при взятии и освобождении
- [+] Реализован механизм предотвращения deadlock
- [+] Отсутствуют глобальные переменные
- [+] Все потоки корректно завершаются через pthread_join
- [+] Все мьютексы корректно уничтожаются после завершения программы

## Критерии оценивания
- [+] Используются потоки (pthread)
- [+] Используются мьютексы для синхронизации доступа к вилкам
- [+] Используются два enum (обычный и флаговый)
- [+] Реализована корректная многопоточная работа
- [+] Отсутствует deadlock
- [+] Потоки корректно завершаются через pthread_join
- [+] Все ресурсы освобождаются

## Штрафы
- [-] Отсутствие обычного enum
- [-] Отсутствие флагового enum
- [-] Отсутствие мьютексов или неправильная синхронизация
- [-] Deadlock или зависание программы
- [-] Использование глобальных переменных
- [-] Ошибки компиляции с -Wall -Wextra -Werror -pthread
- [-] Утечки ресурсов или незакрытые мьютексы
