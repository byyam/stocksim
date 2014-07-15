#!/usr/bin/env python

import socket
import time
import random
import threading
from Queue import Queue

IP_ADDR = '127.0.0.1'
IP_PORT = 8001

QUEUE_LEN = 20

STOCK_NUM = 10
PROFIT = 0.01

rlock = threading.RLock()


class Pusher(threading.Thread):
	def __init__(self, thread_name, queue):
		threading.Thread.__init__(self, name = thread_name)
		self.queue = queue
		
	def run(self):
		sock=socket.socket(socket.AF_INET, socket.SOCK_STREAM)
		sock.bind((IP_ADDR, IP_PORT))
		sock.listen(5)
		while True:
			print 'listening on ', IP_ADDR, ':', IP_PORT
			connection, address = sock.accept()
			print 'Connected by ip-address: ', address
			for x in range(0, 10000):
				msg = self.queue.get(True)
				data = '{\"name\":\"' + msg['name'] + '\",' \
						+ '\"code\":\"' + str(msg['code']) + '\",' \
						+ '\"in_price\":\"' + str(msg['in_price']) + '\",' \
						+ '\"out_price\":\"' + str(msg['out_price']) + '\",' \
						+ '\"trade\":\"' + str(msg['trade']) + '\"}'
				print 'send the data: ' + data
				try:
					connection.send(data)
					time.sleep(0.1)
				except:
					print 'Disconnected.'
					connection.close()
					break


class StockSim(threading.Thread):
	def __init__(self, stock_name, stock_code, init_price, init_trade, queue):
		threading.Thread.__init__(self, name = stock_name)
		self.stock_info = {}
		self.stock_info['name'] = stock_name
		self.stock_info['code'] = stock_code
		self.stock_info['in_price'] = init_price
		self.stock_info['out_price'] = self.stock_info['in_price'] * (1 + PROFIT)
		self.stock_info['trade'] = init_trade
		self.queue = queue

	def run(self):
		while True:
			time.sleep(random.randint(1, 5))
			self.flush(random.uniform(-0.2, 0.2), random.uniform(-0.2, 0.2))
			self.send()

	def flush(self, price_float, trade_float):
		self.stock_info['in_price'] = self.stock_info['in_price'] * (1 + price_float)
		self.stock_info['trade'] = int(self.stock_info['trade'] * (1 + trade_float))
		if self.stock_info['in_price'] < 0:
			self.stock_info['in_price'] = 1
		if self.stock_info['trade'] < 0:
			self.stock_info['trade'] = 1
		self.stock_info['out_price'] = self.stock_info['in_price'] * (1 + PROFIT)
		return True

	def send(self):
		#print 'name: ', self.stock_info['name'], ', code: ', self.stock_info['code'], ', price: ', self.stock_info['in_price'], '/', self.stock_info['out_price'], ', trade: ', self.stock_info['trade']
		rlock.acquire()
		if self.queue.full():
			print 'Data-queue is full, get newer data, flush queue.'
			self.queue.queue.clear()
		try:
			self.queue.put(self.stock_info)
		except:
			print 'cannot put to queue, message lost.'
		rlock.release()
		return True


def start_sim():
	queue = Queue(QUEUE_LEN)
	t = Pusher('anaylzer', queue)
	t.daemon = True
	t.start()

	start_code = 8000
	for x in range(0, STOCK_NUM):
		stock_name = 'stock_' + str(x)
		t = StockSim(stock_name, start_code + x, random.uniform(10, 20), random.randint(3000, 10000), queue)
		t.daemon = True
		t.start()

	while True:
		time.sleep(1)

	

if __name__ == '__main__':
	start_sim()
