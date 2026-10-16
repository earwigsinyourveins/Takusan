#include <stdio.h>
#include <stdlib.h>

typedef struct
    {
    char Name[32];
    long Offset;
    }SYM_TABLE, *SYM_TABLEp;

extern int _nullarea;
extern int __begtext;

char MapFileBuffer[1<<20];
SYM_TABLE SymTableCode[4000];
SYM_TABLE SymTableData[4000];

long SymCountCode = 0;
long SymCountData = 0;

long SymDataSegNum = 3;
long SymCodeSegNum = 1;
char SymDataSegPrefix[16] = "0003:";
char SymCodeSegPrefix[16] = "0001:";

// thats what these default to for some reason
long SymDataSegOffset = 0;
long SymCodeSegOffset = 3;

long filesize( FILE *fp )
  {
    long save_pos, size_of_file;

    save_pos = ftell( fp );
    fseek( fp, 0L, SEEK_END );
    size_of_file = ftell( fp );
    fseek( fp, save_pos, SEEK_SET );
    return( size_of_file );
  }

////////////////////////////////////////////////////////////////////////////////    
//
//  Loading the map file
//    
////////////////////////////////////////////////////////////////////////////////    

    
char * LoadMapFile(char *name, char *base_ptr)
    {
    FILE *fin;
    long bytes_read;
    long size;
    
    fin = fopen(name, "rb");
    
    if (!fin)
        return(NULL);

    size = filesize(fin);

    bytes_read = fread(base_ptr, size, 1, fin);

    base_ptr[size] = '\0';

    fclose(fin);

    return(base_ptr);
    }


////////////////////////////////////////////////////////////////////////////////    
//
//  Parsing Map File
//    
////////////////////////////////////////////////////////////////////////////////    

char * NextLine(char *ptr)    
    {
    char *new_ptr;
    
    //note that ptr passed in must be null terminated somewhere
    
    new_ptr = strchr(ptr,'\n');

    // only increment if non 0
    if (new_ptr)
        new_ptr++;

    return(new_ptr);
    }


char * StartOfLine(char *base_ptr, char *ptr)    
    {
    
    // back up to either the previous line    
    while (1)
        {
        ptr--;
        
        if (ptr <= base_ptr)
            return(base_ptr);
            
        if (*ptr == '\n')
            {
            ptr++;
            return(ptr);
            }
        }    
        
    }
    
void FindDataSegPrefix(char *base_ptr)
    {
    char *ptr;

    ptr = strstr(base_ptr, "__nullarea");
    
    if (ptr == NULL)
        return;
        
    ptr = StartOfLine(base_ptr, ptr);
    
    sscanf(ptr,"%4d", &SymDataSegNum);
    sscanf(ptr,"%5s%8d", SymDataSegPrefix, &SymDataSegOffset);
    
    printf("\nSymDataSegNum %d, SymDataSegPrefix %s, SymDataSegOffset %d\n", SymDataSegNum, SymDataSegPrefix, SymDataSegOffset);    
    }    

void FindCodeSegPrefix(char *base_ptr)
    {
    char *ptr;

    ptr = strstr(base_ptr, "___begtext");
    
    if (ptr == NULL)
        return;
        
    ptr = StartOfLine(base_ptr, ptr);
    
    sscanf(ptr,"%4d", &SymCodeSegNum);
    sscanf(ptr,"%5s%8d", SymCodeSegPrefix, &SymCodeSegOffset);
    
    printf("\nSymCodeSegNum %d, SymCodeSegPrefix %s, SymCodeSegOffset %d\n", SymCodeSegNum, SymCodeSegPrefix, SymCodeSegOffset);    
    }    
    
long ParseMapFile(char *base_ptr, SYM_TABLEp SymTable, char *prefix)
    {
    char *line_ptr;
    long ndx = 0;
    char tmp[256];
    
    line_ptr = base_ptr;
    
    do    
        {
        if (memcmp(line_ptr, prefix, 5) == 0)
            {
            sscanf(line_ptr, "%*5s %[0123456789abcdefABCDEF\+\*] %30s", tmp, SymTable[ndx].Name);
            sscanf(tmp,"%x", &SymTable[ndx].Offset);

            SymTable[ndx].Name[sizeof(SymTable[ndx].Name)-1] = '\0';
            
            ndx++;
            }
        }
    while ((line_ptr = NextLine(line_ptr)) != NULL);
    
    return(ndx);
    }    

#if 1
void SaveSymTable(char *name, SYM_TABLEp SymTable, int num_elem)
    {
    FILE *fout;
    
    fout = fopen(name, "wb");
    
    if (!fout)
        return;

    fwrite(SymTable, sizeof(SYM_TABLE), num_elem, fout);

    fclose(fout);
    }
#else
void SaveSymTable(char *name, SYM_TABLEp SymTable, int num_elem)
    {
    FILE *fout;
    int i;
    
    fout = fopen(name, "w");
    
    if (!fout)
        return;

    for (i = 0; i < num_elem; i++)
        {
        fprintf(fout, "%-40s %08x\n", SymTable[i].Name, SymTable[i].Offset);
        }    

    fclose(fout);
    }
#endif
    
    
////////////////////////////////////////////////////////////////////////////////    
//
//  main
//    
////////////////////////////////////////////////////////////////////////////////    


void main(short int argc, char *argv[])
    {
    char *base_ptr;
    long count = 0;
    
    if (argc < 3)
        {
        printf("\nUsage: MAKESYM <mapfile> <data_symbol_file> <code_symbol_file> \n\n");
        exit(0);
        }
    
    base_ptr = LoadMapFile(argv[1], MapFileBuffer);
    
    FindDataSegPrefix(base_ptr);
    FindCodeSegPrefix(base_ptr);

    if (base_ptr)
        {
        count = ParseMapFile(base_ptr, SymTableData, SymDataSegPrefix);
        SaveSymTable(argv[2], SymTableData, count);

        count = ParseMapFile(base_ptr, SymTableCode, SymCodeSegPrefix);
        SaveSymTable(argv[3], SymTableCode, count);
        }
    else
        {
        printf("ERROR: Unable to save symbol table");
        }    
    }    

