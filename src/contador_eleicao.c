#include "contador_eleicao.h"

typedef struct tm tm;

typedef struct chapa {
	uint id;
	uint votos;
	char nome_candidato[NAME_SIZE];
	char nome_vice[NAME_SIZE];
	tm data_nasc;
} Chapa;

struct urna {
	Chapa *chapa_vetor;
	uint qtd_max;
	uint qtd_atual;
	uint votos_brancos;
	uint votos_nulos;
	uint votos_totais;
};

Urna *new_urna(uint vetor_size) {
	Urna *urna = (Urna*)malloc(sizeof(Urna));
	if (urna == NULL) {
		PRINT_ERROR_DEBUG("INCAPAZ DE ALOCAR PARA URNA");
		exit(EXIT_FAILURE);
	}
	urna->qtd_atual = 0;
	urna->qtd_max = vetor_size;
	urna->votos_brancos = 0;
	urna->votos_nulos = 0;
	urna->votos_totais = 0;
	urna->chapa_vetor = malloc(sizeof(Chapa) * vetor_size);
	if(urna->chapa_vetor == NULL){
		PRINT_ERROR_DEBUG("INCAPAZ DE ALOCAR VETOR DE CHAPAS");
		exit(EXIT_FAILURE);
	}
	return urna;
}

void add_chapa(Urna *urna){
	char nome_tmp[NAME_SIZE+1];
	uint new_line_pos, tmp_id = 0;
	tm tmp_date = {0};
	if(urna->qtd_atual == urna->qtd_max) {
		PRINT_ERROR_DEBUG("NAO E POSSIVEL MAIS INSERIR CHAPAS");
		return;
	}
	while(tmp_id < 1 || tmp_id > 99){
		printf("Digite o n. do partido: ");
		scanf("%u", &tmp_id);
	}
	urna->chapa_vetor[urna->qtd_atual].votos = 0;
	urna->chapa_vetor[urna->qtd_atual].id = tmp_id;
	printf("Digite o nome do prefeito: ");
	setbuf(stdin, NULL);
	fgets(nome_tmp, NAME_SIZE+1, stdin);
	for (new_line_pos = 0; nome_tmp[new_line_pos] != '\n'; new_line_pos++);
	nome_tmp[new_line_pos] = '\0';
	strcpy(urna->chapa_vetor[urna->qtd_atual].nome_candidato, nome_tmp);
	printf("Digite o nome do vice: ");
	setbuf(stdin, NULL);
	fgets(nome_tmp, NAME_SIZE+1, stdin);
	for (new_line_pos = 0; nome_tmp[new_line_pos] != '\n'; new_line_pos++);
	nome_tmp[new_line_pos] = '\0';
	strcpy(urna->chapa_vetor[urna->qtd_atual].nome_vice, nome_tmp);
	printf("Digite o dia de nascimento do candidato: ");
	scanf("%ld", &urna->chapa_vetor[urna->qtd_atual].data_nasc.tm_mday);
	printf("Digite o mes de nascimento do candidato: ");
	scanf("%ld", &urna->chapa_vetor[urna->qtd_atual].data_nasc.tm_mon);
	printf("Digite o ano de nascimento do candidato: ");
	scanf("%ld", &urna->chapa_vetor[urna->qtd_atual].data_nasc.tm_year);
	urna->qtd_atual++;
}

void add_voto(Urna *urna, uint voto, FaseUrna fase_atual) {
	CLEAR_TERMINAL;
	urna->votos_totais++;
	if(voto == 0){
		urna->votos_brancos++;
		return;
	}

	if (fase_atual == SEGUNDO_TURNO) {
		for (uint i = 0; i < 2; i++) {
			if (urna->chapa_vetor[i].id == voto) {
				urna->chapa_vetor[i].votos++;
				printf("\033[3mSeu voto em %s com Vice %s foi confirmado!\n\033[23m", urna->chapa_vetor[i].nome_candidato, urna->chapa_vetor[i].nome_vice);
				delay(2);
				CLEAR_TERMINAL;
				return;
			}
		}
	} else {
		for (uint i = 0; i < urna->qtd_atual; i++) {
			if (urna->chapa_vetor[i].id == voto) {
				urna->chapa_vetor[i].votos++;
				printf("\033[3mSeu voto em %s com Vice %s foi confirmado!\n\033[23m", urna->chapa_vetor[i].nome_candidato, urna->chapa_vetor[i].nome_vice);
				delay(2);
				CLEAR_TERMINAL;
				return;
			}
		}
	}
	urna->votos_nulos++;
}

void print_chapas(Urna *urna, FaseUrna fase_atual){
	uint i;
	if (fase_atual == SEGUNDO_TURNO) {
		for (i = 0; i < 2; i++) {
			printf("---------------------------------------------\n");
			printf("N. do partido: %u\n", urna->chapa_vetor[i].id);
			printf("Nome do candidato: %s\n", urna->chapa_vetor[i].nome_candidato);
			printf("Nome do vice: %s\n", urna->chapa_vetor[i].nome_vice);
			printf("---------------------------------------------\n");
		}
		return;
	}
	for (i = 0; i < urna->qtd_atual; i++) {
		printf("---------------------------------------------\n");
		printf("N. do partido: %u\n", urna->chapa_vetor[i].id);
		printf("Nome do candidato: %s\n", urna->chapa_vetor[i].nome_candidato);
		printf("Nome do vice: %s\n", urna->chapa_vetor[i].nome_vice);
		printf("---------------------------------------------\n");
	}
}

void ordena_urna_by_voto(Urna *urna){
	Chapa tmp;
	int j;
	for (int i = 1; i < urna->qtd_atual; i++){
		tmp = urna->chapa_vetor[i];
		j = i - 1;
		while((j >=0) && (tmp.votos > urna->chapa_vetor[j].votos)) {
			urna->chapa_vetor[j+1] = urna->chapa_vetor[j];
			j--;
		}
		urna->chapa_vetor[j+1] = tmp;
	}
}

void computar_turno(Urna *urna, const char *file_path, FaseUrna *fase_atual){
	FILE *fp = fopen(file_path, "a");
	uint votos_validos = 0;
	ordena_urna_by_voto(urna);
	if(fp == NULL) {
		PRINT_ERROR_DEBUG("INCAPAZ DE ABRIR OU CRIAR ARQUIVO");
		exit(EXIT_FAILURE);
	}

	switch (*fase_atual){
	case PRIMEIRO_TURNO:
		votos_validos = urna->votos_totais - (urna->votos_brancos + urna->votos_nulos);
		fprintf(fp, "Total de votos: %u\n", urna->votos_totais);
		fprintf(fp, "Votos validos: %u\n", votos_validos);
		fprintf(fp, "Votos brancos: %u\n", urna->votos_brancos);
		fprintf(fp, "Votos nulos: %u\n", urna->votos_nulos);
		for (uint i = 0; i < urna->qtd_atual; i++) {
			fprintf(fp, "Numero do partido: %u\n", urna->chapa_vetor[i].id);
			fprintf(fp, "Nome do candidato: %s\n", urna->chapa_vetor[i].nome_candidato);
			fprintf(fp, "Nome do vice: %s\n", urna->chapa_vetor[i].nome_vice);
			fprintf(fp, "Data de nascimento do candidato: %2d/%2d/%d\n", urna->chapa_vetor[i].data_nasc.tm_mday, urna->chapa_vetor[i].data_nasc.tm_mon, urna->chapa_vetor[i].data_nasc.tm_year);
			fprintf(fp, "Porcentagem de votos (em relacao ao total): %.2f%%\n",(float) urna->chapa_vetor[i].votos/urna->votos_totais * 100);
			fprintf(fp, "Porcentagem de votos (em relacao aos votos validos): %.2f%%\n",(float) urna->chapa_vetor[i].votos/votos_validos * 100);
		}

		if(urna->qtd_atual > 1 && urna->votos_totais >= MIN_SEGUNDO_TURNO &&
			urna->chapa_vetor[0].votos <= votos_validos/2) {
			fprintf(fp, "Houve segundo turno!\n");
			urna->votos_brancos = 0;
			urna->votos_nulos = 0;
			urna->votos_totais = 0;
			urna->chapa_vetor[0].votos = 0;
			urna->chapa_vetor[1].votos = 0;
			*fase_atual = SEGUNDO_TURNO;
			break;
		}
		printf("Partido %u venceu as eleicoes!\n", urna->chapa_vetor[0].id);
		*fase_atual = TERMINADO;
		break;
	case SEGUNDO_TURNO:
		*fase_atual = TERMINADO;
		uint id_vencedor = 100;
		llong idade_diff = (llong)
			(urna->chapa_vetor[0].data_nasc.tm_mday + urna->chapa_vetor[0].data_nasc.tm_mon * 31
				+ (ANO_ATUAL - urna->chapa_vetor[0].data_nasc.tm_year) * 365) - (llong)
			(urna->chapa_vetor[1].data_nasc.tm_mday + urna->chapa_vetor[1].data_nasc.tm_mon * 31
				+ (ANO_ATUAL - urna->chapa_vetor[1].data_nasc.tm_year) * 365);

		if(urna->chapa_vetor[0].votos > urna->chapa_vetor[1].votos){
			id_vencedor = urna->chapa_vetor[0].id;
		}

		if(urna->chapa_vetor[0].votos < urna->chapa_vetor[1].votos){
			id_vencedor = urna->chapa_vetor[1].id;
		}
		
		if (id_vencedor == 100) {
			if (idade_diff > 0) {
				id_vencedor = urna->chapa_vetor[0].id;
			}
			if (idade_diff < 0) {
				id_vencedor = urna->chapa_vetor[1].id;
			}
		}

		printf("Partido %u venceu as eleicoes!\n", id_vencedor);
		break;
	default:
		break;
	}
	fclose(fp);
}

void free_urna(Urna **urna){
	free((*urna)->chapa_vetor);
	free(*urna);
	*urna = NULL;
}

void delay(time_t seconds) {
	time_t to_go = time(0) + seconds;
	while(to_go > time(0));
}