#!/bin/bash

function usage() {
    echo "Usage: $0 -o output-file -a \"/path/to/app -param\""
    exit 1
}

cd "$(dirname "${0}")"

OUTPUT=
APP=

while : ; do
    case "$1" in
        -o)
            [ -n "$2" ] || usage ; OUTPUT="$2" ; shift 2 ;;
        -a)
            [ -n "$2" ] || usage ; APP="$2" ; shift 2 ;;
        *)
            break ;;
    esac
done

if [[ -z $OUTPUT ]] || [[ -z $APP ]] ; then
    usage
fi

if ./sample.d -o "${OUTPUT}.tmp" -c "$APP";
then
    grep -Ev '^ *(value.*)?$' "${OUTPUT}.tmp" | sed 's#^ *\([0-9]*\) |[^0-9]*\([0-9]*\)#        "\1": \2,#' > "$OUTPUT"
    rm "${OUTPUT}.tmp"
fi

