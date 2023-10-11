//
//  mainFunctionImplementation.c
//  Parker_pa3
//
//  Created by Kyle Parker on 9/23/20.
//

#include "main.h"

/*
 * Function: readDouble(FILE* infile)
 * Date Created: 9/23/20
 * Date Modified: 9/23/20
 * Description: read a double from a file.
 * Input Parameters: infile: FILE pointer of target file.
 * Returns: double read from infile.
 * Preconditions: infile points to an existing file with a readable double. File must be open.
 * Postconditions: double returned from infile. infile is not modified.
 */
double readDouble(FILE* infile) {
    double d;
    fscanf(infile, "%lf", &d);
    return d;
}

/*
 * Function: readInt(FILE* infile)
 * Date Created: 9/23/20
 * Date Modified: 9/23/20
 * Description: read an int from a file.
 * Input Parameters: infile: FILE pointer of target file.
 * Returns: int read from infile.
 * Preconditions: infile points to an existing file with a readable int. File must be open.
 * Postconditions: int returned from infile. infile is not modified.
 */
int readInteger(FILE* infile) {
    int i;
    fscanf(infile, "%d", &i);
    return i;
}

/*
 * Function: calculateSum(int nums[])
 * Date Created: 9/23/20
 * Date Modified: 9/23/20
 * Description: Return the sum from an array.
 * Input Parameters: nums: Integer array with a ll of the
 * Returns: sum of double within the array.
 * Preconditions: nums has at least one element.
 * Postconditions: sum of nums is calculated and retuned.
 */
double calculateSum(double * nums) {
    double sum = 0;
    
    for (int index = 0; index < 5; index += 1) {
        sum += nums[index];
    }
    
    return sum;
}

/*
 * Function: calculateMean(double sum, int count)
 * Date Created: 9/23/20
 * Date Modified: 9/23/20
 * Description: return the mean from the sum and count. return -1 if count is 0.
 * Input Parameters: sum: a double of the sum of some numbers. count: the total of numbers used in sum.
 * Returns: a double rep of the mean of the sum.
 * Preconditions: count is not zero, with sum and count having a value.
 * Postconditions: the mean is retuned, unless the count is zero where -1 will be the value no matter the value of sum.
 */
double calculateMean(double sum, int count) {
    return count != 0 ? sum/count : -1;
}

/*
 * Function: calculateDeviation(double num, double mean)
 * Date Created: 9/23/20
 * Date Modified: 9/23/20
 * Description: return the deviation form the mean
 * Input Parameters: num: a double to get deviation from. mean: the mean of the series.
 * Returns: the number's deviation from the mean
 * Preconditions: num and mean have a value
 * Postconditions: the deviation is retuned.
 */
double calculateDeviation(double num, double mean) {
    return num - mean;
}

/*
 * Function: calculateVariance(double nums[])
 * Date Created: 9/23/20
 * Date Modified: 9/23/20
 * Description: Calculate and return the variance from the array.
 * Input Parameters: nums: pointer to array.
 * Returns: the variance of numbers in arugments nums.
 * Preconditions: nums is not empty, has values
 * Postconditions: the variance of numbers is retuned.
 */
double calculateVariance(double * nums) {
    double sum = 0;
    
    int len = 5;
    
    for (int index = 0; index < 5; index += 1) {
        sum += pow(nums[index],2);
    }
    
    
    
    return sum / len;
}

/*
 * Function: calculateStdDev(double variance)
 * Date Created: 9/23/20
 * Date Modified: 9/23/20
 * Description: calculate the standard deviation from the variance
 * Input Parameters: variance: a double returned from calculateVariance(double[]).
 * Returns: standard deviation as a double
 * Preconditions: variance is at least 0
 * Postconditions: std dev is calcualted and retuned.
 */
double calculateStdDev(double variance) {
    return sqrt(variance);
}

/*
 * Function: findMax(double nums[])
 * Date Created: 9/23/20
 * Date Modified: 9/23/20
 * Description: return the max of numbers in array, nums
 * Input Parameters: nums: an array of doubles
 * Returns: the max of all elements in array.
 * Preconditions: nums[] must be initalized with atleast one element.
 * Postconditions: the max is found and returned.
 */
double findMax(double * nums) {
    double max = -100;
    for (int index = 0; index < 5; index += 1) {
        if (max < fmax(nums[index],max)) {
            max = nums[index];
        }
    }
    return max;
}

/*
 * Function: findMin(double nums[])
 * Date Created: 9/23/20
 * Date Modified: 9/23/20
 * Description: return the min of numbers in array, nums
 * Input Parameters: nums: an array of doubles
 * Returns: the min of all elements in array.
 * Preconditions: nums[] must be initalized with atleast one element.
 * Postconditions: the min is found and returned.
 */
double findMin(double * nums) {
    double min = 100;
    for (int index = 0; index < 5; index += 1) {
        if (min > fmin(nums[index],min)) {
            min = nums[index];
        }
    }
    return min;
}

/*
 * Function: printDouble(FILE* outfile, double num)
 * Date Created: 9/23/20
 * Date Modified: 9/23/20
 * Description: outputs a double, num, to a file, outfile.
 * Input Parameters: outfile: the file to output num. num: the number to print to file.
 * Preconditions: File must be open.
 * Postconditions: num is not stored in outfile.
 */
void printDouble(FILE* outfile, double num) {
    fprintf(outfile, "%.2lf\n", num);
}


