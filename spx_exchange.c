/**
 * comp2017 - assignment 3
 * <Jaron Choe>
 * <jcho9719>
 */


#include "spx_exchange.h"

/*Initialise linked list to store product names*/
product_node* list_init(char* product_name) {
    product_node* product_head = malloc(sizeof(product_node));
    if (product_head == NULL) {
        return NULL;
    }
    strcpy(product_head->product_name, product_name);
    product_head->next = NULL;
    product_head->buy_head = NULL;
    product_head->sell_head = NULL;
    product_head->trader_qty = 0;
    product_head->trader_price = 0;
    return product_head;
};

/*Adds next product_node to list*/
void list_add(product_node* product_head, char* product_name) {
    while (product_head->next) {
        product_head = product_head->next;
    }
    product_head->next = list_init(product_name);
};

void list_print(struct product_node* product_head) {
    if (product_head == NULL) {
        return;
    }
    while (product_head != NULL) {
		if (product_head->next == NULL) {
			printf("%s\n", product_head->product_name);
			break;
		}
        printf("%s ", product_head->product_name);
        product_head = product_head->next;
    }
};

/*Frees all existing nodes in the list*/
void list_free(product_node* product_head) {
    if (product_head == NULL) {
        return;
    }

    product_node* curr = product_head;
    product_node* prev = product_head;
    while (curr) {
        curr = prev->next;
        if (prev->buy_head != NULL) {
            buy_list_free(prev->buy_head);
            prev->buy_head = NULL;
        }
        if (prev->sell_head != NULL) {
            sell_list_free(prev->sell_head);
            prev->sell_head = NULL;
        }
        free(prev);
        prev = curr;
    }
};

/*Initialise linked list to store buy orders*/
order* buy_list_init(long buy_qty, long buy_price, int trader_id, int order_id) {
    order* buy_head = malloc(sizeof(order));
    if (buy_head == NULL) {
        return NULL;
    }
    buy_head->is_buy = 1;
    buy_head->qty = buy_qty;
    buy_head->price = buy_price;
    buy_head->trader_id = trader_id;
    buy_head->order_id = order_id;
    buy_head->dup_num = 0;
    buy_head->dup_qty = 0;
    buy_head->is_dup = 0;
    buy_head->time_id = global_time_id;
    global_time_id += 1;
    buy_head->next = NULL;
    return buy_head;
};

/*Adds next buy order to list*/
void buy_list_add(order* buy_head, long buy_qty, long buy_price, int trader_id, int order_id) {
    while (buy_head->next) {
        buy_head = buy_head->next;
    }
    buy_head->next = buy_list_init(buy_qty, buy_price, trader_id, order_id);
};

/*Frees all existing nodes in the list*/
void buy_list_free(order* buy_head) {
    if (buy_head == NULL) {
        return;
    }

    order* curr = buy_head;
    order* prev = buy_head;
    while (curr) {
        curr = prev->next;
        free(prev);
        prev = curr;
    }
};

/*Initialise linked list to store buy orders*/
order* sell_list_init(long sell_qty, long sell_price, int trader_id, int order_id) {
    order* sell_head = malloc(sizeof(order));
    if (sell_head == NULL) {
        return NULL;
    }
    sell_head->is_buy = 0;
    sell_head->qty = sell_qty;
    sell_head->price = sell_price;
    sell_head->trader_id = trader_id;
    sell_head->order_id = order_id;
    sell_head->dup_num = 0;
    sell_head->dup_qty = 0;
    sell_head->is_dup = 0;
    sell_head->time_id = global_time_id;
    global_time_id += 1;
    sell_head->next = NULL;
    return sell_head;
};

/*Adds next buy order to list*/
void sell_list_add(order* sell_head, long sell_qty, long sell_price, int trader_id, int order_id) {
    while (sell_head->next) {
        sell_head = sell_head->next;
    }
    sell_head->next = sell_list_init(sell_qty, sell_price, trader_id, order_id);
};

/*Frees all existing nodes in the list*/
void sell_list_free(order* sell_head) {
    if (sell_head == NULL) {
        return;
    }

    order* curr = sell_head;
    order* prev = sell_head;
    while (curr) {
        curr = prev->next;
        free(prev);
        prev = curr;
    }
};

// search for order in given product, given trader and order ids
order* order_search(product_node* curr_product, int trader_id, int order_id) {
    order* buy_cursor = curr_product->buy_head;
    while (buy_cursor != NULL) {
        if (buy_cursor->trader_id == trader_id && buy_cursor->order_id == order_id) {
            return buy_cursor;
        }
        buy_cursor = buy_cursor->next;
    }
    order* sell_cursor = curr_product->sell_head;
    while (sell_cursor != NULL) {
        if (sell_cursor->trader_id == trader_id && sell_cursor->order_id == order_id) {
            return sell_cursor;
        }
        sell_cursor = sell_cursor->next;
    }
    return NULL;
}

/*Helper function to check product_name valid.
Returns pointer to product struct if exists, else, returns NULL*/
product_node* product_search(product_node* product_head, char* product_name) {
    product_node* cursor = product_head;
    while (cursor != NULL) {
        if (strcmp(cursor->product_name, product_name) == 0) {
            return cursor;
        }
        cursor = cursor->next;
    }
    return NULL;
};

/* Removes empty buy orders from current product*/
void remove_empty_buy(product_node* curr_product) {
    order* temp = curr_product->buy_head;
    order* prev = temp;
    // if head needs to be freed
    if (temp != NULL && temp->qty == 0) {
        curr_product->buy_head = temp->next; 
        free(temp);
        return;
    }
 
    while (temp != NULL && temp->qty != 0) {
        prev = temp;
        temp = temp->next;
    }
    if (temp == NULL) {
        return;
    }
    // reached here means that temp needs to be freed
    prev->next = temp->next;
    free(temp);
    return;
}

/* Removes empty buy orders from current product*/
void remove_empty_sell(product_node* curr_product) {
    order* temp = curr_product->sell_head;
    order* prev = temp;
    // if head needs to be freed
    if (temp != NULL && temp->qty == 0) {
        curr_product->sell_head = temp->next; 
        free(temp);
        return;
    }
 
    while (temp != NULL && temp->qty != 0) {
        prev = temp;
        temp = temp->next;
    }
    if (temp == NULL) {
        return;
    }
    // reached here means that temp needs to be freed
    prev->next = temp->next;
    free(temp);
    return;
}

/*Initialise trader linked list to store trader info*/
trader_node* trader_list_init(int trader_id, int trader_pid, 
char* x2t_pipe, char* t2x_pipe, int x2t_w_fd, int t2x_r_fd) {
    trader_node* trader_head = malloc(sizeof(trader_node));
    if (trader_head == NULL) {
        return NULL;
    }
    trader_head->trader_id = trader_id;
    trader_head->trader_pid = trader_pid;
    trader_head->x2t_pipe = x2t_pipe;
    trader_head->t2x_pipe = t2x_pipe;
    trader_head->x2t_w_fd = x2t_w_fd;
    trader_head->t2x_r_fd = t2x_r_fd;
    trader_head->next = NULL;
    trader_head->product_info = NULL;
    trader_head->valid_order_id = 0;
    trader_head->trader_disconnected = 0;
    return trader_head;
};

/*Adds next product_node to list*/
void trader_list_add(trader_node* trader_head, int trader_id, int trader_pid, 
char* x2t_pipe, char* t2x_pipe, int x2t_w_fd, int t2x_r_fd) {
    while (trader_head->next) {
        trader_head = trader_head->next;
    }
    trader_head->next = trader_list_init(trader_id, trader_pid, 
        x2t_pipe, t2x_pipe, x2t_w_fd, t2x_r_fd);
};

void trader_init_product_info(trader_node* trader_head, product_node* product_info) {
    trader_node* cursor = trader_head;
    while (cursor != NULL) {
        if (cursor->product_info == NULL) {
            cursor->product_info = product_info;
            return;
        }
        cursor = cursor->next;
    }
}

// updates product info when order matched
void update_product_info(trader_node* curr_trader, char* product_name, long qty, long value, int is_buy) {
    product_node* product_info_head = curr_trader->product_info;
    product_node* curr_product = product_search(product_info_head, product_name);
    if (is_buy == 1) {
        curr_product->trader_price -= value;
        curr_product->trader_qty += qty;
    } else { // sell
        curr_product->trader_price += value;
        curr_product->trader_qty -= qty;
    }
}

void update_valid_order_id(trader_node* curr_trader) {
    curr_trader->valid_order_id += 1;
}

int check_valid_order_id(trader_node* curr_trader, int new_order_id) {
    if (curr_trader->valid_order_id == new_order_id) {
        return 1;
    }
    return 0;
}

/*Frees all existing nodes in the list*/
void trader_list_free(trader_node* trader_head) {
    if (trader_head == NULL) {
        return;
    }

    trader_node* curr = trader_head;
    trader_node* prev = trader_head;
    while (curr) {
        curr = prev->next;
        list_free(prev->product_info);
        close(prev->x2t_w_fd);
        close(prev->t2x_r_fd);
        unlink(prev->x2t_pipe);
        unlink(prev->t2x_pipe);
        free(prev->t2x_pipe);
        free(prev->x2t_pipe);
        free(prev);
        prev = curr;
    }
};

/*Helper function to return trader node, doesn't exist returns NULL*/
trader_node* trader_search(trader_node* trader_head, int trader_id) {
    trader_node* cursor = trader_head;
    while (cursor != NULL) {
        if (trader_id == cursor->trader_id) {
            return cursor;
        }
        cursor = cursor->next;
    }
    return NULL;
};

void amend_order(product_node* curr_product, int trader_id, int order_id, int qty, int price) {
    order* order_to_amend = order_search(curr_product, trader_id, order_id);
    order_to_amend->qty = qty;
    order_to_amend->price = price;
    order_to_amend->time_id = global_time_id;
    global_time_id += 1;
}

int valid_to_amend_cancel(product_node* curr_product, int trader_id, int order_id) {
    order* potential_order = order_search(curr_product, trader_id, order_id);
    if (potential_order == NULL) {
        return 0;
    }
    return 1;
}

void cancel_order(product_node* curr_product, int trader_id, int order_id) {
    order* order_to_cancel = order_search(curr_product, trader_id, order_id);
    order_to_cancel->qty = 0;
    order_to_cancel->price = 0;
    if (order_to_cancel->is_buy == 1) {
        remove_empty_buy(curr_product);
    } else {
        remove_empty_sell(curr_product);
    }
}

int order_is_buy(product_node* curr_product, int trader_id, int order_id) {
    order* curr_order = order_search(curr_product, trader_id, order_id);
    if (curr_order->is_buy == 1) {
        return 1;
    } else {
        return 0;
    }
}

void trader_send_market_open(trader_node* trader_head) {
    if (trader_head == NULL) {
        return;
    }
    while (trader_head != NULL) {
        if (write(trader_head->x2t_w_fd, MARKET_OPEN_MSG, strlen(MARKET_OPEN_MSG)) == -1) {
            perror("Market open failure\n");
			return;
        }
        trader_head = trader_head->next;
    }
}

void trader_send_sigusr1(trader_node* trader_head) {
    if (trader_head == NULL) {
        return;
    }
    while (trader_head != NULL) {
        kill(trader_head->trader_pid, SIGUSR1);
        trader_head = trader_head->next;
    }
}

int count_words(const char str[]) {
    int total = 1;
    int i = 0;
    while (str[i] != '\0') {
        if (str[i] == ' ' || str[i] == '\n' || str[i] == '\t') {
            total += 1;
        }
        i += 1;
    }
  	return total;
}

/*read command given pid*/
void trader_place_order(trader_node* trader_head, product_node* product_head, int trader_pid) {
    trader_node* cursor = trader_head;
    int curr_trader_id;
    while (cursor != NULL) {
        if (cursor->trader_pid == trader_pid) {
            curr_trader_id = cursor->trader_id;
            break;
        }
        cursor = cursor->next;
    }

    // read command from t2x_pipe
    char buffer[COMMAND_BUF_SIZE] = "";
    char buff = 0;
    while (buff != ';') {
        // printf("Was here\n");
        read(cursor->t2x_r_fd, &buff, 1);
        // printf("1\n");
        if (buff == ';') break;
        strncat(buffer, &buff, 1);
        // printf("%s\n", &buff);
    }
    // buffer[strlen(buffer)-1] = '\0';


    // parse trader's message
    char buffer_read[COMMAND_BUF_SIZE];
    strcpy(buffer_read, buffer);
    int invalid = 0;
    char order_id_str[MAX_INT_STR_LEN] = {0};
    char product[PRODUCT_STR_LEN];
    char qty_str[MAX_INT_STR_LEN];
    char price_str[MAX_INT_STR_LEN];
    char command_type_str[MAX_COMMAND_LEN];
    char command_type[MAX_COMMAND_LEN];
    // command_type = strtok_r(buffer_strtok, " ", &context);
    // printf("%s\n", buffer_read);
    sscanf(buffer_read, "%s", command_type);
    if  (buffer_read[0] == ' ') {
        invalid = 1;
    }
    // printf("%s\n", command_type);
	// buffer_read[strlen(buffer_read)-1] = '\0';
    // How about for commands with too many words?
    if (strcasecmp(command_type, "BUY") == 0 || 
        strcasecmp(command_type, "SELL") == 0) {
        sscanf(buffer_read, "%s %s %s %s %s", command_type_str,
            order_id_str, product, qty_str, price_str);
        if (count_words(buffer_read) != 5) {
            invalid = 1;
        }
    } else if (strcasecmp(command_type, "AMEND") == 0) {
        sscanf(buffer_read, "%s %s %s %s", command_type_str,
            order_id_str, qty_str, price_str);
        if (count_words(buffer_read) != 4) {
            invalid = 1;
        }
    } else if (strcasecmp(command_type, "CANCEL") == 0) {
        sscanf(buffer_read, "%s %s", command_type_str, order_id_str);
        if (count_words(buffer_read) != 2) {
            invalid = 1;
        }
    } else {
        invalid = 1;
    }

    // Check product is on product_list
    product_node* curr_product = product_search(product_head, product);
    if (curr_product == NULL) {
        invalid = 1;
    }
    // Check order_id, price, qty are all valid
    int order_id_num = atoi(order_id_str);
    long qty_num = atoi(qty_str);
    long price_num = atoi(price_str);
    if (order_id_num == 0 && order_id_str[0] != '0') {
        invalid = 1;
    } else if (qty_num == 0 && qty_str[0] != '0') {
        invalid = 1;
    } else if (price_num == 0 && price_str[0] != '0') {
        invalid = 1;
    }
    if (qty_num < 1 || qty_num > 999999) {
        invalid = 1;
    }
    if (price_num < 1 || price_num > 999999) {
        invalid = 1;
    }

    if (strcasecmp(command_type_str, "BUY") == 0 || strcasecmp(command_type_str, "SELL") == 0) {
        if (check_valid_order_id(cursor, order_id_num) == 0) {
            invalid = 1;
        }
    } else if (strcasecmp(command_type_str, "AMEND") == 0 || strcasecmp(command_type_str, "CANCEL") == 0) {
        if (valid_to_amend_cancel(curr_product, curr_trader_id, order_id_num) == 0) {
            invalid = 1;
        }
    }
    
    printf("%s [T%d] Parsing command: <%s>\n", LOG_PREFIX, curr_trader_id, buffer_read);
    // Creating message to send back to trader
    char message_to_trader[GENERAL_MSG_BUF_SIZE];
    if (invalid == 1) {
        strcpy(message_to_trader, "INVALID;");
    } else if (strcasecmp(command_type_str, "AMEND") == 0) {
        if (order_is_buy(curr_product, curr_trader_id, order_id_num) == 1) {
            strcpy(command_type_str, "BUY");
        } else {
            strcpy(command_type_str, "SELL");
        }
        amend_order(curr_product, curr_trader_id, order_id_num, qty_num, price_num);
        sprintf(message_to_trader, "AMENDED %s;", order_id_str);
    } else if (strcasecmp(command_type_str, "CANCEL") == 0) {
        if (order_is_buy(curr_product, curr_trader_id, order_id_num) == 1) {
            strcpy(command_type_str, "BUY");
        } else {
            strcpy(command_type_str, "SELL");
        }
        qty_num = 0;
        strcpy(qty_str,"0");
        price_num = 0;
        strcpy(price_str,"0");
        cancel_order(curr_product, curr_trader_id, order_id_num);
        sprintf(message_to_trader, "CANCELLED %s;", order_id_str);
    } else {
        // increment valid_order_id
        update_valid_order_id(cursor);
        // add accepted order to linked list structure
        if (strcasecmp(command_type_str, "BUY") == 0) {
            order* curr_buy_order = curr_product->buy_head;
            if (curr_product->buy_head == NULL) {
                curr_buy_order = buy_list_init(qty_num, price_num, curr_trader_id, order_id_num);
                curr_product->buy_head = curr_buy_order;
            } else {
                buy_list_add(curr_buy_order, qty_num, price_num, curr_trader_id, order_id_num);
            }
        } else if (strcasecmp(command_type_str, "SELL") == 0) {
            order* curr_sell_order = curr_product->sell_head;
            if (curr_product->sell_head == NULL) {
                curr_sell_order = sell_list_init(qty_num, price_num, curr_trader_id, order_id_num);
                curr_product->sell_head = curr_sell_order;
            } else {
                sell_list_add(curr_sell_order, qty_num, price_num, curr_trader_id, order_id_num);
            }
        }
        sprintf(message_to_trader, "ACCEPTED %s;", order_id_str);
    }
    // Writing message to trader and notifying with signal
    if (write(cursor->x2t_w_fd, message_to_trader, strlen(message_to_trader)) == -1) {
        perror("Market open failure\n");
        return;
    }
    kill(trader_pid, SIGUSR1);

    // Writing message to other traders and notify with signal
    if (invalid == 0) {
        // creating message
        char message_to_others[GENERAL_MSG_BUF_SIZE];
        // if (strcasecmp(command_type_str, "CANCEL") == 0) {
        //     // TO DO!!!
        // } else {
        //     sprintf(message_to_others, "MARKET %s %s %s %s;", command_type_str,
        //         product, qty_str, price_str);
        // }
        sprintf(message_to_others, "MARKET %s %s %s %s;", command_type_str,
            product, qty_str, price_str);
    
        // writing to traders
        trader_node* cursor2 = trader_head;
        while (cursor2 != NULL) {
            if (cursor2->trader_pid == trader_pid || cursor2->trader_disconnected == 1) {
                cursor2 = cursor2->next;
                continue;
            } // important to use strlen here instead of sizeof when using buffer array
            if (write(cursor2->x2t_w_fd, message_to_others, strlen(message_to_others)) == -1) {
                perror("Market open failure\n");
                return;
            }
            cursor2 = cursor2->next;
        }
        // sending signal to traders
        cursor2 = trader_head;
        while (cursor2 != NULL) {
            if (cursor2->trader_pid == trader_pid || cursor2->trader_disconnected == 1) {
                cursor2 = cursor2->next;
                continue;
            }
            kill(cursor2->trader_pid, SIGUSR1);
            cursor2 = cursor2->next;
        }
        order* new_order = order_search(curr_product, curr_trader_id, order_id_num);
        if (strcasecmp(command_type, "CANCEL") != 0) {
            matching_orders(curr_product, new_order);
        }
        orderbook(product_head);
        positions(trader_head);
    

    }  
}

int price_sort_comparator(const void* a, const void * b) {
  order*  orderA = *(order**) a;
  order* orderB = *(order**) b;
  return (orderB->price - orderA->price);
}

int sell_order_comparator(const void* a, const void * b) {
  order*  orderA = *(order**) a;
  order* orderB = *(order**) b;
  int compare_result = orderA->price - orderB->price;
  if (compare_result == 0) {
      int global_time_diff = orderA->time_id - orderB->time_id;
      return global_time_diff;
  }
  return compare_result;
}

int buy_order_comparator(const void* a, const void * b) {
  order*  orderA = *(order**) a;
  order* orderB = *(order**) b;
  int compare_result = orderB->price - orderA->price;
  if (compare_result == 0) {
      int global_time_diff = orderA->time_id - orderB->time_id;
      return global_time_diff;
  }
  return compare_result;
}

void matching_orders(product_node* curr_product, order* new_order) {
    // if not at least one BUY and one SELL, return as no match possible
    if (curr_product->buy_head == NULL || curr_product->sell_head == NULL) {
            return;
    }

    if (new_order->is_buy == 1) { // buy order trying to match with a sell order
        int num_sell_orders = 0;
        order* sell_cursor = curr_product->sell_head;
        while (sell_cursor != NULL) {
            num_sell_orders += 1;
            sell_cursor = sell_cursor->next;
        }

        // mallocing an array of orders (just used buy_order for struct)
        // exact same as sell_order struct size, so can use to store both types
        order** sell_list = malloc(sizeof(order*)*num_sell_orders);

        int index = 0;
        sell_cursor = curr_product->sell_head;
        while (sell_cursor != NULL) {
            sell_list[index] = sell_cursor;
            index += 1;
            sell_cursor = sell_cursor->next;
        }
        // sorted sell orders in terms of price and time
        qsort(sell_list, num_sell_orders, sizeof(order*), sell_order_comparator);
        
        // loop through sell prices from lowest to highest
        index = 0;
        while (index < num_sell_orders) {
            if (sell_list[index]->price <= new_order->price) {
                long match_qty = 0;
                long match_value = 0;
                int old_order_id = sell_list[index]->order_id;
                int old_trader_id = sell_list[index]->trader_id;
                int new_order_id = new_order->order_id;
                int new_trader_id = new_order->trader_id;
                int order_fully_filled = 0;
                
                if (sell_list[index]->qty >= new_order->qty) {
                    match_qty = new_order->qty;
                    sell_list[index]->qty -= match_qty;
                    new_order->qty = 0;
                    match_value = match_qty*sell_list[index]->price;
                    order_fully_filled = 1;
                    // removing when empty
                    remove_empty_buy(curr_product);
                    
                    if (sell_list[index]->qty == 0) {
                        remove_empty_sell(curr_product);
                        // num_sell_orders -= 1;
                    } else {
                        // num_sell_orders -= 1;
                    }
                } else {
                    match_qty = sell_list[index]->qty;
                    sell_list[index]->qty = 0;
                    new_order->qty -= match_qty;
                    match_value = match_qty*sell_list[index]->price;

                    // only sell_list[index] should be empty
                    remove_empty_sell(curr_product);
                    // num_sell_orders -= 1;
                }
                long exchange_fee = match_value*(FEE_PERCENTAGE/100.0) + 0.5;
                printf("%s Match: Order %d [T%d], New Order %d [T%d], value: $%ld, fee: $%ld.\n", 
                    LOG_PREFIX, old_order_id, old_trader_id,
                    new_order_id, new_trader_id, match_value, exchange_fee);
                
                trader_node* old_trader = trader_search(trader_head, old_trader_id);
                trader_node* new_trader = trader_search(trader_head, new_trader_id);
                
                // update the trader positions
                update_product_info(old_trader, curr_product->product_name, match_qty, match_value, 0);
                update_product_info(new_trader, curr_product->product_name, match_qty, match_value+exchange_fee, 1);

                // composing message to traders
                char message_to_old_trader[GENERAL_MSG_BUF_SIZE];
                char message_to_new_trader[GENERAL_MSG_BUF_SIZE];
                sprintf(message_to_old_trader, "FILL %d %ld;", old_order_id, match_qty);
                sprintf(message_to_new_trader, "FILL %d %ld;", new_order_id, match_qty);
                
                 // Writing message to trader and notifying with signal
                if (new_trader->trader_disconnected != 1) {
                    if (write(new_trader->x2t_w_fd, message_to_new_trader, strlen(message_to_new_trader)) == -1) {
                        perror("Market open failure\n");
                        return;
                    }
                    kill(new_trader->trader_pid, SIGUSR1);
                }
                if (old_trader->trader_disconnected != 1) {
                    if (write(old_trader->x2t_w_fd, message_to_old_trader, strlen(message_to_old_trader)) == -1) {
                        perror("Market open failure\n");
                        return;
                    }
                    kill(old_trader->trader_pid, SIGUSR1);
                }
                
                
                //incrememt global exchange total
                exchange_fee_total += exchange_fee;

                // if no more than break
                if (order_fully_filled == 1) {
                    break;
                }
            }   
            index += 1;
        }
        free(sell_list);
    } else { // sell order trying to match with a buy order
        int num_buy_orders = 0;
        order* buy_cursor = curr_product->buy_head;
        while (buy_cursor != NULL) {
            num_buy_orders += 1;
            buy_cursor = buy_cursor->next;
        }

        order** buy_list = malloc(sizeof(order*)*num_buy_orders);

        int index = 0;
        buy_cursor = curr_product->buy_head;
        while (buy_cursor != NULL) {
            buy_list[index] = buy_cursor;
            index += 1;
            buy_cursor = buy_cursor->next;
        }
        // sorted buy orders in terms of price and time
        qsort(buy_list, num_buy_orders, sizeof(order*), buy_order_comparator);
        
        // loop through buy prices from lowest to highest
        index = 0;
        while (index < num_buy_orders) {
            if (buy_list[index]->price >= new_order->price) {
                long match_qty = 0;
                long match_value = 0;
                int old_order_id = buy_list[index]->order_id;
                int old_trader_id = buy_list[index]->trader_id;
                int new_order_id = new_order->order_id;
                int new_trader_id = new_order->trader_id;
                int order_fully_filled = 0;

                if (buy_list[index]->qty >= new_order->qty) {
                    match_qty = new_order->qty;
                    buy_list[index]->qty -= match_qty;
                    new_order->qty = 0;
                    match_value = match_qty*buy_list[index]->price;
                    order_fully_filled = 1;
                    // sell order will be empty
                    remove_empty_sell(curr_product);
                    
                    if (buy_list[index]->qty == 0) {
                        remove_empty_buy(curr_product);
                        // num_buy_orders -= 1;
                    } else {
                        // num_buy_orders -= 1;
                    }
                } else {
                    match_qty = buy_list[index]->qty;
                    buy_list[index]->qty = 0;
                    new_order->qty -= match_qty;
                    match_value = match_qty*buy_list[index]->price;

                    remove_empty_buy(curr_product);
                    // num_buy_orders -= 1;
                }
                long exchange_fee = match_value*(FEE_PERCENTAGE/100.0) + 0.5;
                printf("%s Match: Order %d [T%d], New Order %d [T%d], value: $%ld, fee: $%ld.\n", 
                    LOG_PREFIX, old_order_id, old_trader_id,
                    new_order_id, new_trader_id, match_value, exchange_fee);
                
                trader_node* old_trader = trader_search(trader_head, old_trader_id);
                trader_node* new_trader = trader_search(trader_head, new_trader_id);
                
                // update the trader positions
                update_product_info(old_trader, curr_product->product_name, match_qty, match_value, 1);
                update_product_info(new_trader, curr_product->product_name, match_qty, match_value-exchange_fee, 0);

                // composing message to traders
                char message_to_old_trader[GENERAL_MSG_BUF_SIZE];
                char message_to_new_trader[GENERAL_MSG_BUF_SIZE];
                sprintf(message_to_old_trader, "FILL %d %ld;", old_order_id, match_qty);
                sprintf(message_to_new_trader, "FILL %d %ld;", new_order_id, match_qty);
                // Writing message to trader and notifying with signal   
                if (old_trader->trader_disconnected != 1) {
                    if (write(old_trader->x2t_w_fd, message_to_old_trader, strlen(message_to_old_trader)) == -1) {
                        perror("Market open failure\n");
                        return;
                    }
                    kill(old_trader->trader_pid, SIGUSR1);
                }
                if (new_trader->trader_disconnected != 1) {
                    if (write(new_trader->x2t_w_fd, message_to_new_trader, strlen(message_to_new_trader)) == -1) {
                        perror("Market open failure\n");
                        return;
                    }
                    kill(new_trader->trader_pid, SIGUSR1);
                }
                
                //incrememt global exchange total
                exchange_fee_total += exchange_fee;

                // if no more, than break
                if (order_fully_filled == 1) {
                    break;
                }
            }   
            index += 1;
        }
        free(buy_list);
    }
}

void orderbook(product_node* product_head) {
    printf("%s\t--ORDERBOOK--\n", LOG_PREFIX);
    product_node* cursor = product_head;
    while (cursor != NULL) {
        // go to next product if no orders
        if (cursor->buy_head == NULL && cursor->sell_head == NULL) {
            printf("%s\tProduct: %s; Buy levels: 0; Sell levels: 0\n", LOG_PREFIX, cursor->product_name);
            cursor = cursor->next;
            continue;
        }
        // else we will add orders to a list and sort them to print to screen
        int num_buy_orders = 0;
        order* buy_cursor = cursor->buy_head;
        while (buy_cursor != NULL) {
            num_buy_orders += 1;
            buy_cursor = buy_cursor->next;
        }

        int num_sell_orders = 0;
        order* sell_cursor = cursor->sell_head;
        while (sell_cursor != NULL) {
            num_sell_orders += 1;
            sell_cursor = sell_cursor->next;
        }

        int num_of_orders = num_buy_orders + num_sell_orders;
        // mallocing an array of orders (just used buy_order for struct)
        // exact same as sell_order struct size, so can use to store both types
        order** orderbook_list = malloc(sizeof(order*)*num_of_orders);

        int index = 0;
        buy_cursor = cursor->buy_head;
        while (buy_cursor != NULL) {
            orderbook_list[index] = buy_cursor;
            index += 1;
            buy_cursor = buy_cursor->next;
        }

        sell_cursor = cursor->sell_head;
        while (sell_cursor != NULL) {
            orderbook_list[index] = sell_cursor;
            index += 1;
            sell_cursor = sell_cursor->next;
        }

        qsort(orderbook_list, num_of_orders, sizeof(order*), price_sort_comparator);

        // Will now go through to correctly present the num of dup prices at each level
        // Also calculate num buy_levels and sell_levels
        int buy_levels = 0;
        int sell_levels = 0;
        int i = 0;
        order* curr = orderbook_list[0];
        while (i < num_of_orders) {
            curr = orderbook_list[i];
            // if (curr == NULL) {
            //     break;
            // }
            if (curr->is_buy == 1) {
                buy_levels += 1;
            } else {
                sell_levels += 1;
            }
            int j = i + 1;
            if (j >= num_of_orders) {
                break;
            }
            order* future_dup = orderbook_list[j];
    
            while (future_dup->price == curr->price) {
                if (future_dup->is_dup != 1) {
                    curr->dup_num += 1;
                    curr->dup_qty += future_dup->qty;
                    future_dup->is_dup = 1;
                }
                j += 1;

                if (j >= num_of_orders) {
                    break;
                }
                future_dup = orderbook_list[j];
            }
            i = j;
        }

        // Everything is ready to print
        printf("%s\tProduct: %s; Buy levels: %d; Sell levels: %d\n", LOG_PREFIX, cursor->product_name,
            buy_levels, sell_levels);
        i = 0;
        curr = orderbook_list[0];
        while (i < num_of_orders) {
            curr = orderbook_list[i];
            // if (curr == NULL) {
            //     break;
            // }
            // printing
            char* str;
            if (curr->is_buy == 1) {
                str = "BUY";
            } else {
                str = "SELL";
            }
            if (curr->dup_num == 0) {
                printf("%s\t\t%s %ld @ $%ld (1 order)\n", LOG_PREFIX, str, curr->qty, curr->price);
            } else {
                printf("%s\t\t%s %ld @ $%ld (%d orders)\n", LOG_PREFIX, str, 
                    curr->qty + curr->dup_qty, curr->price, curr->dup_num + 1);
            }

            int j = i + 1;
            if (j >= num_of_orders) {
                break;
            }
            order* future_dup = orderbook_list[j];
            while (future_dup->is_dup == 1) {
                j += 1;
                if (j >= num_of_orders) {
                    break;
                }
                future_dup = orderbook_list[j];
            }
            i = j;
        }
        i = 0;
        while (i < num_of_orders) {
            curr = orderbook_list[i];
            curr->dup_num = 0;
            curr->dup_qty = 0;
            curr->is_dup = 0;
            i += 1;
        }

        free(orderbook_list);
        orderbook_list = NULL;
        cursor = cursor->next;
    }
}
void positions(trader_node* trader_head) {
    printf("%s\t--POSITIONS--\n", LOG_PREFIX);
    trader_node* cursor = trader_head;
    while (cursor != NULL) {
        printf("%s\tTrader %d: ", LOG_PREFIX, cursor->trader_id);
        product_node* product_cursor = cursor->product_info;
        printf("%s %ld ($%ld)", product_cursor->product_name, 
            product_cursor->trader_qty, product_cursor->trader_price);
        product_cursor = product_cursor->next;
        while (product_cursor != NULL) {
            printf(", %s %ld ($%ld)", product_cursor->product_name,
                product_cursor->trader_qty, product_cursor->trader_price);
            product_cursor = product_cursor->next;
        }
        printf("\n");
        cursor = cursor->next;
    }
}

/*returns trader id given pid*/
int find_disconnected_trader_id(trader_node* trader_head, int child_pid, int fd) {
    while (trader_head != NULL) {
        if (child_pid == trader_head->trader_pid 
            || fd == trader_head->t2x_r_fd) {
            if (trader_head->trader_disconnected == 1) {
                return -1;
            }
            trader_head->trader_disconnected = 1;
            num_of_traders -= 1;
            return trader_head->trader_id;
        }     
        trader_head = trader_head->next;
    }
	return 0;
}

void sigchld_handler(int sig) {
    pid_t child_pid;
    while ((child_pid = waitpid(-1, NULL, WNOHANG)) > 0) {
        int trader_id = find_disconnected_trader_id(trader_head, child_pid, -1);
        if (trader_id != -1) {
            printf("%s Trader %d disconnected\n", LOG_PREFIX, trader_id);
        }
        
    }
}

static void sigusr1_handler(int sig, siginfo_t* siginfo, void* context) {
	trader_pid_to_read = siginfo->si_pid;
    trader_place_order(trader_head, product_head, trader_pid_to_read);
}

#ifndef TESTING
int main(int argc, char **argv) {
    printf("%s Starting\n", LOG_PREFIX);
	if (argc < 2) {
		perror("Not enough arguments\n");
		return 1;
	}

    // Sigaction set up
    struct sigaction sa_sigchld;
    sa_sigchld.sa_handler = &sigchld_handler;
    sa_sigchld.sa_flags = 0;
    sigaction(SIGCHLD, &sa_sigchld, NULL);
    num_of_traders = argc - 2;

	struct sigaction sa_sigusr1;
    sigset_t block_mask;
    sigset_t old_mask;
    sigemptyset(&block_mask);
    sa_sigchld.sa_mask = block_mask;
    sigaddset(&block_mask, SIGUSR1);
    sigaddset(&block_mask, SIGCHLD);
	memset(&sa_sigusr1, '\0', sizeof(sa_sigusr1));
    sa_sigusr1.sa_mask = block_mask;
    sa_sigusr1.sa_sigaction = &sigusr1_handler;
    sa_sigusr1.sa_flags = SA_SIGINFO;
    sigaction(SIGUSR1, &sa_sigusr1, NULL);
    
	// Open and check product file actually exists
	FILE *product_file = fopen(argv[1], "r");
	if (product_file == NULL) {
		perror("Product file does not exist\n");
		return 1;
	}
	
	char line[PRODUCT_STR_LEN];
	int i = 0;
	int num_of_products;
	// product_node* product_head;
    while (fgets(line, sizeof(line), product_file) != NULL) {
		if (i == 0) {
			// Check if integer?!?
			num_of_products = atoi(line);
		} else if (i == 1) {
			char* new_head = strtok(line,"\n");
			product_head = list_init(new_head);
		} else {
			char* new_product = strtok(line, "\n");
			list_add(product_head, new_product);
		}
        i += 1;
    }

    printf("%s Trading %d products: ", LOG_PREFIX, num_of_products);
    list_print(product_head);

    struct pollfd* pollfds = malloc(sizeof(struct pollfd)*(argc-2));
    char* x2t_pipe;
    char* t2x_pipe;
	char* trader_id_str;
	i = 2;
	while (i < argc) {
        // Setting up names of named pipes
        int digit_num = snprintf(NULL, 0, "%d", i - 2);
        int len_x2t = strlen(FIFO_EXCHANGE) - 2 + digit_num;
        int len_t2x = strlen(FIFO_TRADER) - 2 + digit_num;
        x2t_pipe = malloc(len_x2t + 1);
        t2x_pipe = malloc(len_t2x + 1);
        if (i == 2) {
			trader_id_str = malloc(digit_num + 1);
        } else {
			trader_id_str = realloc(trader_id_str, digit_num + 1);
        }
        sprintf(x2t_pipe, FIFO_EXCHANGE, i - 2);
        sprintf(t2x_pipe, FIFO_TRADER, i - 2);
		sprintf(trader_id_str, "%d", i -2);
        printf("%s Created FIFO %s\n", LOG_PREFIX, x2t_pipe);
        printf("%s Created FIFO %s\n", LOG_PREFIX, t2x_pipe);
        // Creating pipes with mkfifo
        if (mkfifo(x2t_pipe, S_IRWXU | S_IRWXG) < 0) {
            if (errno != EEXIST) {
                perror("Mkfifo error1\n");
                return 1;
            }
        }
        if (mkfifo(t2x_pipe, S_IRWXU | S_IRWXG) < 0) {
            if (errno != EEXIST) {
                perror("Mkfifo error2\n");
                return 1;
            }
        }
        
        // Forking to launch trader binaries
        int pid = fork();
        if (pid < 0) {
            perror("Forking error\n");
            return 1;
        } else if (pid == 0) {
            // child process
            printf("%s Starting trader %d (%s)\n", LOG_PREFIX, i - 2, argv[i]);
            if (execlp(argv[i], argv[i], trader_id_str, (char*) NULL) < 0) {
                perror("execlp error for command line argument\n");
                return 1;
            }
        } else {
            int x2t_w_fd = open(x2t_pipe, O_WRONLY);
            if (x2t_w_fd == -1) {
                perror("mkfifo x2t_w_fd open error\n");
                return 1;
            }
            printf("%s Connected to %s\n", LOG_PREFIX, x2t_pipe);

            int t2x_r_fd = open(t2x_pipe, O_RDONLY);
            if (t2x_r_fd == -1) {
                perror("mkfifo t2x_r_fd open error\n");
                return 1;
            }
            printf("%s Connected to %s\n", LOG_PREFIX, t2x_pipe);
            
            // initalise polling stuff
            pollfds[i-2].fd = t2x_r_fd;
            pollfds[i-2].events = POLLHUP;

            if (i == 2) {
                trader_head = trader_list_init(i - 2, pid, x2t_pipe, 
                    t2x_pipe, x2t_w_fd, t2x_r_fd);
            } else {
                trader_list_add(trader_head, i - 2, pid, x2t_pipe, 
                    t2x_pipe, x2t_w_fd, t2x_r_fd);
            }
            // adding product info to traders
            int k = 0;
            // int num_of_products1;
            product_node* product_info;
            fseek(product_file, 0, SEEK_SET);
            while (fgets(line, sizeof(line), product_file) != NULL) {
                if (k == 0) {
                    // Check if integer?!?
                    // num_of_products1 = atoi(line);
                } else if (k == 1) {
                    char* new_head = strtok(line,"\n");
                    product_info = list_init(new_head);
                } else {
                    char* new_product = strtok(line, "\n");
                    list_add(product_info, new_product);
                }
                k += 1;
            }
            trader_init_product_info(trader_head, product_info);
        }
        i += 1;
	}
    fclose(product_file);
	trader_send_market_open(trader_head);
	trader_send_sigusr1(trader_head);

    
    while (num_of_traders > 0) {
        int result = poll(pollfds, argc-2, -1);
        if (result <= 0) {
            continue;
        } else {
            sigprocmask(SIG_BLOCK, &block_mask, &old_mask);
            for (int k = 0; k < argc - 2; k++) {
                if (pollfds[k].revents & POLLHUP) {
                    int trader_id = find_disconnected_trader_id(trader_head, -1, pollfds[k].fd);
                    if (trader_id != -1) {
                        printf("%s Trader %d disconnected\n", LOG_PREFIX, trader_id);
                    }
                    
                }
            }
            if (num_of_traders <= 0) {
                break;
            }
            sigprocmask(SIG_UNBLOCK, &block_mask, NULL);
            
        }
    }
    free(pollfds);
	free(trader_id_str);
    list_free(product_head);
    trader_list_free(trader_head);
    printf("%s Trading completed\n", LOG_PREFIX);
    printf("%s Exchange fees collected: $%ld\n", LOG_PREFIX, exchange_fee_total);
	return 0;
}
#endif