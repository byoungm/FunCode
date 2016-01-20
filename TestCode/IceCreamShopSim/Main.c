// Main.c

#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define NUM_CUSTOMERS 10
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

int get_rand(int min, int max)
{
	return (min + rand() % (max+1));
}

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
	int num_cones_approved = 0;

	while (num_cones_approved < total_cones_needed) {
		sem_wait(&inspection.requested);
		num_cones_inspeced++;
		inspection.passed = get_rand(FALSE, TRUE);
		if (inspection.passed) {
			num_cones_approved++;
			printf("\nManager Approved %d Cones", num_cones_approved);
		}
		sem_post(&inspection.finished);
	}
	return NULL;
}

void *clerk(void *clerk_done)
{
	int passed = FALSE;
	while (!passed)
	{
		//make_cone();
		sem_wait(&inspection.manager_lock);
		sem_post(&inspection.requested);
		sem_wait(&inspection.finished);
		passed = inspection.passed;
		sem_post(&inspection.manager_lock);
	}
	sem_post((sem_t *)clerk_done);
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
	// Walk to Cashier
	sem_wait(&cashier_pay.num_lock);
	int ticket_num = cashier_pay.next_ticket_num++;
	sem_post(&cashier_pay.num_lock);
	sem_post(&cashier_pay.requested);
	sem_wait(&cashier_pay.customers[ticket_num]);
	return NULL;
}

void *cashier(void *val)
{
	for (int i=0; i < NUM_CUSTOMERS; i++) {
		sem_wait(&cashier_pay.requested);
		// Checkout Customer (i)
		sem_post(&cashier_pay.customers[i]);
	}
	printf("\nAll Customers Paid");
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
		if(pthread_create(&customer_threads[i], NULL, customer, (void *)&tmp_cones))
		{
			printf("\n ERROR creating customer thread: %d", i);
			exit(1);
		}
		total_cones += tmp_cones;
	}

	if(pthread_create(&cashier_thread, NULL, cashier, NULL))
	{
		printf("\n ERROR creating cashier thread");
		exit(1);
	}

	if(pthread_create(&manager_thread, NULL, manager, (void *)&total_cones))
	{
		printf("\n ERROR creating manager thread");
		exit(1);
	}

	printf("\nTotal cones needed to be made: %d", total_cones);
	if(pthread_join(cashier_thread, NULL)) // Once the cashier is done we are done
	{
		printf("\n ERROR exiting thread could not be joined");
		exit(1);
	}
	destroy_semaphoes();

	printf("\nSTORE CLOSED");
	printf("\n");
	pthread_exit(NULL);
	return 0;
}
