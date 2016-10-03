#include <stdint-gcc.h>
#define xTorre 8
#define yTorre 8
#define zTorre 16
#define numPecas 10
#define numMCP 3
#define boolean uint8_t
//**********************************************************************************variaveis globais***************************************************************************************************************************************************************************

char contador_pisos=0;//sinaliza em que piso esta [1, 6]
char contador_ciclos=0;//250 para descer peça 1 andar
char pontuacao_atual=0;//contar pontuacao obtida cada ronda
char blocoEscolhido=0; //para identificar o bloco sorteado a ser desenhado

boolean fundo_torre[xTorre][yTorre][zTorre]= {0}; //vetor que guarda informação sobre que leds acessos/apagados fundo
boolean ativo_torre[xTorre][yTorre][zTorre]= {0}; ////vetor que guarda informação sobre que leds acessos ativos(peca em queda)
boolean comecar=true;//caso fazer novo jogo
boolean flag=false;//para executar a tarefa dentro do loop
boolean in_game=false;//esta um jogo a decorrer
boolean saida=false;//sinaliza o sair do jogo atual



//estrutura para pecas
typedef struct
{
    //boolean pontoCentral[3][3]; // igual á posicao no vetor
    boolean local[3][3]; // 9 posicoes
    int rotacao_atual; //nao esquecer ao criar instancias inicializar isto a 0
} bloco;

//vetor para N pecas diferentes
bloco vetorBlocos[numPecas]; //vetor com as 10 pecas diferentes

void ISR(void)
{

    flag=true;
    contador_pisos=contador_pisos+1;
    contador_ciclos++;
    if(contador_ciclos>=250)
    {
        contador_ciclos=0;
    }
    if(contador_pisos>11)  //caso saia dos shiftRegister inserir dado para ter clock
    {
        contador_pisos=0;
    }

}

void resetArrays()
{

    char i=0;
    char j=0;
    char k=0;

    for(i=0; i<xTorre; i++)
    {
        for(j=0; j<yTorre; j++)
        {
            for(k=0; k<zTorre; k++)
            {
                fundo_torre[i][j][k]=0;
                ativo_torre[i][j][k]=0;
            }
        }
    }

    //dadosEnviar();
}

void newGame()
{

    //apaga todos os led's

    //mete a pontuacao a zero
    pontuacao_atual=0;

    //limpa os vetores
    resetArrays();
}

void MoveDown() //verfica se pode descer um piso (se nao puder passa ativo para o fundo)
{
    char i,j,k,a,b;
    for(k=5; k>=0; k--)
    {
        for(j=5; j>=0; j--)
        {
            for(i=5; i>=0; i--)
            {
                if(fundo_torre[i][j][k]) //se background estiver ativo verifica se tem algum bloco ativo no nivel acima
                {
                    if(ativo_torre[i][j][k+1]) //se o nivel acima está ativo então nao pode descer
                    {
                        for(a=0; a<6; a++)
                        {
                            for(b=0; b<6; b++)
                            {
                                if(ativo_torre[a][b][k+1])
                                {
                                    fundo_torre[a][b][k+1]=ativo_torre[a][b][k+1];//bloco ativo passa a fundo
                                    ativo_torre[a][b][k+1]=0;
                                }
                            }
                        } //depois de meter no fundo retorna
                        //PEDIR NOVO BLOCO
                        return;
                    }
                }
                else if(k==0) //ativo chega ao ultimo piso
                {
                    for(a=0; a<6; a++)
                    {
                        for(b=0; b<6; b++)
                        {
                            if(ativo_torre[a][b][k+1])
                            {
                                fundo_torre[a][b][k]=ativo_torre[a][b][k];//bloco ativo passa a fundo
                                ativo_torre[a][b][k]=0;
                            }
                        }
                    } //depois de meter no fundo retorna
                    //PEDIR NOVO BLOCO
                    return;
                }
            }
        }
    }
    //se não passar a fundo desce um piso
    for(i=1; i<6; i++)
    {
        for(j=1; i<6; i++)
        {
            for(j=1; i<6; i++)
            {
                ativo_torre[i][j][k-1]=ativo_torre[i][j][k];
            }
        }
    }
    return;
}

void refreshTorre(char andar)
{

    char i;
    char j;
    char k;
    char s=0;
    boolean flag=0;
    uint8_t auxiliar[numMCP*2]; //byte a enviar para acender leds


        MoveDown();
        for(j=0; j<yTorre; j++) //percorre vetores para ver se muda alguma coisa
        {
            auxiliar[j]=0x00; //coluna toda apagada
            for(i=0; i<xTorre; i++)
            {
                if(fundo_torre[i][j][andar] || ativo_torre[i][j][andar])  //OR, pois basta apenas um em ambas ligado
                {
                    flag=true;
                }
                if(flag)
                {
                    flag=false;
                    if(j%2==0)
                    {
                        if(i==0)
                            auxiliar[j] |=0x01;
                        else if(i==1)
                            auxiliar[j] |=0x02;
                        else if(i==2)
                            auxiliar[j] |=0x04;
                        else if(i==3)
                            auxiliar[j] |=0x08;
                        else if(i==4)
                            auxiliar[j] |=0x10;
                        else if(i==5)
                            auxiliar[j] |=0x20;
                    }
                    if(j%2!=0)
                    {
                        if(i==0)
                            auxiliar[j] |=0x04;
                        else if(i==1)
                            auxiliar[j] |=0x08;
                        else if(i==2)
                            auxiliar[j] |=0x10;
                        else if(i==3)
                            auxiliar[j] |=0x20;
                        else if(i==4)
                            auxiliar[j] |=0x40;
                        else if(i==5)
                            auxiliar[j] |=0x80;
                    }
                }
            }


        //para negar bits pois logica negada transistores
    }
}

uint8_t getcolor(int x,int y,int z)
{
    return ativo_torre[x][y][z];
}

void loop()
    {

        if(comecar)
        {
            newGame();
            comecar=false;
            in_game=true;
        }

        if(in_game)
        {

                    refreshTorre(contador_pisos);
                    flag=false;
                if(saida)
                    return;
            }


    }


