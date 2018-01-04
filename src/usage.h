#ifndef __CLI_USAGE__
#define __CLI_USAGE__

static const char USAGE[] =
R"(
Usage:
    skafos (setup|init|auth)...
    skafos init [<name>] [--template <template_name>]
    skafos templates [--update] [--search <search_term>]
    skafos logs <project_token> [-n <num>] [--tail]
    skafos -h | --help
    skafos -v | --version
Commands:
    setup       Setup development environment.
    init        Create a new project
    auth        Authenticate request.    
    logs        Get logs for a project.
Options:
    -v --version    Shows version.

If you need help, feel free to reach out:
    https://metismachine.com
    https://twitter.com/metismachine
    https://github.com/metismachine

)";

#endif
