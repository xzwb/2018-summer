#include<stdio.h>
#include<string.h>
int main()
{
    char haab[19][10] = {"pop", "no", "zip", "zotz", "tzec", "xul", "yoxkin", "mol", 
                     "chen", "yax", "zac", "ceh", "mac", "kankin", "muan", "pax",
                     "koyab", "cumhu", "uayet"};
    char tzplkin[20][10] = {"imix", "ik", "akbal", "kan", "chicchan", "cimi", "manik",
                      "lamat", "muluk", "ok",  "chuen", "eb", "ben", "ix", "mem", 
                       "cib", "caban", "eznab", "canac", "ahau"};
    int c;
    scanf("%d",&c);
    printf("%d\n",c);
    
    int number,year;
    int sum;
    char name_of_the_day[10];
    while(n--)
    {
        scanf("%d. %s %d",&number,name_of_the_day,&year);
        int i;
        for(i = 0;i <= 18;i++)
	{
            if(strcmp(haab[i],name_of_the_day) == 0)
                break;
        }
        sum = year*365 + i*20 + number;
        printf("%d %s %d\n",sum%13+1,tzplkin[sum%20],sum/260);
    }
    return 0;
}

