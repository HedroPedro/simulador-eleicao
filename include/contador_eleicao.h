#ifndef CONTADOR_ELEICAO_H_
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#define NAME_SIZE 50
#define PRIMEIRO 1
#define SEGUNDO 2
#define EMPATE -1
#define BOLETIM_PATH "boletim.txt"
#define MIN_SEGUNDO_TURNO 10
#define PRINT_ERROR_DEFAULT(fd, reason) fprintf(fd, "ERRO NA LINHA %i; RAZAO: ", __LINE__, reason)
#define PRINT_ERROR_DEBUG(reason) PRINT_ERROR_DEFAULT(stderr, reason)
#define PRINT_ERROR(reason) fprintf(stderr,"%s\n", reason);

typedef unsigned char id;
typedef unsigned int uint;
typedef struct chapa Chapa;
typedef struct lista Lista;
typedef struct urna Urna;
typedef long long llong;
typedef enum {
	TERMINADO = 0,
	INICIO = 1,
	CADASTRO,
	VOTACAO,
	SEGUNDA_FASE
} FaseUrna;

Urna *new_urna(void);
void save_urna_to_file(Urna* urna, FaseUrna* fase_atual);
void create_chapa(Urna *urna);
void add_voto(Urna *urna, id n_chapa);
void print_chapas_for_eleitores(Urna* urna);
void sort_urna_by_votos(Urna *urna);
void* free_urna(Urna *urna);
void print_second_turn_chapas(Urna *urna);
void determine_winner_from_second_turn(Urna* urna);
void delay(time_t seconds);
#endif // !CONTADOR_ELEICAO_H_