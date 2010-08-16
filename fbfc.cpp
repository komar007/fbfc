#include <cstdio>
#include <cerrno>
#include <cstdlib>
#include <cstring>

#define COMPILER
#include "fbf_impl.h"
#undef COMPILER

const char* main_begin = "#include <cstdio>\nint main(){turing_ptr p; int c;\n";
const char* main_end   = "return 0;}\n";

using namespace std;

int main(int argc, char* argv[])
{
	if (argc != 3 && argc != 2) {
		fprintf(stderr, "Fast brainfuck translator\n"
				"usage:\n"
				"\t%s <infile.bf> [<outfile>]\n", argv[0]);
		exit(0);
	}
	FILE *fp;
	fp = fopen(argv[1], "r");
	if (fp == NULL) {
		fprintf(stderr, "error opening input file %s: %s\n",
				argv[1], strerror(errno));
		exit(-1);
	}
	char *outbin;
	if (argc == 3)
		outbin = argv[2];
	else
		outbin = (char*)"a.out";
	char *outf = tmpnam(NULL);
	FILE *out = fopen(outf, "w");
	if (out == NULL) {
		fprintf(stderr, "error opening temp file %s: %s\n",
				outf, strerror(errno));
		exit(-1);
	}
	fprintf(out, "%s\n", header);
	fprintf(out, "%s\n", main_begin);
	int c;
	while ((c = fgetc(fp)) != EOF) {
		switch (c) {
		case '+': fprintf(out, "++*p;");           break;
		case '-': fprintf(out, "--*p;");           break;
		case '>': fprintf(out, "++p;");            break;
		case '<': fprintf(out, "--p;");            break;
		case '[': fprintf(out, "while(*p){");      break;
		case ']': fprintf(out, "}");               break;
		case ',': fprintf(out, "if ((c=getchar())!=EOF)*p=c;"); break;
		case '.': fprintf(out, "putchar(*p);");    break;
		default: break;
		}
	}
	fclose(fp);
	fprintf(out, "%s\n", main_end);
	fclose(out);

	char inv[200];
	sprintf(inv, "g++ -xc++ %s -o%s", outf, outbin);
	(void)system(inv);
	sprintf(inv, "strip %s", outbin);
	(void)system(inv);

	return 0;
}
