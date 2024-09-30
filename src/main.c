#include "main.h"

int main(int argc, char *argv[]){
	FaseUrna fase_atual = INICIO;
	Urna *urna = new_urna();
	int qtd_max;
	int qtd_atual = 0;
	int comando = -1;
	char actual_color[] = "\033[32m";
	while (fase_atual != TERMINADO){
		printf("%s------ URNA ------\n", actual_color);
		switch (fase_atual) {
		case INICIO:
			printf("Digite a quantidade de inicial de chapas: ");
			scanf("%i", &qtd_max);
			if (qtd_max < 1 || qtd_max > 99) {
				CLEAR_TERMINAL;
				PRINT_ERROR("Valor invalido!");
				delay(1);
				CLEAR_TERMINAL;
				continue;
			}
			fase_atual = CADASTRO;
			break;
		case CADASTRO:
			if (qtd_atual == qtd_max) {
				PRINT_ERROR("INCAPAZ DE CADASTRAR MAIS CHAPAS INDO PARA O PRIMEIRO TURNO!");
				fase_atual = VOTACAO;
				strcpy(actual_color, "\033[36;5m");
				delay(1);
				CLEAR_TERMINAL;
				break;
			}
			printf("CADASTRO DE CHAPA\n");
			printf("1- Cadastrar chapa\n");
			printf("0- Encerrar cadastro de chapa\n");
			scanf("%i", &comando);
			switch (comando){
			case 1:
				create_chapa(urna);
				qtd_atual++;
				break;
			case 0:
				strcpy(actual_color, "\033[36m");
				fase_atual = VOTACAO;
				CLEAR_TERMINAL;
				break;
			default:
				PRINT_ERROR("Valor invalido!");
				break;
			}
			break;
		case VOTACAO:
			printf("VOTACAO\n");
			printf("1- Cadastrar voto\n");
			printf("0- Finalizar votacao\n");
			scanf("%i", &comando);
			switch (comando){
			case 0:
				sort_urna_by_votos(urna);
				save_urna_to_file(urna, &fase_atual);
				break;
			case 1:
				print_chapas_for_eleitores(urna);
				printf("Digite numero do voto: ");
				scanf("%i", &comando);
				add_voto(urna, comando);
				delay(2);
				CLEAR_TERMINAL;
				break;
			default:
				PRINT_ERROR("COMANDO INVALIDO!");
				break;
			}
			break;
		case SEGUNDA_FASE:
			printf("VOTACAO");
			print_second_turn_chapas(urna);
			printf("1- Votar\n");
			printf("0- Finalizar\n");
			scanf("%i", &comando);
			switch (comando){
			case 0:
				fase_atual = TERMINADO;
				void determine_winner_from_second_turn(urna);
				break;
			case 1:
				print_second_turn_chapas(urna);
				printf("Digite o numero do partido: ");
				scanf("%d", &comando);
				add_voto(urna, comando);
				delay(1);
				CLEAR_TERMINAL;
				break;
			default:
				PRINT_ERROR("COMANDO INVALIDO!");
				break;
			}
			break;
		}
	}
	system("color");
	printf("PROGRAMA FINALIZADO!\n");
	urna = free_urna(urna);
}

