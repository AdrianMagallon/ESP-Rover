#include "HtmlResponse.h"

void sendHtmlResponse(WiFiClient &client)
{
    client.println("HTTP/1.1 200 OK");
client.println("Content-type:text/html");
client.println();
client.println(R"rawliteral(
    <!DOCTYPE html>
    <html lang="en">
    <head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Arrow Joystick</title>
    <style>
    body {
    display: flex;
    justify-content: center;
    align-items: center;
    height: 100vh;
    background-color: #f0f0f0;
    font-family: Arial, sans-serif;
    }
    .joystick {
    display: grid;
    grid-template-areas:
    "left-forward forward right-forward"
    "left stop right"
    "left-backward backward right-backward";
    gap: 5px;
    }
    .button {
    width: 100px;
    height: 100px;
    display: flex;
    justify-content: center;
    align-items: center;
    background-color: #000000;
    color: white;
    text-decoration: none;
    border-radius: 10px;
    font-size: 24px;
    font-weight: bold;
    border: none;
    cursor: pointer;
    user-select: none;
    -webkit-tap-highlight-color: rgba(0, 0, 0, 0);
    -webkit-user-select: none;
    -khtml-user-select: none;
    }
    .button:hover {
    background-color: #0056b3;
    }
    .stop-button {
    grid-area: stop;
    }
    .left {
    grid-area: left;
    }
    .left-forward {
    grid-area: left-forward;
    }
    .forward {
    grid-area: forward;
    }
    .right-forward {
    grid-area: right-forward;
    }
    .right {
    grid-area: right;
    }
    .right-backward {
    grid-area: right-backward;
    }
    .backward {
    grid-area: backward;
    }
    .left-backward {
    grid-area: left-backward;
    }
    </style>
    <script>
    // How often (ms) to resend the active command while a button is held down.
    // The server handles one request at a time and has no memory of "held"
    // state between requests, so holding a button only keeps the robot moving
    // if the browser keeps re-sending that command for as long as it's pressed.
    const REPEAT_INTERVAL_MS = 150;
    let repeatTimer = null;
    let activeCommand = null;

    function sendCommand(command) {
    fetch(command, { method: 'POST' });
    }

    function startCommand(command) {
    if (activeCommand === command) return;
    stopCommand();
    activeCommand = command;
    sendCommand(command);
    repeatTimer = setInterval(() => sendCommand(command), REPEAT_INTERVAL_MS);
    }

    function stopCommand() {
    if (repeatTimer !== null) {
    clearInterval(repeatTimer);
    repeatTimer = null;
    }
    if (activeCommand !== null) {
    activeCommand = null;
    sendCommand('/stop');
    }
    }
    </script>
    </head>
    <body>
    <div class="joystick">
    <button class="button left-forward" onmousedown="startCommand('/leftForward')" onmouseup="stopCommand()" onmouseleave="stopCommand()" ontouchstart="startCommand('/leftForward')" ontouchend="stopCommand()">↖</button>
    <button class="button forward" onmousedown="startCommand('/forward')" onmouseup="stopCommand()" onmouseleave="stopCommand()" ontouchstart="startCommand('/forward')" ontouchend="stopCommand()">↑</button>
    <button class="button right-forward" onmousedown="startCommand('/rightForward')" onmouseup="stopCommand()" onmouseleave="stopCommand()" ontouchstart="startCommand('/rightForward')" ontouchend="stopCommand()">↗</button>
    <button class="button left" onmousedown="startCommand('/left')" onmouseup="stopCommand()" onmouseleave="stopCommand()" ontouchstart="startCommand('/left')" ontouchend="stopCommand()">←</button>
    <button class="button stop-button" onmousedown="sendCommand('/stop')" ontouchstart="sendCommand('/stop')">Stop</button>
    <button class="button right" onmousedown="startCommand('/right')" onmouseup="stopCommand()" onmouseleave="stopCommand()" ontouchstart="startCommand('/right')" ontouchend="stopCommand()">→</button>
    <button class="button left-backward" onmousedown="startCommand('/leftBackward')" onmouseup="stopCommand()" onmouseleave="stopCommand()" ontouchstart="startCommand('/leftBackward')" ontouchend="stopCommand()">↙</button>
    <button class="button backward" onmousedown="startCommand('/backward')" onmouseup="stopCommand()" onmouseleave="stopCommand()" ontouchstart="startCommand('/backward')" ontouchend="stopCommand()">↓</button>
    <button class="button right-backward" onmousedown="startCommand('/rightBackward')" onmouseup="stopCommand()" onmouseleave="stopCommand()" ontouchstart="startCommand('/rightBackward')" ontouchend="stopCommand()">↘</button>
    </div>
    </body>
    </html>
    )rawliteral");
client.println();
}
