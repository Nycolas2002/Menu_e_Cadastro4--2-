#ifndef ARQUIVO_H
#define ARQUIVO_H

#include "categoria.h"
#include "produto.h"

/* Cada lista e independente e usa seu proprio arquivo de texto. */
int carregarCategorias(Categoria categorias[]);
int carregarProdutos(Produto produtos[], const Categoria categorias[], int totalCategorias);
int salvarCategorias(const Categoria categorias[], int total);
int salvarProdutos(const Produto produtos[], int total);

#endif
