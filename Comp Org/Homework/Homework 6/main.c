#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// prints the mips equation
void printopperation(char *opp)
{
    printf("%s", opp);
    return;
}

// prints the different choices for the first cycle
void printCycles1(int i)
{

    switch (i)
    {
    case 0:
        printf("IF\t.\t.\t.\t.\t.\t.\t.\t.\n");
        break;

    default:
        printf(".\t.\t.\t.\t.\t.\t.\t.\t.\n");
        break;
    }
    return;
}

// prints the different choices for the second cycle
void printCycles2(int i)
{

    switch (i)
    {
    case 0:
        printf("IF\tID\t.\t.\t.\t.\t.\t.\t.\n");
        break;

    case 1:
        printf(".\tIF\t.\t.\t.\t.\t.\t.\t.\n");
        break;

    default:
        printf(".\t.\t.\t.\t.\t.\t.\t.\t.\n");
        break;
    }
    return;
}

// prints the different choices for the third cycle
void printCycles3(int i)
{
    switch (i)
    {
    case 0:
        printf("IF\tID\tEX\t.\t.\t.\t.\t.\t.\n");
        break;

    case 1:
        printf(".\tIF\tID\t.\t.\t.\t.\t.\t.\n");
        break;

    case 2:
        printf(".\t.\tIF\t.\t.\t.\t.\t.\t.\n");
        break;

    default:
        printf(".\t.\t.\t.\t.\t.\t.\t.\t.\n");
        break;
    }
    return;
}

// prints the different choices for the fourth cycle
void printCycles4(int i, char mips[4][128])
{
    printopperation(mips[i]);
    printf(".\tIF\tID\t*\t.\t.\t.\t.\t.\n");
    i++;

    printopperation(mips[i]);
    printf(".\tIF\tID\tID\t.\t.\t.\t.\t.\n");
    i++;

    printopperation(mips[i]);
    printf(".\t.\tIF\tIF\t.\t.\t.\t.\t.\n");
}

// checks the first regs and if the command is storeword
void firstReg(char prevreg[4][2], char curreg1[4][2], char nextreg[4][2], char mips[4][128], int *op2, int linecount, int index, int i, int lwnum)
{
    prevreg[linecount][0] = mips[linecount][i + 1];
    prevreg[linecount][1] = mips[linecount][i + 2];
    if (mips[linecount][0] == 's')
    {
        if (mips[linecount][1] == 'w')
        {
            for (index = 0; index < linecount; index++)
            {
                if (prevreg[linecount][0] == curreg1[index][0] && prevreg[linecount][1] == curreg1[index][1])
                {
                    lwnum = index;
                    break;
                }
                else if (prevreg[linecount][0] == nextreg[index][0] && prevreg[linecount][1] == nextreg[index][1])
                {
                    lwnum = index;
                    break;
                }
            }
        }
    }
}

// checks the second regs and if there are dependancies
void secondReg(char prevreg[4][2], char curreg1[4][2], char mips[4][128], int *op1, int linecount, int index, int i)
{
    curreg1[linecount][0] = mips[linecount][i + 1];
    curreg1[linecount][1] = mips[linecount][i + 2];
    if (linecount > 0)
    {
        for (index = 0; index < linecount; index++)
        {
            if (prevreg[index][0] == curreg1[linecount][0] && prevreg[index][1] == curreg1[linecount][1])
            {
                op1[linecount] = index;
                break;
            }
        }
    }
}

// checks the third regs and if there are dependancies
void thirdReg(char prevreg[4][2], char nextreg[4][2], char mips[4][128], int *op2, int linecount, int index, int i)
{
    nextreg[linecount][0] = mips[linecount][i + 1];
    nextreg[linecount][1] = mips[linecount][i + 2];
    if (linecount > 0)
    {
        for (index = 0; index < linecount; index++)
        {
            if (prevreg[index][0] == nextreg[linecount][0] && prevreg[index][1] == nextreg[linecount][1])
            {
                op2[linecount] = index;
                break;
            }
        }
    }
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        fprintf(stderr, "Invalid usage\n");
        return EXIT_FAILURE;
    }

    // Open file and check for null
    char *fileName = argv[1];
    FILE *myfile = fopen(fileName, "r");

    if (myfile == NULL)
    {
        fprintf(stderr, "Error: Cannot open file\n");
        return EXIT_FAILURE;
    }

    // create all the variables and arrays then initialize them
    char prevreg[4][2], curreg1[4][2], nextreg[4][2];
    char mips[4][128];
    int linecount = 0, i, regcount, cyclecount = 1, index = 0, k, lwnum = -1;
    int op1[5], op2[5], hasnop[5], original[5];
    op1[0] = op1[1] = op1[2] = op1[3] = op1[4] = -1;
    op2[0] = op2[1] = op2[2] = op2[3] = op2[4] = -1;
    hasnop[0] = hasnop[1] = hasnop[2] = hasnop[3] = hasnop[4] = -1;
    original[0] = original[1] = original[2] = original[3] = original[4] = -1;

    // Parse the lines that were read in and calls the corresponding functions
    while (fgets(mips[linecount], sizeof(mips[linecount]), myfile) != NULL)
    {
        i = 0;
        regcount = 0;

        while (mips[linecount][i] != '\n')
        {
            if (mips[linecount][i] == '$')
            {
                // If the first register of this line
                if (regcount == 0)
                {
                    firstReg(prevreg, curreg1, nextreg, mips, op2, linecount, index, i, lwnum);
                }
                // If the second register of this line
                else if (regcount == 1)
                {
                    secondReg(prevreg, curreg1, mips, op1, linecount, index, i);
                }
                // If the third register of this line
                else if (regcount == 2)
                {
                    thirdReg(prevreg, nextreg, mips, op2, linecount, index, i);
                }
                i += 2;
                regcount++;
            }
            i++;
        }
        mips[linecount][i] = '\t';
        linecount++;
    }

    printf("START OF SIMULATION\n\n");

    // loops through the cycles and only goes till 9
    for (cyclecount = 1; cyclecount < 10; cyclecount++)
    {
        if (cyclecount == linecount + 5)
        {
            break;
        }
        else
        {
            int mipscount = 0;
            for (int i = 0; i < 5; i++)
            {
                if (mips[i] != NULL)
                {
                    mipscount++;
                }
            }
            // prints the cycle header for each cycle then goes through and checks which cycle it is and runs the coresponing logic
            printf("CPU Cycles ===>\t1\t2\t3\t4\t5\t6\t7\t8\t9\n");
            i = 0;

            while (i != linecount)
            {

                switch (cyclecount)
                {
                case 1:
                    if (mipscount != 0)
                    {
                        printopperation(mips[i]);
                        mipscount--;
                    }
                    printCycles1(i);
                    break;

                case 2:
                    if (mipscount != 0)
                    {
                        printopperation(mips[i]);
                        mipscount--;
                    }
                    printCycles2(i);
                    break;

                case 3:
                    if (mipscount != 0)
                    {
                        printopperation(mips[i]);
                        mipscount--;
                    }
                    printCycles3(i);
                    break;

                case 4:
                    switch (i)
                    {
                    case 0:
                        if (mipscount != 0)
                        {
                            printopperation(mips[i]);
                            mipscount--;
                        }
                        printf("IF\tID\tEX\tMEM\t.\t.\t.\t.\t.\n");
                        break;

                    case 1:
                        if (op1[i] == 0 || op2[i] == 0)
                        {
                            for (k = linecount; k > i; k--)
                            {
                                strncpy(mips[k], mips[k - 1], sizeof(mips[k]));
                                original[k] = k - 1;
                            }
                            strncpy(mips[i], "nop\t\t", sizeof(mips[i]));
                            linecount++;

                            if (mipscount != 0)
                            {
                                printopperation(mips[i]);
                                mipscount--;
                            }
                            printf(".\tIF\tID\t*\t.\t.\t.\t.\t.\n");
                            i++;

                            if (op1[i] == 0 || op2[i] == 0)
                            {
                                for (k = linecount; k > i; k--)
                                {
                                    strncpy(mips[k], mips[k - 1], sizeof(mips[k]));
                                    original[k] = original[k - 1];
                                }
                                strncpy(mips[i], "nop\t\t", sizeof(mips[i]));
                                linecount++;
                                printCycles4(i, mips);
                                hasnop[i - 1] = hasnop[i] = 6;
                            }
                            else
                            {
                                if (mipscount != 0)
                                {
                                    printopperation(mips[i]);
                                    mipscount--;
                                }
                                printf(".\tIF\tID\tID\t.\t.\t.\t.\t.\n");
                                hasnop[i] = 6;
                                i++;
                                if (mips[i][0] != '\0')
                                {
                                    if (mipscount != 0)
                                    {
                                        printopperation(mips[i]);
                                        mipscount--;
                                    }
                                    printf(".\t.\tIF\tID\t.\t.\t.\t.\t.\n");
                                }
                            }
                        }
                        else
                        {
                            if (mipscount != 0)
                            {
                                printopperation(mips[i]);
                                mipscount--;
                            }
                            printf(".\tIF\tID\tEX\t.\t.\t.\t.\t.\n");
                            i++;
                            if (mipscount != 0)
                            {
                                printopperation(mips[i]);
                                mipscount--;
                            }
                            printf(".\t.\tIF\tID\t.\t.\t.\t.\t.\n");
                        }
                        break;

                    case 3:
                        if (mipscount != 0)
                        {
                            printopperation(mips[i]);
                            mipscount--;
                        }
                        printf(".\t.\t.\tIF\t.\t.\t.\t.\t.\n");
                        break;

                    default:
                        if (mipscount != 0)
                        {
                            printopperation(mips[i]);
                            mipscount--;
                        }
                        switch (original[i])
                        {
                        case 3:
                            printf(".\t.\t.\tIF\t.\t.\t.\t.\t.\n");
                            break;

                        default:
                            printf(".\t.\t.\t.\t.\t.\t.\t.\t.\n");
                            break;
                        }
                        break;
                    }
                    break;

                case 5:
                    switch (i)
                    {
                    case 0:
                        if (mipscount != 0)
                        {
                            printopperation(mips[i]);
                            mipscount--;
                        }
                        printf("IF\tID\tEX\tMEM\tWB\t.\t.\t.\t.\n");
                        break;

                    case 1:
                        if (mipscount != 0)
                        {
                            printopperation(mips[i]);
                            mipscount--;
                        }
                        if (strcmp(mips[i], "nop\t\t") == 0)
                        {
                            printf(".\tIF\tID\t*\t*\t.\t.\t.\t.\n");
                        }
                        else
                        {
                            printf(".\tIF\tID\tEX\tMEM\t.\t.\t.\t.\n");
                        }
                        break;

                    case 2:
                        if (strcmp(mips[i], "nop\t\t") == 0)
                        {
                            if (mipscount != 0)
                            {
                                printopperation(mips[i]);
                                mipscount--;
                            }
                            printf(".\tIF\tID\t*\t*\t.\t.\t.\t.\n");
                        }
                        else if (original[i] == -1)
                        {
                            if (op1[i] >= 0 || op2[i] >= 0)
                            {

                                // If the dependencies are the same
                                if (op1[i] == op2[i])
                                {
                                    for (k = linecount; k > i; k--)
                                    {
                                        strncpy(mips[k], mips[k - 1], sizeof(mips[k]));
                                        if (original[k - 1] > 0)
                                            original[k] = original[k - 1];
                                        else
                                            original[k] = k - 1;
                                    }
                                    strncpy(mips[i], "nop\t\t", sizeof(mips[i]));
                                    linecount++;

                                    if (op1[i] == 0)
                                    {
                                        hasnop[i + 1] = 6;
                                    }
                                    else if (op1[i] == 1)
                                    {
                                        hasnop[i + 1] = 7;
                                    }
                                    if (lwnum == op1[i])
                                        lwnum = -1;
                                }
                                else if (op1[i] == -1)
                                {
                                    for (k = linecount; k > i; k--)
                                    {
                                        strncpy(mips[k], mips[k - 1], sizeof(mips[k]));
                                        if (original[k - 1] > 0)
                                            original[k] = original[k - 1];
                                        else
                                            original[k] = k - 1;
                                    }
                                    strncpy(mips[i], "nop\t\t", sizeof(mips[i]));
                                    linecount++;

                                    if (op2[i] == 0)
                                    {
                                        hasnop[i + 1] = 6;
                                    }
                                    else if (op2[i] == 1)
                                    {
                                        hasnop[i + 1] = 7;
                                    }
                                    if (lwnum == op2[i])
                                        lwnum = -1;
                                }
                                else if (op2[i] == -1)
                                {
                                    for (k = linecount; k > i; k--)
                                    {
                                        strncpy(mips[k], mips[k - 1], sizeof(mips[k]));
                                        if (original[k - 1] > 0)
                                            original[k] = original[k - 1];
                                        else
                                            original[k] = k - 1;
                                    }
                                    strncpy(mips[i], "nop\t\t", sizeof(mips[i]));
                                    linecount++;

                                    if (op1[i] == 0)
                                    {
                                        hasnop[i + 1] = 6;
                                    }
                                    else if (op1[i] == 1)
                                    {
                                        hasnop[i + 1] = 7;
                                    }
                                    if (lwnum == op1[i])
                                        lwnum = -1;
                                }
                                else if (op1[i] >= 0 && op2[i] >= 0 && op1[i] != op2[i])
                                {
                                    strncpy(mips[i + 2], mips[i], sizeof(mips[i + 2]));
                                    strncpy(mips[i + 1], "nop\t\t", sizeof(mips[i + 1]));
                                    strncpy(mips[i], "nop\t\t", sizeof(mips[i]));
                                    linecount += 2;
                                    original[i + 2] = i;
                                    hasnop[i + 2] = 7;
                                    if (mipscount != 0)
                                    {
                                        printopperation(mips[i]);
                                        mipscount--;
                                    }
                                    printf(".\t.\tIF\tID\t*\t.\t.\t.\t.\n");
                                    i++;
                                }
                                if (mipscount != 0)
                                {
                                    printopperation(mips[i]);
                                    mipscount--;
                                }
                                printf(".\t.\tIF\tID\t*\t.\t.\t.\t.\n");
                                i++;

                                if (lwnum >= 0)
                                {
                                    strncpy(mips[linecount], mips[i], sizeof(mips[linecount]));
                                    original[linecount] = original[i];
                                    strncpy(mips[i], "nop\t\t", sizeof(mips[i]));
                                    hasnop[4] = hasnop[3];
                                    hasnop[3] = original[i] = -1;
                                    if (mipscount != 0)
                                    {
                                        printopperation(mips[i]);
                                        mipscount--;
                                    }
                                    printf(".\t.\tIF\tID\t*\t.\t.\t.\t.\n");
                                    i++;
                                    linecount++;
                                }
                                if (mipscount != 0)
                                {
                                    printopperation(mips[i]);
                                    mipscount--;
                                }
                                printf(".\t.\tIF\tID\tID\t.\t.\t.\t.\n");
                            }
                            // else
                        }
                        else
                        {
                            if (mipscount != 0)
                            {
                                printopperation(mips[i]);
                                mipscount--;
                            }
                            if (hasnop[i] == 6)
                            {
                                printf(".\tIF\tID\tID\tID\t.\t.\t.\t.\n");
                            }
                            else
                            {
                                printf(".\t.\tIF\tID\tEX\t.\t.\t.\t.\n");
                            }
                        }
                        break;

                    case 3:
                        if (mipscount != 0)
                        {
                            printopperation(mips[i]);
                            mipscount--;
                        }
                        if (hasnop[i] == 6)
                        {
                            if (strcmp(mips[1], "nop\t\t") == 0 && strcmp(mips[2], "nop\t\t") == 0)
                            {
                                printf(".\tIF\tID\tID\tID\t.\t.\t.\t.\n");
                            }
                            else
                            {
                                printf(".\t.\tIF\tID\tID\tEX\t.\t.\t.\n");
                            }
                        }
                        else
                        {
                            if (original[i] == 2)
                            {
                                printf(".\t.\tIF\tID\tEX\t.\t.\t.\t.\n");
                            }
                            else
                            {
                                printf(".\t.\t.\tIF\tID\t.\t.\t.\t.\n");
                            }
                        }
                        break;

                    case 4:
                        if (mipscount != 0)
                        {
                            printopperation(mips[i]);
                            mipscount--;
                        }
                        if (original[i] == 2 && strcmp(mips[1], "nop\t\t") == 0 && strcmp(mips[2], "nop\t\t") == 0)
                        {
                            printf(".\t.\tIF\tIF\tIF\t.\t.\t.\t.\n");
                        }
                        else if (original[i] == 3)
                        {
                            printf(".\t.\t.\tIF\tID\t.\t.\t.\t.\n");
                        }
                        else
                        {
                            printf(".\t.\t.\t.\tIF\t.\t.\t.\t.\n");
                        }
                        break;

                    default:
                        break;
                    }
                    break;

                case 6:
                    switch (i)
                    {
                    case 0:
                        if (mipscount != 0)
                        {
                            printopperation(mips[i]);
                            mipscount--;
                        }
                        printf("IF\tID\tEX\tMEM\tWB\t.\t.\t.\t.\n");
                        break;

                    case 1:
                        if (mipscount != 0)
                        {
                            printopperation(mips[i]);
                            mipscount--;
                        }
                        if (strcmp(mips[i], "nop\t\t") == 0)
                        {
                            printf(".\tIF\tID\t*\t*\t*\t.\t.\t.\n");
                        }
                        else
                        {
                            printf(".\tIF\tID\tEX\tMEM\tWB\t.\t.\t.\n");
                        }
                        break;

                    case 2:
                        if (mipscount != 0)
                        {
                            printopperation(mips[i]);
                            mipscount--;
                        }
                        if (strcmp(mips[i], "nop\t\t") == 0)
                        {
                            if (strcmp(mips[i - 1], "nop\t\t") == 0)
                            {
                                printf(".\tIF\tID\t*\t*\t*\t.\t.\t.\n");
                            }
                            else
                            {
                                printf(".\t.\tIF\tID\t*\t*\t.\t.\t.\n");
                            }
                        }
                        else
                        {
                            if (hasnop[i] == 6)
                            {
                                printf(".\tIF\tID\tID\tID\tEX\t.\t.\t.\n");
                            }
                            else
                            {
                                printf(".\t.\tIF\tID\tEX\tMEM\t.\t.\t.\n");
                            }
                        }
                        break;

                    case 3:
                        if (strcmp(mips[i], "nop\t\t") == 0)
                        {
                            if (mipscount != 0)
                            {
                                printopperation(mips[i]);
                                mipscount--;
                            }
                            printf(".\t.\tIF\tID\t*\t*\t.\t.\t.\n");
                        }
                        else if (hasnop[i] == 6)
                        {
                            if (mipscount != 0)
                            {
                                printopperation(mips[i]);
                                mipscount--;
                            }
                            if (strcmp(mips[1], "nop\t\t") == 0 && strcmp(mips[2], "nop\t\t") == 0)
                            {
                                printf(".\tIF\tID\tID\tID\tEX\t.\t.\t.\n");
                            }
                            else
                            {
                                printf(".\t.\tIF\tID\tID\tEX\t.\t.\t.\n");
                            }
                        }
                        else if (hasnop[i] == 7)
                        {
                            if (mipscount != 0)
                            {
                                printopperation(mips[i]);
                                mipscount--;
                            }
                            printf(".\t.\tIF\tID\tID\tID\t.\t.\t.\n");
                        }
                        else
                        {
                            if (op1[i] == 1 || op2[i] == 1)
                            {
                                if (linecount == 4)
                                {
                                    strncpy(mips[linecount], mips[i], sizeof(mips[linecount]));
                                    strncpy(mips[i], "nop\t\t", sizeof(mips[i]));
                                    original[linecount] = i;
                                    linecount++;

                                    if (mipscount != 0)
                                    {
                                        printopperation(mips[i]);
                                        mipscount--;
                                    }
                                    printf(".\t.\t.\tIF\tID\t*\t.\t.\t.\n");
                                    i++;
                                    hasnop[i] = 7;
                                    if (mipscount != 0)
                                    {
                                        printopperation(mips[i]);
                                        mipscount--;
                                    }
                                    printf(".\t.\t.\tIF\tID\tID\t.\t.\t.\n");
                                }
                            }
                            else if (original[i] == 2)
                            {
                                if (mipscount != 0)
                                {
                                    printopperation(mips[i]);
                                    mipscount--;
                                }
                                printf(".\t.\tIF\tID\tEX\tMEM\t.\t.\t.\n");
                            }
                            else
                            {
                                if (mipscount != 0)
                                {
                                    printopperation(mips[i]);
                                    mipscount--;
                                }
                                printf(".\t.\t.\tIF\tID\tEX\t.\t.\t.\n");
                            }
                        }
                        break;

                    case 4:
                        if (mipscount != 0)
                        {
                            printopperation(mips[i]);
                            mipscount--;
                        }
                        if (original[i] == 2)
                        {
                            if (hasnop[i] == 7 && strcmp(mips[i - 2], "nop\t\t") == 0)
                            {
                                printf(".\t.\tIF\tID\tID\tID\t.\t.\t.\n");
                            }
                            else if (hasnop[i] == 6 && strcmp(mips[1], "nop\t\t") == 0 && strcmp(mips[2], "nop\t\t") == 0)
                            {
                                printf(".\t.\tIF\tIF\tIF\tID\t.\t.\t.\n");
                            }
                        }
                        else if (original[i] == 3)
                        {
                            printf(".\t.\t.\tIF\tID\tEX\t.\t.\t.\n");
                        }
                        else
                        {
                            printf(".\t.\t.\t.\tIF\tID\t.\t.\t.\n");
                        }
                        break;

                    default:
                        break;
                    }
                    break;

                case 7:
                    switch (i)
                    {
                    case 0:
                        if (mipscount != 0)
                        {
                            printopperation(mips[i]);
                            mipscount--;
                        }
                        printf("IF\tID\tEX\tMEM\tWB\t.\t.\t.\t.\n");
                        break;

                    case 1:
                        if (mipscount != 0)
                        {
                            printopperation(mips[i]);
                            mipscount--;
                        }
                        if (strcmp(mips[i], "nop\t\t") == 0)
                        {
                            printf(".\tIF\tID\t*\t*\t*\t.\t.\t.\n");
                        }
                        else
                        {
                            printf(".\tIF\tID\tEX\tMEM\tWB\t.\t.\t.\n");
                        }
                        break;

                    case 2:
                        if (mipscount != 0)
                        {
                            printopperation(mips[i]);
                            mipscount--;
                        }
                        if (strcmp(mips[i], "nop\t\t") == 0)
                        {
                            if (strcmp(mips[i - 1], "nop\t\t") == 0)
                            {
                                printf(".\tIF\tID\t*\t*\t*\t.\t.\t.\n");
                            }
                            else
                            {
                                printf(".\t.\tIF\tID\t*\t*\t*\t.\t.\n");
                            }
                        }
                        else
                        {
                            if (hasnop[i] == 6)
                            {
                                printf(".\tIF\tID\tID\tID\tEX\tMEM\t.\t.\n");
                            }
                            else
                            {
                                printf(".\t.\tIF\tID\tEX\tMEM\tWB\t.\t.\n");
                            }
                        }
                        break;

                    case 3:
                        if (mipscount != 0)
                        {
                            printopperation(mips[i]);
                            mipscount--;
                        }
                        if (hasnop[i] == 6)
                        {
                            if (strcmp(mips[1], "nop\t\t") == 0 && strcmp(mips[2], "nop\t\t") == 0)
                            {
                                printf(".\tIF\tID\tID\tID\tEX\tMEM\t.\t.\n");
                            }
                            else
                            {
                                printf(".\t.\tIF\tID\tID\tEX\tMEM\t.\t.\n");
                            }
                        }
                        else if (hasnop[i] == 7)
                        {
                            printf(".\t.\tIF\tID\tID\tID\tEX\t.\t.\n");
                        }
                        else if (strcmp(mips[i], "nop\t\t") == 0)
                        {
                            if (strcmp(mips[i - 1], "nop\t\t") == 0)
                            {
                                printf(".\t.\tIF\tID\t*\t*\t*\t.\t.\n");
                            }
                            else
                            {
                                printf(".\t.\t.\tIF\tID\t*\t*\t.\t.\n");
                            }
                        }
                        else
                        {
                            if (hasnop[2] >= 0)
                            {
                                printf(".\t.\tIF\tID\tEX\tMEM\tWB\t.\t.\n");
                            }
                            else
                            {
                                printf(".\t.\t.\tIF\tID\tEX\tMEM\t.\t.\n");
                            }
                        }
                        break;

                    case 4:
                        if (mipscount != 0)
                        {
                            printopperation(mips[i]);
                            mipscount--;
                        }
                        if (original[i] == 2)
                        {
                            if (hasnop[i] == 7)
                            {
                                if (strcmp(mips[i - 2], "nop\t\t") == 0)
                                {
                                    printf(".\t.\tIF\tID\tID\tID\tEX\t.\t.\n");
                                }
                                else
                                {
                                    printf(".\t.\t.\tIF\tID\tID\tEX\t.\t.\n");
                                }
                            }
                            else if (hasnop[i] == 6 && strcmp(mips[1], "nop\t\t") == 0 && strcmp(mips[2], "nop\t\t") == 0)
                            {
                                printf(".\t.\tIF\tIF\tIF\tID\tEX\t.\t.\n");
                            }
                        }
                        else if (original[i] == 3)
                        {
                            if (hasnop[i] == 7)
                            {
                                printf(".\t.\t.\tIF\tID\tID\tEX\t.\t.\n");
                            }
                            else
                            {
                                printf(".\t.\t.\tIF\tID\tEX\tMEM\t.\t.\n");
                            }
                        }
                        else
                        {
                            printf(".\t.\t.\t.\tIF\tID\tEX\t.\t.\n");
                        }
                        break;

                    default:
                        break;
                    }
                    break;

                case 8:

                    switch (i)
                    {
                    case 0:
                        if (mipscount != 0)
                        {
                            printopperation(mips[i]);
                            mipscount--;
                        }
                        printf("IF\tID\tEX\tMEM\tWB\t.\t.\t.\t.\n");
                        break;

                    case 1:
                        if (mipscount != 0)
                        {
                            printopperation(mips[i]);
                            mipscount--;
                        }
                        if (strcmp(mips[i], "nop\t\t") == 0)
                        {
                            printf(".\tIF\tID\t*\t*\t*\t.\t.\t.\n");
                        }
                        else
                        {
                            printf(".\tIF\tID\tEX\tMEM\tWB\t.\t.\t.\n");
                        }
                        break;

                    case 2:
                        if (mipscount != 0)
                        {
                            printopperation(mips[i]);
                            mipscount--;
                        }
                        if (strcmp(mips[i], "nop\t\t") == 0)
                        {
                            if (strcmp(mips[i - 1], "nop\t\t") == 0)
                            {
                                printf(".\tIF\tID\t*\t*\t*\t.\t.\t.\n");
                            }
                            else
                            {
                                printf(".\t.\tIF\tID\t*\t*\t*\t.\t.\n");
                            }
                        }
                        else
                        {
                            if (hasnop[i] == 6)
                            {
                                printf(".\tIF\tID\tID\tID\tEX\tMEM\tWB\t.\n");
                            }
                            else
                            {
                                printf(".\t.\tIF\tID\tEX\tMEM\tWB\t.\t.\n");
                            }
                        }
                        break;

                    case 3:
                        if (mipscount != 0)
                        {
                            printopperation(mips[i]);
                            mipscount--;
                        }
                        if (hasnop[i] == 6)
                        {
                            if (strcmp(mips[1], "nop\t\t") == 0 && strcmp(mips[2], "nop\t\t") == 0)
                            {
                                printf(".\tIF\tID\tID\tID\tEX\tMEM\tWB\t.\n");
                            }
                            else
                            {
                                printf(".\t.\tIF\tID\tID\tEX\tMEM\tWB\t.\n");
                            }
                        }
                        else if (hasnop[i] == 7)
                        {
                            printf(".\t.\tIF\tID\tID\tID\tEX\tMEM\t.\n");
                        }
                        else if (strcmp(mips[i], "nop\t\t") == 0)
                        {
                            if (strcmp(mips[i - 1], "nop\t\t") == 0)
                            {
                                printf(".\t.\tIF\tID\t*\t*\t*\t.\t.\n");
                            }
                            else
                            {
                                printf(".\t.\t.\tIF\tID\t*\t*\t*\t.\n");
                            }
                        }
                        else
                        {
                            if (hasnop[2] >= 0)
                            {
                                printf(".\t.\tIF\tID\tEX\tMEM\tWB\t.\t.\n");
                            }
                            else
                            {
                                printf(".\t.\t.\tIF\tID\tEX\tMEM\tWB\t.\n");
                            }
                        }
                        break;

                    case 4:
                        if (mipscount != 0)
                        {
                            printopperation(mips[i]);
                            mipscount--;
                        }
                        if (original[i] == 2)
                        {
                            if (hasnop[i] == 7)
                            {
                                if (strcmp(mips[i - 2], "nop\t\t") == 0)
                                {
                                    printf(".\t.\tIF\tID\tID\tID\tEX\tMEM\t.\n");
                                }
                                else
                                {
                                    printf(".\t.\t.\tIF\tID\tID\tEX\tMEM\t.\n");
                                }
                            }
                            else if (hasnop[i] == 6 && strcmp(mips[1], "nop\t\t") == 0 && strcmp(mips[2], "nop\t\t") == 0)
                            {
                                printf(".\t.\tIF\tIF\tIF\tID\tEX\tMEM\t.\n");
                            }
                        }
                        else if (original[i] == 3)
                        {
                            if (hasnop[i] == 7)
                            {
                                printf(".\t.\t.\tIF\tID\tID\tEX\tMEM\t.\n");
                            }
                            else
                            {
                                printf(".\t.\t.\tIF\tID\tEX\tMEM\tWB\t.\n");
                            }
                        }
                        else
                        {
                            printf(".\t.\t.\t.\tIF\tID\tEX\tMEM\t.\n");
                        }
                        break;

                    default:
                        break;
                    }
                    break;

                case 9:

                    switch (i)
                    {
                    case 0:
                        if (mipscount != 0)
                        {
                            printopperation(mips[i]);
                            mipscount--;
                        }
                        printf("IF\tID\tEX\tMEM\tWB\t.\t.\t.\t.\n");
                        break;

                    case 1:
                        if (mipscount != 0)
                        {
                            printopperation(mips[i]);
                            mipscount--;
                        }
                        if (strcmp(mips[i], "nop\t\t") == 0)
                        {
                            printf(".\tIF\tID\t*\t*\t*\t.\t.\t.\n");
                        }
                        else
                        {
                            printf(".\tIF\tID\tEX\tMEM\tWB\t.\t.\t.\n");
                        }
                        break;

                    case 2:
                        if (mipscount != 0)
                        {
                            printopperation(mips[i]);
                            mipscount--;
                        }
                        if (strcmp(mips[i], "nop\t\t") == 0)
                        {
                            if (strcmp(mips[i - 1], "nop\t\t") == 0)
                            {
                                printf(".\tIF\tID\t*\t*\t*\t.\t.\t.\n");
                            }
                            else
                            {
                                printf(".\t.\tIF\tID\t*\t*\t*\t.\t.\n");
                            }
                        }
                        else
                        {
                            if (hasnop[i] == 6)
                            {
                                printf(".\t.\tIF\tID\tID\tEX\tMEM\tWB\t.\n");
                            }
                            else
                            {
                                printf(".\t.\tIF\tID\tEX\tMEM\tWB\t.\t.\n");
                            }
                        }
                        break;

                    case 3:
                        if (mipscount != 0)
                        {
                            printopperation(mips[i]);
                            mipscount--;
                        }
                        if (hasnop[i] == 6)
                        {
                            if (strcmp(mips[1], "nop\t\t") == 0 && strcmp(mips[2], "nop\t\t") == 0)
                            {
                                printf(".\tIF\tID\tID\tID\tEX\tMEM\tWB\t.\n");
                            }
                            else
                            {
                                printf(".\t.\tIF\tID\tID\tEX\tMEM\tWB\t.\n");
                            }
                        }
                        else if (hasnop[i] == 7)
                        {
                            printf(".\t.\tIF\tID\tID\tID\tEX\tMEM\tWB\n");
                        }
                        else if (strcmp(mips[i], "nop\t\t") == 0)
                        {
                            if (strcmp(mips[i - 1], "nop\t\t") == 0)
                            {
                                printf(".\t.\tIF\tID\t*\t*\t*\t.\t.\n");
                            }
                            else
                            {
                                printf(".\t.\t.\tIF\tID\t*\t*\t*\t.\n");
                            }
                        }
                        else
                        {
                            if (hasnop[2] >= 0)
                            {
                                printf(".\t.\tIF\tID\tEX\tMEM\tWB\t.\t.\n");
                            }
                            else
                            {
                                printf(".\t.\t.\tIF\tID\tEX\tMEM\tWB\t.\n");
                            }
                        }
                        break;

                    case 4:
                        if (mipscount != 0)
                        {
                            printopperation(mips[i]);
                            mipscount--;
                        }
                        if (original[i] == 2)
                        {
                            if (hasnop[i] == 7)
                            {
                                if (strcmp(mips[i - 2], "nop\t\t") == 0)
                                {
                                    printf(".\t.\tIF\tID\tID\tID\tEX\tMEM\tWB\n");
                                }
                                else
                                {
                                    printf(".\t.\t.\tIF\tID\tID\tEX\tMEM\tWB\n");
                                }
                            }
                            else if (hasnop[i] == 6 && strcmp(mips[1], "nop\t\t") == 0 && strcmp(mips[2], "nop\t\t") == 0)
                            {
                                printf(".\t.\tIF\tIF\tIF\tID\tEX\tMEM\tWB\n");
                            }
                        }
                        else if (original[i] == 3)
                        {
                            if (hasnop[i] == 7)
                            {
                                printf(".\t.\t.\tIF\tID\tID\tEX\tMEM\t.\n");
                            }
                            else
                            {
                                printf(".\t.\t.\tIF\tID\tEX\tMEM\tWB\t.\n");
                            }
                        }
                        else
                        {
                            printf(".\t.\t.\t.\tIF\tID\tEX\tMEM\tWB\n");
                        }

                    default:
                        break;
                    }
                    break;

                default:
                    break;
                }
                i++;
            }
        }
        printf("\n");
    }

    // Ends the program and closes the file
    printf("END OF SIMULATION\n");
    fclose(myfile);

    return EXIT_SUCCESS;
}