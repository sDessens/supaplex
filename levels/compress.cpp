#include <stdio.h>
#include <stdlib.h>

int bitcounter = 0, bytecounter = 0;
char compressed[2000];		//aka stack

int max(int a, int b) {
	return (a > b) ? a : b;
}

void findnext(char c, FILE *ifp) {
	int i;
	do {
	i = getc(ifp);
	} while ( i != EOF && i != c);
}

int chartohex(char c) {
	c = (c >= 'a' && c <= 'f') ? c - 32 : c;		//converteert naar hoofdletters
	if (c >= '0' && c <= '9')
		return c - '0';
	if (c >= 'A' && c <= 'F')
		return c - 'A' + 10;
	return 0;	
}

void error(char str[]) {
	printf("\n%s\npress any key to quit\n",str);
	getchar();
	exit(1);
}

int *findstr(int *array, int start) {
	//printf("atempting to find string at #%d\n",start);
	int i;
	static int value[2];	
	for(int lengt = 32+4; lengt > 4; lengt--) {
		//printf("lengt:%d\n",lengt);
		for(int backtrack = -1; backtrack >= max(-255,-start) ; backtrack--){
			//printf("\rcurrent lengt: %d\tcurrent backtrack: %d\t\n",lengt, backtrack);
			value[0] = backtrack;
			value[1] = lengt;
			for(i = 0;(i < lengt) &&(array[start+i] == array[start+backtrack+i]); i++);
			if ((i)>=lengt) {
			return value;
			}
		}
	}
	return NULL;	
}

void pushbit(int i) {
	compressed[bytecounter] = compressed[bytecounter] << 1;
	if (i != 0)
		compressed[bytecounter]++;
	bitcounter++;
	if (bitcounter != 4)
		return;
	bitcounter = 0;
	bytecounter++;
}
		
void pushbyte(int byte, int bits) {
	printf("%x%d\t",byte,bits);
	for(; bits > 0; bits--)
		pushbit((1 << (bits - 1)) & byte);
}

char int2char(int i) {
	return i + '0' + (('A'-'9'-1) * (i > 9));
}

int main(int argc, char *argv[]) {

	for(int i = 0; i < 2000; i++)	//leeg compressed array
		compressed[i] = 0;
		
	if (argc < 2)
		error("too few args");
		
	printf("atempting to compress: %s....\n",argv[1]);
	
	FILE *ifp = fopen(argv[1], "r");
	if (ifp == NULL)
		error("unable to open file");

	
	
	int hex, hex2, c;
	int tilemap[58*22];
	int pointer = 0;
	int tilemap_x = 0, tilemap_y = 0;
	while (c != EOF) {			//load tilemap loop
		c = getc(ifp);
		if (';' == c)			//comment?
			findnext('\n',ifp);	//zoek eerstvolgende nieuwe regel
		if ('.' == c) { //&& 'd' == getc(ifp) && ('b' == getc(ifp)))	{		//.db??
			while (c != '\n' && c != ';' && c != EOF) {					//zolang GEEN nieuwe regel of comment..
				c = getc(ifp);
				if (c == '$') {
					tilemap[pointer] = (chartohex(getc(ifp)) << 4) + (chartohex(getc(ifp)));		//haal eerste hex-char op
					//printf("%d\t%d\n", pointer, tilemap[pointer]); 
					pointer++;
					if (pointer > 22*58)
						error("target is not a valid 58*22 tilemap: tilemap is too large");
									
				}
			}
		}
	}
	if (pointer < 22*58)
		error("no vailid 58*22 tilemap found");
		
	pointer = 0;
	printf("valid tilemap found, starting LZ77 compression...\n");
	while (pointer < 22*58) {
		int *value = findstr(&tilemap[0], pointer);
		//printf("pointer: %d\n",pointer);
		if (value != 0) {	//als er een geldige string is gevonden, push 3 bytes op de stack
			
			//printf("pushing values on stack\n");
			pushbyte(7,3);
			pushbyte(-(value[0]),8);
			pushbyte(value[1]-5,5);
			//printf("backtrack: %d (%d)\tlengt:%d (%d)\n", value[0], -(value[0]), value[1], value[1]-5);
			pointer += value[1];	//update pointer naar array	
		}
		else {			//zoniet, bepaal de byte, en push die op de stack
			int tile = tilemap[pointer];
			//printf("no string was found\ncurrent tile is: %d\n",tile);	
			if (tile <=5) 	//als tile in 3 bits past
				pushbyte(tile, 3);
			else {
				pushbyte(6, 3);
				tile = (tile >= 21) ? (tile - 1) : tile;
				tile = (tile >= 19) ? (tile - 1) : tile;
				tile -= 6;	//conversie naar tiles die daadwerkelijk worden gebruikt...
				pushbyte(tile, 4);		
			}	
		pointer++;
		}
	}
	while (bitcounter != 0) 
	pushbit(0);
	
	printf("compression finished, compressed data is %d bytes (%d, %d)\n",(bytecounter + 2 - (bitcounter == 0)) / 2, bytecounter, bitcounter);
	
	char *path = argv[1];
	char filename[255];
	int ptr, i;
	for(ptr = 0; path[ptr] != '\0'; ptr++);	//zoek ende van pad
	for(; path[ptr] != '\\'; ptr--);	//zoek laatste \ in pad
	ptr++;
	for(i = 0; path[ptr + i - 1] != '\0'; filename[i] = path[ptr + i++]) {}			//kopieer filename
	ptr -= 2;
	for(; path[ptr] != '\\'; ptr--);	//zoek een-na laatste in pad
	ptr++;
	for(i = 0; path[ptr + i -1] != '\0'; path[ptr + i] = "compressed\\"[i++]) {}	//wijzig pad naar \compressed\.
	ptr += i -1;
	for(i = 0; path[ptr+i-1] != '\0'; path[ptr + i] = filename[i++]);		//plak filename
	printf("now outputting to file %s\n",path);
	FILE *ofp = fopen(path,"w");
	if (ofp == NULL)
		error("unable to open file");
		
	int counter = 0;
	int chars = (bytecounter + 2 - (bitcounter == 0)) / 2;
	while (counter < chars) {
		fprintf(ofp,".db ");
		for( int i = counter; i + 10 > counter; counter++) {
			if (counter < chars)
				fprintf(ofp, "$%c%c,", int2char(compressed[2*counter]), int2char(compressed[2*counter+1]));
		}
		fprintf(ofp, "\n");
	}
	printf("sucess!\n");
	fclose(ofp);
	getchar();
}
