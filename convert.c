#include <stdio.h>
#include <string.h>
#include <locale.h>
#include <malloc.h>


char * SaisirChaineClavier()
{
     unsigned int c, nbc = 0 ;
     char * ch = NULL ;

     while((c = getchar()) != '\n')
     {
        ch = realloc(ch, (nbc+1)*sizeof(char));
        ch[nbc++] = c ;
     }
     ch = realloc(ch, (nbc+1)*sizeof(char));
     ch[nbc] = '\0' ;

     return ch ;
}

char * SaisirLigneFichier(FILE * pf)
{
    unsigned int c, nbc = 0 ;
    char * ch = NULL ;

    while(((c = fgetc(pf)) != '\n') && (c != EOF))
    {
        ch = realloc(ch, (nbc+1)*sizeof(char));
        ch[nbc++] = c ;
    }
    if((nbc > 0) || (c == '\n'))
    {
        ch = realloc(ch, (nbc+1)*sizeof(char));
        ch[nbc] = '\0' ;
        return ch ;
    }

    return NULL ;
}

char ** CreerVecteurNombres(char *nf , unsigned int * taille)
{
    FILE * pf = fopen(nf, "r") ;
    char * str = NULL ;
    unsigned int i = 0 ;
    char ** VN = NULL ;

    if(pf == NULL)
    {
        printf("\n probleme lors de l ouverture du fichier %s\n", nf) ;
        fclose(pf) ;
        *taille = 0 ;
        return NULL ;
    }

    while((str = SaisirLigneFichier(pf)) != NULL)
    {
        VN = realloc(VN, (i+1)*sizeof(char*)) ;
        VN[i++] = str ;
    }

    fclose(pf) ;
    *taille = i ;
    return VN ;
}

//-------------------------------------------------------------------------
//-----------------------------PROGRAMME----------------------------------- :

// fonction qui convertit un chiffre de type char ---> type int :
int CnvToNum(char charValue)
{
    int number = charValue - '0';
    return number;
    /* car le code ascii de '0' est 48 est donc (par exemple '2' a code ascii 50
                        donc 2='2'-'0'=50-48=2                                  */
}


/*le role principale de cette fonction est de prendre comme parametre un nombre qui est
représenté par un ou plusieurs chiffres de type char et l'afficher en lettre :*/
void fct(char *str)
{

    int q, nbc, l, r;

    /* la creation d'un tableau TC qui contient des nombre en lettre de
        0 à 99 avec des indices à partir d'un fichier( exemple TC[3]="trois")*/
    char ** TC = CreerVecteurNombres("CL_FR0.txt" , &nbc) ;
    setlocale(LC_CTYPE,"") ;// permet d'afficher les accents

    //creer un tableau Tstr  qui contient les chiffre de type int qui compose la chaine qui on veut traiter :
        /* exemple (str="74" , str[0]='7' , str[1]='4')--->( Tstr[0]=7 , Tstr[1]=4) */
    l=strlen(str);
    int Tstr[l];
    for(int j=0;j<l;j++)
    {
        Tstr[j]=CnvToNum(str[j]);
    }


    //1er cas de nombre 0000000 il faut afficher zéro
    int etat=0;
    for(int j=0;j<l;j++)
    {
        if(Tstr[j]!=0)etat=1;
    }
    if(etat==0)printf("%s ",TC[0]);

    //cas diffèrent de zéro
    /* le principe de notre programme est de diviser notre nombre en á des blocs qui contient 3 chiffre
        chaque 3 chiffre va être traiter de la même manière mais on change seulement L'unité (mille ,million...)
        on a trois possibilité:

        1-  NOMBRE COMPLET sa longueur est multiple de 3 :
            exemple : 234 567 890 son longueur est 9 donc nombre de bloc est q=9/3=3 et le rest r=9%3=0
            donc on a un nombre complet de 3 bloc 234 et 567 et 890 on les traite de la même façon, on fait la concaténation :
                "deux" + "cent" + "trente-quatre" + "millions"
              + "cinq" + "cent" + "soixante-sept" + "mille"
              + "huit" + "cent" + "quatre-vingt-dix" +  ""
            on remarque qu'on a une répétition , donc on écrit une boucle qui fait le traitement d'un NOMBRE COMPLET.

        2- NOMBRE NON COMPLET sa longueur est n'est pas une multiple de 3
            On trouve deux cas:

                A- si le reste  r=longueur%3=1:
                    exemple : 1234567890 sa longueur est 6 donc nombre de bloc est q=10/3=3 et le rest r=10%3=1
                    donc on a un chiffre (1) et un nombre complet de 3 bloc (234 et 567 et 890)
                    on le traite de la même façon, on fait concaténation :
                         "un" + "miliard"
                       + "deux" + "cent" + "trente-quatre" + "millions"
                       + "cinq" + "cent" + "soixante-sept" + "mille"
                       + "huit" + "cent" + "quatre-vingt-dix" +  ""
                    on remarque que on a un seul différent c'est qu'on dit le  premier chiffre avec son unité
                    et après on fait le traitement d'un nombre complet.

                B-si le reste  r=longueure%3=2:
                 exemple : 34567890 sa longueur est 8 donc nombre de bloc est q=8/3=2 et le rest r=10%3=1
                    donc on a deux chiffre (34) et un nombre complet de 2 bloc (567 et 890)
                    on le traite de même façon, on fait concaténation :
                         "trente-quatre" + "millions"
                       + "cinq" + "cent" + "soixante-sept" + "mille"
                       + "huit" + "cent" + "quatre-vingt-dix" +  ""
                    on remarque que on a un seul differnt c'est qu'on dit les 2 premiers chiffres avec unité
                    et après on fait le traitement d'un nombre complet.
    */
// le code du cas d'un nombre different de 0:
    else
    {
        char *TSU[]={"","mille","million","milliard","billion"}; // tableau special qui contient les unités de (1)
        char *TS[]={"","mille","milions","miliards","bilions"}; /* tableau special qui contient les unités de ( 2,3,...,999)
                                                                et mille est invariant pour tout les nombre*/
        q=l/3;//nombre de bloc
        r=l%3;//le reste qui donne le cas qu'on a
        int i=0;

    //------------------------------------------------------------------------

        if(r==2)/* cas : " 2 chiffre "+ " nombre complet" ,exemple: 05 ou 12 ou 00 123 ou 12 345 ou 01 234 ou 12 345678
                    alors on traite les 2 premiers chiffre*/
        {
            int dnb=Tstr[i]*10+Tstr[i+1]; //exemple(12 345) donc  Tstr[i]*10+Tstr[i+1] = 1*10+2=12
            if(dnb!=0)// cas comme 00123 donc donc il ne faut pas écrir " zero mille cent vingt-trois"
            {
                if(dnb==1)
                {
                    if(l==5)printf("mille ");/* c'est une règle ,exemple : 01 234 on ne dit pas un mile.. mais on dit mille..,
                                                et l=5 et l-r=3 car la règle est appliqué seulement dans les milles*/
                    else  printf("un %s ",TSU[q]);/* exemple (01 234567=un million...),(01 234567890=un milliard)...
                                                    on a utilisé le tableau TSU car on a le cas de un*/
                }
                else printf("%s %s ",TC[dnb],TS[q]);// exemple (12345=douze mille..), (05345678=cinq milions..)
            }
            i=i+2;//si on a r=2 donc on a traité les deux premiers chiffre et on passe à traiter les blocs
            if(i>=l) return;//si on a just 2 chiffre dans toute la chaine .exemple 12 ou 03 , donc il faut stoper notre programme
        }

    //------------------------------------------------------------------------

        if(r==1)/* cas : " 1 chiffre "+ " nombre complet" ,exemple: 6 ou 2 345 ou 0 123 ou 3 456789 .
                    alors on traite le premier chiffre */
        {
            if(Tstr[i]==1)
            {
                if(l==4)printf("mille ");/* c'est une règle et l=4 car la règle est appliqué seulement dans les milles
                                            et l=4 et l-r=3 car on a une seule cas c'est exemple(1234 ou 1456 ou .. )*/
                else  printf("un %s ",TSU[q]);/* exemple (1 234 567=un million...),(1 234 567 890=un milliard)...
                                                    on a utilisé le tableau TSU car on a les cas de un*/
            }
            else if(Tstr[i]!=0)  printf("%s %s ",TC[Tstr[i]],TS[q]);/*exemple (2 345= 2 mille..),(6 345678=six millions ..)
                                                                        on a utilisé le tableau TS car on a pas le cas de un*/
            i++;//si on a r=1 donc on a traité le premier premiers chiffre et on passe à traiter les blocs
            if(i>=l) return;//si on a just 1 chiffre dans toute la chaine . exemple 2 ou 7 , donc il faut stoper notre programme
        }

    //------------------------------------------------------------------------

    /*après le traitement  du cas les premiers chiffres quui reste d'un nombre a un longueur qui n'a pas multiple de 3
            on passe maitenant à traiter les blocs */
        do
        {
            if(Tstr[i]!=0) //traitement du premier chiffre dans le bloc
            {
                if(Tstr[i]==1)printf("cent ");//exemple 123 ou 123456 (on dit cent..)
                else printf("%s cent ",TC[Tstr[i]]);//exemple 234 ou 345678 (on dit deux cent ..) , (on dit trois cent..)
            }
            i++;// on passe à la 2em et la 3em chiffre dans le bloc
            q--;/* le tableau  TS et TSU commence par indice 0 donc si on a par exemple 123456 donc on a q=2 bloc
                    et la premiere unité est "mille" qui est indicé par 1 et donc q-- .
                    on n'a pas fait q-- quand r!=0 car on a par exemple 1 234 567 qui a q=2 et l'unité  est "million"
                    qui est indicé par 2==q */
            unsigned int dnb=Tstr[i]*10+Tstr[i+1];//traitement du 2em chiffre dans le bloc
            if(dnb!=0)// cas comme 100 donc donc il ne faut pas écrir " cent zero zero "
            {
                if(dnb==1)
                {
                    if(l-i==5)printf("mille ");/* c'est une règle et l=5 car la règle est appliqué seulement dans les milles
                     exemple l=6 et quand l-i=5 car on a une seule cas c'est exemple(123 456 789 ou 123 456 ou .. )*/
                    else  printf("un %s ",TSU[q]);
                }
                else printf("%s %s ",TC[dnb],TS[q]);
            }
            else if(Tstr[i-1]!=0){ printf("%s  ",TS[q]);}
            i=i+2;//i va etre indicer 2em chiffre dans le bloc et il faut deplacer vers le bloc suivant exemple( 1[2]3 456 ==> 123 [4]56)
        } while(i<l || q>0);
    }
}


void fctR(char * str)
{
    int l=strlen(str);
    int i=0;
    while(str[i]!=',' && i<l )i++;
    if(i==l)//cas d'en nombre entier
    {
         if (strlen(str)>15)
        {
            printf("vous avez depasse la limite\n");
            return ;
        }
        fct(str);
    }
    else//cas d'un nombre reel
    {
        char CG[i];//chaine gauche =partie entier
         if (i>15 || l-i-1>15) // i la longueur de la partie entiere et l-i-1 longueur de la partie dicimal
        {
            printf("vous avez depasse la limite\n");
            return ;
        }
        for(int k=0; k<i ;k++)
        {
            CG[k]=str[k];
        }
        CG[i]='\0';
        fct(CG);
        printf("virgule ");
        //chaine droit
        int ld=l-i-1;
        char CD[ld];//chaine droit= partie dicimal
        int s=0,etat=0;
        for(int k=i+1; k<l ;k++)
        {
            CD[s++]=str[k];
        }
        CD[ld]='\0';
        s=0;
        int k=0;
        while(CD[s++]=='0')k++;
        if (k==ld)//exemple  1,00000000 il faut dire: un virgule zero
        fct(CD);
        else
        {
            s=0;
            while(CD[s++]=='0')fct("0");//exemple 1,000023 il faut dire :un  virgule zéro zéro zéro zéro vingt-trois
            fct(CD);
        }

    }

}
int test(char *str)
{

    if(str[strlen(str)-1]==',') return 1;//exemple 123,
    for(int i=0 ; i<strlen(str); i++)
    {
        if(str[i]> 57 || str[i]<48 && str[i]!=',') return 1;// code qui teste si la chaine est une nombre
    }
    return 0;
}
void menu ()
{
    char *str;
    do
    {
    printf("\n donner un nombre : ");
    str=SaisirChaineClavier();
    }
    while(test(str));
    printf("\n votre nombre en chiffre est : %s",str);
    printf("\n votre nombre en lettre est : ");
    fctR(str);
}
int main()
{
    menu();
}


