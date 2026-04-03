#!/bin/bash
# Start Xvfb on display :99
Xvfb :99 -screen 0 1024x768x24 &
XVFB_PID=$!

# Set DISPLAY environment variable
export DISPLAY=:99

# Wait for Xvfb to start
sleep 1

# Run the seeder simulator
"${@}"

# Kill Xvfb
kill $XVFB_PID 2>/dev/null
