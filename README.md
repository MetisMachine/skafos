# Skafos CLI

## Need help? Talk to these people

1. [Wess Cope](mailto:wess@metismachine.com)
2. [Jeremy Tregunna](mailto:jeremy@metismachine.com)

## Building

### Linux

To build and install the Skafos CLI, you can run:

```
$ make install
```

If you want to put the result in a different location than `/usr/bin`, please
run it like this:

```
$ PREFIX=/opt/skafos make install
```

#### Debian

To make a Debian package you can also run:

```
$ make build_dist_deb
```

This will result in a package you can install on a debian based system.

### macOS

You can install [https://brew.sh/](Homebrew), or use the same process as Linux.

## Usage:
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
    skafos -h | --help
    skafos -v | --version
## Commands:
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
## Options:
    -V --version             Shows version.

---

If you need help, feel free to reach out:
- https://metismachine.com
- https://twitter.com/metismachine
- https://github.com/metismachine

Skafos is currently under a private beta but don't despair 
because we are offering an opportunity for you to get early access. 

Please go to https://metismachine.typeform.com/to/JzdHLh to make it so. 

Fire it up!
