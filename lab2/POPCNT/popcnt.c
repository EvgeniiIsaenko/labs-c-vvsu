
int popcnt64(unsigned long long w) {
//  return __builtin_popcountll(w);  // эта функция медленная
    w -= (w >> 1) & 0x5555555555555555ULL;
    w = (w & 0x3333333333333333ULL) + ((w >> 2) & 0x3333333333333333ULL);
    w = (w + (w >> 4)) & 0x0f0f0f0f0f0f0f0fULL;
    return (int)((w * 0x0101010101010101ULL) >> 56);
}
int popcnt32 ( unsigned int i)
{
//   return __builtin_popcountl(i);   // эта функция более медленная
     i = i - ((i >> 1) & 0x55555555);
     i = (i & 0x33333333) + ((i >> 2) & 0x33333333);
     return (((i + (i >> 4)) & 0x0F0F0F0F) * 0x01010101) >> 24;
}
        
int popcnt16 ( unsigned short int i)
{
return __builtin_popcount(i);    // Эта функция быстрая
//     i = i - ((i >> 1) & 0x5555);
//     i = (i & 0x3333) + ((i >> 2) & 0x3333);
//     return (((i + (i >> 4)) & 0x0F0F) * 0x0101) >> 8;
}

int popcnt8 ( unsigned char i)
{   int count;          // соответствующей встроенной функции нет
     count=0;
     while(i)
     { ++count;
       i=(i-1)&i; }
      return count;
}
 
