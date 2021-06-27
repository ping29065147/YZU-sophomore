#include <iostream>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <ctime>
#include <queue>
#include <signal.h>
#include <string.h>

using namespace std;

void ext_mode_init_timer(void);
int ext_mode_timer_start(int unit_id, int delay_second);
int ext_mode_timer_stop(int unit_id);
#define MAX_UNITS 1
int TS;

struct ext_mode_timer_t
{
	timer_t timer_id;
	int is_occupied;
};
static struct ext_mode_timer_t ext_mode_timer[MAX_UNITS];

void timer_handler(int sig, siginfo_t *si, void *uc)
{
    TS += 10;
    //printf("TS=%d\n", TS);
}

void ext_mode_timer_delete(int unit_id)
{
	timer_delete(ext_mode_timer[unit_id].timer_id);
}

static int ext_mode_timer_create(int unit_id)
{
	struct sigaction sa;
	struct sigevent sev;

	memset (&sa, '\0', sizeof(sa));
	sa.sa_flags = SA_SIGINFO;
	sa.sa_sigaction = timer_handler;
	sigemptyset(&sa.sa_mask);
	if (sigaction(SIGRTMIN, &sa, NULL) == -1)
	{
		printf("%s: sigaction error\n", __func__);
		return -1;
	}
	memset (&sev, '\0', sizeof(sev));
	sev.sigev_notify = SIGEV_SIGNAL;
	sev.sigev_signo = SIGRTMIN;
	sev.sigev_value.sival_ptr = &ext_mode_timer[unit_id].timer_id;
	if (timer_create(CLOCK_REALTIME, &sev, &ext_mode_timer[unit_id].timer_id) == -1)
	{
		printf("%s: err timer_create [%d] \n", __func__, unit_id);
		return -1;
	}
	ext_mode_timer[unit_id].is_occupied = 0;
	return 0;
}

// Stop the timer
int ext_mode_timer_stop(int unit_id)
{
	struct itimerspec value;
	value.it_value.tv_sec = 0;
	value.it_value.tv_nsec = 0;
	value.it_interval.tv_sec = 0;
	value.it_interval.tv_nsec = 0;

	if (timer_settime(ext_mode_timer[unit_id].timer_id, 0, &value, NULL) == -1)
	{
		printf("%s: err timer_settime [%d] \n", __func__, unit_id);
		return -1;
	}
	ext_mode_timer[unit_id].is_occupied = 0;
	return 0;
}

int ext_mode_timer_start(int unit_id, int delay_ms_second)
{

	struct ext_mode_timer_t *p = (struct ext_mode_timer_t *) &ext_mode_timer[unit_id];
	struct itimerspec value;

	if ((0 == delay_ms_second) || (0 != p->is_occupied))
		return 0;

	/* Start the timer */
	value.it_value.tv_sec = 0;
	value.it_value.tv_nsec = delay_ms_second*1000000;
	value.it_interval.tv_sec = 0;
	value.it_interval.tv_nsec = delay_ms_second*1000000;

	if (timer_settime(p->timer_id, 0, &value, NULL) == -1)
	{
		printf("%s: err timer_settime [%d] \n", __func__, unit_id);
		return -1;
	}
	p->is_occupied = 1;	// used
	return 0;
}

void ext_mode_init_timer(void)
{
	int unit_id;
	for (unit_id = 0; unit_id < MAX_UNITS; unit_id++)
		ext_mode_timer_create(unit_id);
}

int T = 0;
int N, K;
int pork_cnt = 0;
bool cut_end = false, pack_end = false;
bool no_cut, no_pack;

struct item
{
    int pork_num;
    bool is_cut;
    int Tm;
};
vector<item> storage;
vector<item> frez_pool;

sem_t sem1, sem2, sem3;
pthread_mutex_t mutex;

void SLEEP(int ns)
{
    usleep(ns * 1000);
}

int get_time(int start, int end)
{
    int range = (end - start) / 10 + 1;
    int t = ((rand() % range) + start / 10) * 10;
    return t;
}

void* pork(void *msg)
{
    while(pork_cnt < N)
    {
        int tmp = get_time(50, 100);
        SLEEP(tmp);

        pthread_mutex_lock(&mutex);
            if (storage.size() < K)
            {
                cout << TS << "ms - Pork#" << ++pork_cnt << " : waiting in the slot\n";
                storage.push_back( {pork_cnt, false, 0} );
                sem_post(&sem2);
            }
            else
            {
                int tmp2 = get_time(300, 500);
                cout << TS << "ms - Pork#" << ++pork_cnt << " has been sent to the Freezer (" << tmp2 << "ms)\n";
                frez_pool.push_back( {pork_cnt, false, TS + tmp2} );
                sem_post(&sem1);
            }
        pthread_mutex_unlock(&mutex);
    }
    
    pthread_exit(0);
}

void* cutter(void *msg) 
{
    int i;
    item cut;
    no_cut = true;
    sem_wait(&sem2);
    no_cut = false;

    while(1)
    {
        pthread_mutex_lock(&mutex);
            for (i = 0; i < K && i < storage.size(); ++i) if (storage[i].is_cut == false)
            {
                cut = storage[i];
                break;
            }
        pthread_mutex_unlock(&mutex);

        if (pork_cnt == N && i == storage.size() && frez_pool.empty()) break;
        else if (i == storage.size())
        {
            no_cut = true;
            sem_wait(&sem2);
            no_cut = false;
            continue;
        }

        pthread_mutex_lock(&mutex);
            storage.erase(storage.begin() + i);
            cout << TS << "ms - Pork#" << cut.pork_num << " : enters the CUTTER\n";
        pthread_mutex_unlock(&mutex);

        pthread_mutex_lock(&mutex);
            int tmp = get_time(100, 300);
            cout << TS << "ms - CUTTER: cutting... cutting... Pork#" << cut.pork_num << " (" << tmp << "ms)\n";
        pthread_mutex_unlock(&mutex);

        SLEEP(tmp);

        pthread_mutex_lock(&mutex);
            cut.is_cut = true;
            cout << TS << "ms - Pork#" << cut.pork_num << " : leaves CUTTER (complete 1st stage)\n";
            storage.push_back(cut);
        pthread_mutex_unlock(&mutex);

        sem_post(&sem3);
    }
	
    cut_end = true;
    pthread_exit(0);
}

void* packer(void *msg) 
{
    int i;
    item pack;
    no_pack = true;
    sem_wait(&sem3);
    no_pack = false;

    while(!storage.empty() || !cut_end)
    {
        pthread_mutex_lock(&mutex);
            for (i = 0; i < K && i < storage.size(); ++i) if (storage[i].is_cut == true)
            {
                pack = storage[i];
                break;
            }
        pthread_mutex_unlock(&mutex);

        if (i == storage.size())
        {
            no_pack = true;
            sem_wait(&sem3);
            no_pack = false;
            continue;
        }

        pthread_mutex_lock(&mutex);
            storage.erase(storage.begin() + i);
            cout << TS << "ms - Pork#" << pack.pork_num << " : enters to the factory (PACKER)\n";
        pthread_mutex_unlock(&mutex);

        pthread_mutex_lock(&mutex);
            int tmp = get_time(500, 1000);
            cout << TS << "ms - PACKER : processing & Packing the Pork#" << pack.pork_num << " (" << tmp << "ms)\n";
        pthread_mutex_unlock(&mutex);

        SLEEP(tmp);

        pthread_mutex_lock(&mutex);
            cout << TS << "ms - Pork#" << pack.pork_num << " : leaves PACKER (Complete)\n";
        pthread_mutex_unlock(&mutex);
    }
	
    pack_end = true;
    pthread_exit(0);
}

void* freezer(void *msg)
{
    sem_wait(&sem1);
    SLEEP(300);

    while(pork_cnt < N || !frez_pool.empty())
    {
        for (int i = 0; i < frez_pool.size(); ++i)
        {
            if (frez_pool[i].Tm == TS)
            {
                pthread_mutex_lock(&mutex);
                    if (storage.size() < 5)
                    {
                        cout << TS << "ms - Pork#" << frez_pool[i].pork_num << " : waiting in the slot\n";
                        storage.push_back( frez_pool[i] );
                        frez_pool.erase(frez_pool.begin() + i);
                        sem_post(&sem2);
                    }
                    else
                    {
                        int tmp2 = get_time(300, 500);
                        cout << TS << "ms - Pork#" << frez_pool[i].pork_num << " has been sent to the Freezer (" << tmp2 << "ms)\n";
                        frez_pool[i].Tm = TS + tmp2;
                    }
                pthread_mutex_unlock(&mutex);
            }
        }
    }

    pthread_exit(0);
}

void* no_work(void *msg)
{

    while(!cut_end && !pack_end)
    {
        if (no_cut && no_pack)
        {
            int tmp = rand()%2;
            if (tmp) cout << TS << "ms - CUTTER: under reviewing together...\n";
            else cout << TS << "ms - PACKER: under reviewing together...\n";
        }
        else if (no_cut) cout << TS << "ms - CUTTER: under maintenance.\n";
        else if (no_pack) cout << TS << "ms - PACKER: under maintenance.\n";

        SLEEP(10);
    }

    pthread_exit(0);
}

int main(int argc, char *argv[]) 
{
    pthread_t t1, t2, t3, t4, t5;
    if (argc == 3) N = atoi(argv[1]), K = atoi(argv[2]);
    else N = 10, K = 5;

    //srand(time(NULL));
    srand(5);

    ext_mode_init_timer();
    ext_mode_timer_start(0, 10);

    pthread_mutex_init(&mutex, NULL);
    pthread_create(&t1, NULL, pork, (void *)1);
    pthread_create(&t2, NULL, cutter, (void *)2);
    pthread_create(&t3, NULL, packer, (void *)3);
    pthread_create(&t4, NULL, freezer, (void *)4);
    pthread_create(&t5, NULL, no_work, (void *)5);
    
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    pthread_join(t3, NULL);
    pthread_join(t4, NULL);
    pthread_join(t5, NULL);

    pthread_mutex_destroy(&mutex);
    sem_destroy(&sem1);
    sem_destroy(&sem2);
    sem_destroy(&sem3);

    return 0;
}
