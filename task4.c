//TASK 4: Gaussian Blur with multithreading 

#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include "lodepng.h"

//structure to hold the start points, end points and heigth & width of the image
struct threadArgs{
	int start, end;
	unsigned int width, height;
};

//arrays declared to hold the pixels of old and new blurred images
unsigned char *oldImage;
unsigned char *newImg;

//thread function to blur an image
void *blurImage(void *args){
	int i, k, l, div, R, G, B;
	struct threadArgs *thread = args;
	int start = thread->start, end = thread->end;
	unsigned int w = thread->width, h = thread->height;
	
	for(i=start; i<=end; i+=4){
		R = G = B = 0;
		//taking average for the TOP LEFT CORNER pixel of the image
		if(i == 0){
			R += oldImage[i+4] + oldImage[i+(w*4)] + oldImage[(w*4)+i+4] + oldImage[i];
			G += oldImage[i+5] + oldImage[i+(w*4)+1] + oldImage[(w*4)+i+5] + oldImage[i+1];
			B += oldImage[i+6] + oldImage[i+(w*4)+2] + oldImage[(w*4)+i+6] + oldImage[i+2];		
			div = 4; //as the pixel is only surrounded by 3 other pixels
		} 
		//taking average for the TOP RIGHT CORNER pixel of the image
		else if(i == ((w*4)-4)){
			R += oldImage[i-4] + oldImage[i+(w*4)] + oldImage[(w*4)+i-4] + oldImage[i];
			G += oldImage[i-3] + oldImage[i+(w*4)+1] + oldImage[(w*4)+i-3] + oldImage[i+1];
			B += oldImage[i-2] + oldImage[i+(w*4)+2] + oldImage[(w*4)+i-2] + oldImage[i+2];
			div = 4; 
		}
		//taking average for the BOTTOM LEFT CORNER pixel of the image
		else if(i == (w*4*(h-1))){
			R += oldImage[i+4] + oldImage[i-(w*4)] + oldImage[i-(w*4)+4] + oldImage[i];
			G += oldImage[i+5] + oldImage[i-(w*4)+1] + oldImage[i-(w*4)+5] + oldImage[i+1];
			B += oldImage[i+6] + oldImage[i-(w*4)+2] + oldImage[i-(w*4)+6] + oldImage[i+2];	
			div = 4;
		}
		//taking average for the BOTTOM RIGHT CORNER pixel of the image
		else if(i == ((w*h*4)-4)){
			R += oldImage[i-4] + oldImage[i-(w*4)] + oldImage[i-(w*4)-4] + oldImage[i];
			G += oldImage[i-3] + oldImage[i-(w*4)+1] + oldImage[i-(w*4)-3] + oldImage[i+1];
			B += oldImage[i-2] + oldImage[i-(w*4)+2] + oldImage[i-(w*4)-2] + oldImage[i+2];
			div = 4;
		}
		//taking average for the LEFT COLUMN pixels of the image 
		else if((i%(w*4)) == 0){
			for(k=i-(w*4); k<=(i+(w*4)); k+= (w*4)){
				for(l=k; l<=k+4; l+=4){
					R += oldImage[l];
					G += oldImage[l+1];
					B += oldImage[l+2];
				}
			}
			div = 6; //as the pixel is surrounded by 5 other pixels
		}
		//taking average for the RIGHT COLUMN pixels of the image
		else if((i%(w*4)) == ((w*4)-4)){
			for(k=(i-(w*4)); k<=(i+(w*4)); k+=(w*4)){
				for(l=k; l>=(k-4); l-=4){
					R += oldImage[l];
					G += oldImage[l+1];
					B += oldImage[l+2];
				}
			}
			div = 6;
		}
		//taking average for the TOP ROW pixels of the image
		else if(i!=0 && i<((w*4)-4)){
			for(k=0; k<=(w*4); k += (w*4)){
				for(l=i-4; l<=(i+4); l+=4){
					R += oldImage[k+l];
					G += oldImage[k+l+1];
					B += oldImage[k+l+2];
				}
			}
			div = 6;
		}
		//taking average for the BOTTOM ROW pixels of the image
		else if(i > (w*4*(h-1)) && i < ((w*h*4)-4)){
			for(k=(-(w*4)); k<=0; k+=(w*4)){
				for(l=(i-4); l<=(i+4); l+=4){
					R += oldImage[k+l];
					G += oldImage[k+l+1];
					B += oldImage[k+l+2];
				}
			}
			div = 6;
		}
		//taking average for ALL OTHER pixels of the image
		else {
			for(k=(i-4); k<=(i+4); k+=4){
				for(l=(k-(w*4)); l<=(k+(w*4)); l+=(w*4)){
					R += oldImage[l];
					G += oldImage[l+1];
					B += oldImage[l+2];
				}
			}
			div = 9;
		}
		
		//averaging the values of RGB of surrounding pixels and storing the new pixel value to the newImg array	
		newImg[i] = R/div;
		newImg[i+1] = G/div;
		newImg[i+2] = B/div;
		newImg[i+3] = oldImage[i+3];
	}
	
	pthread_exit(0);
}


void main(){
	int i, numThreads;
	unsigned int error, w, h;
	unsigned char *png;
	char filename[100], outfilename[100];
	
	printf("\n---------- Gaussian Blur with multithreading ----------");
	printf("\n\nEnter a input file name: ");
	scanf("%s", filename);
	
	error = lodepng_decode32_file(&oldImage, &w, &h, filename); //reading an image file into oldImage array
	
	if (error){
		printf("\nError in decoding image %u: %s\n", error, lodepng_error_text(error));
		exit(0);
	}  
	
	printf("\n\nDimension of the image: %d X %d \n\nTotal number of pixels: %d\n", h, w, w*h);
	
	newImg = malloc((w*h)*sizeof(int)); //creating a dynamic array to store the new image pixels with the same dimension as the old image
	
	printf("\n\nEnter amount of threads: ");
	scanf("%d", &numThreads); //input number of threads to be used
		
	int rem = (w*h) % numThreads; //calculating remainder to evenly distribute among thread slices in case there are any
	
	int slicedValue[numThreads]; //declaring slicedValue to slice the iterations to run on different threads
	
	for (i = 0; i<numThreads; i++){ //for loop to add slices in the sliced values excluding the remainder, if any
		slicedValue[i] = (w*h)/numThreads;
	}
	
	for (i=0; i<rem; i++){ //for loop to add the remainder iterations to the slices evenly, if any
		slicedValue[i] = slicedValue[i] + 1;
	}
	
	//arrays to store the start points and end points for each thread
	int startPoints[numThreads], endPoints[numThreads];
	
	//for loop to allocate the start and end points for each thread
	for(i=0; i<numThreads; i++){
		if (i == 0){
			startPoints[i] = 0; //start point for first thread (to start the calculation of pi from 0)
		} else {
			startPoints[i] = endPoints[i-1] + 4; //start points of other threads
		}
		endPoints[i] = startPoints[i] + (slicedValue[i]*4) - 4; //end points of threads
	}
	
	pthread_t threads[numThreads]; //creating thread array variable
	struct threadArgs slice[numThreads]; //initializing the structure
	
	printf("\n\nCreating threads........\n");
	
	for (i=0; i<numThreads; i++){
		//putting the start points, end points, width and height in the structure to pass to the threads as parameter
		slice[i].start = startPoints[i];
		slice[i].end = endPoints[i];
		slice[i].width = w;
		slice[i].height = h;
		pthread_create(&threads[i], NULL, blurImage, &slice[i]); //creating thread
	}
	
	printf("\nBlurring image........\n\n");
	
	for (i=0; i<numThreads; i++){
		pthread_join(threads[i], NULL); //joining threads for waiting to ensure that all threads are completed
	}
	
	printf("\n\t\t------ Image Blurred -----\n\n\nEnter a output file name to save the blurred image: ");
	scanf("%s", outfilename); //input the blurred Image file name
	size_t pngsize;
	error = lodepng_encode32(&png, &pngsize, newImg, w, h); //encoding the blurred image in a new file
	if (!error){
		lodepng_save_file(png, pngsize, outfilename); //saving the blurred image file
	} else {
		printf("Error in encoding image %u: %s\n", error, lodepng_error_text(error)); //error msg if any error occurs in encoding the blurred image
	}
	
	printf("\n\n\nNote: The blurred image '%s' has been saved in the same directory.\n\n", outfilename);
	//freeing the dynamically allocated oldImage and newImg array
	free(oldImage); 
	free(newImg); 
}

