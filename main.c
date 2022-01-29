#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

typedef struct
{
    char palavra[100];
    int posicionada;
    int orientacao;
    int x;
    int y;
    int comparacao;
} Palavra;

typedef struct
{
    int posicao_posicionada;
    int posicao_inserida;
} Comparacao;


Palavra* ler_palavras(int *tamanho_array)
{
    Palavra *palavras; // lista de palavras
    char palavra[100] = "";
    int count = 0;

    palavras = (Palavra*)malloc(1 * sizeof(Palavra)); // N PRECISA DO 1 *

    puts("Digite as palavras para a palavra cruzada ou pressione Enter para encerrar o cadastro \n");

    do
    {
        setbuf(stdin, NULL);
        palavra[0] = '\0'; // limpa palavra

        printf("\nPalavra %d: ", count + 1);
        fgets(palavra, sizeof(palavra), stdin);

        if(strlen(palavra) > 1)
        {
            strcpy(palavras[count].palavra, palavra); //coloca a palavra lida na lista
            count++;
            *tamanho_array = count; // incrementa o numero de palavras

            palavras = (Palavra*)realloc(palavras, count + 1 * sizeof(Palavra)); //realoca o tamanho da lista
        }

    }
    while (strlen(palavra) > 1);

    return palavras;
}


Palavra* inicializa_palavras(Palavra palavras[], int tamanho_array)
{
    for(int i=0; i<tamanho_array; i++)
    {
        for(int j=0; j<strlen(palavras[i].palavra); j++)
        {
            palavras[i].palavra[j] = toupper(palavras[i].palavra[j]);
        }
        palavras[i].posicionada = 0;
        palavras[i].orientacao = 0;
        palavras[i].x = 0;
        palavras[i].y = 0;
        palavras[i].comparacao = -1;
    }

    return palavras;
}


int define_tamanho_matriz(Palavra palavras[], int tamanho_array)
{
    int somatoria = 0;

    for(int i=0; i<tamanho_array; i++)
    {
        somatoria = somatoria + strlen(palavras[i].palavra);
    }

    return somatoria / 2;
}


Comparacao compara_palavras(char palavra_inserir[100], char palavra_posicionada[100], int ultima_posicao)
{
    Comparacao comp;

    for(int i=0; i<strlen(palavra_inserir) - 1; i++)
    {
        for(int j=0; j<strlen(palavra_posicionada) - 1; j++)
        {
            if(palavra_inserir[i] == palavra_posicionada[j] && j > ultima_posicao) //compara se a letra é igual e se já não foi usada antes
            {
                comp.posicao_inserida = i;
                comp.posicao_posicionada = j;
                return comp;
            }
        }
    }

    comp.posicao_inserida = -1; // -1 nao achou nenhuma letra igual
    comp.posicao_posicionada = -1;
    return comp;
}


void mostra_palavras_cruzadas(Palavra palavras[], int tamanho_matriz, int tamanho_array)
{
    char quadro[tamanho_matriz][tamanho_matriz];
    int count = 0;
    int orientacao = 1;

    for(int i=0; i<tamanho_matriz; i++) // inicializa o quadro
    {
        for(int j=0; j<tamanho_matriz; j++)
        {
            quadro[i][j] = ' ';
        }
    }


    do
    {

        if(count == 0) // insere primeira palavra
        {
            for(int m=0; m<strlen(palavras[count].palavra) - 1; m++)
            {
                quadro[0][m] = palavras[count].palavra[m];
            }

            palavras[count].posicionada = 1;
            orientacao = 0;
            palavras[count].orientacao = orientacao;
            palavras[count].x = 0;
            palavras[count].y = 0;
            count++;
        }

        int count_letra = 0;
        int verifica_posicoes = 1;
        int ultima_posicao = -1;

        for(int p=1; p<tamanho_array; p++) //corre por todas as palavras não posicionadas
        {

            for(int pp=0; pp<tamanho_array; pp++) //corre por todas as palavras posicionadas
            {

                ultima_posicao = -1;

                if(palavras[p].posicionada == 0 && palavras[pp].posicionada == 1) //verifica se é possivel posicionar
                {

                    Comparacao comp = compara_palavras(palavras[p].palavra, palavras[pp].palavra, ultima_posicao); // compara as duas palavras

                    if(comp.posicao_inserida != -1 && comp.posicao_posicionada != -1) //caso tenha letra igual
                    {

                        if(palavras[pp].orientacao == 0)// caso a palavra posicionada esteja na horizontal, tenta posicionar na vertical
                        {

                            if(palavras[pp].x - comp.posicao_inserida >= 0) //verifica se cabe na matriz
                            {

                                ultima_posicao = comp.posicao_posicionada; //guarda ultima posicao usada na comparacao

                                for(int l=0; l<strlen(palavras[pp].palavra); l++) //tenta posicionar tantas vezes quanto o numero de letras da palavra posicionada
                                {
                                    count_letra = 0;
                                    verifica_posicoes = 1;

                                    for(int m=0; m<strlen(palavras[p].palavra) - 1; m++) //verifica se não vai estragar outra palavra já posicionada no quadro
                                    {
                                        if(quadro[palavras[pp].y + m - comp.posicao_inserida][palavras[pp].x + comp.posicao_posicionada] != ' ' &&
                                                quadro[palavras[pp].y + m - comp.posicao_inserida][palavras[pp].x + comp.posicao_posicionada] != palavras[pp].palavra[comp.posicao_posicionada])
                                        {
                                            verifica_posicoes = 0;
                                        }
                                        count_letra++;
                                    }

                                    count_letra = 0;

                                    if(verifica_posicoes == 1 && palavras[p].posicionada == 0) //caso dê tudo certo posiciona a palavra no quadro na vertical
                                    {
                                        for(int m=0; m<strlen(palavras[p].palavra) - 1; m++)
                                        {
                                            quadro[palavras[pp].y + m - comp.posicao_inserida][palavras[pp].x + comp.posicao_posicionada] = palavras[p].palavra[m];
                                            count_letra++;
                                        }

                                        palavras[p].posicionada = 1;
                                        orientacao = 1;
                                        palavras[p].orientacao = orientacao;
                                        palavras[p].x = palavras[pp].y;
                                        palavras[p].y = palavras[pp].x + comp.posicao_posicionada;
                                    }
                                    else //caso dê algum problema tenta posicionar em outra letra
                                    {
                                        comp = compara_palavras(palavras[p].palavra, palavras[pp].palavra, ultima_posicao);
                                        ultima_posicao = comp.posicao_posicionada;
                                    }
                                }
                            }
                        }
                        else // caso a palavra posicionada esteja na vertical, tenta posicionar na horizontal
                        {

                            if(palavras[pp].y - comp.posicao_inserida >= 0) //verifica se cabe na matriz
                            {

                                ultima_posicao = comp.posicao_posicionada; //guarda ultima posicao usada na comparacao

                                for(int l=0; l<strlen(palavras[pp].palavra); l++) //tenta posicionar tantas vezes quanto o numero de letras da palavra posicionada
                                {
                                    count_letra = 0;
                                    verifica_posicoes = 1;

                                    for(int m=0; m<strlen(palavras[p].palavra) - 1; m++) //verifica se não vai estragar outra palavra já posicionada no quadro
                                    {
                                        if(quadro[palavras[pp].x + comp.posicao_posicionada ][palavras[pp].y + m - comp.posicao_inserida] != ' ' &&
                                                quadro[palavras[pp].x + comp.posicao_posicionada ][palavras[pp].y + m - comp.posicao_inserida] != palavras[pp].palavra[comp.posicao_posicionada])
                                        {
                                            verifica_posicoes = 0;
                                        }
                                        count_letra++;
                                    }

                                    count_letra = 0;

                                    if(verifica_posicoes == 1  && palavras[p].posicionada == 0) //caso dê tudo certo posiciona a palavra no quadro na horizontal
                                    {
                                        for(int m=0; m<strlen(palavras[p].palavra) - 1; m++)
                                        {
                                            quadro[palavras[pp].x + comp.posicao_posicionada ][palavras[pp].y + m - comp.posicao_inserida] = palavras[p].palavra[m];
                                            count_letra++;
                                        }

                                        palavras[p].posicionada = 1;
                                        orientacao = 0;
                                        palavras[p].orientacao = orientacao;
                                        palavras[p].x = palavras[pp].y;
                                        palavras[p].y = palavras[pp].x + comp.posicao_posicionada;
                                    }
                                    else //caso dê algum problema tenta posicionar em outra letra
                                    {
                                        comp = compara_palavras(palavras[p].palavra, palavras[pp].palavra, ultima_posicao);
                                        ultima_posicao = comp.posicao_posicionada;
                                    }
                                }

                            }
                        }

                    }

                }

            }

        }

        count++;
    }
    while(count < tamanho_array * 2);

    puts("\n\n\nResultado!\n\n\n");
    for(int i=0; i<tamanho_matriz; i++) //exibe o quadro
    {
        for(int j=0; j<tamanho_matriz; j++)
        {
            printf("%c", quadro[i][j]);
        }
        printf("\n");
    }
}


int main()
{
    Palavra *palavras;
    int tamanho_array = 0;

    puts("Caca Palavras!\n\n");

    palavras = ler_palavras(&tamanho_array);

    palavras = inicializa_palavras(palavras, tamanho_array);

    mostra_palavras_cruzadas(palavras, define_tamanho_matriz(palavras, tamanho_array), tamanho_array);

    return 0;
}
