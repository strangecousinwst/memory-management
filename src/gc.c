/**
 * @file gc.c
 * @brief Implementação principal do coletor de lixo.
 *
 * Este arquivo contém a implementação das funções principais
 * do coletor de lixo.
 *
 * @author Joao Mendes
 * @date Abril 2025
 */

#include "gc.h"
#include "gc_interno.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * @brief Inicializar um novo coletor de lixo.
 *
 * @param tamanho_heap Tamanho inicial do heap em bytes.
 * @return Apontador para o coletor de lixo inicializado, ou NULL em caso de
 * falha.
 */
GC *gc_inicializar(size_t tamanho_heap) {
  if (tamanho_heap == 0) {
    tamanho_heap = 1024 * 1024; // 1MB por padrão
  }

  GC *gc = (GC *)malloc(sizeof(GC));
  if (!gc) {
    return NULL;
  }

  // Inicializar campos do coletor
  gc->objetos = NULL;
  gc->num_raizes = 0;
  gc->num_referencias = 0;
  gc->tamanho_heap = tamanho_heap;
  gc->memoria_usada = 0;
  gc->coletas_realizadas = 0;

  return gc;
}

/**
 * @brief Aloca memoria gerenciada pelo coletor de lixo.
 *
 * @param gc Apontador para o coletor de lixo.
 * @param tamanho Tamanho da memoria a ser alocada em bytes.
 * @return Apontador para a memoria alocada, ou NULL em caso de falha.
 */
void *gc_alocar(GC *gc, size_t tamanho) {
  if (!gc || tamanho == 0) {
    return NULL;
  }

  // Verifica se é necessário coletar lixo antes de alocar.
  if (gc_verificar_limiar_coleta(gc)) {
    gc_coletar(gc);
  }

  // Criar novo objeto gc
  GCObject *novo_objeto = (GCObject *)malloc(sizeof(GCObject));
  if (!novo_objeto) {
    return NULL;
  }

  // Alocar memoria para os dados do objeto
  void *dados = malloc(tamanho);
  if (!dados) {
    free(novo_objeto);
    return NULL;
  }

  // Inicializar o novo_objeto
}
