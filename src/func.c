/**
 * @file func.c
 * @author Bruno (a31496@alunos.ipca.pt)
 * @brief
 * @version 1.2
 * @date 2025-03-25
 * @copyright Copyright (c) 2025
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/antena.h"
#include "../include/func.h"

//  variaveis globais para saber as dimenções máximas
int max_x = 0;
int max_y = 0;

#pragma region Ficheiros

/**
 * @brief carrega as antenas através de um ficheiro
 * @param nomeFicheiro nome do ficheiro txt
 * @return lista de antenas
 */

Antena *carregarFicheiro(const char *nomeFicheiro)
{
    FILE *file = fopen(nomeFicheiro, "r");

    if (!file)
    {
        printf("Erro ao abrir o ficheiro!\n");
        return NULL;
    }

    Antena *lista = NULL;

    char linha[100];

    // +1 porque coordenadas > 0
    int y = 1;

    while (fgets(linha, sizeof(linha), file) != NULL)
    {
        linha[strcspn(linha, "\n")] = '\0'; // retira o caractere de mudança de linha da string
        int tam_linha = strlen(linha);      //  conta as colunas da primeira linha

        //  impede de atualizar várias vezes (só atualiza na primeira)
        if (max_y == 0)
        {
            max_x = tam_linha;
        }

        // conta as linhas
        max_y++;

        for (int x = 0; linha[x] != '\0'; x++)
        {
            if (linha[x] != '.')
            {
                lista = inserirAntena(lista, criarAntena(x + 1, y, linha[x])); // +1 porque coordenadas > 0
            }
        }
        y++;
    }

    fclose(file);
    return lista;
}

#pragma endregion

#pragma region Antenas

/**
 * @brief Cria antena através dos parametros
 * @param x coordenada x
 * @param y coordenada y
 * @param freq  frequência
 * @return
 */

Antena *criarAntena(int x, int y, char freq)
{
    Antena *temp;

    temp = (Antena *)malloc(sizeof(Antena));

    if (temp != NULL)
    {
        temp->freq = freq;
        temp->x = x;
        temp->y = y;
        temp->next = NULL;
        temp->efeitos = NULL;
    }
    return temp;
}

/**
 * @brief Insere uma antena na lista em ordem crescente de coordenadas (x, y)
 * @param lista lista de antenas
 * @param nova nova antena a ser inserida
 * @return lista atualizada
 */

Antena *inserirAntena(Antena *lista, Antena *nova)
{
    // lista null ou antena maior
    if (lista == NULL || (nova->x < lista->x) ||
        (nova->x == lista->x && nova->y < lista->y))
    {
        nova->next = lista;
        return nova;
    }

    Antena *atual = lista;

    // compara as antenas
    while (atual->next != NULL &&
           ((atual->next->x < nova->x) ||
            (atual->next->x == nova->x && atual->next->y < nova->y)))
    {
        atual = atual->next;
    }

    nova->next = atual->next;
    atual->next = nova;

    return lista;
}

/**
 * @brief remove uma antena através das suas coordenadas
 * @param lista
 * @return
 */

Antena *removerAntena(Antena *lista, int x, int y)
{
    if (lista == NULL)
    {
        return NULL;
    }

    Antena *temp = lista;
    Antena *tempAnterior = temp;

    if (lista->x == x && lista->y == y)
    {
        //  valor encontra-se no inicio da lista
        lista = lista->next;
        free(temp);
        return lista;
    }

    //  loop até encontrar a exata antena
    while (temp && !(temp->x == x && temp->y == y))
    {
        tempAnterior = temp;
        temp = temp->next;
    }
    //  verifica se existe
    if (temp != NULL)
    {
        tempAnterior->next = temp->next;
        free(temp);
    }

    return lista;
}

/**
 * @brief função para verificar a existencia de uma antena em determinadas coordenadas
 * @param lista
 * @param x coordenada x
 * @param y coordenada y
 * @return 0 não existe, 1 existe
 */

int existeAntena(Antena *lista, int x, int y)
{
    while (lista)
    {
        if (lista->x == x && lista->y == y)
        {
            return 1;
        }
        lista = lista->next;
    }
    return 0;
}

#pragma endregion

#pragma region Nefasto

/**
 * @brief cria um novo efeito nefasto
 * @param x coordenada x
 * @param y coordenada y
 * @return novo efeito
 */

EfeitoNefasto *criarEfeito(int x, int y)
{
    EfeitoNefasto *novo = (EfeitoNefasto *)malloc(sizeof(EfeitoNefasto));
    if (novo != NULL)
    {
        novo->x = x;
        novo->y = y;
        novo->next = NULL;
    }
    return novo;
}

/**
 * @brief insere um efeito nefasto na lista de uma antena
 * @param lista lista de efeitos
 * @param x coordenada x
 * @param y coordenada y
 * @return lista atualizada
 */

EfeitoNefasto *inserirEfeito(EfeitoNefasto *lista, int x, int y)
{
    EfeitoNefasto *novo = criarEfeito(x, y);
    if (novo == NULL)
        return lista;

    novo->next = lista;
    return novo;
}

/**
 * @brief procura na lista de antenas se tem efeitos nefastos e adiciona-os à lista de efeitos
 * @param lista
 * @return
 */

Antena *adicionarEfeitoNefasto(Antena *lista)
{
    Antena *atual = lista;

    while (atual)
    {
        Antena *proximo = atual->next;

        while (proximo)
        {
            //  percorre a lista até encontrar duas antenas com a mesma frequencia
            if (atual->freq == proximo->freq)
            {
                //  distancia de coordenadas entre a proxima antena e atual
                int diferencaX = proximo->x - atual->x;
                int diferencaY = proximo->y - atual->y;

                //  coordenadas do primeiro efeito nefasto
                int efeitoX1 = atual->x - diferencaX;
                int efeitoY1 = atual->y - diferencaY;

                // coordenadas do segundo efeito nefasto
                int efeitoX2 = proximo->x + diferencaX;
                int efeitoY2 = proximo->y + diferencaY;

                //  verifica se as coordenadas são possiveis e se existe antena nas coordenadas
                if (efeitoX1 >= 1 && efeitoX1 <= max_x &&
                    efeitoY1 >= 1 && efeitoY1 <= max_y &&
                    !existeAntena(lista, efeitoX1, efeitoY1))
                {
                    //  adiciona efeito à lista da antena atual
                    atual->efeitos = inserirEfeito(atual->efeitos, efeitoX1, efeitoY1);
                }
                //  verifica se as coordenadas são possiveis e se existe antena nas coordenadas
                if (efeitoX2 >= 1 && efeitoX2 <= max_x &&
                    efeitoY2 >= 1 && efeitoY2 <= max_y &&
                    !existeAntena(lista, efeitoX2, efeitoY2))
                {
                    //  adiciona efeito à lista da antena proxima
                    proximo->efeitos = inserirEfeito(proximo->efeitos, efeitoX2, efeitoY2);
                }
            }
            proximo = proximo->next;
        }
        atual = atual->next;
    }
    return lista;
}

#pragma endregion

#pragma region Binário

/**
 * @brief função para escrever um bloco em um ficheiro bin
 * @param tag tag de 3 caracteres para identificar os dados exmpl "ANT" - antenas
 * @param f ficheiro binário aberto
 * @return 1 = sucesso
 */

int escreverBloco(char *tag, FILE *f)
{
    fwrite(tag, sizeof(char), 3, f);
    return 1;
}
/**
 * @brief Função para escrever antenas e efeitos nefastos em ficheiro binário
 * @param lista Lista de antenas
 * @param nome_ficheiro Nome do ficheiro binário de saída
 * @return 1 em caso de sucesso, 0 em caso de erro
 */

int escreverBin(Antena *lista, const char *nome_ficheiro)
{
    FILE *f = fopen(nome_ficheiro, "wb");
    if (!f)
    {
        perror("Erro ao criar ficheiro binário");
        return 0;
    }

    escreverBloco("ANT", f);

    // contar número total de antenas
    int total_antenas = 0;
    Antena *temp = lista;
    while (temp != NULL)
    {
        total_antenas++;
        temp = temp->next;
    }
    fwrite(&total_antenas, sizeof(int), 1, f);

    // escrever cada antena
    temp = lista;
    while (temp != NULL)
    {
        fwrite(&temp->x, sizeof(int), 1, f);
        fwrite(&temp->y, sizeof(int), 1, f);
        fwrite(&temp->freq, sizeof(char), 1, f);

        // contar efeitos
        int total_efeitos = 0;
        EfeitoNefasto *efeito = temp->efeitos;
        while (efeito != NULL)
        {
            total_efeitos++;
            efeito = efeito->next;
        }
        fwrite(&total_efeitos, sizeof(int), 1, f);

        // escrever efeitos da antena atual
        efeito = temp->efeitos;
        while (efeito != NULL)
        {
            fwrite(&efeito->x, sizeof(int), 1, f);
            fwrite(&efeito->y, sizeof(int), 1, f);
            efeito = efeito->next;
        }

        temp = temp->next;
    }

    fclose(f);
    return 1;
}

/**
 * @brief Função para ler o ficheiro binário de antenas e efeitos
 * @param nome_ficheiro Nome do ficheiro binário
 */

int lerBin(const char *nome_ficheiro)
{
    FILE *f = fopen(nome_ficheiro, "rb");
    if (!f)
    {
        perror("Erro ao abrir ficheiro binário");
        return 0;
    }

    char bloco[4] = {0};
    if (fread(bloco, sizeof(char), 3, f) != 3 || strcmp(bloco, "ANT") != 0)
    {
        printf("Formato de ficheiro inválido!\n");
        fclose(f);
        return 0;
    }

    int total_antenas;
    fread(&total_antenas, sizeof(int), 1, f);
    printf("=== ANTENAS (%d) ===\n", total_antenas);

    for (int i = 0; i < total_antenas; i++)
    {
        int x, y, total_efeitos;
        char freq;

        fread(&x, sizeof(int), 1, f);
        fread(&y, sizeof(int), 1, f);
        fread(&freq, sizeof(char), 1, f);
        fread(&total_efeitos, sizeof(int), 1, f);

        printf("Antena: (%d, %d) - Frequência: %c\n", x, y, freq);
        printf("  Efeitos nefastos (%d):", total_efeitos);

        for (int j = 0; j < total_efeitos; j++)
        {
            int efeito_x, efeito_y;
            fread(&efeito_x, sizeof(int), 1, f);
            fread(&efeito_y, sizeof(int), 1, f);
            printf("   (%d, %d) \n", efeito_x, efeito_y);
        }
    }

    fclose(f);
    return 1;
}