// Main.c

#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#define NUM_CUSTOMERS 3
#define MAX_NUM_CONES_PER_CUSTOMER 4
#define TRUE 1
#define FALSE 0

struct s_inspection {
	int passed;
	sem_t requested;
	sem_t finished;
	sem_t manager_lock;
} inspection;

struct s_cashier {
	int next_ticket_num;
	sem_t requested;
	sem_t customers[NUM_CUSTOMERS];
	sem_t num_lock;
} cashier_pay;

int manager_num_cones_approved = 0;


int get_rand(int min, int max)
{
	return (min + rand() % (max+1));
}

void rand_sleep()
{
	usleep(get_rand(1000,10000));
}
#define make_cone() (rand_sleep())
#define walk_to_cashier() (rand_sleep())
#define checkout_customer()(rand_sleep())

void setup_semaphoes()
{
	inspection.passed = FALSE;
	cashier_pay.next_ticket_num = 0;
	sem_init(&inspection.requested, 0, 0);
	sem_init(&inspection.finished, 0, 0);
	sem_init(&inspection.manager_lock, 0, 1);
	sem_init(&cashier_pay.requested, 0, 0);
	sem_init(&cashier_pay.num_lock, 0, 1);
	for (int i=0; i < NUM_CUSTOMERS; i++) {
		sem_init(&cashier_pay.customers[i], 0, 0);
	}
}

void destroy_semaphoes()
{
	sem_destroy(&inspection.requested);
	sem_destroy(&inspection.finished);
	sem_destroy(&inspection.manager_lock);
	sem_destroy(&cashier_pay.requested);
	sem_destroy(&cashier_pay.num_lock);
	for (int i=0; i < NUM_CUSTOMERS; i++) {
		sem_destroy(&cashier_pay.customers[i]);
	}
}

void *manager(void *val)
{
	int total_cones_needed = *((int*)val);
	int num_cones_inspeced = 0;

	while (manager_num_cones_approved < total_cones_needed) {
		sem_wait(&inspection.requested);
		num_cones_inspeced++;
		inspection.passed = get_rand(FALSE, TRUE);
		if (inspection.passed) {
			manager_num_cones_approved++;
			printf("\nManager Approved %d Cones", manager_num_cones_approved);
		}
		sem_post(&inspection.finished);
	}
		printf("\n####### Manager Finished");
	pthread_exit(NULL);
	return NULL;
}

void *clerk(void *clerk_done)
{
	int passed = FALSE;
	while (!passed)
	{
		make_cone();
		sem_wait(&inspection.manager_lock);
		sem_post(&inspection.requested);
		sem_wait(&inspection.finished);
		passed = inspection.passed;
		sem_post(&inspection.manager_lock);
	}
	sem_post((sem_t *)clerk_done);
	pthread_exit(NULL);
	return NULL;
}

void *customer(void *val)
{
	sem_t clerks_done;
	pthread_t clerk_threads[MAX_NUM_CONES_PER_CUSTOMER];
	int num_cones = *((int *)val);

	sem_init(&clerks_done, 0, 0);
	for (int i=0; i < num_cones; i++) {
		if(pthread_create(&clerk_threads[i], NULL, clerk, &clerks_done))
		{
			printf("\n ERROR creating clerk thread");
			exit(1);
		}
	}

	for (int i=0; i < num_cones; i++) {
		sem_wait(&clerks_done);
	}

	sem_destroy(&clerks_done);
	walk_to_cashier();
	sem_wait(&cashier_pay.num_lock);
	int ticket_num = cashier_pay.next_ticket_num++;
	sem_post(&cashier_pay.num_lock);
	sem_post(&cashier_pay.requested);
	sem_wait(&cashier_pay.customers[ticket_num]);
	pthread_exit(NULL);
	return NULL;
}

void *cashier(void *val)
{
	for (int i=0; i < NUM_CUSTOMERS; i++) {
		sem_wait(&cashier_pay.requested);
		checkout_customer();
		sem_post(&cashier_pay.customers[i]);
		printf("\nCustomers Paid: %d", i+1);

	}
	printf("\n$$$$$$$ Cashier Finished");
	pthread_exit(NULL);
	return NULL;
}

int main(int argc, char * argv[])
{
	srand(time(NULL));
	int total_cones = 0;
	pthread_t customer_threads[NUM_CUSTOMERS];
	pthread_t cashier_thread;
	pthread_t manager_thread;

	setup_semaphoes();

	for (int i=0; i < NUM_CUSTOMERS; i++) {
		int tmp_cones = get_rand(1, MAX_NUM_CONES_PER_CUSTOMER);
		pthread_create(&customer_threads[i], NULL, customer, (void *)&tmp_cones);
		total_cones += tmp_cones;
		printf("\nCustomer %d cones: %d", i, tmp_cones);
	}

	pthread_create(&cashier_thread, NULL, cashier, NULL);
	pthread_create(&manager_thread, NULL, manager, (void *)&total_cones);

	printf("\nTotal cones needed to be made: %d\n\n", total_cones);

	// Wait for Threads to Finish
	pthread_join(cashier_thread, NULL);

	if (manager_num_cones_approved != total_cones)
	{
		printf("\nSomething went WRONG!!!");
	}
	else
	{
		printf("\nSUCCESS");

	}
	printf("\n");

	destroy_semaphoes();
	return 0;
}
