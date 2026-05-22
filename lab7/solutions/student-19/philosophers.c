// подключение библиотеки POSIX Threads для создания потоков, работы с мьютексами, синхронизации
#include <pthread.h>  
#include <stdio.h> // вывод в консоль
#include <unistd.h> // для функции usleep() чтобы делать паузу (чтобы потоки не работали слишком быстро)

#define PHILO_COUNT 5 // количество философов = количество потоков = количество вилок
#define CYCLES 3 // сколько раз филосов будет думать и есть
 
typedef enum e_state // перечисление состояний философа, хранит одно значение из набора
{
    THINKING,
    HUNGRY,
    EATING
}   t_state;

typedef enum e_philo_flags // флаговый enum, хранит несколько флагов одновременно
{
    PHILO_NONE      = 0, // нет вилок
    PHILO_HAS_LEFT  = 1 << 0, // левая вилка
    PHILO_HAS_RIGHT = 1 << 1, // правая вилка 
}   t_philo_flags;

typedef struct s_philosopher
{
    int             id; // номер философа
    t_state         state; // состояние
    t_philo_flags   flags; // какие вилки держит философ
    pthread_t       thread; // поток
    pthread_mutex_t *left_fork; // указатель на левую вилку
    pthread_mutex_t *right_fork; // указатель на правую вилку
}   t_philosopher;

static void print_state(t_philosopher *philo, const char *msg) // const char *msg указатель на массив символов
{
    printf("Philosopher %d %s\n", philo->id, msg);
}

void think(t_philosopher *philo)
{
    philo->state = THINKING; // философ думает
    print_state(philo, "is THINKING");
    usleep(200000); // пауза 0,2 с
}

void take_forks(t_philosopher *philo) // взятие вилок (захват мьютексов)
{

    if (philo->id % 2 == 0)  // для избежания deadlock
    {
        pthread_mutex_lock(philo->left_fork); // блокирует мьютекс, если он уже занят, поток ждёт освобождения
        // |= добавляет флаг
        philo->flags |= PHILO_HAS_LEFT; // установка флага
        print_state(philo, "took left fork");

        pthread_mutex_lock(philo->right_fork); 
        philo->flags |= PHILO_HAS_RIGHT;
        print_state(philo, "took right fork");
    }
    else
    {
        pthread_mutex_lock(philo->right_fork);
        philo->flags |= PHILO_HAS_RIGHT;
        print_state(philo, "took right fork");

        pthread_mutex_lock(philo->left_fork);
        philo->flags |= PHILO_HAS_LEFT;
        print_state(philo, "took left fork");
    }
}

void put_forks(t_philosopher *philo)
{
    if (philo->flags & PHILO_HAS_LEFT) // есть ли у философа левая вилка (по флагу)
    {
        pthread_mutex_unlock(philo->left_fork); // освобождает левую вилку (разблокирует mutex)
        philo->flags &= ~PHILO_HAS_LEFT; // удаление флага левая вилка у философа
    }

    if (philo->flags & PHILO_HAS_RIGHT) // есть ли у философа правая вилка (по флагу)
    {
        pthread_mutex_unlock(philo->right_fork); // освобождает правую вилку (разблокирует мьютекс)
        philo->flags &= ~PHILO_HAS_RIGHT; // удаление флага правая вилка у философа
    }

    print_state(philo, "put forks"); // философ положил вилки
}

void eat(t_philosopher *philo)
{
    philo->state = EATING; // состояние "философ ест"
    print_state(philo, "is EATING");
    usleep(200000); // пауза 0,2 с
}

void *philosopher_routine(void *arg) // выполняет каждый поток-философ
{
    t_philosopher *philo; // указатель на структуру философа
    int             i; // сколько раз философ уже поел

    philo = (t_philosopher *)arg; // указатель на  конкретного философа
    i = 0; // философ не выполнил ни одног цикла
    while (i < CYCLES) // цикл жизни философа, действия повторяются CYCLES раз
    {
        think(philo);

        philo->state = HUNGRY;
        print_state(philo, "is HUNGRY");

        take_forks(philo);
        eat(philo);
        put_forks(philo);

        i++;
    }
    return (NULL); // завершение потока
}

int main(void)
{
    pthread_mutex_t forks[PHILO_COUNT]; // массив мьютексов
    t_philosopher   philosophers[PHILO_COUNT]; // массив философов
    int             i;

    i = 0;
    while (i < PHILO_COUNT) // цикл поо всем вилкам
    {
        pthread_mutex_init(&forks[i], NULL); // &forks[i] - адрес вилки
        i++;
    }

    i = 0; // сброс счетчика

    while (i < PHILO_COUNT) //  цикл по всем философам
    {
        philosophers[i].id = i + 1;
        philosophers[i].state = THINKING; // начальное состояние философов
        philosophers[i].flags = PHILO_NONE; // философ не держит вилки

        philosophers[i].left_fork = &forks[i]; // левая вилка — текущий индекс
        philosophers[i].right_fork = &forks[(i + 1) % PHILO_COUNT]; // правая вилка — следующая по кругу (последний берёт первую)

        pthread_create(
            &philosophers[i].thread, // сохранение thread id
            NULL,
            philosopher_routine, // функция, которую будет выполнять поток
            &philosophers[i] // данные философа
        );

        i++; // следующий философ
    }

    i = 0; // сброс 
    while (i < PHILO_COUNT) // проход по потокам
    {
        pthread_join(philosophers[i].thread, NULL); // поток ждет пока философ закончит работу
        i++; // следующий поток
    }

    i = 0; // сброс 
    while (i < PHILO_COUNT) // проход по вилкам
    {
        pthread_mutex_destroy(&forks[i]); // освобождение ресурсов ос
        i++; // следующая вилка
    }

    return (0);
}