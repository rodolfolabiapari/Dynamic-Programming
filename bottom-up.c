/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.c
 * Author: pripyat
 *
 * Created on 18 de Junho de 2016, 17:22
 */

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


void Mochila_Bottom_Up(int *peso, int *valor, int ***M){
	int ** M_local = *M;
	int capacidade_local = 0, itens_local = 0; 
	
	// percorre todas as capacidades
	for (capacidade_local = 1; capacidade_local <= glob_capacidade; capacidade_local++) {
		// para cada capacidade, percorre os itens
		for (itens_local = 1; itens_local <= glob_quantidade_itens; itens_local++) {
			
			if (glob_imprime)
				printf("[Verificação de capacidade]      Peso atual do item %d > %d Capacidade atual Mochila\n", peso[itens_local], capacidade_local);
			
			// Verifica se o peso atual é suportado pela mochila
			if (peso[itens_local] > capacidade_local) {
				
				if (glob_imprime)
					printf("[Mochila cheia. Pega anterior]   %d = %d\n", M_local[itens_local][capacidade_local], M_local[itens_local - 1][capacidade_local]);
				M_local[itens_local][capacidade_local] = M_local[itens_local - 1][capacidade_local];
				
			} else {
				
				// Se sim verifica se ele é melhor que algum encontrado
				if (glob_imprime)
					printf("[Verifica melhor solução atual]  %d > %d + %d\n", M_local[itens_local - 1][capacidade_local], M_local[itens_local - 1][capacidade_local - peso[itens_local]], valor[itens_local]);

				if (M_local[itens_local - 1][capacidade_local]
						>
						(M_local[itens_local - 1][capacidade_local - peso[itens_local]] + valor[itens_local])) {
					if (glob_imprime)
						printf("[Melhor solução é a anterior]    %d = %d\n", M_local[itens_local][capacidade_local], M_local[itens_local - 1][capacidade_local]);
					M_local[itens_local][capacidade_local] = M_local[itens_local - 1][capacidade_local];

				} else {
					if (glob_imprime)	
						printf("[Melhor solução é a encontrada]  %d = %d = %d + %d\n", M_local[itens_local][capacidade_local], M_local[itens_local - 1][capacidade_local - peso[itens_local]] + valor[itens_local],  M_local[itens_local - 1][capacidade_local - peso[itens_local]], valor[itens_local]);
					M_local[itens_local][capacidade_local] = M_local[itens_local - 1][capacidade_local - peso[itens_local]] + valor[itens_local];
				}
			}
			
			Imprime_Tabela(M_local);
		}
	}
}


int main(int argc, char** argv) {
	int *peso, *valor, **M;
	
	Le_Arquivo("usp.txt", &peso, &valor, &M);
	//Le_Arquivo("knp_025_027.txt", &peso, &valor, &M);
	//Le_Arquivo("knp_400_458.txt", &peso, &valor, &M);
	
	if (glob_imprime)
		printf("[Inicialização]                  Variáveis iniciadas com sucesso.");
	
	Imprime_Tabela(M);
	
	Mochila_Bottom_Up(peso, valor, &M);
	
	Imprime_Tabela_Final(M);
	
	Desaloca_Vetor(&peso);
	Desaloca_Vetor(&valor);
	
	Desaloca_Matriz(&M);
	
	return (EXIT_SUCCESS);
}

