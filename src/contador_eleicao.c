#include "contador_eleicao.h"
#include <time.h>
#include <string.h>

struct chapa {
	id id;
	uint votos;
	char nome_candidato[NAME_SIZE];
	char nome_vice[NAME_SIZE];
	struct tm data_nasc;
};

struct lista {
	Chapa chapa;
	struct lista *next;
};

struct urna {
	Lista *lista;
	uint votos_brancos;
	uint votos_nulos;
	uint votos_totais;
};

void create_chapa(Urna* urna){
	char tmp_string[NAME_SIZE+1];
	uint new_line_pos;
	int mes;
	int ano;
	Lista* to_add = (Lista*)malloc(sizeof(Lista));
	if(to_add == NULL) {
		PRINT_ERROR_DEBUG("INCAPAZ DE ALOCAR NOVO ELEMENTO PARA LISTA!");
		exit(EXIT_FAILURE);
	}
	to_add->next = urna->lista;
	urna->lista = to_add;
	to_add->chapa.id = 0;
	to_add->chapa.votos = 0;
	while (to_add->chapa.id < 1 || to_add->chapa.id > 99) {
		printf("Digite o numero da chapa: ");
		scanf("%i", &to_add->chapa.id);
	}
	printf("Digite o nome do candidato: ");
	setbuf(stdin, NULL);
	fgets(tmp_string, NAME_SIZE+1, stdin);
	for (new_line_pos = 0; tmp_string[new_line_pos] != '\n'; new_line_pos++);
	tmp_string[new_line_pos] = '\0';
	strcpy(to_add->chapa.nome_candidato, tmp_string);
	printf("Digite o dia de nascimento do candidato: ");
	scanf("%d", &to_add->chapa.data_nasc.tm_mday);
	printf("Digite o mes de nascimento do candidato: ");
	scanf("%d", &mes);
	printf("Digite o ano de nascimento do candidato: ");
	scanf("%d", &ano);
	to_add->chapa.data_nasc.tm_mon = mes - 1;
	to_add->chapa.data_nasc.tm_year = ano - 1900;
	printf("Digite o nome do vice: ");
	setbuf(stdin, NULL);
	fgets(tmp_string, NAME_SIZE + 1, stdin);
	for (new_line_pos = 0; tmp_string[new_line_pos] != '\n'; new_line_pos++);
	tmp_string[new_line_pos] = '\0';
	strcpy(to_add->chapa.nome_vice, tmp_string);
}

Urna *new_urna(void){
	Urna *urna = (Urna *) malloc(sizeof(Urna));
	if(!urna){
		PRINT_ERROR_DEBUG("INCAPAZ DE ALOCAR PARA URNA");
		exit(EXIT_FAILURE);
	}
	urna->votos_brancos = 0;
	urna->votos_nulos = 0;
	urna->votos_totais = 0;
	urna->lista = NULL;
	return urna;
}

void save_urna_to_file(Urna* urna, FaseUrna *fase_atual){
	uint votos_validos = urna->votos_totais - (urna->votos_brancos + urna->votos_nulos);
	FILE *fp = fopen(BOLETIM_PATH, "w");
	if(!fp){
		PRINT_ERROR_DEBUG("Incapaz de abrir ouu criar aquivo");
		exit(EXIT_FAILURE);
	}
	fprintf(fp, "Votos totais: %d\n", urna->votos_totais);
	fprintf(fp, "Votods brancos: %d\n", urna->votos_brancos);
	fprintf(fp, "Votos nulos: %d\n", urna->votos_nulos);
	fprintf(fp, "Votos validos: %d\n", votos_validos);
	fprintf(fp, "Porcentagem de votos validos em relacao ao total: %2.2f%%\n", (float) 100*votos_validos/urna->votos_totais);
	for(Lista *tmp = urna->lista; tmp != NULL; tmp = tmp->next){
		fprintf(fp, "N. do partido: %uc\n", tmp->chapa.id);
		fprintf(fp, "Votos: %d\n", tmp->chapa.votos);
		fprintf(fp, "Porcentagem em relacao ao total: %2.2f%%\n", (float) 100*tmp->chapa.votos/urna->votos_totais);
		fprintf(fp, "Porcentagem em relacao aos validos: %2.2f%%\n", (float) 100 *tmp->chapa.votos/votos_validos);
		fprintf(fp, "Nome do candidato: %s\n", tmp->chapa.nome_candidato);
		fprintf(fp, "Nome do vice: %s\n", tmp->chapa.nome_vice);
		fprintf(fp, "Data de nascimento: %d/%d/%d\n\n", tmp->chapa.data_nasc.tm_mday, tmp->chapa.data_nasc.tm_mon+1, tmp->chapa.data_nasc.tm_year+1900);
	}
	if(urna->lista){
		if(urna->lista->next != NULL && 
			(votos_validos >> 1) + 1 < (urna->lista->chapa.votos)
			&& urna->votos_totais >= MIN_SEGUNDO_TURNO){
			fprintf(fp, "HOUVE SEGUNDO TURNO!\n");
			printf("SEGUNDO TURNO IRÁ COMECAR!\n");
			fclose(fp);
			*fase_atual = SEGUNDA_FASE;
			urna->votos_brancos = 0;
			urna->votos_nulos = 0;
			urna->votos_totais = 0;
			urna->lista->chapa.votos = 0;
			urna->lista->next->chapa.votos = 0;
			return;
		}
		printf("Candidato %s com Vice %s venceu as eleicoes!\n", urna->lista->chapa.nome_candidato,
			urna->lista->chapa.nome_vice);
	}
	fclose(fp);
	*fase_atual = TERMINADO;
}

void *free_urna(Urna *urna){
	Lista *next, *to_free = urna->lista;
	while(to_free != NULL) {
		next = to_free->next;
		free(to_free);
		to_free = next;
	}
	free(urna);
	return NULL;
}

void add_voto(Urna* urna, id n_chapa) {
	urna->votos_totais++;
	if (n_chapa == 0) {
		urna->votos_brancos++;
		return;
	}

	for(Lista* lista = urna->lista; lista != NULL; lista = lista->next) {
		if(lista->chapa.id == n_chapa){
			printf("Seu voto em %s com Vice %s foi confirmado!\n", lista->chapa.nome_candidato, lista->chapa.nome_vice);
			lista->chapa.votos++;
			return;
		}
	}

	urna->votos_nulos++;
}

void sort_urna_by_votos(Urna *urna){
	Chapa *tmp_chapa, *to_permute;
	for (Lista* lista = urna->lista; lista->next != NULL; lista = lista->next) {
		tmp_chapa = &lista->chapa;
		for (Lista* to_get = lista->next; to_get != NULL; to_get = to_get->next) {
			if (tmp_chapa->votos < to_get->chapa.votos) {
				tmp_chapa = &to_get->chapa;
			}
		}
		to_permute = &lista->chapa;
		lista->chapa = *tmp_chapa;
		*tmp_chapa = *to_permute;
	}
}

void print_chapas_for_eleitores(Urna *urna){
	for(Lista *l = urna->lista;l != NULL; l = l->next){
		printf("Numero da chapa: %d\n", l->chapa.id);
		printf("Nome do candidato: %s\n", l->chapa.nome_candidato);
		printf("Nome do vice: %s\n", l->chapa.nome_vice);
	}
}

void print_second_turn_chapas(Urna* urna) {
	Lista *l = urna->lista;
	for (unsigned char c = 0; c < 2; c++) {
		printf("N. do partido: %d\n", l->chapa.id);
		printf("Nome do candidato: %s\n", l->chapa.nome_candidato);
		printf("Nome do vice: %s\n", l->chapa.nome_vice);
		l = l->next;
	}
}

void determine_winner_from_second_turn(Urna *urna){
	Lista *first_chapa = urna->lista, *second_chapa = urna->lista->next;
	char* winner_name = NULL;
	id winner_id = 0;
	size_t candidato1_idade_dias, candidato2_idade_dias;
	
	if(first_chapa->chapa.votos > second_chapa->chapa.votos){
		winner_name = first_chapa->chapa.nome_candidato;
		winner_id = first_chapa->chapa.id;
	}

	if(first_chapa->chapa.votos < second_chapa->chapa.votos) {
		winner_name = second_chapa->chapa.nome_candidato;
		winner_id = second_chapa->chapa.id;
	}

	if (winner_name == NULL) {
		candidato1_idade_dias = (llong) first_chapa->chapa.data_nasc.tm_mday +
			(first_chapa->chapa.data_nasc.tm_mon + 1) * 31
			- ((first_chapa->chapa.data_nasc.tm_year) * 365);

		candidato2_idade_dias = (llong)second_chapa->chapa.data_nasc.tm_mday +
			((llong)second_chapa->chapa.data_nasc.tm_mon + 1) * 31
			- ((llong)(second_chapa->chapa.data_nasc.tm_year) * 365);

		if (candidato1_idade_dias > candidato2_idade_dias) {
			winner_name = first_chapa->chapa.nome_candidato;
			winner_id = first_chapa->chapa.id;
		}

		if (candidato1_idade_dias < candidato2_idade_dias) {
			winner_name = second_chapa->chapa.nome_candidato;
			winner_id = second_chapa->chapa.id;
		}
	}

	printf("Candidato: %s do partido numero %d venceu!\n", winner_name, winner_id);
}

void delay(time_t seconds) {
	time_t to_go = time(0) + seconds;
	while(to_go > time(0));
}