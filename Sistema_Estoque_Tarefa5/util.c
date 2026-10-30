#include <ctype.h>
#include <errno.h>
#include <limits.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include "util.h"

void limparBuffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {
        /* Descarta os caracteres restantes da linha. */
    }
}

int lerLinha(FILE *origem, char texto[], size_t tamanho) {
    size_t comprimento;
    int proximo;

    if (tamanho < 2 || fgets(texto, (int)tamanho, origem) == NULL) {
        return -1;
    }
    comprimento = strlen(texto);
    if (comprimento > 0 && texto[comprimento - 1] == '\n') {
        texto[--comprimento] = '\0';
    } else {
        /* Aceita o limite exato do campo e rejeita linhas muito longas. */
        proximo = fgetc(origem);
        if (proximo != '\n' && proximo != EOF) {
            while (proximo != '\n' && proximo != EOF) {
                proximo = fgetc(origem);
            }
            return 0;
        }
    }
    if (comprimento > 0 && texto[comprimento - 1] == '\r') {
        texto[comprimento - 1] = '\0';
    }
    return 1;
}

int lerTexto(const char *mensagem, char texto[], size_t tamanho) {
    printf("%s", mensagem);
    fflush(stdout);
    return lerLinha(stdin, texto, tamanho);
}

int converterInteiro(const char *texto, int *valor) {
    char *fim;
    long numero;
    errno = 0;
    numero = strtol(texto, &fim, 10);
    if (texto == fim || errno == ERANGE || numero < INT_MIN || numero > INT_MAX) {
        return 0;
    }
    while (isspace((unsigned char)*fim)) {
        fim++;
    }
    if (*fim != '\0') {
        return 0;
    }
    *valor = (int)numero;
    return 1;
}

int converterReal(const char *texto, float *valor) {
    char *fim;
    float numero;
    errno = 0;
    numero = strtof(texto, &fim);
    if (texto == fim || errno == ERANGE || !isfinite(numero)) {
        return 0;
    }
    while (isspace((unsigned char)*fim)) {
        fim++;
    }
    if (*fim != '\0') {
        return 0;
    }
    *valor = numero;
    return 1;
}

int lerInteiro(const char *mensagem, int *valor) {
    char texto[128];
    int resultado = lerTexto(mensagem, texto, sizeof(texto));
    return resultado == 1 ? converterInteiro(texto, valor) : resultado;
}

int lerReal(const char *mensagem, float *valor) {
    char texto[128];
    int resultado = lerTexto(mensagem, texto, sizeof(texto));
    return resultado == 1 ? converterReal(texto, valor) : resultado;
}

int validarInteiroPositivo(int valor) {
    return valor > 0;
}

int validarInteiroNaoNegativo(int valor) {
    return valor >= 0;
}

int validarRealPositivo(float valor) {
    return isfinite(valor) && valor > 0;
}

int validarTextoPreenchido(const char texto[]) {
    if (texto == NULL) {
        return 0;
    }
    while (*texto != '\0') {
        if (!isspace((unsigned char)*texto)) {
            return 1;
        }
        texto++;
    }
    return 0;
}

int validarPrateleira(char prateleira) {
    return prateleira == 'A' || prateleira == 'B' ||
           prateleira == 'C' || prateleira == 'D';
}
