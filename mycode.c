#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Function prototypes
void processFile(FILE *inputFile, FILE *outputFile);
int isValidRoman(char *roman);
int romanToDecimal(char *roman);
void numberToWords(int num, char *output);
int performOperation(int num1, int num2, char operation);

// Main function to open files and call other functions
int main() {
    FILE *inputFile = fopen("input.txt", "r");
    FILE *outputFile = fopen("output.txt", "w");

    // Check if the files are opened successfully
    if (inputFile == NULL || outputFile == NULL) {
        printf("Error opening files. Ensure input.txt exists.\n");
        return 1;
    }

    // Process the input file and generate output
    processFile(inputFile, outputFile);

    printf("Processing complete. Check output.txt for results.\n");

    // Close the files
    fclose(inputFile);
    fclose(outputFile);
    return 0;
}

// Function to validate Roman numerals
int isValidRoman(char *roman) {
    for (int i = 0; roman[i] != '\0'; i++) {
        if (roman[i] != 'I' && roman[i] != 'V' && roman[i] != 'X' && roman[i] != 'L' && roman[i] != 'C' && roman[i] != 'D' && roman[i] != 'M') {
            return 0;  // Invalid Roman numeral
        }
    }
    return 1;  // Valid Roman numeral
}

// Function to convert Roman numerals to decimals
int romanToDecimal(char *roman) {
    int total = 0;
    int i;
    // Loop through each character in the Roman numeral string
    for (i = 0; roman[i] != '\0'; i++) {
        int current = 0;

        // Check the Roman numeral and assign its value
        if (roman[i] == 'I') current = 1;
        else if (roman[i] == 'V') current = 5;
        else if (roman[i] == 'X') current = 10;
        else if (roman[i] == 'L') current = 50;
        else if (roman[i] == 'C') current = 100;
        else if (roman[i] == 'D') current = 500;
        else if (roman[i] == 'M') current = 1000;

        // Adjust value if the next numeral is larger (subtract logic)
        if (roman[i + 1] != '\0') {
            if (roman[i] == 'I' && (roman[i + 1] == 'V' || roman[i + 1] == 'X')) current = -1;
            if (roman[i] == 'X' && (roman[i + 1] == 'L' || roman[i + 1] == 'C')) current = -10;
            if (roman[i] == 'C' && (roman[i + 1] == 'D' || roman[i + 1] == 'M')) current = -100;
        }

        total += current; // Add the value to the total
    }
    return total;
}

// Function to convert a number to words
void numberToWords(int num, char *output) {
    // Arrays for ones, teens, tens, and thousands to map numbers to words
    char *ones[] = {"", "One", "Two", "Three", "Four", "Five", "Six", "Seven", "Eight", "Nine"};
    char *teens[] = {"Ten", "Eleven", "Twelve", "Thirteen", "Fourteen", "Fifteen", "Sixteen", "Seventeen", "Eighteen", "Nineteen"};
    char *tens[] = {"", "", "Twenty", "Thirty", "Forty", "Fifty", "Sixty", "Seventy", "Eighty", "Ninety"};
    char *thousands[] = {"", "Thousand", "Million", "Billion"};

    if (num == 0) {
        strcpy(output, "Zero");
        return;
    }

    char result[1000] = "";  // Temporary result string
    if (num < 0) {
        strcat(result, "Minus ");  // Handle negative numbers
        num = -num; // Work with positive value for conversion
    }

    int part, group = 0;
    // Process the number in groups of 1000 (thousands, millions, etc.)
    while (num > 0) {
        part = num % 1000;  // Get the last 3 digits
        num /= 1000;  // Remove the last 3 digits

        if (part > 0) {
            char temp[100] = "";  // Temporary string to hold words for the current group

            // Process hundreds
            if (part / 100 > 0) {
                strcat(temp, ones[part / 100]);
                strcat(temp, " Hundred ");
                part %= 100;
            }

            // Process tens and ones
            if (part >= 10 && part < 20) {
                strcat(temp, teens[part - 10]);
            } else {
                if (part / 10 > 0) {
                    strcat(temp, tens[part / 10]);
                    strcat(temp, " ");
                }
                if (part % 10 > 0) {
                    strcat(temp, ones[part % 10]);
                }
            }

            // Add group (thousand, million, etc.)
            if (group > 0) {
                strcat(temp, " ");
                strcat(temp, thousands[group]);
            }

            // Combine the temporary result with the final result
            if (strlen(result) > 0) {
                char tempResult[1000];
                strcpy(tempResult, temp);
                strcat(tempResult, " ");
                strcat(tempResult, result);
                strcpy(result, tempResult);
            } else {
                strcpy(result, temp);
            }
        }
        group++;  // Move to the next group (thousands, millions, etc.)
    }

    // Remove trailing spaces
    int len = strlen(result);
    if (len > 0 && result[len - 1] == ' ') {
        result[len - 1] = '\0';
    }

    strcpy(output, result);  // Copy the final result to the output
}

// Function to validate and perform arithmetic operations (addition, subtraction, multiplication, division)
int performOperation(int num1, int num2, char operation) {
    int result = 0;
    if (operation != '+' && operation != '-' && operation != '*' && operation != '/') {
        return -1;  // Invalid operator
    }
    switch (operation) {
        case '+': 
            result = num1 + num2;
            break;
        case '-': 
            result = num1 - num2;
            break;
        case '*': 
            result = num1 * num2;
            break;
        case '/': 
            if (num2 == 0) {
                return -2;  // Division by zero error
            }
            result = num1 / num2;
            break;
    }
    return result;
}

// Function to read data from a file and process operations
void processFile(FILE *inputFile, FILE *outputFile) {
    char line[256];
    while (fgets(line, sizeof(line), inputFile)) {
        // Parse Roman numerals and operator
        char roman1[100], roman2[100], operation;
        if (sscanf(line, "%s %c %s", roman1, &operation, roman2) != 3) {
            fprintf(outputFile, "Invalid input format in line: %s", line);
            continue;
        }

        // Validate Roman numerals and operator
        if (!isValidRoman(roman1) || !isValidRoman(roman2)) {
            fprintf(outputFile, "Invalid Roman numeral in line: %s", line);
            continue;
        }

        if (performOperation(0, 0, operation) == -1) {
            fprintf(outputFile, "Invalid operator in line: %s", line);
            continue;
        }

        // Convert Roman numerals to decimals
        int num1 = romanToDecimal(roman1);
        int num2 = romanToDecimal(roman2);

        // Perform the arithmetic operation using the performOperation function
        int result = performOperation(num1, num2, operation);
        if (result == -1) {
            fprintf(outputFile, "Error: Invalid operator in line: %s", line);
            continue;
        } else if (result == -2) {
            fprintf(outputFile, "Error: Division by zero in line: %s", line);
            continue;
        }

        // Convert result to words using the numberToWords function
        char wordResult[1000];
        numberToWords(result, wordResult);

        // Write the result to the output file
        fprintf(outputFile, "%s\n", wordResult);
    }
}
