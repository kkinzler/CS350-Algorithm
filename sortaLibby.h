//library of functions for testing algorithms

#include <stdio.h>
#include <stdlib.h>
#include <ctime>
#include <chrono>
#include <cmath>
#include <cstdlib>
#include <iostream>
#include <algorithm>
#include <fstream>
#include "cutItOutTim.h"
//#include "gallopingTim.h"

using namespace std;

int getRando(int range){
    //seeding rand() doesn't do any good when the
    //function is called multiple times a second.
    //every array will be built with the same random
    //numbers but it should make no difference to the
    //sorting algorithm.
    int rando = rand();
    rando = rando % range;

return rando;
}

int ascending(int size, int address[]){
    for(int t = 0; t <size; ++t)
        address[t] = t;
return 1;
}

int random(int size, int address[]){
    ascending(size, address);
    random_shuffle(&address[0], &address[size-1]);
return 1;
}

int descending(int size, int address[]){
    int j = 0;
    for(int t = size-1; t >= 0; --t){
        address[j] = t;
        ++j;
    }
return 1;
}

int sevenSixSeven(int size, int address[]){
    for(int t = 0; t <size; ++t)
        address[t] = 767;
return 1;
}

int eightyFiveRando(int size, int address[]){
    int count = int(size * .08);
    int randomFirst = 0;
    int randomSecond = 1;
    int buffer = 0;

    //could just create a swap function to clean things up...
    for(int t = 0; t < count; ++t){
        //get two random numbers in range(size)
        //swap them
        randomFirst = getRando(size);
        randomSecond = getRando(size);        
        buffer = address[randomFirst];
        address[randomFirst] = address[randomSecond];
        address[randomSecond] = buffer;
    }

return 1;
}


int oneZeroEtc(int size, int address[]){
    int number = 0;
    for(int t = 0; t <size; ++t){
        if(number == 0){
            address[t] = 0;
            number = 1;
        }
        else{
            address[t] = 1;
            number = 0;
        }
    }

return 1;
}

int oneZeroRando(int size, int address[]){
    int rando = 0;
    
    for(int g = 0; g < size; ++g){
        rando = getRando(2);
        address[g] = rando;
    }
return 1;
}

int zeroToFourRando(int size, int address[]){
    int rando = 0;
    
    for(int g = 0; g < size; ++g){
        rando = getRando(5);
        address[g] = rando;
    }
return 1;
}

int organPipes(int size, int address[]){
    int pipe = 64;
    if(size < 64)
        pipe = 4;

    for(int g = 0; g < size; g += pipe*2){
        for(int b = 0; b < pipe; ++b)
            address[g+b] = b;

        int count = 0;
        for(int j = pipe-1; j >= 0; --j){
            address[g+pipe+count] = j;
            ++count;
        }
    }
return 1;
}

//write size, duration, dataSet... to file.
//you need to name file based on which algorithm you are testing
int writeToFile(int size, double duration, int dataSet){

    std::ofstream outfile;
    //REPLACE THE FILE NAME TO SOMETHING THAT MAKES SENSE
    outfile.open("timSort.csv", std::ios_base::app);
    outfile << size << "," << duration << "," << dataSet << "\n"; 

return 1;
}

int populateArrays(int** address, int size, int dataSet){
    //create each new array from array of arrays
    for(int p = 0; p < 100; ++p)
        address[p] = new int[size];

    switch(dataSet) {
        case 0 : //random
                for(int p = 0; p < 100; ++p)
                    random(size, address[p]);
                break;
        case 1 : //ascending
                for(int p = 0; p < 100; ++p)
                    ascending(size, address[p]);
                break;
        case 2 : //descending
                for(int p = 0; p < 100; ++p)
                    descending(size, address[p]);
                break;
        case 3 : //all equal
                for(int p = 0; p < 100; ++p)
                    sevenSixSeven(size, address[p]);
                break;
        case 4 : //85% equal
                for(int p = 0; p < 100; ++p)
                    eightyFiveRando(size, address[p]);
        case 5 : //organ pipes
                for(int p = 0; p < 100; ++p)
                    organPipes(size, address[p]);
                break;
        case 6 : //just 0's and 1's
                for(int p = 0; p < 100; ++p)
                    oneZeroRando(size, address[p]);
                break;
        case 7 : //101010101...
                for(int p = 0; p < 100; ++p)
                    oneZeroEtc(size, address[p]);
                break;
        case 8 : //0-4 random
                for(int p = 0; p < 100; ++p)
                    zeroToFourRando(size, address[p]);
                break;
    }

return 1;
}

int populateArray(int* address, int size, int dataSet){

    switch(dataSet) {
        case 0 : //random
                random(size, address);
                break;
        case 1 : //ascending
                ascending(size, address);
                break;
        case 2 : //descending
                descending(size, address);
                break;
        case 3 : //all equal
                sevenSixSeven(size, address);
                break;
        case 4 : //85% equal
                eightyFiveRando(size, address);
                break;
        case 5 : //organ pipes
                organPipes(size, address);
                break;
        case 6 : //just 0's and 1's
                oneZeroRando(size, address);
                break;
        case 7 : //101010101...
                oneZeroEtc(size, address);
                break;
        case 8 : //0-4 random
                zeroToFourRando(size, address);
                break;
    }
return 1;
}

int sortAndStore(int** address, int size, int dataSet){
    //time the sort
    auto start = std::chrono::system_clock::now();
    for(int k = 0; k < 100; ++k){
            //cout << "\ntest: " << k << "\n";
        //REPLACE THIS SORTING ALGORITHM WITH YOUR OWN
        timSort(&address[k][0], size);
        //gallopingTimSort(&address[k][0], size);
        //sort(&address[k][0], &address[k][size]);
    }
    auto end = std::chrono::system_clock::now();

    std::chrono::duration<double> elapsed_seconds = end-start;
    double duration = elapsed_seconds.count()/100;
    cout << "elapsed time: " << duration << "s\n";

    writeToFile(size, duration, dataSet);    

return 1;
}

int sortAndStore(int* address, int size, int dataSet){
    //time the sort
    auto start = std::chrono::system_clock::now();
    //REPLACE THIS SORTING ALGORITHM WITH YOUR OWN
    //sort(&address[0], &address[size]);
    //insertionSort(&address[0], 0, size - 1);
    timSort(&address[0], size);
    //gallopingTimSort(&address[0], size);
    auto end = std::chrono::system_clock::now();

    std::chrono::duration<double> elapsed_seconds = end-start;
    double duration = elapsed_seconds.count();
    cout << "elapsed time: " << duration << "s\n";

    writeToFile(size, duration, dataSet);    

return 1;
} 