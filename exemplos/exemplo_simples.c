/**
 * @file exemplo_simples.c
 * @brief Exemplo simples do uso do coletor de lixo.
 * 
 * Este exemplo demonstra a alocaçao de memoria, criaçao de referencias
 * e coleta de lixo.
 * 
 * @author Joao Mendes
 * @date Abril 2025
 */

#include "../src/gc.h"
#include <stdio.h>
#include <stdlib.h>

/**
 * @brief Estrutura de dados de exemplo para teste.
 */
typedef struct Pessoa {
    int id;
    char *nome;
} pessoa_t;

/**
 * @brief Ponto de entrada do programa.
 */
int main()
{
    // Inicializa o coletor de lixo com 1MB de heap
    gc_t *gc = gc_inicializar(1024 * 1024);
    if (!gc) {
        fprintf(stderr, "Erro ao inicializar o coletor de lixo.\n");
        return 1;
    }

    printf("Exemplo simples de Coletor de Lixo\n");

    // Alocar memoria para objetos
    pessoa_t *p1 = (pessoa_t*)gc_alocar(gc, sizeof(pessoa_t));
    p1->id = 1;
    p1->nome = (char*)gc_alocar(gc, 20);
    snprintf(p1->nome, 20, "Zacarias");

    pessoa_t *p2 = (pessoa_t*)gc_alocar(gc, sizeof(pessoa_t));
    p2->id = 2;
    p2->nome = (char*)gc_alocar(gc, 20);
    snprintf(p2->nome, 20, "Lagostim");

    // Registar as raizes
    gc_registar_raiz(gc, p1);
    gc_registar_raiz(gc, p2);

    // Registar as referencias
    gc_registar_referencia(gc, p1->nome, p1);
    gc_registar_referencia(gc, p2->nome, p2);

    // Imprimir os dados
    printf("Pessoa 1: ID=%d, Nome=%s\n", p1->id, p1->nome);
    printf("Pessoa 2: ID=%d, Nome=%s\n", p2->id, p2->nome);

    // Mostrar estatisticas antes da coleta
    size_t total_alocado, total_livre, num_objetos;
    gc_obter_estatisticas(gc, &total_alocado, &total_livre, &num_objetos);
    printf("Estatisticas antes da coleta:\n");
    printf("Total alocado: %zu bytes\n", total_alocado);
    printf("Total livre: %zu bytes\n", total_livre);
    printf("Numero de objetos: %zu\n", num_objetos);

    // Remove uma raiz, tornando o objeto inacessivel
    gc_remover_raiz(gc, p2);

    // Forçar a coleta de lixo
    size_t bytes_libertados = gc_coletar(gc);
    printf("Coleta de lixo realizada. Bytes libertados: %zu\n", bytes_libertados);

    // Mostrar estatisticas apos a coleta
    gc_estatisticas(gc, &total_alocado, &total_livre, &num_objetos);
    printf("Estatisticas apos a coleta:\n");
    printf("Total alocado: %zu bytes\n", total_alocado);
    printf("Total livre: %zu bytes\n", total_livre);
    printf("Numero de objetos: %zu\n", num_objetos);

    // Tentar acessar o objeto p2 (deve falhar)
    printf("Tentando acessar p1 (ainda valido): ID=%d, Nome=%s\n", p1->id, p1->nome);

    // Finalizar o coletor de lixo
    gc_finalizar(gc);

    return 0;
}