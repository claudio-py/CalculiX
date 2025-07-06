/*
My approach was to create a parser, that is, a program that analyzes and converts a string into valid mathematical operations in C.
To use this code, you type the complete mathematical expression you want to operate.
You can also perform multiple operations at once, see the examples:
- sqrt(144) // for square root
- 1+2+1+5-8 //basic operations
- 5^2 // power
Just as in mathematics, you can also perform complex operations by separating scopes with parentheses.
- sqrt((3+4+5)/4)
-((5*3)^2)/4

*/

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <math.h>
#include <setjmp.h>

jmp_buf error_handler;

// Pointer to the input expression
const char *input;
// Current character being analyzed
char current_char;

void skip_whitespace(); // Forward declaration

// Advance to the next character in the expression
void next_char() {
    current_char = *input++;
}

// Forward declaration of the main evaluation function
double expression();

// Displays an error message and terminates the program
void error(const char *message) {
    printf("Error: %s\n", message);
    longjmp(error_handler, 1);
}

// Skips whitespace characters
void skip_whitespace() {
    while (isspace(current_char)) {
        next_char();
    }
}

// Reads a number from the expression, including the decimal part
double number() {
    double result = 0;
    while (isdigit(current_char)) {
        result = result * 10 + (current_char - '0');
        next_char();
    }

    // Handles the decimal part, if it exists
    if (current_char == '.') {
        next_char();
        double fraction = 0, divisor = 10;
        while (isdigit(current_char)) {
            fraction += (current_char - '0') / divisor;
            divisor *= 10;
            next_char();
        }
        result += fraction;
    }

    return result;
}

// Function that handles factors: function calls (e.g. sqrt), parentheses or numbers
double base();

// Parses function calls like sqrt(...)
double parse_function() {
    char function_name[10];
    int i = 0;

    // Reads the function name (e.g. sqrt)
    while (isalpha(current_char) && i < 9) {
        function_name[i++] = current_char;
        next_char();
    }
    function_name[i] = '\0';

    skip_whitespace();

    // Expects the next part to be an opening parenthesis
    if (current_char != '(')
        error("Expected '(' after function name");

    next_char(); // Skip '('
    double argument = expression(); // Evaluates what is inside the parentheses
    if (current_char != ')') error("Expected ')' after function argument");
    next_char(); // Skip ')'

    // Only supports sqrt for now
    if (strcmp(function_name, "sqrt") == 0) return sqrt(argument);
    else if (strcmp(function_name, "raiz") == 0) return sqrt(argument); // Keep support for "raiz"
    else error("Unknown function");
    return 0;
}

// Returns the value of a numeric base: number, parentheses or function
double base() {
    skip_whitespace();
    if (isalpha(current_char)) {
        return parse_function(); // calls function like sqrt(...)
    } else if (current_char == '(') {
        next_char();
        double result = expression(); // evaluates subexpression in parentheses
        skip_whitespace();
        if (current_char != ')') error("Expected ')'");
        next_char();
        return result;
    } else if (isdigit(current_char) || current_char == '.') {
        return number(); // reads a number (integer or decimal)
    } else if (current_char == '-') { // Handle unary minus
        next_char();
        return -base();
    } else {
        error("Unexpected character in base()");
        return 0;
    }
}

// Handles power operation (exponentiation with ^), right-associative
double power() {
    double left = base();
    skip_whitespace();
    while (current_char == '^') {
        next_char();
        double right = power(); // power is right-associative
        left = pow(left, right);
        skip_whitespace();
    }
    return left;
}

// Handles multiplication and division
double term() {
    double result = power();
    skip_whitespace();
    while (current_char == '*' || current_char == '/') {
        char op = current_char;
        next_char();
        double right = power();
        if (op == '*') result *= right;
        else {
            if (right == 0.0) error("Division by zero");
            result /= right;
        }
        skip_whitespace();
    }
    return result;
}

// Handles addition and subtraction
double expression() {
    double result = term();
    skip_whitespace();
    while (current_char == '+' || current_char == '-') {
        char op = current_char;
        next_char();
        double right = term();
        if (op == '+') result += right;
        else result -= right;
        skip_whitespace();
    }
    return result;
}

// Main function of the program
int main() {
    char operation[256];

    printf("Enter the expression (or type 'exit' to quit): ");
    while (fgets(operation, 256, stdin) != NULL) {
        // Remove newline character from fgets
        operation[strcspn(operation, "\n")] = 0;

        // Check for exit condition
        if (strcmp(operation, "exit") == 0) {
            break;
        }

        if (setjmp(error_handler)) {
            // Error occurred, loop will continue
        } else {
            input = operation;
            next_char(); // Initialize with the first character

            // Check for empty input
            skip_whitespace();
            if (current_char == '\0') {
                printf("Enter the expression (or type 'exit' to quit): ");
                continue;
            }

            double result = expression();

            // Checks if the entire input was consumed correctly
            skip_whitespace();
            if (current_char != '\0') {
                printf("Error: Unexpected character '%c'\n", current_char);
            } else {
                // Prints the result
                printf("Result: %g\n", result);
            }
        }

        printf("Enter the expression (or type 'exit' to quit): ");
    }

    printf("\nExiting calculator.\n");
    return 0;
}
