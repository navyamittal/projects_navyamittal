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
    int exq = 0;

    double hww = 0.4;
    double lww = 0.1;
    double engw = 0.05;
    double exw = 0.4;
    double readw = 0.05;

    int exscore = 0;
    int hwscore = 0;
    int finalscore = 0;
    int lwscore = 0;
    int readingscore = 0;
    int engscore = 0;
    int compscore = 0;

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
            exq +=1;
            examtot = (exscore/examtot) * exw;
        } else if (category == "final-exam") {
            // TODO(student): process final score
            finalscore += score;
            exscore += score;
            exq += 1;
            if (finalscore >= examtot){
                examtot = finalscore;
            } else {examtot = exscore/exq;
            }
            examtot += 1;
            examtot = (exscore/examtot) * exw;
        
        } else if (category == "hw") {
            // TODO(student): process hw score
            hwtot += 1;
            hwscore +=score;
            hwtot = (hwscore/hwtot)*hww;
        } else if (category == "lw") {
            // TODO(student): process lw score
            lwtot +=1;
            lwscore += score;
            lwtot = (lwscore/lwtot)*lww;
        } else if (category == "reading") {
            // TODO(student): process reading score
            readingscore += score;
            readingtot+=1;
            readingtot = (readingscore/readingtot) * readw;
        } else if (category == "engagement") {
            // TODO(student): process engagement score
             engagementtot +=1;
             engscore += score;
             engagementtot =(engscore/engagementtot) * engw;
        }else if  (category == "compile-check") {
            // TODO(student): process compile-check score
            compscore += score;
            if (compscore == 1){
                lwtot*=0.5;
            }
            else if (compscore == 0){
                lwtot = 0;
            }

        }else {
            cout << "ignored invalid input" << endl;
        }

        // get the next line from standard input
        getline(cin, line);
    }

    // TODO(student): compute component averages
    double exam_average =  examtot;
    double hw_average = hwtot;
    double lw_average = lwtot;
    double reading = readingtot;
    double engagement = engagementtot;
    double compileCheck_average = compscore;
    char final_letter_grade = 'A';
   

    // TODO(student): compute weighted total of components
    double weighted_total = exam_average + hw_average + lw_average + reading + engagement + compileCheck_average ;

    // TODO(student): compute final letter grade
    if (weighted_total >= 90){
        char final_letter_grade = 'A';
    }
    else if (weighted_total >= 80){
        char final_letter_grade = 'B';
    }
    else if (weighted_total >= 70){
        char final_letter_grade = 'C';
    }
    else if (weighted_total >= 60){
        char final_letter_grade = 'D';
    }
    else {
        char final_letter_grade = 'F';
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
