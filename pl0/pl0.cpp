/*����ϸע�͵�C���԰汾PL/0�������
*��������л���:
*1Visual C++6.0,VisualC++.NET and Visual C++.NET 2003
*WinNT, Win2000, WinXP and  Win2003
*2 gcc version 3.3.2  20031022(Red Hat Linux 3.3.2-1)
*Redhat Fedora core 1
*Intel 32 platform
*ʹ�÷���:
*���к�����PL/0 Դ�����ļ���
*�ش��Ƿ�������������
*�ش��Ƿ�������ֱ�
*fa.tmp ������������
*fa1.tmp  ���Դ�ļ�������ж�Ӧ���׵�ַ
*fa2.tmp  ������
*fas.tmp  ������ֱ�
*/
#include<stdio.h>
#include"pl0.h"
#include"string.h"
/*����ִ��ʱʹ�õ�ջ*/
#define stacksize 500
int main()
{
	bool nxtlev[symnum];
	printf("Input pl/0 file ?");
	scanf("%s",fname);                               /*�����ļ���*/
	fin=fopen(fname,"r");
	if(fin)
	{
		printf("List object code ?(Y/N)");                /*�Ƿ�������������*/
		scanf("%s",fname);
		listswitch=(fname[0]=='y'||fname[0]=='Y');
		printf("List symbol table ? (Y/N)");              /*�Ƿ�������ֱ�*/
		scanf("%s",fname);
		tableswitch=(fname[0]=='y'||fname[0]=='Y');
		fa1=fopen("fa1.tmp","w");
		fprintf(fa1,"Iput pl/0 file ?");
		fprintf(fa1,"%s\n", fname);
		init();                                      /*��ʼ��*/
		err=0;
		cc=cx=ll=0;
		ch=' ';
		if(-1!=getsym())/*�ɹ���ȡ��һ������*/
		{
			fa=fopen("fa.tmp","w");
			fas=fopen("fas.tmp","w");
			addset(nxtlev,declbegsys,statbegsys,symnum);
			nxtlev[period]=true;
			if(-1==block(0,0,nxtlev))			/*���ñ������*/
			{
				fclose(fa);
				fclose(fa1);
				fclose(fas);
				fclose(fin);/*�������δ�ɹ��������ر������Ѵ򿪵��ļ�������*/
				printf("\n");
				return 0;
			}
			fclose(fa);
			fclose(fa1);
			fclose(fas);
			if(sym!=period)
			{
				error(9);
			}
			if(err==0)
			{
				fa2=fopen("fa2.tmp", "w");
				interpret();/*���ý���ִ�г���*/
				fclose(fa2);
			}
			else
			{
				printf("Errors in pl/0 program");
			}
		}
		fclose(fin);
	}
	else
	{
		printf("Can't open file! \n");
	}
	printf("\n");
	return 0;
}
/*
*��ʼ��
*/
void init()
{
	int i;
	for(i=0;i<=255;i++)
	{
		ssym[i]=nul;
	}
	ssym['+']=plus;
	ssym['-']=minus;
	ssym['*']=times;
	ssym['/']=slash;
	ssym['(']=lparen;
	ssym[')']=rparen;
	ssym['=']=eql;
	ssym[',']=comma;
	ssym['.']=period;
	ssym['#']=neq;
	ssym[';']=semicolon;

	ssym['['] = lbrack;
	ssym[']'] = rbrack;
	ssym[':'] = colon;
	ssym['!'] = not;
	ssym['@'] = logic;
	ssym['%'] = mod;
	/*���ñ���������,������ĸ˳��,�����۰����*/
	strcpy(&(word[0][0]),"begin");
	strcpy(&(word[1][0]),"call");
	strcpy(&(word[2][0]),"const");
	strcpy(&(word[3][0]),"do");
    strcpy(&(word[4][0]),"else");
	strcpy(&(word[5][0]),"end");
	strcpy(&(word[6][0]),"for");
	strcpy(&(word[7][0]),"if");
	strcpy(&(word[8][0]),"odd");
	strcpy(&(word[9][0]),"procedure");
	strcpy(&(word[10][0]),"read");
	strcpy(&(word[11][0]),"repeat");
	strcpy(&(word[12][0]),"then");
	strcpy(&(word[13][0]),"until");
	strcpy(&(word[14][0]),"var");
	strcpy(&(word[15][0]),"while");
	strcpy(&(word[16][0]),"write");
	/*���ñ����ַ���*/
	wsym[0]=beginsym;
	wsym[1]=callsym;
	wsym[2]=constsym;
	wsym[3]=dosym;
    wsym[4]=elsesym;
	wsym[5]=endsym;
	wsym[6]=forsym;
	wsym[7]=ifsym;
	wsym[8]=oddsym;
	wsym[9]=procsym;
	wsym[10]=readsym;
	wsym[11]=repeatsym;
	wsym[12]=thensym;
	wsym[13]=untilsym;
	wsym[14]=varsym;
	wsym[15]=whilesym;
	wsym[16]=writesym;
	/*����ָ������*/
	strcpy(&(mnemonic[lit][0]),"lit");
	strcpy(&(mnemonic[opr][0]),"opr");
	strcpy(&(mnemonic[lod][0]),"lod");
	strcpy(&(mnemonic[sto][0]),"sto");
	strcpy(&(mnemonic[cal][0]),"cal");
	strcpy(&(mnemonic[inte][0]),"int");
	strcpy(&(mnemonic[jmp][0]),"jmp");
	strcpy(&(mnemonic[jpc][0]),"jpc");

	strcpy(&(mnemonic[lda][0]), "lda");
	strcpy(&(mnemonic[sta][0]), "sta");

	/*���÷��ż�*/
	for(i=0;i<symnum;i++)
	{
		declbegsys[i]=false;
		statbegsys[i]=false;
		facbegsys[i]=false;
	}
	/*����������ʼ���ż�*/
	declbegsys[constsym]=true;
	declbegsys[varsym]=true;
	declbegsys[procsym]=true;
	/*������俪ʼ���ż�*/
    statbegsys[beginsym]=true;
	statbegsys[callsym]=true;
	statbegsys[forsym]=true;
	statbegsys[ifsym]=true;
	statbegsys[repeatsym]=true;
	statbegsys[whilesym]=true;
	/*�������ӿ�ʼ���ż�*/
	facbegsys[ident]=true;
	facbegsys[number]=true;
	facbegsys[lparen]=true;
	facbegsys[addadd]=true;     /*ǰ��++*/
	facbegsys[subsub]=true;     /*ǰ��--*/
	facbegsys[not]=true;        /*��Ӷ�!��ʶ��*/
	facbegsys[logic]=true;		/*��Ӷ�@��ʶ��*/
}
 /*
  *������ʵ�ּ��ϵļ�������
  */
int inset(int e,bool* s)
{
    return s[e];/*����e������s�е�ֵ*/
}
int addset(bool* sr,bool* s1,bool* s2,int n)
{
    int i;
    for(i=0;i<n;i++)
    {
       sr[i]=s1[i]||s2[i];/*�߼�������*/
    }
    return 0;
}
int subset(bool* sr,bool* s1,bool* s2,int n)
{
    int i;
    for(i=0;i<n;i++)
    {
       sr[i]=s1[i]&&(!s2[i]);
    }
    return 0;
}
int mulset(bool* sr,bool* s1,bool* s2,int n)
{
    int i;
    for(i=0;i<n;i++)
    {
       sr[i]=s1[i]&&s2[i];/*�߼�������*/
    }
    return 0;
}
/*
 *��������ӡ����λ�úʹ������
 */
void error(int n)

{
	char space[81];
	memset(space,32,81); printf("-------%c\n",ch);
	space[cc-1]=0;/*����ʱ��ǰ�����Ѿ����꣬����cc-1*/
	printf("****%s!%d\n",space,n);
	switch(n)
	{
        case 1:
            printf("****����˵���еġ�=��д�ɡ���=����\n");
            break;
        case 2:
            printf("****����˵���еġ�=����Ӧ�����֡�\n");
            break;
        case 3:
            printf("****����˵���еı�ʶ����Ӧ�ǡ�=����\n");
            break;
        case 4:
            printf("****const,var,procedure��ӦΪ��ʶ����\n");
            break;
        case 5:
            printf("****©���ˡ�,����;����\n");
            break;
        case 6:
            printf("****����˵����ķ��Ų���ȷ(Ӧ����俪ʼ��,����̶����)\n");
            break;
        case 7:
            printf("****Ӧ����俪ʼ����\n");
            break;
        case 8:
            printf("****����������䲿�ֵĺ��������ȷ��\n");
            break;
        case 9:
            printf("****�����β���˾�š�.��\n");
            break;
        case 10:
            printf("****���֮��©�ˡ�;����\n");
            break;
        case 11:
            printf("****��ʶ��δ˵����\n");
            break;
        case 12:
            printf("****��ֵ����У���ֵ���󲿱�ʶ������Ӧ�Ǳ�����\n");
            break;
        case 13:
            printf("****��ֵ����󲿱�ʶ����Ӧ�Ǹ�ֵ�š�:=����\n");
            break;
        case 14:
            printf("****call��ӦΪ��ʶ����\n");
            break;
        case 15:
            printf("****call���ʶ������ӦΪ���̡�\n");
            break;
        case 16:
            printf("****��������ж��ˡ�then����\n");
            break;
        case 17:
            printf("****���ˡ�end����;����\n");
            break;
        case 18:
            printf("****while��ѭ������ж��ˡ�do����\n");
            break;
        case 19:
            printf("****����ķ��Ų���ȷ��\n");
            break;
        case 20:
            printf("****ӦΪ��ϵ�������\n");
            break;
        case 21:
            printf("****���ʽ�ڱ�ʶ�����Բ����ǹ��̡�\n");
            break;
        case 22:
            printf("****���ʽ��©�������š�)����\n");
            break;
        case 23:
            printf("****���Ӻ�ķǷ����š�\n");
            break;
        case 24:
            printf("****���ʽ�Ŀ�ʼ�������Ǵ˷��š�\n");
            break;
        case 30:
            printf("****����Խ�硣\n");
            break;
        case 31:
            printf("****���ʽ�ڳ���Խ�硣\n");
            break;
        case 32:
            printf("****Ƕ����ȳ�������ֵ��\n");
            break;
        case 33:
            printf("****read��write��for�����ȱ��������\n");
            break;
        case 34:
            printf("****read��write��for�����ȱ��������\n");
            break;
        case 35:
            printf("****read��������еı�ʶ�����Ǳ�����\n");
            break;
	}
	err++;
}
/*
 *  ©���ո񣬶�ȡһ���ַ�
 *
 *  ÿ�ζ�һ�У�����line��������line��getsymȡ�պ��ٶ�һ��
 *
 *  ������getsym����
 */
int getch()
{
	if(cc==ll)  /*cc��ʾ��ǰ�ַ�ch��λ�ã�llָ��line����������β
            ����л�����ָ��ָ���л��������һ���ַ��ʹ��ļ���һ�е��л�����*/
    {
		if(feof(fin))   /*feof�Ǽ�����ϵ��ļ�������
                        ��������ļ�ĩβ*/
		{
			printf("program incomplete");   /*��ӡ����*/
			return -1;
		}
		ll=0;/*��ָ�������ʼλ��*/
		cc=0;
		printf("%d ",cx );  /*���������ָ�룬ȡֵ��Χ[0,cxmax-1]*/
		fprintf(fa1,"%d ",cx);  /*fa1��һ���ļ�����ʾ���Դ�ļ�������ж�Ӧ���׵�ַ�����ｫcxָ��λ��д��fa1*/
		ch=' ';
		while(ch!=10)   /*10��ʾ���з����������з�����Ϊ����һ����*/
		{
            if(EOF==fscanf(fin,"%c",&ch))   /*EOF��ֵΪ-1*/
			{
				line[ll]=0; /*fscanf��ȡ�ļ����������÷���-1����ʾ�ļ�����*/
				break;
			}
			printf("%c",ch);
			fprintf(fa1,"%c",ch);   /*��ӡ�������ַ�����д���ļ�*/
			line[ll]=ch;    /*���������ַ�д�뻺����*/
			ll++;
		}
		printf("\n");
		fprintf(fa1,"\n");  /*��ӡ��д�뻻�з�*/
	}
	ch=line[cc];    /*��ȡһ�����������ַ���ch*/
	cc++;
	return 0;
}
 /*�ʷ���������ȡһ������
 */
int getsym()
{
	int i,j,k;
	while( ch==' '||ch==10||ch==9)  /*���Կո񡢻��к�TAB*/
	{
		getchdo;
	}
	if(ch>='a'&&ch<='z')    /*���ֻ�������a.z��ͷ*/
	{
		k=0;
		do{
			if(k<al)
			{
				a[k]=ch;    /*a[k]��ʱ���ţ������һ���ֽ����ڴ��0*/
				k++;
			}
			getchdo;
		}while(ch>='a'&&ch<='z'||ch>='0'&&ch<='9');
		a[k]=0;
		strcpy(id,a);   /*��a��ֵ���ݸ�id*/
		i=0;
		j=norw-1;   /*norwΪ�ؼ��ָ���*/
		do{
			k=(i+j)/2;  /*�۰���ҵĿ�ʼ��ȷ����ǰ�����Ƿ�Ϊ������*/
			if(strcmp(id,word[k])<=0)   /*�����ǰ�ı�ʶ��С��k��ָ�ı�����*/
			{
				j=k-1;
			}
			if(strcmp(id,word[k])>=0)
			{
				i=k+1;
			}

		}while(i<=j);
		if(i-1>j)
		{
			sym=wsym[k];    /*���ҳɹ����Ǳ�����*/
		}
		else
		{
			sym=ident;  /*����ʧ�ܣ��������ֻ�����*/
		}
	}
	else
	{
		if(ch>='0'&&ch<='9')    /*����Ƿ�Ϊ���֣���0..9��ͷ*/
		{
			k=0;    /*����λ��*/
			num=0;
			sym=number;
			do{
				num=10*num+ch-'0';  /*num * 10��������������ַ���ASCIIֵ��'0'��ASCIIֵ�õ���Ӧ����ֵ*/
				k++;
				getchdo;
			}while(ch>='0'&&ch<='9'); /*��ȡ���ֵ�ֵ*/
			k--;
			if(k>nmax)  /*�����ɵ�����λ������������������λ��*/
			{
				error(30);
			}
		}
		else
		{	if(ch=='%') 
			{
				sym=mod;
			}
			if(ch=='!') 
			{
				sym=not;
			}
			if(ch=='@')
			{
				sym=logic;
			}
			if(ch==':')             /*��⸳ֵ����*/
			{
				getchdo;
				if(ch=='=')
				{
					sym=becomes;
					getchdo;
				}
				else
				{
					sym=colon;     /*����ʶ��ķ���*/
					
				}
			}
			else
			{
				if(ch=='<')        /*���С�ڻ�С�ڵ��ڷ���*/
				{
					getchdo;
					if(ch=='=')
					{
						sym=leq;
						getchdo;
					}
					else
					{
						sym=lss;
					}
				}
				else
				{
					if(ch=='>')        /*�����ڻ���ڵ��ڷ���*/
					{
						getchdo;
						if(ch=='=')
						{
							sym=geq;
							getchdo;
						}
						else
						{
						    sym=gtr;
						}
					}
					else
                    {
                        if(ch=='+')
						{
							getchdo;
							if(ch=='=')
							{
								sym=addequal;
								getchdo;
							}
							else if(ch=='+'){
								sym=addadd;
								getchdo;
							}
							else{
								sym=plus;
							}
						}
						else
						{
							if(ch=='-')
							{
								getchdo;
								if(ch=='=')
								{
									sym=subequal;
									getchdo;
								}
								else if(ch=='-'){
									sym=subsub;
									getchdo;
								}
								else{
									sym=minus;
								}
							}
                            else
                            {
                                if(ch=='*')
                                {
                                    getchdo;
                                    if(ch=='=')
                                    {
                                        sym=timeseql;
                                        getchdo;
                                    }
                                    else
                                    {
                                        sym=times;
                                    }
                                }
                                else
                                {
                                    if(ch=='/')
                                    {
                                        getchdo;
                                        if(ch=='=')
                                        {
                                            sym=slasheql;
                                            getchdo;
                                        }
                                        else if(ch=='*')/*���ע��*/
                                        {
                                            getchdo;
                                           // printf("ע��/*%c",ch);
                                            while(1)
                                            {
                                                getchdo;
                                                //printf("%c",ch);
                                                if(ch=='*')
                                                {
                                                    getchdo;
                                                    if(ch=='/')
                                                    {
                                                       // printf("%cע��\n",ch);
                                                        break;
                                                    }
                                                    continue;
                                                }
                                            }
                                            getchdo;
                                        }
                                        else
                                        {
                                            sym=slash;
                                        }
                                    }
                                    else
                                    {
                                        sym=ssym[ch]; /*�����Ų�������������ʱ��ȫ�����յ��ַ��Ŵ���*/
                                        if(sym!=period)
                                        {
                                            getchdo;
                                        }
                                    }
                                }
                            }
						}
                    }
				}
			}
		}
	}
	return 0;
}
/*
*�������������
*
*x:instruction.f;
*y:instruction.l;
*z:instruction.a;
*/
int gen(enum fct x,int y,int z)
{
	if(cx>=cxmax)
	{
		printf("Program too long"); /*�������*/
		return -1;
	}
	code[cx].f=x;   /*�Ѵ���д��Ŀ���������ĵ�ǰcx��ָλ��*/
	code[cx].l=y;
	code[cx].a=z;
	cx++;
	return 0;
}
/*
*���Ե�ǰ�����Ƿ�Ϸ�
*
*��ĳһ���֣���һ����䣬һ�����ʽ����Ҫ����ʱʱ����ϣ����һ����������ĳ����
*���ò��ֵĺ�����ţ� test ���������⣬���Ҹ��𵱼�ⲻͨ��ʱ�Ĳ��ȴ�ʩ
*��������Ҫ���ʱָ����ǰ��Ҫ�ķ��ż��ϺͲ����õļ��ϣ���֮ǰδ��ɲ��ֵĺ��
*���ţ����Լ���ͨ��ʱ�Ĵ����
*
*S1��������Ҫ�ķ���
*s2:�������������Ҫ�ģ�����Ҫһ�������õļ���
*n:�����
*/
int test(bool* s1,bool* s2,int n)
{
    if(! inset(sym,s1))
    {
		error(n);
		/*����ⲻͨ��ʱ����ͣ��ȡ���ţ�ֱ����������Ҫ�ļ��ϻ򲹾ȵļ���*/
		while((! inset(sym,s1))&&(! inset(sym,s2)))
		{
			getsymdo;
		}
    }
    return 0;
}
/*
 *�����������
 *
 *lev:��ǰ�ֳ������ڲ�
 *tx:���ֱ�ǰβָ��
 *fsys:��ǰģ�������ż���
 */
int block(int lev,int tx,bool* fsys)
{
    int i;
    int dx;                         /*���ַ��䵽����Ե�ַ*/
    int tx0;                        /*������ʼtx*/
    int cx0;                        /*������ʼcx*/
    bool nxtlev[symnum];            /*���¼������Ĳ����У����ż��Ͼ�Ϊֵ�Σ�������ʹ������
                                      ʵ�֣����ݽ�������ָ�룬Ϊ��ֹ�¼������ı��ϼ�������
                                      ���ϣ������µĿռ䴫�ݸ��¼�����*/
    dx=3;/*�ó�ʼֵΪ3��ԭ���ǣ�ÿһ���ʼ��λ���������ռ䣬���ڴ�ž�̬��SL����̬��DL�ͷ��ص�ַRA */
    tx0=tx;                         /*��¼�������ֵĳ�ʼλ��*/
    table[tx].adr=cx;               /*���ű�ǰλ�ü��µ�ǰ�����Ŀ�ʼλ��*/
    gendo(jmp,0,0);                 /*����һ����תָ���תλ����ʱδ֪��0*/
    if(lev > levmax)
    {
		error(32);
    }
    do{
        if(sym==constsym)          /*�յ������������ţ���ʼ����������*/
        {
			getsymdo;
			do{
                constdeclarationdo(&tx,lev,&dx);    /*dx��ֵ�ᱻconstdeclaration�ı䣬ʹ��ָ��*/
                while(sym==comma)
                {
                   getsymdo;
                   constdeclarationdo(&tx,lev,&dx);
                }
                if(sym==semicolon)
                {
                  getsymdo;
				}
				else
				{
					error(5);   /*©���˶��Ż��߷ֺ�*/
				}
			}while(sym==ident);
		}
		if(sym==varsym) /*�յ������������ţ���ʼ�����������*/
		{
			getsymdo;
			do{
				vardeclarationdo(&tx,lev,&dx);
				while(sym==comma)
				{
					getsymdo;
					vardeclarationdo(&tx,lev,&dx);
				}
				if(sym==semicolon)
				{
					getsymdo;
				}
				else
				{
					error(5);
				}
			}while(sym==ident);
		}
		while(sym==procsym) /*�յ������������ţ���ʼ�����������*/
		{
			getsymdo;
			if(sym==ident)
			{
				enter(procedur,&tx,lev,&dx);    /*��¼��������*/
				getsymdo;
			}
			else
			{
				error(4);   /*procedure��ӦΪ��ʶ��*/
			}
			if(sym==semicolon)
			{
				getsymdo;
			}
			else
			{
				error(5);   /*©���˷ֺ�*/
			}
			memcpy(nxtlev,fsys,sizeof(bool)*symnum);
			nxtlev[semicolon]=true;
			if(-1==block(lev+1,tx,nxtlev))
			{
				return -1;  /*�ݹ����*/
			}
            if(sym==semicolon)
            {
                getsymdo;
                memcpy(nxtlev,statbegsys,sizeof(bool)*symnum);
                nxtlev[ident]=true;
                nxtlev[procsym]=true;
                testdo(nxtlev,fsys,6);  /*��鵱ǰ�����Ƿ�Ϸ������Ϸ�����fsys�ָ��﷨����ͬʱ��6�Ŵ�*/
             }
             else
             {
                 error(5);                       /*©���˷ֺ�*/
             }
        }
        memcpy(nxtlev,statbegsys,sizeof(bool)*symnum);
        nxtlev[ident]=true;
        nxtlev[period]=true;
        testdo(nxtlev,declbegsys,7);    /*��鵱ǰ״̬�Ƿ�Ϸ������Ϸ�����������ʼ����������ָ�����7�Ŵ�*/
    }while(inset(sym,declbegsys));                /*ֱ��û����������*/
    code[table[tx0].adr].a=cx;     /*��ǰ�����ɵ���ת������תλ�øĳɵ�ǰλ�ã���ʼ���ɵ�ǰ���̴���*/
    table[tx0].adr=cx;                          /*��ǰ���̴����ַ*/
    table[tx0].size=dx;                         /*����������ÿ����һ�����������dx����1,���������Ѿ�����,dx���ǵ�ǰ�������ݵ�size*/
    cx0=cx;
    gendo(inte,0,dx);                          /*���ɷ����ڴ���룬����dx���ռ�*/
    if(tableswitch)                            /*������ֱ�*/
    {
        printf("TABLE:\n");
        if(tx0+1>tx)
        {
			printf("NULL\n");
        }
        for(i=tx0+1;i<=tx;i++)
        {
            switch(table[i].kind)
            {
                case constant:
					printf("%d const %s",i,table[i].name);
					printf("val=%d\n",table[i].val);
					fprintf(fas,"%d const %s",i,table[i].name);
					fprintf(fas,"val=%d\n",table[i].val);
                    break;
                case variable:
                    printf("%d var %s",i,table[i].name);
                    printf(" lev=%d addr=%d\n",table[i].level,table[i].adr);
                    fprintf(fas,"%d var %s",i,table[i].name);
                    fprintf(fas,"lev=%d addr=%d\n",table[i].level,table[i].adr);
                    break;
                case procedur:
                    printf("%d proc%s",i,table[i].name);
                    printf("lev=%d addr=%d size=%d\n",table[i].level,table[i].adr,table[i].size);
					fprintf(fas,"%d proc%s",i,table[i].name);
					fprintf(fas,"lev=%d adr=%d size=%d \n",table[i].level,table[i].adr,table[i].size);
					break;
				case arrays:
					printf("%d array %s ", i, table[i].name);
					printf("lev=%d addr=%d size=%d\n", table[i].level, table[i].adr, table[i].size);
					fprintf(fas, "%d array %s ", i, table[i].name);
					fprintf(fas, "lev=%d addr=%d size=%d\n", table[i].level, table[i].adr, table[i].size);
			}
		}
		printf("\n");
	}
	/*���������Ϊ�ֺŻ�end*/
	memcpy(nxtlev,fsys,sizeof(bool)*symnum);    /*ÿ��������ż��Ͷ������ϲ������ż��ͣ��Ա㲹��*/
	nxtlev[semicolon]=true;
	nxtlev[endsym]=true;
	statementdo(nxtlev,&tx,lev);
	gendo(opr,0,0);     /*ÿ�����̳��ڶ�Ҫʹ�õ��ͷ����ݶ�����*/
	memset(nxtlev,0,sizeof(bool)*symnum);   /*�ֳ���û�в��ȼ���*/
	test(fsys,nxtlev,8);                  /*�����������ȷ��*/
	listcode(cx0);                          /*�������*/
	return 0;
}
/*
*�����ֱ��м���һ��
*
*k:��������const,var or procedure
*ptx:���ֱ�βָ���ָ�룬Ϊ�˿��Ըı����ֱ�βָ�����ֵ
*lev:�������ڵĲ�Σ��Ժ����е�lev��������
*pdx:Ϊ��ǰӦ����ı�������Ե�ַ�������Ҫ����1
*/
void enter (enum object k,int *ptx,int lev, int *pdx)
{
	(*ptx)++;
	strcpy(table[(*ptx)].name,id);       /*ȫ�ֱ���id���Ѵ��е�ǰ���ֵ�����*/
	table[(*ptx)].kind=k;
	switch(k)
	{
		case constant:               /*��������*/
			if (num>amax)
			{
				error(31);          /*��Խ��*/
				num=0;
			}
			table[(*ptx)].val=num;  /*���ǺϷ�����ֵ���͵�½�����ű�*/
			break;
		case variable:                /*��������*/
			table[(*ptx)].level=lev;    /*�����������Ĳ�κ�*/
			table[(*ptx)].adr=(*pdx);   /*�������ڵ�ǰ���е�ƫ����*/
			(*pdx)++;
			break;                  /*��������*/
		case procedur:
			table[(*ptx)].level=lev;    /*��¼������������ڲ��*/
			break;
		case arrays:     /* ������,���м�¼�½�� */
			table[(*ptx)].level = lev;
			table[(*ptx)].adr = (*pdx);
			table[(*ptx)].data = g_arrBase;
			table[(*ptx)].size = g_arrSize;
			*pdx = (*pdx)+g_arrSize;
			break;
	}

}
/*
 *�������ֵ�λ��
 *�ҵ��򷵻������ֱ��е�λ�ã����򷵻�0
 *
 *idt: Ҫ���ҵ�����
 *tx:����ǰ���ֱ�βָ��
 */
int position(char *  idt,int  tx)
{
	int i;
	strcpy(table[0].name,idt);  /*�Ȱ�id�������ֱ�0��λ��*/
	i=tx;
	while(strcmp(table[i].name,idt)!=0)
	{
		i--;
	}
	return i;
}
/*
 *������������
 */
int constdeclaration(int *  ptx,int lev,int *  pdx)
{
	if(sym==ident)
	{
		getsymdo;
		if(sym==eql ||sym==becomes)
		{
			if(sym==becomes)
			{
				error(1);                  /*��=д�����ˣ�=*/
			}
			getsymdo;
			if(sym==number)
			{
				enter(constant,ptx,lev,pdx);    /*�����������½�����ֱ�*/
				getsymdo;
			}
			else
			{
				error(2);                  /*����˵��=��Ӧ������*/
			}
		}
		else
		{
			error(3);                      /*����˵����ʶ��Ӧ��=*/
		}
	}
	else
	{
		error(4);                         /*const��Ӧ�Ǳ�ʶ*/
	}
	return 0;
}
/*
 *
 */
//������������
int vardeclaration(int * ptx, int lev, int * pdx)
{
	int arrayRet=-1;
    if (sym==ident)
    {
        arrayRet=arraydeclaration(ptx,lev,pdx); /* ���ж����� */
        switch(arrayRet)
        {
        case 1:
            enter(arrays,ptx,lev,pdx); // ��д������
			getsymdo;
            break;
        case 0:
            enter(variable,ptx,lev,pdx); // ��д���ֱ�
            //getsymdo;
            break;
        default:
            return -1;  /* ���鶨��������� */
        }
    }
    else
    {
        error(4);   /* var��Ӧ�Ǳ�ʶ */
    }
    return 0;
}
 /*
  *���Ŀ������嵥
  */
void listcode(int cx0)
{
	int i;
   	if (listswitch)
   	{
   		for(i=cx0;i<cx;i++)
   	 	{
   	 		printf("%d %s %d %d\n",i,mnemonic[code[i].f],code[i].l,code[i].a);
   	 		fprintf(fa,"%d %s %d %d\n",i,mnemonic[code[i].f],code[i].l,code[i].a);
   	    }
   	 }
}
/*
*��䴦��
*/
int statement(bool* fsys,int * ptx,int lev)
{
	int i,cx1,cx2,cx3,cx4,cx5;;
   	bool nxtlev[symnum];
   	if(sym==ident)              /*׼�����ո�ֵ��䴦��*/
   	{
		i=position(id,*ptx);    /*������������Ӧ��tableλ�� */
   	 	if(i==0)
   	 	{
   	 		error(11);          /*����δ�ҵ� */
   	 	}
   	 	else
   	 	{
   	 		if((table[i].kind != variable)&&(table[i].kind != arrays))
   	 		{
   	 		 	error(12);          /*��ֵ����ʽ���� */
   	 		 	i=0;               /*i��0��Ϊ�����־*/
   	 		}
   	 		else
   	 		{
   	 			
				enum fct fct1 = sto;
				switch(table[i].kind)
				{
				case arrays:
					arraycoefdo(fsys, ptx, lev);
					fct1 = sta;  /* ���鱣��,Ҫ���һ��ջ */
							/* go through */
				case variable:
				{	getsymdo;
					
   	 				if(sym==becomes)
   	 				{
   	 					getsymdo;
						memcpy(nxtlev,fsys,sizeof(bool)* symnum);
						expressiondo(nxtlev,ptx,lev);   /*����ֵ�����Ҳ���ʽ*/
						if(i!=0)
						{                                   /*expression��ִ��һϵ��ָ������ս��
															���ᱣ����ջ����ִ��sto������ɸ�ֵ*/
							gendo(fct1,lev-table[i].level,table[i].adr);
						}
   	 				}
					else if(sym==mod)       /*��⵽%����*/
					{
						getsymdo;
						//����a%b = a - (a/b)*b 
						//��a��ֵ��ջ
						gendo(lod,lev-table[i].level,table[i].adr);  /*�ҵ�������ַ������ֵ��ջ*/
					
						if(sym==semicolon)
						{
							getsymdo;
						}
						memcpy(nxtlev,fsys,sizeof(bool)* symnum);
						expressiondo(nxtlev,ptx,lev);
						//��a��b��ֵ���
						gendo(opr,0,5);
						//��ȡb��ֵ��ջ��
						gendo(lod,lev-table[i+1].level,table[i+1].adr);
						gendo(opr,0,4);
						gendo(lod,lev-table[i].level,table[i].adr);
						gendo(opr,0,3);
						gendo(opr,0,1);
		
						if(i!=0)
						{
							gendo(fct1,lev-table[i].level,table[i].adr);
						}
					}
					else if(sym==addequal)       /*��⵽+=����*/
					{
						getsymdo;
						gendo(lod,lev-table[i].level,table[i].adr);  /*�ҵ�������ַ������ֵ��ջ*/
						if(sym==semicolon)
						{
							getsymdo;
							printf("+=����ֱ�Ӹ��˷ֺ�");
						}
						memcpy(nxtlev,fsys,sizeof(bool)* symnum);
						expressiondo(nxtlev,ptx,lev);
						gendo(opr,0,2);
						if(i!=0)
						{
							gendo(fct1,lev-table[i].level,table[i].adr);
						}
					}
					else if(sym==subequal)  /*��⵽-=����*/
					{
							getsymdo;
							gendo(lod,lev-table[i].level,table[i].adr);    /*�ҵ�������ַ������ֵ��ջ*/
							if(sym==semicolon)
							{
								getsymdo;
							}
							memcpy(nxtlev,fsys,sizeof(bool)* symnum);
							expressiondo(nxtlev,ptx,lev);
							gendo(opr,0,3);
							if(i!=0)
							{
								gendo(fct1,lev-table[i].level,table[i].adr);
							}
					}
					else if(sym==timeseql)   /*��⵽*=����*/
					{
						getsymdo;
						gendo(lod,lev-table[i].level,table[i].adr);    /*�ҵ�������ַ������ֵ��ջ*/
						if(sym==semicolon)
						{
							getsymdo;
						}
						memcpy(nxtlev,fsys,sizeof(bool)* symnum);
						expressiondo(nxtlev,ptx,lev);
						gendo(opr,0,4);
						if(i!=0)
						{
							gendo(fct1,lev-table[i].level,table[i].adr);
						}
					}
					else if(sym==slasheql)  /*��⵽/=����*/
					{
						getsymdo;
						gendo(lod,lev-table[i].level,table[i].adr);    /*�ҵ�������ַ������ֵ��ջ*/
						if(sym==semicolon)
						{
							getsymdo;
						}
						memcpy(nxtlev,fsys,sizeof(bool)* symnum);
						expressiondo(nxtlev,ptx,lev);
						gendo(opr,0,5);
						if(i!=0)
						{
							gendo(fct1,lev-table[i].level,table[i].adr);
						}
					}
					else if(sym==addadd)    /*��⵽����++����*/
					{
						getsymdo;
						if(i!=0)
						{
							gendo(lod,lev-table[i].level,table[i].adr);
							gendo(lit,0,1);
							gendo(opr,0,2);
							gendo(fct1,lev-table[i].level,table[i].adr);
						}
					}
					else if(sym==subsub)  /*��⵽����--����*/
					{
						getsymdo;
						if(i!=0)
						{
							gendo(lod,lev-table[i].level,table[i].adr);
							gendo(lit,0,1);
							gendo(opr,0,3);
							gendo(fct1,lev-table[i].level,table[i].adr);
						}
					}
					else
   	 				{
   	 			 		error(13);      /*û�м�⵽��ֵ����*/
   	 				}
				}
					break;
				default:
					error(12);
					i=0;
					break;
				}
   	 		}
        }
    }
    else
    {

		if(sym==logic) /*����߼�ȡ������@*/
		{
			getsymdo;
			if(sym==ident)
			{
				 i=position(id,*ptx);
                 if(i==0)
                 {
                    error(11);
                 }
				 else
				 {
					 if(table[i].kind!=variable)
                    {  
                        error(12);
                        i=0;
                    }
                    else
                    { 
						getsymdo;
						gendo(lod,lev-table[i].level,table[i].adr);
						gendo(lit,0,0);
						gendo(opr,0,8);
						gendo(sto,lev-table[i].level,table[i].adr);      
                           
                    }
					
				 }
			}
		
		}

		if(sym==not) 
		{
			getsymdo;
			if(sym==ident)
			{
				 i=position(id,*ptx);
                 if(i==0)
                 {
                    error(11);
                 }
				 else
				 {
					 if(table[i].kind!=variable)
                    {  
                        error(12);
                        i=0;
                    }
                    else
                    { 
						getsymdo;
						gendo(lod,lev-table[i].level,table[i].adr);
						gendo(opr,0,1);
						gendo(sto,lev-table[i].level,table[i].adr);      
                           
                    }
					
				 }
			}
		
		}
       if(sym==addadd) /*��⵽ǰ��++����*/
        {
            getsymdo;
            if(sym==ident)  /*��������Ǳ���*/
            {
                 i=position(id,*ptx);
                 if(i==0)
                 {
                    error(11);
                 }
                 else
                 {
                    if(table[i].kind!=variable)
                    {   /*++��û������������*/
                        error(12);
                        i=0;
                    }
                    else
                    { /*++������������������м����*/
                            getsymdo;
                            gendo(lod,lev-table[i].level,table[i].adr);/*��ȡֵ��ջ��*/
                            gendo(lit,0,1);              /*��ֵ��ջ*/
                            gendo(opr,0,2);     /*�ӷ�����+1��ջ���Ӵ�ջ��*/
                            gendo(sto,lev-table[i].level,table[i].adr);/*��ջȡֵ���ڴ�*/
                    }
                }
            }
        }
        else if(sym==subsub)    /*��⵽ǰ��--����*/
         {
            getsymdo;
            if(sym==ident)    /*��������Ǳ���*/
            {
                i=position(id,*ptx);
                if(i==0)
                {
                    error(11);
                }
                else
                {
                    if(table[i].kind!=variable)   /*--��û������������*/
                    {
                        error(12);
                        i=0;
                    }
                    else        /*--������������������м����*/
                    {
                        if(table[i].kind==variable) /*�������*/
                        {
                            getsymdo;
                            gendo(lod,lev-table[i].level,table[i].adr);/*��ȡֵ��ջ��*/
                            gendo(lit,0,1);       /*��ֵΪ��ջ*/
                            gendo(opr,0,3);      /*��������-1��ջ������ջ��*/
                            gendo(sto,lev-table[i].level,table[i].adr);/*��ջȡֵ���ڴ�*/
                        }
                    }
                }
            }
         }
       
         if(sym == forsym)
		{
			getsymdo;
			if(sym != lparen)  error(34);//û�������ų���
			else 
			{
				getsymdo;
				statementdo(nxtlev, ptx, lev);  //S1����
				if(sym != semicolon)  error(10); //���ȱ�ٷֺų���
				else
				{
					cx1=cx;
					getsymdo;
					conditiondo(nxtlev, ptx, lev);   //E����
					if(sym != semicolon)  error(10);  //���ȱ�ٷֺų���
					else 
					{	cx2=cx;
						gendo(jpc,0,0);
						cx3=cx;
						gendo(jmp,0,0);
						getsymdo;
						cx4=cx;
						statementdo(nxtlev, ptx, lev);	//S2����
						if(sym != rparen)  error(22);  //ȱ�������ų���
						else 
						{
							gendo(jmp,0,cx1);
							getsymdo;
							cx5=cx;
							statementdo(nxtlev, ptx, lev);  //S3����
							code[cx3].a=cx5;
							gendo(jmp,0,cx4);
							code[cx2].a=cx;
						}
					}
				}
			}
        }
        else if(sym==repeatsym)
        {
            cx1=cx;	 /*���浱ǰָ���ַ*/
            getsymdo;
            memcpy(nxtlev,fsys,sizeof(bool)*symnum);
            nxtlev[untilsym]=true;
            statementdo(fsys,ptx,lev);
            if(sym==semicolon)
            {
                getsymdo;
                if(sym==untilsym)
                {
                    getsymdo;
                    conditiondo(fsys,ptx,lev);
                    gendo(jpc,0,cx1);/*��condition�����cx1Ϊrepeat��ѭ������λ�ã�����Ϊ��ʱһֱѭ��*/
                }
            }
            else
            {
                error(5);
            }
        }
        else
        {
            if(sym==readsym)        /*׼������read��䴦��*/
            {
                getsymdo;
                if(sym!=lparen)
                {
                   error(34);       /*��ʽ����Ӧ��������*/
                }
                else
                {
                    do{
                        getsymdo;
                        if(sym==ident)
                        {
                            i=position(id, *ptx);       /*����Ҫ���ı���*/
                        }
                        else
                        {
                            i=0;
                        }
                        if(i==0)
                        {
                            error(35);              /*read()��Ӧ���������ı�����*/
                        }
                        else
                        {
                            gendo(opr,0,16);        /*��������ָ���ȡֵ��ջ��*/
                            gendo(sto,lev-table[i].level,table[i].adr);	/* ���浽����*/
                        }
                        getsymdo;
                    }while (sym==comma);	/*һ��read���ɶ�������� */
                }
                if(sym!=rparen)
                {
                    error(33);			/* ��ʽ����Ӧ��������*/
                    while(!inset(sym,fsys)) /* �����ȣ�ֱ���յ��ϲ㺯���ĺ������*/
                    {
                        getsymdo;
                    }
                }
                else
                {
                    getsymdo;
                }
            }
            else
            {
                if(sym==writesym)			/* ׼������write��䴦����read����*/
                {
                    getsymdo;
                    if(sym==lparen)
                    {
                        do{
                            getsymdo;
                            memcpy(nxtlev,fsys,sizeof(bool)*symnum);
                            nxtlev[rparen]=true;
                            nxtlev[comma]=true;		/* write�ĺ������Ϊ��or��*/
                            expressiondo(nxtlev,ptx,lev);   /* ���ñ��ʽ�����˴���read��ͬ��readΪ��������ֵ*/
                            gendo(opr,0,14);    /* �������ָ����ջ����ֵ*/
                        }while(sym==comma);
                        if(sym!=rparen)
                        {
                            error(33);  /* write()��ӦΪ�������ʽ*/
                        }
                        else
                        {
                            getsymdo;
                        }
                    }
                    gendo(opr,0,15);		/* �������*/
                }
                else
                {
                    if(sym==callsym)		/* ׼������call��䴦��*/
                    {
                        getsymdo;
                        if(sym!=ident)
                        {
                            error(14);    /*call��ӦΪ��ʶ��*/
                        }
                        else
                        {
                            i=position(id,*ptx);
                            if(i==0)
                            {
                                error(11); /*����δ�ҵ�*/
                            }
                            else
                            {
                                if(table[i].kind==procedur)     /*��������ʶ��Ϊһ��������*/
                                {
                                    gendo(cal,lev-table[i].level,table[i].adr);  /*����callָ��*/
                                }
                                else
                                {
                                    error(15);  /*call���ʶ��ӦΪ����*/
                                }
                            }
                            getsymdo;
                        }
                    }
                    else
                    {
                        if(sym==ifsym)         /*׼������if��䴦��*/
                        {
                            getsymdo;
                            memcpy(nxtlev,fsys,sizeof(bool)*symnum);
                            nxtlev[thensym]=true;
                            nxtlev[dosym]=true;    /*�������Ϊthen��do*/
                            conditiondo(nxtlev,ptx,lev);   /*�������������߼����㣩����*/
                            if(sym==thensym)
                            {
                                getsymdo;
                            }
                            else
                            {
                                error(16);             /*ȱ��then*/
                            }
                            cx1=cx;                  /*���浱ǰָ���ַ*/
                            gendo(jpc,0,0);            /*����������תָ���ת��ַ��д0*/
                            statementdo(fsys,ptx,lev);   /*����then������*/
                            if(sym==semicolon)
                            {
                                getsymdo;
                                if(sym==elsesym)/*then�������else*/
                                {
                                    getsymdo;
                                    cx2=cx;
                                    code[cx1].a=cx+1;   /*cxΪ��ǰ��ָ���ַ��
                                                      cx+1��Ϊthen���ִ�к��else����λ�ã������ַ*/
                                    gendo(jmp,0,0);
                                    statementdo(fsys,ptx,lev);
                                    code[cx2].a=cx;     /*��statement�����cxΪelse�����ִ��
                                                        ���λ�ã�������ǰ��δ������ת��ַ�������ַ*/
                                }
                                else
                                {
								
                                    code[cx1].a=cx;         /*��statement�����cxΪthen�����ִ��
                                                        ���λ�ã�������ǰ��δ������ת��ַ*/
                                }
                            }
                            else
                            {
                                error(5);
                            }
                        }
                        else
                        {
                            if(sym==beginsym)   /*׼�����ո�����䴦��*/
                            {
                                getsymdo;
                                memcpy(nxtlev,fsys,sizeof(bool)*symnum);
                                nxtlev[semicolon]=true;
                                nxtlev[endsym]=true;/*�������Ϊ�ֺŻ�end*/
                                /*ѭ��������䴦������ֱ����һ�����Ų�����俪ʼ���Ż��յ�end*/
                                statementdo(nxtlev,ptx,lev);
                                while(inset(sym,statbegsys)||sym==semicolon)
                                {
                                    if(sym==semicolon)
                                    {
                                        getsymdo;
                                    }
                                    else
                                    {
                                        error(10);/*ȱ�ٷֺ�*/
                                    }
                                    statementdo(nxtlev,ptx,lev);
                                }
                                if(sym==endsym)
                                {
                                    getsymdo;
                                }
                                else
                                {
                                    error(17); /*ȱ��end��ֺ�*/
                                }
                            }
                            else
                            {
                                if(sym==whilesym)/*׼������while��䴦��*/
                                {
                                    cx1=cx;        /*�����ж�����������λ��*/
                                    getsymdo;
                                    memcpy(nxtlev,fsys,sizeof(bool)*symnum);
                                    nxtlev[dosym]=true; /*�������Ϊdo*/
                                    conditiondo(nxtlev,ptx,lev);  /*������������*/
                                    cx2=cx;       /*����ѭ����Ľ�������һ��λ��*/
                                    gendo(jpc,0,0); /*����������ת��������ѭ���ĵ�ַδ֪*/
                                    if(sym==dosym)
                                    {
                                        getsymdo;
                                    }
                                    else
                                    {
                                        error(18);      /*ȱ��do*/
                                    }
                                    statementdo(fsys,ptx,lev); /*ѭ����*/
                                    gendo(jmp,0,cx1);   /*��ͷ�����ж�����*/
                                    code[cx2].a=cx;   /*��������ѭ���ĵ�ַ����if����*/
                                }
                                else
                                {
                                    memset(nxtlev,0,sizeof(bool)*symnum);/*�������޲��ȼ���*/
                                    testdo(fsys,nxtlev,19);/*�������������ȷ��*/
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    return 0;
}
/*
*���ʽ����
*/
int expression(bool*fsys,int*ptx,int lev)
{
	enum symbol addop;              /*���ڱ���������*/
	bool nxtlev[symnum];
	if(sym==plus||sym==minus)        /*��ͷ�������ţ���ʱ��ǰ���ʽ������һ�����Ļ򸺵���*/
	{
		addop=sym;                 /*���濪ͷ��������*/
		getsymdo;
		memcpy(nxtlev,fsys,sizeof(bool)*symnum);    /*��fsys����n���ֽڵ�nxtlev*/
		nxtlev[plus]=true;
		nxtlev[minus]=true;
		termdo(nxtlev,ptx,lev);         /*������*/
		if(addop==minus)
		{
			gendo(opr,0,1);           /*�����ͷΪ��������ȡ��ָ��*/
		}
	}
	else                             /*��ʱ���ʽ��������ļӼ�*/
	{
		memcpy(nxtlev,fsys,sizeof(bool)*symnum);
		nxtlev[plus]=true;
		nxtlev[minus]=true;
		//nxtlev[mod]=true;
		termdo(nxtlev,ptx,lev);          /*������*/
	}
	while(sym==plus||sym==minus)
	{
		addop=sym;
		getsymdo;
		memcpy(nxtlev,fsys,sizeof(bool)*symnum);
		nxtlev[plus]=true;
		nxtlev[minus]=true;
		termdo(nxtlev,ptx,lev);              /*������*/
		if(addop==plus)
		{
			gendo(opr,0,2);                /*���ɼӷ�ָ��*/
		}
		else
		{
			 gendo(opr,0,3);               /*���ɼ���ָ��*/
		}
	}
	return 0;
}
/*
*���
*/
int term(bool*fsys,int *ptx,int lev)
{
    enum symbol mulop;               /*���ڱ���˳�������*/
    bool nxtlev[symnum];
    memcpy(nxtlev,fsys,sizeof(bool)*symnum) ;
    nxtlev[times]=true;
    nxtlev[slash]=true;
//	nxtlev[mod]=true;
    factordo(nxtlev,ptx,lev);            /*��������*/
    while(sym==times||sym==slash)
    {
		mulop=sym;
        getsymdo;
        factordo(nxtlev,ptx,lev);
        if(mulop==times)
        {
            gendo(opr,0,4);          /*���ɳ˷�ָ��*/
        }
        else
        {
            gendo(opr,0,5);           /*���ɳ���ָ��*/
        }
    }
     return 0;
}
/*
*���Ӵ���
*/
int factor(bool*fsys,int *ptx,int lev)
 {
    int i;
    bool nxtlev[symnum];
    testdo(facbegsys,fsys,24);           /*������ӵĿ�ʼ���ú�*/
    while(inset(sym,facbegsys))         /*ѭ��ֱ���������ӿ�ʼ����*/
    {
        if(sym==ident)               /*����Ϊ�������߱���*/
        {
            i=position(id,*ptx);        /*��������*/
            if(i==0)
            {
                error(11);            /*��ʶ��δ����*/
            }
            else
            {
				switch(table[i].kind)
				{
					case constant:                               /*����Ϊ����*/
						gendo(lit,0,table[i].val);                   /*ֱ�Ӱѳ�����ֵ��ջ*/
						break;
					case variable:                               /*����Ϊ����*/
						gendo(lod,lev-table[i].level,table[i].adr);      /*����lodָ�
                            ��λ�ھ��뵱ǰ��level�Ĳ��ƫ�Ƶ�ַΪadr�ı�����ֵ�ŵ�ջ��*/
						break;
					case procedur:                               /*����Ϊ����*/
						error(21);                               /*����Ϊ����*/
						break;
					case arrays:     /* ����Ϊ������ */
						arraycoefdo(fsys,ptx,lev);
						gendo(lda,lev-table[i].level,table[i].adr);   /* �ҵ�������ַ������ֵ��ջ */
						break;
				}
			}
			getsymdo;
			if(sym==addadd)     /*���ӳ���b:=a++����*/
            {
                gendo(lit,lev-table[i].level,1);            /*��ֵ��ջ*/
                gendo(opr,lev-table[i].level,2); /*�ӷ�����+1��ջ���Ӵ�ջ��*/
                gendo(sto,lev-table[i].level,table[i].adr); /*��ջȡֵ���ڴ�*/
                gendo(lod,lev-table[i].level,table[i].adr); /*ȡֵ��ջ��*/
                gendo(lit,0,1);
                gendo(opr,0,3);                             /*ջ��ֵ��*/
                getsymdo;
            }
            else if(sym==subsub)  /*���ӳ���b:=a--����*/
            {
                gendo(lit,lev-table[i].level,1);            /*��ֵ��ջ*/
                gendo(opr,lev-table[i].level,3);            /*��������-1��ջ������ջ��*/
                gendo(sto,lev-table[i].level,table[i].adr); /*��ջȡֵ���ڴ�*/
                gendo(lod,lev-table[i].level,table[i].adr);
                gendo(lit,0,1);
                gendo(opr,0,2);                             /*ջ��ֵ��*/
                getsymdo;
            }
		}
	
		else if(sym==logic) 
		{
	
			getsymdo;
            if(sym==ident)
            {
				//getsymdo;
                i=position(id,*ptx);
                if(i==0)
                {
                    error(11);
                }
				else 
				{
					
                    gendo(lod,lev-table[i].level,table[i].adr);/*��ȡֵ��ջ��*/
					gendo(lit,0,0);
                    gendo(opr,0,8);/*ջ���ʹ�ջ����Ƚ�*/
                    gendo(sto,lev-table[i].level,table[i].adr);/*��ջȡֵ���ڴ�*/
                 
				}
			}
			
		}
		else if(sym==not) 
		{
			getsymdo;
            if(sym==ident)
            {
				//getsymdo;
                i=position(id,*ptx);
                if(i==0)
                {
                    error(11);
                }
				else 
				{
                    gendo(lod,lev-table[i].level,table[i].adr);/*��ȡֵ��ջ��*/
                    gendo(opr,0,1);/*ȡ��*/
                    gendo(sto,lev-table[i].level,table[i].adr);/*��ջȡֵ���ڴ�*/
                 
				}
			}
			
		}
		else if(sym==addadd)    /*���ӳ���b:=++a����*/
        {
            getsymdo;
            if(sym==ident)
            {
                getsymdo;
                i=position(id,*ptx);
                if(i==0)
                {
                    error(11);
                }
                else
                {
                    if(table[i].kind==variable)  /*����*/
                    {                     /*�ȼӺ�����a*/
                        gendo(lod,lev-table[i].level,table[i].adr);/*��ȡֵ��ջ��*/
                        gendo(lit,0,1);/*��ֵ��ջ*/
                        gendo(opr,0,2);/*�ӷ�����+1��ջ���Ӵ�ջ��*/
                        gendo(sto,lev-table[i].level,table[i].adr);/*��ջȡֵ���ڴ�*/
                        gendo(lod,lev-table[i].level,table[i].adr);/*ȡֵ��ջ��*/
                    }
                }
            }
        }
        else if(sym==subsub)    /*���ӳ���b:=--a����*/
        {
            getsymdo;
            if(sym==ident)
            {
                getsymdo;
                i=position(id,*ptx);
                if(i==0)
                {
                    error(11);
                }
                else
                {
                    if(table[i].kind==variable)  /*����*/
                    {              /*�ȼ�������a*/
                        gendo(lod,lev-table[i].level,table[i].adr);/*��ȡֵ��ջ��*/
                        gendo(lit,0,1);                /*��ֵ��ջ*/
                        gendo(opr,0,3);                /*��������-1��ջ������ջ��*/
                        gendo(sto,lev-table[i].level,table[i].adr);/*��ջȡֵ���ڴ�*/
                        gendo(lod,lev-table[i].level,table[i].adr); /*ȡֵ��ջ��*/
                    }
                }
            }
        }
        else
        {
            if(sym==number)              /*����Ϊ��*/
            {
                if(num>amax)
                {
                    error(31);
                    num=0;
                }
                gendo(lit,0,num);
                getsymdo;
            }
            else
            {
                if(sym==lparen)            /*����Ϊ���ʽ*/
                {
                    getsymdo;
                    memcpy(nxtlev,fsys,sizeof(bool)*symnum);
                    nxtlev[rparen]=true;
                    expressiondo(nxtlev,ptx,lev);
                    if(sym==rparen)
                    {
                        getsymdo;
                    }
                    else
                    {
                        error(22);         /*ȱ��������*/
                    }
                }
                testdo(fsys,facbegsys,23);    /*���Ӻ��зǷ�����*/
            }
}
    }
	return 0;
}
/*
 ��������*/
int condition(bool* fsys,int* ptx,int lev)
{
    enum symbol relop;
    bool nxtlev[symnum];
    if(sym==oddsym)                        /*׼������odd���㴦��*/
   	{
		getsymdo;
		expressiondo(fsys,ptx,lev);
		gendo(opr,0,6);                     /*����oddָ��*/
    }
    else
    {
		memcpy(nxtlev,fsys,sizeof(bool)*symnum);
		nxtlev[eql]=true;  /*=*/
		nxtlev[neq]=true;  /*#*/
		nxtlev[lss]=true;  /*<*/
		nxtlev[leq]=true;  /*<=*/
		nxtlev[gtr]=true;  /*>*/
		nxtlev[geq]=true;  /*>=*/
		expressiondo(nxtlev,ptx,lev);
		if(sym!=eql&&sym!=neq&&sym!=lss&&sym!=leq&&sym!=gtr&&sym!=geq)
		{
			error(20);
		}
		else
		{
			relop=sym;
			getsymdo;
			expressiondo(fsys,ptx,lev);
			switch(relop)
			{
				case eql:
					gendo(opr,0,8); /*�Ⱥţ�����8���е�ָ��*/
					break;
				case neq:
					gendo(opr,0,9); /*���Ⱥţ�����9���в���ָ��*/
					break;
				case lss:
					gendo(opr,0,10); /*С�ںţ�����10����С��ָ��*/
					break;
				case geq:
					gendo(opr,0,11); /*���ڵ��ںţ�����11���в�С��ָ��*/
					break;
				case gtr:
					gendo(opr,0,12); /*���ںţ�����12���д���ָ��*/
					break;
				case leq:
					gendo(opr,0,13); /*С�ڵ��ںţ�����13���в�����ָ��*/
					break;
			}

		}
    }
    return 0;
}
/*���ͳ���*/
void interpret()
{
	int p,b,t;                /*ָ��ָ�룬ָ���ַ��ջ��ָ��*/
	struct instruction i;       /*��ŵ�ǰָ��*/
	int s[stacksize];          /*ջ*/
	printf("start pl0\n");
	t=0;
	b=0;
	p=0;
	s[0]=s[1]=s[2]=0;
	do{
		i=code[p];         /*����ǰָ��*/
		p++;
		switch(i.f)
		{
			case lit:        /*��a��ֵȡ��ջ��*/
				s[t]=i.a;
				t++;
				break;
			case opr:       /*���֡��߼�����*/
				switch(i.a)
				{
					case 0:         /*�ͷ��ڴ�*/
						t=b;
						p=s[t+2];
						b=s[t+1];
						break;
					case 1:
						s[t-1]=-s[t-1]; /*ȡ��*/
						break;
					case 2:
						t--;
						s[t-1]=s[t-1]+s[t];/*�ӷ�*/
						break;
					case 3:
						t--;
						s[t-1]=s[t-1]-s[t];/*����*/
						break;
					case 4:
						t--;
						s[t-1]=s[t-1]*s[t];/*�˷�*/
						break;
					case 5:
						t--;
						s[t-1]=s[t-1]/s[t];/*����*/
             			break;
					case 6:
						s[t-1]=s[t-1]%2;  /*��ż�жϣ�����Ϊ�棬ż��Ϊ��*/
						break;
					case 8:
						t--;
						s[t-1]=(s[t-1]==s[t]);/*�ж��Ƿ����*/
 						break;
					case 9:
						t--;
						s[t-1]=(s[t-1]!=s[t]);/*�ж��Ƿ񲻵�*/
 						break;
					case 10:
						t--;
						s[t-1]=(s[t-1]<s[t]);/*�ж��Ƿ�С��*/
 						break;
					case 11:
						t--;
						s[t-1]=(s[t-1]>=s[t]);/*�ж��Ƿ���ڵ���*/
 						break;
					case 12:
						t--;
						s[t-1]=(s[t-1]>s[t]);/*�ж��Ƿ����*/
 						break;
					case 13:
						t--;
						s[t-1]=(s[t-1]<=s[t]);/*�ж��Ƿ�С�ڵ���*/
 						break;
					case 14:
						printf("%d",s[t-1]);/*��ջ��ֵ�������Ļ*/
						fprintf(fa2,"%d",s[t-1]);
						t--;
						break;
					case 15:
						printf("\n");       /*������з�����Ļ*/
						fprintf(fa2,"\n");
						break;
					case 16:
						printf("?");        /*�������ж���һ��������ջ��*/
						fprintf(fa2,"?");
						scanf("%d",&(s[t]));
						fprintf(fa2,"%d\n",s[t]);
						t++;
						break;
				}
				break;
			case lod:       /*ȡ��Ե�ǰ���̵����ݻ���ַΪ����ڴ��ֵ��ջ��*/
				s[t]=s[base(i.l,s,b)+i.a];
				t++;
				break;
			case sto:       /*ջ����ֵ�浽��Ե�ǰ���̵����ݻ���ַΪ����ڴ�*/
				t--;
	            s[base(i.l,s,b)+i.a]=s[t];
				break;
			case cal:                /*�����ӳ���*/
				s[t]=base(i.l,s,b); /*�������̻���ַ��ջ*/
				s[t+1]=b;           /*�������̻���ַ��ջ������������base����*/
				s[t+2]=p;           /*����ǰָ��ָ����ջ*/
				b=t;               /*�ı����ַָ��ֵΪ�¹��̵Ļ���ַ*/
				p=i.a;              /*��ת*/
				break;
			case inte:               /*�����ڴ�*/
				t+=i.a;
				break;
			case jmp:              /*ֱ����ת*/
				p=i.a;
				break;
			case jpc:              /*������ת*/
				t--;
				if(s[t]==0)
				{
					p=i.a;
				}
	    		break;
			case lda:   /* ����Ԫ�ط���,��ǰջ��ΪԪ������,ִ�к�,ջ�����Ԫ�ص�ֵ */
				s[t-1] = s[base(i.l,s,b) + i.a + s[t-1]];
				break;
			case sta:   /* ջ����ֵ�浽������,����Ϊ��ջ�� */
				t-=2;
				s[base(i.l,s,b) + i.a + s[t]] = s[t+1];
				break;
		}
	}while (p!=0);
}
/*ͨ�����̻�ַ����1����̵Ļ�ַ*/
int base(int l,int * s,int b)
{
	int b1;
	b1=b;
	 while(l>0)
	 {
		 b1=s[b1];/*�õ�ǰ����������ַ�е����ݣ���һ��Ļ�ַ����Ϊ�µĵ�ǰ�㣬����������һ��*/
     	 l--;
	 }
	 return b1;
}


/*
* ������������, �½���Ͻ������Ѿ�������ĳ�����ʶ��
*/
int arraydeclaration(int* ptx, int lev, int* pdx)
{
    char arrId[al];             /* �ݴ������ʶ��,���ⱻ���� */
    int cstId;                  /* ������ʶ����λ�� */
    int arrBase=-1, arrTop=-1;  /* �����½硢�Ͻ����ֵ */
	getsymdo;
    if(sym==lbrack)  /* ��ʶ��֮����'[',��ʶ��Ϊ���� */
    {
        strcpy(arrId, id);
        //getchdo;

        /* ����½� */
        getsymdo;
        if(sym==ident)
        {
            if((cstId=position(id,(*ptx)))!=0)
                arrBase=(constant==table[cstId].kind)?table[cstId].val:-1;
        }
        else
        {
            arrBase=(sym==number)?num:-1;
        }
        if(-1==arrBase)
		{
			error(50);
			return -1;
		}
        
        /* ���ð�� */
		getsymdo;
		if(sym!=colon)
        {
            error(50);
			return -1;
        }
        //getchdo;
        
        /* ����Ͻ� */
        getsymdo;
        if(sym==ident)
        {
            if((cstId=position(id,(*ptx)))!=0)
				arrTop=(constant==table[cstId].kind)?table[cstId].val:-1;
        }
        else
        {
			arrTop=(number==sym)?num:-1;
        }
        if(arrTop==-1)
		{
			error(50);	//����ָ��,��Ϊԭ����Դ���ŵĹ滮����!
			return -1;
		}

        /* ���']' */
        getsymdo;
        if(sym!=rbrack)
        {
            error(50);
			return -1;
        }

        /* ���½��Ƿ����������� */
        g_arrSize=arrTop-arrBase+1;
        g_arrBase=arrBase;        
        if(g_arrSize<=0)
		{
			error(50);
			return -1;
		}
        
        /* �ָ�����ı�ʶ�� */
        strcpy(id, arrId);
        return 1;
    }
    return 0;
}


/*
* ����Ԫ�����������롰�����������
*/
int arraycoef(bool *fsys,int *ptx,int lev)
{
    bool nxtlev[symnum];
    int i = position(id,*ptx);
    getsymdo;
    if (sym==lbrack) /* �����������ڵı��ʽ */
    {
        getsymdo;
        memcpy(nxtlev,fsys,sizeof(bool)*symnum);
        nxtlev[rbrack]=true;
        expressiondo(nxtlev,ptx,lev);
        if (sym==rbrack)
        {
            gendo(lit,0,table[i].data);
            gendo(opr,0,3);   /* ϵ������,��ȥ�½��ֵ */
            return 0;
        }
        else
        {
            error(22);  /* ȱ�������� */
        }
    }
    else
    {
        error(51);  /* ������ʴ��� */
    }
    return -1;
}