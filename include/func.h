/**
 * @file func.h
 * @author Bruno (a31496@alunos.ipca.pt)
 * @brief
 * @version 1.2
 * @date 2025-03-25
 * @copyright Copyright (c) 2025
 */

#ifndef FUNC_H
#define FUNC_H

#include "antena.h"

#pragma region Ficheiros

/**
 * @brief carrega as antenas através de um ficheiro
 * @param nomeFicheiro nome do ficheiro txt
 * @return lista de antenas
 */

Antena *carregarFicheiro(const char *nomeFicheiro);

#pragma endregion

#pragma region Antenas

/**
 * @brief Cria antena através dos parametros
 * @param x coordenada x
 * @param y coordenada y
 * @param freq  frequência
 * @return
 */

Antena *criarAntena(int x, int y, char freq);

/**
 * @brief insere uma antena no inicio da lista para os efeitos nefastos
 * @param lista
 * @param novo
 * @return
 */

Antena *inserirNefasto(Antena *lista, Antena *nova);

/**
 * @brief insere uma antena no fim da lista
 * @param lista
 * @param nova
 * @return
 */

Antena *inserirAntena(Antena *lista, Antena *nova);

/**
 * @brief remove uma antena através das suas coordenadas
 * @param lista
 * @return
 */

Antena *removerAntena(Antena *lista);

/**
 * @brief função para verificar a existencia de uma antena em determinadas coordenadas
 * @param lista
 * @param x coordenada x
 * @param y coordenada y
 * @return 0 não existe, 1 existe
 */

int eisteAntena(Antena *lista, int x, int y);

/**
 * @brief procura na lista de antenas se tem efeits nefastos, se tiver cria uma antena(#) para indicar os efeitos
 * @param lista
 * @return
 */

Antena *adicionarEfeitoNefasto(Antena *lista);

/**
 * @brief exibe as coordenadas das antenas tendo em conta os efeitos nefastos se existirem
 * @param lista
 */

void mostrarCoords(Antena *lista);
#pragma endregion

#endif