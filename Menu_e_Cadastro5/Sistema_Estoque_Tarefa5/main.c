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
        printf("4 - Alterar Estoque de um Produto\n");
        printf("5 - Remover Produto\n");
        printf("6 - Categorias\n");
        printf("7 - Sair\n");

        resultado = lerInteiro("Escolha uma opcao: ", &opcao);
        if (resultado == -1) opcao = 7;
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
                alterarEstoqueProduto(produtos, totalProdutos);
                break;
            case 5:
                removerProduto(produtos, totalProdutos);
                break;
            case 6:
                menuCategorias(categorias, &totalCategorias);
                break;
            case 7:
                break;
            default:
                printf("\nOpcao invalida! Digite um numero de 1 a 7.\n");
        }
    } while (opcao != 7);

    categoriasSalvas = salvarCategorias(categorias, totalCategorias);
    produtosSalvos = salvarProdutos(produtos, totalProdutos);
    if (categoriasSalvas && produtosSalvos) {
        printf("\nDados salvos. Sistema encerrado.\n");
        return 0;
    }
    printf("\nAtencao: ocorreu um erro ao salvar os dados!\n");
    return 1;
}
