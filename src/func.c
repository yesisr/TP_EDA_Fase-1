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
    }
    return temp;
}

/**
 * @brief insere uma antena no inicio da lista para os efeitos nefastos
 * @param lista
 * @param novo
 * @return
 */

Antena *inserirNefasto(Antena *lista, Antena *nova)
{
    nova->next = lista;
    return nova;
}

/**
 * @brief insere uma antena no fim da lista
 * @param lista
 * @param nova
 * @return
 */

Antena *inserirAntena(Antena *lista, Antena *nova)
{
    if (lista == NULL)
    {
        return nova;
    }
    Antena *temp = lista;
    while (temp->next != NULL)
    {
        temp = temp->next;
    }
    temp->next = nova;
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

/**
 * @brief procura na lista de antenas se tem efeits nefastos, se tiver cria uma antena(#) para indicar os efeitos
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
            //  precorre a lista até encontrar duas antenas com a mesma frequencia
            if (atual->freq == proximo->freq)
            {
                //  distancia de coordenadas entre a proxima antena e atual
                //  para calcular o efeito nefasto
                //  exmpl: 2,2 - 1,1 = 1,1
                int diferencaX = proximo->x - atual->x;
                int diferencaY = proximo->y - atual->y;

                //  coordenadas do primeiro efeito nefasto
                //  exmpl: 1,1 - 1,1 = 0,0
                int efeitoX1 = atual->x - diferencaX;
                int efeitoY1 = atual->y - diferencaY;

                // coordenadas do segundo efeito nefasto
                //  exmpl: 2,2 + 1,1 = 3,3
                int efeitoX2 = proximo->x + diferencaX;
                int efeitoY2 = proximo->y + diferencaY;

                //  verifica se as coordenadas são possiveis e se existe antena nas coordenadas
                if (efeitoX1 >= 1 && efeitoX1 <= max_x &&
                    efeitoY1 >= 1 && efeitoY1 <= max_y &&
                    !existeAntena(lista, efeitoX1, efeitoY1))
                {
                    //  cria efeito antes da primeira antena
                    lista = inserirNefasto(lista, criarAntena(efeitoX1, efeitoY1, '#'));
                }
                //  verifica se as coordenadas são possiveis e se existe antena nas coordenadas
                if (efeitoX2 >= 1 && efeitoX2 <= max_x &&
                    efeitoY2 >= 1 && efeitoY2 <= max_y &&
                    !existeAntena(lista, efeitoX2, efeitoY2))
                {
                    //  cria efeito depois da segunda antena
                    lista = inserirNefasto(lista, criarAntena(efeitoX2, efeitoY2, '#'));
                }
            }
            proximo = proximo->next;
        }
        atual = atual->next;
    }

    return lista;
}

/**
 * @brief exibe as coordenadas das antenas tendo em conta os efeitos nefastos se existirem
 * @param lista
 */

void mostrarCoords(Antena *lista)
{
    if (lista == NULL)
    {
        printf("Nenhuma antena encontrada.\n");

        return;
    }

    printf("\nLista de Antenas:\n");
    while (lista)
    {
        // se a freq for um # então é um efeito nefasto
        if (lista->freq == '#')
            printf("Efeito Nefasto em (%d, %d)\n", lista->x, lista->y);
        else
            printf("Antena em (%d, %d) - Frequência: %c\n", lista->x, lista->y, lista->freq);
        lista = lista->next;
    }
}

#pragma endregion