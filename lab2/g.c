// Найти все области, статистику глифов:
// количество черных пикселей, связность, диаметр наибольшей области
#include <stdio.h>
#include <stdlib.h>
#define DEBUG 3 
// #undef DEBUG
#define ISBIT(n,x) (((01<<(n))&(x))?1:0)

void outbyte( char byte)
{          // Вывод байта в двоичном виде
  int i;
  for ( i=7; i>-1; i-- )
    putchar(ISBIT((i),byte)?'*':' ');
  fflush(stdout);
}

void outbytes( int n, char *byte)
{          // Вывод массива байтов в двоичном виде
  int i;
 for (i=0; i<n; i++)
   outbyte(byte[i]);
 putchar('\n');
}



typedef struct img { //  структура глифа - картинки
   int w; int h;  // ширина и высота в пикслелях
   int dx;      // расстояние до следующего глифа (если буква)
   int count;   // черных пикселей всего
   int id;      // иднтификатор - "номер" глифа
   int bytes;   // /количество байтов в битовой карте
   double density;
   int diam;
   int perim;
   int conn;
   unsigned char *data;  //  битовая карта (неупакованная)
  } IMG;
 
int popcnt8 ( unsigned char i)
{   int count;
     count=0;
     while(i)
     { ++count;
       i=(i-1)&i; }
      return count;
}


IMG *init_img(int id, int w, int h)
{             // создание пустого (чистого) глифа с заданными размерами
 IMG *t;
 t=(IMG *)malloc(sizeof(IMG)); 
  t->w=w; t->h=h; t->dx=0;
  t->count=0;
  t->id=id;
  t->bytes=(((w+7)/8)*h);
  t->data=(unsigned char *)calloc(t->bytes,1);
 return t;
}



IMG *load_img(int id, char *s)          
{
  FILE *F;          // считывание из файла глифа с номером id
   IMG *I;
   I = (IMG *)malloc(sizeof(IMG));
    F = fopen(s,"rb");
  fread(&(I->w),sizeof(int),1,F);
  fread(&(I->h),sizeof(int),1,F);
  fread(&(I->dx),sizeof(int),1,F);
  fread(&(I->count),sizeof(int),1,F);
  fread(&(I->id),sizeof(int),1,F);
  fread(&(I->bytes),sizeof(int),1,F);
   I->data=(unsigned char *)calloc(I->bytes,1);
  fread(I->data,1,I->bytes,F);
  fclose(F);
  return I;
}

int xcomp (IMG **a, IMG **b)
{
   return	-(*a)->w+(*b)->w;
}

int ycomp (IMG **a, IMG **b)
{
   return	-(*a)->h+(*b)->h;
}

 IMG *G[50000];	

 int N;

int ISPIX(int argc, int i, int j)
    {       if (i<0) return 0;
	    if (j<0) return 0;
            if (i>=G[argc]->w) return 0;
                    if (j>=G[argc]->h) return 0;
	 return   ISBIT((7-j%8), G[argc]->data[i*(G[argc]->bytes/G[argc]->h) +j/8 ]);
    }  

int main( int argc, char *argv[])
{       long int c;
	int len;
	unsigned char *s;
	unsigned long long *t;
   N = argc-1;
   while (argc-->1)
      { 
         G[argc]=load_img(argc, argv[argc]);
	 printf("Loaded %s\n",argv[argc]);
     
	 c=0;  
	 t=(unsigned long long *)G[argc]->data;
	 s=(unsigned char *)G[argc]->data;

	 len=G[argc]->bytes;
	 for (int i=0; i<G[argc]->h; i++)
	 {
#ifdef DEBUG
		 printf("%d %d \n",G[argc]->w,G[argc]->h); fflush(stdout);
#endif

    outbytes((G[argc]->w+7)/8, &G[argc]->data[i*(G[argc]->w+7)/8]);
	 }
   for (int i=(len/8)*8; i<len; i++)
   {       c+=popcnt8(s[i]);
#ifdef DEBUG
  printf("COUNT8=%d %d\n",c,i); fflush(stdout);
#endif
   }
     G[argc]->count=c;
     
      }
  printf("%d %d\n",N,c);
}
