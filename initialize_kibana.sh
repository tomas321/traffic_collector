#!/usr/bin/env bash

if [[ $# != 1 ]]; then
    echo "Error: 'requires 1 argument': USAGE: $0 <host>" > /dev/stderr
    exit 1
fi

host="$1"

# only execute if netcat is installed
command -v nc 1>/dev/null 2>/dev/null
if [[ $? == 0 ]]; then
    nc -vz "$host" 9200 2>/dev/null
    if [[ $? != 0 ]]; then
        echo "Error: cannot connect to Elasticsearch daemon" > /dev/stderr
        exit 2
    fi
    nc -vz "$host" 5601 2>/dev/null
    if [[ $? != 0 ]]; then
        echo "Error: cannot connect to Kibana daemon" > /dev/stderr
        exit 2
    fi
else
    echo "Warning: 'missing netcat': unable to verify connection to daemons" > /dev/stderr
fi

script_dir="$(dirname $0)"
template_path="$script_dir/resources/elk/bp_index_template.json"
objects_path="$script_dir/resources/elk/saved_objects.json"

tmp=${template_path##*/}
template_name=${tmp%.*}

# read json file ("$1") to a single json string
function readfile() {
    local data=""
    while read -r line; do
        data="$data$line"
    done < "$1"
    echo "${data}"
}

template_data="$(readfile ${template_path})"
curl -X PUT "$host:9200/_template/$template_name" -H "Content-Type: application/json" -d "$template_data" 1>/dev/null


objects_data="$(readfile ${objects_path})"
curl -X POST "$host:5601/api/saved_objects/_bulk_create?overwrite=true" -H 'kbn-xsrf: true' -H 'Content-Type: application/json' -d "$(echo ${objects_data})" 1>/dev/null

url="$host:5601/api/kibana/settings/visualization:regionmap:showWarnings"
curl -X POST "$url" -H "Content-Type: application/json" -H "kbn-xsrf: true" -d '{"value": false}' 1>/dev/null

exit 0
