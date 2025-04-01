/**
 * @file main_prog.c
 * @author Bruno (31496), Diogo Pereira (31513), Dinis Silva (31503)
 * @brief
 * @version 1.2
 * @date 2024-12-28
 *
 * @copyright Copyright (c) 2024
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//  Tamanho das arrays
#define MAX_FUNCIONARIOS 100
#define MAX_UTENTES 100
#define MAX_EMENTAS 100
#define MAX_NOME 50
#define MAX_PRATO 100
#define MAX_DIA 10

//  Tabela funcionario
typedef struct
{
    int numero_func;
    char nome[MAX_NOME];
    char nif[9];
    char telefone[9];
} Funcionario;

//  Tabela ementas
typedef struct
{
    char dia[MAX_DIA];
    char data[11];
    char prato_peixe[MAX_PRATO];
    int kcal_peixe;
    char prato_carne[MAX_PRATO];
    int kcal_carne;
    char prato_dieta[MAX_PRATO];
    int kcal_dieta;
    char prato_veg[MAX_PRATO];
    int kcal_veg;
} Ementa;

//  Tabela Escolhas
typedef struct
{
    char dia[MAX_DIA];
    int n_func;
    char tipo_prato;
    // 'C' - carne
    // 'P' - peixe
    // 'D' - dieta
    // 'V' - vegetariano
} Escolhas;

//  Inicializar tabelas
Funcionario funcionarios[MAX_FUNCIONARIOS];
Ementa ementas[MAX_EMENTAS];
Escolhas escolhas[MAX_UTENTES];

//  Variavéis para contagem (são lidas nas funções de leitura)
int num_funcionarios = 0, num_ementas = 0, num_escolhas = 0;

void lerFuncionarios(const char *filename)
{
    FILE *fp = fopen(filename, "r");
    if (!fp)
    {
        printf("Erro: Não foi possível abrir o ficheiro '%s'.\n", filename);
        return;
    }

    while (fscanf(fp, "%d;%[^;];%[^;];%[^\n]\n",
                  &funcionarios[num_funcionarios].numero_func,
                  funcionarios[num_funcionarios].nome,
                  funcionarios[num_funcionarios].nif,
                  funcionarios[num_funcionarios].telefone) != EOF)
    {
        num_funcionarios++;
    }
    fclose(fp);
}

void lerEmentas(const char *filename)
{
    FILE *fp = fopen(filename, "r");
    if (!fp)
    {
        printf("Erro: Não foi possível abrir o ficheiro '%s'.\n", filename);
        return;
    }

    while (fscanf(fp, "%[^;];%[^;];%[^;];%d;%[^;];%d;%[^;];%d;%[^;];%d\n",
                  ementas[num_ementas].dia,
                  ementas[num_ementas].data,
                  ementas[num_ementas].prato_peixe,
                  &ementas[num_ementas].kcal_peixe,
                  ementas[num_ementas].prato_carne,
                  &ementas[num_ementas].kcal_carne,
                  ementas[num_ementas].prato_dieta,
                  &ementas[num_ementas].kcal_dieta,
                  ementas[num_ementas].prato_veg,
                  &ementas[num_ementas].kcal_veg) != EOF)
    {
        num_ementas++;
    }
    fclose(fp);
}

void lerEscolhas(const char *filename)
{
    FILE *fp = fopen(filename, "r");
    if (!fp)
    {
        printf("Erro: Não foi possível abrir o ficheiro '%s'.\n", filename);
        return;
    }

    while (fscanf(fp, "%[^;];%d;%c\n",
                  escolhas[num_escolhas].dia,
                  &escolhas[num_escolhas].n_func,
                  &escolhas[num_escolhas].tipo_prato) != EOF)
    {
        num_escolhas++;
    }
    fclose(fp);
}

void apresentarDia(char *dia)
{
    printf("\nRefeições para o dia %s:\n", dia);
    printf("--------------------------------------------------\n");
    printf("| Nº Func. |         Nome         | Tipo Prato |\n");
    printf("--------------------------------------------------\n");

    for (int i = 0; i < num_escolhas; i++)
    {
        if (strcmp(escolhas[i].dia, dia) == 0)
        {
            for (int j = 0; j < num_funcionarios; j++)
            {
                if (funcionarios[j].numero_func == escolhas[i].n_func)
                {
                    printf("| %-8d | %-20s | %-9c |\n",
                           funcionarios[j].numero_func,
                           funcionarios[j].nome,
                           escolhas[i].tipo_prato);
                }
            }
        }
    }
    printf("--------------------------------------------------\n");
}

void ordenarFuncionarios()
{
    for (int i = 0; i < num_funcionarios - 1; i++)
    {
        for (int j = 0; j < num_funcionarios - i - 1; j++)
        {
            if (funcionarios[j].numero_func < funcionarios[j + 1].numero_func) // Decrescente
            {
                // Trocar posições
                Funcionario temp = funcionarios[j];
                funcionarios[j] = funcionarios[j + 1];
                funcionarios[j + 1] = temp;
            }
        }
    }
}

/**
 * @brief Mostrar Refeição do funcionário Número (n_func)
 *
 * @param n_func
 */

void mostrarRefeicoesFuncionario(int n_func)
{
    printf("\nRefeições e calorias do funcionário %d:\n", n_func);

    for (int i = 0; i < num_escolhas; i++)
    {
        if (escolhas[i].n_func == n_func)
        {
            for (int j = 0; j < num_ementas; j++)
            {
                if (strcmp(escolhas[i].dia, ementas[j].dia) == 0)
                {
                    int kcal = 0;
                    if (escolhas[i].tipo_prato == 'P')
                        kcal = ementas[j].kcal_peixe;
                    if (escolhas[i].tipo_prato == 'C')
                        kcal = ementas[j].kcal_carne;
                    if (escolhas[i].tipo_prato == 'D')
                        kcal = ementas[j].kcal_dieta;
                    if (escolhas[i].tipo_prato == 'V')
                        kcal = ementas[j].kcal_veg;

                    printf("%s - %c - %d kcal\n",
                           escolhas[i].dia, escolhas[i].tipo_prato, kcal);
                }
            }
        }
    }
}

void mediaKcalDia()
{
    printf("\nMédia de calorias consumidas por dia:\n");
    for (int i = 0; i < num_ementas; i++)
    {
        int total_kcal = 0, count = 0;
        for (int j = 0; j < num_escolhas; j++)
        {
            if (strcmp(escolhas[j].dia, ementas[i].dia) == 0)
            {
                total_kcal += ementas[i].kcal_peixe + ementas[i].kcal_carne + ementas[i].kcal_dieta + ementas[i].kcal_veg;
                count += 4; // 4 pratos
            }
        }
        if (count > 0)
            printf("%s: %.2f kcal\n", ementas[i].dia, total_kcal / (float)count);
    }
}

void start()
{
    printf("\n========== MENU INICIAL ==========\n");
    printf("1. Mostrar dados de funcionários\n");
    printf("2. Mostrar dados de ementas\n");
    printf("3. Mostrar dados das escolhas\n");
    printf("4. Apresentar refeições para um dia específico\n");
    printf("5. Ordenar funcionários\n");
    printf("6. Listar refeições e calorias de um funcionário\n");
    printf("7. Calcular média de calorias por dia\n");
    printf("8. Ajuda\n");
    printf("0. Sair\n");
    printf("===================================\n");
    printf("Escolha uma opção: ");
}

void ajuda() {
    printf("\n========== AJUDA ==========\n");
    printf("Este programa gerencia funcionários, ementas e escolhas de refeições através de ficheiros\n\n");
    printf("1. Mostrar dados de funcionários:\n");
    printf("   Exibe a lista completa dos funcionários, incluindo número, nome, NIF e telefone.\n\n");
    printf("2. Mostrar dados de ementas:\n");
    printf("   Exibe o cardápio semanal com pratos disponíveis (peixe, carne, dieta e vegetariano) e suas calorias.\n\n");
    printf("3. Mostrar dados das escolhas:\n");
    printf("   Exibe as escolhas feitas pelos funcionários, incluindo o dia, número do funcionário e tipo de prato escolhido.\n\n");
    printf("4. Apresentar refeições para um dia específico:\n");
    printf("   Permite visualizar as refeições escolhidas por cada funcionário em um dia específico.\n\n");
    printf("5. Ordenar funcionários:\n");
    printf("   Ordena os funcionários em ordem decrescente por número e atualiza o arquivo de funcionários.\n\n");
    printf("6. Listar refeições e calorias de um funcionário:\n");
    printf("   Mostra as refeições e calorias consumidas por um funcionário específico.\n\n");
    printf("7. Calcular média de calorias por dia:\n");
    printf("   Calcula e exibe a média de calorias consumidas para cada dia baseado nas escolhas feitas.\n\n");
    printf("8. Ajuda\n");
    printf("   Mostra o menu de Ajuda.\n\n");
    printf("0. Sair:\n");
    printf("   Encerra o programa.\n");
    printf("===========================\n");
}


void mostrarFuncionarios()
{
    printf("\nTabela de funcionários\n");
    for (int i = 0; i < num_funcionarios; i++)
    {
        printf("Funcionário %d: %s, NIF: %s, Telefone: %s\n",
               funcionarios[i].numero_func,
               funcionarios[i].nome,
               funcionarios[i].nif,
               funcionarios[i].telefone);
    }
}

void mostrarEmentas()
{
    printf("\nTabela da Ementa Semanal:\n");
    printf("--------------------------------------------------------------------------------------------------------------------------------------------------------------------------\n");
    printf("| Dia       | Data       | Prato Peixe                | Kcal | Prato Carne                | Kcal | Prato Dieta                | Kcal | Prato Vegetariano          | Kcal |\n");
    printf("--------------------------------------------------------------------------------------------------------------------------------------------------------------------------\n");

    for (int i = 0; i < num_ementas; i++)
    {
        printf("| %-9s | %-10s | %-25s | %-4d | %-25s | %-4d | %-25s | %-4d | %-25s | %-4d |\n",
               ementas[i].dia,
               ementas[i].data,
               ementas[i].prato_peixe, ementas[i].kcal_peixe,
               ementas[i].prato_carne, ementas[i].kcal_carne,
               ementas[i].prato_dieta, ementas[i].kcal_dieta,
               ementas[i].prato_veg, ementas[i].kcal_veg);
    }
    printf("--------------------------------------------------------------------------------------------------------------------------------------------------------------------------\n");
}

void mostrarEscolhas()
{
    printf("\nTabela de escolhas dos funcionários:\n");
    printf("--------------------------------------\n");
    printf("| Dia       | Nº Func. | Tipo Prato |\n");
    printf("--------------------------------------\n");

    for (int i = 0; i < num_escolhas; i++)
    {
        printf("| %-9s | %-8d | %-9c |\n",
               escolhas[i].dia,
               escolhas[i].n_func,
               escolhas[i].tipo_prato);
    }

    printf("--------------------------------------\n");
}

void atualizarFuncionarios()
{
    char confirm;

    printf("Deseja salvar os dados no ficheiro 'funcionarios.txt'? (s/n): ");
    scanf(" %c", &confirm);

    if (confirm == 's' || confirm == 'S')
    {
        FILE *fp = fopen("funcionarios.txt", "w");
        if (!fp)
        {
            printf("Erro: Não foi possível abrir o ficheiro 'funcionarios.txt' para escrita.\n");
            return;
        }

        for (int i = 0; i < num_funcionarios; i++)
        {
            fprintf(fp, "%d;%s;%s;%s\n",
                    funcionarios[i].numero_func,
                    funcionarios[i].nome,
                    funcionarios[i].nif,
                    funcionarios[i].telefone);
        }
        fclose(fp);
        printf("Funcionários atualizados com sucesso no ficheiro 'funcionarios.txt'.\n");
    }
    else
    {
        printf("Atualização do ficheiro cancelada.\n");
    }
}

int main(int argc, char *argv[])
{
    int opcao;
    char dia[MAX_DIA];
    int numero_funcionario;

    //  Leitura dos dados
    /**
     * @brief Leitura dos dados
     */

    if (argc < 4) // caso o utilizador use mais ficheiros
    {
        //  stderr erro padrão para controlo de execução
        fprintf(stderr, "Uso: %s <ficheiro_funcionarios> <ficheiro_ementas> <ficheiro_escolhas>\n", argv[0]);
        return 1;
    }

    lerFuncionarios(argv[1]);
    lerEmentas(argv[2]);
    lerEscolhas(argv[3]);

    do
    {
        start();
        scanf("%d", &opcao);

        switch (opcao)
        {

            //  Mostrar funcionários
            /**
             * @brief Mostrar funcionários
             */

        case 1:
            mostrarFuncionarios();
            break;

            // Mostrar ementas
            /**
             * @brief Mostrar Ementas
             */

        case 2:
            mostrarEmentas();
            break;

            //  Mostrar escolhas
            /**
             * @brief Mostrar Escolhas
             */

        case 3:
            mostrarEscolhas();
            break;

            //  Apresenta dia
            /**
             * @brief Apresenta Dia
             */

        case 4:
            printf("Insira o dia da semana (ex: 2feira): ");
            scanf("%s", dia);
            apresentarDia(dia);
            break;

            //  Ordena funcionários
            /**
             * @brief Ordena funcionários
             */

        case 5:
            ordenarFuncionarios();
            printf("A tabela dos funcionários foi ordenada com sucesso!");
            mostrarFuncionarios();
            atualizarFuncionarios();
            break;

            //  Consulta escolha funcionário
            /**
             * @brief Consulta escolha funcionário
             */

        case 6:
            printf("Insira o número do funcionário: ");
            scanf("%d", &numero_funcionario);
            mostrarRefeicoesFuncionario(numero_funcionario);
            break;

            //  Calcula média de kcal/dia
            /**
             * @brief Calcula média de kcal/dia
             */

        case 7:
            mediaKcalDia();
            break;
        case 8:
            ajuda();
            break;
        case 0:
            printf("A fechar o programa..\n");
            break;
        default:
            printf("Opção inválida. Por favor, tente novamente.\n");
        }
    } while (opcao != 0);

    return 0;
}
