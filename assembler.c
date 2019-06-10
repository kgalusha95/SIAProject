#include <stdio.h>
#include <string.h>
#include <stdlib.h>

char *ltrim(char *s) {
	while (*s == ' ' || *s == '\t') s++;
	return s;
}

char getRegister(char *text) {
	if (*text == 'r' || *text=='R') text++;
	return atoi(text);
}

int assembleLine(char *text, unsigned char* bytes) {
	text = ltrim(text);
	char *keyWord = strtok(text," ");
    int offset = 0;
	if (strcmp("add",keyWord) == 0) {
		bytes[0] = 0x10; // opcode
        bytes[0] |= getRegister(strtok(NULL," ")); // OR equal with 0x10 = 1#
        //get next register - shift so format is #0 then or with last register
        bytes[1] = getRegister(strtok(NULL," ")) << 4 | getRegister(strtok(NULL," "));
		return 2; //number of bytes in instruction
	}
    
    if (strcmp("subtract",keyWord) == 0) {
        bytes[0] = 0x50; // opcode
        bytes[0] |= getRegister(strtok(NULL," ")); // OR equal with 0x10 = 1#
        //get next register - shift so format is #0 then or with last register
        bytes[1] = getRegister(strtok(NULL," ")) << 4 | getRegister(strtok(NULL," "));
        return 2; //number of bytes in instruction
    }
    
    if (strcmp("and",keyWord) == 0) {
        bytes[0] = 0x20; // opcode
        bytes[0] |= getRegister(strtok(NULL," ")); // OR equal with 0x10 = 1#
        //get next register - shift so format is #0 then or with last register
        bytes[1] = getRegister(strtok(NULL," ")) << 4 | getRegister(strtok(NULL," "));
        return 2; //number of bytes in instruction
    }
    if (strcmp("or",keyWord) == 0) {
        bytes[0] = 0x60; // opcode
        bytes[0] |= getRegister(strtok(NULL," ")); // OR equal with 0x10 = 1#
        //get next register - shift so format is #0 then or with last register
        bytes[1] = getRegister(strtok(NULL," ")) << 4 | getRegister(strtok(NULL," "));
        return 2; //number of bytes in instruction
    }
    
    if (strcmp("divide",keyWord) == 0) {
        bytes[0] = 0x30; // opcode
        bytes[0] |= getRegister(strtok(NULL," ")); // OR equal with 0x10 = 1#
        //get next register - shift so format is #0 then or with last register
        bytes[1] = getRegister(strtok(NULL," ")) << 4 | getRegister(strtok(NULL," "));
        return 2; //number of bytes in instruction
    }
    
    if (strcmp("multiply",keyWord) == 0) {
        bytes[0] = 0x40; // opcode
        bytes[0] |= getRegister(strtok(NULL," ")); // OR equal with 0x10 = 1#
        //get next register - shift so format is #0 then or with last register
        bytes[1] = getRegister(strtok(NULL," ")) << 4 | getRegister(strtok(NULL," "));
        return 2; //number of bytes in instruction
    }
    
    if (strcmp("addimmediate",keyWord) == 0) {
        bytes[0] = 0x90; // opcode
        bytes[0] |= getRegister(strtok(NULL," ")); // OR equal with 0x10 = 1#
        //get next register - shift so format is #0 then or with last register
        bytes[1] = atoi(strtok(NULL," ")) & 255;
        return 2; //number of bytes in instruction
    }
    
    if (strncmp("halt", text, 4) == 0) {
        bytes[0] = 0x00; // opcode
        bytes[1] = 0x00;
        return 2; //number of bytes in instruction
    }
    
    if (strcmp("interrupt",keyWord) == 0) {
        bytes[0] = 0x80; // opcode
        offset = atoi(strtok(NULL, " "));
        bytes[1] = offset & 255;
        bytes[0] |= (offset >> 8) & 15;
        return 2; //number of bytes in instruction
    }
    
    if (strcmp("branchifequal",keyWord) == 0) {
        bytes[0] = 0xA0; // opcode
        bytes[0] |= getRegister(strtok(NULL," ")); // OR equal with 0x10 = 1#
        //get next register - shift so format is #0 then or with last register
        bytes[1] = getRegister(strtok(NULL," ")) << 4;
        offset = atoi(strtok(NULL," "));
        bytes[3] = offset & 255;
        bytes[2] = (offset >> 8) & 255;
        bytes[1] |= (offset >> 16) & 15;
        return 4; //number of bytes in instruction
    }
    if (strcmp("branchifless",keyWord) == 0) {
        bytes[0] = 0xB0; // opcode
        bytes[0] |= getRegister(strtok(NULL," ")); // OR equal with 0x10 = 1#
        //get next register - shift so format is #0 then or with last register
        bytes[1] = getRegister(strtok(NULL," ")) << 4;
        offset = atoi(strtok(NULL," "));
        bytes[3] = offset & 255;
        bytes[2] = (offset >> 8) & 255;
        bytes[1] |= (offset >> 16) & 15;
        return 4; //number of bytes in instruction
    }
    if (strcmp("iterateover",keyWord) == 0) {
        bytes[0] = 0xD0; // opcode
        bytes[0] |= getRegister(strtok(NULL," ")); // OR equal with 0x10 = 1#
        //get next register - shift so format is #0 then or with last register
        bytes[1] = atoi(strtok(NULL," ")) & 255;
        offset = atoi(strtok(NULL," "));
        bytes[3] = offset & 255;
        bytes[2] = (offset >> 8) & 255;
        return 4; //number of bytes in instruction
    }
    
    if (strcmp("jump",keyWord) == 0) {
        bytes[0] = 0xC0; // opcode
        offset = atoi(strtok(NULL," "));
        bytes[3] = offset & 255;
        bytes[2] = (offset >> 8) & 255;
        bytes[1] = (offset >> 16) & 255;
        bytes[0] |= (offset >> 24) & 15;
        return 4; //number of bytes in instruction
    }
    
    if (strcmp("load",keyWord) == 0) {
        bytes[0] = 0xE0; // opcode
        bytes[0] |= getRegister(strtok(NULL," ")); // OR equal with 0x10 = 1#
        //get next register - shift so format is #0 then or with last register
        bytes[1] = getRegister(strtok(NULL," ")) << 4 |(atoi(strtok(NULL," ")) & 15);
        return 2; //number of bytes in instruction
    }
    
    if (strcmp("store",keyWord) == 0) {
        bytes[0] = 0xF0; // opcode
        bytes[0] |= getRegister(strtok(NULL," ")); // OR equal with 0x10 = 1#
        //get next register - shift so format is #0 then or with last register
        bytes[1] = getRegister(strtok(NULL," ")) << 4 |(atoi(strtok(NULL," ")) & 15);
        return 2; //number of bytes in instruction
    }
    
    if (strcmp("leftshift",keyWord) == 0) {
        bytes[0] = 0x70; // opcode
        bytes[0] |= getRegister(strtok(NULL," ")); // OR equal with 0x10 = 1#
        bytes[1] = atoi(strtok(NULL, " ")) & 31;
        return 2; //number of bytes in instruction
    }
    if (strcmp("rightshift",keyWord) == 0) {
        bytes[0] = 0x70; // opcode
        bytes[0] |= getRegister(strtok(NULL," ")); // OR equal with 0x10 = 1#
        //get next register - shift so format is #0 then or with last register
        bytes[1] = 0x20;
        bytes[1] |= atoi(strtok(NULL, " ")) & 31;
        return 2; //number of bytes in instruction
    }
    else
    {
        printf("Invalid Command\n\n");
        return 0;
    }
    
}

int main(int argc, char **argv) {
	FILE *src = fopen(argv[1],"r");
	FILE *dst = fopen(argv[2],"w");
	while (!feof(src)) {
		unsigned char bytes[4];
		char line[1000];
		printf ("about to read\n");
		if (NULL != fgets(line, 1000, src)) {
			printf ("read: %s\n",line);
			int byteCount = assembleLine(line,bytes);
			fwrite(bytes,byteCount,1,dst);
		}
	}
	fclose(src);
	fclose(dst);
	return 0;
}
