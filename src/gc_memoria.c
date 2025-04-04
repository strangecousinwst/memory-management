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
void* gc_realocar(gc_t *gc, void *ptr, size_t novo_tamanho) {
  if (!gc) {
    return NULL; // Erro: coletor nulo
  }

  // Se o apontador for nulo, aloca nova memoria
  if (!ptr) {
    return gc_alocar(gc, novo_tamanho);
  }

  // Se o novo tamanho for zero, liberta a memoria
  if (novo_tamanho == 0) {
    gc_object_t *gc_obj = gc_encontrar_objeto(gc, ptr);
    if (gc_obj) {
      gc_obj->marcado = GC_OBJETO_NAO_MARCADO;
      gc_varrer(gc);
    }
    return NULL;
  }

  // Encontrar o objeto correspondente ao apontador
  gc_object_t *gc_obj = gc_encontrar_objeto(gc, ptr);
  if (!gc_obj) {
    return NULL; // Erro: objeto nao encontrado
  }

  // Alocar novo bloco de memoria
  void *novo_ptr = gc_alocar(gc, novo_tamanho);
  if (!novo_ptr) {
    return NULL; // Erro: falha na alocacao
  }

  // Copiar o conteudo antigo para o novo
  size_t tamanho_copia = gc_obj->tamanho < novo_tamanho ? gc_obj->tamanho : novo_tamanho;
  memcpy(novo_ptr, ptr, tamanho_copia);

  // Marca o objeto antigo para ser coletado
  gc_obj->marcado = GC_OBJETO_NAO_MARCADO;

  // Atualizar as referencias
  for (size_t i = 0; i < gc->num_referencias; i++) {
    if (gc->referencias[i].de == ptr) {
      gc->referencias[i].de = novo_ptr;
    }
    if (gc->referencias[i].para == ptr) {
      gc->referencias[i].para = novo_ptr;
    }
  }

  // Varrer a memoria para libertar o objeto antigo
  gc_varrer(gc);

  return novo_ptr; // Retorna o novo apontador
}  

/**
 * @brief Reserva memoria para um bloco contiguo de objetos.
 * 
 * Esta funçao e util quando se necessita alocar varios objetos
 * do mesmo tipo de uma so vez.
 * 
 * @param gc Apontador para o coletor de lixo.
 * @param num_elementos Numero de elementos a serem alocados.
 * @param tamanho_elemento Tamanho de cada elemento em bytes.
 * @return Apontador para a memoria alocada ou NULL em caso de falha.
 */
void *gc_alocar_array(gc_t *gc, size_t num_elementos, size_t tamanho_elemento) {
  if (!gc || num_elementos == 0 || tamanho_elemento == 0) {
    return NULL; // Erro: coletor nulo ou tamanhos invalidos
  }

  // Calcular o tamanho total a ser alocado
  size_t tamanho_total = num_elementos * tamanho_elemento;

  // Verificar se houve overflow na multplicacao
  if (tamanho_total / num_elementos != tamanho_elemento) {
    return NULL;
  }

  // Alocar memoria para o array
  void *array = gc_alocar(gc, tamanho_total);
  if (array) {
    // Inicializar o array com zeros
    memset(array, 0, tamanho_total);
  }

  return array; // Retorna o apontador para o array alocado
}

/**
 * @brief Duplica uma string e coloca-a sob gestao do coletor de lixo.
 * 
 * @param gc Apontador para o coletor de lixo.
 * @param str String a ser duplicada.
 * @return Apontador para a nova string ou NULL em caso de falha.
 */
char *gc_duplica_string(gc_t *gc, const char *str) {
  if (!gc || !str) {
    return NULL; // Erro: coletor nulo ou string nula
  }

  // Calcular o tamanho da string
  size_t tamanho = strlen(str) + 1; // +1 para o terminador nulo

  // Alocar memoria para a nova string
  char *nova_str = (char*)gc_alocar(gc, tamanho);
  if (nova_str) {
    // Copiar a string original
    strcpy(nova_str, str);
  }

  return nova_str; // Retorna o apontador para a nova string
} 