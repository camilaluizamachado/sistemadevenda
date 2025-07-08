#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_VENDAS 100

typedef struct {
    int id;
    char nome[50];
} Cliente;

typedef struct {
    int id;
    char nome[50];
    int quantidade;
    float valor;
} Produto;

typedef struct Venda {
    Cliente cliente;
    Produto produto;
    int quantidadeVendida;
    float valorTotal;
} Venda;

typedef struct NodeProduto {
    Produto produto;
    struct NodeProduto* esquerda;
    struct NodeProduto* direita;
} NodeProduto;

typedef struct NodeCliente {
    Cliente cliente;
    struct NodeCliente* esquerda;
    struct NodeCliente* direita;
} NodeCliente;

NodeProduto* raizProduto = NULL;
NodeCliente* raizCliente = NULL;
Venda vendas[MAX_VENDAS];
int totalVendas = 0;

// Funções para produtos
NodeProduto* inserirProduto(NodeProduto* raiz, Produto produto);
void exibirProdutosInOrdem(NodeProduto* raiz);
Produto* buscarProduto(NodeProduto* raiz, int id);
void liberarProdutos(NodeProduto* raiz);

// Funções para clientes
NodeCliente* inserirCliente(NodeCliente* raiz, Cliente cliente);
void exibirClientesInOrdem(NodeCliente* raiz);
Cliente* buscarCliente(NodeCliente* raiz, int id);
NodeCliente* excluirCliente(NodeCliente* raiz, int id);
void liberarClientes(NodeCliente* raiz);

// Funções de vendas
void realizarVenda();
void mostrarVendaMaiorValor();
void mostrarQuantidadeProdutos();

void menu();

int main() {
    menu();
    liberarProdutos(raizProduto);
    liberarClientes(raizCliente);
    return 0;
}

void menu() {
    int opcao;
    do {
        printf("\nMENU DE GERENCIAMENTO DE ESTOQUE\n");
        printf("1. Inserir Produto\n");
        printf("2. Exibir Produtos em In-Ordem\n");
        printf("3. Exibir Clientes em In-Ordem\n");
        printf("4. Cadastrar Cliente\n");
        printf("5. Excluir um Cliente\n");
        printf("6. Realizar Vendas\n");
        printf("7. Mostrar a Venda com o Maior Valor\n");
        printf("8. Mostrar a Quantidade de Produtos\n");
        printf("0. Sair\n");
        printf("Escolha: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1: {
                Produto novoProduto;
                printf("Digite o ID do produto: ");
                scanf("%d", &novoProduto.id);
                if (buscarProduto(raizProduto, novoProduto.id) != NULL) {
                    printf("Já existe um produto com esse ID!\n");
                    break;
                }
                printf("Digite o nome do produto: ");
                scanf(" %[^\n]", novoProduto.nome);
                printf("Digite a quantidade em estoque: ");
                scanf("%d", &novoProduto.quantidade);
                if (novoProduto.quantidade < 0) {
                    printf("A quantidade não pode ser negativa!\n");
                    break;
                }
                printf("Digite o valor unitário: ");
                scanf("%f", &novoProduto.valor);
                if (novoProduto.valor < 0) {
                    printf("O valor não pode ser negativo!\n");
                    break;
                }
                raizProduto = inserirProduto(raizProduto, novoProduto);
                break;
            }
            case 2:
                exibirProdutosInOrdem(raizProduto);
                break;
            case 3:
                exibirClientesInOrdem(raizCliente);
                break;
            case 4: {
                Cliente novoCliente;
                printf("Digite o ID do cliente: ");
                scanf("%d", &novoCliente.id);
                if (buscarCliente(raizCliente, novoCliente.id) != NULL) {
                    printf("Já existe um cliente com esse ID!\n");
                    break;
                }
                printf("Digite o nome do cliente: ");
                scanf(" %[^\n]", novoCliente.nome);
                raizCliente = inserirCliente(raizCliente, novoCliente);
                break;
            }
            case 5: {
                int id;
                printf("Digite o ID do cliente a ser excluído: ");
                scanf("%d", &id);
                raizCliente = excluirCliente(raizCliente, id);
                break;
            }
            case 6:
                realizarVenda();
                break;
            case 7:
                mostrarVendaMaiorValor();
                break;
            case 8:
                mostrarQuantidadeProdutos();
                break;
            case 0:
                printf("Saindo...\n");
                break;
            default:
                printf("Opção inválida!\n");
        }
    } while (opcao != 0);
}

// Funções para produtos
NodeProduto* inserirProduto(NodeProduto* raiz, Produto produto) {
    if (raiz == NULL) {
        NodeProduto* novo = (NodeProduto*)malloc(sizeof(NodeProduto));
        novo->produto = produto;
        novo->esquerda = NULL;
        novo->direita = NULL;
        return novo;
    }
    if (produto.id < raiz->produto.id) {
        raiz->esquerda = inserirProduto(raiz->esquerda, produto);
    } else {
        raiz->direita = inserirProduto(raiz->direita, produto);
    }
    return raiz;
}

void exibirProdutosInOrdem(NodeProduto* raiz) {
    if (raiz != NULL) {
        exibirProdutosInOrdem(raiz->esquerda);
        printf("Produto ID: %d, Nome: %s, Quantidade: %d, Valor: R$ %.2f\n",
               raiz->produto.id, raiz->produto.nome, raiz->produto.quantidade, raiz->produto.valor);
        exibirProdutosInOrdem(raiz->direita);
    }
}

Produto* buscarProduto(NodeProduto* raiz, int id) {
    if (raiz == NULL || raiz->produto.id == id) {
        return (raiz != NULL) ? &raiz->produto : NULL;
    }
    if (id < raiz->produto.id) {
        return buscarProduto(raiz->esquerda, id);
    }
    return buscarProduto(raiz->direita, id);
}

void liberarProdutos(NodeProduto* raiz) {
    if (raiz != NULL) {
        liberarProdutos(raiz->esquerda);
        liberarProdutos(raiz->direita);
        free(raiz);
    }
}

// Funções para clientes
NodeCliente* inserirCliente(NodeCliente* raiz, Cliente cliente) {
    if (raiz == NULL) {
        NodeCliente* novo = (NodeCliente*)malloc(sizeof(NodeCliente));
        novo->cliente = cliente;
        novo->esquerda = NULL;
        novo->direita = NULL;
        return novo;
    }
    if (cliente.id < raiz->cliente.id) {
        raiz->esquerda = inserirCliente(raiz->esquerda, cliente);
    } else {
        raiz->direita = inserirCliente(raiz->direita, cliente);
    }
    return raiz;
}

void exibirClientesInOrdem(NodeCliente* raiz) {
    if (raiz != NULL) {
        exibirClientesInOrdem(raiz->esquerda);
        printf("Cliente ID: %d, Nome: %s\n", raiz->cliente.id, raiz->cliente.nome);
        exibirClientesInOrdem(raiz->direita);
    }
}

Cliente* buscarCliente(NodeCliente* raiz, int id) {
    if (raiz == NULL || raiz->cliente.id == id) {
        return (raiz != NULL) ? &raiz->cliente : NULL;
    }
    if (id < raiz->cliente.id) {
        return buscarCliente(raiz->esquerda, id);
    }
    return buscarCliente(raiz->direita, id);
}

NodeCliente* excluirCliente(NodeCliente* raiz, int id) {
    if (raiz == NULL) {
        return NULL;
    }
    if (id < raiz->cliente.id) {
        raiz->esquerda = excluirCliente(raiz->esquerda, id);
    } else if (id > raiz->cliente.id) {
        raiz->direita = excluirCliente(raiz->direita, id);
    } else {
        // Cliente encontrado
        if (raiz->esquerda == NULL) {
            NodeCliente* temp = raiz->direita;
            free(raiz);
            return temp;
        } else if (raiz->direita == NULL) {
            NodeCliente* temp = raiz->esquerda;
            free(raiz);
            return temp;
        }
        // Cliente com dois filhos
        NodeCliente* temp = raiz->direita;
        while (temp && temp->esquerda != NULL) {
            temp = temp->esquerda;
        }
        raiz->cliente = temp->cliente;
        raiz->direita = excluirCliente(raiz->direita, temp->cliente.id);
    }
    return raiz;
}

void liberarClientes(NodeCliente* raiz) {
    if (raiz != NULL) {
        liberarClientes(raiz->esquerda);
        liberarClientes(raiz->direita);
        free(raiz);
    }
}

// Funções de vendas
void realizarVenda() {
    if (totalVendas >= MAX_VENDAS) {
        printf("Limite de vendas atingido!\n");
        return;
    }

    int idCliente, idProduto, quantidade;
    printf("Digite o ID do cliente: ");
    scanf("%d", &idCliente);
    Cliente* cliente = buscarCliente(raizCliente, idCliente);
    if (cliente == NULL) {
        printf("Cliente não encontrado ou excluído!\n");
        return;
    }

    printf("Digite o ID do produto: ");
    scanf("%d", &idProduto);
    Produto* produto = buscarProduto(raizProduto, idProduto);
    if (produto == NULL) {
        printf("Produto não encontrado!\n");
        return;
    }

    printf("Digite a quantidade a ser vendida: ");
    scanf("%d", &quantidade);
    if (quantidade <= 0 || quantidade > produto->quantidade) {
        printf("Quantidade inválida ou estoque insuficiente!\n");
        return;
    }

    produto->quantidade -= quantidade;

    Venda novaVenda;
    novaVenda.cliente = *cliente; // Cópia do cliente no momento da venda
    novaVenda.produto = *produto;   // Cópia do produto no momento da venda
    novaVenda.quantidadeVendida = quantidade;
    novaVenda.valorTotal = quantidade * produto->valor;

    vendas[totalVendas++] = novaVenda;
    printf("Venda realizada com sucesso! Total: R$ %.2f\n", novaVenda.valorTotal);
}

void mostrarVendaMaiorValor() {
    if (totalVendas == 0) {
        printf("Nenhuma venda registrada!\n");
        return;
    }

    float maiorValor = 0;
    Venda* vendaMaior = NULL;

    for (int i = 0; i < totalVendas; i++) {
        if (vendas[i].valorTotal > maiorValor) {
            maiorValor = vendas[i].valorTotal;
            vendaMaior = &vendas[i];
        }
    }

    if (vendaMaior != NULL) {
        printf("Venda com o maior valor:\n");
        printf("Cliente: %s (ID: %d)\n", vendaMaior->cliente.nome, vendaMaior->cliente.id);
        printf("Produto: %s (ID: %d)\n", vendaMaior->produto.nome, vendaMaior->produto.id);
        printf("Quantidade: %d\n", vendaMaior->quantidadeVendida);
        printf("Valor Total: R$ %.2f\n", vendaMaior->valorTotal);
    }
}

void mostrarQuantidadeProdutos() {
    int totalProdutos = 0;
    // Função recursiva para contar produtos
    void contarProdutos(NodeProduto* raiz) {
        if (raiz != NULL) {
            totalProdutos += raiz->produto.quantidade;
            contarProdutos(raiz->esquerda);
            contarProdutos(raiz->direita);
        }
    }
    contarProdutos(raizProduto);
    printf("Quantidade total de produtos em estoque: %d\n", totalProdutos);
}
