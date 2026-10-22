#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include "produto.h"
#include "util.h"

void exibirProduto(Produto produto, const Categoria categorias[], int totalCategorias) {
    int posicao = buscarCategoriaPorCodigo(categorias, totalCategorias,
                                           produto.codigo_categoria);
    printf("\n===== DADOS DO PRODUTO =====\n");
    printf("Codigo: %d\n", produto.codigo);
    printf("Nome: %s\n", produto.nome);
    printf("Prateleira: %c\n", produto.prateleira);
    printf("Preco unitario: R$ %.2f\n", produto.preco);
    printf("Quantidade em estoque: %d\n", produto.quantidade);
    printf("Disponivel para venda: %d\n", produto.disponivel);
    printf("Categoria: %s\n", posicao != -1 ? categorias[posicao].nome : "Desconhecida");
}

int buscarPorCodigo(const Produto produtos[], int total, int codigo) {
    int i;
    for (i = 0; i < total; i++) {
        if (produtos[i].codigo == codigo) return i;
    }
    return -1;
}

int buscarPorNome(const Produto produtos[], int total, const char nome[]) {
    int i;
    for (i = 0; i < total; i++) {
        if (strcmp(produtos[i].nome, nome) == 0) return i;
    }
    return -1;
}

int cadastrarProduto(Produto produtos[], int total,
                    const Categoria categorias[], int totalCategorias) {
    Produto produto;
    char texto[20];
    int resultado;

    if (total >= MAX_PRODUTOS) {
        printf("\nA lista de produtos esta cheia! Limite: %d produtos.\n", MAX_PRODUTOS);
        return total;
    }
    if (totalCategorias == 0) {
        printf("\nCadastre ao menos uma categoria na opcao 4 antes de cadastrar um produto.\n");
        return total;
    }

    while (1) {
        resultado = lerInteiro("\nDigite o codigo do produto: ", &produto.codigo);
        if (resultado == -1) return total;
        if (resultado != 1 || !validarInteiroPositivo(produto.codigo)) {
            printf("Codigo invalido! Digite um numero positivo.\n");
        } else if (buscarPorCodigo(produtos, total, produto.codigo) != -1) {
            printf("Esse codigo ja esta cadastrado! Digite outro codigo.\n");
        } else {
            break;
        }
    }

    while (1) {
        resultado = lerTexto("Digite o nome do produto: ", produto.nome,
                             sizeof(produto.nome));
        if (resultado == -1) return total;
        if (resultado != 1) {
            printf("Nome muito longo! Utilize no maximo 49 caracteres.\n");
        } else if (!validarTextoPreenchido(produto.nome)) {
            printf("O nome nao pode ficar em branco.\n");
        } else {
            break;
        }
    }

    while (1) {
        resultado = lerTexto("Digite a prateleira (A, B, C ou D): ", texto, sizeof(texto));
        if (resultado == -1) return total;
        produto.prateleira = resultado == 1 && strlen(texto) == 1
                              ? (char)toupper((unsigned char)texto[0]) : '\0';
        if (validarPrateleira(produto.prateleira)) break;
        printf("Prateleira invalida! Digite A, B, C ou D.\n");
    }

    while (1) {
        resultado = lerReal("Digite o preco unitario: R$ ", &produto.preco);
        if (resultado == -1) return total;
        if (resultado == 1 && validarRealPositivo(produto.preco)) break;
        printf("O preco deve ser um numero maior que zero (use ponto decimal).\n");
    }

    while (1) {
        resultado = lerInteiro("Digite a quantidade em estoque: ", &produto.quantidade);
        if (resultado == -1) return total;
        if (resultado == 1 && validarInteiroNaoNegativo(produto.quantidade)) break;
        printf("A quantidade deve ser um numero inteiro nao negativo.\n");
    }

    /* Referencia por codigo, nunca por copia da struct Categoria. */
    listarCategorias(categorias, totalCategorias);
    while (1) {
        resultado = lerInteiro("Digite o codigo da categoria do produto: ",
                               &produto.codigo_categoria);
        if (resultado == -1) return total;
        if (resultado == 1 && buscarCategoriaPorCodigo(categorias, totalCategorias,
                                                       produto.codigo_categoria) != -1) break;
        printf("Categoria inexistente! Informe um codigo da lista acima.\n");
    }

    produto.disponivel = produto.quantidade > 0 ? 1 : 0;
    produtos[total++] = produto;
    printf("\nProduto cadastrado com sucesso!\n");
    exibirProduto(produto, categorias, totalCategorias);
    return total;
}

void listarProdutos(const Produto produtos[], int total,
                   const Categoria categorias[], int totalCategorias) {
    int i;
    if (total == 0) {
        printf("\nNenhum produto foi cadastrado.\n");
        return;
    }
    printf("\n===== LISTA DE PRODUTOS =====\n");
    for (i = 0; i < total; i++) {
        exibirProduto(produtos[i], categorias, totalCategorias);
    }
}

void pesquisarProduto(const Produto produtos[], int total,
                     const Categoria categorias[], int totalCategorias) {
    int opcao, codigo, posicao, resultado;
    char nome[50];

    printf("\n===== PESQUISAR PRODUTO =====\n");
    printf("1 - Por codigo\n");
    printf("2 - Por nome\n");
    resultado = lerInteiro("Escolha uma opcao: ", &opcao);
    if (resultado != 1) {
        if (resultado != -1) printf("Opcao invalida!\n");
        return;
    }

    if (opcao == 1) {
        resultado = lerInteiro("Digite o codigo do produto: ", &codigo);
        if (resultado != 1) {
            if (resultado != -1) printf("Codigo invalido!\n");
            return;
        }
        posicao = buscarPorCodigo(produtos, total, codigo);
    } else if (opcao == 2) {
        resultado = lerTexto("Digite o nome do produto: ", nome, sizeof(nome));
        if (resultado != 1) {
            if (resultado != -1) printf("Nome invalido!\n");
            return;
        }
        posicao = buscarPorNome(produtos, total, nome);
    } else {
        printf("Opcao invalida! Digite 1 ou 2.\n");
        return;
    }

    if (posicao != -1) {
        exibirProduto(produtos[posicao], categorias, totalCategorias);
    } else {
        printf("\nNenhum produto foi encontrado.\n");
    }
}
