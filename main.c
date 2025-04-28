#include <stdio.h>
#include <string.h>
#include <ctype.h>

int main()
{
    char namn[5][11];
    int resultat[5][13];
    float medelpoang[5];
    float total_medel = 0;
    int i, j;

    // Inmatning av namn och poäng
    for (i = 0; i < 5; i++)
    {
        scanf("%10s", namn[i]);
        for (j = 0; j < 13; j++)
        {
            scanf("%d", &resultat[i][j]);
        }
    }

    // Beräkning av medelpoäng
    for (i = 0; i < 5; i++)
    {
        int summa = 0;
        for (j = 0; j < 13; j++)
        {
            summa += resultat[i][j];
        }
        medelpoang[i] = summa / 13.0;
        total_medel += medelpoang[i];
    }

    // Beräkning av total medelpoäng
    total_medel = total_medel / 5.0;

    // Hitta index för elev med högst medel
    int max_index = 0;
    for (i = 1; i < 5; i++)
    {
        if (medelpoang[i] > medelpoang[max_index])
        {
            max_index = i;
        }
    }

    // Skriv ut bästa elev (med stor bokstav först)
    char temp_namn[11];
    strcpy(temp_namn, namn[max_index]);
    temp_namn[0] = toupper(temp_namn[0]);
    printf("%s\n", temp_namn);

    // Skriv ut elever under medel (med stor bokstav först, i ordning)
    for (i = 0; i < 5; i++)
    {
        if (i != max_index && medelpoang[i] < total_medel)
        {
            strcpy(temp_namn, namn[i]);
            temp_namn[0] = toupper(temp_namn[0]);
            printf("%s\n", temp_namn);
        }
    }
    return 0;
}