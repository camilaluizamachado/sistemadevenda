#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

typedef struct {
    int idVenda;
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

typedef struct NodeVenda {
    Venda venda;
    struct NodeVenda* esquerda;
    struct NodeVenda* direita;
    int altura;
} NodeVenda;


NodeProduto* raizProduto = NULL;
NodeCliente* raizCliente = NULL;
NodeVenda* raizVenda = NULL;

int proximoIdVenda = 1;

NodeProduto* inserirProduto(NodeProduto* raiz, Produto produto);
void exibirProdutosInOrdem(NodeProduto* raiz);
Produto* buscarProduto(NodeProduto* raiz, int id);
void liberarProdutos(NodeProduto* raiz);

NodeCliente* inserirCliente(NodeCliente* raiz, Cliente cliente);
void exibirClientesInOrdem(NodeCliente* raiz);
Cliente* buscarCliente(NodeCliente* raiz, int id);
NodeCliente* excluirCliente(NodeCliente* raiz, int id);
void liberarClientes(NodeCliente* raiz);

int altura(NodeVenda* N);
int max(int a, int b);
NodeVenda* novaVendaNode(Venda venda);
NodeVenda* rotacaoDireita(NodeVenda* y);
NodeVenda* rotacaoEsquerda(NodeVenda* x);
int fatorBalanceamento(NodeVenda* N);
NodeVenda* inserirVenda(NodeVenda* node, Venda venda);
void liberarVendas(NodeVenda* raiz);

void realizarVenda();
void mostrarVendaMaiorValor();
void mostrarQuantidadeProdutos();

void menu();

int main() {
    menu();
    liberarProdutos(raizProduto);
    liberarClientes(raizCliente);
    liberarVendas(raizVenda);
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
        if (raiz->esquerda == NULL) {
            NodeCliente* temp = raiz->direita;
            free(raiz);
            return temp;
        } else if (raiz->direita == NULL) {
            NodeCliente* temp = raiz->esquerda;
            free(raiz);
            return temp;
        }
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

int altura(NodeVenda* N) {
    if (N == NULL)
        return 0;
    return N->altura;
}

int max(int a, int b) {
    return (a > b) ? a : b;
}

NodeVenda* novaVendaNode(Venda venda) {
    NodeVenda* node = (NodeVenda*)malloc(sizeof(NodeVenda));
    node->venda = venda;
    node->esquerda = NULL;
    node->direita = NULL;
    node->altura = 1;
    return (node);
}

NodeVenda* rotacaoDireita(NodeVenda* y) {
    NodeVenda* x = y->esquerda;
    NodeVenda* T2 = x->direita;

    x->direita = y;
    y->esquerda = T2;

    y->altura = max(altura(y->esquerda), altura(y->direita)) + 1;
    x->altura = max(altura(x->esquerda), altura(x->direita)) + 1;

    return x;
}

NodeVenda* rotacaoEsquerda(NodeVenda* x) {
    NodeVenda* y = x->direita;
    NodeVenda* T2 = y->esquerda;

    y->esquerda = x;
    x->direita = T2;

    x->altura = max(altura(x->esquerda), altura(x->direita)) + 1;
    y->altura = max(altura(y->esquerda), altura(y->direita)) + 1;

    return y;
}

int fatorBalanceamento(NodeVenda* N) {
    if (N == NULL)
        return 0;
    return altura(N->esquerda) - altura(N->direita);
}

NodeVenda* inserirVenda(NodeVenda* node, Venda venda) {
    if (node == NULL)
        return (novaVendaNode(venda));

    if (venda.valorTotal < node->venda.valorTotal)
        node->esquerda = inserirVenda(node->esquerda, venda);
    else
        node->direita = inserirVenda(node->direita, venda);

    node->altura = 1 + max(altura(node->esquerda), altura(node->direita));

    int balance = fatorBalanceamento(node);

    if (balance > 1 && venda.valorTotal < node->esquerda->venda.valorTotal)
        return rotacaoDireita(node);

    if (balance < -1 && venda.valorTotal >= node->direita->venda.valorTotal)
        return rotacaoEsquerda(node);

    if (balance > 1 && venda.valorTotal >= node->esquerda->venda.valorTotal) {
        node->esquerda = rotacaoEsquerda(node->esquerda);
        return rotacaoDireita(node);
    }
    if (balance < -1 && venda.valorTotal < node->direita->venda.valorTotal) {
        node->direita = rotacaoDireita(node->direita);
        return rotacaoEsquerda(node);
    }

    return node;
}


void realizarVenda() {
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
    novaVenda.idVenda = proximoIdVenda++;
    novaVenda.cliente = *cliente;
    novaVenda.produto = *produto;
    novaVenda.quantidadeVendida = quantidade;
    novaVenda.valorTotal = quantidade * produto->valor;

    raizVenda = inserirVenda(raizVenda, novaVenda);
    printf("Venda (ID: %d) realizada com sucesso! Total: R$ %.2f\n", novaVenda.idVenda, novaVenda.valorTotal);
}

void mostrarVendaMaiorValor() {
    if (raizVenda == NULL) {
        printf("Nenhuma venda registrada!\n");
        return;
    }

    NodeVenda* atual = raizVenda;
    while (atual->direita != NULL) {
        atual = atual->direita;
    }

    printf("Venda com o maior valor:\n");
    printf("Venda ID: %d\n", atual->venda.idVenda);
    printf("Cliente: %s (ID: %d)\n", atual->venda.cliente.nome, atual->venda.cliente.id);
    printf("Produto: %s (ID: %d)\n", atual->venda.produto.nome, atual->venda.produto.id);
    printf("Quantidade: %d\n", atual->venda.quantidadeVendida);
    printf("Valor Total: R$ %.2f\n", atual->venda.valorTotal);
}

void liberarVendas(NodeVenda* raiz) {
    if (raiz != NULL) {
        liberarVendas(raiz->esquerda);
        liberarVendas(raiz->direita);
        free(raiz);
    }
}


void mostrarQuantidadeProdutos() {
    int totalProdutos = 0;
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
