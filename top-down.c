//TODO

#include <stdio.h>
#include <stdlib.h>
int glob_capacidade = 5;
int glob_quantidade_itens = 4;
char glob_imprime = 0;


void Imprime_Tabela_Final(int **m) {
	int i = 0, j = 0;
	
	printf("\n");

	for (i = 0; i < glob_capacidade + 1; i++) 
		printf("%5d ", i);

	printf("\n\n");

	for (i = 0; i < glob_quantidade_itens + 1; i++) {

		for (j = 0; j < glob_capacidade + 1; j++) {
			printf("%5d ", m[i][j]);
		}

		printf("\n");
	}
}

void Imprime_Tabela(int **m) {
	int i = 0, j = 0;
	
	if (glob_imprime) {
		printf("\n");

		for (i = 0; i < glob_capacidade + 1; i++) 
			printf("%5d ", i);

		printf("\n\n");

		for (i = 0; i < glob_quantidade_itens + 1; i++) {

			for (j = 0; j < glob_capacidade + 1; j++) {
				printf("%5d ", m[i][j]);
			}

			printf("\n");
		}

		printf("\n");
	}
}


void Le_Arquivo(char * diretorio, int ** peso, int ** valor, int *** m){
	FILE * arq;
	int i = 0, j = 0;
	
	arq = fopen(diretorio, "r");

	if (arq) {
		
		fscanf(arq, "%d", &glob_quantidade_itens);
		
		*peso  = calloc(glob_quantidade_itens + 1, sizeof(int));
		*valor = calloc(glob_quantidade_itens + 1, sizeof(int));
		
		fscanf(arq, "%d", &glob_capacidade);
		
		
		*m = calloc(glob_quantidade_itens + 1, sizeof(int*));
		
		for (i = 0; i < glob_quantidade_itens + 1; i++) {
			(*m)[i] = calloc(glob_capacidade + 1, sizeof(int));
		}
		
		
		if (glob_capacidade > 0 && glob_quantidade_itens > 0) {
			
			for (i = 0; i < glob_quantidade_itens; i++) {
				fscanf(arq, "%d", &(*peso)[i]);
			}
			
			for (i = 0; i < glob_quantidade_itens; i++) {
				fscanf(arq, "%d", &(*valor)[i]);
			}
			
			fclose(arq);
			
		} else {
			fclose(arq);
			
			exit(-2);
		}
		
	} else {
		exit(-1);
	}
}


void Desaloca_Vetor(int **v) {
	if (*v)
		free(*v);
}

void Desaloca_Matriz(int ***m) {
	int i = 0;
	
	if (*m) {
		for (i = 0; i < glob_quantidade_itens + 1; i++)
			free((*m)[i]);
		
		free(*m);
	}
}


int Mochila_Top_Down(int itens, int capacidade, int *peso, int *valor, int ***M){
	int ** M_local = * M, lucro_maximo = 0, espaco_sobrando_local = 0, 
			lucro_item_anterior = 0, lucro_item_atual = 0;
	
	// Caso a árvore chegue na raiz
	if (capacidade < 1 || itens < 1)
		return 0;
	
	// Caso já tenha o valor armazenado
	if (M_local[itens][capacidade] != 0)
		return M_local[itens][capacidade];
	
	//Mochila_Top_Down(itens, capacidade - 1, peso, valor, M);
	//Mochila_Top_Down(itens - 1, capacidade, peso, valor, M);
	
	
	// Verifica se o peso atual é suportado pela mochila
	if (peso[itens] > capacidade) {

		if (glob_imprime)
			printf("[Mochila cheia. Pega anterior]   %d = %d\n", M_local[itens][capacidade], M_local[itens - 1][capacidade]);

		M_local[itens][capacidade] = Mochila_Top_Down(itens - 1, capacidade, peso, valor, M);

	} else {

		// Se sim verifica se ele é melhor que algum encontrado
		if (glob_imprime)
			printf("[Verifica melhor solução atual]  %d > %d + %d\n", M_local[itens - 1][capacidade], 
					M_local[itens - 1][capacidade - peso[itens]], valor[itens]);

		lucro_item_anterior = Mochila_Top_Down(itens - 1, capacidade, peso, valor, M);
		lucro_item_atual    = Mochila_Top_Down(itens - 1, capacidade - peso[itens], peso, valor, M) + valor[itens];
		
		if (lucro_item_anterior > lucro_item_atual) {
			if (glob_imprime)
				printf("[Melhor solução é a anterior]    %d = %d\n", M_local[itens][capacidade], M_local[itens - 1][capacidade]);
			
			M_local[itens][capacidade] = M_local[itens - 1][capacidade];

		} else {
			if (glob_imprime)	
				printf("[Melhor solução é a encontrada]  %d = %d = %d + %d\n", M_local[itens][capacidade], 
						M_local[itens - 1][capacidade - peso[itens]] + valor[itens],  
						M_local[itens - 1][capacidade - peso[itens]], valor[itens]);
			
			M_local[itens][capacidade] = M_local[itens - 1][capacidade - peso[itens]] + valor[itens];
		}
	}
	
	return M_local[itens][capacidade];
	
}


int main(int argc, char** argv) {
	int *peso, *valor, **M;
	
	//Le_Arquivo("usp.txt", &peso, &valor, &M);
	//Le_Arquivo("knp_025_027.txt", &peso, &valor, &M);
	Le_Arquivo("knp_400_458.txt", &peso, &valor, &M);
	
	if (glob_imprime)
		printf("[Inicialização]                  Variáveis iniciadas com sucesso.");
	
	Imprime_Tabela(M);
	
	Mochila_Top_Down(glob_quantidade_itens, glob_capacidade, peso, valor, &M);
	
	
	Imprime_Tabela_Final(M);
	
	Desaloca_Vetor(&peso);
	Desaloca_Vetor(&valor);
	
	Desaloca_Matriz(&M);
	
	return (EXIT_SUCCESS);
}