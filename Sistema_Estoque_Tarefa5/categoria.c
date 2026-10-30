#include <stdio.h>
#include "categoria.h"
#include "util.h"

int buscarCategoriaPorCodigo(const Categoria categorias[], int total, int codigo) {
    int i;
    for (i = 0; i < total; i++) {
        if (categorias[i].codigo == codigo) {
            return i;
        }
    }
    return -1;
}

int cadastrarCategoria(Categoria categorias[], int total) {
    Categoria categoria;
    int resultado;

    if (total >= MAX_CATEGORIAS) {
        printf("\nLimite de %d categorias atingido.\n", MAX_CATEGORIAS);
        return total;
    }

    while (1) {
        resultado = lerInteiro("\nDigite o codigo da categoria: ", &categoria.codigo);
        if (resultado == -1) return total;
        if (resultado != 1 || !validarInteiroPositivo(categoria.codigo)) {
            printf("Codigo invalido! Digite um numero inteiro positivo.\n");
        } else if (buscarCategoriaPorCodigo(categorias, total, categoria.codigo) != -1) {
            printf("Esse codigo de categoria ja esta cadastrado!\n");
        } else {
            break;
        }
    }

    while (1) {
        resultado = lerTexto("Digite o nome da categoria: ", categoria.nome,
                             sizeof(categoria.nome));
        if (resultado == -1) return total;
        if (resultado != 1) {
            printf("Nome muito longo! Utilize no maximo 29 caracteres.\n");
        } else if (!validarTextoPreenchido(categoria.nome)) {
            printf("O nome da categoria nao pode ficar em branco.\n");
        } else {
            break;
        }
    }

    categorias[total++] = categoria;
    printf("\nCategoria cadastrada com sucesso: %s (codigo %d).\n",
           categoria.nome, categoria.codigo);
    return total;
}

void listarCategorias(const Categoria categorias[], int total) {
    int i;
    if (total == 0) {
        printf("\nNenhuma categoria foi cadastrada.\n");
        return;
    }
    printf("\n===== LISTA DE CATEGORIAS =====\n");
    for (i = 0; i < total; i++) {
        printf("Codigo: %d | Nome: %s\n", categorias[i].codigo, categorias[i].nome);
    }
}

static void removerCategoria(Categoria categorias[], int *total,
                             const int codigosCategoriasEmUso[], int totalProdutos) {
    int codigo, resultado, posicao, i;

    if (*total == 0) {
        printf("\nNenhuma categoria cadastrada para remover.\n");
        return;
    }

    resultado = lerInteiro("\nDigite o codigo da categoria que deseja remover: ", &codigo);
    if (resultado == -1) return;
    if (resultado != 1 || !validarInteiroPositivo(codigo)) {
        printf("Codigo invalido! Digite um numero inteiro positivo.\n");
        return;
    }

    posicao = buscarCategoriaPorCodigo(categorias, *total, codigo);
    if (posicao == -1) {
        printf("Categoria com codigo %d nao encontrada.\n", codigo);
        return;
    }

    for (i = 0; i < totalProdutos; i++) {
        if (codigosCategoriasEmUso[i] == codigo) {
            printf("Nao e possivel remover esta categoria: ela esta vinculada a um produto ativo.\n");
            return;
        }
    }

    for (i = posicao; i < *total - 1; i++) {
        categorias[i] = categorias[i + 1];
    }
    (*total)--;
    printf("Categoria removida com sucesso.\n");
}

void menuCategorias(Categoria categorias[], int *total,
                    const int codigosCategoriasEmUso[], int totalProdutos) {
    int opcao, resultado;
    do {
        printf("\n===== CATEGORIAS =====\n");
        printf("1 - Cadastrar Categoria\n");
        printf("2 - Listar Categorias\n");
        printf("3 - Remover Categoria\n");
        printf("4 - Voltar\n");
        resultado = lerInteiro("Escolha uma opcao: ", &opcao);
        if (resultado == -1) return;
        if (resultado != 1) opcao = 0;

        switch (opcao) {
            case 1:
                *total = cadastrarCategoria(categorias, *total);
                break;
            case 2:
                listarCategorias(categorias, *total);
                break;
            case 3:
                removerCategoria(categorias, total,
                                 codigosCategoriasEmUso, totalProdutos);
                break;
            case 4:
                break;
            default:
                printf("\nOpcao invalida! Digite 1, 2, 3 ou 4.\n");
        }
    } while (opcao != 4);
}
