# SPX: Marketplace Exchange Simulator
## Table of contents
* [General info](#general-info)
* [Functionalities](#functionalities)
* [Technologies](#technologies)
* [Setup](#setup)
* [Project Status](#project-status)
* [Examples of Use](#examples-of-use)

## General info
SPX is a marketplace exchange simulator, implemented with C using Inter-Process Communication (IPC) and dynamic memory allocation between various trader binaries that interact with the market exchange ‘spx_exchange.c’.

This project was completed individually as a part of uni coursework following specifications, with a final mark of 28.25/30.

## Functionalities
Traders can place orders on products listed to be traded on the exchange by communicating to exchange whether they want to BUY, SELL, CANCEL, or AMEND, with a unique order ID, and quantity and price if applicable.
As orders are placed onto the exchange, the exchange will attempt to match orders if there is a BUY and SELL order for the particular product, but only if the price of the BUY order is larger than or equal to the price of the SELL order. The exchange will charge a 1% transaction fee to the trader who placed the order last.
The exchange reports on the current available orders and trader positions after every successful order.
Trader binaries gracefully disconnect when they reach their end of their program, closing their end of pipes, freeing all memory, and cleaning up processes.

## Technologies
Project is created with:
C and its standard libraries, using IPC, dynamic memory allocation, signal techniques	

## Setup
To run this project, have C and gcc compiler installed. 
To install the gcc compiler:
```
sudo apt-get update
sudo apt install build-essential
```
To compile spx_exchange:

```
gcc spx_exchange.c -o spx_exchange
```
An already compiled binary file of spx_exchange exists in the repository for convenience.

To run spx_exchange, the first command_line argument must be a .txt file of the products, and the following command_line arguments should be the trader binaries, whose process will be communicating and receiving communication from the exchange.

Example list of products and trader binaries have been provided. To run them:
```
./spx_exchange products.txt ./tests/sell_orders1 ./tests/sell_orders2

```

## Project Status
Next step is to clean up race conditions in some specific circumstances. Then to develop trader binaries that will allow SPX to run in real-time with orders given to trader binaries process’ through command line to be communicated to the exchange process and handled accordingly. Also to create an auto-trader program that will automatically lodge in a buy order for any sell order sent to the exchange.

## Examples of Use:
To run:
```
./spx_exchange products.txt ./tests/sell_orders1 ./tests/sell_orders2
```

Trader 0 (./tests/sell_orders1) will send these commands to exchange:
```
SELL 0 GPU 5 100;
SELL 1 GPU 10 99;
SELL 2 GPU 10 100;
SELL 3 GPU 115 100;
```

Trader 1 (./tests/sell_orders2) will send these commands to exchange:
```
BUY 0 GPU 100 1000;
```

The output printed to terminal is:
```
[SPX] Starting
[SPX] Trading 2 products: GPU Router
[SPX] Created FIFO /tmp/spx_exchange_0
[SPX] Created FIFO /tmp/spx_trader_0
[SPX] Starting trader 0 (./sell_orders1)
[SPX] Connected to /tmp/spx_exchange_0
[SPX] Connected to /tmp/spx_trader_0
[SPX] Created FIFO /tmp/spx_exchange_1
[SPX] Created FIFO /tmp/spx_trader_1
[SPX] Starting trader 1 (./sell_orders2)
[SPX] Connected to /tmp/spx_exchange_1
[SPX] Connected to /tmp/spx_trader_1
[SPX] [T0] Parsing command: <SELL 0 GPU 5 100>
[SPX]    --ORDERBOOK--
[SPX]    Product: GPU; Buy levels: 0; Sell levels: 1
[SPX]             SELL 5 @ $100 (1 order)
[SPX]    Product: Router; Buy levels: 0; Sell levels: 0
[SPX]    --POSITIONS--
[SPX]    Trader 0: GPU 0 ($0), Router 0 ($0)
[SPX]    Trader 1: GPU 0 ($0), Router 0 ($0)
[SPX] [T0] Parsing command: <SELL 1 GPU 10 99>
[SPX]    --ORDERBOOK--
[SPX]    Product: GPU; Buy levels: 0; Sell levels: 2
[SPX]            SELL 5 @ $100 (1 order)
[SPX]            SELL 10 @ $99 (1 order)
[SPX]    Product: Router; Buy levels: 0; Sell levels: 0
[SPX]    --POSITIONS--
[SPX]    Trader 0: GPU 0 ($0), Router 0 ($0)
[SPX]    Trader 1: GPU 0 ($0), Router 0 ($0)
[SPX] [T0] Parsing command: <SELL 2 GPU 10 100>
[SPX]    --ORDERBOOK--
[SPX]    Product: GPU; Buy levels: 0; Sell levels: 2
[SPX]            SELL 15 @ $100 (2 orders)
[SPX]            SELL 10 @ $99 (1 order)
[SPX]    Product: Router; Buy levels: 0; Sell levels: 0
[SPX]    --POSITIONS--
[SPX]    Trader 0: GPU 0 ($0), Router 0 ($0)
[SPX]    Trader 1: GPU 0 ($0), Router 0 ($0)
[SPX] [T0] Parsing command: <SELL 3 GPU 115 100>
[SPX]    --ORDERBOOK--
[SPX]    Product: GPU; Buy levels: 0; Sell levels: 2
[SPX]            SELL 130 @ $100 (3 orders)
[SPX]            SELL 10 @ $99 (1 order)
[SPX]    Product: Router; Buy levels: 0; Sell levels: 0
[SPX]    --POSITIONS--
[SPX]    Trader 0: GPU 0 ($0), Router 0 ($0)
[SPX]    Trader 1: GPU 0 ($0), Router 0 ($0)
[SPX] Trader 0 disconnected
[SPX] [T1] Parsing command: <BUY 0 GPU 100 1000>
[SPX] Match: Order 1 [T0], New Order 0 [T1], value: $990, fee: $10.
[SPX] Match: Order 0 [T0], New Order 0 [T1], value: $500, fee: $5.
[SPX] Match: Order 2 [T0], New Order 0 [T1], value: $1000, fee: $10.
[SPX] Match: Order 3 [T0], New Order 0 [T1], value: $7500, fee: $75.
[SPX]    --ORDERBOOK--
[SPX]    Product: GPU; Buy levels: 0; Sell levels: 1
[SPX]            SELL 40 @ $100 (1 order)
[SPX]    Product: Router; Buy levels: 0; Sell levels: 0
[SPX]    --POSITIONS--
[SPX]    Trader 0: GPU -100 ($9990), Router 0 ($0)
[SPX]    Trader 1: GPU 100 ($-10090), Router 0 ($0)
[SPX] Trader 1 disconnected
[SPX] Trading completed
[SPX] Exchange fees collected: $100
```
