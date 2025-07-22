
#ifndef CALCULATOR_H

#define CALCULATOR_H

/*
esta calculadora trata se de um parser, ou seja, um programa que analisa e converte
uma string em operações matemáticas válidas em C.  veja os exemplos:
- r(144) // para raiz quadrada
- 1+2+1+5-8 // operações básicas
- 5^2 // potência
Assim como na matemática, você pode realizar operações complexas separando
escopos com parênteses.
- r((3+4+5)/4)
-((5*3)^2)/4

*/

#include <stdio.h>
#include <ctype.h>
#include <math.h>
#include <setjmp.h>
#include <string.h>

// Buffer para manipulação de erros
jmp_buf error_handler;

// Ponteiro para a expressão de entrada
const char *input;

// Caractere atual sendo analisado
char current_char;

void skip_whitespace(); // Declaração antecipada

// Avança para o próximo caractere na expressão
void next_char() { current_char = *input++; }

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

// Função que trata fatores: chamadas de função (ex: sqrt), parênteses ou
// números
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

  next_char();                    // Pula '('
  double argument = expression(); // Avalia o que está dentro dos parênteses
  if (current_char != ')')
    error("Esperado ')' após o argumento da função");
  next_char(); // Pula ')'

//aceita reconhece r como substituto para funcao sqrt
  if (strcmp(function_name, "r") == 0)
    return sqrt(argument);
  else
    error("Função desconhecida");
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
    if (current_char != ')')
      error("Esperado ')'");
    next_char();
    return result;
  } else if (isdigit(current_char) || current_char == '.') {
    return number();                // lê um número (inteiro ou decimal)
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
    if (op == '*')
      result *= right;
    else {
      if (right == 0.0)
        error("Divisão por zero");
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
    if (op == '+')
      result += right;
    else
      result -= right;
    skip_whitespace();
  }
  return result;
}

// Função principal do programa
char *backend(char operation[]) {
  static char result_str[64]; // static buffer to hold the result string

  // Remove newline character from operation
  operation[strcspn(operation, "\n")] = 0;

  if (setjmp(error_handler)) {
    // An error occurred
    return "erro inesperado";
  } else {
    input = operation;
    next_char(); // Initialize with the first character

    double result = expression();

    // Check if the entire input was consumed correctly
    skip_whitespace();
    if (current_char != '\0') {
      return "erro inesperado";
    }
    snprintf(result_str, sizeof(result_str), "%g", result);
    return result_str;
  }
}

#endif
