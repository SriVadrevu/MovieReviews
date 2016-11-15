
//  CS 201 - Theys - Project 1

// Goal: To create a statistical calculator by analyzing data of various individual entities (records)
// available in an array format from a file.
//  Created by Sri Vadrevu on 9/09/16.
//  Copyright © 2016 SriVadrevu. All rights reserved.
// Written in xCode.

#include <stdio.h>
#include <stdlib.h>

struct statData {
    float avg;
    float median;
    float max;
    float min;
};

struct metaData {
    char recordIdentifier[20];
    float numOfNonZero;
    float percentNonZero;
};

int main() {
    
    //--------------------------------------
    // OPENING FILE + CHECKING IF IT EXISTS
    //--------------------------------------
    char fileName[20];
    printf("What is the name of the file?\n");
    scanf("%s", fileName);
    
    FILE * arrayFile = fopen(fileName, "r");
    if(arrayFile == 0) {
        perror("fopen");
        exit(1);
    }

    //----------------------------------------------------------------------
    // READING FIRST TWO LINES OF FILE TO DECIDE HOW MUCH SPACE TO ALLOCATE
    //----------------------------------------------------------------------
    int numRecords;
    int numValuesPerRecord;
    
    fscanf(arrayFile, "%i", &numRecords);
    fscanf(arrayFile, "%i", &numValuesPerRecord);

    float dataArray[numRecords][numValuesPerRecord];
    struct statData statsOfRecords[numValuesPerRecord];
    struct metaData recordInfo[numRecords];
    
    int i;
    int k;
    
    //--------------------------------------------------------------------
    //  READING IN OUR DATA INTO OUR ARRAY + NAMES OF RECORDS INTO STRUCT
    //--------------------------------------------------------------------

    printf("\nThis prints out everything we have just scanned from file\n");
    for (i=0; i<numRecords; i++){
        fscanf(arrayFile, "%s", (recordInfo+i)->recordIdentifier);
        printf("%-12s ", (recordInfo+i)->recordIdentifier);
        for (k=0; k<numValuesPerRecord; k++) {
            fscanf(arrayFile, "%f", *(dataArray+i)+k);
            printf("%.2f", *(*(dataArray+i)+k));
            printf("<%i,%i>  ", i,k);
        }
        printf("\n");
    }

    
    printf("\n\n");
    
    //--------------------
    //GETTING RECORD INFO
    //--------------------
    for (i=0; i<numRecords; i++) {
        (recordInfo+i)->numOfNonZero = numValuesPerRecord;
        for (k=0; k<numValuesPerRecord; k++) {
            if (*(*(dataArray+i)+k) == 0) {
                (recordInfo+i)->numOfNonZero--;
            }
        }
        (recordInfo+i)->percentNonZero = (recordInfo+i)->numOfNonZero / numValuesPerRecord;
        (recordInfo+i)->percentNonZero *= 100;
    }
    
    //--------------
    // FINDING MEAN
    //--------------
    int numZeroes;
    
    for (k=0; k<numValuesPerRecord; k++) {
        numZeroes = 0;
        (statsOfRecords+k)->avg = 0;
        for (i=0; i<numRecords; i++) {
            if (*(*(dataArray+i)+k) != 0) {
                (statsOfRecords+k)->avg += *(*(dataArray+i)+k);
            }
            else {
                numZeroes++;
            }
        }
        (statsOfRecords+k)->avg /= (numRecords-numZeroes);
    }
   
    //----------------
    // FINDING MEDIAN
    //----------------
    int min;
    int j;
    float tmp;

    // Let's sort all columns of array using selection sort
    printf("Here is our array from file sorted by columns, from least to greatest\n");
    for (k=0; k<numValuesPerRecord; k++) {
        for (i=0; i<numRecords; i++) {
            min = i;
            for(j=i;j<numRecords;j++){
                if (*(*(dataArray+j)+k)<*(*(dataArray+min)+k))
                    min =j;
            }
            // swap position min and position i
            tmp = *(*(dataArray+i)+k);
            *(*(dataArray+i)+k) = *(*(dataArray+min)+k);
            *(*(dataArray+min)+k) = tmp;
        }
        
    }
    
    // Let's print our sorted array to see if it sorted properly
    for (i=0; i<numRecords; i++){
        fscanf(arrayFile, "%s", &(recordInfo+i)->recordIdentifier[0]);
        printf("%-12s ", (recordInfo+i)->recordIdentifier);
        for (k=0; k<numValuesPerRecord; k++) {
            fscanf(arrayFile, "%f", &*(*(dataArray+i)+k));
            printf("%.2f", *(*(dataArray+i)+k));
            printf("<%i,%i>  ", i,k);
        }
        printf("\n");
    }
    
    // Let's count the number of non-zero values in each column
    int nonZeroesColumn[numValuesPerRecord];
    
    for (k=0; k<numValuesPerRecord; k++) {
        *(nonZeroesColumn+k) = numRecords;
        for (i=0; i<numRecords; i++) {
            if (*(*(dataArray+i)+k) == 0) {
                *(nonZeroesColumn+k) = *(nonZeroesColumn+k) - 1;
            }
        }
    }
    
    // Now since we've known that, let's increment each column to the row where the
    // median is, keeping in mind the number of total non-zero values we have.
    int looper = 0;
    
    for (k=0; k<numValuesPerRecord; k++) {
        if ((nonZeroesColumn[k] % 2) == 0 ) {
            i=0;
            looper = (nonZeroesColumn[k]/2) - 1;
            
            while (looper != 0) {
                if (*(*(dataArray+i)+k) != 0) {
                    looper--;
                }
                i++;
            }
        
            looper=0;
            (statsOfRecords+k)->median = *(*(dataArray+i)+k);
            
            i++;
            while(*(*(dataArray+i)+k) == 0) {
                i++;
            }
           
            (statsOfRecords+k)->median += *(*(dataArray+i)+k);
            (statsOfRecords+k)->median /= 2.0;
        }
        else {
            i = nonZeroesColumn[k]/2;
            (statsOfRecords+k)->median = *(*(dataArray+i)+k);
        }
    }
    
    //-------------
    // FINDING MIN
    //-------------

    for (k=0; k<numValuesPerRecord; k++) {
        
        while (*(*(dataArray+looper)+k) == 0) {
            looper++;
        }
        (statsOfRecords+k)->min = *(*(dataArray+looper)+k);
        looper=0;
        
        for (i=0; i<numRecords; i++) {
            if ((*(*(dataArray+i)+k) < (statsOfRecords+k)->min) && (*(*(dataArray+i)+k) !=0)) {
                (statsOfRecords+k)->min = *(*(dataArray+i)+k);
            }
        }
    }
    
    //--------------
    // FINDING MAX
    //--------------
    looper = 0;
    for (k=0; k<numValuesPerRecord; k++) {
        
        while (*(*(dataArray+looper)+k) == 0) {
            looper++;
        }
        (statsOfRecords+k)->max = *(*(dataArray+looper)+k);
        looper=0;
        
        for (i=0; i<numRecords; i++) {
            if ((*(*(dataArray+i)+k) > (statsOfRecords+k)->max) && (*(*(dataArray+i)+k) !=0)) {
                (statsOfRecords+k)->max = *(*(dataArray+i)+k);
            }
        }
    }
    
    
    
    
    fclose(arrayFile);
    
    //-----------------
    // OUTPUT OUR DATA
    //-----------------
    printf("\n\n********OUR DATA **********\n\n");
    
    printf("---Information of Records---\n");
    for (i=0; i<numRecords; i++) {
        printf("%-10s ", (recordInfo+i)->recordIdentifier);
        printf("%f ", (recordInfo+i)->numOfNonZero);
        printf("%.0f\n", (recordInfo+i)->percentNonZero);
    }
    
    printf("\n---Calculated Stats of Record Pool---");
    printf("\nvalue  ");
    
    for (k=1; k<=numValuesPerRecord; k++) {
        printf("  %i   ", k);
    }

    printf("\nmean   ");
    for (k=0; k<numValuesPerRecord; k++) {
        printf(" %.2f ", (statsOfRecords+k)->avg);
    }
    
    printf("\nmedian ");
    for (k=0; k<numValuesPerRecord; k++) {
        printf(" %.2f ", (statsOfRecords+k)->median);
    }
    
    printf("\nmin    ");
    for (k=0; k<numValuesPerRecord; k++) {
        printf(" %.2f ", (statsOfRecords+k)->min);
    }
    
    printf("\nmax    ");
    for (k=0; k<numValuesPerRecord; k++) {
        printf(" %.2f ", (statsOfRecords+k)->max);
    }
    
    printf("\n\n");
   
    return 0;
}
