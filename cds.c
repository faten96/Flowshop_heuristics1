#include <stdio.h>
#include <stdlib.h>
#include <math.h>

float **x; //johnson's array
float **problem; //processing time of each job in each machine
int machines=0,jobs=0;
char *nomfichier;

struct Ref {
    float min;
    int indligne;
    int indcolonne;};


void read(float** array, int rows, int cols){
  int i, j;
  for (i=0; i<rows; i++)
  {
    for (j=0; j<cols; j++)
    {
      printf("Enter value for Array[%d][%d]: ",i,j);
      scanf("%f",&array[i][j]);
    }
  }
}

void display(float** array, int rows, int cols){
  int i, j;
  for (i=0; i<rows; i++)
  {
    for (j=0; j<cols; j++)
    {
      printf("%f    ",array[i][j]);
    }
    printf("\n");
  }
}

int valueExist(int val,int taille, int array[taille]){

    int i=0,exist=0;
    while(i<taille && exist==0){

        if(array[i]==val){
            exist=1;
        }
        i++;
    }
    return exist;
}

void johnsonRule(int m,int sequence[m]){

    int i=0,j=0,trouv=0,cpt=1,ind1,ind2;
    float min=100000;
    struct Ref array[m];
    int eliminatedJobs[m];

    for(i=0;i<m;i++){
        eliminatedJobs[i]=-1;
    }

    // build the array of struct
    while(cpt<=m){
        //search for the new min
        for(i=0;i<2;i++){
            for(j=0;j<m;j++){

                if(x[i][j]<=min && valueExist(j,m,eliminatedJobs)==0) {

                    min=x[i][j];
                    ind1=i;
                    ind2=j;
                }
            }
        }
        //eliminate the job
        eliminatedJobs[cpt-1]=ind2;

        // fill up the struct
        array[cpt-1].min=min;
        array[cpt-1].indligne=ind1;
        array[cpt-1].indcolonne=ind2;


        min=100000;
        cpt++;
    }
    //construct the right job sequence
    int index1=0,index2=m-1;
    for(i=0;i<m;i++){

        if(array[i].indligne==0){
            sequence[index1]=array[i].indcolonne; //jobs are from 0 to 1
            index1++;
        }
        else{
            sequence[index2]=array[i].indcolonne;
            index2--;
        }
    }


}

float maximun(float a, float b){

    if(a>=b) return a;
    else return b;
}

void fill_matrix(){

      FILE *myfile;
      double myvariable;
      int i,j;

      myfile=fopen(nomfichier,"r");

      //read number of rows and columns (jobs and machines)
      int rows,cols;
      fscanf(myfile,"%d",&rows); //machines
      fscanf(myfile,"%d",&cols); //jobs
      machines=rows;
      jobs=cols;

      problem = malloc(machines * sizeof *problem);
      for (i=0; i<machines; i++){
        problem[i] = malloc(jobs * sizeof *problem[i]);
      }

      for(i = 0; i < rows; i++)
      {
        for (j = 0 ; j < cols; j++)
        {
          fscanf(myfile,"%lf",&myvariable);
          problem[i][j]=myvariable;
          //printf("%.4f ",tab[i][j]);
        }
        //printf("\n");
      }
      fclose(myfile);
}

int main(int argc, char *argv[])
{
  int i,j;
  //int machines=0,jobs=0;
    nomfichier=argv[1]; //set the file name
    //test namefile
   /* printf("%s\n",argv);
    printf("%s",nomfichier);*/

  //*************************************** Data initialization *****************************************//
   /* obtain machines number
       printf("Machines number:");
       scanf("%d", &machines);
   /* obtain number of jobs
       printf("Job number:");
       scanf("%d", &jobs);*/

      /*problem = malloc(machines * sizeof *problem);
      for (i=0; i<machines; i++){
        problem[i] = malloc(jobs * sizeof *problem[i]);
      }*/

      //read(problem,machines,jobs);
      fill_matrix();
      //display(problem,machines,jobs);

     //************************************** Treatment ***************************************************//

    ///Step1: Find out the sum of processing time of jobs in each machine
    float sum[machines],tmp=0;
    for(i=0;i<machines;i++){
        for(j=0;j<jobs;j++){
            tmp=tmp+problem[i][j];
        }
        sum[i]=tmp;
        tmp=0;
    }
     ///Step2: Make two groups from m machines in such away that the diff between the sum is minimun

        int cpt=1,trouv=0,sum1=0,sum2=0;
        while(trouv==0){

            sum1=0; sum2=0;
            /* first sum */
            for(i=0;i<cpt;i++) sum1=sum1+sum[i];
            /* second sum */
            for(i=cpt;i<machines;i++) sum2=sum2+sum[i];

            if((sum1-sum2)>=0) trouv=1;
            else cpt++;

        }
 ///Step3: Find out the total number of machines in each group.Let the number of machines in Group I =a and the number of machines in Group II =b
    int a=cpt,b=machines-cpt;
        //test afficher la nombre de machines dans chaque groupe
       // printf("\n  a=%d   et b=%d",a,b);



///Step4: Calculate total operational time of jobs in each group using the formula and tabulate them.
    float s1=0,s2=0,n=b-1;

      /* allocate the array */
      x = malloc(machines * sizeof *x);
      for (i=0; i<machines; i++){
        x[i] = malloc(jobs * sizeof *x[i]);
      }

      for(j=0;j<jobs;j++){

            for(i=0;i<a;i++){

                s1=s1+(a-i)*problem[i][j];
            }
            x[0][j]=s1; /* fill the first rows of johnson'array */
            s1=0;

            for(i=a;i<machines;i++){

                s2=s2+(b-n)*problem[i][j];
                n--;
            }
            x[1][j]=s2;
            s2=0; n=b-1;

      }

      ///Step5: Apply Johnson's Rule to find the best sequence

    int sequence[jobs];
    johnsonRule(jobs,sequence);

      /** display the optimal sequence **/
      /*  printf("\n__________The optimal jobs sequence is:__________\n");
        for(i=0;i<jobs;i++) {
            printf("%d | ",sequence[i]+1);
        }*/


   ///Step6: Calculate the make-span time for the sequence obtained in step5

        /** this array contains the time of ending of each task in each machine **/
        int schedule[machines][jobs]; /* we use it to calculate the make-span it represents the termination time of each job on each machine */

        for(i=0;i<machines;i++){
            for(j=0;j<jobs;j++){

                if(i==0 && j==0) schedule[i][j]=problem[i][sequence[j]];
                if(i==0 && j!=0) schedule[i][j]=schedule[i][j-1]+problem[i][sequence[j]];
                if(i!=0 && j==0) schedule[i][j]=schedule[i-1][j]+problem[i][sequence[0]];
                if(i!=0 && j!=0) schedule[i][j]=maximun(schedule[i][j-1],schedule[i-1][j])+problem[i][sequence[j]];

            }

        }

        /** display schedule array **/
        /*printf("\n__________Schedule array:__________ \n");
        for(i=0;i<machines;i++){
            for(j=0;j<jobs;j++){
                printf("%f  |",schedule[i][j]);
            }
            printf("\n");
        }*/
        int makespan=schedule[machines-1][jobs-1];
        /** display the optimal make-span and the optimal sequence**/
         //printf("\n__________The optimal jobs sequence is:__________\n");
        for(i=0;i<jobs;i++) {
            printf("%d,",sequence[i]+1);
        }
        printf("%d\n",makespan);


  /* free memory space */
  /*free(x);
  free(problem);
  free(schedule);*/


}
