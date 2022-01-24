//TASK 1: LINEAR REGRESSION

#include <stdio.h>
#include <stdlib.h>

void main(){
	int i;
	//declaring floats to store the values from file, sum of the required calculations, result and further calculations
	float x, y, sumX=0, sumX2=0, sumY=0, sumXY=0, n=0, a, b, valx, valy;
	//declaring character to store the end of file, the array to store the filename from which to read data and user choice for calculation
	char eof, filename[20], choice;
	//declaring file pointer
	FILE *fp;
	
	
	//for loop to read data from file and do the necessary calculations
	for(i=1; i<=4; i++){
		sprintf(filename, "datasetLR%d.txt", i);
		fp = fopen (filename,"r");
		while(1){
			eof = fscanf (fp,"%f, %f", &x, &y);
			if (eof == EOF){
				break;
			}		 
			sumX += x;
			sumY += y;
			sumX2 += (x*x);
			sumXY += (x*y);
			n++;	
		}
		fclose(fp);
	}
	
	//calculating the values of a and b by applying formula
	a = ((sumX2*sumY)-(sumX*sumXY))/((n*sumX2)-(sumX*sumX));
	b = ((n*sumXY)-(sumX*sumY))/((n*sumX2)-(sumX*sumX));
	
	//printing the values of a and b and the best fit line
	printf("\n----------\tLINEAR REGRESSION\t---------");
	printf("\n\nUsing the Linear Regression formula, \n\tThe values are: a = %f \t and \t b = %f",a,b);
	printf("\n\nEquation of best fit is: \n\ty = %0.2f x + %0.2f", b, a);
	printf("\n\n-----------------------------------------------------------------------------------");
		
	//taking user input for x to calculate the value of y using the best fit line
	calculation: 
	printf("\n\nEnter a value of x: ");
	scanf("%f", &valx);
	valy = (b*valx) + a;
	printf("\nUsing the above equation, when x = %0.2f,\n\tThe value of y is: %f", valx, valy);
	printf("\n\n-----------------------------------------------------------------------------------");
	
	//user input if the user wants to do any more calculations
	ask:
	printf("\n\nDo you want to do any more calculations? (y/n): ");
	scanf(" %c", &choice); 
	//conditions according to user choice
	if(choice == 'Y' || choice == 'y'){
		goto calculation; //to calculate the value of y again
	} else if(choice == 'N' || choice == 'n'){
		exit(0);
	} else {
		printf("\nPlease enter either 'y' or 'n'."); //to inform user to input correct choice
		goto ask;
	}
}

