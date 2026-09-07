#!/bin/bash

# python_versions="$(ls /usr/bin | grep python)"
# for p in $python_versions
# do
#     version=$($p --version)
#     if [[ $version=~'^Python\s+3\.5.*$' ]]
#     then
#         printf "%s\n%s\n" $p $version
#     fi
# done
# 

python -m rasa_nlu.train -c nlu_config.yml --data nlu.md -o models --fixed_model_name nlu --project current --verbose