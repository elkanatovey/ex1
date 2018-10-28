#include <stdio.h>
#include<string.h>

#define NO_FILES_ERROR "Usage: AnalyzeProtein <pdb1> <pdb2> ...\n"
#define FILE_NAME_START 1
#define BAD_FILE_NAME_ERROR "Error opening file: %s\n"
#define SHORT_LINE_ERROR "ATOM line is too short %d characters\n"
#define ATOM_CHECK "ATOM  "
#define COMPLETED_MOLECULE_MESSAGE "PDB file %s, %d atoms were read\n"
#define CG_MESSAGE "Cg = % % %\n"
#define RG_MESSAGE "Rg = %\n"
#define DMAX_MESSAGE "Dmax = %\n"
#define X_COORDINATE 0
#define Y_COORDINATE 1
#define Z_COORDINATE 2
#define MAX_LINE_LENGTH 80
#define MIN_LINE_LENGTH 60
#define ATOM_LENGTH 6
#define MAX_ATOMS 20000
#define COORDINATE_NUMBER 3
#define TRUE 1
#define FALSE 0




int legalLineLengthCheck(const int lineLength, const char* currentLine)
{
    if(lineLength > ATOM_LENGTH)
    {
        if(strncmp(ATOM_CHECK, currentLine, ATOM_LENGTH)==0)
        {
            fprintf(stderr, SHORT_LINE_ERROR, lineLength);
            return TRUE;
        }
    }
    return FALSE;

}


int insertCoordinates(char* currentLine, int* atomArray[][], i)
{

}


int receiveInputFile(const FILE *currentFile)
{
    int i = 0;
    int atomArray[MAX_ATOMS][COORDINATE_NUMBER];
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
            if(insertCoordinates())
            {
                return TRUE;
            }
            i++;
        }

    return 0;
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
            int result = receiveInputFile(currentFile);
            fclose(currentFile);
            if (result != 0){
                return -1;
            }
        }
    }
    return 0;
}