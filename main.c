#include <stdio.h>
#include <string.h>
#include "function.h"
#include "opt.h"

#define BSIZE 1024
#define SSIZE 8
#define COLS 16
#define BASE 16
#define DELIM ','

void showErr (const char *errstr[], int errNO);
int showHelp (const char *path, const char *param[], const char *pdetail[],
	      int ret);
static unsigned int basename (const char *ch);

static char suffix[SSIZE];
static char buff[BSIZE];

static const char *errstr[] =
  { "Parameters is not match", "Input file is not found",
  "Can not assign an output file", "(1<= base <=10) AND (base=16)"
};
static const char *param[] = { "-b", "-c", "-h", NULL };
static const char *pdetail[] = { "base Number", "cols" };

enum __errNO
{
  e_param, e_input, e_output, e_limitbase
};
enum __param
{
  e_base, e_cols, e_help
};

int
main (int argc, const char *argv[])
{
  FILE *in, *out;
  unsigned int ch, i, base, syn, cols;
  int parm;
  static char instr[1024], outstr[1024];

  base = BASE;
  cols = COLS;
  suffix[0] = '.';
  ui2s (base, suffix + 1, SSIZE - 1, 10);
  while ((parm =
	  opt_action (argc, argv, param, buff, BSIZE, DSTART)) != e_optend)
    {
      switch (parm)
	{
	case e_base:
	  if (!isUint (buff))
	    {
	      showErr (errstr, e_param);
	      return showHelp (argv[0], param, pdetail, 1);
	    }
	  strcpy (suffix + 1, buff);
	  base = s2ui (buff);

	  if (base == 0 || (base > 10 && base != 16))
	    {
	      showErr (errstr, e_limitbase);
	      return showHelp (argv[0], param, pdetail, 1);
	    }

	  break;
	case e_cols:

	  if (!isUint (buff))
	    {
	      showErr (errstr, e_param);
	      return showHelp (argv[0], param, pdetail, 1);
	    }
	  cols = s2ui (buff);
	  break;
	case e_help:
	  return showHelp (argv[0], param, pdetail, 1);
	  break;
	case e_optother:
	  syn = 0;
	  strcpy (instr, buff);
	  strcpy (outstr, buff);
	  strcat (outstr, suffix);

	  if (!(in = fopen (instr, "rb")))
	    {
	      showErr (errstr, e_input);
	      fprintf (stderr, "FILE: %s\n\n", instr);
	      syn = 1;
	      break;
	    }

	  if (!(out = fopen (outstr, "wb")))
	    {
	      showErr (errstr, e_output);
	      fprintf (stderr, "FILE: %s\n\n", outstr);
	      fclose (in);
	      syn = 1;
	      break;
	    }
	  i = 0;
	  syn = 0;
	  while ((ch = fgetc (in)) != EOF)
	    {
	      ui2s (ch, buff, BSIZE, base);
	      fprintf (out, "%s%c", buff, DELIM);

	      if ((cols) && (++i > cols - 1))
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
	  break;
	}
    }


/*****************************************************/



  return 0;
}

void
showErr (const char *errstr[], int errNO)
{
  fprintf (stderr, "ERR NO:%u = %s\n", errNO, errstr[errNO]);
}

int
showHelp (const char *path, const char *param[], const char *pdetail[],
	  int ret)
{
  fprintf (stderr, "\nUSAGE: %s {%sN} {%sN} file1 file2 ...\n",
	   path + basename (path), param[e_base], param[e_cols]);
  fprintf (stderr, "%s = %s\n", param[e_base], pdetail[e_base]);
  fprintf (stderr, "%s = %s\n", param[e_cols], pdetail[e_cols]);

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
