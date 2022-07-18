#!/bin/bash

set -xe

gcc *.c -Werror -Wall -pedantic -g -o mem
