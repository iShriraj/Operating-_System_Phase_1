#include<stdio.h>
#include<string.h>

//Declare variables Globally

int IC, C;
char IR[4], R[4];
int SI;
char M[100][4];
char buffer[41];
int i = 0, loc;
int t = 0, a = 0;
int j;

//Pointers for input and output

FILE* input;
FILE* output;

//Function Declaration

void EXECUTEUSERPROGRAM();
void LOAD();
void MOS();
void START();
void READ();
void WRITE();
void TERMINATE();
void INIT();
void clearbuff();

int main()
{
    //Open read and write files

    input = fopen("inputfile.txt", "r");
    output = fopen("outputfile.txt", "w");
    LOAD();
    return 0;
}

//Performs GD,PD,LR,SR,BT,CR instructions

void EXECUTEUSERPROGRAM()
{

    for (i = 0;i < 4;i++)
    {
        IR[i] = M[IC][i];       //copy instruction from memory to IR
    }

    loc = ((int)IR[2] - 48) * 10 + ((int)IR[3] - 48); //typecasting string into integer address
    IC++;
    printf("\n\n%d\n\n", loc);

    if (IR[0] == 'G' && IR[1] == 'D')
    {
        SI = 1;
        printf("\nIN GD\n");                         //If the instruction is GD , SI = 1 (Read function)
        MOS();
    }

    else if (IR[0] == 'P' && IR[1] == 'D')
    {
        SI = 2;                                      //If the instruction is PD , SI = 2 (Write function)
        printf("\nIN PD\n");
        for (i = loc;i < loc + 10;i++)
        {
            printf("M[%d] : ", i);
            for (j = 0;j < 4;j++)
            {
                printf("%c", M[i][j]);
            }
            printf("\n");
        }
        MOS();
    }

    else if (IR[0] == 'H')
    {
        SI = 3;                                     //If the instruction is H,SI = 3( Halt/Terminate function)
        printf("Program Terminated");
        MOS();
    }

    else if (IR[0] == 'L' && IR[1] == 'R')
    {
        printf("\nIn LR\n");                        //If the instruction is LR , Load the data from memory to R
        for (i = 0;i < 4;i++)
        {
            R[i] = M[loc][i];                        //R<-M[IC]
            printf("%c", R[i]);
        }
        EXECUTEUSERPROGRAM();
    }

    else if (IR[0] == 'S' && IR[1] == 'R')
    {
        printf("\nIN SR\n");                        //If the instruction is LR , Store the data from R to memory
        printf("\nmemory stored with data-->>");
        for (i = 0;i < 4;i++)
        {
            M[loc][i] = R[i];                       //M[IC]->R
            printf("%c", M[loc][i]);
        }
        EXECUTEUSERPROGRAM();
    }

    else if (IR[0] == 'C' && IR[1] == 'R')
    {
        for (i = 0;i < 4;i++)
        {
            if (R[i] == M[loc][i])                  //If the instruction is CR, Compare the data at memory location with data in R
            {
                printf("\nStrings Match!\n");
                C = 1;
            }
            else
            {                                       //Toggle register content set or reset
                C = 0;
                printf("\nStrings Don't Match!\n");
                break;
            }
        }
        EXECUTEUSERPROGRAM();
    }

    else if (IR[0] == 'B' && IR[1] == 'T')
    {
        if (C == 1)
        {                                         //If instruction is BT , jump to the location
            IC = loc;
            printf("\nValue of IC ->>%d", IC);    //Prints pointer location between 0-99
        }

        EXECUTEUSERPROGRAM();
    }

}

//Initialize the memory with character 0
void INIT()
{
    int i, j;
    for (i = 0;i < 100;i++)
    {
        for (j = 0;j < 4;j++)
        {
            M[i][j] = '\0';   //Initializing the memory with 0 character
        }
    }

    for (i = 0;i < 4;i++)
    {
        IR[i] = R[i] = '\0'; //Initializing the IR and General purpose register with character 0
    }

}

//Read the data from File

void READ()
{
    int i, j, k;

    if (fgets(buffer, 41, input) != NULL)
    {
        k = 0;
        for (i = loc;i < loc + 10;i++)
        {
            printf("M[%d] : ", i);
            for (j = 0;j < 4 && buffer[k] != '\0';j++)
            {
                M[i][j] = buffer[k];                //Transfer the data from buffer to memory
                k++;

                printf("%c", M[i][j]);              //Printing the characters in input file 4 at a time
            }
            printf("\n");
        }

    }
    EXECUTEUSERPROGRAM();
}

//Delete all the data from buffer
void clearbuff()
{
    int asd;
    for(asd = 0;asd < 41;asd++)
    {
        buffer[asd] = NULL;
    }
}

//Write the output data in file

void WRITE()
{
    int i, j, k;
    IR[3] = '0';
    k = 0;
    printf("\n");
    clearbuff();                                //Clear the buffer
    for (i = loc; i < loc + 10; i++)
    {
        for (j = 0; j < 4 && M[i][j] != '$'; j++)
        {
            if (M[i][j] == '\n')
            {
                buffer[k] = '\n';
            }
            else
                buffer[k] = M[i][j];
                printf("%c", buffer[k]);
            k++;
        }
    }
    buffer[k] = '\n';
    printf("\nString is\n");
    int as;
    for (as = 0;as < 41;as++)           //Print buffer
    {
        printf("%c", buffer[as]);
    }
    fputs(buffer, output);
    EXECUTEUSERPROGRAM();
}

//Terminate the Program

void TERMINATE()
{
                                        // Two blank lines in the output file
    memset(buffer, 0, 41);
    buffer[0] = '\n';
    buffer[1] = '\n';
    fputs(buffer, output);
}

//Switch case for SI content

void MOS()
{
    switch (SI)
    {
    case 1:
        READ();
        break;
    case 2:
        WRITE();
        break;
    case 3:
        TERMINATE();
        break;
    default:
        SI = 0;
    }
}

//Start function by loading data into memory
void START()
{
    IC = 0; //set Instruction Counter to zero
    EXECUTEUSERPROGRAM();
}

void LOAD()
{
    int t = 0;
    int i, j, k;
    while (getc(input) != EOF)//Get all the characters in the input file till the end is encountered
    {
        fseek(input, -1, SEEK_CUR);
        memset(buffer, 0, 42);
        fgets(buffer, 42, input);


        //If the instruction is $AMJ
        if (buffer[0] == '$' && buffer[1] == 'A' && buffer[2] == 'M' && buffer[3] == 'J')
        {
            printf("\nProcess started...\n");
            t = 0;
            INIT();
        }

        //If the instruction is $DTA
        else if (buffer[0] == '$' && buffer[1] == 'D' && buffer[2] == 'T' && buffer[3] == 'A')
        {
            START();
        }

        //If the instruction is $END
        else if (buffer[0] == '$' && buffer[1] == 'E' && buffer[2] == 'N' && buffer[3] == 'D')
        {
            continue;
        }

        //For GD,PD,LR,SR,BT,CR instructions
        else
        {
            int k;
            for (k = t;k < t + 10;k++)
            {
                printf("M[%d]:", k);
                int i;
                for (i = 0;i < 4;i++)
                {
                    M[k][i] = buffer[a];
                    a++;
                    printf("%c", M[k][i]);
                }
                printf("\n");
            }
            t = t + 10;
            a = 0;
        }
    }
    fclose(input);
    fclose(output);
}
