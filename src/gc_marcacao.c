/**
 * @file gc_marcacao.c
 * @brief Implementação do algoritmo de marcacao do coletor de lixo.
 * 
 * Este arquivo contem as funções relacionadas a fase de marcacao
 * do algoritmo de mark-and-sweep.
 * 
 * @author Joao Mendes
 * @date Abril 2025
 */

#include "gc.h"
#include "gc_interno.h"
#include <stdlib.h>

/** 
 * @brief Marca um objeto e todos os objetos alcançaveis a partir dele.
 * 
 * Esta função marca o objeto e recursivamente marca todos os objetos
 * alcançaveis a partir dele.
 * 
 * @param gc Apontador para o coletor de lixo.
 * @param objeto Apontador para o objeto a ser marcado.
*/
void gc_marcar(gc_t *gc, void *objeto) {
  if (!gc || !objeto) {
    return; // Erro: coletor nulo ou objeto nulo
  }

  // Encontrar o objeto correspondente ao apontador
  gc_object_t *gc_obj = gc_encontrar_objeto(gc, objeto);
    if (!gc_obj || gc_obj->marcado == GC_OBJETO_MARCADO) {
      return; // Erro: objeto não encontrado ou já marcado
    }

  // Marca o objeto
  gc_obj->marcado = GC_OBJETO_MARCADO;

  // Marca recursivamente todos os objetos referenciados por este objeto
  for (size_t i = 0; i < gc->num_referencias; i++) {
    if (gc->referencias[i].de == objeto) {
      gc_marcar(gc, gc->referencias[i].para);
    }
  }
}   

/**
 * @brief Desmarca todos os objetos.
 * 
 * Esta função é usada no início da fase de marcaçao para
 * redefinir o estado de todos os objetos.
 * 
 * @param gc Ponteiro para o coletor de lixo.
 */
void gc_desmarcar_todos(gc_t* gc) {
    if (!gc) {
        return; // Erro: coletor nulo
    }

    gc_object_t *gc_obj = gc->objetos;
    while (gc_obj) {
        gc_obj->marcado = GC_OBJETO_NAO_MARCADO;
        gc_obj = gc_obj->proximo;
    }   
}

/**
 * @brief Marca todos os objetos alcançaveis a partir das raizes.
 * 
 * Esta funçao percorre todas as raizes registadas e marca
 * todos os objetos alcançaveis a partir delas.
 * 
 * @param gc Apontador para o coletor de lixo.
 */
void gc_marcar_alcancaveis(gc_t *gc) {
    if (!gc) {
        return; // Erro: coletor nulo
    }

    // Desrmarcar todos os objetos primeiro
    gc_desmarcar_todos(gc);

    for (size_t i = 0; i < gc->num_raizes; i++) {
        gc_marcar(gc, gc->raizes[i]);
    }
}