/**
 * @file antena.h
 * @author Bruno (a31496@alunos.ipca.pt)
 * @brief
 * @version 1.2
 * @date 2025-03-25
 * @copyright Copyright (c) 2025
 */

#ifndef ANTENA_H
#define ANTENA_H

/**
 * @brief Estrutura de efeito nefasto
 */

typedef struct EfeitoNefasto
{
    int x, y;                   // coordenadas do efeito
    struct EfeitoNefasto *next; // próximo efeito
} EfeitoNefasto;

/**
 * @brief Estrutura de antena
 */

typedef struct Antena
{
    int x, y;               //  coordenadas da antena (posição)
    char freq;              //  frequência (nome na matriz)
    struct Antena *next;    //  pointer para a proxima
    EfeitoNefasto *efeitos; // lista de efeitos nefastos
} Antena;

#endif
