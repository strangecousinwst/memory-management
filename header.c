
// struct que define o cabeçalho
// de um bloco de memória
typedef struct header header_t;

// size: tamanho do bloco de memória
// next: apontador para o próximo bloco de memória
typedef struct header {
  unsigned int size;
  struct header *next;
} header_t;

// base: bloco de memória sem tamanho
static header_t base;
// freep: aponta para o primeiro bloco de memória livre
static header_t *freep = &base;
// usedp: aponta para o primeiro bloco de memória ocupado
static header_t *usedp;

static void add_to_free_list(header_t *bp) {

  header_t *p;

  for (p = freep; !(bp > p && bp < p->next); p = p->next) {
    if (p >= p->next && (bp > p || bp < p->next)) {
      break;
    }
  }

  if (bp + bp->size == p->next) {
    bp->size += p->next->size;
    bp->next = p->next->next;
  } else {
    bp->next = p->next;
  }

  if (p + p->size == bp) {
    p->size += bp->size;
    p->next = bp->next;
  } else {
    p->next = bp;
  }

  freep = p;
}
