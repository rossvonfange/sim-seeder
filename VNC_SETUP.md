# ISOBUS Seeder Simulator - VNC Viewer Setup

When you run the debug configuration (F5), the application will launch on a virtual X11 display.

## To View the Application Window:

1. **Install a VNC Viewer:**
   - On Linux: `sudo apt install -y vncviewer` or use TigerVNC
   - On macOS: Download RealVNC Viewer or TigerVNC
   - On Windows: Download VNC Viewer from https://www.realvnc.com/en/connect/download/viewer/

2. **Connect to the Virtual Display:**
   - VNC Server: `localhost:5900`
   - Password: None (no password required)

3. **Run the debugger:**
   - Press F5 in VS Code
   - The VNC server will start automatically
   - Open your VNC Viewer and connect to `localhost:5900`
   - You'll see the ISOBUS Seeder Simulator GUI running

## How It Works:

- **Xvfb**: Virtual X11 display server (`:99`)
- **x11vnc**: VNC server that exposes the virtual display
- The application runs on the virtual display and you connect remotely via VNC

## Alternative (Without VNC):

If you want to run the app locally with a physical display:
```bash
cd /home/cycix/sim-seeder/build
./seeder-simulator
```

This will work if you have an X11 display available (e.g., SSH with X11 forwarding, or a desktop environment).
