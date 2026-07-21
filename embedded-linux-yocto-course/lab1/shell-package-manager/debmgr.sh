#!/bin/bash

if ! command -v dpkg-query &> /dev/null; then 
	echo"[ERROR] dpkg-query is not installed, are you on a DEBIAN distro ?"
	exit 1
fi

usage(){
	printf '%\n' \
	"Usage: ${0} info|install|remove|create|help ARGS" \
	""\
	" - info    : show information of a given package" \
	" - install : install a given package" \
	" - remove  : remove a given package" \
	" - create  : create a .deb package from a given directory" \
	" -help     : show this help"

	local exit_code="$([ ${1} != 1] && echo 0 || echo 1)"
	exit "${exit_code}"
}
