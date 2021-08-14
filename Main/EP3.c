///////////////////////////////////////////////////////////////////
// EP3 - IP - PROFESSOR FLÁVIO COUTINHO - Turma 04               //
// Aluno 1: Guilherme José da Silva Nascimento \ NUSP: 12543252  //
// Aluno 2: Lucas Panta de Moura \ NUSP: 12608650                //
///////////////////////////////////////////////////////////////////

// Bibliotecas usadas no EP3
#include <stdio.h> 
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define VALOR_MAXIMO 255 // Valor máximo a ser atribuido em um pixel para sua cor

#define BRANCO VALOR_MAXIMO // As duas cores usadas no EP3 para os testes exemplos
#define PRETO 0

typedef struct {

	int altura;	// altura da imagem (numero de linhas)
	int largura;   	// largura da imagem (numero de colunas na imagem)
	int ** matriz;  // ponteiro para a matriz que armazena o conteudo da imagem

} Imagem;

// estrutura que representa um Ponto no plano 2D.

typedef struct {

	int x; // coordenada horizontal
	int y; // coordenada vertical

} Ponto2D;


// Funcao que cria uma imagem vazia e a devolve para ser desenvolvida pelo programa
Imagem * cria_imagem(int altura, int largura){

    int i, j;

	Imagem * imagem = (Imagem *) malloc(sizeof(Imagem));

	imagem->altura = altura;
	imagem->largura = largura;

	imagem->matriz = (int**) malloc(imagem->altura * sizeof(int*));

    for(int i = 0; i < imagem->altura; i++){

        imagem->matriz[i] = (int*) malloc(imagem->largura * sizeof(int));
    }

    for (i = 0; i < imagem->altura; i++){
	    for(j = 0; j < imagem->largura; j++){

			imagem->matriz[i][j] = 0;
			
		}    
	}

	return imagem;
}
// Funcao  que libera os recursos de memoria associados a uma imagem.
void libera_imagem(Imagem * imagem){

	int lin;

	for(lin = 0; lin < imagem->altura; lin++) free(imagem->matriz[lin]); // liberacao de cada linha da matriz (cada linha é um vetor de int)

	free(imagem->matriz);	// liberacao da matriz (que é na realidade um vetor de linhas, ou um vetor de "vetor de int")
	free(imagem);		// liberacao da estrutura em si
}

void salva(Imagem * imagem, char * nomeArquivo){

	int lin, col;
	FILE * saida = fopen(nomeArquivo, "w"); // abertura do arquivo para escrita ("w" - write)

	if(saida){

		// o 'fprintf' funciona de forma igual ao 'printf', com a diferença que 
		// o conteudo eh escrito em um arquivo (associado a variavel 'saida')
	
		fprintf(saida, "P2\n%d %d\n%d\n", imagem->largura, imagem->altura, VALOR_MAXIMO);

		
		for(lin = 0; lin < imagem->altura; lin++){

			for(col = 0; col < imagem->largura; col++){

				fprintf(saida, (col == 0 ? "%d" : " %d"), imagem->matriz[lin][col]);  				
			}
			
			fprintf(saida, "\n");
		}
	}

	fclose(saida); // fechamento do arquivo
}
// Equação para construção da reta a partir de dois pontos dados, mais a matriz onde ela será desenhada e sua respectiva cor
void reta(Imagem * imagem, Ponto2D p1, Ponto2D p2, int cor){

    int p;
	Ponto2D d;

	d.x = p2.x - p1.x;
	d.y = p2.y - p1.y;

	p = (abs(d.x) >= abs(d.y)) ? 2 * abs(d.y) - abs(d.x) : 2 * abs(d.x) - abs(d.y);

	imagem->matriz[p1.y][p1.x] = cor;

	while(p1.x != p2.x || p1.y != p2.y){

		if (abs(d.x) >= abs(d.y))
			d.x > 0 ? p1.x++ : p1.x--;

		else if (d.x != 0){
			if (p >= 0){
				p -= 2 * abs(d.y);
				d.x > 0 ? p1.x++ : p1.x--;
			}
			p += 2 * abs(d.x);
		}

		if (abs(d.y) >= abs(d.x))
			d.y > 0 ? p1.y++ : p1.y--;

	
		else if (d.y != 0){
			if (p >= 0){
				p -= 2 * abs(d.x);
				d.y > 0 ? p1.y++ : p1.y--;
			}
			p += 2 * abs(d.y);
		}
		imagem->matriz[p1.y][p1.x] = cor;
	}
    
}

// Funcao que resgata a funçao reta para criar o contorno do rentangulo 
void retangulo_contorno(Imagem * imagem, Ponto2D p1, Ponto2D p2, int cor){
	Ponto2D p3, p4;
	p3.x = p2.x; p3.y = p1.y;
	p4.x = p1.x; p4.y = p2.y;

	// Execucao das 4 retas de contorno com base nos 4 pontos
	reta (imagem,p1,p3,cor);
	reta (imagem,p3,p2,cor);
	reta (imagem,p2,p4,cor);
	reta (imagem,p4,p1,cor);
	
}
//Funcao que realiza uma operação semelhante a funcao anterior
//entretanto o resgate da funcao reta se encontra em "loops" par
//para que assim o retangulo fique preenchido
void retangulo_preenchido(Imagem * imagem, Ponto2D p1, Ponto2D p2, int cor){
	
	Ponto2D p3;
	p3.y = p1.y;
	p3.x = p2.x;

	if(p1.x < p2.x){
		if(p1.y > p2.y) {
			for (; p1.y > p2.y; p1.y--){
				reta(imagem, p1, p3, cor);
				p3.y = p3.y - 1;
			}
		} else {
			for (; p1.y < p2.y; p1.y++){
				reta(imagem, p1, p3, cor);
				p3.y = p3.y + 1;
			}
		}
	} else {
		if(p1.y < p2.y){
			for (; p1.y < p2.y; p1.y++){
				reta(imagem, p1, p3, cor);
				p3.y = p3.y + 1;
			}
		} else{
			for (; p1.y > p2.y; p1.y--){
				reta(imagem, p1, p3, cor);
				p3.y = p3.y - 1;
			}
		}
	}

}

// copia o conteudo da area compreendida no retangulo delimitado pelos cantos opostos 'p1' e 'p2' para uma região destino 
// na imagem. Para esta funcao, voce pode assumir que 'p1' representa o canto superior esquerdo da area a ser copiada, 
// 'p2' o canto inferior direito desta mesma area, e 'p3' o canto superior esquerdo da regiao que receberá a copia.
void clona(Imagem * imagem, Ponto2D p1, Ponto2D p2, Ponto2D p3){
    
	int lar, alt, i, j;

	lar = abs(p2.x - p1.x);
	
	alt = abs(p2.y - p1.y);

    for (int i = 0; i <=alt ; i++){
	    for(int j = 0; j <=lar; j++){
            if (p1.x <= p2.x && p1.y <= p2.y){
			    imagem->matriz[p3.y + i][p3.x + j] = imagem->matriz[p1.y + i][p1.x + j];
			}
			else{
				imagem->matriz[p3.y + i][p3.x + j] = imagem->matriz[p2.y + i][p2.x + j];
			}
		}    
	}
}

// similar a funcao acima, mas invertendo o valor dos pixels copiados para a região destino. Isto é, pixels brancos devem
// se tornar pretos, pixels pretos se tornar brancos, e pixels em tons de cinza devem assumir o valor complementar (aquele
// que somado ao valor original resulta no valor definido na constante VALOR_MAXIMO).

void clona_inverte_cor(Imagem * imagem, Ponto2D p1, Ponto2D p2, Ponto2D p3){

	int lar, alt, i, j;

	lar = abs(p2.x - p1.x);
	
	alt = abs(p2.y - p1.y);

    for (i = 0; i <=alt - 1; i++){
	    for(j = 0; j <=lar - 1; j++){
            if (p1.x <= p2.x && p1.y <= p2.y){
			    imagem->matriz[p3.y + i][p3.x + j] = abs(imagem->matriz[p1.y + i][p1.x + j] - 255);
			}
			else{
				imagem->matriz[p3.y + i][p3.x + j] = abs(imagem->matriz[p2.y + i][p2.x + j] - 255);
			}
		}    
	}
}
// similar a funcao 'clona', mas espelhando horizontalmente a região copiada.

void clona_espelha_horizontal(Imagem * imagem, Ponto2D p1, Ponto2D p2, Ponto2D p3){
    
	int lar, alt, i, j;

	lar = abs(p2.x - p1.x);
	
	alt = abs(p2.y - p1.y);

	
    for (i = 0; i <=alt - 1; i++){
	    for(j = 0;j <=lar - 1; j++){
            if (p1.x <= p2.x && p1.y <= p2.y){
			    imagem->matriz[p3.y + i][p3.x + j] = imagem->matriz[p1.y + i][p2.x - j];
			}
			else{
				imagem->matriz[p3.y + i][p3.x + j] = imagem->matriz[p2.y + i][p1.x - j];
			}
		}    
	}
	
}

// similar a funcao 'clona', mas espelhando verticalmente a região copiada.

void clona_espelha_vertical(Imagem * imagem, Ponto2D p1, Ponto2D p2, Ponto2D p3){

	int lar, alt, i, j;
 
	lar = abs(p2.x - p1.x);
	
	alt = abs(p2.y - p1.y);

	
    for (i = 0; i <=alt - 1; i++){
	    for(j = 0; j <=lar - 1; j++){
            if (p1.x <= p2.x && p1.y <= p2.y){
			    imagem->matriz[p3.y + i][p3.x + j] = imagem->matriz[p2.y - i][p1.x + j];
			}
			else{
				imagem->matriz[p3.y + i][p3.x + j] = imagem->matriz[p1.y - i][p2.x + j];
			}
		}    
	}
	
}


// Funcao main que resgata todas as funcoes já desenvolvidas para 
//a criação da imagem de acordo com os valores digitados
int main(){
	
	int largura, altura, cor, i , j; // variaveis necessárias para o cálculo das funcoes
	char operacao[32], nome_arquivo[256];
	Ponto2D p1, p2, p3; // 3 Pontos (x,y)
	Imagem * img; // matriz tipo Imagem 

	scanf("%s %d %d", nome_arquivo, &largura, &altura);
	img = cria_imagem(altura, largura); // Criando imagem vazia a ser trabalhada

	
	while(scanf("%s", operacao) == 1 && strcmp(operacao, "FIM") != 0){

		if(!scanf("%d %d %d %d", &p1.x, &p1.y, &p2.x, &p2.y) || p1.x < 0 || p1.x >= largura || p1.y < 0 || p1.y >= altura || p2.x < 0 || p2.x >= largura || p2.y < 0 || p2.y >= largura){
			printf("Coordenada inserida invalida %s\n", operacao); // Verifica validade do conteúdo digitado
			return 1;
		}
		
		if(strstr(operacao, "CLONA")){
			if(!scanf("%d %d", &p3.x, &p3.y)
			|| p3.x < 0 || p3.x >= largura || p3.y < 0 || p3.y >= altura){
				printf("Coordenada inserida invalida %s\n", operacao);// Verifica validade do conteúdo digitado
				return 1;
			}
		}
		else if (!scanf("%d", &cor) || cor < PRETO || cor > BRANCO){
			printf("Cor inserida invalida %s\n", operacao); // Verifica validade do conteúdo digitado
			return 1;
		}
		
		if (!strcmp(operacao, "RETA")){
			reta (img,p1,p2,cor); // Resgate da funcao reta
		}

		else if (!strcmp(operacao, "RETANGULO_CONTORNO"))
			retangulo_contorno (img,p1,p2,cor); // Resgate da funcao retangulo contorno

		else if (!strcmp(operacao, "RETANGULO_PREENCHIDO"))
			retangulo_preenchido (img,p1,p2,cor); // Resgate da funcao retangulo preenchido

		else if (!strcmp(operacao, "CLONA"))
			clona (img,p1,p2,p3); // Resgate da funcao clona

		else if (!strcmp(operacao, "CLONA_INV"))
			clona_inverte_cor (img,p1,p2,p3); // Resgate da funcao clona inverte cor

		else if (!strcmp(operacao, "CLONA_HOR"))
			clona_espelha_horizontal (img ,p1,p2,p3); // Resgate da funcao clona espelha horizontal

		else if (!strcmp(operacao, "CLONA_VER"))
			clona_espelha_vertical (img ,p1,p2,p3); // Resgate da funcao clona espelha horizontal

		else{
			printf("Operacao \"%s\" mal sucedida, tente novamente", operacao); 
			return 1; //finaliza o programa em caso de erro
		}
		
	}

    salva(img, nome_arquivo); // Salva a imagem criada
	libera_imagem(img); // libera o arquivo .pgm
	
	return 0;	//finaliza o programa sem erros

	
}


