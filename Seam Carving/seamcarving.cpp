#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <cmath>
#include "functions.h"

using namespace std;

int main() {
  string filename;
  int width = 0;
  int height = 0;
  int targetWidth = 0;
  int targetHeight = 0;
  
  // Add code to validate input (Do in part 1)
  cout << "Input filename: ";
  cin >> filename;
  cout << "Input width and height: ";
  cin >> width;
  if (cin.fail()){
    cout << "Error: width is a non-integer value";
    return 0;
  }
  cin >> height;
  if (cin.fail()){
    cout << "Error: height is a non-integer value";
    return 0;
  }
  cout << "Input target width and target height: ";
  cin >> targetWidth;
  if (cin.fail()){
    cout << "Error: target width is a non-integer value";
    return 0;
  }
  cin >> targetHeight;
  if (cin.fail()){
    cout << "Error: target height is a non-integer value";
    return 0;
  }

  if (width <=0){
    cout << "Error: width must be greater than 0. You entered " << width << endl;
    return 0;
  }

  else if (height <=0){
    cout << "Error: height must be greater than 0. You entered " << height << endl;
    return 0;
  }

  else if (targetWidth<=0){
    cout << "Error: target width must be greater than 0. You entered " << targetWidth << endl;
    return 0;
  }

  else if (targetHeight <=0){
    cout << "Error: target height must be greater than 0. You entered " << targetHeight << endl;
    return 0;
    
  }
  int originalWidth = width; // need to delete image array at end of program

  Pixel** image = createImage(width, height); // create array of size that we need
  if (image != nullptr) {
    if (loadImage(filename, image, width, height)) {
      cout << "Start carving..." << endl;
      // Add code to remove seams from image (Do in part 2)

            // set up output filename
      stringstream ss;
      ss << "carved" << width << "X" << height << "." << filename;
      if (!outputImage(ss.str().c_str(), image, width, height)) return 1;
    } else return 1;

      while (width > targetWidth) {
        int* min = findMinVerticalSeam(image, width, height);
        removeVerticalSeam(image, width, height, min);
        deleteSeam(min);
        width= width - 1; 
      }

    
  
    // call last to remove the memory from the heap
    deleteImage(image, originalWidth);
  }
  // else 
  
}