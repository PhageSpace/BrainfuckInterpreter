#include <stdio.h>
#include <stdlib.h>



#define MAXMEMORY 30000

typedef struct Opcode
{
    char type;
    int value;
} Opcode;



int main(int argc, char const *argv[])
{


    int InstructionPtr = 0;
    FILE *f = fopen(argv[1], "rb");
    if(f == NULL)
    {
        printf("error, cannot open file");
        return 0;
    }
    fseek(f, 0, SEEK_END);
    long fsize = ftell(f);
    fseek(f, 0, SEEK_SET);  /* same as rewind(f); */

    char *BFCode = malloc(fsize + 1);
    if(BFCode == 0){
        printf("couldn't allocate memory for program");
        return 0;
        }
    fread(BFCode, fsize, 1, f);
    fclose(f);

    BFCode[fsize] = 0;
    char EndOfFile = 0;

    Opcode *OptimisedCode = malloc(5*fsize);
    int i1 = 0;
    while (EndOfFile == 0)
    {
        
        switch (BFCode[InstructionPtr])
        {
        case '>':
        case '<':
        case '+':
        case '-':
            {
            int Firstinst = InstructionPtr;
            char InsType = BFCode[InstructionPtr];
            while (BFCode[InstructionPtr] == InsType)
            {
                InstructionPtr++;
            }
            OptimisedCode[i1].type = InsType;
            OptimisedCode[i1].value = InstructionPtr - Firstinst;
            InstructionPtr--;
            i1++;
            }
            break;
        case '.':
            OptimisedCode[i1].type = '.';
            i1++;
            break;
        case ',':
            OptimisedCode[i1].type = ',';
            i1++;
            break;
        case '[':

            if(BFCode[InstructionPtr+1] == '-'  && BFCode[InstructionPtr+2] == ']')
            {
                OptimisedCode[i1].type = '*';
                i1++;
                InstructionPtr +=2;
                break;
            }
            OptimisedCode[i1].type = '[';
            i1++;
            break;
        case ']':
            OptimisedCode[i1].type = ']';
            i1++;
            break;
        default:
            break;
        }
        //printf("%c",BFCode[InstructionPtr]);
        if(InstructionPtr == fsize){
            EndOfFile = 1;
            }
        
        InstructionPtr++;
    }
    int nsize = i1--; 
    EndOfFile = 0;
    int i2 = 0;
    while (EndOfFile ==0)
    {        
        switch (OptimisedCode[i2].type)
        {
        case '[':
        {
            int secondaryiterator = i2;
            int scope = 1;
            while (scope != 0)
            {
                secondaryiterator++;
                if (OptimisedCode[secondaryiterator].type == '[')
                {
                    scope++;
                }
                if (OptimisedCode[secondaryiterator].type == ']')
                {
                    scope--;
                }
            }
            OptimisedCode[i2].value = secondaryiterator;
        }
            break;
        case ']':
        {
            int secondaryiterator = i2;
            int scope = 1;
            while (scope != 0)
            {
                secondaryiterator--;
                if (OptimisedCode[secondaryiterator].type == ']')
                {
                    scope++;
                }
                if (OptimisedCode[secondaryiterator].type == '[')
                {
                    scope--;
                }
            }
            OptimisedCode[i2].value = secondaryiterator;
        }
        default:
            break;
        }
        if(i2 == nsize)
        {
            EndOfFile = 1;
        }
        i2++;
    }
    int m = 0;
    while (m != nsize)
    {
        printf("%d:%c'%d , ",m,OptimisedCode[m].type,OptimisedCode[m].value);
        m++;
    }
    m = 0;
    while (m != nsize)
    {
        printf("%c",OptimisedCode[m].type);
        m++;
    }
    

    char Memory[MAXMEMORY+1]={0};
    int DataPtr = 0;
    int Exited = 0;
    InstructionPtr = 0;
    while (Exited == 0)
    {
        if (InstructionPtr == nsize)
        {
            return 0;
        }
        
        switch (OptimisedCode[InstructionPtr].type)
        {
        case '-':
            Memory[DataPtr] -= OptimisedCode[InstructionPtr].value;
            break;
        case '+':
            Memory[DataPtr] += OptimisedCode[InstructionPtr].value;
            break;
        case '*':
            Memory[DataPtr] = 0;
            break;
        case '<':
            DataPtr -= OptimisedCode[InstructionPtr].value;
            if (DataPtr < 0)
            {
                DataPtr = MAXMEMORY + DataPtr +1;
            }
            
            break;
        case '>':
            DataPtr += OptimisedCode[InstructionPtr].value;
            if (DataPtr > MAXMEMORY)
            {
                DataPtr = DataPtr - MAXMEMORY - 1;
            }
            break;
        case '[':
            if (Memory[DataPtr] == 0)
            {
                InstructionPtr = OptimisedCode[InstructionPtr].value;
            }
            break;
        case ']':
            if (Memory[DataPtr] != 0)
            {
                InstructionPtr = OptimisedCode[InstructionPtr].value;
            }
            break;
        case '.':
            putchar(Memory[DataPtr]);
            break;
        case ',':
            getchar(Memory[DataPtr]);
            break;
        default:
            break;
        }

        char DebugData = Memory[DataPtr];

        InstructionPtr++;
    }
    

    return 0;
}
/*            if(Memory[DataPtr] != 0)
            {
                int scope = 1;

                while (scope > 0)
                {
                    InstructionPtr--;
                    if (BFCode[InstructionPtr] == ']')
                    {
                        scope++;
                    }
                    
                    if (BFCode[InstructionPtr] == '[')
                    {
                        scope--;
                    }
                }
            }
            break;*/