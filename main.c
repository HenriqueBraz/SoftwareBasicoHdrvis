#include <math.h>
#include <string.h>		// para usar strings

// Rotinas para acesso da OpenGL
#include "opengl.h"

// Rotinas para leitura de arquivos .hdr
#include "rgbe.h"

// Variáveis globais a serem utilizadas:

// Dimensões da imagem de entrada
int sizeX, sizeY;

//arquivo para guardar as urls para os nomes dos arquivos
char urls[20];

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

float r = 0.0;
float g = 0.0;
float b = 0.0;

// Função pow mais eficiente (cerca de 7x mais rápida)
float fastpow(float a, float b);
float fastpow(float a, float b)
{
    union
    {
        float f;
        int i;
    }
    u = { a };
    u.i = (int)(b * (u.i - 1065307417) + 1065307417);
    return u.f;
}

// Função principal de processamento: ela deve chamar outras funções
// quando for necessário (ex: algoritmos de tone mapping, etc)
void process()
{
    printf("Exposure: %.3f\n", exposure);


    for(int i=0; i<width*height; i++)
    {



        if(modo == 0)
        {
            //Tone mapping por escala
            r = ((image[i].r / (image[i].r + 0.5))* exposure);
            g = ((image[i].g / (image[i].g + 0.5))* exposure);
            b = ((image[i].b / (image[i].b + 0.5))*exposure);

        }
        else if (modo == 1)
        {

            //Tone mapping por correção gama
            r = ((fastpow(image[i].r,(1.0/1.8)))* exposure);
            g = ((fastpow(image[i].g,(1.0/1.8)))* exposure);
            b = ((fastpow(image[i].b,(1.0/1.8)))* exposure);

        }


        //2.4: Conversão para 24 bits
        image8[i].r = (unsigned char) (fmin(1.0,r) * 255);
        image8[i].g = (unsigned char) (fmin(1.0,g) * 255);
        image8[i].b = (unsigned char) (fmin(1.0,b) * 255);

    }


    buildTex();
}

//------------------------------------------------------------------------------
// 2.1: código para LER a imagem de entrada (lendo a imagem tree.hdr)


void leitura()
{


// Abre o arquivo
    FILE* arq;
    arq = fopen(urls,"rb");

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
    if (result == RGBE_RETURN_FAILURE)
    {
        /// Tratamento de erro...
        printf("ERRO!\n");
    }
    fclose(arq);
}
int main(int argc, char** argv)
{

    //aloca memória para o ponteiro url
    char* url;
    url = (char*)malloc(20*sizeof(char));
    url = (char*)NULL;

    int flag = 0;
    int num = 0;
    printf("Escolha o arquivo HDR de imagem a ser carregado:\n\n");
    printf("1 - tree       2 - table    3 - mount\n\n");
    printf("4 - memorial   5 - fog      6 - desk\n\n");
    printf("7 - cathedral  8 - apartment\n\n");
    printf("Escolha um numero de 1 a 8:\n");


    while(flag == 0)
    {

        scanf("%d", &num);

        switch (num)
        {
        case 1:
            url = "tree.hdr";
            flag = 1;
            break;

        case 2:
            url = "table.hdr";
            flag = 1;
            break;

        case 3:
            url = "mount.hdr";
            flag = 1;
            break;

        case 4:
            url = "memorial.hdr";
            flag = 1;
            break;

        case 5:
            url = "fog.hdr";
            flag = 1;
            break;

        case 6:
            url = "desk.hdr";
            flag = 1;
            break;

        case 7:
            url = "cathedral.hdr";
            flag = 1;
            break;

        case 8:
            url = "apartment.hdr";
            flag = 1;
            break;

        default:
            printf("Opcao Invalida, digite novamente\n");


        }


    }


    if(argc==1)
    {
        printf("hdrvis [tree.hdr]\n");
        exit(1);
    }

    // Inicialização da janela gráfica
    init(argc,argv);


    exposure = 1.0f; // exposição inicial


    strcpy(urls,url); //passa o valor do ponteiro para urls


    leitura(); //chama o arquivo de elitura


    // Aplica processamento inicial
    process();

    // Não retorna... a partir daqui, interação via teclado e mouse apenas, na janela gráfica
    glutMainLoop();
    return 0;
}

