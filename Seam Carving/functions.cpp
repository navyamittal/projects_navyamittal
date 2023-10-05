#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <cmath>
#include "functions.h"

using namespace std;

Pixel** createImage(int width, int height) {
  cout << "Start createImage... " << endl;
  
  // Create a one dimensional array on the heap of pointers to Pixels 
  //    that has width elements (i.e. the number of columns)
  Pixel** image = new Pixel*[width];
  
  bool fail = false;
  
  for (int i=0; i < width; ++i) { // loop through each column
    // assign that column to a one dimensional array on the heap of Pixels
    //  that has height elements (i.e. the number of rows)
    image[i] = new Pixel[height];
    
    if (image[i] == nullptr) { // failed to allocate
      fail = true;
    }
  }
  
  if (fail) { // if any allocation fails, clean up and avoid memory leak
    // deallocate any arrays created in for loop
    for (int i=0; i < width; ++i) {
      delete [] image[i]; // deleting nullptr is not a problem
    }
    delete [] image; // delete array of pointers
    return nullptr;
  }
  
  // initialize cells
  //cout << "Initializing cells..." << endl;
  for (int row=0; row<height; ++row) {
    for (int col=0; col<width; ++col) {
      //cout << "(" << col << ", " << row << ")" << endl;
      image[col][row] = { 0, 0, 0 };
    }
  }
  cout << "End createImage... " << endl;
  return image;
}

void deleteImage(Pixel** image, int width) {
  cout << "Start deleteImage..." << endl;
  // avoid memory leak by deleting the array
  for (int i=0; i<width; ++i) {
    delete [] image[i]; // delete each individual array placed on the heap
  }
  delete [] image;
  image = nullptr;
}

// implement for part 1

int* createSeam(int length) {
  int* seam = new int[length];
  for (int i = 0; i<length; i++){
    seam[i] = 0;
  }
  return seam;
}

void deleteSeam(int* seam) {
  delete[] seam;
  seam = nullptr;  
}

bool loadImage(string filename, Pixel** image, int width, int height) {
  ifstream fin(filename);
  //filename=trim(filename);
  string word;
  int red = 0;
  int blue = 0;
  int green = 0;
  int i = 0;
  int j = 0;
  int w = 0;
  int h = 0;
  
  if (!fin.is_open()) {
    cout << "Error: failed to open input file - " << filename;
    return false;
  }
    fin >> word;
    if ((word!= "P3" && word!= "p3")){
      cout << "Error: type is " << word << " instead of P3" << endl;
      return false;
    }
    fin >> w;
    if (fin.fail()){
      cout << "Error: read non-integer value" << endl;
      return false;
    }
    if (w!=width){
      cout << "Error: input width "<< "("<< width << ")"<< " does not match value in file " << "("<< w << ")" << endl;
      return false;

    }
    fin >> h;
    if (fin.fail()){
      cout << "Error: read non-integer value" << endl;
      return false;
    }
    if (h!= height){
      cout << "Error: input height "<< "("<< height << ")"<< " does not match value in file " << "("<< h << ")" << endl;
      return false;
    }
    
    int max = 0;
    fin >> max;
    if (fin.fail()){
      return false;
    }
    for (i = 0; i<height; i++){
      for (j=0; j< width; j++){
        if (fin.eof()) {
            cout << "Error: not enough color values" << endl;
            return false;
        }
        
        fin >> red;
        
        if (fin.eof() && fin.fail()){
          cout << "Error: not enough color values" << endl;
          return false;
        }
        else if (fin.fail()){
          cout << "Error: read non-integer value" << endl;
          return false;
        }
        
        fin >> green;
        
        if (fin.eof() && fin.fail()){
          cout << "Error: not enough color values" << endl;
          return false;
        }
        else if (fin.fail()){
          cout << "Error: read non-integer value" << endl;
          return false;
        }
          
        fin >> blue;  
                  
        if (fin.eof() && fin.fail()){
          cout << "Error: not enough color values" << endl;
          return false;
        }
        else if (fin.fail()){
          cout << "Error: read non-integer value" << endl;
          return false;
        }
        //else if (!fin.empty())
        if (red <0 || red > 255){
          cout << "Error: invalid color value "<< red << endl;
          return false;
        }
        else if (green <0 || green > 255){
          cout << "Error: invalid color value " << green << endl;
          return false;
        }
        else if (blue <0 || blue > 255){
          cout << "Error: invalid color value " << blue << endl;
          return false;
        }
        image[j][i].r = red;
        image[j][i].g= green;
        image[j][i].b = blue;  
        
      }
    }
  /*if (image[width][height].r || image[width][height].g || image[width][height].b){
    cout << "Error: too many color values" << endl;
    return false;
  }*/
  int temp=0;
  /*
  if (fin>> temp && temp/1 == temp){
    cout << "Error: too many color values" << endl;
    return false;    
  }*/
  
  //fin >> temp;
  if (fin >> temp/*&& fin >> red*/){
    cout << "Error: too many color values" << endl;
  }
  return true;
}

bool outputImage(string filename, const Pixel*const* image, int width, int height) {
  ofstream fout(filename);
  if (!fout.is_open()) {
    cout << "Error: failed to open output file - " << filename;
    return false;
  }
  fout << "P3" << endl;
  fout << width << " " << height << endl;
  fout << 255 << endl;
  int i = 0;
  int j =0;
  for (i=0; i<height; i++){
    for (j=0; j<width; j++){
      fout << image[j][i].r << " " << image[j][i].g << " "<< image[j][i].b << "   ";
    }
    fout << endl;
  }
  return true;
}

int energy(const Pixel*const* image, int x, int y, int width, int height) { 
  int xh1=0, xh2 = 0;
  int yh1 = 0,yh2 = 0;
  int xv1 = 0,xv2 = 0;
  int yv1 = 0,yv2 = 0;
  int r1=0,r2=0;
  int g1=0, g2=0;
  int b1=0, b2=0;
  int rv1=0, rv2=0;
  int gv1=0,gv2=0;
  int bv1=0, bv2=0;
  int rdif=0;
  int gdif = 0;
  int bdif = 0;
  int rvdif=0;
  int gvdif = 0;
  int bvdif = 0;
  int xout=0;
  int yout=0;
  int tot=0;
  if (width==1){
    xout = 0;
    xv1 = 0;
    yv1 = (y-1)%height;
    if (yv1<0){
      yv1 += height;
    }
    xv2 = 0;
    yv2 = (y+1)%height;    
  }
  else if (height == 1){
    yout = 0;
    xh1 = (x-1);
    if (xh1<0){
      xh1 = width-1;
    }
    xh2= (x+1)%width;
    yh1 = 0;
    yh2= 0;
  }
  else if (x==0 && y==0){ //0,0
    xh1 = width -1;
    yh1 = 0;
    xh2 = 1;
    yh2 = 0;

    xv1 = 0;
    yv1 = height -1;
    xv2 = 0;
    yv2 = 1;
  }
  else if (x==0 && y ==height-1){ //bottom left
    xh1 = width -1;
    yh1 = y;
    xh2 = x+1;
    yh2 = y;

    xv1 = 0;
    yv1 = y -1;
    xv2 = 0;
    yv2 = 0;    
  }
  

  else if (x == width-1 && y == height-1){ //bottom right
    xh1 = x - 1;
    xh2 = (x+1)%width;
    yh1 = y;
    yh2 = y;

    xv1 = x;
    xv2 = x;
    yv1 = y-1;
    yv2 = 0;    
  }

  else if (y==0 && x ==width-1){ //top left
    xh1 = x - 1;
    xh2 = 0;
    yh1 = 0;
    yh2 = 0;

    xv1 = x;
    xv2 = x;
    yv1 = height-1;
    yv2 = y+1;    
  }
  else if (x==0 && y!=0){
    xh1 = width - 1;
    xh2 = x+1;
    yh1 = y;
    yh2 = y;

    xv1 = 0;
    xv2 = 0;
    yv1 = y-1;
    yv2 = y+1%height;
  }


  else if (y==0 && x!=0){
    xh1 = x - 1;
    xh2 = (x+1)%width;
    yh1 = y;
    yh2 = y;

    xv1 = x;
    xv2 = x;
    yv1 = height - 1;
    yv2 = 1;
  }

  else if (x==width-1){ 
    xh1 = x - 1;
    xh2 = (x+1)%width;
    yh1 = y;
    yh2 = y;

    xv1 = x;
    xv2 = x;
    yv1 = y - 1;
    yv2 = y + 1;    
  }

  else if (y==height-1){
    xh1 = x - 1;
    xh2 = (x+1)%width;
    yh1 = y;
    yh2 = y;

    xv1 = x;
    xv2 = x;
    yv1 = y - 1;
    yv2 = (y + 1)%height;     
  }

  else {
    xh1 = x-1;
    xh2 = x+1;
    yh1 = y;
    yh2 = y;

    xv1 = x;
    xv2 = x;
    yv1 = y-1;
    yv2 = y+1;
  }
  r1 = image[xh1][yh1].r;
  r2 = image[xh2][yh2].r;
  rdif = r2 - r1;
  g1 = image[xh1][yh1].g;
  g2 = image[xh2][yh2].g;
  gdif = g2 - g1;
  b1 = image[xh1][yh1].b;
  b2 = image[xh2][yh2].b;
  bdif = b2 - b1;
  if (width!=1){
    xout = (rdif*rdif) + (gdif*gdif) + (bdif*bdif);
  }
  rv1 = image[xv1][yv1].r;
  rv2 = image[xv2][yv2].r;
  rvdif = rv2 - rv1;
  gv1 = image[xv1][yv1].g;
  gv2 = image[xv2][yv2].g;
  gvdif = gv2 - gv1;
  bv1 = image[xv1][yv1].b;
  bv2 = image[xv2][yv2].b;
  bvdif = bv2 - bv1;
  if (height!=1){
    yout = (rvdif*rvdif) + (gvdif*gvdif) + (bvdif*bvdif);
  }
  
  tot = xout +yout;

  return tot;

  return 0;
}

// implement for part 2

// uncomment for part 2
 

int loadVerticalSeam(Pixel** image, int start_col, int width, int height, int* seam) {
  int total_energy=0;
  total_energy = energy(image, start_col,0, width, height);
  seam[0] = start_col;
  int prev=0; //right
  int next =0; //left
  int down=0; //forward
  int min = 0;
  if (height==1){
    return total_energy;
  }  
  for (int i =1; i< height; i++){
    down = energy(image, start_col,i, width, height);
    if (start_col==width-1){
      prev = energy(image, start_col-1,i, width, height);      
      next = INT32_MAX;
    }
    else if (start_col==0){       
      next = energy(image, start_col+1, i, width, height);   
      prev = INT32_MAX;     
    }
    else {
      prev = energy(image, start_col-1,i, width, height);
      next = energy(image, start_col+1, i, width, height);
    }
    
    if (down <= next){
      min = down;
    }
    else if (next < down){ 
      min = next;
    }
    if (prev <min){
      min = prev;
    }    
    if (min == down){
      seam[i] = start_col;
      total_energy += down;
    }
    else if (min == next){
      seam[i] = start_col +1;
      total_energy += next;
      start_col = start_col +1;
    }
    else if (min == prev){
      seam[i] = start_col-1;
      total_energy += prev;
      start_col = start_col-1;
    }                
  }
  return total_energy;
}
/*
int loadHorizontalSeam(Pixel** image, int start_row, int width, int height, int* seam) {
  return 0;
}*/

int* findMinVerticalSeam(Pixel** image, int width, int height) {
  int* minptr; //stores current energy values
  minptr = createSeam(height);
  //int* ptr = new int[width];
  int min = 0;
  int n=0; //energy of current seam
  min = loadVerticalSeam(image,0,width,height,minptr);
  for (int i = 1; i < width; i++) {
    int* seam = createSeam(height); //stores min col values
    n = loadVerticalSeam(image, i, width, height, seam);    
    if (n < min) { //current less than min
      for (int i = 0; i < width; i++) {
        minptr[i] = seam[i];
        min = n;
      }
    }
    deleteSeam(seam);
  }
  return minptr;
}
/*
int* findMinHorizontalSeam(Pixel** image, int width, int height) {
  return nullptr;
}*/

void removeVerticalSeam(Pixel** image, int width, int height, int* verticalSeam) {
  //verticalSeam = findMinVerticalSeam(image,width,height);
  if (width == 1){
    return;
  }
  for (int i =0; i<height; i++){
    for (int j=verticalSeam[i]; j <width-1; j++){
      image[j][i].r = image[j+1][i].r;
      image[j][i].g = image[j+1][i].g;
      image[j][i].b = image[j+1][i].b;

    }
    /*image[width-1][i].r = image[width-2][i].r;
    image[width-1][i].g = image[width-2][i].g;
    image[width-1][i].b = image[width-2][i].b;*/
  }
  //width=width-1;     
}

/*
void removeHorizontalSeam(Pixel** image, int width, int height, int* horizontalSeam) {
}*/

