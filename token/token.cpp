/* strtok example */
#include <stdio.h>
#include <string.h>

int main ()
{
  char str[] ="- This, a sample string.";
  char * pch;
  printf ("Splitting string \"%s\" into tokens:\n",str);///- This, a sample string.'null'
  pch = strtok (str," ,.-");
  printf ("%s\n",pch);
  ///printf ("Splitting string \"%s\" into tokens:\n",str);///- This'null', a sample string.'null'
  ///pch = strtok (str," ,.-");
  ///printf ("%s\n",pch);
  while (pch != NULL)
  {
    printf ("%s\n",pch);
    pch = strtok (NULL, " ,.-");
  }
  return 0;
}
