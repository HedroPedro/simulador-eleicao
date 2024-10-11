#ifndef CONTADOR_ELEICAO_H_
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <string.h>
#define ANO_ATUAL 2024
#define NAME_SIZE 50
#define BOLETIM_PATH "boletim.txt"
#define MIN_SEGUNDO_TURNO 10
#define PRINT_ERROR_DEBUG(reason) fprintf(stderr, "ERRO NA LINHA %i; RAZAO: %s\n", __LINE__, reason)
#define PRINT_ERROR(reason) fprintf(stderr,"\033[4;31m%s\n\033[0m", reason);

#ifdef _WIN32 
#define CLEAR_TERMINAL system("cls") 
#else
#define CLEAR_TERMINAL system("clear")
#endif

typedef struct urna Urna;
typedef unsigned int uint;
typedef long long llong;

typedef enum {
	TERMINADO = 0,
	INICIO = 1,
	CADASTRO_CHAPAS,
	PRIMEIRO_TURNO,
	SEGUNDO_TURNO
} FaseUrna;

Urna *new_urna(uint vetor_size);
void add_chapa(Urna *urna);
void add_voto(Urna *urna, uint voto, FaseUrna fase_atual);
void print_chapas(Urna *urna, FaseUrna fase_atual);
void ordena_urna_by_voto(Urna *urna);
void computar_turno(Urna* urna, const char* file_path, FaseUrna *fase_atual);
void free_urna(Urna **urna);
void delay(time_t seconds);
#endif