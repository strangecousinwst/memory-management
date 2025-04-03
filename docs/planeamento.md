# Projeto Gerenciamento de memória em C

## Objetivo

* Desenvolver um garbage collector para a linguagem C, utilizando o algoritmo mark-and-sweep para gerenciar automaticamente a memória alocada, e libertar a não utilizada.

* O objetivo com este projeto é estudar e investigar a gestão de memória, documentar o processo, e disso tudo resultar a criação de uma API que permita fazer operações semelhantes às de um garbage collector como aqueles que se encontram em linguagens de programação como Java, Python, etc..

* Este objetivo surge de uma grande curiosidade para perceber melhor como funciona a memória por detrás de todas as abstrações que as linguagens modernas nos fornecem.

* Para este fim, vou estudar como foram implementados garbage collectors noutras linguagens, algoritmos como mark-and-sweep, o funcionamento do heap e do stack.

## Autor, data, contexto do trabalho

_**João Mendes**_

_**Março/Abril 2025**_

_**UF26 – 10790 - Projeto de programação**_

## Descrição do problema

* Em linguagens de programação mais recentes e sofisticadas, existem mecanismos de gerenciamento de memória, que, em segundo plano, retiram ao programador a preocupação e responsabilidade de questões triviais como a alocação/realocação e a libertação da memória.

* Desta maneira o programador não tem de estar constantemente com o gerenciamento manual da memória no seu programa.
Esta tarefa é feita de modo automático e dinâmica pelo Garbage Collector.

* O problema, na minha opinião consiste na grande abstração que existe por detrás destes mecanismos, e a falta de conhecimento fundamental que resulta do comodismo de usar estas facilidades, por vezes fazem com que tomemos estas operações cruciais como garantidas, uma vez que as linguagens tratam destes processos todos.

## Ferramentas e versões

* Linguagem: C

* Compilador: Gcc --version 14.2.1

* Editor de texto: Nvim --version 0.11.0

* Geração de executáveis: Gmake --version 4.4.1

## Análise de dados

* O projeto trabalhará com estruturas de dados para gerenciar a memória.

* Será implementada uma lista ligada para rastrear todos os objetos alocados, arrays para manter referências e raízes, e um sistema de marcação para identificar objetos alcançáveis.  

* O coletor deverá guardar dados como memória total alocada, memória disponível, número de objetos gerenciados e número de coletas realizadas.

## Análise de processos

* O projeto será dividido em várias etapas, incluindo a definição da arquitetura, implementação das funcionalidades principais, e a criação de exemplos de uso e documentação.

## Descrição de processos

1. **Definição da Arquitetura:**

    * Estruturar o projeto com múltiplos arquivos para modularidade.

2. **Implementação das Funcionalidades Principais:**

    * Sistema de mark-and-sweep,

    * Alocação de memória,

    * Rastreamento de referências,

    * Liberação de memória.

3. **Documentação:**

    * Adicionar comentrios e documentação em português.

4. **Criação de Exemplos:**  

    * Demonstrar o uso do coletor de lixo com exemplos simples e complexos.

5. **Instruções de Compilação e Execução:**

    * Criar um Makefile para facilitar a compilação e execução dos exemplos.

## Dicionário

1. **Garbage Collector:**

    * Componente que gerencia automaticamente a memória alocada, liberando memória não utilizada.

2. **Mark-and-Sweep:**

    * Algoritmo utilizado para identificar e liberar memória não utilizada.

3. **Raiz:**

    * Objeto diretamente acessível pelo programa.

4. **Referência:**

    * Conexão entre dois objetos, permitindo que um objeto acesse outro.

## String connection, instâncias, bases de dados

* Como o projeto não envolve diretamente bases de dados, não há string connection ou instâncias a serem listadas.

## Lista de tabelas, campos e tipos

* Como o projeto não envolve diretamente bases de dados, não há tabelas, campos ou tipos a serem listados.

## Namespaces

* O projeto será estruturado em múltiplos ficheiros C, utilizando header files para definir a API pública e os header files internos para as definições das funções.

* Em C não há namespaces específicos como em C++, mas a modularidade será mantida através da separação de arquivos.

## Classes e métodos

* Em C, vou utilizar estruturas (structs) para definir objetos e funções para métodos. As principais estruturas e funções serão:

1. **Estruturas:**

    **GC:** Estrutura principal do coletor de lixo.

    **GCObject:** Estrutura que representa um objeto gerenciado pelo coletor de lixo.

2. **Funções:**

    * **Inicialização e Finalização:**

    `gc_inicializar(tamanho_heap);`

    `gc_finalizar(GC);`

    * **Alocação de Memória:**

    `gc_alocar(GC);`

    * **Gerenciamento de Raízes:**

    `gc_registrar_raiz(GC, raiz);`

    `gc_remover_raiz(GC, raiz);`

    * **Gerenciamento de Referências:**

    `gc_registrar_referencia(GC, de, para);`

    * **Coleta de Lixo:**

    `gc_coletar(GC);`

    * **Estatísticas:**

    `gc_estatisticas(GC, total_alocado, total_livre, num_objetos);`

## Outras informações

1. Documentação:
    * A documentação será fornecida em português, incluindo um README.md e um manual.

2. Exemplos:
    * Serão fornecidos exemplos para demonstrar o uso do coletor de lixo.
