#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ARQUIVO_DADOS "clientes.dat"

typedef struct {
    int numero_conta;
    char nome[50];
    double saldo;
    int ativo;
} Cliente;

void cadastrarCliente(FILE *arq) {
    int posicao;
    Cliente c;

    printf("Digite a posição onde deseja cadastrar (0, 1, 2...): ");
    scanf("%d", &posicao);
    getchar();

    printf("Número da Conta: ");
    scanf("%d", &c.numero_conta);
    getchar();
    
    printf("Nome do Cliente: ");
    fgets(c.nome, 50, stdin);
    c.nome[strcspn(c.nome, "\n")] = 0;

    printf("Saldo Inicial: ");
    scanf("%lf", &c.saldo);
    
    c.ativo = 1; 

    fseek(arq, posicao * sizeof(Cliente), SEEK_SET);
    
    fwrite(&c, sizeof(Cliente), 1, arq);
    printf("Cliente cadastrado com sucesso na posição %d!\n", posicao);
}

void consultarCliente(FILE *arq) {
    int contaProcurada;
    Cliente c;
    int encontrado = 0;

    printf("Digite o número da conta para consulta: ");
    scanf("%d", &contaProcurada);

    rewind(arq);

    while (fread(&c, sizeof(Cliente), 1, arq) == 1) {
        if (c.ativo && c.numero_conta == contaProcurada) {
            printf("\n--- Cliente Encontrado ---\n");
            printf("Conta: %d\n", c.numero_conta);
            printf("Nome: %s\n", c.nome);
            printf("Saldo: R$ %.2f\n", c.saldo);
            encontrado = 1;
            break;
        }
    }

    if (!encontrado) {
        printf("Conta número %d não foi encontrada ou está inativa.\n", contaProcurada);
    }
}

void atualizarSaldo(FILE *arq) {
    int contaProcurada;
    Cliente c;
    int encontrado = 0;

    printf("Digite o número da conta para atualizar o saldo: ");
    scanf("%d", &contaProcurada);

    rewind(arq);
    while (fread(&c, sizeof(Cliente), 1, arq) == 1) {
        if (c.ativo && c.numero_conta == contaProcurada) {
            printf("Saldo atual: R$ %.2f\n", c.saldo);
            printf("Digite o novo saldo: ");
            scanf("%lf", &c.saldo);

            fseek(arq, -sizeof(Cliente), SEEK_CUR);
            fwrite(&c, sizeof(Cliente), 1, arq);
            
            printf("Saldo atualizado com sucesso!\n");
            encontrado = 1;
            break;
        }
    }
    if (!encontrado) printf("Conta não encontrada.\n");
}

void encerrarConta(FILE *arq) {
    int contaProcurada;
    Cliente c;
    int encontrado = 0;

    printf("Digite o número da conta que deseja encerrar: ");
    scanf("%d", &contaProcurada);

    rewind(arq);
    while (fread(&c, sizeof(Cliente), 1, arq) == 1) {
        if (c.ativo && c.numero_conta == contaProcurada) {
            c.ativo = 0;

            fseek(arq, -sizeof(Cliente), SEEK_CUR);
            fwrite(&c, sizeof(Cliente), 1, arq);

            printf("Conta %d encerrada com sucesso (remoção lógica).\n", contaProcurada);
            encontrado = 1;
            break;
        }
    }
    if (!encontrado) printf("Conta não encontrada.\n");
}

void listarClientes(FILE *arq) {
    Cliente c;
    int posicao = 0;
    int temClientes = 0;

    printf("\n--- Lista de Clientes Ativos ---\n");
    while (fread(&c, sizeof(Cliente), 1, arq) == 1) {
        if (c.ativo) {
            printf("Posição: %d | Conta: %d | Nome: %-20s | Saldo: R$ %.2f\n", posicao, c.numero_conta, c.nome, c.saldo);
            temClientes = 1;
        }
        posicao++;
    }

    if (!temClientes) {
        printf("Nenhum cliente ativo encontrado ou fim do arquivo atingido. Use a opção 6 para resetar a leitura.\n");
    }
}

void exibirMenu() {
    printf("\n=== SISTEMA DE MANUTENÇÃO DE CONTAS ===\n");
    printf("1. Cadastrar cliente em posição específica\n");
    printf("2. Consultar cliente pelo número da conta\n");
    printf("3. Atualizar saldo de um cliente\n");
    printf("4. Encerrar conta (remover cliente)\n");
    printf("5. Listar todos os clientes\n");
    printf("6. Reiniciar leitura do arquivo (rewind)\n");
    printf("7. Encerrar\n");
    printf("Escolha uma opção: ");
}

int main() {
    int opcao;
    
    FILE *arq = fopen(ARQUIVO_DADOS, "rb+");
    if (arq == NULL) {
        arq = fopen(ARQUIVO_DADOS, "wb+");
        if (arq == NULL) {
            printf("Erro ao abrir ou criar o arquivo de dados.\n");
            return 1;
        }
    }

    do {
        exibirMenu();
        if (scanf("%d", &opcao) != 1) {
            printf("\nEntrada inválida. Por favor, digite um número.\n");
            getchar();
            continue;
        }
        getchar(); 

        switch(opcao) {
            case 1: cadastrarCliente(arq); break;
            case 2: consultarCliente(arq); break;
            case 3: atualizarSaldo(arq); break;
            case 4: encerrarConta(arq); break;
            case 5: listarClientes(arq); break;
            case 6: 
                rewind(arq); 
                printf("\nPonteiro de leitura reiniciado (rewind) com sucesso!\n");
                break;
            case 7: printf("\nSaindo...\n"); break;
            default: printf("\nOpção inválida!\n");
        }
    } while(opcao != 7);

    fclose(arq);
    return 0;
}