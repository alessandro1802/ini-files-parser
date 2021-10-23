// Made by Oleksandr Yasinskyi 150570 and Maciej Moroz 147538
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

// Define a structure
typedef struct Section{
    char* s;
    char* k;
    char* val;
}Section;

int main(int argc, char *argv[]){

    // Open the file for reading and validate it
    FILE *in;
    if (( in = fopen (argv[1], "r")) == NULL) {
        fprintf (stderr, "error: file open failed '%s'\n", argv[1]);
        perror ("fopen");
        return 1;
    }

    // Calculate the length of sect and key
    int keyl=0;
    int sectl=0;
    for (int i=0; i<=strlen(argv[2]); i++){
        if (argv[2][i]=='.'){
            sectl=i;
        }
        if (sectl!=0){
            keyl++;
        }
    }
    keyl-=2;

    // Split section.key into sect and key
    char* sect;
    sect = strtok(argv[2], ".");
    char* key;
    key = strtok(NULL, ".");
    if (sect==NULL || key==NULL){
        fputs ("error: invalid section.key\n", stderr);
        return 1;
    }

    // Check if Section and Key are alphanumeric
    for (int i=0; sect[i]; i++){
        if (isalnum(sect[i])==0){
            if (sect[i]!= '-'){
                printf("The Section name is not alphanumeric!");
                return 1;
            }
        }
    }
    for (int i=0; key[i]; i++){
        if (isalnum(key[i])==0){
            if (key[i]!= '-'){
                printf("The Key is not alphanumeric!");
                return 1;
            }
        }
    }

    char* inisect;
    char* buf;
    int buflen=256;
    buf=(char *)malloc(256*sizeof(char));

    // Read line by line, check if it starts with '[', parse the section name, compare to the needed name
    while (fgets (buf, buflen, in)) {
        if (buf[strlen(buf)-1]!='\n'){
            fseek(in,-strlen(buf), SEEK_CUR);
            buflen*=2;
            buf=malloc(buflen);
        }
        else{
            if (buf[0] == '[') {
            inisect = malloc( strlen(buf)-1 );
            for (int i=0; i<strlen(buf)-3; i++){
                inisect[i]=buf[i+1];
            }
            if (strcmp (sect, inisect) == 0)
                    break;
            }
        }
    }


    char* inikey;
    inikey = malloc( keyl+1 );
    char* val;
    
    // Continue reading, parse key and value, find the key, check if it matches,parse the result to a structure, print it
    while (fgets (buf, buflen, in)) {

        if (buf[strlen(buf)-1]!='\n'){
            //printf("%d,%s\n",sizeof(buf),buf);
            fseek(in,-strlen(buf), SEEK_CUR);
            buflen*=2;
            buf=malloc(buflen);
        }
        else{
            if (buf[0]=='\n'){
                fprintf (stderr, "error: end of section '%s' reached "
                        "with no matching key found.\n", sect);
                return 1;
            }
            for (int i=0; i<strlen(key); i++){
                    inikey[i]=buf[i];
            }
            if (strcmp (key, inikey) == 0) {
                val = malloc( strlen(buf)-4-strlen(key) );
                for (int i=0; i<(strlen(buf)-4-strlen(key)); i++){
                    val[i]=buf[strlen(key)+3+i];
                }
                Section res;
                res.s=sect;
                res.k=key;
                res.val=val;
                printf("Value: %s\n", res.val);
                break;
            }
        }
    }

    free(inisect);
    free(inikey);
    free(val);
    free(buf);

    fclose(in);
    return 0;
}