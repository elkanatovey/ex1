#include <stdio.h>
#include<string.h>
#include <stdlib.h>
#include <errno.h>

#define NO_FILES_ERROR "Usage: AnalyzeProtein <pdb1> <pdb2> ...\n"
#define FILE_NAME_START 1
#define MINIMUM_ATOMS 1
#define BAD_FILE_NAME_ERROR "Error opening file: %s\n"
#define SHORT_LINE_ERROR "ATOM line is too short %d characters\n"
#define ATOM_CHECK "ATOM  "
#define  COORDINATE_CONVERSION_ERROR "Error in coordinate conversion %s!\n"
#define  NO_ATOM_ERROR "Error - 0 atoms were found in the file %s\n"
#define COMPLETED_MOLECULE_MESSAGE "PDB file %s, %d atoms were read\n"
#define CG_MESSAGE "Cg = %.3f %.3f %.3f\n"
#define RG_MESSAGE "Rg = %\n"
#define DMAX_MESSAGE "Dmax = %\n"
#define X_COORDINATE 0
#define Y_COORDINATE 1
#define Z_COORDINATE 2
#define MAX_LINE_LENGTH 81
#define MIN_LINE_LENGTH 61
#define ATOM_LENGTH 6
#define MAX_ATOMS 20000
#define COORDINATE_NUMBER 3
#define TRUE 1
#define FALSE 0
#define MAX_NUMBER_OF_COORDINATES 8




/**
 *
 * @param lineLength
 * @param currentLine
 * @return
 */
int legalLineLengthCheck(const int lineLength, const char* currentLine)
{
    if(lineLength > ATOM_LENGTH)
    {
        if(strncmp(ATOM_CHECK, currentLine, ATOM_LENGTH) == FALSE)
        {
            fprintf(stderr, SHORT_LINE_ERROR, lineLength);
            return TRUE;
        }
    }
    return FALSE;

}


//insert the coordinates from the current line
int insertCoordinates(const char currentLine[], float atomArray[MAX_ATOMS][COORDINATE_NUMBER],
                        int i)
{
    int j;
    int coordinateStart = 30;
    for(j=0; j<COORDINATE_NUMBER; j++)
    {
        char coordinateArray[MAX_NUMBER_OF_COORDINATES];
        int k;
        for(k=0; k<MAX_NUMBER_OF_COORDINATES; k++)
        {
            coordinateArray[k] = currentLine[coordinateStart + k];
        }
        coordinateStart += 8;
        char *end;
        errno = 0;
        float currentCoordinate = strtof(coordinateArray, &end);
        if(currentCoordinate == 0 && (errno !=0 || end == coordinateArray))
        {
            fprintf(stderr, COORDINATE_CONVERSION_ERROR, coordinateArray);
            return TRUE;
        }
        atomArray[i][j] = currentCoordinate;
    }
    return FALSE;
}


/**
 *
 * @param atomArray
 * @param i
 * @param fileName
 */
void calculateLocations(float atomArray[MAX_ATOMS][COORDINATE_NUMBER], int i, char *fileName) {
    printf(COMPLETED_MOLECULE_MESSAGE, fileName, i);
    int j, k;
    float pointSum[COORDINATE_NUMBER] = {0, 0, 0};
    for (k = 0; k < COORDINATE_NUMBER; k++) {
        for (j = 0; j < i; j++) {
            pointSum[k] += atomArray[j][k];
        }
        pointSum[k] = pointSum[k] / i;
    }
    printf(CG_MESSAGE, pointSum[X_COORDINATE], pointSum[Y_COORDINATE], pointSum[Z_COORDINATE]);

}


/**
 *
 * @param currentFile
 * @param fileName
 * @return
 */
int receiveInputFile(const FILE *currentFile, char *fileName)
{
    int i = 0;
    float atomArray[MAX_ATOMS][COORDINATE_NUMBER];
    char currentLine[MAX_LINE_LENGTH];
    while(fgets(currentLine, MAX_LINE_LENGTH, (FILE *) currentFile) != NULL)
    {
        int lineLength = (int) strlen(currentLine);
        if(lineLength < MIN_LINE_LENGTH)
        {
            if(legalLineLengthCheck(lineLength, currentLine))
            {
                return TRUE;  // bad line
                }
            continue; //skippable line
            }
        if(strncmp(ATOM_CHECK, currentLine, ATOM_LENGTH) == FALSE) {
            if (insertCoordinates(currentLine, atomArray, i)) {
                return TRUE;
            }
            i++;
        }
    }
    if(i < MINIMUM_ATOMS){
        fprintf(stderr, NO_ATOM_ERROR, fileName);
        return TRUE;
    }
        calculateLocations(atomArray, i, fileName);
        return FALSE;
}


/**
 *
 * @param argc
 * @param argv
 * @return
 */
int main(int argc, char *argv[]) {
    int i = FILE_NAME_START;
    if(argc==FILE_NAME_START) // no files entered
    {
        printf(NO_FILES_ERROR);
        return 1;
    }
    FILE *currentFile;
    for(i; i<argc; i++)
    {
        currentFile = fopen(argv[i], "r");
        if(currentFile == NULL) {
            fprintf(stderr, BAD_FILE_NAME_ERROR, argv[i]);
            return -1;
        }
        else
        {
            int result = receiveInputFile(currentFile, argv[i]);
            fclose(currentFile);
            if (result != 0){
                return -1;
            }
        }
    }
    return 0;
}