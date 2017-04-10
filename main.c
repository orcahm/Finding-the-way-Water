#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int **create_matrix(int **,int ); 						/*Create a dynamic matrix*/
int matrix_delete(int **,int ); 						/*Delete a matrix*/
int *get_random(int ** ,int * ,int );					/*Get a random value*/
int run_time(int **, char **);							/*Try the function program*/
void open_place(int **, int *,int );					/*the function of the excavated soil*/
void water_fill(int **, int *, int );					/*water fills the place opened there is to be filled*/
void water_run(int **,int *,int );						/*previously excavated ground water filling functions*/
int transmission(int **,int );							/*function which checks whether it reaches the bottom of the water*/
void file_write(int **,int,double,double );				/*File's .txt write output*/
double square_root(double );							/*Square root calculate for standartdev*/
double mean(int *,int, int);							/*Calculate mean*/
double standartdev(int *,int,int,double);				/*Calculate standartdev*/

int main(int argc, char *argv[]){
	int **matrix ;										/*matrix will be formed*/		
	srand(time(NULL)); 									/*To change the number of laying random value*/

		run_time(matrix,argv);
	return 0;
}

int run_time(int **matrix,char **argv){

	int i, j; 														/*The values used in the loop*/
	int row_column = atoi(argv[1]); 								/*Matrix's row and column value*/
	int get_repeat_value = atoi(argv[2]);							/*How many will be made involving the transfer value*/
	int repeat_value = 0;											/*holding the number of transfer values*/
	int *rand_value = calloc(2,sizeof(int));						/*holding the value received random sequence*/
	int return_num;													/*it is also worth keeping a few random transfer*/
	int *return_array = calloc(get_repeat_value,sizeof(int));		/*involving the transfer of value range*/
	double mean_value, standartdev_value;							
	
	while(repeat_value!=get_repeat_value){								/*the user wants it back*/
		matrix = create_matrix(matrix, row_column);	
		return_num=1;
 		while(transmission(matrix,row_column)){							/*a return loop transfer*/
			return_num++;
			rand_value = get_random(matrix ,rand_value,row_column);
			open_place(matrix , rand_value, row_column);
			
		}
		*(return_array+repeat_value)=return_num;
		repeat_value++;
		if(repeat_value==get_repeat_value){
			mean_value = mean(return_array, row_column, get_repeat_value);
			standartdev_value = standartdev(return_array, row_column, get_repeat_value, mean_value);
				file_write(matrix, row_column, mean_value, standartdev_value);
	}else	matrix_delete(matrix, row_column);
	}	
	
}

int **create_matrix(int **matrix,int row_column){						/*create a dynamic matrix*/

	int i, j;

	matrix = (int **) calloc(row_column , sizeof(int*));

	for(i=0 ; i < row_column ;i++){
		matrix[i] = (int *) calloc(row_column , sizeof(int));
		}

	return matrix;
}

int *get_random(int **matrix ,int *rand_value,int mod_value){			/*get two random values*/

	int i ,j;

		rand_value[0] = rand()%mod_value;								/* amount x-coordinate value of the random value*/
		rand_value[1] = rand()%mod_value;								/* amount y-coordinate value of the random value*/ 

		if(matrix[rand_value[0]][rand_value[1]]==1 || 
					matrix[rand_value[0]][rand_value[1]]==2){	
			get_random(matrix ,rand_value ,mod_value);					/*If the get_random function returns already taken those values*/
		}else return rand_value;
}

void open_place(int **matrix, int *rand_value,int row_column){			/* Frees received random value*/
	matrix[rand_value[0]][rand_value[1]] = 1;
	water_fill(matrix,rand_value,row_column);
}

void water_fill(int **matrix,int *rand_value,int row_column){			/* water fills the place opened there is to be filled*/

	int x , y ;
	x= *rand_value;
	y= *(rand_value+1);

	if(x==0 || *(*(matrix+(x-1))+y)==2){
		*(*(matrix+x)+y)=2;
		water_run(matrix,rand_value,row_column);
	}
	if(y!=0 && *(*(matrix+x)+(y-1))==2){
		*(*(matrix+x)+y)=2;
		water_run(matrix,rand_value,row_column);
	}
	if(y!=row_column-1 && *(*(matrix+x)+(y+1))==2){
		*(*(matrix+x)+y)=2;
		water_run(matrix,rand_value,row_column);
	}
	if(x!=row_column-1 && *(*(matrix+(x+1))+y)==2){
		*(*(matrix+x)+y)=2;
		water_run(matrix,rand_value,row_column);
	}

}

void water_run(int **matrix,int *rand_value,int row_column){			/*previously excavated ground water filling functions*/

	int x , y ;
	x= *rand_value;
	y= *(rand_value+1);

		if(x!=0 && *(*(matrix+(x-1))+y)==1){							/*  controls the location of high-fill water */
			*(*(matrix+(x-1))+y)=2;
				*rand_value -=1;
					water_run(matrix,rand_value,row_column);
			*rand_value+=1;
		}
		
		if(y!=0 && *(*(matrix+(x))+(y-1))==1){							/* Controls the left around the place of water */
			*(*(matrix+x)+(y-1))=2;
				*(rand_value+1)-=1;
					water_run(matrix,rand_value,row_column);
			*(rand_value+1)+=1;
		}
		
		if(y+1<row_column && *(*(matrix+x)+(y+1))==1){					/* Controls the right place filled water */
			*(*(matrix+x)+(y+1))=2;
				*(rand_value+1)+=1;
					water_run(matrix,rand_value,row_column);
			*(rand_value+1)-=1;
		}
		
		if(x+1<row_column && *(*(matrix+(x+1))+y)==1){					/* Checks the water under the earth around*/
			*(*(matrix+(x+1))+y)=2;
				*rand_value+=1;
					water_run(matrix,rand_value,row_column);
			*rand_value-=1;
		}
		
}

int transmission(int **matrix,int row_column){							/* Controls the transfer of water */
	int i;

	for(i=0 ;i<row_column;i++){
		if(*(*(matrix+(row_column-1))+i)==2){
		return 0;
		}
	}
	return 1;
}

double square_root(double num){											/*Square root calculate for standartdev*/
	
	double square_root_value;
    int i;
    	square_root_value=num/2.0;
   			for(i=0;i<50;i++)
 				 square_root_value=(square_root_value+num/square_root_value)/2;
     	return square_root_value;
}

double mean(int *return_array,int row_column,int get_repeat_value){		/*Calculate mean*/
	
	int i;
	int T = row_column * row_column;
	int sum = 0;
	
	for(i=0;i<get_repeat_value;i++){
		sum += *(return_array+i);
	}
	
	return ((double)sum)/((double)get_repeat_value*(double)T);
}

double standartdev(int *return_array,int row_column,int get_repeat_value,double mean_value){		/*Calculate standartdev*/
	
	int i;
	double T = (double)(row_column*row_column);
	double diff = 0.0;
	double sum = 0.0;
	
	for(i=0;i<get_repeat_value;i++){
		diff = mean_value-((double)*(return_array+i)/T);
		sum += diff*diff;
	}
	return square_root(sum/(double)(get_repeat_value-1));
	
}

void file_write(int **matrix,int row_column,double mean_value,double standartdev_value){			/*File's .txt write output*/
	
	FILE *txt;
	int i, j;
	
	txt = fopen("output.txt" , "w");
		for(i=0;i<row_column;i++){
			for(j=0;j<row_column;j++){
				fprintf(txt,"%d " ,*(*(matrix+i)+j));
			}
			fprintf(txt,"\n");
		}
		fprintf(txt,"\nMean()   = %f", mean_value);
		fprintf(txt,"\nstddev() = %f", standartdev_value);
	fclose(txt);
}

int matrix_delete(int **matrix,int row_column){			//delete a matrix

		int i;

		for(i=0 ; i < row_column ;i++){
			free(matrix[i]);
		}

	free(matrix);
}

