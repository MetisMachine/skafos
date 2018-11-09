#ifndef __CLI_USAGE__
#define __CLI_USAGE__

static const char USAGE[] =
R"(
Usage:
    skafos (setup|init|auth)...
    skafos init [<name>] [--org <org_name>] [--template <template_name>] [--master]
    skafos templates [--update] [--search <search_term>]
    skafos env [<key>] [--set <value>] [--delete]
    skafos create [<kind>] [<name>] [--project <token>]
    skafos logs [<project_token>] [-n <num>] [--tail]
    skafos fetch --table <table_name>
    skafos kill [<project_token>] [--deployments <deployment_ids>] [--job_ids <job_ids>]
    skafos remote info [<project_token>]
    skafos orgs [<name>] [--set-default]
    skafos whoami
    skafos models (list [<name>] [--deployment <deployment_id] [--job <job_id>]|download <name> [-o <output_path>]) [--project <token>] [--tag <tag>] [-v version]
    skafos -h | --help
    skafos --version
Commands:
    setup         Setup development environment.
    init          Create a new project.
    auth          Authenticate request.
    templates     Manage templates.
    create        Create a Job for a project.
    env           Get or set environment variables.
    logs          Get logs for a project.
    fetch         Fetch results from a given table.
    kill          Kill an entire project or specific jobs/deployments.
    remote info   Print command to add a new remote. 
    orgs          List or set your default organization.
    whoami        List your current user info and settings.
    models        List or download models for a project.
Options:
    --version    Shows version.


If you need help, feel free to reach out:
    https://metismachine.com
    https://twitter.com/metismachine
    https://github.com/metismachine

Fire it up!
)";

#endif
