/**
 * @file gc_varredura.c
 * @brief Implementaçao do algoritmo de varredura do coletor de lixo.
 * 
 * Este arquivo contem as funçoes relacionadas a fase de varredura
 * do algoritmo de mark-and-sweep.
 * 
 * @author Joao Mendes
 * @date Abril 2025
 */

#include "gc.h"
#include "gc_interno.h"
#include <stdlib.h>

/**
 * @brief Remove todas as referencias que envolvem um determinado objeto.
 * 
 * @param gc Apontador para o coletor de lixo.
 * @param objeto Apontador para o objeto cujas referencias devem ser removidas.
 */
void gc_remover_referencias(gc_t *gc, void *objeto) {
  if (!gc || !objeto) {
    return; // Erro: coletor nulo ou objeto nulo
  }

  // Remove todas as referencias que envolvem o objeto
  size_t i = 0;
  while (i < gc->num_referencias) {
    if (gc->referencias[i].de == objeto || gc->referencias[i].para == objeto) {
      // Remove a referencia
      gc->referencias[i] = gc->referencias[gc->num_referencias - 1];
      // Diminui o numero de referencias
      gc->num_referencias--;
    } else {
      i++;
    }
  }
}

/**
 * @brief Varre o heap e liberta todos os objetos nao marcados.
 * 
 * Esta funçao implementa a fase de varredura do algoritmo de mark-and-sweep,
 * libertando a memoria dos objetos que nao foram marcados como alcançaveis.
 * 
 * @param gc Apontador para o coletor de lixo.
 * @return Numero de bytes libertados.
 */
size_t gc_varrer(gc_t *gc) {
  if (!gc) {
    return 0; // Erro: coletor nulo
  }

  size_t bytes_libertados = 0;
  gc_object_t **atual = (&gc->objetos);

  while (*atual) {
    if ((*atual)->marcado == GC_OBJETO_NAO_MARCADO) {
      // Objeto nao marcado, libertar memoria
      gc_object_t *obj_nao_marcado = *atual;

      // Atualiza o apontador para o proximo objeto
      *atual = obj_nao_marcado->proximo;

      // Contar bytes libertados
      bytes_libertados += obj_nao_marcado->tamanho;
      gc->memoria_usada -= obj_nao_marcado->tamanho;

      // Remover referencias para este objeto
      gc_remover_referencias(gc, obj_nao_marcado->dados);

      // Libertar memoria do objeto
        free(obj_nao_marcado->dados);
        free(obj_nao_marcado);
    } else {
      // Objeto marcado, avançar para o proximo
      atual = &((*atual)->proximo);
    }
  }

  return bytes_libertados;
}