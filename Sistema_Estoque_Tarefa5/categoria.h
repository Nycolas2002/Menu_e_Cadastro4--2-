#ifndef CATEGORIA_H
#define CATEGORIA_H

#define MAX_CATEGORIAS 20

typedef struct {
    int codigo;
    char nome[30];
} Categoria;

int buscarCategoriaPorCodigo(const Categoria categorias[], int total, int codigo);
int cadastrarCategoria(Categoria categorias[], int total);
void listarCategorias(const Categoria categorias[], int total);
void menuCategorias(Categoria categorias[], int *total,
                    const int codigosCategoriasEmUso[], int totalProdutos);

#endif
