/*
Minha abordagem foi criar um parser, ou seja, um programa que analisa e converte uma string em operações matemáticas válidas em C.
Para usar este código, digite a expressão matemática completa que deseja operar.
Você também pode realizar múltiplas operações de uma vez, veja os exemplos:
- sqrt(144) // para raiz quadrada
- 1+2+1+5-8 // operações básicas
- 5^2 // potência
Assim como na matemática, você pode realizar operações complexas separando escopos com parênteses.
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

// Ponteiro para a expressão de entrada
const char *input;
// Caractere atual sendo analisado
char current_char;

void skip_whitespace(); // Declaração antecipada

// Avança para o próximo caractere na expressão
void next_char() {
    current_char = *input++;
}

// Declaração antecipada da função principal de avaliação
// (avalia a expressão completa)
double expression();

// Exibe uma mensagem de erro e termina o programa
void error(const char *message) {
    printf("Error: %s\n", message);
    longjmp(error_handler, 1);
}

// Pula caracteres de espaço em branco
void skip_whitespace() {
    while (isspace(current_char)) {
        next_char();
    }
}

// Lê um número da expressão, incluindo a parte decimal
double number() {
    double result = 0;
    while (isdigit(current_char)) {
        result = result * 10 + (current_char - '0');
        next_char();
    }

    // Trata a parte decimal, se existir
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

// Função que trata fatores: chamadas de função (ex: sqrt), parênteses ou números
double base();

// Faz o parsing de chamadas de função como sqrt(...)
double parse_function() {
    char function_name[10];
    int i = 0;

    // Lê o nome da função (ex: sqrt)
    while (isalpha(current_char) && i < 9) {
        function_name[i++] = current_char;
        next_char();
    }
    function_name[i] = '\0';

    skip_whitespace();

    // Espera que a próxima parte seja um parêntese de abertura
    if (current_char != '(')
        error("Esperado '(' após o nome da função");

    next_char(); // Pula '('
    double argument = expression(); // Avalia o que está dentro dos parênteses
    if (current_char != ')') error("Esperado ')' após o argumento da função");
    next_char(); // Pula ')'

    // Suporta apenas sqrt por enquanto
    if (strcmp(function_name, "sqrt") == 0) return sqrt(argument);
    else if (strcmp(function_name, "raiz") == 0) return sqrt(argument); // Suporte para "raiz"
    else error("Função desconhecida");
    return 0;
}

// Retorna o valor de uma base numérica: número, parênteses ou função
double base() {
    skip_whitespace();
    if (isalpha(current_char)) {
        return parse_function(); // chama função como sqrt(...)
    } else if (current_char == '(') {
        next_char();
        double result = expression(); // avalia subexpressão entre parênteses
        skip_whitespace();
        if (current_char != ')') error("Esperado ')'");
        next_char();
        return result;
    } else if (isdigit(current_char) || current_char == '.') {
        return number(); // lê um número (inteiro ou decimal)
    } else if (current_char == '-') { // Trata sinal negativo (unário)
        next_char();
        return -base();
    } else {
        error("Caractere inesperado em base()");
        return 0;
    }
}

// Trata operação de potência (expoente com ^), associatividade à direita
double power() {
    double left = base();
    skip_whitespace();
    while (current_char == '^') {
        next_char();
        double right = power(); // potência é associativa à direita
        left = pow(left, right);
        skip_whitespace();
    }
    return left;
}

// Trata multiplicação e divisão
double term() {
    double result = power();
    skip_whitespace();
    while (current_char == '*' || current_char == '/') {
        char op = current_char;
        next_char();
        double right = power();
        if (op == '*') result *= right;
        else {
            if (right == 0.0) error("Divisão por zero");
            result /= right;
        }
        skip_whitespace();
    }
    return result;
}

// Trata adição e subtração
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

// Função principal do programa
int main() {
    char operation[256];

    printf("Digite a expressão (ou 'exit' para sair): ");
    while (fgets(operation, 256, stdin) != NULL) {
        // Remove o caractere de nova linha do fgets
        operation[strcspn(operation, "\n")] = 0;

        // Verifica condição de saída
        if (strcmp(operation, "exit") == 0) {
            break;
        }

        if (setjmp(error_handler)) {
            // Ocorreu um erro, o loop continuará
        } else {
            input = operation;
            next_char(); // Inicializa com o primeiro caractere

            // Verifica entrada vazia
            skip_whitespace();
            if (current_char == '\0') {
                printf("Digite a expressão (ou 'exit' para sair): ");
                continue;
            }

            double result = expression();

            // Verifica se toda a entrada foi consumida corretamente
            skip_whitespace();
            if (current_char != '\0') {
                printf("Erro: Caractere inesperado '%c'\n", current_char);
            } else {
                // Imprime o resultado
                printf("Resultado: %g\n", result);
            }
        }

        printf("Digite a expressão (ou 'exit' para sair): ");
    }

    printf("\nSaindo da calculadora.\n");
    return 0;
}
