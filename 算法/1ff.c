#include <stdio.h>
#include <string.h>

int main()
{
	int n;
	int i;
	int year;
	char day[5];
	int dayd[10000];
	int year1[10000];
	int month[10000];
	char name[7];
	scanf("%d",&n);
	for(i=0;i<n;i++)
	{
		dayd[i] = 0;
		scanf("%s %s %d",day,name,&year);
		int k = strlen(day);
		while(--k)
		{
			if(k==2)
				dayd[i] += day[k-1]-48+1;
			else if(k==1 && dayd[i] != 0)
		       		dayd[i] += (day[k-1]-48)*10;
		 	else
				dayd[i] += (day[k-1]-48)+1;	
		}
		if(strcmp(name,"no")==0)
			dayd[i] += 20;
		if(strcmp(name,"zip")==0)
			dayd[i] += 40;
		if(strcmp(name,"zotz")==0)
			dayd[i] += 60;
		if(strcmp(name,"tzec")==0)
			dayd[i] += 80;
		if(strcmp(name,"xul")==0)
			dayd[i] += 100;
		if(strcmp(name,"yoxkin")==0)
			dayd[i] += 120;
		if(strcmp(name,"mol")==0)
			dayd[i] += 140;
		if(strcmp(name,"chen")==0)
			dayd[i] +=160;
		if(strcmp(name,"yax")==0)
			dayd[i] +=180;
		if(strcmp(name,"zac")==0)
			dayd[i]+=200;
		if(strcmp(name,"ceh")==0)
			dayd[i]+=220;
		if(strcmp(name,"mac")==0)
			dayd[i]+=240;
		if(strcmp(name,"kankin")==0)
			dayd[i]+=260;
		if(strcmp(name,"muan")==0)
			dayd[i]+=280;
		if(strcmp(name,"pax")==0)
			dayd[i]+=300;
		if(strcmp(name,"koyab")==0)
			dayd[i]+=320;
		if(strcmp(name,"cumhu")==0)
			dayd[i]+=340;
		if(strcmp(name,"uayet")==0)
			dayd[i]+=360;
		dayd[i] += year*365;
		year1[i] = dayd[i]/260;
		month[i] = (dayd[i]-year1[i]*260)/20+1;
		dayd[i] = dayd[i]%13;
		if(dayd[i] == 0)
			dayd[i] = 13;
	}
	printf("%d\n",n);
	for(i=0;i<n;i++)
	{
		if(month[i]==1)
			printf("%d %s %d\n",dayd[i],"imix",year1[i]);
		if(month[i]==2)
			printf("%d %s %d\n",dayd[i],"ik",year1[i]);
		if(month[i]==3)
			printf("%d %s %d\n",dayd[i],"akabl",year1[i]);
		if(month[i]==4)
			printf("%d %s %d\n",dayd[i],"kan",year1[i]);
		if(month[i]==5)
			printf("%d %s %d\n",dayd[i],"chicchan",year1[i]);
		if(month[i]==6)
			printf("%d %s %d\n",dayd[i],"cimi",year1[i]);
		if(month[i]==7)
			printf("%d %s %d\n",dayd[i],"manik",year1[i]);
		if(month[i]==8)
			printf("%d %s %d\n",dayd[i],"lamat",year1[i]);
		if(month[i]==9)
			printf("%d %s %d\n",dayd[i],"muluk",year1[i]);
		if(month[i]==10)
			printf("%d %s %d\n",dayd[i],"ok",year1[i]);
		if(month[i]==11)
			printf("%d %s %d\n",dayd[i],"chuen",year1[i]);
		if(month[i]==12)
			printf("%d %s %d\n",dayd[i],"eb",year1[i]);
		if(month[i]==13)
			printf("%d %s %d\n",dayd[i],"ben",year1[i]);
		if(month[i]==14)
			printf("%d %s %d\n",dayd[i],"ix",year1[i]);
		if(month[i]==15)
			printf("%d %s %d\n",dayd[i],"mem",year1[i]);
		if(month[i]==16)
			printf("%d %s %d\n",dayd[i],"cib",year1[i]);
		if(month[i]==17)
			printf("%d %s %d\n",dayd[i],"caban",year1[i]);
		if(month[i]==18)
			printf("%d %s %d\n",dayd[i],"eznab",year1[i]);
		if(month[i]==19)
			printf("%d %s %d\n",dayd[i],"canac",year1[i]);
		if(month[i]==20)
			printf("%d %s %d\n",dayd[i],"ahau",year1[i]);
	}
}
