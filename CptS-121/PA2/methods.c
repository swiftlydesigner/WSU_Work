//
//  methods.c
//  Parkerpa2
//
//  Created by Kyle Parker on 9/16/20.
//

#include "primary.h"

double calculate_total_sales_tax(SalesTaxProblem* STP) {
    
    double adjRate = STP->sales_tax_rate, taxRate = STP->sales_tax_rate;

    switch (STP->type) {
    case 0: // already in decimal form
        adjRate = 1 + STP->sales_tax_rate;
        taxRate = STP->sales_tax_rate * 100;
        break;
    case 1: // tax percentage, must divide by 100.
        adjRate = 1 + (STP->sales_tax_rate / 100);
        // good for percentage, so calculate tax rate
        break;
    case 2: // multipler, good for calculation, bad for percentage
        taxRate = fabs(1 - STP->sales_tax_rate) * 100;
        // good for adjusted rate, so calulate percentage only
        break;
    default:
        printf("[INTERNAL] Unknown mode! (%d)\n\n", STP->type);
        return -1;
    }
    
    return 0;
}

// print sales tax info without cluttering space in main method.
// @paramter - rate: get the rate, cannot tell from this value explicitly, will use ```mode``` to determine
// @paramter - cost: the cost of the item
// @paramter - mode: the form ```rate``` is in. 0 is a decimal, 1 is a percentage, 2 is the multipler that will be used in calculation.
inline void printSalesTax(double rate, double cost, int mode) {
    double adjRate = rate, taxRate = rate;
    switch (mode) {
    case 0: // already in decimal form
        adjRate = 1 + rate;
        taxRate = rate * 100;
        break;
    case 1: // tax percentage, must divide by 100.
        adjRate = 1 + (rate / 100);
        // good for percentage, so calculate tax rate
        break;
    case 2: // multipler, good for calculation, bad for percentage
        taxRate = fabs(1 - rate) * 100;
        // good for adjusted rate, so calulate percentage only
        break;
    default:
        printf("[INTERNAL] Unknown mode! (%d)\n\n", mode);
        return;
    }
    
    printf("Sales tax * Item cost = Total -> %.3lf (%.3lf%%) * %.2lf = %.2lf\n\n", adjRate, taxRate, cost, adjRate * cost);
    return;
}

inline void getSalesTaxInfo(SalesTaxProblem* STP) {
    printf("Enter sales tax (as decimal, multiplier (1.08), or percentage (5.23)): ");
    scanf("%lf", &STP->sales_tax_rate);
    printf("Enter item price: ");
    scanf("%lf", &STP->item_cost);
    // determine if the tax rate is between 1 and 2 then ask the user if it is a mutlipler or tax rate. tax rates are not divided by 100, multiplers are divided by 100 with one added onto it to evulate the total.
    if (STP->sales_tax_rate >= 1 && STP->sales_tax_rate < 2) {
        fflush(stdin); // clear STDIN Stream
        printf("Is %.2lf a multipler? (y)es - multipler, (n)o - tax rate. y/n: ", STP->sales_tax_rate);
        // set isMultiplerForSalesTax to the result of getYNResponse(), see line 139 for more.

        if (getYNResponse() == 0) { // isMultiplerForSalesTax == N
            STP->type = 2;
        }
        else { // isMultiplerForSalesTax == Y
            STP->type = 1;
        }
    }
    else {
        // below is a tuple statement. this is an if statement in essence. in this case, it will determine if the tax rate is a decimal or a percentage. since it was checked above for a value between 1 and 2, we are confident it is a decimal or percetage based off ```sales_tax_rate < 1```.
        STP->type = (STP->sales_tax_rate < 1 ? 0 : 1);
    }
}

// set the resistance Values, call setPositiveValue(_:) to ensure answer is non-zero. type is void because no return value is required.
// @parameter R1 - pointer value for the first resistance.
// @parameter R2 - pointer value for the second resistance.
// @parameter R3 - pointer value for the thrird resistance.
inline void setResistanceValues(int* R1, int* R2, int* R3) {
    printf("Zero prohibited for resistance values R1...R3!\nPlease enter a numeric value for the first resistor in series: ");
    setNonZeroValue(R1);

    printf("Please enter a numeric value for the second resistor in series: ");
    setNonZeroValue(R2);

    printf("Please enter a numeric value for the thrid resistor in series: ");
    setNonZeroValue(R3);
}

// return the series resistance calculation.
// @parameter R - pointer for the resistance structure
// @return - series resistance value in an int
inline int calculate_series_resistance(Resistance* R) {
    return R->R1 + R->R2 + R->R3;
}

// return the series resistance calculation.
// @parameter R - pointer for the resistance structure
// @return - parallel resistance value in a double
inline double calculate_parallel_resistance(Resistance* R) {
    return (1.0 / (1.0 / R->R1 + 1.0 / R->R2 + 1.0 / R->R3));
}


// MARK: Generic Functions

// ensure the response is y or n, then get that value returned as an integer.
// @return - 1 for yes, 0 for no.
inline int getYNResponse(void) {
    fflush(stdin); // clear STDIN stream
    char yn;
    scanf("%c", &yn);
    if (yn == 'y' || yn == 'Y') {
        return 1;
    }
    else if (yn == 'n' || yn == 'N') {
        return 0;
    }
    printf("'%c' is not a valid response! Please enter y/n: ", yn);
    return getYNResponse();
}

// get a positive value to set to value R. Will only be valled from setResistanceValues.
// @parameter - R: integer pointer, this will pass the value onto the argued memory address, so multiple intances of the same variable is not necessary.
inline void setNonZeroValue(int* R) {
    scanf("%d", R);
    fflush(stdin); // clear STDIN stream
    if ((int)(*R) == 0) {
        printf("Could not set the value. Please input a numeric value other than 0: ");
        return setNonZeroValue(R); // denotes method ends here, calling itself first.
    }
    return;
}

inline double calculate_volume_pyramid(double l, double w, double h) {
    return (l * w * h) / 3;
}

inline char calculate_encoded_character(char plaintxt_char, int shift_int) {
    return (plaintxt_char - 'a') + 'A' - (char)shift_int;
}

inline double calculate_distance_between(double x1, double y1, double x2, double y2) {
    return sqrt(pow((x1 - x2), 2) + pow((y1 - y2), 2));
}

inline double calculate_general_equation(double y, double z, double x, int a) {
    return y / (double)(3 / 17) - z + x / (a % 2) + PI;
}
