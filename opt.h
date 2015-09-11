#define DSTART 1

enum __opt__
{
  e_optother = -1, e_optend = -2
};

int opt_action (int argc, const char *argv[], const char *param[], char *buff,
		unsigned int bsize, int start);

int opt_getindex (void);

void opt_setindex (int index);
