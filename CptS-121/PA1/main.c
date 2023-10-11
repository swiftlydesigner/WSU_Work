//
//  main.c
//  Programming Assignment 1: Equation Evaluator
//
//  Created by Kyle Parker on 9/6/20.
//

#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h> // printf, scanf, fflush, stdin
#include <math.h> // sqrt, pow, fabs, isinf

#define PI 3.1415

// define methods for implementation later
int getYNResponse(void); // no arguemnts allowed. ommiting void in this case would allow any args of any type. will return 1 or 0 depending upon the value, y or n, respectively
void printSalesTax(double, double, int); // print the sales tax, without cluttering the main function.
void setResistanceValues(int*, int*, int*); // request resistance values, call setNonZeroValue(_:)
void setNonZeroValue(int*); // will return any integer use types, except zero. will call itself when needed.

// called when the program enters the memory. setup program and call initalizing methods here.
int main() {
    // define variables:
    char plaintext_character, encoded_character;
    int shift_int, R1, R2, R3, isMultiplerForSalesTax, a;
    double sales_tax_rate, item_cost, l, w, h, x1, x2, y1, y2, x, y, z;


    // #1 get resistance for series as defined by arguments gathered.
    setResistanceValues(&R1, &R2, &R3);

    printf("R1 + R2 + R3 -> %d + %d + %d = %d.\n\n", R1, R2, R3, (R1 + R2 + R3));
    
    // #4: Get parallel resistance from previously defined R1, R2, and R3. Cannot divide by zero, as it was invalid from the original input.
    printf("Parallel resistance: 1 / (1 / R1 + 1 / R2 + 1 / R3) = Reistance -> 1 / (1 / %d + 1 / %d + 1 / %d) = %.4lf\n\n",
           R1, R2, R3,
           (1.0 / (1.0 / R1 + 1.0 / R2 + 1.0 / R3)));

    // #2 get sales tax rate and item cost

    fflush(stdin); // clear STDIN stream
    printf("Enter sales tax (as decimal, multiplier (1.08), or percentage (5.23)): ");
    scanf("%lf", &sales_tax_rate);
    printf("Enter item price: ");
    scanf("%lf", &item_cost);
    // determine if the tax rate is between 1 and 2 then ask the user if it is a mutlipler or tax rate. tax rates are not divided by 100, multiplers are divided by 100 with one added onto it to evulate the total.
    if (sales_tax_rate >= 1 && sales_tax_rate < 2) {
        fflush(stdin); // clear STDIN Stream
        printf("Is %.2lf a multipler? (y)es - multipler, (n)o - tax rate. y/n: ", sales_tax_rate);
        // set isMultiplerForSalesTax to the result of getYNResponse(), see line 139 for more.

        isMultiplerForSalesTax = getYNResponse();
        if (isMultiplerForSalesTax == 0) { // isMultiplerForSalesTax == N
            printSalesTax(sales_tax_rate, item_cost, 2);
        }
        else { // isMultiplerForSalesTax == Y
            printSalesTax(sales_tax_rate, item_cost, 1);
        }
    }
    else {
        // below is a tuple statement. this is an if statement in essence. in this case, it will determine if the tax rate is a decimal or a percentage. since it was checked above for a value between 1 and 2, we are confident it is a decimal or percetage based off ```sales_tax_rate < 1```.
        printSalesTax(sales_tax_rate, item_cost, (sales_tax_rate < 1 ? 0 : 1));
    }


    // #3: Get volume of a pyramid defined by the user, input gathered below.

    fflush(stdin); // clear STDIN stream
    printf("Please enter the length of the pyramid: ");
    scanf("%lf", &l);
    printf("Please enter the width of the pyramid: ");
    scanf("%lf", &w);
    printf("Please enter the height of the pyramid: ");
    scanf("%lf", &h);
    printf("Volume of pyramid: (l * w * h) / 3 -> (%lf * %lf * %lf) / 3 = %lf\n\n", l, w, h, (l * w * h) / 3);


    // #5 Character encoding: encoded_character = (plaintext_character – 'a') + 'A' – shift_int; shift_int is an integer (note: what happens if plaintext_character is lowercase? What happens with various shift_int values? Please use the ASCII table to help you understand how to interpret the encoded character produced.)

    scanf("%c",&plaintext_character); // skip new line char
    printf("Enter a character: ");
    scanf("%c", &plaintext_character);
    printf("How far would you like to shift? ");
    scanf("%d", &shift_int);

    encoded_character = (plaintext_character - 'a') + 'A' - (char)shift_int;

    printf("plaintext_character - 'a') + 'A' - shift_int = encoded_character -> (%c - 'a') + 'A' - %d = %c\n\n", plaintext_character, shift_int, encoded_character);

    // #6 Distance between two points: distance = square root of ((x1 - x2)2 + (y1 - y2)2) (note: you will need to use sqrt ( ) out of <math.h>)

    printf("Please enter x1 and x2 with the format (floating point values) 'x1 x2': ");
    scanf("%lf%lf", &x1, &x2);
    printf("Please enter y1 and y2 with the format (floating point values) 'y1 y2': ");
    scanf("%lf%lf", &y1, &y2);

    double result = sqrt(pow((x1 - x2), 2) + pow((y1 - y2), 2));

    // check if result is infinite, if so, add infinity sign instead of teh system's 'inf' result.
    if (isinf(result)) {
        printf("\nsqrt((x1 - x2)^2 + (y1 - y2)^2) = d -> sqrt((%.2lf - %.2lf)^2 - (%.2lf - %.2lf)^2) = %c\n\n",
            x1, x2, y1, y2, (char)236);
    }
    else {
        printf("\nsqrt((x1 - x2)^2 + (y1 - y2)^2) = d -> sqrt((%.2lf - %.2lf)^2 - (%.2lf - %.2lf)^2) = %.3lf\n\n",
            x1, x2, y1, y2,
            result);
    }


    // #7 General equation: y = y / (3/17) - z + x / (a % 2) + PI (recall: a is an integer; the 3 and 17 constants in the equation should be left as integers initially, but explicitly type-casted as floating-point values)

    printf("\nFill in the values for: F(a,x,y,z) = y / (3/17) - z + x / (a %% 2) + PI");
    printf("Please enter value for a: ");
    scanf("%d", &a);
    printf("Please enter a floating-point value for x: ");
    scanf("%lf", &x);
    printf("Please enter a floating-point value for y: ");
    scanf("%lf", &y);
    printf("Please enter a floating-point value for z: ");
    scanf("%lf", &z);

    result = y / (double)(3 / 17) - z + x / (a % 2) + PI;

    // check if result is infinite, if so, add infinity sign instead of teh system's 'inf' result.
    if (isinf(result)) {
        printf("F(a,x,y,z) = y / (3/17) - z + x / (a %% 2) + PI -> %.2lf / (3/17) - %.2lf + %.2lf / (%d %% 2) + %.4lf = %c\n\n", y, z, x, a, PI, (char)236);
    }
    else {
        printf("F(a,x,y,z) = y / (3/17) - z + x / (a %% 2) + PI -> %.2lf / (3/17) - %.2lf + %.2lf / (%d %% 2) + %.4lf = %.5lf\n\n", y, z, x, a, PI, result);
    }

    return 0; // clean exit
}

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
        // good for percentage, so only one line is needed
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
    
    double finalCost = adjRate * cost;
    
    return;
}

// set the resistance Values, call setPositiveValue(_:) to ensure answer is non-zero. type is void because no return value is required.
// @parameter R1: pointer value for the first resistance.
// @parameter R2: pointer value for the second resistance.
// @parameter R3: pointer value for the thrird resistance.
inline void setResistanceValues(int* R1, int* R2, int* R3) {
    printf("Do not enter zero for resistance values R1...R3!\nPlease enter a numeric value for the first resistor in series: ");
    setNonZeroValue(R1);

    printf("Please enter a numeric value for the second resistor in series: ");
    setNonZeroValue(R2);

    printf("Please enter a numeric value for the thrid resistor in series: ");
    setNonZeroValue(R3);
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
