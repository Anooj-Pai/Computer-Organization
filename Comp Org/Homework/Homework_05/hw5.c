#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <math.h>
#include <string.h>

int powers(int num)
{
    int count = 0;
    while (num != 1)
    {
        if (num % 2 != 0)
        {
            return 0;
        }
        num = num / 2;
        count++;
    }
    return count;
}

// check for variable in reg list
int regCheck(char var, char *arr)
{
    int i = 0;
    while (arr[i])
    {
        if (var == arr[i])
        {
            return i;
        }
        else
        {
            i++;
        }
    }
    arr[i] = var;
    return i;
}

int main(int argc, char *argv[])
{
    // checks arg count
    if (argc != 2)
    {
        fprintf(stderr, "Wrong amount of args\n");
        return EXIT_FAILURE;
    }
    // Open input file
    char *fileName = argv[1];
    FILE *file = fopen(fileName, "r");

    // check if the file can be opened
    if (!file)
    {
        fprintf(stderr, "Error: Cannot open file!\n");
        return EXIT_FAILURE;
    }

    // creates 2 arrays to store the variables and the equation and all the other variables needed. These act as counters and boolean statements
    char *variables = malloc(8 * sizeof(char));  // number of S regs
    char *equation = malloc(128 * sizeof(char)); // holds the equation gotten from the input file
    int i, j, initial, left1, left2, right1, right2, rearNeg, initNeg, hasTemp, variablesIndex;
    int k = 0, tempCount = 0, labelnum = 0, reset = 0, powernums = 0;

    char line[128];
    while (fgets(line, sizeof(line), file))
    {
        // initializes all the variables needed for the parsing of the line
        i = j = initial = hasTemp = 0;
        printf("# %s", line);

        // Eliminate all spaces
        while (line[i] != '\0')
        {
            if (isspace(line[i]))
            {
                i++;
            }
            else
            {
                equation[j++] = line[i++];
            }
        }

        i = 2;
        j = i + 1;
        k = j + 1;
        variablesIndex = regCheck(equation[0], variables);

        do
        {
            left1 = left2 = right1 = right2 = rearNeg = initNeg = -1;
            // check if the left number is negative
            if (equation[i] == '-')
            {
                initNeg = 1;
                i++;
            }

            // check if the right number is negative
            if (equation[k] == '-')
            {
                rearNeg = 1;
                k++;
            }

            // checks the left side is a number instead of a variable
            if (isdigit(equation[i]) != 0)
            {
                left1 = 0;
                left2 = equation[i] - '0';
                while (isdigit(equation[i]) != 0)
                {
                    left1 = left1 * 10 + left2;
                    i++;
                    left2 = equation[i] - '0';
                }
                j = i;
                k = i + 1;
            }

            // checks the right side is a number instead of a variable
            if (isdigit(equation[k]) != 0)
            {
                right1 = 0;
                right2 = equation[k] - '0';
                while (isdigit(equation[k]) != 0)
                {
                    right1 = right1 * 10 + right2;
                    k++;
                    right2 = equation[k] - '0';
                }
                k--;
            }

            // moves the c variable definition to the mips register
            if (equation[j] == ';' && initial == 0)
            {
                switch (initNeg)
                {
                case 1:
                    printf("li $s%d,-%d\n", variablesIndex, left1);
                    break;

                default:
                    printf("li $s%d,%d\n", variablesIndex, left1);
                    break;
                }
            }

            // check which operator is called
            if (equation[j] == '-' || equation[j] == '+' || equation[j] == '*' || equation[j] == '/' || equation[j] == '%')
            {
                if (equation[j] == '-') // checks if the operator is subtraction
                {
                    if (right1 >= 0) // checks if the second value is a number and if its negative then uses addi instead of sub
                    {
                        printf("addi ");
                        rearNeg = 1;
                    }
                    else // otherwise uses sub
                    {
                        printf("sub ");
                    }
                    switch (equation[k + 1])
                    {
                    case ';':
                        printf("$s%d,", variablesIndex);

                        if (tempCount == 10)
                        {
                            printf("$t9,");
                            tempCount = 0;
                        }
                        else if (initial == 0)
                        {
                            printf("$s%d,", regCheck(equation[i], variables));
                            initial++;
                        }
                        else
                        {
                            if (tempCount == 0)
                            {
                                printf("$t%d,", tempCount);
                            }
                            else
                            {
                                printf("$t%d,", tempCount - 1);
                            }
                        }

                        if (right1 >= 0)
                        {
                            if (rearNeg == 1)
                            {
                                printf("-%d\n", right1);
                            }
                            else
                            {
                                printf("%d\n", right1);
                            }
                        }
                        else
                        {
                            printf("$s%d\n", regCheck(equation[k], variables));
                        }
                        break;

                    default:
                        switch (tempCount)
                        {
                        case 10:
                            printf("$t0,$t9,");
                            tempCount = 1;
                            break;

                        case 0:
                            printf("$t%d,", tempCount);
                            tempCount++;
                            initial++;
                            printf("$s%d,", regCheck(equation[i], variables));
                            break;

                        default:
                            if (tempCount == 0)
                            {
                                printf("$t%d,$t%d,", tempCount, tempCount);
                            }
                            else
                            {
                                printf("$t%d,$t%d,", tempCount, tempCount - 1);
                            }
                            tempCount++;
                            break;
                        }
                        if (right1 >= 0)
                        {
                            if (rearNeg == 1)
                            {
                                printf("-%d\n", right1);
                            }
                            else
                            {
                                printf("%d\n", right1);
                            }
                        }
                        else
                        {
                            printf("$s%d\n", regCheck(equation[k], variables));
                        }
                        break;
                    }
                }
                else if (equation[j] == '+') // checks if the operator is addition
                {
                    if (right1 >= 0) // if the right value is a number we need to use addi
                    {
                        printf("addi ");
                    }
                    else
                    {
                        printf("add "); // otherwise we use add
                    }
                    switch (equation[k + 1])
                    {
                    case ';':
                        printf("$s%d,", variablesIndex);

                        if (tempCount == 10)
                        {
                            printf("$t9,");
                            tempCount = 0;
                        }
                        else if (initial == 0)
                        {
                            printf("$s%d,", regCheck(equation[i], variables));
                            initial++;
                        }
                        else
                        {
                            if (tempCount == 0)
                            {
                                printf("$t%d,", tempCount);
                            }
                            else
                            {
                                printf("$t%d,", tempCount - 1);
                            }
                        }

                        if (right1 >= 0)
                        {
                            if (rearNeg == 1)
                            {
                                printf("-%d\n", right1);
                            }
                            else
                            {
                                printf("%d\n", right1);
                            }
                        }
                        else
                        {
                            printf("$s%d\n", regCheck(equation[k], variables));
                        }
                        break;

                    default:
                        switch (tempCount)
                        {
                        case 10:
                            printf("$t0,$t9,");
                            tempCount = 1;
                            break;

                        case 0:
                            printf("$t%d,", tempCount);
                            tempCount++;
                            initial++;
                            printf("$s%d,", regCheck(equation[i], variables));
                            break;

                        default:
                            if (tempCount == 0)
                            {
                                printf("$t%d,$t%d,", tempCount, tempCount);
                            }
                            else
                            {
                                printf("$t%d,$t%d,", tempCount, tempCount - 1);
                            }
                            tempCount++;
                            break;
                        }
                        if (right1 >= 0)
                        {
                            if (rearNeg == 1)
                            {
                                printf("-%d\n", right1);
                            }
                            else
                            {
                                printf("%d\n", right1);
                            }
                        }
                        else
                        {
                            printf("$s%d\n", regCheck(equation[k], variables));
                        }
                        break;
                    }
                }
                else if (equation[j] == '*') // checks if the operator is multiplication
                {
                    if (isalpha(equation[i]) && isalpha(equation[k])) // checks if both values are variables
                    {
                        printf("mult ");
                        switch (initial)
                        {
                        case 0:
                            printf("$s%d,", regCheck(equation[i], variables));
                            break;

                        default:
                            if (tempCount == 0 && reset == 1)
                            {
                                printf("$t0,");
                                reset = 0;
                            }
                            else
                            {
                                printf("$t%d,", tempCount);
                            }
                            if (tempCount == 9)
                            {
                                tempCount = 0;
                                reset = 1;
                            }
                            else
                                tempCount++;
                            break;
                        }
                        printf("$s%d\n", regCheck(equation[k], variables));
                        if (equation[k + 1] == ';')
                        {
                            printf("mflo $s%d\n", regCheck(equation[0], variables));
                        }
                        else
                        {
                            printf("mflo $t%d\n", tempCount);
                        }
                    }
                    else if (isdigit(equation[k]) && isalpha(equation[i])) // If the left value is an register and the right one is a number
                    {
                        hasTemp++;
                        powernums = 0;
                        int *powers = malloc(50 * sizeof(int));
                        int size = 0, temp;

                        switch (right1)
                        {
                        case 1:
                            printf("move $t%d,$s%d\n", tempCount, regCheck(equation[i], variables));
                            switch (rearNeg)
                            {
                            case 1:
                                if (equation[k + 1] == ';')
                                {
                                    printf("sub $s%d,$zero,$t%d\n", variablesIndex, tempCount);
                                }
                                else
                                {
                                    printf("sub $t%d,$zero,$t%d\n", tempCount, tempCount);
                                }

                                if (tempCount == 9)
                                {
                                    tempCount = 0;
                                    reset = 1;
                                }
                                else
                                {
                                    tempCount++;
                                }
                                break;

                            default:
                                if (equation[k + 1] == ';')
                                {
                                    printf("move $s%d,$t%d\n", regCheck(equation[0], variables), tempCount);
                                }
                                if (tempCount == 9)
                                {
                                    tempCount = 0;
                                    reset = 1;
                                }
                                else
                                {
                                    tempCount++;
                                }
                                break;
                            }
                            break;

                        case 0:
                            printf("mult $t%d,$s%d\n", tempCount, regCheck(equation[i], variables));
                            printf("mflo $t%d\n", tempCount + 1);
                            printf("li $s%d,0\n", regCheck(equation[0], variables));
                            if (tempCount == 9)
                            {
                                tempCount = 0;
                                reset = 1;
                            }
                            else
                            {
                                tempCount++;
                            }
                            break;

                        default:
                            while (right1 > 0)
                            {
                                temp = right1 % 2;
                                powers[size++] = temp;
                                right1 = right1 / 2;
                            }
                            if (right1 == 0)
                            {
                                powernums = size;
                            }
                            else
                            {
                                powernums = 0;
                            }
                            break;
                        }

                        if (powernums > 0)
                        {
                            int firstShift = 0;
                            for (int a = powernums - 1; a >= 0; a--)
                            {
                                if (firstShift == 0 && powers[a] == 1)
                                {
                                    printf("sll $t%d,$s%d,%d\n", tempCount, regCheck(equation[i], variables), a);
                                    printf("move $t%d,$t%d\n", tempCount + 1, tempCount);
                                    firstShift++;
                                }
                                else if (a == 0 && powers[a] == 1)
                                {
                                    printf("add $t%d,$t%d,$s%d\n", tempCount + 1, tempCount + 1, regCheck(equation[i], variables));
                                }
                                else if (powers[a] == 1)
                                {
                                    printf("sll $t%d,$s%d,%d\n", tempCount, regCheck(equation[i], variables), a);
                                    printf("add $t%d,$t%d,$t%d\n", tempCount + 1, tempCount + 1, tempCount);
                                }
                            }

                            switch (equation[k + 1])
                            {
                            case ';':
                                switch (tempCount)
                                {
                                case 9:
                                    tempCount = 0;
                                    break;
                                default:
                                    tempCount++;
                                    break;
                                }

                                switch (rearNeg)
                                {
                                case 1:
                                    printf("sub $s%d,$zero,$t%d\n", regCheck(equation[0], variables), tempCount);
                                    break;
                                default:
                                    printf("move $s%d,$t%d\n", regCheck(equation[0], variables), tempCount);
                                    break;
                                }

                                switch (tempCount)
                                {
                                case 8:
                                    tempCount = 0;
                                    reset = 0;
                                    break;
                                default:
                                    tempCount += 2;
                                    break;
                                }

                            default:
                                switch (tempCount)
                                {
                                case 8:
                                    tempCount = 0;
                                    reset = 0;
                                    break;
                                default:
                                    tempCount += 2;
                                    break;
                                }
                            }
                        }
                        else if (right1 != 1 && powernums == 0 && right1 != 0)
                        {
                            printf("li $t%d,%d\n", tempCount, right1);
                            printf("mult $s%d,$t%d\n", regCheck(equation[i], variables), tempCount);

                            switch (equation[k + 1])
                            {
                            case ';':
                                if (rearNeg == 1)
                                {
                                    printf("mflo $s%d\n", regCheck(equation[0], variables));
                                    printf("sub $s%d,$zero,$s%d\n", regCheck(equation[0], variables), regCheck(equation[0], variables));
                                }
                                else
                                {
                                    printf("mflo $s%d\n", regCheck(equation[0], variables));
                                }
                                tempCount += 2;
                                break;
                            default:
                                switch (tempCount)
                                {
                                case 9:
                                    tempCount = 0;
                                    break;
                                default:
                                    tempCount++;
                                    break;
                                }
                                printf("mflo $t%d\n", tempCount);
                                switch (tempCount)
                                {
                                case 9:
                                    tempCount = 0;
                                    reset = 1;
                                    break;
                                default:
                                    tempCount++;
                                    break;
                                }
                            }
                        }
                    }

                    else if (left1 >= 0 && isalpha(equation[k]) && equation[k + 2] != '0')
                    {
                        hasTemp++;
                        if (initial > 0)
                        {
                            printf("mult $t%d,$s%d\n", tempCount, regCheck(equation[k], variables));
                            switch (equation[k + 1])
                            {
                            case ';':
                                printf("mflo $s%d\n", variablesIndex);
                                break;

                            default:
                                printf("mflo $t%d\n", tempCount);
                                break;
                            }

                            switch (tempCount)
                            {
                            case 9:
                                tempCount = 0;
                                reset = 1;
                                break;

                            default:
                                tempCount++;
                                break;
                            }
                        }
                        else if (initial == 0 && left1 != 0)
                        {
                            printf("li $t%d,%d\n", tempCount, left1);
                            printf("mult $t%d,$s%d\n", tempCount, regCheck(equation[k], variables));
                            switch (equation[k + 1])
                            {
                            case ';':
                                printf("mflo $s%d\n", variablesIndex);
                                break;

                            default:
                                printf("mflo $t%d\n", tempCount);
                                break;
                            }

                            switch (tempCount)
                            {
                            case 9:
                                tempCount = 0;
                                reset = 1;
                                break;

                            default:
                                tempCount++;
                                break;
                            }
                        }
                    }

                    else if (left1 >= 0 && right1 >= 0)
                    {
                        hasTemp++;
                        if ((left1 == 0 && initial == 0) || right1 == 0)
                        {
                            printf("li $s%d,0\n", variablesIndex);
                            if (initial > 0)
                            {
                                switch (tempCount)
                                {
                                case 9:
                                    tempCount = 0;
                                    reset = 1;
                                    break;

                                default:
                                    tempCount++;
                                    break;
                                }
                            }
                            break;
                        }

                        if (initial == 0)
                        {
                            printf("li $t%d,%d\n", tempCount, left1);
                            switch (tempCount)
                            {
                            case 9:
                                tempCount = 0;
                                reset = 1;
                                break;

                            default:
                                tempCount++;
                                break;
                            }
                        }

                        switch (right1)
                        {
                        case 1:
                            switch (tempCount)
                            {
                            case 9:
                                tempCount = 0;
                                reset = 1;
                                break;

                            default:
                                tempCount++;
                                break;
                            }

                            switch (rearNeg)
                            {
                            case 1:

                                switch (reset)
                                {
                                case 1:
                                    printf("sub $t0,$zero,$t9\n");
                                    reset = 0;
                                    break;

                                default:
                                    printf("sub $t%d,$zero,$t%d\n", tempCount, tempCount - 1);
                                    break;
                                }
                                break;
                            default:
                                switch (reset)
                                {
                                case 1:
                                    printf("move $t0,$t9\n");
                                    reset = 0;
                                    break;

                                default:
                                    printf("move $t%d,$t%d\n", tempCount, tempCount - 1);
                                    break;
                                }
                                break;
                            }
                            break;

                        default:
                            switch (rearNeg)
                            {
                            case 1:
                                printf("li $t%d,-%d\n", tempCount, right1);
                                break;

                            default:
                                printf("li $t%d,%d\n", tempCount, right1);
                                break;
                            }

                            switch (reset)
                            {
                            case 1:
                                printf("mult $t9,$t0\n");
                                reset = 0;
                                break;

                            default:
                                printf("mult $t%d,$t%d\n", tempCount - 1, tempCount);
                                break;
                            }

                            printf("mflo $t%d\n", tempCount + 1);
                            switch (tempCount)
                            {
                            case 9:
                                tempCount = 0;
                                reset = 1;
                                break;

                            default:
                                tempCount++;
                                break;
                            }
                            break;
                        }
                    }
                    initial++;
                }
                else if (equation[j] == '/') // if the operator is division...
                {
                    if (isalpha(equation[i]) && isalpha(equation[k])) // checks if both values are registers
                    {
                        printf("div ");
                        switch (initial)
                        {
                        case 0:
                            printf("$s%d,", regCheck(equation[i], variables));
                            break;

                        default:
                            printf("$t%d,", tempCount);
                            switch (tempCount)
                            {
                            case 9:
                                tempCount = 0;
                                reset = 1;
                                break;
                            default:
                                tempCount++;
                                break;
                            }
                            break;
                        }
                        printf("$s%d\n", regCheck(equation[k], variables));

                        switch (equation[k + 1])
                        {
                        case ';':
                            printf("mflo $s%d\n", regCheck(equation[0], variables));
                            break;

                        default:
                            if (reset == 1)
                            {
                                reset = 0;
                            }
                            printf("mflo $t%d\n", tempCount);
                            break;
                        }
                    }
                    if (isalpha(equation[i]) && isdigit(equation[k])) // checks if the right value is a number and the left is a register
                    {
                        int ispower;
                        hasTemp++;
                        switch (right1)
                        {
                        case 0:
                            fprintf(stderr, "Division by zero error\n");
                            break;

                        case 1:
                            switch (equation[k + 1])
                            {
                            case ';':

                                switch (rearNeg)
                                {
                                case 1:
                                    printf("sub $s%d,$zero,", regCheck(equation[0], variables));

                                    switch (initial)
                                    {
                                    case 0:
                                        printf("$s%d\n", regCheck(equation[i], variables));
                                        break;

                                    default:
                                        printf("$t%d\n", tempCount);
                                        tempCount++;
                                        break;
                                    }
                                    break;

                                default:
                                    printf("move $s%d,", regCheck(equation[0], variables));
                                    switch (initial)
                                    {
                                    case 0:
                                        printf("$s%d\n", regCheck(equation[i], variables));
                                        break;

                                    default:
                                        printf("$t%d\n", tempCount);
                                        tempCount++;
                                        break;
                                    }
                                    break;
                                }
                                break;

                            default:

                                switch (rearNeg)
                                {
                                case 1:
                                    printf("sub $t%d,$zero,$s%d\n", tempCount, regCheck(equation[i], variables));
                                    break;

                                default:
                                    printf("move $t%d,", tempCount);

                                    switch (initial)
                                    {
                                    case 0:
                                        printf("$s%d\n", regCheck(equation[i], variables));
                                        break;

                                    default:
                                        printf("$t%d\n", tempCount);
                                        switch (tempCount)
                                        {
                                        case 9:
                                            tempCount = 0;
                                            reset = 1;
                                            break;
                                        default:
                                            tempCount++;
                                            break;
                                        }
                                        break;
                                    }
                                    break;
                                }
                                break;
                            }
                            break;

                        default:
                            ispower = powers(right1);
                            break;
                        }

                        if (ispower > 0 && right1 > 1)
                        {
                            hasTemp++;
                            if (labelnum > 0)
                                labelnum++;

                            switch (initial)
                            {
                            case 0:
                                printf("bltz $s%d,L%d\n", regCheck(equation[i], variables), labelnum);
                                break;

                            default:
                                if (reset == 1)
                                    reset = 0;
                                printf("bltz $t%d,L%d\n", tempCount, labelnum);
                                break;
                            }
                            printf("srl $s%d,", regCheck(equation[0], variables));

                            switch (initial)
                            {
                            case 0:
                                printf("$s%d,", regCheck(equation[i], variables));
                                break;

                            default:
                                printf("$t%d,", tempCount);
                                break;
                            }
                            printf("%d\n", ispower);

                            if (rearNeg == 1)
                            {
                                printf("sub $s%d,$zero,$s%d\n", regCheck(equation[0], variables), regCheck(equation[0], variables));
                            }
                            printf("j L%d\n", labelnum + 1);
                            printf("L%d:\n", labelnum);

                            switch (rearNeg)
                            {
                            case 1:
                                switch (initial)
                                {
                                case 0:
                                    if (reset == 1)
                                        reset = 0;
                                    printf("li $t%d,-%d\n", tempCount, right1);
                                    break;

                                default:
                                    switch (tempCount)
                                    {
                                    case 9:
                                        tempCount = 0;
                                        reset = 1;
                                        break;

                                    default:
                                        tempCount++;
                                        break;
                                    }
                                    printf("li $t%d,-%d\n", tempCount, right1);
                                    break;
                                }
                                break;

                            default:
                                switch (initial)
                                {
                                case 0:

                                    switch (reset)
                                    {
                                    case 1:
                                        printf("li $t9,%d\n", right1);
                                        break;

                                    default:
                                        printf("li $t%d,%d\n", tempCount, right1);
                                        break;
                                    }
                                    break;

                                default:
                                    switch (tempCount)
                                    {
                                    case 9:
                                        tempCount = 0;
                                        reset = 1;
                                        break;

                                    default:
                                        tempCount++;
                                        break;
                                    }
                                    printf("li $t%d,%d\n", tempCount, right1);
                                    break;
                                }
                                break;
                            }

                            switch (initial)
                            {
                            case 0:
                                printf("div $s%d,$t%d\n", regCheck(equation[i], variables), tempCount);
                                break;

                            default:
                                switch (reset)
                                {
                                case 1:
                                    printf("div $t0,$t9\n");
                                    reset = 0;
                                    break;

                                default:
                                    printf("div $t%d,$t%d\n", tempCount - 1, tempCount);
                                    break;
                                }
                                break;
                            }

                            printf("mflo $s%d\n", regCheck(equation[0], variables));
                            printf("L%d:\n", labelnum + 1);
                            labelnum++;
                            tempCount++;
                        }

                        else if (ispower == 0 && right1 > 0)
                        {
                            switch (rearNeg)
                            {
                            case 1:
                                printf("li $t%d,-%d\n", tempCount, right1);
                                break;

                            default:
                                printf("li $t%d,%d\n", tempCount, right1);
                                break;
                            }

                            if (initial == 0 && hasTemp > 0)
                            {
                                printf("div $s%d,$t%d\n", regCheck(equation[i], variables), tempCount);
                            }
                            else
                            {
                                switch (reset)
                                {
                                case 1:
                                    printf("div $t0,$t9\n");
                                    reset = 0;
                                    break;

                                default:
                                    printf("div $t%d,$t%d\n", tempCount - 1, tempCount);
                                    break;
                                }
                            }

                            switch (equation[k + 1])
                            {
                            case ';':
                                printf("mflo $s%d\n", regCheck(equation[0], variables));
                                break;

                            default:
                                switch (tempCount)
                                {
                                case 9:
                                    printf("mflo $t0\n");
                                    tempCount = 0;
                                    break;

                                default:
                                    tempCount++;
                                    printf("mflo $t%d\n", tempCount);
                                    break;
                                }
                                break;
                            }
                        }
                    }

                    else if (left1 >= 0 && isalpha(equation[k]))
                    {
                        hasTemp++;
                        if (initial > 0)
                        {
                            printf("div $t%d,$s%d\n", tempCount, regCheck(equation[k], variables));

                            switch (equation[k + 1])
                            {
                            case ';':
                                printf("mflo $s%d\n", variablesIndex);
                                break;

                            default:
                                switch (tempCount)
                                {
                                case 9:
                                    tempCount = 0;
                                    reset = 1;
                                    break;

                                default:
                                    tempCount++;
                                    break;
                                }
                                printf("mflo $t%d\n", tempCount);
                                break;
                            }
                        }
                    }

                    else if (left1 >= 0 && right1 >= 0)
                    {
                        hasTemp++;

                        switch (right1)
                        {
                        case 0:
                            fprintf(stderr, "Division by zero error\n");
                            break;

                        case 1:
                            if (initial == 0)
                            {
                                printf("li $t%d,%d\n", tempCount, left1);
                            }

                            switch (equation[k + 1])
                            {
                            case ';':
                                switch (rearNeg)
                                {
                                case 1:
                                    printf("sub $s%d,$zero,", regCheck(equation[0], variables));
                                    printf("$t%d\n", tempCount);
                                    break;
                                default:
                                    printf("move $s%d,", regCheck(equation[0], variables));
                                    printf("$t%d\n", tempCount);
                                    break;
                                }

                                switch (tempCount)
                                {
                                case 9:
                                    tempCount = 0;
                                    reset = 1;
                                    break;
                                default:
                                    tempCount++;
                                    break;
                                }
                                break;

                            default:
                                switch (rearNeg)
                                {
                                case 1:
                                    switch (tempCount)
                                    {
                                    case 8:
                                        tempCount = 0;
                                        reset = 1;
                                        break;
                                    default:
                                        tempCount++;
                                        break;
                                    }

                                    switch (reset)
                                    {
                                    case 1:
                                        printf("sub $t0,$zero,$t9\n");
                                        reset = 0;
                                        break;
                                    default:
                                        printf("sub $t%d,$zero,$t%d\n", tempCount, tempCount - 1);
                                        break;
                                    }
                                    break;

                                default:
                                    switch (tempCount)
                                    {
                                    case 8:
                                        tempCount = 0;
                                        reset = 1;
                                        break;
                                    default:
                                        tempCount++;
                                        break;
                                    }

                                    switch (reset)
                                    {
                                    case 1:
                                        printf("move $t0,$t9\n");
                                        reset = 0;
                                        break;
                                    default:
                                        printf("move $t%d,", tempCount);
                                        printf("$t%d\n", tempCount - 1);
                                        break;
                                    }
                                    break;
                                }
                                break;
                            }
                            break;

                        default:
                            if (initial == 0)
                            {
                                printf("li $t%d,%d\n", tempCount, left1);
                            }

                            int ispower = powers(right1);
                            if (ispower > 0 && right1 > 1)
                            {
                                if (labelnum > 0)
                                    labelnum++;
                                printf("bltz $t%d,L%d\n", tempCount, labelnum);
                                printf("srl $s%d,$t%d,", regCheck(equation[0], variables), tempCount);

                                printf("%d\n", ispower);

                                if (rearNeg == 1)
                                {
                                    printf("sub $s%d,$zero,$s%d\n", regCheck(equation[0], variables), regCheck(equation[0], variables));
                                }
                                printf("j L%d\n", labelnum + 1);
                                printf("L%d:\n", labelnum);

                                switch (tempCount)
                                {
                                case 9:
                                    tempCount = 0;
                                    reset = 1;
                                    break;
                                default:
                                    tempCount++;
                                    break;
                                }

                                switch (rearNeg)
                                {
                                case 1:
                                    printf("li $t%d,-%d\n", tempCount, right1);
                                    break;
                                default:
                                    printf("li $t%d,%d\n", tempCount, right1);
                                    break;
                                }

                                switch (reset)
                                {
                                case 1:
                                    printf("div $t9,$t0\n");
                                    break;
                                default:
                                    printf("div $t%d,$t%d\n", tempCount - 1, tempCount);
                                    break;
                                }

                                printf("mflo $s%d\n", regCheck(equation[0], variables));
                                printf("L%d:\n", labelnum + 1);
                                labelnum++;
                                tempCount++;
                            }
                            else if (ispower == 0 && right1 > 0)
                            {
                                switch (tempCount)
                                {
                                case 9:
                                    tempCount = 0;
                                    reset = 1;
                                    break;
                                default:
                                    tempCount++;
                                    break;
                                }

                                switch (rearNeg)
                                {
                                case 1:
                                    printf("li $t%d,-%d\n", tempCount, right1);
                                    break;
                                default:
                                    printf("li $t%d,%d\n", tempCount, right1);
                                    break;
                                }

                                switch (reset)
                                {
                                case 1:
                                    printf("div $t9,$t0\n");
                                    break;
                                default:
                                    printf("div $t%d,$t%d\n", tempCount - 1, tempCount);
                                    break;
                                }

                                switch (equation[k + 1])
                                {
                                case ';':
                                    printf("mflo $s%d\n", regCheck(equation[0], variables));
                                    switch (tempCount)
                                    {
                                    case 9:
                                        tempCount = 0;
                                        reset = 1;
                                        break;
                                    default:
                                        tempCount++;
                                        break;
                                    }
                                    break;

                                default:
                                    switch (tempCount)
                                    {
                                    case 9:
                                        tempCount = 0;
                                        reset = 1;
                                        break;
                                    default:
                                        tempCount++;
                                        break;
                                    }
                                    printf("mflo $t%d\n", tempCount);
                                    break;
                                }
                            }
                            break;
                        }
                    }

                    initial++;
                }
                else if (equation[j] == '%') // this is the same as the division section but I just changed mflo to mfhi
                {
                    int ispower;
                    if (isalpha(equation[i]) && isalpha(equation[k])) // checks if both values are registers
                    {
                        printf("div ");
                        switch (initial)
                        {
                        case 0:
                            printf("$s%d,", regCheck(equation[i], variables));
                            break;

                        default:
                            printf("$t%d,", tempCount);
                            switch (tempCount)
                            {
                            case 9:
                                tempCount = 0;
                                reset = 1;
                                break;
                            default:
                                tempCount++;
                                break;
                            }
                            break;
                        }
                        printf("$s%d\n", regCheck(equation[k], variables));

                        switch (equation[k + 1])
                        {
                        case ';':
                            printf("mfhi $s%d\n", regCheck(equation[0], variables));
                            break;

                        default:
                            if (reset == 1)
                            {
                                reset = 0;
                            }
                            printf("mfhi $t%d\n", tempCount);
                            break;
                        }
                    }

                    if (isalpha(equation[i]) && isdigit(equation[k])) // checks if the right value is a number and the left is a register
                    {
                        hasTemp++;
                        int ispower;

                        switch (right1)
                        {
                        case 0:
                            fprintf(stderr, "Division by zero error\n");
                            break;

                        case 1:
                            switch (equation[k + 1])
                            {
                            case ';':

                                switch (rearNeg)
                                {
                                case 1:
                                    printf("sub $s%d,$zero,", regCheck(equation[0], variables));

                                    switch (initial)
                                    {
                                    case 0:
                                        printf("$s%d\n", regCheck(equation[i], variables));
                                        break;

                                    default:
                                        printf("$t%d\n", tempCount);
                                        tempCount++;
                                        break;
                                    }
                                    break;

                                default:
                                    printf("move $s%d,", regCheck(equation[0], variables));
                                    switch (initial)
                                    {
                                    case 0:
                                        printf("$s%d\n", regCheck(equation[i], variables));
                                        break;

                                    default:
                                        printf("$t%d\n", tempCount);
                                        tempCount++;
                                        break;
                                    }
                                    break;
                                }
                                break;

                            default:

                                switch (rearNeg)
                                {
                                case 1:
                                    printf("sub $t%d,$zero,$s%d\n", tempCount, regCheck(equation[i], variables));
                                    break;

                                default:
                                    printf("move $t%d,", tempCount);

                                    switch (initial)
                                    {
                                    case 0:
                                        printf("$s%d\n", regCheck(equation[i], variables));
                                        break;

                                    default:
                                        printf("$t%d\n", tempCount);
                                        switch (tempCount)
                                        {
                                        case 9:
                                            tempCount = 0;
                                            reset = 1;
                                            break;
                                        default:
                                            tempCount++;
                                            break;
                                        }
                                        break;
                                    }
                                    break;
                                }
                                break;
                            }
                            break;

                        default:
                            ispower = powers(right1);
                            break;
                        }

                        if (ispower > 0 && right1 > 1)
                        {
                            hasTemp++;
                            if (labelnum > 0)
                                labelnum++;

                            switch (initial)
                            {
                            case 0:
                                printf("bltz $s%d,L%d\n", regCheck(equation[i], variables), labelnum);
                                break;

                            default:
                                if (reset == 1)
                                    reset = 0;
                                printf("bltz $t%d,L%d\n", tempCount, labelnum);
                                break;
                            }
                            printf("srl $s%d,", regCheck(equation[0], variables));

                            switch (initial)
                            {
                            case 0:
                                printf("$s%d,", regCheck(equation[i], variables));
                                break;

                            default:
                                printf("$t%d,", tempCount);
                                break;
                            }
                            printf("%d\n", ispower);

                            if (rearNeg == 1)
                            {
                                printf("sub $s%d,$zero,$s%d\n", regCheck(equation[0], variables), regCheck(equation[0], variables));
                            }
                            printf("j L%d\n", labelnum + 1);
                            printf("L%d:\n", labelnum);

                            switch (rearNeg)
                            {
                            case 1:
                                switch (initial)
                                {
                                case 0:
                                    if (reset == 1)
                                        reset = 0;
                                    printf("li $t%d,-%d\n", tempCount, right1);
                                    break;

                                default:
                                    switch (tempCount)
                                    {
                                    case 9:
                                        tempCount = 0;
                                        reset = 1;
                                        break;

                                    default:
                                        tempCount++;
                                        break;
                                    }
                                    printf("li $t%d,-%d\n", tempCount, right1);
                                    break;
                                }
                                break;

                            default:
                                switch (initial)
                                {
                                case 0:

                                    switch (reset)
                                    {
                                    case 1:
                                        printf("li $t9,%d\n", right1);
                                        break;

                                    default:
                                        printf("li $t%d,%d\n", tempCount, right1);
                                        break;
                                    }
                                    break;

                                default:
                                    switch (tempCount)
                                    {
                                    case 9:
                                        tempCount = 0;
                                        reset = 1;
                                        break;

                                    default:
                                        tempCount++;
                                        break;
                                    }
                                    printf("li $t%d,%d\n", tempCount, right1);
                                    break;
                                }
                                break;
                            }

                            switch (initial)
                            {
                            case 0:
                                printf("div $s%d,$t%d\n", regCheck(equation[i], variables), tempCount);
                                break;

                            default:
                                switch (reset)
                                {
                                case 1:
                                    printf("div $t0,$t9\n");
                                    reset = 0;
                                    break;

                                default:
                                    printf("div $t%d,$t%d\n", tempCount - 1, tempCount);
                                    break;
                                }
                                break;
                            }

                            printf("mflo $s%d\n", regCheck(equation[0], variables));
                            printf("L%d:\n", labelnum + 1);
                            labelnum++;
                            tempCount++;
                        }

                        else if (ispower == 0 && right1 > 0)
                        {

                            if (rearNeg == 1)
                            {
                                printf("li $t%d,-%d\n", tempCount, right1);
                            }
                            else
                            {
                                printf("li $t%d,%d\n", tempCount, right1);
                            }

                            if (initial == 0 && hasTemp > 0)
                            {
                                printf("div $s%d,$t%d\n", regCheck(equation[i], variables), tempCount);
                            }
                            else
                            {
                                if (reset == 1)
                                {
                                    printf("div $t0,$t9\n");
                                    reset = 0;
                                }
                                else
                                {
                                    printf("div $t%d,$t%d\n", tempCount - 1, tempCount);
                                }
                            }

                            if (equation[k + 1] == ';')
                            {
                                printf("mfhi $s%d\n", regCheck(equation[0], variables));
                            }
                            else
                            {
                                if (tempCount == 9)
                                {
                                    printf("mfhi $t0\n");
                                }
                                else
                                {
                                    printf("mfhi $t%d\n", tempCount + 1);
                                }
                                if (tempCount == 9)
                                {
                                    tempCount = 0;
                                }
                                else
                                {
                                    tempCount++;
                                }
                            }
                        }
                    }

                    else if (left1 >= 0 && isalpha(equation[k]))
                    {
                        hasTemp++;
                        if (initial > 0)
                        {
                            printf("div $t%d,$s%d\n", tempCount, regCheck(equation[k], variables));

                            switch (equation[k + 1])
                            {
                            case ';':
                                printf("mfhi $s%d\n", variablesIndex);
                                break;

                            default:
                                switch (tempCount)
                                {
                                case 9:
                                    tempCount = 0;
                                    reset = 1;
                                    break;

                                default:
                                    tempCount++;
                                    break;
                                }
                                printf("mfhi $t%d\n", tempCount);
                                break;
                            }
                        }
                    }

                    else if (left1 >= 0 && right1 >= 0)
                    {
                        hasTemp++;

                        switch (right1)
                        {
                        case 0:
                            fprintf(stderr, "Division by zero error\n");
                            break;

                        case 1:
                            if (initial == 0)
                            {
                                printf("li $t%d,%d\n", tempCount, left1);
                            }

                            switch (equation[k + 1])
                            {
                            case ';':
                                switch (rearNeg)
                                {
                                case 1:
                                    printf("sub $s%d,$zero,", regCheck(equation[0], variables));
                                    printf("$t%d\n", tempCount);
                                    break;
                                default:
                                    printf("move $s%d,", regCheck(equation[0], variables));
                                    printf("$t%d\n", tempCount);
                                    break;
                                }

                                switch (tempCount)
                                {
                                case 9:
                                    tempCount = 0;
                                    reset = 1;
                                    break;
                                default:
                                    tempCount++;
                                    break;
                                }
                                break;

                            default:
                                switch (rearNeg)
                                {
                                case 1:
                                    switch (tempCount)
                                    {
                                    case 8:
                                        tempCount = 0;
                                        reset = 1;
                                        break;
                                    default:
                                        tempCount++;
                                        break;
                                    }

                                    switch (reset)
                                    {
                                    case 1:
                                        printf("sub $t0,$zero,$t9\n");
                                        reset = 0;
                                        break;
                                    default:
                                        printf("sub $t%d,$zero,$t%d\n", tempCount, tempCount - 1);
                                        break;
                                    }
                                    break;

                                default:
                                    switch (tempCount)
                                    {
                                    case 8:
                                        tempCount = 0;
                                        reset = 1;
                                        break;
                                    default:
                                        tempCount++;
                                        break;
                                    }

                                    switch (reset)
                                    {
                                    case 1:
                                        printf("move $t0,$t9\n");
                                        reset = 0;
                                        break;
                                    default:
                                        printf("move $t%d,", tempCount);
                                        printf("$t%d\n", tempCount - 1);
                                        break;
                                    }
                                    break;
                                }
                                break;
                            }
                            break;

                        default:
                            ispower = powers(right1);
                            if (initial == 0)
                            {
                                printf("li $t%d,%d\n", tempCount, left1);
                            }

                            if (ispower > 0 && right1 > 1)
                            {
                                if (labelnum > 0)
                                    labelnum++;
                                printf("bltz $t%d,L%d\n", tempCount, labelnum);
                                printf("srl $s%d,$t%d,", regCheck(equation[0], variables), tempCount);

                                printf("%d\n", ispower);

                                if (rearNeg == 1)
                                {
                                    printf("sub $s%d,$zero,$s%d\n", regCheck(equation[0], variables), regCheck(equation[0], variables));
                                }
                                printf("j L%d\n", labelnum + 1);
                                printf("L%d:\n", labelnum);

                                switch (tempCount)
                                {
                                case 9:
                                    tempCount = 0;
                                    reset = 1;
                                    break;
                                default:
                                    tempCount++;
                                    break;
                                }

                                switch (rearNeg)
                                {
                                case 1:
                                    printf("li $t%d,-%d\n", tempCount, right1);
                                    break;
                                default:
                                    printf("li $t%d,%d\n", tempCount, right1);
                                    break;
                                }

                                switch (reset)
                                {
                                case 1:
                                    printf("div $t9,$t0\n");
                                    break;
                                default:
                                    printf("div $t%d,$t%d\n", tempCount - 1, tempCount);
                                    break;
                                }

                                printf("mfhi $s%d\n", regCheck(equation[0], variables));
                                printf("L%d:\n", labelnum + 1);
                                labelnum++;
                                tempCount++;
                            }
                            else if (ispower == 0 && right1 > 0)
                            {
                                switch (tempCount)
                                {
                                case 9:
                                    tempCount = 0;
                                    reset = 1;
                                    break;
                                default:
                                    tempCount++;
                                    break;
                                }

                                switch (rearNeg)
                                {
                                case 1:
                                    printf("li $t%d,-%d\n", tempCount, right1);
                                    break;
                                default:
                                    printf("li $t%d,%d\n", tempCount, right1);
                                    break;
                                }

                                switch (reset)
                                {
                                case 1:
                                    printf("div $t9,$t0\n");
                                    break;
                                default:
                                    printf("div $t%d,$t%d\n", tempCount - 1, tempCount);
                                    break;
                                }

                                switch (equation[k + 1])
                                {
                                case ';':
                                    printf("mfhi $s%d\n", regCheck(equation[0], variables));
                                    switch (tempCount)
                                    {
                                    case 9:
                                        tempCount = 0;
                                        reset = 1;
                                        break;
                                    default:
                                        tempCount++;
                                        break;
                                    }
                                    break;

                                default:
                                    switch (tempCount)
                                    {
                                    case 9:
                                        tempCount = 0;
                                        reset = 1;
                                        break;
                                    default:
                                        tempCount++;
                                        break;
                                    }
                                    printf("mfhi $t%d\n", tempCount);
                                    break;
                                }
                            }
                            break;
                        }
                    }

                    initial++;
                }
            }

            i = k;
            j = i + 1;
            k = j + 1;
            initial++;
        } while (equation[j] != ';');
    }

    fclose(file);
    free(variables);
    free(equation);
    return EXIT_SUCCESS;
}