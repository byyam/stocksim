var net = require('net')

var IP_ADDR = '127.0.0.1'
var IP_PORT = 8001

var client = new net.Socket();
client.connect(IP_PORT, IP_ADDR, function() {
	console.log('Connected to : ' + IP_ADDR + ':' + IP_PORT);
});


client.on('data', function(data) {
	console.log('DATA: ' + data);
	//client.destroy();
});


client.on('close', function() {
	console.log('Connection closed');
});
