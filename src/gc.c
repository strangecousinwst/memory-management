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
gc_t *gc_inicializar(size_t tamanho_heap) {
  if (tamanho_heap == 0) {
    tamanho_heap = 1024 * 1024; // 1MB por padrão
  }

  gc_t *gc = (gc_t *)malloc(sizeof(gc_t));
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
void *gc_alocar(gc_t *gc, size_t tamanho) {
  if (!gc || tamanho == 0) {
    return NULL;
  }

  // Verifica se é necessário coletar lixo antes de alocar.
  if (gc_verificar_limiar_coleta(gc)) {
    gc_coletar(gc);
  }

  // Criar novo objeto gc
  gc_object_t *novo_objeto = (gc_object_t *)malloc(sizeof(gc_object_t));
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
  novo_objeto->dados = dados;
  novo_objeto->tamanho = tamanho;
  novo_objeto->marcado = GC_OBJETO_NAO_MARCADO;

  // Adicionar o novo objeto à lista de objetos do coletor
  novo_objeto->proximo = gc->objetos;
  gc->objetos = novo_objeto;

  // Atualizar a memoria usada
  gc->memoria_usada += tamanho;

  return dados;
}

/**
 * @brief Registra uma referência de um objeto para outro.
 * 
 * @param gc Ponteiro para o coletor de lixo.
 * @param de Ponteiro para o objeto de origem.
 * @param para Ponteiro para o objeto de destino.
 * @return 0 em caso de sucesso, valor negativo em caso de erro.
 */
int gc_registar_referencia(gc_t *gc, void *de, void *para) {
  if (!gc || !de || !para) {
    return -1; // Erro: um dos apontadores está nulo
  }

  // Verifica se já atingimos o limite de referências
  if (gc->num_referencias >= GC_MAX_REFERENCIAS) {
    return -2; // Erro: limite de referências atingido
  }

  // Regista nova referência
  gc->referencias[gc->num_referencias].de = de;
  gc->referencias[gc->num_referencias].para = para;
  gc->num_referencias++;

  return 0;
}

/**
 * @brief Executa o algoritmo de coleta de lixo.
 * 
 * Esta função força a execuçao do algoritmo de coletar memoria,
 * marca os objetos alcançáveis e libera os não alcançáveis.
 * 
 * @param gc Apontador para o coletor de lixo.
 * @return Número de bytes liberados.
 */
size_t gc_coletar(gc_t *gc) {
  if (!gc) {
    return 0; // Erro: coletor de lixo nulo
  }

  size_t memoria_anterior = gc->memoria_usada;

  // Desmarcar todos os objetos
  gc_object_t *obj = gc->objetos;
  while (obj) {
    obj->marcado = GC_OBJETO_NAO_MARCADO;
    obj = obj->proximo;
  }
  
  // Marcar objetos alcançaveis a partir das raízes
  for (size_t i = 0; i < gc->num_raizes; i++) {
    gc_marcar(gc, gc->raizes[i]);
  }

  // Varrer objetos nao marcados
  size_t bytes_libertados = gc_varrer(gc);

  // Incrementar contador
  gc->coletas_realizadas++;

  return bytes_libertados;
}

/**
 * @brief Libera todos os recursos do coletor de lixo.
 * 
 * @param gc Apontador para o coletor de lixo a ser libertado.
 */
void gc_finalizar(gc_t *gc) {
  if (!gc) {
    return; // Erro: coletor de lixo nulo
  }

  // Liberar todos os objetos alocados
  gc_object_t *obj = gc->objetos;
  while (obj) {
    gc_object_t *prox = obj->proximo;
    free(obj->dados);
    free(obj);
    obj = prox;
  }

  // Liberar o coletor de lixo
  free(gc);
}

/**
 * @brief Regista uma raiz para o algoritmo de marcação.
 * 
 * @param gc Ponteiro para o coletor de lixo.
 * @param raiz Ponteiro para a raiz a ser registrada.
 * @return 0 em caso de sucesso, valor negativo em caso de erro.
 */
int gc_registar_raiz(gc_t *gc, void *raiz) {
  if (!gc || !raiz) {
    return -1; // Erro: coletor de lixo ou raiz nulo
  }

  // Verifica se já atingimos o limite de raízes
  if (gc->num_raizes >= GC_MAX_RAIZES) {
    return -2; // Erro: limite de raízes atingido
  }

  // Registra nova raiz
  gc->raizes[gc->num_raizes++] = raiz;

  return 0;
}

/**
 * @brief Remove uma raiz do registro do coletor de lixo.
 * 
 * @param gc Ponteiro para o coletor de lixo.
 * @param raiz Ponteiro da raiz a ser removida.
 * @return 0 em caso de sucesso, valor negativo em caso de erro.
 */
int gc_remover_raiz(gc_t *gc, void *raiz) {
  if (!gc || !raiz) {
   return -1; // Erro: coletor de lixo ou raiz nulo 
  }

  // Procurar a raiz
  for (size_t i = 0; i < gc->num_raizes; i++) {
    // Se encontrar, remove
    if (gc->raizes[i] == raiz) {
      // Deslocar as restantes raízes para a esquerda
      for (size_t j = i; j < gc->num_raizes - 1; j++) {
        gc->raizes[j] = gc->raizes[j + 1];
      }
      gc->num_raizes--;
      return 0; // Sucesso
    }
  }

  return -2; // Erro: raiz nao encontrada
}

/**
 * @brief Retorna estatísticas sobre o uso de memória do coletor de lixo.
 * 
 * @param gc Ponteiro para o coletor de lixo.
 * @param total_alocado Ponteiro onde será armazenado o total de memória alocada.
 * @param total_livre Ponteiro onde será armazenado o total de memória livre.
 * @param num_objetos Ponteiro onde será armazenado o número total de objetos.
 */
void gc_estatisticas(gc_t *gc, size_t *total_alocado, size_t *total_livre,
                      size_t *num_objetos) {
  if (!gc) {
    if (total_alocado) *total_alocado = 0;
    if (total_livre) *total_livre = 0;
    if (num_objetos) *num_objetos = 0;
    return; // Erro: coletor nulo
  }

  // Cacular total alocado
  if (total_alocado) {
    *total_alocado = gc->memoria_usada;
  }

  // Cacular total livre
  if (total_livre) {
    *total_livre = gc->tamanho_heap - gc->memoria_usada;
  }

  // Conta o número de objetos
  size_t count = 0;
  gc_object_t *obj = gc->objetos;
  while (obj) {
    count++;
    obj = obj->proximo;
  }

  // Armazena o número de objetos
  if (num_objetos) { *num_objetos = count; }
}

/**
 * @brief Verifica se é necessário realizar uma coleta de lixo.
 * 
 * @param gc Ponteiro para o coletor de lixo.
 * @return true se uma coleta deve ser realizada, false caso contrário.
 */
bool gc_verificar_limiar_coleta(gc_t *gc) {
  if (!gc) {
    return false; // Erro: coletor nulo
  }

  // Verifica se a memória usada ultrapassa o limiar
  float ocupacao = (float)gc->memoria_usada / (float)gc->tamanho_heap;
  return ocupacao > GC_LIMIAR_COLETA;
}

/**
 * @brief Encontra o objeto correspondente a um apontador de dados.
 * 
 * @param gc Apontador para o coletor de lixo.
 * @param dados Apontador para os dados.
 * @return Apontador para o gc_object_t ou NULL se não encontrado.
 */
gc_object_t *gc_encontrar_objeto(gc_t *gc, void *dados) {
  if (!gc || !dados) {
    return NULL; // Erro: coletor nulo ou dados nulos
  }

  gc_object_t *obj = gc->objetos;
  while (obj) {
    if (obj->dados == dados) {
      return obj; // Objeto encontrado
    }
    obj = obj->proximo;
  }

  return NULL; // Objeto não encontrado
}