/**
 * @file gc_memoria.c
 * @brief Implementaçao da gestao de memoria do coletor de lixo.
 * 
 * Este arquivo contem as funçoes relacionadas a alocaçao e gestao
 * de memoria do coletor de lixo.
 * 
 * @author Joao Mendes
 * @date Abril 2025
 */

#include "gc.h"
#include "gc_interno.h"
#include <stdlib.h>
#include <string.h>

/**
 * @brief Realoca memoria para um objeto gerenciado pelo coletor de lixo.
 * 
 * Esta funçao aumenta ou diminui o tamanho de um objeto ja alocado,
 * mantendo seu conteudo original.
 * 
 * @param gc Apontador para o coletor de lixo.
 * @param ptr Apontador para o objeto a ser realocado.
 * @param novo_tamanho Novo tamanho desejado em bytes.
 * @return Apontador para a memoria realocada ou NULL em caso de falha.
 */
void gc_realocar(gc_t *gc, void *ptr, size_t novo_tamanho) {
  if (!gc) {
    return NULL; // Erro: coletor nulo
  }

  // Se o apontador for nulo, aloca nova memoria
  if (!ptr) {
    return gc_alocar(gc, novo_tamanho);
  }

  // Se o novo tamanho for zero, liberta a memoria
  if (novo_tamanho == 0) {
    gc_libertar(gc, ptr);
    return NULL;
  }
}       