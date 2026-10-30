#include <ctype.h>
#include <limits.h>
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
    printf("Disponivel para venda: %d\n", produto.quantidade > 0 ? 1 : 0);
    printf("Categoria: %s\n", posicao != -1 ? categorias[posicao].nome : "Desconhecida");
}

/* Busca qualquer registro, inclusive os removidos, para impedir reutilizacao de codigo. */
static int buscarCodigoExistente(const Produto produtos[], int total, int codigo) {
    int i;
    for (i = 0; i < total; i++) {
        if (produtos[i].codigo == codigo) return i;
    }
    return -1;
}

/* A busca usada pela pesquisa publica ignora produtos removidos. */
int buscarPorCodigo(const Produto produtos[], int total, int codigo) {
    int posicao = buscarCodigoExistente(produtos, total, codigo);
    if (posicao != -1 && produtos[posicao].ativo == 1) return posicao;
    return -1;
}

int buscarPorNome(const Produto produtos[], int total, const char nome[]) {
    int i;
    for (i = 0; i < total; i++) {
        if (produtos[i].ativo == 1 && strcmp(produtos[i].nome, nome) == 0) {
            return i;
        }
    }
    return -1;
}

int cadastrarProduto(Produto produtos[], int total,
                    const Categoria categorias[], int totalCategorias) {
    Produto produto;
    char texto[20];
    int resultado;

    if (total >= MAX_PRODUTOS) {
        printf("\nA lista de produtos atingiu o limite de %d registros.\n", MAX_PRODUTOS);
        return total;
    }
    if (totalCategorias == 0) {
        printf("\nCadastre ao menos uma categoria na opcao 6 antes de cadastrar um produto.\n");
        return total;
    }

    while (1) {
        resultado = lerInteiro("\nDigite o codigo do produto: ", &produto.codigo);
        if (resultado == -1) return total;
        if (resultado != 1 || !validarInteiroPositivo(produto.codigo)) {
            printf("Codigo invalido! Digite um numero positivo.\n");
        } else if (buscarCodigoExistente(produtos, total, produto.codigo) != -1) {
            printf("Esse codigo ja foi utilizado, inclusive por produto removido. Digite outro.\n");
        } else {
            break;
        }
    }

    while (1) {
        resultado = lerTexto("Digite o nome do produto: ", produto.nome, sizeof(produto.nome));
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

    listarCategorias(categorias, totalCategorias);
    while (1) {
        resultado = lerInteiro("Digite o codigo da categoria do produto: ",
                               &produto.codigo_categoria);
        if (resultado == -1) return total;
        if (resultado == 1 &&
            buscarCategoriaPorCodigo(categorias, totalCategorias,
                                     produto.codigo_categoria) != -1) break;
        printf("Categoria inexistente! Informe um codigo da lista acima.\n");
    }

    produto.disponivel = produto.quantidade > 0 ? 1 : 0;
    produto.ativo = 1;
    produtos[total++] = produto;
    printf("\nProduto cadastrado com sucesso!\n");
    exibirProduto(produto, categorias, totalCategorias);
    return total;
}

void listarProdutos(const Produto produtos[], int total,
                   const Categoria categorias[], int totalCategorias) {
    int i, encontrados = 0;
    for (i = 0; i < total; i++) {
        if (produtos[i].ativo == 1) {
            if (!encontrados) printf("\n===== LISTA DE PRODUTOS =====\n");
            exibirProduto(produtos[i], categorias, totalCategorias);
            encontrados++;
        }
    }
    if (!encontrados) {
        printf("\nNenhum produto ativo foi encontrado.\n");
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
        printf("\nNenhum produto ativo foi encontrado.\n");
    }
}

void alterarEstoqueProduto(Produto produtos[], int total) {
    int codigo, posicao, tipo, quantidade, resultado;
    long novaQuantidade;

    resultado = lerInteiro("\nDigite o codigo do produto: ", &codigo);
    if (resultado != 1) {
        if (resultado != -1) printf("Codigo invalido!\n");
        return;
    }

    posicao = buscarPorCodigo(produtos, total, codigo);
    if (posicao == -1) {
        printf("\nProduto ativo nao encontrado. O estoque nao foi alterado.\n");
        return;
    }

    printf("\n1 - Entrada de mercadoria\n");
    printf("2 - Saida de mercadoria\n");
    resultado = lerInteiro("Escolha o tipo de movimentacao: ", &tipo);
    if (resultado != 1 || (tipo != 1 && tipo != 2)) {
        if (resultado != -1) printf("Tipo de movimentacao invalido.\n");
        return;
    }

    resultado = lerInteiro("Digite a quantidade: ", &quantidade);
    if (resultado != 1) {
        if (resultado != -1) printf("Quantidade invalida.\n");
        return;
    }
    if (quantidade <= 0) {
        printf("A quantidade da movimentacao deve ser maior que zero.\n");
        return;
    }

    if (tipo == 1) {
        if (quantidade > INT_MAX - produtos[posicao].quantidade) {
            printf("A entrada excede o limite de quantidade permitido.\n");
            return;
        }
        novaQuantidade = (long)produtos[posicao].quantidade + quantidade;
        produtos[posicao].quantidade = (int)novaQuantidade;
        printf("\nEntrada registrada com sucesso.\n");
    } else {
        if (quantidade > produtos[posicao].quantidade) {
            printf("\nEstoque insuficiente! A operacao foi cancelada.\n");
            return;
        }
        produtos[posicao].quantidade -= quantidade;
        printf("\nSaida registrada com sucesso.\n");
    }

    produtos[posicao].disponivel = produtos[posicao].quantidade > 0 ? 1 : 0;
    printf("Estoque atualizado: %d unidade(s).\n", produtos[posicao].quantidade);
    printf("Disponivel para venda: %d\n", produtos[posicao].disponivel);
}

void removerProduto(Produto produtos[], int total) {
    int codigo, posicao, resultado;
    resultado = lerInteiro("\nDigite o codigo do produto que deseja remover: ", &codigo);
    if (resultado != 1) {
        if (resultado != -1) printf("Codigo invalido!\n");
        return;
    }

    posicao = buscarCodigoExistente(produtos, total, codigo);
    if (posicao == -1) {
        printf("\nProduto nao encontrado.\n");
        return;
    }
    if (produtos[posicao].ativo == 0) {
        printf("\nEsse produto ja esta removido.\n");
        return;
    }

    produtos[posicao].ativo = 0;
    printf("\nProduto removido com sucesso. O registro foi mantido no sistema.\n");
}
