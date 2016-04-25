#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include <time.h>
#include <unistd.h>
typedef struct demanda
{
	struct demanda *ant, *prox;
	int periodo;
	int quantidade;
}Demanda;
typedef struct produto
{
	struct produto *ant, *prox;
	int cod_item;
	char desc[100];
	int uni_ex;
	Demanda *demanda;
	int media_demanda;
	float desvio,em;
}Produto;
typedef struct controle
{
	int tamanho;
	Produto *ini, *fim;	
}Controle;

FILE *abrir_arquivo(char arquivo[]);
void fechar_arquivo(FILE *arq);
int menu();
void percorrer_printar(Controle *controle);
void insere_demanda (Produto *produto, int periodo, int quantidade);
void adicionar_produto(Controle *controle, FILE *arq);
void ler(Controle *controle, FILE *arq);
void consultar_estoque(Controle *controle);
void relatorio_gerencial(Controle *controle);
void printar_relatorio(Controle *controle);
void consultar_produto(Controle *controle);