#include <stdio.h>
#include <string.h>
#include "function.h"

#define BSIZE 64
#define COL 16
#define BASE 10
#define DELIM ','

void showErr (const char *errstr[], int errNO);
int showHelp (char *path, const char *param[], const char *pdetail[],
	      int ret);
static unsigned int basename (const char *ch);

static char suffix[BSIZE];
static char buff[BSIZE];

static const char *errstr[] =
  { "Parameters is not match", "Input file is not found",
  "Can not assign an output file", "Base number must not be zero"
};
static const char *param[] = { "-b" };
static const char *pdetail[] = { "base Number" };

enum __errNO
{
  e_param, e_input, e_output, e_basezero
};
enum __param
{
  e_base
};

int
main (int argc, char *argv[])
{
  FILE *in, *out;
  unsigned int ch, i, j, base, syn;
  static char instr[1024], outstr[1024];
/******************* Parameters **********************/

  if (argc < 2)
    {
      showErr (errstr, e_param);
      return showHelp (argv[0], param, pdetail, 0);
      return 1;
    }

  base = BASE;
  j = 1;
  if (!strncmp (argv[1], param[e_base], sLen (param[e_base])))
    {
      if (!isUint (argv[1] + 2))
	{
	  showErr (errstr, e_param);
	  return showHelp (argv[0], param, pdetail, 0);
	}
      base = s2ui (argv[1] + 2);
      j = 2;
    }

  if (!base)
    {
      showErr (errstr, e_basezero);
      return showHelp (argv[0], param, pdetail, 0);
    }

  suffix[0] = '.';
  ui2s (base, suffix + 1, BSIZE - 1, 10);

/******************* Parameters **********************/
  for (syn = 0; j < argc; j++)
    {
      strcpy (instr, argv[j]);
      strcpy (outstr, argv[j]);
      strcat (outstr, suffix);

      if (!(in = fopen (instr, "rb")))
	{
	  fprintf (stderr, "FILE: %s\n\n", instr);
	  syn = 1;
	  showErr (errstr, e_input);
	  continue;
	}

      if (!(out = fopen (outstr, "wb")))
	{
	  showErr (errstr, e_output);
	  fprintf (stderr, "FILE: %s\n\n", outstr);
	  fclose (in);
	  syn = 1;
	  continue;
	}
      i = 0;
      syn = 0;
      while ((ch = fgetc (in)) != EOF)
	{
	  ui2s (ch, buff, BSIZE, base);
	  fprintf (out, "%s%c", buff, DELIM);
	  if (++i > COL - 1)
	    {
	      fprintf (out, "\n");
	      i = 0;
	    }

	}
      if (!syn)
	{
	  fclose (in);
	  fclose (out);
	}
    }
  return 0;
}

void
showErr (const char *errstr[], int errNO)
{
  fprintf (stderr, "ERR NO:%u = %s\n", errNO, errstr[errNO]);
}

int
showHelp (char *path, const char *param[], const char *pdetail[], int ret)
{
  fprintf (stderr, "\nUSAGE: %s {%sN} file1 file2 ...\n",
	   path + basename (path), param[e_base]);
  fprintf (stderr, "%s = %s\n", param[e_base], pdetail[e_base]);

  return ret;
}

static unsigned int
basename (const char *ch)
{
  unsigned int i, j;
  for (i = 0, j = 0; ch[i]; i++)
    {
      if (ch[i] == '\\' || ch[i] == '/')
	{
	  j = i;
	}
    }
  return (j == 0) ? 0 : j + 1;
}
