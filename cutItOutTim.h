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

int minRun = 32;
int minGallop = 8;
int sorting = 0;

//file for testing different aspects of timSort before putting it all together
int timSort(int seal[], int size){
    
    int runOne = 0; 
    int startOne = 0;
    int runTwo = 0; 
    int startTwo = 0;
    int runThree = 0; 
    int startThree = 0;
    int newStart = 0;

    bool init = false;

    if(size <= 64){
        insertionSort(seal, 0, size);
        return 1;
    }

   // cout << "\n";
    while(sorting < size){
    //for(int k = 0; k < size; ++k)
     //   cout << seal[k] << " ";
    //cout << "\n";
 //   cout << "sorting: " << sorting << "\tsize: " << size << "\n"; 
        if(!init){
            for(int k = 0; k < 3; ++k){
                if(sorting == size){
                    break;
                }

                if(k == 0){
                runOne = grabArray(seal, sorting, size);
                startOne = sorting;
                sorting += runOne;
            //    cout << "\nsorting: " << sorting << "\n";
            //    for(int j = startOne; j < runOne + startOne; ++j)
             //       cout << seal[j] << " ";
              //  cout << "\n";
                }
                else if(k == 1){
                runTwo = grabArray(seal, sorting, size);
                startTwo = sorting;
                sorting += runTwo;
             //   cout << "\nsorting: " << sorting << "\n";
          //      for(int j = startTwo; j < runTwo + startTwo; ++j)
           //         cout << seal[j] << " ";
            //    cout << "\n";
                }
                else{
                runThree = grabArray(seal, sorting, size);
                startThree = sorting;
                sorting += runThree;
  //              cout << "\nsorting: " << sorting << "\n";
         //       for(int j = startThree; j < runThree + startThree; ++j)
          //          cout << seal[j] << " ";
           //     cout << "\n";
                }
            }
            init = true;
        if(sorting == size && runTwo == 0){
            sorting = 0;
            return 1;
        }
        }

        else{
            runThree = grabArray(seal, sorting, size);
            startThree = sorting;
            sorting += runThree;
   //             cout << "\nsorting: " << sorting << "\n";
      //      for(int j = startThree; j < runThree + startThree; ++j)
      //          cout << seal[j] << " ";
      //      cout << "\n";
        }

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
            runTwo = runThree;
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

    if(runOne <= runTwo){
       // cout << "does it happen here\n";
        newStart = mergeLow(seal, runOne, startOne, runTwo, startTwo);
    } 
    else{
    //    cout << "or here\n";
     //           cout << "\nsorting: " << sorting << "\n";
        newStart = mergeHigh(seal, runOne, startOne, runTwo, startTwo);
    }
//    for(int k = 0; k < size; ++k)
 //       cout << seal[k] << " ";
    sorting = 0;
return 0;
}


int mergeLow(int seal[], int firstLength, int firstStart, int secondLength, int secondStart){
     //   cout << "mergeLow: " << "firstLength: " << firstLength << "secondLength: " << secondLength << "\n";
     //   cout << "mergeLow: " << " firststart: " << firstStart << " secondStart: " << secondStart << "\n";
 //   for(int k = 0; k < 32; ++k)
 //       cout << seal[k] << " ";
 //   cout << "\n";
    int buffCount = 0;
    int firstPace = 0;
    int secondPace = 0;
    int trackOne = firstStart;
    int trackTwo = secondStart;
    int* buffer = new int[firstLength];
    bool merging = true;
    for(int k = 0; k < firstLength; ++k){
        buffer[k] = seal[trackOne];
        ++trackOne;
    }
    trackOne = firstStart;
    while(merging){
       // cout << "trackOne: " << trackOne << "1length+1start: " << firstLength+firstStart << "trackTwo: " << trackTwo << "\n";
        if(buffCount == firstLength){
  //          cout << "\nbuffCount == firstLenght\n";
            //copy the rest of second array into seal
            //and break from the while loop
            for(int i = trackTwo; i < secondLength + secondStart; ++i){
   //             cout << seal[i] << " ";
                seal[trackOne] = seal[i];
                ++trackOne;
            }
            merging = false;
            break;
        }
        if(trackTwo == secondLength + secondStart){
//            cout << "\ntrackTwo == secondLenght + start\n";
            //copy the rest of buffer array into seal
            //and break from the while loop
            for(int i = buffCount; i < firstLength; ++i){
//                cout << buffer[i] << " ";
                seal[trackOne] = buffer[i];
                ++trackOne;
            }
            merging = false;
            break;
        }
            
        if(buffer[buffCount] < seal[trackTwo]){
            seal[trackOne] = buffer[buffCount];
//            cout << buffer[buffCount] << " ";
            ++firstPace;
            ++buffCount;
            ++trackOne;
            secondPace = 0;
        }
        else{
            seal[trackOne] = seal[trackTwo];
//            cout << seal[trackTwo] << " ";
            ++secondPace;
            ++trackTwo;
            ++trackOne;
            firstPace = 0;
        }
            
    }
//    cout << "\n";
    delete[] buffer;
return firstStart;
}

int mergeHigh(int seal[], int firstLength, int firstStart, int secondLength, int secondStart){
    //    cout << "mergeHigh: " << "firstLength: " << firstLength << "secondLength: " << secondLength << "\n";
    //    cout << "mergeHigh: " << " firststart: " << firstStart << " secondStart: " << secondStart << "\n";
//    for(int k = 0; k < 32; ++k)
//        cout << seal[k] << " ";
//    cout << "\n";
    int buffCount = secondLength - 1;
    int firstPace = 0;
    int secondPace = 0;
    int trackOne = firstStart + firstLength - 1;
    int trackTwo = secondStart;
    int* buffer = new int[secondLength];
    bool merging = true;
    for(int k = 0; k < secondLength; ++k){
        buffer[k] = seal[trackTwo];
        ++trackTwo;
    }
    trackTwo = secondStart + secondLength - 1;

    while(merging){
        //cout << "trackOne: " << trackOne << "buffCount: " << buffCount << "trackTwo: " << trackTwo << "\n";
        if(buffCount < 0){
//            cout << "\nbuffCount < 0\n";
            //copy the rest of second array into seal
            //and break from the while loop
            for(int i = trackOne; i >= firstStart; --i){
                seal[trackTwo] = seal[i];
//                cout << seal[i] << " ";
                --trackTwo;
            }
            merging = false;
        }
        if(trackOne < firstStart){
//            cout << "\ntrackOne < firstStart\n";
            //copy the rest of buffer array into seal
            //and break from the while loop
            for(int i = buffCount; i >= 0; --i){
                seal[trackTwo] = buffer[i];
//                cout << buffer[i] << " ";
                --trackTwo;
            }
            merging = false;
        }
            
        if(buffer[buffCount] >= seal[trackOne]){
            seal[trackTwo] = buffer[buffCount];
//            cout << buffer[buffCount] << " ";
            ++firstPace;
            --buffCount;
            --trackTwo;
            secondPace = 0;
        }
        else{
            seal[trackTwo] = seal[trackOne];
//            cout << seal[trackOne] << " ";
            ++secondPace;
            --trackTwo;
            --trackOne;
            firstPace = 0;
        }
            
    }
//    cout << "\n";
    delete[] buffer;
return firstStart;
}
//random data gives runs less than minRun so tack on 
        //moving right on the array to make merges equal
int grabArray(int seal[], int start, int size){
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

return run;
}

int reverseOrder(int seal[], int start, int end){
    int mid = (end - start) / 2;
    int buffer = 0;
    --end;
    int checkOne = start;
    int checkTwo = end;
    //cout << "\nfirst ele: " << seal[checkOne] << "\tlast ele: " << seal[checkTwo] << "\n";

    for(int k = 0; k < mid; ++k){
        buffer = seal[start];
        seal[start] = seal[end];
        seal[end] = buffer;
        ++start;
        --end;
    }
     //   cout << "\nfirst ele: " << seal[checkOne] << "\tlast ele: " << seal[checkTwo] << "\n";
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
   // cout << "\n in descender \trun: " << run << "\n";
return run;
}
