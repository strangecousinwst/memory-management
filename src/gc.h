/**
 * @file gc.h
 * @brief Definições dos métodos públicos do coletor de lixo.
 *
 * Este arquivo contém a API pública para o coletor de lixo implementado em C.
 * O coletor utiliza o algoritmo mark-and-sweep.
 *
 * @author Joao Mendes
 * @date Abril 2025
 */

#ifndef GC_H
#define GC_H

#include <cstddef>
#include <stddef.h>

/**
 * @brief Estrutura que representa o coletor de lixo.
 *
 * Esta estrutura contém todos os dados necessários para o funcionamento
 * do coletor de lixo.
 */
typedef struct GC GC;

/**
 * @brief Estrutura que representa o objecto gerenciado pelo coletor de lixo.
 *
 * Esta estrutura é usada para manter informações sobre os objectos
 * alocados e gerenciados pelo coletor de lixo.
 */
typedef struct GCObject GCObject;

/**
 * @brief Estrutura que representa o objecto gerenciado pelo coletor de lixo.
 *
 * Esta estrutura é usada para manter informações sobre os objectos
 * alocados e gerenciados pelo coletor de lixo.
 */
GC *gc_inicializar(size_t tamanho_heap);

/**
 * @brief Aloca memória gerenciada pelo coletor de lixo.
 *
 * @param gc Apontador para o coletor de lixo a ser usado.
 * @param tamanho_heap Tamanho da memoria a ser alocada em bytes.
 * @return Apontador para a memoria alocada, ou NULL em caso de falha.
 */
void *gc_alocar(GC *gc, size_t tamanho_heap);

/**
 * @brief Regista uma referência de um objecto para outro.
 *
 * Esta função é usada para obter as referências entre objectos
 *
 * @param gc Apontador para o  coletor de lixo a ser usado.
 * @param de Apontador para o objecto de origem.
 * @param para Apontador para o objecto de destino.
 * @return 0 em caso de sucesso, negativo em caso de falha.
 */
int gc_registar_referencia(GC *gc, void *de, void *para);

/**
 * @brief Remove uma referência de um objecto para outro.
 *
 * Esta função executa a o algoritmo do coletor de lixo,
 * marcando objectos alcancaveis, e libertando os não alcancaveis.
 *
 * @param gc Apontador para o  coletor de lixo a ser usado.
 * @return Numero de bytes libertados.
 */
size_t gc_coletar(GC *gc);

/**
 * @brief Liberta todos os recursos do garbage collector
 *
 * @param gc Apontador para o  coletor de lixo a ser usado.
 */
void gc_finalizar(GC *gc);

/**
 * @brief Regista uma raiz para o algoritmo de marcação.
 *
 * Raízes são objectos que são sempre acessíveis,
 * como variáveis globais ou do stack.
 * Estas vao ser pontos de partida para o algoritmo de marcação.
 *
 * @param gc Apontador para o  coletor de lixo a ser usado.
 * @param raiz Apontador para a raiz a ser registada.
 * @return 0 em caso de sucesso, negativo em caso de erro.
 */
int gc_registar_raiz(GC *gc, void *raiz);

/**
 * @brief Remove uma raiz do registo do coletor de lixo.
 *
 * @param gc Apontador para o coletor de lixo a ser usado.
 * @param raiz Apontador para a raiz a ser removida.
 * @return 0 em caso de sucesso, negativo em caso de erro.
 */
int gc_remover_raiz(GC *gc, void *raiz);

/**
 * @brief Retorna estatisticas sobre o uso da memória.
 *
 * @param gc Apontador para o coletor de lixo a ser usado.
 * @param total_alocado Apontador para onde será guardado o total de memoria
 * alocada.
 * @param total_livre Apontador para onde será guardado o total de memória
 * livre.
 * @param num_objetos Apontador para onde será guardado o total de objectos.
 * @return 0 em caso de sucesso, negativo em caso de erro.
 */
void gc_estatisticas(GC *gc, size_t *total_alocado, size_t *total_livre,
                     size_t *num_objetos);

#endif // !GC_H
