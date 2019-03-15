//For you consideration...Tim Sort

#include <iostream>
using namespace std;

int ascend(int seal[], int start, int end);
int descend(int seal[], int start, int end);
int reverseOrder(int seal[], int start, int end);
void insertionSort(int arr[], int first, int last);
int defineRun(int seal[], int start, int end);
int grabArray(int seal[], int start, int end);
int mergeLow(int seal[], int firstLength, int firstStart, int secondLength, int secondStart);
int mergeHigh(int seal[], int firstLength, int firstStart, int secondLength, int secondStart);
int gallopForward(int buff[], int buffCount, int station, int buffSize);
int gallopBackward(int buff[], int buffCount, int station, int buffSize);

int minRun = 32;//no runs less than 32
int minGallop = 8;//after selecting 8 consecutive times enter gallop mode
int sorting = 0;//used through out to check bounds of array

//file for testing different aspects of timSort before putting it all together
int gallopingTimSort(int seal[], int size){

    int runOne = 0; 
    int startOne = 0;
    int runTwo = 0; 
    int startTwo = 0;
    int runThree = 0; 
    int startThree = 0;
    int newStart = 0;

    bool init = false;

    //step one: if array less than 64 skip tim sort
    if(size <= 64){
        insertionSort(seal, 0, size);
        return 1;
    }

    //while you haven't reached the end of the array
    while(sorting < size){
        //step two: grab three runs to get started
        if(!init){
            for(int k = 0; k < 3; ++k){
                if(sorting == size){
                    break;
                }
                //certainly could make this more readable
                if(k == 0){
                runOne = grabArray(seal, sorting, size);
                startOne = sorting;
                sorting += runOne;
                }
                else if(k == 1){
                runTwo = grabArray(seal, sorting, size);
                startTwo = sorting;
                sorting += runTwo;
                }
                else{
                runThree = grabArray(seal, sorting, size);
                startThree = sorting;
                sorting += runThree;
                }
            }
            //when init is true you won't initiate 3 runs again
            //grab one at a time from now on
            init = true;

            //simple check to see if you only grabbed one run
            //must have already been in order!!!
            if(sorting == size && runTwo == 0){
                sorting = 0;
                return 1;
            }
        }

        //after initializing runs this is where the while loop starts
        //on each iteration. grab a new run.
        else{
            runThree = grabArray(seal, sorting, size);
            startThree = sorting;
            sorting += runThree;
        }

    //once you have a new run you need to check which 2 of the 
    //3 runs you need to merge together. important to merge
    //balanced runs as much as you can

    //if necessary give run 1 and 2 new start and length values
    if(runOne >= runTwo + runThree){
        if(runTwo <= runThree)
            newStart = mergeLow(seal, runTwo, startTwo, runThree, startThree);
        else{
            newStart = mergeHigh(seal, runTwo, startTwo, runThree, startThree);
        }
        runTwo = runTwo + runThree;
        startTwo = newStart;
    }
    else{
        if(runOne < runThree){
            if(runOne <= runTwo)
                newStart = mergeLow(seal, runOne, startOne, runTwo, startTwo);
            else
                newStart = mergeHigh(seal, runOne, startOne, runTwo, startTwo);
            runOne = runOne + runTwo;
            startOne = newStart;
            runTwo = runThree;//free up runThree to grab the next run
            startTwo = startThree;
        }
        else{
            if(runTwo <= runThree){
                newStart = mergeLow(seal, runTwo, startTwo, runThree, startThree);
            }
            else{
                newStart = mergeHigh(seal, runTwo, startTwo, runThree, startThree);
            }
            runTwo = runTwo + runThree;
            startTwo = newStart;
        }
    }

    }

    //great job!!! all done but to tie up loose ends.
    //make sure the last two runs are merged
    if(runOne <= runTwo){
        newStart = mergeLow(seal, runOne, startOne, runTwo, startTwo);
    } 
    else{
        newStart = mergeHigh(seal, runOne, startOne, runTwo, startTwo);
    }

    //sorting is a global. reset or you'll be timing 
    //how fast you can NOT sort an array
    sorting = 0;
return 1;
}

//mergeLow and mergeHigh are mirror images of one another
//essentially merge sort with galloping
int mergeLow(int seal[], int firstLength, int firstStart, int secondLength, int secondStart){

    int buffCount = 0;
    int firstPace = 0;
    int secondPace = 0;
    int trackOne = firstStart;
    int trackTwo = secondStart;
    int gallop = 0;
    int copy = 0;
    int* buffer = new int[firstLength];
    bool merging = true;

    //copy shorter run into a buffer for sorting
    for(int k = 0; k < firstLength; ++k){
        buffer[k] = seal[trackOne];
        ++trackOne;
    }
    trackOne = firstStart;

    while(merging){
        //if you've copied all elements from shorter array copy the 
        //rest of the longer one and exit
        if(buffCount == firstLength){
            for(int i = trackTwo; i < secondLength + secondStart; ++i){
                seal[trackOne] = seal[i];
                ++trackOne;
            }
            merging = false;
            break;
        }
        //if you've copied all elements from longer array copy the 
        //rest of the shorter one and exit
        if(trackTwo == secondLength + secondStart){
            for(int i = buffCount; i < firstLength; ++i){
                seal[trackOne] = buffer[i];
                ++trackOne;
            }
            merging = false;
            break;
        }
        
        //classic "which number is larger" check
        if(buffer[buffCount] < seal[trackTwo]){
            seal[trackOne] = buffer[buffCount];
            ++firstPace;
            ++buffCount;
            ++trackOne;
            secondPace = 0;
            if(firstPace == minGallop){
                //!!!minGallop has been reached. start galloping through!!!
                //!!!run to find the next placement of element in buffer!!!
                gallop = gallopForward(buffer, buffCount, seal[trackTwo], firstLength);
                copy = gallop - buffCount;
                //copy over all elements up to the point found in gallopForward 
                for(int i = 0; i < copy; ++i){
                    seal[trackOne] = buffer[buffCount];
                    ++buffCount;
                    ++trackOne;
                    firstPace = 0;
                }
            }
        }
        else{
            seal[trackOne] = seal[trackTwo];
            ++secondPace;
            ++trackTwo;
            ++trackOne;
            firstPace = 0;
            if(secondPace == minGallop){
                //!!!minGallop has been reached. start galloping through!!!
                //!!!run to find the next placement of element in seal  !!! 
                gallop = gallopForward(seal, trackTwo, buffer[buffCount], secondStart + secondLength);
                copy = gallop - trackTwo;
                //copy over all elements up to the point found in gallopForward 
                for(int i = 0; i < copy; ++i){
                    seal[trackOne] = buffer[trackTwo];
                    ++trackOne;
                    ++trackTwo;
                    secondPace = 0;
                }
            }   
        }
            
    }
    delete[] buffer;
return firstStart;
}


int mergeHigh(int seal[], int firstLength, int firstStart, int secondLength, int secondStart){

    int buffCount = secondLength - 1;
    int firstPace = 0;
    int secondPace = 0;
    int trackOne = firstStart + firstLength - 1;
    int trackTwo = secondStart;
    int gallop = 0;
    int copy = 0;
    int* buffer = new int[secondLength];
    bool merging = true;
    for(int k = 0; k < secondLength; ++k){
        buffer[k] = seal[trackTwo];
        ++trackTwo;
    }
    trackTwo = secondStart + secondLength - 1;

    while(merging){
        if(buffCount < 0){
            for(int i = trackOne; i >= firstStart; --i){
                seal[trackTwo] = seal[i];
                --trackTwo;
            }
            merging = false;
        }
        if(trackOne < firstStart){
            for(int i = buffCount; i >= 0; --i){
                seal[trackTwo] = buffer[i];
                --trackTwo;
            }
            merging = false;
        }
            
        if(buffer[buffCount] >= seal[trackOne]){
            seal[trackTwo] = buffer[buffCount];
            ++firstPace;
            --buffCount;
            --trackTwo;
            secondPace = 0;
            if(firstPace == minGallop){
                gallop = gallopBackward(buffer, buffCount, seal[trackTwo], firstLength);
                copy = buffCount - gallop;
                for(int j = 0; j < copy; ++j){
                    seal[trackOne] = buffer[buffCount];
                    --buffCount;
                    --trackOne;
                }
                firstPace = 0;
            }

        }
        else{
            seal[trackTwo] = seal[trackOne];
            ++secondPace;
            --trackTwo;
            --trackOne;
            firstPace = 0;
            if(firstPace == minGallop){
                gallop = gallopBackward(seal, trackTwo, buffer[buffCount], secondStart + secondLength);
                copy = trackTwo - gallop;
                for(int i = 0; i < copy; ++i){
                    seal[trackOne] = buffer[trackTwo];
                    --trackOne;
                    ++trackTwo;
                }
                secondPace = 0;
            }
        }
            
    }
    delete[] buffer;
return firstStart;
}
//random data gives runs less than minRun so tack on 
//moving right on the array to make merges equal
int grabArray(int seal[], int start, int size){

        //how long is the run
        int run = defineRun(seal, start, size);

        if(minRun > run){
           //if run < minRun but not bc end of array was reached
            if(start + run < size){
                run = minRun;
                insertionSort(seal, start, start + run);
            }
            //run ran into the end of the list so just
            //sort what you have so its ready to merge 
            else
                insertionSort(seal, start, start + run);
        }

        //run is in descending order so swap both ends
        if(seal[start] > seal[start+1])
            reverseOrder(seal, start, start + run);
//returnn length of run
return run;
}

//simple reverse by swapping ends 
int reverseOrder(int seal[], int start, int end){
    int mid = (end - start) / 2;
    int buffer = 0;
    --end;

    for(int k = 0; k < mid; ++k){
        buffer = seal[start];
        seal[start] = seal[end];
        seal[end] = buffer;
        ++start;
        --end;
    }

return 0;
}

//insertionSort
void insertionSort(int arr[], int first, int last){ 
    for (int t = first + 1; t <last; t++) 
    { 
        int buffer = arr[t]; 
        int traveler = t - 1; 
        while (arr[traveler] > buffer && traveler >= first) 
        { 
            arr[traveler+1] = arr[traveler]; 
            --traveler; 
        } 
        arr[traveler + 1] = buffer; 
    } 
} 

//find length of run. it can be ascending or
//descending
int defineRun(int seal[], int start, int end){
    int run = 0;
    if(start <= end)
        return run;

    if(seal[start] < seal[start+1])
        run = ascend(seal, start, end);
    else
        run = descend(seal, start, end);
return run;
}

//find length of ascending run
int ascend(int seal[], int start, int end){
    int run = 1;
    int a = start;
    int b = start+1;
    if(b >= end)
        return run;

    int first = seal[a];
    int second = seal[b];
    while(first <= second && b < end){
        ++run;
        ++a;
        ++b;
        first = seal[a];
        second = seal[b];
    }
return run;
}

//find length of descending run
int descend(int seal[], int start, int end){
    int run = 1;
    int a = start;
    int b = start + 1;
    if(b >= end)
        return run;

    int first = seal[a];
    int second = seal[b];

    while(first > second && b < end){
        ++run;
        ++a;
        ++b;
        first = seal[a];
        second = seal[b];
    }
return run;
}

//search array incrementing k in 2^k to search faster
//and faster through the array. return spot to copy up to
//gallopFoward is a mirror image of gallopBackward
int gallopForward(int buff[], int buffCount, int station, int buffSize){
    int k = 0;
    int traveler = buff[buffCount];
    if(traveler > station)
        return buffCount;
    //traveler travels through array for the station at first
    //traveler runs right passed the station.
    while(traveler <= station && buffCount < buffSize){
        buffCount = buffCount + pow(2, k);
        traveler = buff[buffCount];
        ++k;
    }
    //then traveler backs up to far
    buffCount = buffCount - pow(2, k - 1);
    traveler = buff[buffCount];
    //slowly traveler walks forward as to not miss the station
    while(traveler <= station && buffCount < buffSize){
        ++buffCount;
        traveler = buff[buffCount];
    }

    --buffCount;
return buffCount;
}

int gallopBackward(int buff[], int buffCount, int station, int buffSize){

    int k = 0;
    int traveler = buff[buffCount];
    if(traveler < station)
        return buffCount;
    while(traveler > station && buffCount >= buffSize){
        buffCount = buffCount - pow(2, k);
        traveler = buff[buffCount];
        ++k;
    }

    buffCount = buffCount - pow(2, k - 1);
    traveler = buff[buffCount];
    while(traveler > station && buffCount >= buffSize){
        --buffCount;
        traveler = buff[buffCount];
    }
    ++buffCount;
return buffCount;
}