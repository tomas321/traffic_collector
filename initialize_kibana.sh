#!/usr/bin/env bash

host="$1"
template_path="$2"
objects_path="$3"

tmp=${template_path##*/}
template_name=${tmp%.*}

# read json file ("$1") to a single json string
function readfile() {
    local data=""
    while read line; do
        data="$data$line"
    done < "$1"
    echo "$data"
}

template_data="$(readfile ${template_path})"
curl -X PUT "$host:9200/_template/$template_name" -H "Content-Type: application/json" -d"$template_data"


objects_data="$(readfile ${objects_path})"
curl -X POST "$host:5601/api/saved_objects/_bulk_create?overwrite=true" -H 'kbn-xsrf: true' -H 'Content-Type: application/json' -d"$objects_data"