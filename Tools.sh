#!/usr/bin/env bash

cmake=${cmake:-cmake}

OCD=/home/hmmm_fedora/Workspace/Code/MCU/ARM/AT32/OpenOCD_Linux_x86-64_V2.0.8

PjctName="$(basename "$PWD")"

function __tools_completion {
	local cur="${COMP_WORDS[COMP_CWORD]}"

	if [[ $COMP_CWORD -eq 1 ]]; then
		COMPREPLY=($(compgen -W "build clean flash" -- "$cur"))
		return 0
	fi

	if [[ $COMP_CWORD -eq 2 ]]; then
		case "${COMP_WORDS[1]}" in
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

function flash {
	"${OCD}/bin/openocd" \
		-s "${OCD}/scripts" \
		-f "${OCD}/scripts/interface/cmsis-dap.cfg" \
		-f "${OCD}/scripts/target/at32f421xx.cfg" \
		-c "program ./build/$1/${PjctName}.hex verify reset exit"
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
flash)
	if [[ $# -lt 2 ]]; then
		echo "Usage: $0 flash <preset>" >&2
		exit 1
	fi
	flash "$2"
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
