#ifndef __CLI_USAGE__
#define __CLI_USAGE__

static const char USAGE[] =
R"(
Usage:
    skafos (setup|init|auth)...
    skafos init [<name>] [--template <template_name>]
    skafos templates [--update] [--search <search_term>]
    skafos env [<key>] [--set <value>]
    skafos logs <project_token> [-n <num>] [--tail]
    skafos -h | --help
    skafos -v | --version
Commands:
    setup       Setup development environment.
    init        Create a new project
    auth        Authenticate request.    
    templates   Manage templates
    env         Get or set environment variables
    logs        Get logs for a project.
Options:
    -v --version    Shows version.


If you need help, feel free to reach out:
    https://metismachine.com
    https://twitter.com/metismachine
    https://github.com/metismachine


Skafos is currently under a private beta but don't despair 
because we are offering an opportunity for you to get early access. 

Please go to https://metismachine.typeform.com/to/JzdHLh to make it so. 

Fire it up!
)";

#endif
