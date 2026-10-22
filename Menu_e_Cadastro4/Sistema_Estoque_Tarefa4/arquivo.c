#include <stdio.h>
#include <string.h>
#include "arquivo.h"
#include "util.h"

#define ARQUIVO_CATEGORIAS "categorias.txt"
#define ARQUIVO_PRODUTOS "produtos.txt"

/*
 * Formato texto (um campo por linha), sem separadores especiais:
 * categorias.txt: codigo, nome.
 * produtos.txt: codigo, nome, prateleira, preco, quantidade,
 *               disponivel, codigo_categoria.
 * Essa representacao preserva nomes com espacos e pontuacao.
 */
int carregarCategorias(Categoria categorias[]) {
    FILE *arquivo = fopen(ARQUIVO_CATEGORIAS, "r");
    char linha[64];
    Categoria categoria;
    int total = 0;

    if (arquivo == NULL) {
        /* Na primeira execucao, o arquivo ainda pode nao existir. */
        return 0;
    }

    while (total < MAX_CATEGORIAS) {
        if (lerLinha(arquivo, linha, sizeof(linha)) != 1) break;
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

int carregarProdutos(Produto produtos[], const Categoria categorias[], int totalCategorias) {
    FILE *arquivo = fopen(ARQUIVO_PRODUTOS, "r");
    Produto produto;
    char linha[128];
    int total = 0;
    int codigoValido, precoValido, quantidadeValida, disponivelValido, categoriaValida;

    if (arquivo == NULL) {
        return 0;
    }

    while (total < MAX_PRODUTOS) {
        if (lerLinha(arquivo, linha, sizeof(linha)) != 1) break;
        codigoValido = converterInteiro(linha, &produto.codigo);
        if (lerLinha(arquivo, produto.nome, sizeof(produto.nome)) != 1) break;
        if (lerLinha(arquivo, linha, sizeof(linha)) != 1) break;
        produto.prateleira = strlen(linha) == 1 ? linha[0] : '\0';
        if (lerLinha(arquivo, linha, sizeof(linha)) != 1) break;
        precoValido = converterReal(linha, &produto.preco);
        if (lerLinha(arquivo, linha, sizeof(linha)) != 1) break;
        quantidadeValida = converterInteiro(linha, &produto.quantidade);
        if (lerLinha(arquivo, linha, sizeof(linha)) != 1) break;
        disponivelValido = converterInteiro(linha, &produto.disponivel);
        if (lerLinha(arquivo, linha, sizeof(linha)) != 1) break;
        categoriaValida = converterInteiro(linha, &produto.codigo_categoria);

        if (codigoValido && precoValido && quantidadeValida && disponivelValido &&
            categoriaValida && validarInteiroPositivo(produto.codigo) &&
            validarTextoPreenchido(produto.nome) && validarPrateleira(produto.prateleira) &&
            validarRealPositivo(produto.preco) &&
            validarInteiroNaoNegativo(produto.quantidade) &&
            (produto.disponivel == 0 || produto.disponivel == 1) &&
            buscarPorCodigo(produtos, total, produto.codigo) == -1 &&
            buscarCategoriaPorCodigo(categorias, totalCategorias, produto.codigo_categoria) != -1) {
            /* A disponibilidade depende da quantidade, como na Tarefa 3. */
            produto.disponivel = produto.quantidade > 0 ? 1 : 0;
            produtos[total++] = produto;
        }
    }
    fclose(arquivo);
    return total;
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
        if (fprintf(arquivo, "%d\n%s\n%c\n%.9g\n%d\n%d\n%d\n",
                    produtos[i].codigo, produtos[i].nome, produtos[i].prateleira,
                    produtos[i].preco, produtos[i].quantidade, produtos[i].disponivel,
                    produtos[i].codigo_categoria) < 0) erro = 1;
    }
    if (fclose(arquivo) != 0) erro = 1;
    if (erro) fprintf(stderr, "Nao foi possivel salvar todos os produtos.\n");
    return !erro;
}
