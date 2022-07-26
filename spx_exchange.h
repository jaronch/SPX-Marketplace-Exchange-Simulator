#ifndef SPX_EXCHANGE_H
#define SPX_EXCHANGE_H

#include "spx_common.h"

#define LOG_PREFIX "[SPX]"

#define PRODUCT_STR_LEN (30)
#define COMMAND_BUF_SIZE (128)
#define GENERAL_MSG_BUF_SIZE (128)
#define MAX_INT_STR_LEN (30) //max strlen for int varaibles of order id, qty, price (0,1 - 999999)
#define MAX_COMMAND_LEN (30) // buy, sell, cancel, amend - max string length
typedef struct product_node product_node;
typedef struct trader_node trader_node;
typedef struct order order;
#define MARKET_OPEN_MSG "MARKET OPEN;"
trader_node* trader_head;
product_node* product_head;
volatile int num_of_traders = 0;
volatile int trader_pid_to_read = 0;
volatile int global_time_id = 0;
volatile long exchange_fee_total = 0;
int snprintf(char *buf, size_t size, const char *fmt, ...);

// Some function declarations
void buy_list_free(order* buy_head);
void sell_list_free(order* sell_head);
void orderbook(product_node* product_head);
void positions(trader_node* trader_head);
void matching_orders(product_node* curr_product, order* new_order);

/*Structure to help with input parsing as linked list*/
struct product_node {
    char product_name[PRODUCT_STR_LEN];
    product_node* next;
    order* buy_head;
    order* sell_head;
    long trader_qty; // only for trader orderbook usage
    long trader_price; // only for trader orderbook usage
};


/*Structure to help with storing buy orders as linked list*/
struct order {
    int is_buy;
    long qty;
    long price;
    int trader_id;
    int order_id;
    int dup_num; // num of duplicates: just for order book usage
    long dup_qty; // qty of dup - for orderbook
    int is_dup;
    int time_id; // to help when matching orders, which one earliest
    order* next;
};

/*Structure to help store trader_info*/
struct trader_node {
    int trader_id;
    int trader_pid;
    char* x2t_pipe;
    char* t2x_pipe;
    int x2t_w_fd;
    int t2x_r_fd;
    int valid_order_id;
    int trader_disconnected;
    trader_node* next;
    product_node* product_info;
};

#endif