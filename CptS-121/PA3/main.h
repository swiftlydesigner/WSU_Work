//
//  main.h
//  Parker_pa3
//
//  Created by Kyle Parker on 9/23/20.
//

#ifndef main_h
#define main_h

// import stdio and math header files for later use.
#include <stdio.h>
#include <math.h>

// reference structures:
typedef struct Student Student;

// define structures:
struct Student {
    int ID;
    double GPA;
    int classStanding;
    double age;
    double dev;
};

// define method protocol stubs here:
double readDouble(FILE*);
int readInteger(FILE*);
double calculateSum(double[]);
double calculateMean(double, int);
double calculateDeviation(double, double);
double calculateVariance(double[]);
double calculateStdDev(double variance);
double findMax(double[]);
double findMin(double[]);
void printDouble(FILE*, double);

#endif /* main_h */
