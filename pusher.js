var net = require('net')

var SERVER_IP_ADDR = '127.0.0.1'
var SERVER_IP_PORT = 8001
var PUSH_IP_PORT = 8231

var client = new net.Socket();
client.connect(SERVER_IP_PORT, SERVER_IP_ADDR, function() {
	console.log('Connected to : ' + SERVER_IP_ADDR + ':' + SERVER_IP_PORT);
});


//-----------protobufjs: format push message
var ProtoBuf = require("protobufjs");

var builder = ProtoBuf.loadProtoFile("message.proto"),
	stock = builder.build("stock"),
	Stock = stock.entity.Stock;



//-----------zmq: pub/sub mode
var zmq = require('zmq');
var publisher = zmq.socket("pub");

publisher.bind("tcp://*:" + PUSH_IP_PORT, function(err) {
  if (err) throw err;
});

var count = 0;

client.on('data', function(data) {
	var stock_obj = JSON.parse(data.toString());
	var topic = stock_obj.name;

	var stock_msg = new Stock({
		"name": stock_obj.name,
		"code": stock_obj.code,
		"in_price": stock_obj.in_price,
		"out_price": stock_obj.out_price,
		"trade": stock_obj.trade
	});
	var buffer = stock_msg.encode();
	publisher.send(topic + ' ' + buffer.toArrayBuffer());
});


client.on('close', function() {
	console.log('Connection closed');
});

client.on('error', function(e){
	console.log(e);
});

client.on('end', function(){
	console.log('Client disconnected');
});
