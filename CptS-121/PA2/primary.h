//
//  primary.h
//  Parkerpa2
//
//  Created by Kyle Parker on 9/16/20.
//

#ifndef primary_h
#define primary_h

#include <stdio.h> // printf, scanf, fflush, stdin
#include <math.h> // sqrt, pow, fabs, isinf

#define PI 3.1415

typedef struct Resistance Resistance;
typedef struct SalesTaxProblem SalesTaxProblem;

// Define structures:
struct Resistance {
    int R1, R2, R3, series;
    double parallel;
};

struct SalesTaxProblem {
    int type;
    double sales_tax_rate, item_cost, totalCost;
};

// define methods for implementation later
int getYNResponse(void); // no arguemnts allowed. ommiting void in this case would allow any args of any type. will return 1 or 0 depending upon the value, y or n, respectively
int calculate_series_resistance(Resistance*); // calculate series resistance based off the resistance structure passed in param
void printSalesTax(double, double, int); // print the sales tax, without cluttering the main function.
void setResistanceValues(int*, int*, int*); // request resistance values, call setNonZeroValue(_:)
void setNonZeroValue(int*); // will return any integer use types, except zero. will call itself when needed.
double calculate_parallel_resistance(Resistance*); // calculate parallel resistance based off the resistance structure passed in param
double calculate_total_sales_tax(SalesTaxProblem*); // calculate the sales based off sales tax and cost.
double calculate_volume_pyramid(double l, double w, double h); // calculate the volume of a pyramid and return it.
char calculate_encoded_character(char, int);
double calculate_distance_between(double, double, double, double);
double calculate_general_equation(double, double, double, int);


#endif /* primary_h */
