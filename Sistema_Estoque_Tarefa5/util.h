#ifndef UTIL_H
#define UTIL_H

#include <stddef.h>
#include <stdio.h>

/* Funcoes gerais para entrada e validacao de dados. */
void limparBuffer(void);
int lerLinha(FILE *origem, char texto[], size_t tamanho);
int lerTexto(const char *mensagem, char texto[], size_t tamanho);
int lerInteiro(const char *mensagem, int *valor);
int lerReal(const char *mensagem, float *valor);
int converterInteiro(const char *texto, int *valor);
int converterReal(const char *texto, float *valor);
int validarInteiroPositivo(int valor);
int validarInteiroNaoNegativo(int valor);
int validarRealPositivo(float valor);
int validarTextoPreenchido(const char texto[]);
int validarPrateleira(char prateleira);

/* Leituras: 1 = sucesso, 0 = entrada invalida, -1 = fim da entrada. */
#endif
