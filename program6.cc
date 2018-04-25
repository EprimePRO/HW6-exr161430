//Emanuel Rios
//exr161430@utdallas.edu
//CS 3377.502 - Program6


/*
 * Usage of CDK Matrix
 *
 * File:   example1.cc
 * Author: Stephen Perkins
 * Email:  stephen.perkins@utdallas.edu
 */

#include <iostream>
#include <fstream>
#include <stdint.h>
#include <string>
#include "cdk.h"

#define MATRIX_WIDTH 3
#define MATRIX_HEIGHT 5
#define BOX_WIDTH 20
#define MATRIX_NAME_STRING "Binary File Contents"

using namespace std;

class BinaryHeaderFile{
public:
  uint32_t magicNumber;
  uint32_t versionNumber;
  uint64_t numRecords;
};

const int maxRecordStringLength = 25;

class BinaryFileRecord {
public:
  uint8_t strLength;
  char stringBuffer[maxRecordStringLength];
};

int main()
{

  WINDOW	*window;
  CDKSCREEN	*cdkscreen;
  CDKMATRIX     *myMatrix;           // CDK Screen Matrix

  // Remember that matrix starts out at 1,1.
  // Since arrays start out at 0, the first entries
  // below ("R0", and "C0") are just placeholders
  // 
  // Finally... make sure your arrays have enough entries given the
  // values you choose to set for MATRIX_WIDTH and MATRIX_HEIGHT
  // above.

  const char 		*rowTitles[] = {"o", "a", "b", "c", "d", "e"};
  const char 		*columnTitles[] = {"o", "a", "b", "c", "d", "e"};
  int		boxWidths[] = {BOX_WIDTH, BOX_WIDTH, BOX_WIDTH, BOX_WIDTH, BOX_WIDTH, BOX_WIDTH};
  int		boxTypes[] = {vMIXED, vMIXED, vMIXED, vMIXED,  vMIXED,  vMIXED};

  /*
   * Initialize the Cdk screen.
   *
   * Make sure the putty terminal is large enough
   */
  window = initscr();
  cdkscreen = initCDKScreen(window);

  /* Start CDK Colors */
  initCDKColor();

  /*
   * Create the matrix.  Need to manually cast (const char**) to (char **)
  */
  myMatrix = newCDKMatrix(cdkscreen, CENTER, CENTER, MATRIX_HEIGHT, MATRIX_WIDTH, MATRIX_HEIGHT, MATRIX_WIDTH,
			  MATRIX_NAME_STRING, (char **) rowTitles, (char **) columnTitles, boxWidths,
				     boxTypes, 1, 1, ' ', ROW, true, true, false);

  if (myMatrix ==NULL)
    {
      printf("Error creating Matrix\n");
      _exit(1);
    }

  /* Display the Matrix */
  drawCDKMatrix(myMatrix, true);
  
  /* Open the binary file for reading. */
  ifstream binInFile ("cs3377.bin", ios::in | ios::binary);
  if(!binInFile.is_open()){
    cerr << "Binary File did not open correctly." << endl;
    exit(-1);
  }

  /* Read the binary file header. */
  BinaryHeaderFile * header = new BinaryHeaderFile();
  binInFile.read((char*) header, sizeof(BinaryHeaderFile));


  /*
   * Dipslay a message
   */
  char* line = new char[1024];
  sprintf(line, "Magic: 0x%X", header->magicNumber);
  setCDKMatrixCell(myMatrix, 1, 1, line);
  sprintf(line, "Version: %u", header->versionNumber);
  setCDKMatrixCell(myMatrix, 1, 2, line);
  sprintf(line, "NumRecords: %lu", header->numRecords);
  setCDKMatrixCell(myMatrix, 1, 3, line);
  
  BinaryFileRecord * record = new BinaryFileRecord();
  
  if((int)header->numRecords>4)
    header->numRecords = 4;
  
  for(int i=1; i<=(int)header->numRecords; i++){
    binInFile.read((char *) record, sizeof(BinaryFileRecord));
    if((int)record->strLength > maxRecordStringLength){
      cerr << "Record string length is too long max is " << maxRecordStringLength << endl;
      exit(-2);
    }
    sprintf(line, "strlen: %hu", record->strLength);
    setCDKMatrixCell(myMatrix, i+1, 1, line);
    
    setCDKMatrixCell(myMatrix, i+1, 2, record->stringBuffer);
  }

  drawCDKMatrix(myMatrix, true);    /* required  */

  /* So we can see results, pause until a key is pressed. */
  unsigned char x;
  cin >> x;

  // Cleanup screen and close Binary File
  endCDK();
  binInFile.close();
}


