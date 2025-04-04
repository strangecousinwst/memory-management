/**
 * @file gc_interno.h
 * @brief Definições privadas do coletor de lixo.
 *
 * Este arquivo contém a API pública para o coletor de lixo implementado em C.
 * O coletor utiliza o algoritmo mark-and-sweep.
 *
 * @author Joao Mendes
 * @date Abril 2025
 */

#ifndef GC_INTERNO_H
#define GC_H

#include <gc.h>
#include <stdbool.h>
#include <stddef.h>

/**
 * @brief Constantes usadas pelo coletor de lixo.
 *
 * @param GC_OBJETO_MARCADO Indica que um objeto está marcado como alcançavel.
 * @param GC_OBJETO_NAO_MARCADO Indica que um objeto não está marcado.
 * @param GC_MAX_RAIZES Número máximo de raízes que podem ser registadas.
 * @param GC_MAX_REFERENCIAS Máximo de referências que podem ser registadas.
 * @param GC_LIMIAR_COLETA Limiar de ocupação da heap para acionar a coleta.
 */
#define GC_OBJETO_MARCADO 1
#define GC_OBJETO_NAO_MARCADO 0
#define GC_MAX_RAIZES 1024
#define GC_MAX_REFERENCIAS 8192
#define GC_LIMIAR_COLETA 0.75

/**
 * @brief Estrutura para representar um objeto gerenciado pelo coletor.
 *
 * @param tamanho Tamanho do objeto em bytes.
 * @param marcado Indica se o objeto está marcado como alcançável.
 * @param dados Apontador para os dados do objeto.
 * @param proximo Apontador para o próximo objeto na lista ligada.
 */
typedef struct GCObject {
  size_t tamanho;
  int marcado;
  void *dados;
  struct GCObject *proximo;
} gc_object_t;

/**
 * @brief Estrutura para representar uma referência entre objetos.
 *
 * @param de Apontador para o objeto de origem.
 * @param para Apontador para o objeto de destino.
 */
typedef struct GCReferencia {
  void *de;
  void *para;
} gc_referencia_t;

/**
 * @brief Estrutura principal do coletor de lixo.
 *
 * @param tamanho Lista de todos os objetos alocados.
 * @param raizes Array de raizes registadas.
 * @param num_raizes Numero de raizes registadas.
 * @param referencias Array de referencias.
 * @param num_referencias Numero de referencias registadas.
 * @param tamanho_heap Tamanho total da heap.
 * @param memoria_usada Memória atualmente usada.
 * @param coletas_realizadas Número de coletas realizadas.
 */
typedef struct GC {
  gc_object_t *objetos;
  void *raizes[GC_MAX_RAIZES];
  size_t num_raizes;
  gc_referencia_t *referências[GC_MAX_REFERENCIAS];
  size_t num_referencias;
  size_t tamanho_heap;
  size_t memoria_usada;
  size_t coletas_realizadas;
} gc_t;

/**
 * @brief Marca um objeto e todos os objetos alcançaveis a partir dele.
 *
 * @param gc Apontador para o coletor de lixo.
 * @param objeto Apontador para o objeto a ser marcado.
 */
void gc_marcar(gc_t *gc, void *objeto);

/**
 * @brief Varre o heap e liberta objetos não marcados/alcançaveis.
 *
 * @param gc Apontador para o coletor de lixo.
 * @return Numero de bytes libertados.
 */
size_t gc_varrer(gc_t *gc);

/**
 * @brief Encontra o objeto referente a um apontador.
 *
 * @param gc Apontador para o coletor de lixo.
 * @param dados Apontador para os dados de um objeto.
 * @return Apontador para um gc_object_t ou NULL se nao encontrado.
 */
gc_object_t gc_encontrar_objeto(gc_t *gc, void *dados);

/**
 * @brief Verifica se é necessario realizar coleta de lixo.
 *
 * @param gc Apontador para o coletor de lixo.
 * @return true se uma coleta for necessario, false caso contrario.
 */
bool gc_verificar_limiar_coleta(gc_t *gc);

#endif // !GC_INTERNO_H
