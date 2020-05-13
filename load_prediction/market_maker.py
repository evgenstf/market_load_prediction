#!/usr/bin/python3

import argparse
import telebot

import threading
import logging
import csv

import json

import random
from random import random
import socket
import time

def cancel_order(id):
    print("cancel_order:", id)

    content = {}

    content['type'] = 'cancel_order'
    content['id'] = id
    content['user'] = 'evgenstf'

    message = json.dumps(content).encode()

    sock = socket.socket()
    sock.connect(('localhost', 1234))
    sock.send(message)

def send_new_order(price, amount, direction):
    content = {}

    content['type'] = 'new_order'
    content['direction'] = direction
    content['price'] = price
    content['amount'] = int(amount)
    content['user'] = "evgenstf"

    print("content:", content)
    message = json.dumps(content).encode()
    print("send:", message)

    sock = socket.socket()
    sock.connect(('localhost', 1234))
    sock.send(message)

    sock.settimeout(1)
    response_string = sock.recv(10000).decode("utf-8")

    print("response_content:", response_string)

    response_content = json.loads(response_string)
    print("response_content:", response_content)

    for response in response_content:
        if response['type'] == 'user_info':
            for order in response['value']['orders']:
                if abs(order['price'] - float(price)) > 100:
                    cancel_order(order['id'])

    sock.close()

def main():
    parser = argparse.ArgumentParser()

    parser.add_argument('--historical-data', help='Path to historical data', required=True)
    parser.add_argument('--market-host', help='Market host to connect', required=False)
    parser.add_argument('--market-port', help='Market port to connect', required=False)

    args = parser.parse_args()

    historical_data = []

    with open(args.historical_data, newline='') as csvfile:
        data = csv.reader(csvfile, delimiter=',')
        cnt = 200
        for row in data:
            cnt -= 1
            """
            if cnt == 0:
                break
            """
            if 'BTCUSDT' in row:
                send_new_order(str(float(row[2]) - 1), random() * 100 + 1, 'bid')
                send_new_order(str(float(row[2]) + 1), random() * 100 + 1, 'ask')
            time.sleep(0.01)


if __name__ == "__main__":
    main()
