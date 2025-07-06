/*
minha abordadagem foi a criação de um parser, ou seja, um programa que analiza e converte uma string, que no caso, é em operações matematicas validas em C.
Para utilizar esse codigo voce digita a expressão matemática completa que voce deseja operar,
você pode também realizar multiplas operações de uma unica vez, veja os exemplos:
- raiz(144) // para raiz quadrada
- 1+2+1+5-8 //operações basicas
- 5^2 // potencia
assim como na matematica voce pode tambem realizar operações complexas separando escopos com parenteses
- raiz((3+4+5)/4)
-((5*3)^2)/4

*/

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <math.h>

// Ponteiro para a expressão de entrada
const char *entrada;
// Caractere atual sendo analisado
char atual;

// Avança para o próximo caractere na expressão
void proximo() {
    atual = *entrada++;
}

// Declaração antecipada da função principal de avaliação
double expressao();

// Exibe mensagem de erro e encerra o programa
void erro(const char *mensagem) {
    printf("Erro: %s\n", mensagem);
    exit(1);
}

// Lê um número da expressão, incluindo parte decimal
double numero() {
    double resultado = 0;
    while (isdigit(atual)) {
        resultado = resultado * 10 + (atual - '0');
        proximo();
    }

    // Trata parte decimal, se existir
    if (atual == '.') {
        proximo();
        double fracao = 0, divisor = 10;
        while (isdigit(atual)) {
            fracao += (atual - '0') / divisor;
            divisor *= 10;
            proximo();
        }
        resultado += fracao;
    }

    return resultado;
}

// Função que trata os fatores: chamadas de função (ex: sqrt), parênteses ou números
double fator();

// Analisa chamadas de funções como sqrt(...)
double funcao() {
    char nome_funcao[10];
    int i = 0;

    // Lê o nome da função (ex: sqrt)
    while (isalpha(atual) && i < 9) {
        nome_funcao[i++] = atual;
        proximo();
    }
    nome_funcao[i] = '\0';

    // Espera que a próxima parte seja um parêntese de abertura
    if (atual != '(')
        erro("Esperado '(' após nome de função");

    proximo(); // Pula '('
    double argumento = expressao(); // Avalia o que está dentro dos parênteses
    if (atual != ')') erro("Esperado ')' após argumento da função");
    proximo(); // Pula ')'

    // Suporta apenas sqrt por enquanto
    if (strcmp(nome_funcao, "raiz") == 0) return sqrt(argumento);
    else erro("Função desconhecida");
    return 0;
}

// Retorna o valor de uma base numérica: número, parênteses ou função
double base() {
    if (isalpha(atual)) {
        return funcao(); // chama função como sqrt(...)
    } else if (atual == '(') {
        proximo();
        double resultado = expressao(); // avalia subexpressão entre parênteses
        if (atual != ')') erro("Esperado ')'");
        proximo();
        return resultado;
    } else if (isdigit(atual) || atual == '.') {
        return numero(); // lê um número (inteiro ou decimal)
    } else {
        erro("Caractere inesperado em base()");
        return 0;
    }
}

// Trata operação de potência (exponenciação com ^), associativa à direita
double potencia() {
    double esquerda = base();
    while (atual == '^') {
        proximo();
        double direita = potencia(); // potência é associativa à direita
        esquerda = pow(esquerda, direita);
    }
    return esquerda;
}

// Trata multiplicação e divisão
double termo() {
    double resultado = potencia();
    while (atual == '*' || atual == '/') {
        char operador = atual;
        proximo();
        double direita = potencia();
        if (operador == '*') resultado *= direita;
        else {
            if (direita == 0.0) erro("Divisão por zero");
            resultado /= direita;
        }
    }
    return resultado;
}

// Trata adição e subtração
double expressao() {
    double resultado = termo();
    while (atual == '+' || atual == '-') {
        char operador = atual;
        proximo();
        double direita = termo();
        if (operador == '+') resultado += direita;
        else resultado -= direita;
    }
    return resultado;
}

// Função principal do programa
int main() {
    char continuar = 'y';

    while(continuar == 'y'){
   char operacao[256];

    printf("Digite a expressao: ");
    scanf("%255s", operacao);

    entrada = operacao;
    proximo(); // Inicializa com o primeiro caractere
    double resultado = expressao();

    // Verifica se toda a entrada foi consumida corretamente
    if (atual != '\0') {
        printf("Erro: Caractere inesperado '%c'\n", atual);
        return 1;
    }

    // Imprime o resultado com 2 casas decimais
    printf("Resultado: %.2lf\n", resultado);
    printf("digite y para continuar ou n para terminar\n");
    scanf(" %c", &continuar);
    
    }
    return 0;
}
