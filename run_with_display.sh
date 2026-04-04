#!/bin/bash

set -euo pipefail

DISPLAY_NUM=99
DISPLAY_NAME=":${DISPLAY_NUM}"
LOCK_FILE="/tmp/.X${DISPLAY_NUM}-lock"
SOCKET_FILE="/tmp/.X11-unix/X${DISPLAY_NUM}"
STARTED_XVFB=0
XVFB_PID=""

cleanup() {
	if [[ "$STARTED_XVFB" -eq 1 && -n "$XVFB_PID" ]]; then
		kill "$XVFB_PID" 2>/dev/null || true
	fi
}
trap cleanup EXIT

if [[ -e "$SOCKET_FILE" ]] || pgrep -f "Xvfb ${DISPLAY_NAME}" >/dev/null; then
	# Reuse existing Xvfb server on :99
	:
else
	# Remove stale lock left by a crashed X server process.
	if [[ -f "$LOCK_FILE" ]]; then
		rm -f "$LOCK_FILE"
	fi

	Xvfb "$DISPLAY_NAME" -screen 0 1024x768x24 &
	XVFB_PID=$!
	STARTED_XVFB=1

	# Give Xvfb a moment to initialize and verify it stayed up.
	sleep 1
	if ! kill -0 "$XVFB_PID" 2>/dev/null; then
		echo "Failed to start Xvfb on ${DISPLAY_NAME}" >&2
		exit 1
	fi
fi

export DISPLAY="$DISPLAY_NAME"

# Run the provided command on the virtual display.
# If no command is given, use the project's normal launcher.
if [[ "$#" -eq 0 ]]; then
	echo "[run_with_display] DISPLAY=${DISPLAY_NAME} launching default: ./run.sh"
	"$(dirname "$0")/run.sh"
else
	echo "[run_with_display] DISPLAY=${DISPLAY_NAME} launching: $*"
	"$@"
fi
