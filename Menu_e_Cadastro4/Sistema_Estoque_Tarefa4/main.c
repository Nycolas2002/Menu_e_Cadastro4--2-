#include <stdio.h>
#include "arquivo.h"
#include "categoria.h"
#include "produto.h"
#include "util.h"

int main(void) {
    Produto produtos[MAX_PRODUTOS];
    Categoria categorias[MAX_CATEGORIAS];
    int totalCategorias = carregarCategorias(categorias);
    int totalProdutos = carregarProdutos(produtos, categorias, totalCategorias);
    int opcao, resultado;
    int categoriasSalvas, produtosSalvos;

    do {
        printf("\n===== SISTEMA DE ESTOQUE =====\n");
        printf("1 - Cadastrar Produto\n");
        printf("2 - Listar Produtos\n");
        printf("3 - Pesquisar Produto\n");
        printf("4 - Categorias\n");
        printf("5 - Sair\n");
        resultado = lerInteiro("Escolha uma opcao: ", &opcao);
        if (resultado == -1) opcao = 5;
        else if (resultado != 1) opcao = 0;

        switch (opcao) {
            case 1:
                totalProdutos = cadastrarProduto(produtos, totalProdutos,
                                                categorias, totalCategorias);
                break;
            case 2:
                listarProdutos(produtos, totalProdutos, categorias, totalCategorias);
                break;
            case 3:
                pesquisarProduto(produtos, totalProdutos, categorias, totalCategorias);
                break;
            case 4:
                menuCategorias(categorias, &totalCategorias);
                break;
            case 5:
                break;
            default:
                printf("\nOpcao invalida! Digite 1, 2, 3, 4 ou 5.\n");
        }
    } while (opcao != 5);

    /* Persiste as duas listas antes de encerrar o sistema. */
    categoriasSalvas = salvarCategorias(categorias, totalCategorias);
    produtosSalvos = salvarProdutos(produtos, totalProdutos);
    if (categoriasSalvas && produtosSalvos) {
        printf("\nDados salvos. Sistema encerrado.\n");
        return 0;
    }
    printf("\nAtencao: ocorreu um erro ao salvar os dados!\n");
    return 1;
}
