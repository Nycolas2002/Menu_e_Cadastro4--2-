#ifndef PRODUTO_H
#define PRODUTO_H

#include "categoria.h"

#define MAX_PRODUTOS 50

typedef struct {
    int codigo;
    char nome[50];
    char prateleira;
    float preco;
    int quantidade;
    int disponivel;
    int codigo_categoria;  /* Referencia ao codigo de uma Categoria. */
} Produto;

void exibirProduto(Produto produto, const Categoria categorias[], int totalCategorias);
int buscarPorCodigo(const Produto produtos[], int total, int codigo);
int buscarPorNome(const Produto produtos[], int total, const char nome[]);
int cadastrarProduto(Produto produtos[], int total,
                    const Categoria categorias[], int totalCategorias);
void listarProdutos(const Produto produtos[], int total,
                   const Categoria categorias[], int totalCategorias);
void pesquisarProduto(const Produto produtos[], int total,
                     const Categoria categorias[], int totalCategorias);

#endif
