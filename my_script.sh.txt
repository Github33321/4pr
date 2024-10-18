#!/bin/bash
# https://github.com/Github33321/Practica-

list_users() {
    cut -d: -f1,6 /etc/passwd | sort
}


list_processes() {
    ps -e --sort=pid
}

show_help() {
    echo "Usage: $0 [OPTIONS]"
    echo ""
    echo "Options:"
    echo "  -u, --users           Show a list of users and their home directories."
    echo "  -p, --processes       Show a list of running processes sorted by PID."
    echo "  -l, --log PATH        Redirect standard output to the file at PATH."
    echo "  -e, --errors PATH     Redirect error output to the file at PATH."
    echo "  -h, --help            Show this help message and exit."
}

check_path() {
    if [[ ! -w $(dirname "$1") ]]; then
        echo "Error: No write access to directory $(dirname "$1")" >&2
        exit 1
    fi
}

LOG_PATH=""
ERROR_PATH=""

while [[ "$#" -gt 0 ]]; do
    case "$1" in
        -u|--users)
            ACTION="users"
            ;;
        -p|--processes)
            ACTION="processes"
            ;;
        -h|--help)
            ACTION="help"
            ;;
        -l|--log)
            if [[ -z "$2" ]]; then
                echo "Error: Missing argument for $1" >&2
                exit 1
            fi
            LOG_PATH="$2"
            check_path "$LOG_PATH"
            shift
            ;;
        -e|--errors)
            if [[ -z "$2" ]]; then
                echo "Error: Missing argument for $1" >&2
                exit 1
            fi
            ERROR_PATH="$2"
            check_path "$ERROR_PATH"
            shift
            ;;
        *)
            echo "Error: Unknown argument $1" >&2
            exit 1
            ;;
    esac
    shift
done

if [[ -n "$LOG_PATH" ]]; then
    exec > "$LOG_PATH"
fi

if [[ -n "$ERROR_PATH" ]]; then
    exec 2> "$ERROR_PATH"
fi

case "$ACTION" in
    users)
        list_users
        ;;
    processes)
        list_processes
        ;;
    help)
        show_help
        ;;
    *)
        echo "Error: No action specified" >&2
        exit 1
        ;;
esac
