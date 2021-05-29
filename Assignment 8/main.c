#include <stdio.h>
#include <stdlib.h> 
#include <math.h>
#include <time.h>

double max(double a, double b);
double min(double a, double b);

int main(int argc, char ** argv){
    if(argc < 15){
        printf("Enough arguments not provided.\n");
        exit (0);
    }

    double ki=atof(argv[2]);               //Taking the command line arguements
    double km=atof(argv[4]);
    double kn=atof(argv[6]);
    double kf=atof(argv[8]);
    double ps=atof(argv[10]);
    int t=atoi(argv[12]);                   //total number of packets to be sent
    char * outfile=argv[14];

    // double ki=1;               //Taking the command line arguements
    // double km=1;
    // double kn=1;
    // double kf=0.2;
    // double ps=0.01;
    // int t=200;                   //total number of packets to be sent
    // char * outfile="output.txt";

    FILE * file_ptr= fopen(outfile, "w");   //Opening the write file
    int i=1;                                //This variable is the counter for CW value.
    int MSS=1;                              //Maximum segment size as 1KB
    double CW=ki*MSS;  
    int threshold=t/4;                  //Threshold Value initialized to t/4 initially 
                                        //because we need some high value to start the slow start/exponential phase       
    int RWS=1024;                       //Reciever Window Size is initialized to 1024 KB
    int sent=0;                         //Initializing the total number of sent packets to 0
    
    CW=ki*MSS;                   //Calculating the initial size of CW
    fprintf(file_ptr, "%d %lf\n", i, CW);
    // printf("%d %lf -Initial\n", i, CW);
    i++;

    while(sent < t){                    //Outer While Loop, runs until all packets are sent
        
        int N = CW/MSS + 1;           //Calculating the number of segments to be transmitted(ceil)
        int segment_count=0;            //Initializing segment count to be 0

        srand(time(NULL));
        while(segment_count<N){         //Inner for loop runs, while all segments in the CW are sent
            int r = rand();             //generating a random int number 

            //segment is lost/Congestion detection
            if(r < ps*RAND_MAX){        //multiplying ps*with RAND_MAX to make the range same, and checking if the packet is lost
                segment_count=0;        //resetting segment_count
                threshold=CW/2;         //recalculating the threshold values
                CW=max(1, kf*CW);       //multiplicative decrease
                fprintf(file_ptr, "%d %lf\n", i, CW);
                // printf("%d %lf -Multiplicative Decrease\n", i, CW);
                i++;
                continue;
            }

//Doubts: Congestion is not dependent on the receiver window size or something
//How to plot 5 plots in 1 graph

            //packet is not lost
            else{
                if(CW<threshold){       //exponential increase, slow start phase.
                    segment_count++;
                    CW=min(CW + km*MSS, RWS);
                    fprintf(file_ptr, "%d %lf\n", i, CW);
                    // printf("%d %lf -Exponential Increase\n", i, CW);
                    i++;
                }
                else{                   //Congestion avoidance phase, Linear increase
                    segment_count++;
                    CW=min(CW + kn*MSS*MSS/CW, RWS);
                    fprintf(file_ptr, "%d %lf\n", i, CW);
                    // printf("%d %lf -Linear Increase\n", i, CW);
                    i++;
                }
            }
        } 
        sent+=N;
    }
    fclose(file_ptr);
    return 0;
}

double max(double a, double b){
    if(a<b){
        return b;
    }
    else{
        return a;
    }
}

double min(double a, double b){
    if(a<b){
        return a;
    }
    else{
        return b;
    }
}