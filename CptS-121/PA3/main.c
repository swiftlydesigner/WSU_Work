//
//  main.c
//  Parker_pa3
//
//  Created by Kyle Parker on 9/23/20.
//
// Read a file, calculate some stats, then output results to an output file.


#include "main.h"

int main() {
//    FILE* infile = fopen("/Users/kyleparker/Documents/WSU/CPT S 121/Prog Assignments/Parker_pa3/Parker_pa3/in.dat", "r");
    FILE * infile = fopen("/Users/main/Library/Mobile Documents/com~apple~CloudDocs/WSU/Files/Archives/Fall '20/CPT S 121/Prog Assignments/Parker_pa3/Parker_pa3/in.dat", "r");
    
    // create collection of students
    Student students[5];
    double GPAs[5], GPAVariances[5], ages[5], classStandings[5];
    for (int i = 0; i < 5; i += 1) {
        Student student;
        student.ID = readInteger(infile);
        student.GPA = readDouble(infile);
        student.classStanding = readInteger(infile);
        student.age = readDouble(infile);
        students[i] = student;
    }
    fclose(infile); //close infile.
    
    for (int i = 0; i < 5; i += 1) {
        GPAs[i] = students[i].GPA;
        classStandings[i] = students[i].classStanding;
        ages[i] = students[i].age;
    }
    
    // calculate sums
    const double sumGPA = calculateSum(GPAs), sumAges = calculateSum(ages), sumClassStanding = calculateSum(classStandings);
    
    // calculate means
    const double meanGPA = calculateMean(sumGPA, 5), meanAges = calculateMean(sumAges, 5), meanClassStanding = calculateMean(sumClassStanding, 5);
    
    // find deviation and store to student:
    for (int i = 0; i < 5; i += 1) {
        students[i].dev = calculateDeviation(students[i].GPA, meanGPA);
        GPAVariances[i] = students[i].dev;
    }
    
    // calculate other
    const double variance = calculateVariance(GPAVariances), GPAStdDev = calculateStdDev(variance), minGPA = findMin(GPAs), maxGPA = findMax(GPAs);
    
    //FILE* outfile = fopen("/Users/kyleparker/Documents/WSU/CPT S 121/Prog Assignments/Parker_pa3/Parker_pa3/out.dat", "w");
    FILE * outfile = fopen("/Users/main/Library/Mobile Documents/com~apple~CloudDocs/WSU/Files/Archives/Fall '20/CPT S 121/Prog Assignments/Parker_pa3/Parker_pa3/out.dat", "w");
    
    // GPA Mean  Class Standing Mean  Age Mean  GPA Std Dev  GPA Min  GPA Max
    fprintf(outfile, "%.2lf\n%.2lf\n%.2lf\n\n%.2lf\n%.2lf\n%.2lf", meanGPA, meanClassStanding, meanAges, GPAStdDev, minGPA, maxGPA);
    
    fclose(outfile); //close outfile
    
    
    return 0;
}
