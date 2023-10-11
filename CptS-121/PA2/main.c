//
//  main.c
//  Parkerpa2
//
//  Created by Kyle Parker on 9/11/20.
//

#include "primary.h"

// called when the program enters the memory. setup program and call initalizing methods here.
int main() {
    // define variables:
    char plaintext_character, encoded_character;
    int shift_int, a;
    double l, w, h, x1, x2, y1, y2, x, y, z;
    
    Resistance R;

    // #1 get resistance for series as defined by arguments gathered.
    setResistanceValues(&R.R1, &R.R2, &R.R3);
    
    R.series = calculate_series_resistance(&R);
    
    printf("R1 + R2 + R3 -> %d + %d + %d = %d.\n\n", R.R1, R.R2, R.R3, R.series);

    // #2 get sales tax rate and item cost
    fflush(stdin);
    
    SalesTaxProblem STP;
    
    printf("Enter sales tax (as decimal, multiplier (1.08), or percentage (5.23)): ");
    scanf("%lf", &STP.sales_tax_rate);
    printf("Enter item price: ");
    scanf("%lf", &STP.item_cost);
    // determine if the tax rate is between 1 and 2 then ask the user if it is a mutlipler or tax rate. tax rates are not divided by 100, multiplers are divided by 100 with one added onto it to evulate the total.
    if (STP.sales_tax_rate >= 1 && STP.sales_tax_rate < 2) {
        fflush(stdin); // clear STDIN Stream
        printf("Is %.2lf a multipler? (y)es - multipler, (n)o - tax rate. y/n: ", STP.sales_tax_rate);
        // set isMultiplerForSalesTax to the result of getYNResponse(), see line 139 for more.

        if (getYNResponse()) { // isMultiplerForSalesTax == N
            STP.type = 2;
        }
        else { // isMultiplerForSalesTax == Y
            STP.type = 1;
        }
    }
    else {
        // below is a tuple statement. this is an if statement in essence. in this case, it will determine if the tax rate is a decimal or a percentage. since it was checked above for a value between 1 and 2, we are confident it is a decimal or percetage based off ```sales_tax_rate < 1```.
        STP.type = (STP.sales_tax_rate < 1 ? 0 : 1);
    }
    
    // calculate the total with sales tax.
    calculate_total_sales_tax(&STP);
    
    //print results
    printSalesTax(STP.sales_tax_rate, STP.item_cost, STP.type);
    

    // #3: Get volume of a pyramid defined by the user, input gathered below.

    printf("Please enter the length of the pyramid: ");
    scanf("%lf", &l);
    printf("Please enter the width of the pyramid: ");
    scanf("%lf", &w);
    printf("Please enter the height of the pyramid: ");
    scanf("%lf", &h);
    printf("Volume of pyramid: (l * w * h) / 3 -> (%lf * %lf * %lf) / 3 = %lf\n\n", l, w, h, calculate_volume_pyramid(l, w, h));


    // #4: Get parallel resistance from previously defined R1, R2, and R3. Cannot divide by zero, as it was invalid from the original input.
    R.parallel = calculate_parallel_resistance(&R);
    printf("Parallel resistance: 1 / (1 / R1 + 1 / R2 + 1 / R3) = Reistance -> 1 / (1 / %d + 1 / %d + 1 / %d) = %.4lf\n\n",
        R.R1, R.R2, R.R3, R.parallel);


    // #5 Character encoding: encoded_character = (plaintext_character – 'a') + 'A' – shift_int; shift_int is an integer (note: what happens if plaintext_character is lowercase? What happens with various shift_int values? Please use the ASCII table to help you understand how to interpret the encoded character produced.)

    scanf("%c",&plaintext_character); // skip new line char
    printf("Enter a character: ");
    scanf("%c", &plaintext_character);
    printf("How far would you like to shift? ");
    scanf("%d", &shift_int);

    encoded_character = calculate_encoded_character(plaintext_character, shift_int);

    printf("plaintext_character - 'a') + 'A' - shift_int = encoded_character -> (%c - 'a') + 'A' - %d = %c\n\n", plaintext_character, shift_int, encoded_character);

    // #6 Distance between two points: distance = square root of ((x1 - x2)2 + (y1 - y2)2) (note: you will need to use sqrt ( ) out of <math.h>)

    printf("Please enter x1 and x2 with the format (floating point values) 'x1 x2': ");
    scanf("%lf%lf", &x1, &x2);
    printf("Please enter y1 and y2 with the format (floating point values) 'y1 y2': ");
    scanf("%lf%lf", &y1, &y2);

    double result = calculate_distance_between(x1, y1, x2, y2);

    // check if result is infinite, if so, add infinity sign instead of teh system's 'inf' result.
    if (isinf(result)) {
        printf("\nsqrt((x1 - x2)^2 + (y1 - y2)^2) = d -> sqrt((%.2lf - %.2lf)^2 - (%.2lf - %.2lf)^2) = %c\n\n",
            x1, x2, y1, y2, (char)236);
    } else {
        printf("\nsqrt((x1 - x2)^2 + (y1 - y2)^2) = d -> sqrt((%.2lf - %.2lf)^2 - (%.2lf - %.2lf)^2) = %.3lf\n\n",
            x1, x2, y1, y2,
            result);
    }

    // #7 General equation: y = y / (3/17) - z + x / (a % 2) + PI (recall: a is an integer; the 3 and 17 constants in the equation should be left as integers initially, but explicitly type-casted as floating-point values)

    printf("Please enter value for a: ");
    scanf("%d", &a);

    printf("Please enter a floating-point value for x: ");
    scanf("%lf", &x);
    printf("Please enter a floating-point value for y: ");
    scanf("%lf", &y);
    printf("Please enter a floating-point value for z: ");
    scanf("%lf", &z);

    result = calculate_general_equation(y, z, x, a);

    // check if result is infinite, if so, add infinity sign instead of teh system's 'inf' result.
    if (isinf(result)) {
        printf("y = y / (3/17) - z + x / (a %% 2) + PI -> %.2lf / (3/17) - %.2lf + %.2lf / (%d %% 2) + %.4lf = %c\n\n", y, z, x, a, PI, (char)236);
    } else {
        printf("y = y / (3/17) - z + x / (a %% 2) + PI -> %.2lf / (3/17) - %.2lf + %.2lf / (%d %% 2) + %.4lf = %.5lf\n\n", y, z, x, a, PI, result);
    }

    return 0; // clean exit
}

