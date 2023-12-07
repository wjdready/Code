const WebSocket = require('ws');

const wss = new WebSocket.Server({ port: 8080 });

wss.on('connection', function (ws) {
    console.log('New connection');

    ws.on('message', function (message) {
        console.log('Received message: ' + message);

        // Send a message back to the client
        ws.send('Hello client!');
    });

    ws.on('close', function () {
        console.log('Connection closed');
    });
});
