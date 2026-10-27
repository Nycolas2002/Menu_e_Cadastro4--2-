#include <stdio.h>
#include <string.h>
#include "arquivo.h"
#include "util.h"

#define ARQUIVO_CATEGORIAS "categorias.txt"
#define ARQUIVO_PRODUTOS "produtos.txt"
#define MAX_LINHAS_PRODUTOS (MAX_PRODUTOS * 8)

int carregarCategorias(Categoria categorias[]) {
    FILE *arquivo = fopen(ARQUIVO_CATEGORIAS, "r");
    char linha[128];
    Categoria categoria;
    int total = 0, resultado;

    if (arquivo == NULL) return 0;

    while (total < MAX_CATEGORIAS) {
        resultado = lerLinha(arquivo, linha, sizeof(linha));
        if (resultado != 1) break;
        if (!converterInteiro(linha, &categoria.codigo)) break;
        if (lerLinha(arquivo, categoria.nome, sizeof(categoria.nome)) != 1) break;

        if (validarInteiroPositivo(categoria.codigo) &&
            validarTextoPreenchido(categoria.nome) &&
            buscarCategoriaPorCodigo(categorias, total, categoria.codigo) == -1) {
            categorias[total++] = categoria;
        }
    }
    fclose(arquivo);
    return total;
}

static int validarRegistroProduto(const Produto *p, const Produto produtos[],
                                  int total, const Categoria categorias[],
                                  int totalCategorias) {
    int i;
    if (!validarInteiroPositivo(p->codigo) ||
        !validarTextoPreenchido(p->nome) ||
        !validarPrateleira(p->prateleira) ||
        !validarRealPositivo(p->preco) ||
        !validarInteiroNaoNegativo(p->quantidade) ||
        (p->ativo != 0 && p->ativo != 1) ||
        buscarCategoriaPorCodigo(categorias, totalCategorias, p->codigo_categoria) == -1) {
        return 0;
    }
    for (i = 0; i < total; i++) {
        if (produtos[i].codigo == p->codigo) return 0;
    }
    return 1;
}

/* Tenta ler o formato indicado: sete campos na Tarefa 4 ou oito na Tarefa 5. */
static int interpretarProdutos(char linhas[][128], int quantidadeLinhas, int campos,
                               Produto destino[], const Categoria categorias[],
                               int totalCategorias) {
    Produto temporarios[MAX_PRODUTOS];
    int total = 0, i, j, valor, ok;

    if (quantidadeLinhas == 0) return 0;
    if (quantidadeLinhas % campos != 0 ||
        quantidadeLinhas / campos > MAX_PRODUTOS) return -1;

    for (i = 0; i < quantidadeLinhas; i += campos) {
        Produto p;
        ok = converterInteiro(linhas[i], &p.codigo);
        if (!ok) return -1;
        if (strlen(linhas[i + 1]) >= sizeof(p.nome)) return -1;
        strcpy(p.nome, linhas[i + 1]);
        if (strlen(linhas[i + 2]) != 1) return -1;
        p.prateleira = linhas[i + 2][0];
        if (!converterReal(linhas[i + 3], &p.preco)) return -1;
        if (!converterInteiro(linhas[i + 4], &p.quantidade)) return -1;
        /* Campo antigo de disponibilidade é lido para validar o formato; o valor é recalculado. */
        if (!converterInteiro(linhas[i + 5], &valor) || (valor != 0 && valor != 1)) return -1;
        if (!converterInteiro(linhas[i + 6], &p.codigo_categoria)) return -1;

        if (campos == 8) {
            if (!converterInteiro(linhas[i + 7], &p.ativo)) return -1;
        } else {
            p.ativo = 1; /* Registros da Tarefa 4 passam a ser ativos. */
        }
        p.disponivel = p.quantidade > 0 ? 1 : 0;

        if (!validarRegistroProduto(&p, temporarios, total, categorias, totalCategorias)) {
            return -1;
        }
        temporarios[total++] = p;
    }

    for (j = 0; j < total; j++) destino[j] = temporarios[j];
    return total;
}

int carregarProdutos(Produto produtos[], const Categoria categorias[], int totalCategorias) {
    FILE *arquivo = fopen(ARQUIVO_PRODUTOS, "r");
    char linhas[MAX_LINHAS_PRODUTOS][128];
    int quantidadeLinhas = 0, resultado, total;

    if (arquivo == NULL) return 0;

    while (quantidadeLinhas < MAX_LINHAS_PRODUTOS) {
        resultado = lerLinha(arquivo, linhas[quantidadeLinhas],
                             sizeof(linhas[quantidadeLinhas]));
        if (resultado == -1) break;
        if (resultado == 0) {
            fclose(arquivo);
            fprintf(stderr, "Aviso: produtos.txt contem uma linha muito longa; dados nao carregados por completo.\n");
            return 0;
        }
        quantidadeLinhas++;
    }
    fclose(arquivo);

    if (quantidadeLinhas == 0) return 0;

    /* Tenta primeiro o formato atual; se falhar, tenta migrar o formato da Tarefa 4. */
    total = interpretarProdutos(linhas, quantidadeLinhas, 8, produtos,
                                categorias, totalCategorias);
    if (total >= 0) return total;

    total = interpretarProdutos(linhas, quantidadeLinhas, 7, produtos,
                                categorias, totalCategorias);
    if (total >= 0) {
        printf("Dados de produtos da Tarefa 4 carregados; serao salvos no formato atualizado ao sair.\n");
        return total;
    }

    fprintf(stderr, "Aviso: nao foi possivel interpretar produtos.txt. Nenhum produto foi carregado.\n");
    return 0;
}

int salvarCategorias(const Categoria categorias[], int total) {
    FILE *arquivo = fopen(ARQUIVO_CATEGORIAS, "w");
    int i, erro = 0;
    if (arquivo == NULL) {
        perror("Erro ao salvar categorias");
        return 0;
    }
    for (i = 0; i < total; i++) {
        if (fprintf(arquivo, "%d\n%s\n", categorias[i].codigo,
                    categorias[i].nome) < 0) erro = 1;
    }
    if (fclose(arquivo) != 0) erro = 1;
    if (erro) fprintf(stderr, "Nao foi possivel salvar todas as categorias.\n");
    return !erro;
}

int salvarProdutos(const Produto produtos[], int total) {
    FILE *arquivo = fopen(ARQUIVO_PRODUTOS, "w");
    int i, erro = 0;
    if (arquivo == NULL) {
        perror("Erro ao salvar produtos");
        return 0;
    }
    for (i = 0; i < total; i++) {
        if (fprintf(arquivo, "%d\n%s\n%c\n%.9g\n%d\n%d\n%d\n%d\n",
                    produtos[i].codigo, produtos[i].nome, produtos[i].prateleira,
                    produtos[i].preco, produtos[i].quantidade,
                    produtos[i].quantidade > 0 ? 1 : 0,
                    produtos[i].codigo_categoria, produtos[i].ativo) < 0) erro = 1;
    }
    if (fclose(arquivo) != 0) erro = 1;
    if (erro) fprintf(stderr, "Nao foi possivel salvar todos os produtos.\n");
    return !erro;
}
