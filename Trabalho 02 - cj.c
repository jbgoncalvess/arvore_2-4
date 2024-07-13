#include <stdio.h>
#include <stdlib.h>
#define TAM 3

///CHAVE DE UM USUÁRIO POR EXEMPLO///
struct registro{
    int chave;
};


///O NÓ QUE VAI CONTER A CHAVE DE USUÁRIOS E SEUS FILHOS///
struct no{
    int num_reg;
    struct registro *dados[3];
    struct no *filhos[4];
};


///ALTERAR EXEMPLO PROF///
struct registro *buscar(struct no *arvore, int chave) {
    int i;
    while (arvore) {
        for (i = 0; i < arvore->num_reg && arvore->dados[i]->chave < chave; i++);
        if (i < arvore->num_reg && arvore->dados[i]->chave == chave)
            return arvore->dados[i];
        arvore = arvore->filhos[i];
    }
    return NULL;
}

///ALTERAR EXEMPLO PROF///
void eliminar(struct no *arvore) {
    int i;
    /* se folha */
    if (!(arvore->filhos[0])) {
        for(i = arvore->num_reg - 1; i >= 0; i--)
        free(arvore->dados[i]);
        return;
    }
    /* se nao folha */
    eliminar(arvore->filhos[arvore->num_reg]);
    for(i = arvore->num_reg - 1; i >= 0; i--) {
        free(arvore->dados[i]);
        eliminar(arvore->filhos[i]);
    }
    free(arvore);
    return;
}


///ALOCAR NÓ PARA QUANDO PRECISAR(SEMPRE NA PRIMEIRA INSERÇÃO VAI PRECISAR)
struct no *alocar_no(void) {
    struct no *arvore;
    int i;
    arvore = (struct no *) malloc (sizeof (struct no));
    for (i = 0; i < 4; i++){
        arvore->filhos[i] = NULL;
    }
    arvore->num_reg = 0;
    return arvore;
}


struct registro *alocar_registro(int chave){
    struct registro *novo;
    novo = (struct registro *) malloc(sizeof(struct registro));
    novo->chave = chave;
    return novo;
}

///PERCORRE NÓ A NÓ///
void percorrer_nos (struct no *corrente, int nivel) {
    int i;
    if (corrente) {
        printf ("\n %d -> ", nivel);
        for (i = 0; i < corrente->num_reg; i++)
            printf("%d ", corrente->dados[i]->chave);
        for (i = 0; i <= corrente->num_reg; i++)
            percorrer_nos(corrente->filhos[i], nivel+1);
    }
}

///INSERIR REGISTRO (NÃO NECESSÁRIAMENTE SERÁ ALOCADO UM NÓ)///
int inserir(struct no *arvore, struct registro **reg, struct no **sub){
    int i, j, houveOverflow;
    struct no *novo;
    struct registro *sobrou;

    //Procurar o lugar onde inserir o elemento. Se for igual não inserir//
    for (i = 0; i < arvore->num_reg && arvore->dados[i]->chave < (*reg)->chave; i++);
    if (i < arvore->num_reg && arvore->dados[i]->chave == (*reg)->chave)
        return -1;

    if (!arvore->filhos[0]) {
        if (arvore->num_reg < 3) {
            for (j = arvore->num_reg; j > i; j--)
                arvore->dados[j] = arvore->dados[j-1];
            arvore->dados[i] = *reg;
            arvore->num_reg++;
            return 0;
        }

    // else overflow
    sobrou = arvore->dados[1];
    novo = alocar_no();
    if (i == 3) {
        novo->dados[1] = *reg;
        novo->dados[0] =  arvore->dados[2];
        arvore->num_reg = 1;
        novo->num_reg = 2;
    }
    else if (i == 2) {
        novo->dados[1] = arvore->dados[2];
        novo->dados[0] = *reg;
        arvore->num_reg = 1;
        novo->num_reg = 2;
    }
    else if (i == 1) {
        novo->dados[0] = arvore->dados[2];
        arvore->dados[1] = *reg;
        arvore->num_reg = 2;
        novo->num_reg = 1;
    }
    else if (i == 0) {
        novo->dados[0] = arvore->dados[2];
        arvore->dados[1] = arvore->dados[0];
        arvore->dados[0] = *reg;
        arvore->num_reg = 2;
        novo->num_reg = 1;
    }

    *sub = novo;
    *reg = sobrou;
    return 1;
    }


    //Aqui foi arrumado, estava passando a propria arvore como parametro, e não o filho.
    //Ai, obviamente, tinhamos uma recursividade (teoricamente) sem fim.
    houveOverflow = inserir(arvore->filhos[i], reg, sub);

    if (houveOverflow < 1)
        return houveOverflow;
    // insercao do registro e do no que voltaram do filho
    if (arvore->num_reg < 3) {   // no nao esta cheio
        for (j = arvore->num_reg; j > i; j--) { // Pedro, esse é o i da 105
            arvore->dados[j] = arvore->dados[j-1];
            arvore->filhos[j+1] = arvore->filhos[j];
        }
    arvore->dados[i] = *reg;
    arvore->filhos[i+1] = *sub;
    arvore->num_reg++;     //   !!!!!!!!!!!!!!!! acrescentei esse comando
    return 0;
    }
    // else overflow
    sobrou = arvore->dados[1];
    novo = alocar_no();
    if (i == 3) {
        novo->dados[1] = *reg;
        novo->dados[0] =  arvore->dados[2];
        novo->filhos[0] = arvore->filhos[2];
        novo->filhos[1] = arvore->filhos[3];
        novo->filhos[2] = *sub;
        arvore->num_reg = 1;
        novo->num_reg = 2;
    }
    else if (i == 2) {
        novo->dados[1] = arvore->dados[2];
        novo->dados[0] = *reg;
        novo->filhos[0] = arvore->filhos[2];
        novo->filhos[1] = *sub;
        novo->filhos[2] = arvore->filhos[3];
        arvore->num_reg = 1;
        novo->num_reg = 2;
    }

    else if (i == 1) {
        novo->dados[0] = arvore->dados[2];
        novo->filhos[0] = arvore->filhos[2];
        novo->filhos[1] = arvore->filhos[3];
        arvore->dados[1] = *reg;
        arvore->filhos[2] = *sub;
        arvore->num_reg = 2;
        novo->num_reg = 1;
    }
    else if (i == 0) {
        novo->dados[0] = arvore->dados[2];
        novo->filhos[0] = arvore->filhos[2];
        novo->filhos[1] = arvore->filhos[3];
        arvore->dados[1] = arvore->dados[0];
        arvore->dados[0] = *reg;
        arvore->filhos[2] = arvore->filhos[1];
        arvore->filhos[1] = *sub;
        arvore->num_reg = 2;
        novo->num_reg = 1;
    }

    *sub = novo;
    *reg = sobrou;
    return 1;
}

struct no *inc (struct registro *reg, struct no *arvore) {
    struct no *sub = NULL;  // !!!!!!!!!!!!! coloquei  a inicializacao com NULL
    int houveOverflow;
    if (!arvore) {
        arvore = alocar_no();
        arvore->dados[0] = reg;
        arvore->num_reg = 1;
        return arvore;
    }

    houveOverflow = inserir(arvore, &reg, &sub);
    if (houveOverflow == 1) {
        struct no *aux;
        aux = arvore;
        arvore = alocar_no();
        arvore->dados[0] = reg;
        arvore->num_reg++;
        arvore->filhos[1] = sub;
        arvore->filhos[0] = aux;
        return arvore;
    }
    return arvore;      // coloquei este comando
  }



///FUNÇÕES PARA AUXÍLIO DA "REMOVER" - Remoção de um registro em Árvore B (ou a especificação em árvore 2-4)
//FUNÇÃO PARA BUSCAR A CHAVE NO NÓ ATUAL (VIAJAR PARA OS FILHOS É RECURSIVO DA "Remover")
int buscar_chave(struct no *arvore, int chave){
    int i;
    for(i = 0; i < arvore->num_reg && arvore->dados[i]->chave < chave; i++);
    return i;
}

///OBTENÇÃO DO MAIOR VALOR MENOR QUE A CHAVE ORIGINAL
int obter_antecessor(struct no *arvore, int i){
    struct no *aux;
    //Como descrito nas regras do item 2, eu preciso achar o antecessor para colocar no
    //lugar do registro I a ser retirado. Aqui eu acho o menor mais próximo da chave retirada
    //Chego ao maior valor menor que a chave original(retirada)
    aux = arvore->filhos[i];
    while(aux->filhos[0]) {
        aux = aux->filhos[aux->num_reg];
    }
    return aux->dados[aux->num_reg-1]->chave;
}

///OBTENÇÃO DO MENOR VALOR MAIOR QUE A CHAVE ORIGINAL
int obter_sucessor(struct no *arvore, int i){
    struct no *aux;
    //Processo simétrico ao do "obter_antecessor", no entanto aqui eu chego no menor valor maior que a chave original.
    aux = arvore->filhos[i+1];
    while(aux->filhos[0]) {
        aux = aux->filhos[0];
    }
    return aux->dados[0]->chave;
}


///FUNDIR ELEMENTOS, SEMPRE REALIZAR A FUSÃO ANTES DE RETIRAR, FACILITA, POIS ASSIM NUNCA VOU TIRAR DE UM NÓ QUE SO TEM 1 REGISTRO
void fundir(struct no *arvore, int i) {
    int j;
    struct no *filho_um, *filho_dois;

    //Entro em fundir normalmente em função do pai com apenas dois "filhos distantes"
    //Vou para o filho da esquerda e vou para o filho da direita
    filho_um = arvore->filhos[i];
    filho_dois = arvore->filhos[i+1];
    printf("\nTESTANDO\n");
    printf("FILHO UM: %d", filho_um->dados[0]->chave);
    printf("\nFILHO DOIS: %d\n", filho_dois->dados[0]->chave);
    printf("\nArvore dados: %d\n", arvore->dados[i]->chave);

    //Adiciono na próxima posição livre  o dado que vou fundir
    filho_um->dados[filho_um->num_reg] = arvore->dados[i];

    for(j = 0; j < filho_dois->num_reg; j++){
        filho_um->dados[j+2] = filho_dois->dados[j];
    }

    if (filho_um->filhos[0]) {
        for (j = 0; j <= filho_dois->num_reg; j++) {
            filho_um->filhos[j+2] = filho_dois->filhos[j];
        }
    }

    //Arrumar a nova raiz, caso o elemento que fique seja o com indice 1 ou 2 (se for o 0 não entra)
    for (j = i+1; j < arvore->num_reg; j++) {
        arvore->dados[j-1] = arvore->dados[j];
        printf("\nTESTANDO1\n");
    }

    for (j = i+2; j <= arvore->num_reg; j++) {
        arvore->filhos[j-1] = arvore->filhos[j];
        printf("\nTESTANDO2\n");
    }

    //Aqui eu deixo o num_reg com um a mais mesmo, pois vou chamar recursivo remover para tirar esse registro adicional
    //que já era pra ter saído
    //tiro um da raiz que foi fundido
    filho_um->num_reg += filho_dois->num_reg + 1;
    arvore->num_reg--;

    //filho dois se fundiu no seu irmao (filho um)
    free(filho_dois);
}

///REMOVER UM REGISTRO DE UM NÓ FOLHA(ESSE REGISTRO NUNCA ESTARÁ SOZINHO - TRATADO FUNDIR). TROCAR AS CHAVES DO NO REMOVIDO QUE NAO FOR FOLHA
void remover_de_no(struct no *arvore, int i){
    int chave, j, antecessor, sucessor;
    chave = arvore->dados[i]->chave;

    //1
    //Se o nó é folha
    if(!arvore->filhos[0]){
        //Removo o registro relativo a chave e desloco os outros registros
        free(arvore->dados[i]);
        for (j = i; j < arvore->num_reg; j++){
            arvore->dados[j] = arvore->dados[j+1];
        }
        arvore->num_reg--;
    }

    //2, 2.1, 2.2
    else {
        //Remover registros de nó não folha
        //Trato de tentar pegar ou da esquerda ou da direita um valor para substituir a chave original
        //caso contrário, funde
        if(arvore->filhos[i]->num_reg >= 2){
            //Adiciono o antecessor no local da chave original e chamo remover para tirar a chave
            //que passei para o local da chave original
            antecessor = obter_antecessor(arvore, i);
            arvore->dados[i]->chave = antecessor;
            remover(arvore->filhos[i], antecessor);
        }

        else if(arvore->filhos[i+1]->num_reg >= 2){
            //Processo simétrico ao de cima, só que com a chave menor que é maior que a original
            sucessor = obter_sucessor(arvore, i);
            arvore->dados[i]->chave = sucessor;
            remover(arvore->filhos[i+1], sucessor);
        }

        //Se bater no caso em que o pai tem os dois filhos distantes entro
        else{
            fundir(arvore, i);
            remover(arvore->filhos[i], chave);
        }
    }
}


///EMPRESTIMO, NO QUAL UM NO ESTÁ SOZINHO E PARA ISSO EMPRESTO DO SEU IRMAO PARA O PAI E DO PAI PARA O IRMAO SOZINHO(FILHO)
void emprestar_do_anterior(struct no *arvore, int i) {
    int j;
    struct no *filho_um, *filho_dois;

    //Atribuo para filho_um o que sera removido e para filho_dois quem fara o empréstimo(nos)
    filho_um = arvore->filhos[i];
    filho_dois = arvore->filhos[i-1];

    printf("Filho um: %d", filho_um->dados[0]->chave);
    printf("\nFilho dois: %d", filho_dois->dados[0]->chave);

    for (j = filho_um->num_reg - 1; j >= 0; j--) {
        filho_um->dados[j+1] = filho_um->dados[j];
    }

    printf("\nFILHO_UM_DADOS %d", filho_um->dados[j+1]->chave);

    if (filho_um->filhos[0]) {
        for (j = filho_um->num_reg; j >= 0; j--) {
            filho_um->filhos[j+1] = filho_um->filhos[j];
        }
    }

    //Empresto pro no que vou retirar o elemento da raiz mais proximo dele(direita)
    filho_um->dados[0] = arvore->dados[i-1];

    if (filho_um->filhos[0]) {
        filho_um->filhos[0] = filho_dois->filhos[filho_dois->num_reg];
    }

    //Empresto pra raiz o maior elemento do filho dois, para fazer parte da raiz
    arvore->dados[i-1] = filho_dois->dados[filho_dois->num_reg-1];

    //Como entrou um membro no no filho um e saiu um membro do no filho dois, faço isso
    filho_um->num_reg++;
    filho_dois->num_reg--;
}

///MESMA COISA DE CIMA, SÓ QUE DO OUTRO LADO
void emprestar_do_proximo(struct no *arvore, int i) {
    int j;
    struct no *filho_um, *filho_dois;

    //Mesma fita da "emprestar_do_anterior", só que aqui empresto do proximo
    //claro que a raiz sempre empresta nas duas
    filho_um = arvore->filhos[i];
    filho_dois = arvore->filhos[i+1];

    filho_um->dados[filho_um->num_reg] = arvore->dados[i];

    if (filho_um->filhos[0]) {
        filho_um->filhos[filho_um->num_reg +1] = filho_dois->filhos[0];
    }

    arvore->dados[i] = filho_dois->dados[0];

    for (j = 1; j < filho_dois->num_reg; ++j) {
        filho_dois->dados[j-1] = filho_dois->dados[j];
    }

    if (filho_dois->filhos[0]) {
        for (j = 1; j <= filho_dois->num_reg; ++j) {
            filho_dois->filhos[j-1] = filho_dois->filhos[j];
        }
    }
    filho_um->num_reg++;
    filho_dois->num_reg--;
}

///FUNÇÃO INTERMEDIARIA PARA VER SE VAI SER FEITO O EMPRESTIMO OU UMA FUSÃO
void preencher(struct no *arvore, int i){

    //O 120 está no I = 2 do seu pai, no entanto a familia do I-1 tem pra emprestar familiar
    if (i != 0 && arvore->filhos[i-1]->num_reg >= 2){
        emprestar_do_anterior(arvore, i);
    }
    //caso simétrico
    else if(i != arvore->num_reg && arvore->filhos[i+1]->num_reg >= 2){
        emprestar_do_proximo(arvore, i);
    }
    else{
        if(i != arvore->num_reg){
            printf("TESTANDOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO");
            printf("\n arvore num_reg: %d", arvore->dados[i]->chave);
            fundir(arvore, i);
        }
        else{
            fundir(arvore, i-1);
        }
    }
}


///FUNÇÃO PARA REMOVER A CHAVE DA ÁRVORE///
void remover(struct no *arvore, int chave) {
    int i, flag;

    //Se não tem arvore retorna
    if (!arvore)
        return;

    //Buscar a chave ou o caminho pro filho onde supostamente tem a chave
    i = buscar_chave(arvore, chave);

    //Se a chave está presente no nó em questão
    if(i < arvore->num_reg && arvore->dados[i]->chave == chave){
        //Caso seja uma folha
        remover_de_no(arvore, i);
    }
    else{
        //Se o nó é folha e não achei a chave antes, logo ela nem ta nessa arvore
        if(!arvore->filhos[0]) {
            return;
        }

        //Indica se a chave que eu vou retirar (supostamente) está no último filho do nó
        if(i == arvore->num_reg){
            flag = 1;
        }
        else{
            flag = 0;
        }

        //Se o filho onde a chave supostamente pode estar tiver somente 1 registro, eu preencho ele
        //para não haver underflow (nó ficar com menos registros que o mínimo permitido)
        printf("\narvorefilhos %d", arvore->filhos[i]->dados[0]->chave);
        if(arvore->filhos[i]->num_reg < 2) {
            printf("\nTestando");
            preencher(arvore, i);

        }

        //Se a chave que eu vou retirar está no último filho do nó, pois a chave que estava no
        //último nó pode ter sido movida para o seu irmão adjacente na hora da utilização do "preencher"
        //caso contrário, segue procurando a chave no nó escolhido
        if(flag && i > arvore->num_reg) {
            remover(arvore->filhos[i - 1], chave);
        }else{
            remover(arvore->filhos[i], chave);
        }
    }
}


int main(){

    struct no *arvore;
    struct registro *reg;
    arvore = NULL;

    arvore = inc(alocar_registro(50), arvore);
    arvore = inc(alocar_registro(100), arvore);
    arvore = inc(alocar_registro(20), arvore);
    arvore = inc(alocar_registro(30), arvore);
    arvore = inc(alocar_registro(40), arvore);
    arvore = inc(alocar_registro(60), arvore);
    arvore = inc(alocar_registro(120), arvore);
    arvore = inc(alocar_registro(70), arvore);
    arvore = inc(alocar_registro(80), arvore);

    reg = buscar(arvore, 9);
    if (reg != NULL)
        printf("O elemento de valor %d foi localizado.\n", reg->chave);
    else
        printf("O elemento nao foi localizado.\n");

    reg = buscar(arvore, 60);
    if (reg != NULL)
        printf("O elemento de valor %d foi localizado.\n", reg->chave);
    else
        printf("O elemento nao foi localizado.\n");

    reg = buscar(arvore, 11);
    if (reg != NULL)
        printf("O elemento de valor %d foi localizado.\n", reg->chave);
    else
        printf("O elemento nao foi localizado.\n");

    reg = buscar(arvore, 15);
    if (reg != NULL)
        printf("O elemento de valor %d foi localizado.\n", reg->chave);
    else
        printf("O elemento nao foi localizado.\n");

    percorrer_nos(arvore, 0);
    printf("\n\n");
    remover(arvore, 300);
    remover(arvore, 300);
    percorrer_nos(arvore, 0);
    printf("\n\n");
    remover(arvore, 70);
    percorrer_nos(arvore, 0);
    printf("\n\n");
    remover(arvore, 30);
    percorrer_nos(arvore, 0);
    printf("\n\n");
    remover(arvore, 50);
    percorrer_nos(arvore, 0);
    printf("\n\n");
    remover(arvore, 60);
    percorrer_nos(arvore, 0);
    //printf("\nNUM_REG DA ARVORE: %d", arvore->filhos[1]->num_reg);

    ///CORRIGIR A REMOÇÃO DE CHAVES QUE NÃO EXISTEM
    eliminar(arvore);
}
