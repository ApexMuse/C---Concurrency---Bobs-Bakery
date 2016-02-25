/*	AUTHOR: Todd Twiggs
*	DATE:	5 Nov 2015
*/

#include <stdio.h>
#include <pthread.h>

pthread_mutex_t cupcake_mutex;
pthread_mutex_t donut_mutex;
int cupcakes = 1;
int donuts = 1;

void removeCupcake(){
	pthread_mutex_lock(&cupcake_mutex);
	printf("A customer purchases a cupcake.\n");
	printf("The number of cupcakes was %d\n", cupcakes);
	cupcakes = cupcakes - 1;
	printf("The number of cupcakes is now %d\n\n", cupcakes);
	pthread_mutex_unlock(&cupcake_mutex);
}

void addCupcake(){
	pthread_mutex_lock(&cupcake_mutex);
	printf("A cupcake was baked.\n");
        printf("The number of cupcakes was %d\n", cupcakes);
	cupcakes = cupcakes + 1;
        printf("The number of cupcakes is now %d\n\n", cupcakes);
	pthread_mutex_unlock(&cupcake_mutex);
}

void removeDonut(){
	pthread_mutex_lock(&donut_mutex);
	printf("A customer purchases a donut.\n");
	printf("The number of donuts was %d\n", donuts);
	donuts = donuts - 1;
	printf("The number of donuts is now %d\n\n", donuts);
	pthread_mutex_unlock(&donut_mutex);
}

void addDonut(){
	pthread_mutex_lock(&donut_mutex);
	printf("A donut was baked.\n");
        printf("The number of donuts was %d\n", donuts);
	donuts = donuts + 1;
	printf("The number of donuts is now %d\n\n", donuts);
	pthread_mutex_unlock(&donut_mutex);
}

// Up to 5 customers make a random purchase choice
void *purchase(void *arg) {
	int numCustomers, i, custChoice;
	while ((cupcakes > 0) || (donuts > 0)) {
		// Create a random number of customers
		numCustomers = rand() % 6;
		for (i = 0; i < numCustomers; i++){
			custChoice = rand() % 3 + 1;   // Random choice from 1 to 3
			if ((custChoice == 1) && (cupcakes > 0)) {   // Customer wants a cupcake
				removeCupcake();
			}
			else if ((custChoice == 2) && (donuts > 0)) {   // Customer wants a donut
				removeDonut();
			}
			else if ((cupcakes > 0) && (donuts > 0)){   // Customer wants a cupcake and a donut
				removeCupcake();
				removeDonut();
			}
			else {
				printf("We don't have what the customer wants.\n");
				printf("The customer left the store.\n\n");
			}
			sleep(1); // Pause for one second while the next customer chooses
		}//End of for loop
		sleep(1); // Wait for new customers to come in
	} // End of while loop
	printf("=======Customer thread complete, cupcakes = %d and donuts = %d=======\n", cupcakes, donuts);
}// End of purchase function

//function that increments the cupcakes every 2 seconds
void *makeCupcake(void *arg) {
     while (cupcakes < 10) {
          addCupcake();
          if (cupcakes == 10) sleep(10);
          else sleep(2);
     }//End of while loop
      printf("=======Cupcake Baker thread complete, cupcakes = %d=======\n", cupcakes);
}//End of makeCupcake function

//function that increments the donuts every 1 second
void *makeDonut(void *arg) {
     while (donuts < 15) {
          addDonut();
          if (donuts == 15) sleep(15);
          else sleep(1);
     }//End of while loop
      printf("=======Donut Baker thread complete, donuts = %d=======\n", donuts);
}//End of makeDonut function

int main(){
     pthread_t cupcakeBaker, donutBaker, customer;
     pthread_create(&cupcakeBaker, NULL, makeCupcake,NULL);
     pthread_create(&donutBaker, NULL, makeDonut, NULL);
     pthread_create(&customer, NULL, purchase,NULL);
     pthread_join(cupcakeBaker, NULL);
     pthread_join(donutBaker, NULL);
     pthread_join(customer, NULL);
     printf("=======Main thread complete, cupcakes = %d and donuts = %d=======\n", cupcakes, donuts);
     return 0; 
}

