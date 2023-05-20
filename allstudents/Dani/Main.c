#include <stdio.h>
#include <stdlib.h>

int main(int arc, char* argv[]){
if(arc != 3){
  printf("argument error");
  return -1;
}

printf("%d", atoi(argv[1]) + atoi(argv[2]));
return 0;
}
