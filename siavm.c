//
//  siavm.c
//  csi404
//
//  Created by Galusha Kostin on 11/5/18.
//  Copyright © 2018 Kostin Galusha. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
unsigned char memory[10000];
signed int reg[16];
long pc = 0;
signed int op1;
signed int op2;
signed int result;
unsigned char instruction[4];
signed long offset;
signed long address;
int halt;

void populate(FILE *src){
    fread(memory, 10000, 1, src);
}

int signExtend(int num, int n)
{
    num = num | (0xffffffff << n);
    return num;
}


void fetch() {
    instruction[0] = memory[pc];
    
    switch(instruction[0] >> 4)
    {
        case 0 ... 9:
        case 14:
        case 15:
            instruction[1] = memory[pc + 1];
            break;
        case 10 ... 13:
            instruction[1] = memory[pc + 1];
            instruction[2] = memory[pc + 2];
            instruction[3] = memory[pc + 3];
            break;
        default: printf("invalid opcode");
    }
}

void dispatch(){
    
    switch(instruction[0] >> 4){
        case 0 ... 6:
            op1 = reg[(instruction[0] & 0x0f)];
            op2 = reg[(instruction[1] >> 4)];
            break;
        case 7:
            printf("%x\n", op2 = instruction[1] >> 5);
            op1 = reg[(instruction[0] & 0x0f)];
            op2 = instruction[1] >> 5;
            offset = instruction[1] & 0x1f;
            break;
        case 8:
            op1 = ((instruction[0] & 0x0f) << 8) | instruction[1];
            break;
        case 9:
            op1 = reg[(instruction[0] & 0x0f)];
            op2 = instruction[1];
            break;
        case 10 ... 11:
            op1 = reg[(instruction[0] & 0x0f)];
            op2 = reg[(instruction[1] >> 4)];
            offset = ((instruction[1] & 0x0f) << 16) + (instruction[2] << 8) +
                        instruction[3];
            break;
        case 12:
            offset = ((instruction[0] & 0x0f)<<24) + (instruction[1] << 16) +
            (instruction[2] << 8) + instruction[3];
            
            break;
        case 13:
            op1 = reg[(instruction[0] & 0x0f)];
            op2 = instruction[1];
            offset = (instruction[2] << 8) | instruction[3];
            break;
        case 14 ... 15:
            op1 = (instruction[0] & 0x0f);
            op2 = (instruction[1] >> 4);
            offset = instruction[1] & 0x0f;
            break;
    }
}

void execute()
{
    switch(instruction[0] >> 4){
        case 0:
            halt = 1;
            pc += 2;
            break;
        case 1:
            result = op1 + op2;
            pc += 2;
            break;
        case 2:
            result = op1 & op2;
            pc += 2;
            break;
        case 3:
            result = op1/op2;
            pc += 2;
            break;
        case 4:
            result = op1 * op2;
            pc += 2;
            break;
        case 5:
            result = op1 - op2;
            pc += 2;
            break;
        case 6:
            result = op1 | op2;
            pc +=2;
            break;
        case 7:
            if(op2 == 1)
                result = op1 >> offset;
            else if(op2 == 0)
                result = op1 << offset;
            pc += 2;
            break;
        case 8:
            switch(op1){
                case 0:
                {
                    for(int i = 0; i < 16; i++)
                        printf("reg %02d: 0x%08x\n", i, reg[i]);
                    printf("\n");
                    break;
                }
                case 1:
                    for(int i = 0; i < 500; i+=8)
                        printf("%04d: %02x%02x %02x%02x %02x%02x %02x%02x\n", i, memory[i], memory[i+1] , memory[i+2], memory[i+3],memory[i+4], memory[i+5], memory[i+6], memory[i+7]);
                    break;
            }
            pc += 2;
            break;
        case 9:
            if(op2 >> 7 == 1)
            {
                result = op1 + signExtend(op2, 8) ;
            }
            else
                result = op1 + op2;
            pc += 2;
            break;
            
        case 10:
            if(op1 == op2)
            {
                if(offset >> 19 == 1)
                    pc += signExtend(offset, 20);
                else
                    pc += offset;
            }
            else
                pc += 4;
            break;
        case 11:
            if(op1 < op2)
            {
                if(offset >> 19 == 1)
                    pc += signExtend(offset, 20);
                else
                    pc += offset;
            }
            else
                pc +=4;
            break;
        case 12:
            pc = offset;
            break;
        case 13:
            if(memory[op1 + op2] != 0)
                {
                    reg[(instruction[0] & 0x0f)] = memory[op1 + op2];
                    pc -= offset;
                }
            else
                pc += 4;
            break;
            
        case 14:
            pc += 2;
            break;
            
        case 15:
            result = reg[op1];
            pc += 2;
            break;
            
    }
    
}

void store()
{
    switch(instruction[0] >> 4){
        case 1 ... 6:
            reg[instruction[1] & 0x0f] = result;
            break;
        case 7:
        case 9:
            reg[(instruction[0] & 0x0f) ] = result;
            break;
        case 14:
            if(offset >> 3 == 1)
            {
                address = reg[op2] - (-offset & 0x0f);
                reg[op1]  = (memory[address + 3] << 24) | (memory[address + 2] << 16) |
                (memory[address + 1] << 8) | memory[address];
            }
            else
            {
                address = reg[op2] + offset;
                reg[op1] = (memory[address + 3] <<24) | (memory[address + 2] << 16 ) |
                (memory[address + 1] << 8) | memory[address];
            }
            break;
            
        case 15:
            if(offset >> 3 == 1)
            {
                offset = reg[op2] - (-offset & 0x0f);
                memory[offset + 3] = result >> 24;
                memory[offset + 2] = (result >> 16) & 0xff;
                memory[offset + 1] = (result >> 8) & 0xff;
                memory[offset] = (result) & 0xff;
            }
            else
            {
                offset = reg[op2] + offset;
                memory[offset + 3] = result >> 24;
                memory[offset + 2] = (result >> 16) & 0xff;
                memory[offset + 1] = (result >> 8) & 0xff;
                memory[offset] = (result) & 0xff;
            }
            break;
    }
}



int main(int argc, char **argv) {
    int i = 0;
    FILE *src = fopen(argv[1],"r");
    
    for(i = 0; i < 1000; i++)
        memory[i] = 0;
    
    populate(src);
    i = 1;
    
    if(sizeof(int) != 4)
    {
        printf("Error! Size of integer is not 4\n");
        exit(0);
    }
    
    while(halt == 0)
        {
            fetch();
            dispatch();
            execute();
            store();
            result = 0;
            offset = 0;
        }
        
    fclose(src);
    return 0;
}
