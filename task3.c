//TASK 3: Finding prime numbers from multiple text files using multithreading 

#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

//structure to hold the start and end point of each slice in thread
struct slicePoints{
	int start;
	int end;
};


int *arrNum; //integer pointer globally created to store the dynamically allocated array, so that it can be used both in main and thread fucntions

FILE *fp; //file pointer (to open a file to write the prime numbers)


//thread function to check if the number is prime or not and write it in another file accordingly
void *checkPrimeThread(void *args){
	int i, j, start, end;
	struct slicePoints *thread = args;
	start = thread->start; //start point
	end = thread->end; //end point
	
	int* primesCount = malloc(sizeof(int)); //creating a dynamic variable primesCount
	*primesCount = 0; //setting the value of primesCount as 0

	//for loop to traverse through the numbers from start point to end point
	for(i = start; i <= end; i++){
		//loop to check if a number is prime or not
		for (j = 2; j <= arrNum[i]-1; j++){
			if(arrNum[i] % j == 0){
				break; //breaking the loop is a number is divisible by any number other than 1 or itself
			}
		}
		//if condition to check if the number was a prime
		if (j == arrNum[i]){ 
			fprintf(fp, "%d\n", arrNum[i]); //if the number was a prime, then write it in the new file
			*primesCount += 1; //increase the count of prime numbers
		}		
	}
	
	pthread_exit((void *)primesCount); //exiting thread by returning the value of primesCount to the calling function
}


void main(){
	void *primes; //variable primes of void data type to catch the returned value from the thread function
	int i, num, count=0, numThreads, arrCount = 0, noOfFiles = 3, totalPrimes;
	char eof, inputFilename[20];
	
	printf("\n---------- Finding prime numbers from multiple text files using multithreading ----------\n\n\n");

	//for loop to read the files and count the numbers
	for(i=1; i<=noOfFiles; i++){
		sprintf(inputFilename, "PrimeData%d.txt", i);
		fp = fopen (inputFilename,"r");
		while(1){
			eof = fscanf (fp,"%d", &num);
			if (eof == EOF){
				break;
			}		 
			count++;
		}
		fclose(fp);
	}
	
	arrNum = malloc(count*sizeof(int)); //dynamically allocating memory to store the numbers from the files
	printf("The amount of numbers in given %d files are: %d\n\n", noOfFiles, count);
	
	printf("Enter amount of threads: ");
	scanf("%d", &numThreads); //input number of threads
	
	//another for loop to read the data from the files and store it in the dynamic array
	for(i=1; i<=noOfFiles; i++){
		sprintf(inputFilename, "PrimeData%d.txt", i);
		fp = fopen (inputFilename,"r");
		
		while(1){
			eof = fscanf (fp,"%d", &arrNum[arrCount]);
			arrCount++;
			if (eof == EOF){
				break;
			}		 
		}
		fclose(fp);
	}
	
	//opening a new file 'PrimeNumbers' in append mode to write the prime numbers in it
	fp = fopen("PrimeNumbers.txt","a");
		
	int rem = count % numThreads; //calculating remainder to evenly distribute among thread slices in case there are any
	
	int slicedValue[numThreads];  //declaring slicedValue to slice the iterations to run on different threads
	
	for (i = 0; i<numThreads; i++){ //for loop to add slices in the sliced values excluding the remainder, if any
		slicedValue[i] = count/numThreads;
	}
	
	for (i=0; i<rem; i++){ //for loop to add the remainder iterations to the slices evenly, if any
		slicedValue[i]= slicedValue[i] + 1;
	}
	
	//arrays to store the start points and end points for each thread
	int startPoints[numThreads], endPoints[numThreads];
	
	//for loop to allocate the start and end points for each thread
	for(i=0; i<numThreads; i++){
		if (i == 0){
			startPoints[i] = 0; //start point for first thread (to check the number from the 0th index of the array)
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
		pthread_create(&threads[i], NULL, checkPrimeThread, &slice[i]);	//creating thread
	}
	
	printf("\nChecking for primes........\n\n");
	
	totalPrimes = 0;
	
	for (i=0; i<numThreads; i++){
		pthread_join(threads[i], &primes); //joining threads for waiting to ensure that all threads are completed
		totalPrimes += *(int*)primes; //adding the number of primes returned from the thread functions
	}
	
	printf("\n\nTotal primes in the given %d files: %d\n\n", noOfFiles, totalPrimes);
	printf("\n\nNote: All the prime numbers are written in a text file named 'PrimeNumbers' in the same directory.");
	
	fprintf(fp, "\nTotal number of prime numbers: %d\n", totalPrimes); //to write the total number of primes in the file
	
	free(arrNum); //freeing the dynamically created arrNum array
	free(primes); //freeing the dynamically allocated primesCount variable
	fclose(fp); //closing the file pointer
}
