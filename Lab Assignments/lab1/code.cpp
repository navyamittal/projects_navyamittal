// These headers define some of the classes and functions we need
#include <iostream>
#include <string>
#include <sstream>
#include <limits>

// ONLY MAKE CHANGES WHERE THERE IS A TODO

// These using declarations let us refer to things more simply
// e.g. instead of "std::cin" we can just write "cin"
using std::cin, std::cout, std::endl;
using std::string, std::getline;

// These methods are defined below the main function

// print instructions for inputting grades
void print_instructions();

// pretty print a summary of the grades
void print_results(double exam_average,
                   double hw_average,
                   double lw_average,
                   double reading,
                   double engagement,
                   double weighted_total,
                   char final_letter_grade);

// YOU ARE NOT EXPECTED TO UNDERSTAND THIS ONE... YET
// extract the category and score from the line
// and store the values in the provided variables
// if line := "exam 95", then category := "exam" and score := 95
// if the line is invalid, then category := "ignore"
void get_category_and_score(const string& line,
                            string* category,
                            double* score);

int main() {
    print_instructions();

    // ONLY MAKE CHANGES WHERE THERE IS A TODO

    // TODO(student): declare and initialize variables that you want
    int examtot = 0;
    //int finalextot = 0;
    int hwtot = 0;
    int lwtot = 0;
    int readingtot = 0;
    int engagementtot = 0;
    //int exq = 0;

    /* double hww = 0.4;
    double lww = 0.1;
    double engw = 0.05;
    double exw = 0.4;
    double readw = 0.05;
    */

    double exscore = 0;
    double hwscore = 0;
    double finalscore = 0;
    double lwscore = 0;
    double readingscore = 0;
    double engscore = 0;
    double compscore = 0;

    string line;
    // read one line from standard input (discards the ending newline character)
    getline(cin, line);
    // read lines until an empty line is read
    while (!line.empty()) {
        string category;
        double score;
        get_category_and_score(line, &category, &score);

        // process the grade entry
        if (category == "exam") {
            // TODO(student): process exam score
            exscore += score;
            examtot += 1;
            //exq +=1;    
        } else if (category == "final-exam") {
            // TODO(student): process final score
            finalscore += score;
        } else if (category == "hw") {
            // TODO(student): process hw score
            hwtot += 1;
            hwscore +=score;
            
        } else if (category == "lw") {
            // TODO(student): process lw score
            lwtot +=1;
            lwscore += score;
        } else if (category == "reading") {
            // TODO(student): process reading score
            readingscore += score;
            readingtot+=1;
            
        } else if (category == "engagement") {
            // TODO(student): process engagement score
             engagementtot +=1;
             engscore += score;
             
        }else if  (category == "compile-check") {
            // TODO(student): process compile-check score
            compscore += score;
        }else {
            cout << "ignored invalid input" << endl;
        }

        // get the next line from standard input
        getline(cin, line);
    }
    // TODO(student): compute component averages
    double exam_average =  0;
    double hw_average = 0;
    double lw_average = 0;
    double reading = 0;
    double engagement = 0;
    //double compileCheck_average = 0;
    
    exam_average = ((exscore+finalscore)/3);
    if ((exam_average)<finalscore){
        exam_average = finalscore;
    }
    if (hwtot>0){
        hw_average = (hwscore/hwtot);
    }
    lw_average = (lwscore/lwtot)*100;
    //reading = (readingscore/readingtot);
    if(readingtot>0){
        reading=(readingscore/readingtot); 
    }
    reading +=15;
    if (reading >100){
        reading = 100;
    }
    if (engagementtot>0){
    engagement =(engscore/engagementtot);
    }
    engagement +=15;
    if (engagement > 100){
        engagement = 100;
    }
    if (compscore == 1){
                lw_average*=0.5;
            }
            else if (compscore == 0){
                lw_average = 0;
            }


    // TODO(student): compute weighted total of components
    double weighted_total = (0.4*hw_average)+(0.1*lw_average)+(0.05*reading)+(0.05*engagement)+(0.4*exam_average);

    // TODO(student): compute final letter grade
    char final_letter_grade = 'X';



    if (weighted_total >= 89.5){
         final_letter_grade = 'A';
    }
    else if (weighted_total >= 79.5){
        final_letter_grade = 'B';
    }
    else if (weighted_total >= 69.5){
        final_letter_grade = 'C';
    }
    else if (weighted_total >= 59.5){
        final_letter_grade = 'D';
    }
    else if (weighted_total < 59.5){
        final_letter_grade = 'F';
    }
    
    print_results(
        exam_average, hw_average, lw_average, reading, engagement,
        weighted_total, final_letter_grade);
}

// These methods are already implemented for you
// You should not need to modify them

void print_instructions() {
    cout << "enter grades as <category> <score>" << endl;
    cout << "  <category> := exam | final-exam | hw | lw | reading | engagement | compile-check" << endl;
    cout << "     <score> := numeric value" << endl;
    cout << "enter an empty line to end input" << endl;
}

void get_category_and_score(
    const string& line,
    string* category,
    double* score) {
    // turn the string into an input stream
    std::istringstream sin(line);

    // read the category (as string) and score (as double) from the stream
    sin >> *category;
    sin >> *score;

    if (sin.fail()) {
        // the stream is in a fail state (something went wrong)
        // clear the flags
        sin.clear();
        // clear the stream buffer (throw away whatever garbage is in there)
        sin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        // signal that the line was invalid
        *category = "ignore";
    }
}

void print_results(
    double exam_average,
    double hw_average,
    double lw_average,
    double reading,
    double engagement,
    double weighted_total,
    char final_letter_grade) {
    cout << "summary:" << endl;
    cout << "      exam average: " << exam_average << endl;
    cout << "        hw average: " << hw_average << endl;
    cout << "        lw average: " << lw_average << endl;
    cout << "           reading: " << reading << endl;
    cout << "        engagement: " << engagement << endl;
    cout << "    ---------------" << endl;

    cout << "    weighted total: " << weighted_total << endl;

    cout << "final letter grade: " << final_letter_grade << endl;
}
