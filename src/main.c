/**
 * @file main.c
 * @author Bruno (a31496@alunos.ipca.pt)
 * @brief
 * @version 1.2
 * @date 2025-03-25
 * @copyright Copyright (c) 2025
 */

#include <stdio.h>
#include <stdlib.h>
#include "../include/antena.h"
#include "func.c"

int main()
{
    Antena *lista;
    int res;

    //  carregar antenas
    lista = carregarFicheiro("antenas.txt");

    printf("\nDeteção de efeitos nefasto \n");

    //  atualizar com efeitos nefastos
    lista = adicionarEfeitoNefasto(lista);

    printf("\nRemoção de antena \n");

    //  remover antenas
    lista = removerAntena(lista, 1, 0);

    //  insere as antenas e efeitos nefasto no ficheiro binário
    res = escreverBin(lista, "output.bin");

    //  le e escreve o conteúdo do ficheiro binário
    res = lerBin("output.bin");

    return 0;
}