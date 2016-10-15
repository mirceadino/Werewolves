typedef struct CommandLineArguments {
  int port;
} CommandLineArguments, *PCommandLineArguments;

int parse_command_line_arguments(int argc, char** argv,
                                 PCommandLineArguments arguments);
