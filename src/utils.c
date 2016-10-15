#include "utils.h"

#include <stdio.h>

int parse_command_line_arguments(int argc, char** argv,
                                 PCommandLineArguments arguments)
{
  arguments->port = -1;

  for (int i = 1; i < argc; ++i)
  {
    if (sscanf(argv[1], "--port = %d", &arguments->port) != 1)
    {
      arguments->port = -1;
    }
  }

  if (arguments->port == -1)
  {
    fprintf(stderr, "Error: port needs to be passed as a flag.\n"
            "Use --port = x, where x is the number of the port.\n");
    return 0;
  }

  return 1;
}
