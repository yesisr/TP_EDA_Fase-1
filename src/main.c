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

    //  carregar antenas
    lista = carregarFicheiro("antenas.txt");

    //  mostrar antenas
    mostrarCoords(lista);

    printf("\nDeteção de efeitos nefasto \n");

    //  atualizar com efeitos nefastos
    lista = adicionarEfeitoNefasto(lista);

    //  mostrar antenas após efeito nefasto
    mostrarCoords(lista);

    printf("\nRemoção de antena \n");

    //  remover antenas
    lista = removerAntena(lista, 1, 0);

    //  mostra antenas após remoção
    mostrarCoords(lista);

    return 0;
}