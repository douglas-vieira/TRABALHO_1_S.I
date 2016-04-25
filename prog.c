#include "prog.h"

int main(){
	FILE *arq=abrir_arquivo("BD.txt");
	int aux=0;
	Controle *header = (Controle *) calloc(1,sizeof (Controle));
	ler(header, arq);
	do
	{
		aux=menu();
		switch (aux)
		{
			case 1:
				relatorio_gerencial(header);
			break;
			
			case 2:
				adicionar_produto(header,arq);
			break;
			
			case 3:
				consultar_produto(header);
			break;

			case 4:
				consultar_estoque(header);
			break;
		}
	}while(aux != 5);
	fechar_arquivo(arq);
	return 0;
}
FILE *abrir_arquivo(char arquivo[]){
	FILE *arq =fopen(arquivo, "a+");
	if(arq!=NULL)
	{
		return (arq);
	}
	else
	{
		printf("Arquivo nao encontrado.");
		exit(1);
	}
}
void fechar_arquivo(FILE *arq){
	
	fclose(arq);
}
int menu(){
	int saida;
	system("clear || cls");
	printf("               ----->  MENU <-----\n");
	printf("+-------------------------------------------------+\n");
	printf("|1 |- Impressao online de relatorio gerencial.    |\n");
	printf("+-------------------------------------------------+\n");
	printf("|2 |- Adicionar produto ao estoque.               |\n");
	printf("+-------------------------------------------------+\n");
	printf("|3 |- Consultar online quantidade de UM produto.  |\n");
	printf("+-------------------------------------------------+\n");
	printf("|4 |- Consulta online geral do estoque.           |\n");
	printf("+-------------------------------------------------+\n");
	printf("|5 |- sair do programa.                           |\n");
	printf("+-------------------------------------------------+\n");
	printf("Digite a opcao desejada:");
	scanf("%d",&saida);
	if (saida <1 || saida >6)
	{
		printf("Opcao inválida, saindo do programa.\n");
		exit (1);
	}
	printf("\n\n");
	return saida;
}
void percorrer_printar(Controle *controle){
	for (Produto *ptr1=controle->ini; ptr1!=NULL; ptr1=ptr1->prox)
	{
		printf("Codigo produto: %d\n",ptr1->cod_item);
		printf("Produto: %s\n",ptr1->desc );
		printf("Unidades disponiveis: %d\n",ptr1->uni_ex );
		for (Demanda *ptr2=ptr1->demanda; ptr2 != NULL; ptr2=ptr2->prox)
		{
			printf("Periodo : ");
			printf("%d | ",ptr2->periodo );
			printf("Demanda:");
			printf("%d\n",ptr2->quantidade );
		}
		printf("\n\n");
	}
}
void insere_demanda (Produto *produto, int periodo, int quantidade){
	Demanda *dem = (Demanda *) calloc(1,sizeof (Demanda));
	dem->periodo = periodo;
	dem->quantidade = quantidade;
	if ((produto)->demanda == NULL)
	{
		(produto)->demanda = dem;
	}
	else
	{
		Demanda *dem_aux;
		dem_aux = (produto)->demanda;
		(produto)->demanda = dem;
		dem->prox = dem_aux;	
	}
}
void adicionar_produto(Controle *controle, FILE *arq){

	int cod_item;
	int uni_ex;
	char desc[100];
	memset (desc,'\0',(100*sizeof(char)));
	
	printf("Digite o código do produto: \n");
	scanf("%d",&cod_item);
	getchar();
	printf("Digite a descricao do produto: \n");
	scanf("%[^\n]s", desc);
	getchar();
	for (int i = 0; i < 100; ++i)
	{
		if (desc[i]==' ')
			desc[i] = '_';
	}
	printf("Digite a quantidade de unidades existentes: \n");
	scanf("%d",&uni_ex);
	fprintf(arq,"\n");
	fprintf(arq,"%d ",cod_item );
	fprintf(arq,"%s ",desc );
	fprintf(arq,"%d\n",uni_ex);
	int aux[3];

	Produto *no = (Produto *) calloc(1,sizeof (Produto));
	no->cod_item = cod_item;
	strcpy(no->desc,desc);
	no->uni_ex = uni_ex;
	do
	{
		printf("Digite periodo da demanda: \n");
		scanf("%d",&aux[0]);
		fprintf(arq,"%d ",aux[0]);
		printf("Digite a quantidade da demanda no periodo: \n");
		scanf("%d",&aux[1]);
		fprintf(arq,"%d ",aux[1]);
		printf("Deseja inserir outro periodo?\n");
		printf("1 - SIM\n");
		printf("0 - NAO\n");
		scanf("%d",&aux[2]);
		insere_demanda (no, aux[0], aux[1]);
	}while(aux[2]!=0);
	if(controle->ini == NULL)
		{
			controle->ini = controle->fim = no;
		}
		else
		{
			controle->fim->prox = no;
			controle->fim=no;
		}	
}
void ler(Controle *controle, FILE *arq){
	int cod_item;
	char desc[100];
	memset (desc,'\0',(100*sizeof(char)));
	int uni_ex;
	while(!feof(arq))
	{
		fscanf(arq,"%d",&cod_item);
		// printf("cod_item: %d\n",cod_item);
		fscanf(arq,"%s", desc);
		// printf("desc: %s\n",desc );
		fscanf(arq,"%d",&uni_ex);
		// printf("uni_ex: %d\n",uni_ex );
		getc(arq);
		Produto *no = (Produto *) calloc(1,sizeof (Produto));
		no->cod_item = cod_item;
		strcpy(no->desc,desc);
		no->uni_ex = uni_ex;
		
		char temp[30];
		memset (temp,'\0',(30*sizeof(char)));
		fscanf(arq,"%[^'\n']s",temp);
		// printf("temp=%s\n",temp );
		char *pch,str[30];
		int peri,quant;
		
		pch = strtok (temp," ");
		sscanf (pch,"%s",str);
		peri = atoi(str);
		pch = strtok (NULL, " ");
		sscanf (pch,"%s",str);
		quant = atoi(str);
		// printf ("Periodo: %d Quantidade: %d\n",peri,quant);
		insere_demanda(no,peri,quant);
		int flag1=0;
		while (pch != NULL)
		{
			pch = strtok (NULL, " ");
			if(pch!=NULL)
			{
				if(flag1==0)
				{
					sscanf (pch,"%s",str);
					peri = atoi(str);
					flag1=1;
				}
				else
				{
					sscanf (pch,"%s",str);
					quant = atoi(str);
					// printf ("Periodo: %d Quantidade: %d\n",peri,quant);
					insere_demanda(no,peri,quant);
					flag1=0;
				}
			}
		}		
		
		if(controle->ini == NULL)
		{
			controle->ini = controle->fim = no;
		}
		else
		{
			controle->fim->prox = no;
			controle->fim=no;
		}
	}
}
void consultar_estoque(Controle *controle){
	
	percorrer_printar (controle);
}
void consultar_produto(Controle *controle){
	int flag = 0,codigo;
	printf("Digite o Codigo do produto a ser consultado: \n");
	scanf("%d",&codigo);
	for (Produto *ptr1=controle->ini; ptr1!=NULL; ptr1=ptr1->prox)
	{
		if(ptr1->cod_item == codigo)
		{
			printf("Codigo produto: %d\n",ptr1->cod_item);
			printf("Produto: %s\n",ptr1->desc );
			printf("Unidades disponiveis: %d\n",ptr1->uni_ex );
			for (Demanda *ptr2=ptr1->demanda; ptr2 != NULL; ptr2=ptr2->prox)
			{
				printf("Periodo : ");
				printf("%d | ",ptr2->periodo );
				printf("Demanda:");
				printf("%d\n",ptr2->quantidade );
			}
			printf("\n\n");
			flag =1;
		}
		if (flag==0)
		{
			printf("Código do produto nao encontrado.\n");
		}
	}
}
void printar_relatorio(Controle *controle){
	for (Produto *ptr1=controle->ini; ptr1!=NULL; ptr1=ptr1->prox)
	{
		printf("Periodo de Referencia: %d\n",ptr1->demanda->periodo);
		printf("Codigo produto: %d\n",ptr1->cod_item);
		printf("Descricao do produto: %s\n",ptr1->desc);
		printf("Unidades Existentes: %d\n",ptr1->uni_ex );
		printf("Total de unidades no periodo de Referencia: %d\n", ptr1->demanda->periodo);
		int total=0;
		for (Demanda *ptr2=ptr1->demanda; ptr2 != NULL; ptr2=ptr2->prox)
		{
			total+=ptr2->quantidade;
		}
		printf("Total de unidades de todos os periodos: %d\n", total);
		printf("Estoque Minimo: %.2f\n", ptr1->em);
		printf("------------------------------------------------------\n");
	}
}
void relatorio_gerencial(Controle *controle){
	
	printf("\t\t\t+-------------------------------------+\n");
	printf("\t\t\t|  DATA:%s  HORA:%s    |\n",__DATE__,__TIME__);
	printf("\t\t\t+-------------------------------------+\n");
	int media=0, count = 0;
	for (Produto *ptr1=controle->ini; ptr1!=NULL; ptr1=ptr1->prox)
	{
		for (Demanda *ptr2=ptr1->demanda; ptr2 != NULL; ptr2=ptr2->prox)
		{
			media = (media + ptr2->quantidade);
			count++;
		}
		media=media/count;
		ptr1->media_demanda=media;
		count = 0;
	}
	int  i=0;
	for (Produto *ptr1=controle->ini; ptr1!=NULL; ptr1=ptr1->prox)
	{
		int passos=0;
		for (Demanda *ptr2=ptr1->demanda; ptr2 != NULL; ptr2=ptr2->prox,i++)
		{
			passos=(passos + ((ptr2->quantidade - ptr1->media_demanda)*(ptr2->quantidade - ptr1->media_demanda)));
		}
		ptr1->desvio=passos; 
		passos=0;
		ptr1->desvio=sqrt(ptr1->desvio/(i-1));
		i =0;
	}
	for (Produto *ptr1=controle->ini; ptr1!=NULL; ptr1=ptr1->prox)
	{
		ptr1->em = (1.6450*ptr1->desvio)*sqrt(2/1);
		// int i=0;
		// for (Demanda *ptr2=ptr1->demanda; ptr2 != NULL; ptr2=ptr2->prox,i++)
		// {
		// 	ptr1->desvio =ptr1->desvio + pow((ptr2->quantidade - ptr1->media_demanda),2);
		// }
		// ptr1->desvio=sqrt(ptr1->desvio/(i-1));
	}
	printar_relatorio(controle);
}