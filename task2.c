//TASK 2: Calculating Pi using Leibniz formula and multithreading 

#include <stdio.h>
#include <pthread.h>
#include <math.h>
#include <stdlib.h>

//structure to hold the start and end point of each slice in thread
struct slicePoints{
	long long start;
	long long end;
};

//thread function to calculate to sum for the formula in different threads
void *threadFunc(void *args){
	long long i, start, end;
	struct slicePoints *slice = args; 
	start = slice->start;
	end = slice->end;
	
	double* sum = malloc(sizeof(double)); //creating a dynamic variable sum
	*sum = 0; //setting the value of sum as 0
	
	for(i=start; i<=end; i++){
        *sum += (pow(-1, i))/((2*i)+1); //calculating the sum part of the formula
	}
	
	pthread_exit((void *)sum); //exiting thread by returning the value of sum to the calling function
}


void main(){
	void *sum; //variable sum of void data type to catch the returned value from the thread function
	double total; //total to add all the returned sum from threads
	int i, numThreads;
	long long numIter;
	char choice;
	
	printf("\n---------- Calculating Pi (%c) using Leibniz formula and multithreading ----------", 227);
	pi:
	printf("\n\nLEIBNIZ FORMULA FOR PI:\n\n\t\t%c %c n=0  (-1)^n / (2*n + 1) = %c/4", 236, 228, 227);
	
	printf("\n\n\nEnter number of iterations for formula: ");
	scanf("%lld", &numIter); //input for iterations of formula
	printf("\nEnter number of threads: ");
	scanf("%d", &numThreads); //input number of threads to be used
    
	int rem = numIter % numThreads; //calculating remainder to evenly distribute among thread slices in case there are any
	
	long long slicedValue[numThreads]; //declaring slicedValue to slice the iterations to run on different threads
	
	for (i = 0; i<numThreads; i++){ //for loop to add slices in the sliced values excluding the remainder, if any
		slicedValue[i] = numIter/numThreads;
	}
	
	for (i=0; i<rem; i++){ //for loop to add the remainder iterations to the slices evenly, if any
		slicedValue[i] = slicedValue[i] + 1;
	}
	
	//arrays to store the start points and end points for each thread
	long long startPoints[numThreads], endPoints[numThreads];
	
	//for loop to allocate the start and end points for each thread
	for(i=0; i<numThreads; i++){
		if (i == 0){
			startPoints[i] = 0; //start point for first thread (to start the calculation of pi from 0)
		} else {
			startPoints[i] = endPoints[i-1] + 1; //start points of other threads
		}
		endPoints[i] = startPoints[i] + slicedValue[i] - 1; //end points of threads
	}
	
	
	pthread_t threads[numThreads]; //creating thread array variable
	struct slicePoints slice[numThreads]; //initializing the structure
	
	printf("\n\n\nCreating threads........\n\n");
	
	for (i=0; i<numThreads; i++){
		//putting the start and end points in the structure to pass to the threads as parameter
		slice[i].start = startPoints[i]; 
		slice[i].end = endPoints[i];
		pthread_create(&threads[i], NULL, threadFunc, &slice[i]); //creating thread
	}
	
	printf("\nCalculating........\n\n");
	
	total = 0;
	
	for (i=0; i<numThreads; i++){
		pthread_join(threads[i], &sum); //joining threads for waiting to ensure that all threads are completed
		total += *(double*)sum; //adding the values of sum returned from the thread functions
	}
	
	printf("\n\n\nThe estimated value of pi with %lld iterations: ", numIter);
    printf("\n\n\t%c = %.14lf\n", 227, total*4); //printing the value of pi by multiplying the total by 4
    printf("\n\n----------------------------------------------------------------------------------------------------\n");
    
	//user input if the user wants to do any more calculations for the value of pi
	ask:
	printf("\n\nDo you want to calculate the value of pi (%c) using different number of iterations? (y/n): ", 227);
	scanf(" %c", &choice); 
	//conditions according to user choice
	if(choice == 'Y' || choice == 'y'){
		goto pi; //to calculate the value of pi again
	} else if(choice == 'N' || choice == 'n'){
		free(sum); //freeing the dynamically allocated sum variable
		exit(0);
	} else {
		printf("\nPlease enter either 'y' or 'n'."); //to inform user to input correct choice
		goto ask;
	}	 
}
