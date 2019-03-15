#include "sortaLibby.h"


//testing environment for the different sorting algorithms
//writing duration of sort results to a file

//9 different types of data will be tested on:
//ascending
//descending
//random
//all equal
//85% equal
//organ pipes
//just 0's and 1's
//01010101...
//0-4 random

//size of data set is increased on each iteration by a power of 2
//so maxPower is the largest dataset at 2^24 this can be toggled
int maxPower = 20;
//tests represent the number of times a particular data set and
//size is run. allowing you to grab the average
int tests = 2;

int main(){

    int count = 0;
    int size = 0;

    //for each particular kind of data set
    for(int dataSet = 0; dataSet < 3; ++dataSet){
        //for increasing sizes of arrays
        for(int k = 8; k < maxPower; ++k){
            size = pow(2,k);
            cout << "dataset: " << dataSet << "\n";
            cout << "size: " << size << "\n";
            count = 0;
            //now that dataset and size are picked run a test 10 times
            while(count < tests){
                cout << "\n" << count << "\n";

                //if size of array is too small sort 100 times in a row to increase resolution
                if(size < 100000){
                    int** address = new int*[100];
                    //populate 100 arrays to be sorted
                    populateArrays(address, size, dataSet);
                    sortAndStore(address, size, dataSet);
                    
                for(int j = 0; j < 100; ++j)
                        delete[] address[j];

                    delete[] address;
                }

            else{
                int* address = new int[size];
                populateArray(address, size, dataSet);
                sortAndStore(address, size, dataSet);

                delete[] address;
                }
            ++count;
            }     
        }
    }
return 0;
}