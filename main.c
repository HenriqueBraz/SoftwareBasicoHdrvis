#include <math.h>
#include <string.h>		// para usar strings

// Rotinas para acesso da OpenGL
#include "opengl.h"

// Rotinas para leitura de arquivos .hdr
#include "rgbe.h"

// Variáveis globais a serem utilizadas:

// Dimensões da imagem de entrada
int sizeX, sizeY;


// Imagem de entrada
RGBf* image;

// Imagem de saída
RGB8* image8;

// Fator de exposição inicial
float exposure = 1.0;

// Modo de exibição atual
int modo;

// Largura e altura da imagem
int width, height;

// Ponteiro para o início da imagem na memória
RGBf* image;

// Função pow mais eficiente (cerca de 7x mais rápida)
float fastpow(float a, float b);
float fastpow(float a, float b) {
     union { float f; int i; }
      u = { a };
      u.i = (int)(b * (u.i - 1065307417) + 1065307417);
      return u.f;
}

// Função principal de processamento: ela deve chamar outras funções
// quando for necessário (ex: algoritmos de tone mapping, etc)
void process()
{
    printf("Exposure: %.3f\n", exposure);
    //
    // EXEMPLO: preenche a imagem com pixels cor de laranja...
    //
    //
    // SUBSTITUA este código pelos algoritmos a serem implementados
    //
   // int pos;
  //  for(pos=0; pos<sizeX*sizeY; pos++) {
   //     image8[pos].r = (unsigned char) (255 * exposure);
   //     image8[pos].g = (unsigned char) (127 * exposure);
   //     image8[pos].b = (unsigned char) (0 * exposure);
   // }

   for(int i=0; i<width*height; i++) {
        float r = (image[i].r / (image[i].r + 0.5));
        float g = (image[i].g / (image[i].g + 0.5));
        float b = (image[i].b / (image[i].b + 0.5));

        r = (image[i].r * exposure);
        g = (image[i].g * exposure);
        b = (image[i].b * exposure);

        image8[i].r = (unsigned char) (fmin(1.0,r) * 255);
        image8[i].g = (unsigned char) (fmin(1.0,g) * 255);
        image8[i].b = (unsigned char) (fmin(1.0,b) * 255);

    }
     //2.4: Conversão para 24 bits



        //
    // NÃO ALTERAR A PARTIR DAQUI!!!!
    //
    buildTex();
}

//------------------------------------------------------------------------------
// 2.1: código para LER a imagem de entrada (lendo a imagem tree.hdr): --->OK


void leitura()
{


// Abre o arquivo
FILE* arq;
arq = fopen("tree.hdr","rb");

// Lê o header do arquivo, de onde são extraídas a largura e altura
RGBE_ReadHeader(arq, &width, &height, NULL);

// TESTE: cria uma imagem de 800x600
sizeX = width;
sizeY = height;

    printf("%d x %d\n", sizeX, sizeY);

    // Aloca imagem float
    image = (RGBf *)malloc(sizeof(RGBf) * sizeX * sizeY);

    // Aloca memória para imagem de 24 bits
    image8 = (RGB8*) malloc(sizeof(RGB8) * sizeX * sizeY);

printf("%d %d\n", width, height);
// Finalmente, lê a imagem para a memória
int result = RGBE_ReadPixels_RLE(arq, (float*)image, width, height);
if (result == RGBE_RETURN_FAILURE) {
   /// Tratamento de erro...
   printf("ERRO!\n");
}
fclose(arq);
}
//----------------------------------------------------------------------------------
//2.2: Aplicação do fator de exposição ---> OK

/*
printf("%d", comp);


    for(int i=0; i<comp; i++) {
        image[i].r = (unsigned char) (image[i].r * exposure);
        image[i].g = (unsigned char) (image[i].g * exposure);
        image[i].b = (unsigned char) (image[i].b * exposure);
    }

//-----------------------------------------------------------------------------------

 //2.3.1: Tone mapping por escala ---> ok

    for(int i=0; i<comp; i++) {
        image[i].r = (unsigned char) (image[i].r / (image[i].r + 0.5));
        image[i].g = (unsigned char) (image[i].g / (image[i].g + 0.5));
        image[i].b = (unsigned char) (image[i].b / (image[i].b + 0.5));
    }
 //------------------------------------------------------------------------------------
    //2.3.2: Tone mapping por Correção gama ---> ok

        for(int i=0; i<comp; i++) {
        image[i].r = (unsigned char) (fastpow(image[i].r,(1.0/2.0)));
        image[i].g = (unsigned char) (fastpow(image[i].g,(1.0/2.0)));
        image[i].b = (unsigned char) (fastpow(image[i].b,(1.0/2.0)));
    }

//---------------------------------------------------------------------------------------





    //
    // NÃO ALTERAR A PARTIR DAQUI!!!!
    //
    buildTex();
}
*/

int main(int argc, char** argv)
{
    if(argc==1) {
        printf("hdrvis [tree.hdr]\n");
        exit(1);
    }

    // Inicialização da janela gráfica
    init(argc,argv);




    // Siga as orientações no enunciado para:
    //
    // 1. Descobrir o tamanho da imagem (ler header)
    // 2. Ler os pixels
    //



    exposure = 1.0f; // exposição inicial

    leitura();

    // Aplica processamento inicial
    process();

    // Não retorna... a partir daqui, interação via teclado e mouse apenas, na janela gráfica
    glutMainLoop();
    return 0;
}

