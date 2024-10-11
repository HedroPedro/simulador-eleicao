#include "contador_eleicao.h"

int main(int argc, char *argv[]){
	FaseUrna fase_atual = INICIO;
	Urna *urna;
	int contador = 0, comando = -1, qtd_urnas = 0, qtd_pessoas = 0;
	FILE *limpar_arq = fopen(BOLETIM_PATH, "w");
	if (limpar_arq == NULL) {
		PRINT_ERROR_DEBUG("INCAPAZ DE ABRIR ARQUIVO");
		exit(EXIT_FAILURE);
	}
	fclose(limpar_arq);
	while(fase_atual != TERMINADO){
		switch (fase_atual){
		case INICIO:
			while (qtd_urnas < 1 || qtd_urnas > 99) {
				printf("\033[32mDigite a quantidade de chapas presente na urna: ");
				scanf("%i", &qtd_urnas);
			}
			urna = new_urna((uint) qtd_urnas);
			while(qtd_pessoas < 1){
				printf("Digite a quantidade de eleitores: ");
				scanf("%i", &qtd_pessoas);
			}
			fase_atual = CADASTRO_CHAPAS;
			break;
		case CADASTRO_CHAPAS:
			if (qtd_urnas == 0) {
				PRINT_ERROR("QUANTIDADE MAXIMA DE CHAPAS ATINGINGIDAS!");
				fase_atual = PRIMEIRO_TURNO;
				delay(2);
				CLEAR_TERMINAL;
				break;
			}
			printf("\033[32m------ CADASTRO ------\n");
			printf("1- Cadastrar chapa\n");
			printf("0- Finalizar cadastro\n");
			printf("Insira sua opcao: ");
			scanf("%i", &comando);
			switch(comando){
			case 0:
				fase_atual = PRIMEIRO_TURNO;
				CLEAR_TERMINAL;
				break;
			case 1:
				add_chapa(urna);
				qtd_urnas--;
				break;
			default:
				PRINT_ERROR("COMANDO INVALIDO! DIGITE NOVAMENTE!");
				delay(1);
				CLEAR_TERMINAL;
				break;
			}
			break;
		case PRIMEIRO_TURNO:
			while(contador < qtd_pessoas){
				printf("\033[1;37m---------- PRIMEIRO TURNO ----------\n");
				print_chapas(urna, fase_atual);
				printf("Digite seu voto: ");
				scanf("%i", &comando);
				add_voto(urna, comando, fase_atual);
				contador++;
			}
			computar_turno(urna, BOLETIM_PATH, &fase_atual);
			contador = 0;
			break;
		case SEGUNDO_TURNO:
			while(contador < qtd_pessoas) {
				printf("\033[1;37m---------- SEGUNDO TURNO ----------\n");
				print_chapas(urna, fase_atual);
				printf("Digite o seu voto: ");
				scanf("%i", &comando);
				add_voto(urna, comando, fase_atual);
				contador++;
			}
			computar_turno(urna, BOLETIM_PATH, &fase_atual);
			break;
		default:
			break;
		}
	}
	printf("\033[0mAcabou as eleicoes!\n");
	free_urna(&urna);
}

