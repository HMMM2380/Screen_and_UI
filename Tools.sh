#!/usr/bin/env bash

cmake=${cmake:-cmake}

function __tools_completion {
	local cur="${COMP_WORDS[COMP_CWORD]}"

	if [[ $COMP_CWORD -eq 1 ]]; then
		COMPREPLY=($(compgen -W "config build clean flash" -- "$cur"))
		return 0
	fi

	if [[ $COMP_CWORD -eq 2 ]]; then
		case "${COMP_WORDS[1]}" in
		config)
			COMPREPLY=($(compgen -W "Debug Release" -- "$cur"))
			;;

		build)
			COMPREPLY=($(compgen -W "Debug Release" -- "$cur"))
			;;

		clean)
			COMPREPLY=($(compgen -W "all Debug Release" -- "$cur"))
			;;
		flash)
			COMPREPLY=($(compgen -W "Debug Release" -- "$cur"))
			;;
		*)
			COMPREPLY=()
			;;
		esac
		return 0
	fi

	COMPREPLY=()
}

if [[ "${BASH_SOURCE[0]}" != "${0}" ]]; then
	complete -F __tools_completion Tools.sh
	complete -F __tools_completion ./Tools.sh
	return 0 2>/dev/null || true
fi

function config {
	${cmake} "--preset" "$1"
}

function build {
	${cmake} "--preset" "$1"
	${cmake} "--build" "--preset" "$1"
}

function clean {
	case "$1" in
	all)
		rm -rf build/*
		;;
	*)
		${cmake} "--build" "--preset" "$1" "--target" "clean"
		;;
	esac
}

case "$1" in
build)
	if [[ $# -lt 2 ]]; then
		echo "Usage: $0 build <preset>" >&2
		exit 1
	fi
	build "$2"
	;;

config)
	if [[ $# -lt 2 ]]; then
		echo "Usage: $0 config <preset>" >&2
		exit 1
	fi
	config "$2"
	;;

clean)
	if [[ $# -lt 2 ]]; then
		echo "Defualt Clean Mode: clean all"
		clean "all"
		exit 0
	fi
	clean "$2"
	;;
*)
	echo "Unknown Argument: $1" >&2
	exit 1
	;;
esac
