//
//  main.c
//  Project3
//
//  HI NATAWUT
//  BYE
//
//  Created by Sri Vadrevu on 11/9/16.
//  Copyright © 2016 SriVadrevu. All rights reserved.
//

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#define HASHTABLESIZE 20000
#define MAXLINESIZE 500

struct movie {
    char *name;
    struct genre *nextGenre;
    struct movie *nextMovie;
    struct review *nextReview;
};

struct genre {
    char *name;
    struct genre *next;
};

struct review {
    float stars;
    int userid;
    struct review *next;
};

// _____                 _   _               ____            _                 _   _
//|  ___|   _ _ __   ___| |_(_) ___  _ __   |  _ \  ___  ___| | __ _ _ __ __ _| |_(_) ___  _ __  ___
//| |_ | | | | '_ \ / __| __| |/ _ \| '_ \  | | | |/ _ \/ __| |/ _` | '__/ _` | __| |/ _ \| '_ \/ __|
//|  _|| |_| | | | | (__| |_| | (_) | | | | | |_| |  __/ (__| | (_| | | | (_| | |_| | (_) | | | \__ \
//|_|   \__,_|_| |_|\___|\__|_|\___/|_| |_| |____/ \___|\___|_|\__,_|_|  \__,_|\__|_|\___/|_| |_|___/

int powmod(int base, int power, int mod);

int hash(char* name);

// __  __       _          ____          _
//|  \/  | __ _(_)_ __    / ___|___   __| | ___
//| |\/| |/ _` | | '_ \  | |   / _ \ / _` |/ _ \
//| |  | | (_| | | | | | | |__| (_) | (_| |  __/
//|_|  |_|\__,_|_|_| |_|  \____\___/ \__,_|\___|

int main() {
    
    // Made this really big because I couldn't do so above for some reason
    struct movie* hashMovies[99823];
    for (int k=0; k<99823; k++) {
        hashMovies[k] = NULL;
    }
    
    //-------------------------------------------
    // READING IN ALL THE MOVIE TITLES + GENRES
    //-------------------------------------------
    
    FILE *movies;
    movies = fopen("movies_proj.csv", "r");
    
    int i;
    char genre[20];
    char title[200];
    char line[MAXLINESIZE];
    
    int placeHolder;
    int key;
    int titleLength;
    char *titleEnding;
    
    // Don't need first line
    fgets(line, MAXLINESIZE, movies);
    while (fgets(line, MAXLINESIZE, movies) != NULL) {

    //-------------MOVIE TITLES---------------
        
        if (line[0] == '"') {
            titleEnding = strstr(line, "\",");
            titleLength = titleEnding - (line+1);
            strncpy(title, &line[1], titleLength);
            title[titleLength] = '\0';
            titleLength = titleLength+3;
        }
        else {
            titleEnding = strchr(line, ',');
            titleLength = titleEnding - line;
           // printf("%d", titleLength);
            strncpy(title, &line[0], titleLength);
            title[titleLength] = '\0';
            titleLength++;
        }
        
        
        key = hash(title);
//        printf("key:%d length:%d", key, strlen(title));
        
        if (hashMovies[key] != NULL) {
            struct movie* temp = (struct movie *)malloc(sizeof(struct movie));
            temp->name = title;
            temp->nextMovie = hashMovies[key];
            temp->nextReview = NULL;
            temp->nextGenre = NULL;
            hashMovies[key] = temp;
        }
        else {
            hashMovies[key] = (struct movie *)malloc(sizeof(struct movie));
            hashMovies[key]->name = title;
            hashMovies[key]->nextMovie = NULL;
            hashMovies[key]->nextGenre = NULL;
            hashMovies[key]->nextReview = NULL;
        }
        
    //----------------GENRES------------------
        placeHolder = titleLength;
        i = titleLength;
        
        while ((line[i] != '\0') && (line[i] != '|')) {
            i++;
        }
        
        strncpy(genre, &line[placeHolder], i-placeHolder);
        genre[i-placeHolder] = '\0';
        
        hashMovies[key]->nextGenre = (struct genre *)malloc(sizeof(struct genre));
        hashMovies[key]->nextGenre->name = genre;
        struct genre* genrePtr;
        genrePtr = hashMovies[key]->nextGenre;
        genrePtr->next = NULL;
        
        // Let's not point to starting letter of next genre, because they're might be only 1
        placeHolder = i+1;
        
        while (line[i] != '\0') {
            i++;
            
            while ((line[i] != '\0') && (line[i] != '|')) {
                i++;
            }
            
            strncpy(genre, &line[placeHolder], i-placeHolder);
            genre[i-placeHolder] = '\0';
        
            genrePtr->next = (struct genre *)malloc(sizeof(struct genre));
            genrePtr = genrePtr->next;
            genrePtr->name = genre;
            genrePtr->next = NULL;
            placeHolder = i+1;
        }
    }
    
    fclose(movies);
  
    
    //-------------------------------------------
    //  READING IN ALL THE REVIEWS BY EACH USER
    //-------------------------------------------
    FILE * ratings;
    ratings = fopen("ratings_names.csv", "r");
    
    char *userID;
    char *rating;
    char *title2;
    
    fgets(line, MAXLINESIZE, ratings);
    while (fgets(line, MAXLINESIZE, ratings) != NULL) {
        
        userID = strtok(line, ",");
        rating = strtok(NULL, ",");
        title2 = strtok(NULL, "\"");

        // This is in place because using strtok gives us longer title strings,
        // resulting in a different hash key, so we need to modify it
        // But modify it so that we are still paying attention if it have quotes
        // or not, because they need to be modified differently.
        titleLength = strlen(title2);
        if (*(title2-1) != '"') {
            title2[titleLength-2] = '\0';
        }
        
        key = hash(title2);
        
        if (hashMovies[key]->nextReview != NULL) {

            struct review* temp2 = (struct review *)malloc(sizeof(struct review));
            temp2->userid = atoi(userID);
            temp2->stars = atof(rating);
            
            temp2->next = hashMovies[key]->nextReview;
            hashMovies[key]->nextReview = temp2;
        }
        else {
            hashMovies[key]->nextReview = (struct review *)malloc(sizeof(struct review));
            hashMovies[key]->nextReview->userid = atoi(userID); // type casting to set data
            hashMovies[key]->nextReview->stars = atof(rating);
        }

    }
    
    fclose(ratings);
    
    
    //---------------------------------------------
    //   CALCULATE AVERAGE RATING OF EACH MOVIE
    //---------------------------------------------
    char userInput[MAXLINESIZE];
    printf("Which movie would you like to see the average review for? (Please enter movie name and date)\n");
    fgets(userInput, 200, stdin);
    
    // terminate string at end so title strings received from user are same as
    // the strings we've been scanning from files
    // resulting in the exact key
    titleLength = strlen(userInput);
    userInput[titleLength-1] = '\0';
    
    key = hash(userInput);
    
    float sum;
    float numRatings;
    struct review * reviewPtr;
    
    if (hashMovies[key] != NULL) {
        
        numRatings= 1;
        
        reviewPtr = hashMovies[key]->nextReview;
        sum = reviewPtr->stars;
        
        while (reviewPtr->next != NULL) {
            reviewPtr = reviewPtr->next;
            sum = sum + reviewPtr->stars;
            numRatings++;
        }
        
        sum = sum/numRatings;
        
        printf("The average rating for this movie is %.2f.\n", sum);

    }
    
    while (hashMovies[key] ==  NULL) {
        
        printf("Incorrect movie title format, or movie does not exist in database.\n");
        printf("Please try again: ");
        fgets(userInput, 200, stdin);
            
        titleLength = strlen(userInput);
        userInput[titleLength-1] = '\0';
            
        key = hash(userInput);
        
//        printf("key:%d length:%d\n", key, strlen(userInput));
//    
        numRatings = 1;
    
        if (hashMovies[key] != NULL) {
    
            reviewPtr = hashMovies[key]->nextReview;
            sum = reviewPtr->stars;

            while (reviewPtr->next != NULL) {
                reviewPtr = reviewPtr->next;
                sum = sum + reviewPtr->stars;
                numRatings++;
            }
    
            sum = sum/numRatings;
            
            printf("The average rating for this movie is %.2f .\n", sum);
        }
    }
    
    return 0;
}



// _____                 _   _
//|  ___|   _ _ __   ___| |_(_) ___  _ __  ___
//| |_ | | | | '_ \ / __| __| |/ _ \| '_ \/ __|
//|  _|| |_| | | | | (__| |_| | (_) | | | \__ \
//|_|   \__,_|_| |_|\___|\__|_|\___/|_| |_|___/

int powmod(int base, int power, int mod) {
    int i;
    int total = 1;
    for (i=0; i<power; i++) {
        total = (total*base) % mod;
    }
    return total;
}

int hash(char* name) {
    int i;
    int x = 23;
    int total = 0;
    for (i=0; i<strlen(name); i++) {
        total = total + name[i]*powmod(x, i, HASHTABLESIZE);
        total = total % HASHTABLESIZE;
    }
    return total;
}


