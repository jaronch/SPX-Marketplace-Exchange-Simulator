
#define INPUT_FILE_NAME "./tests/sell_orders2.in"

#define _POSIX_C_SOURCE 199309L

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <sys/wait.h>
#include <strings.h>
#include <math.h>
#include <stddef.h>
#include <poll.h>

#define FIFO_EXCHANGE "/tmp/spx_exchange_%d"
#define FIFO_TRADER "/tmp/spx_trader_%d"
#define FEE_PERCENTAGE 1
int snprintf(char *buf, size_t size, const char *fmt, ...);

volatile int received_signal = 0;
// #define MAX_ID_LENGTH (10)
static void sigusr1_handler(int sig, siginfo_t* siginfo, void* context) {
    received_signal = 1;
}

int main(int argc, char ** argv) {
    if (argc < 2) {
        printf("Not enough arguments\n");
        return 1;
    }
    // register signal handler
    struct sigaction sa_sigusr1;
    sigset_t block_mask;
    sigset_t old_mask;
    sigemptyset(&block_mask);
    sigaddset(&block_mask, SIGUSR1);
    memset(&sa_sigusr1, '\0', sizeof(sa_sigusr1));
    sa_sigusr1.sa_mask = block_mask;
    sa_sigusr1.sa_sigaction = &sigusr1_handler;
    sa_sigusr1.sa_flags = SA_SIGINFO;
    sigaction(SIGUSR1, &sa_sigusr1, NULL);
    
    // connect to named pipes
    char* trader_id_str = argv[1];
    int trader_id = atoi(trader_id_str);
    int digit_num = snprintf(NULL, 0, "%d", trader_id);
    int len_x2t = strlen(FIFO_EXCHANGE) -2 + digit_num;
    int len_t2x = strlen(FIFO_TRADER) - 2 + digit_num;
    char* x2t_pipe = malloc(len_x2t+1);
    char* t2x_pipe = malloc(len_t2x+1);

    sprintf(x2t_pipe, FIFO_EXCHANGE, trader_id);
    sprintf(t2x_pipe, FIFO_TRADER, trader_id);

    int x2t_r_fd = open(x2t_pipe, O_RDONLY);
    if (x2t_r_fd == -1) {
        perror("mkfifo x2t_r_fd open error\n");
        return 1;
    }
    int t2x_w_fd = open(t2x_pipe, O_WRONLY);
    if (t2x_w_fd == -1) {
        perror("mkfifo t2x_w_fd open error\n");
        return 1;
    }

    FILE *test_input_file = fopen(INPUT_FILE_NAME, "r");
	if (test_input_file == NULL) {
		perror("Test input file does not exist\n");
		return 1;
	}
	sleep(3);
	char line[50];
	// product_node* product_head;
    while (fgets(line, sizeof(line), test_input_file) != NULL) {
        char* new_line = strtok(line, "\n");
        write(t2x_w_fd, new_line, strlen(new_line));
        kill(getppid(), SIGUSR1);
        sleep(1);
    }
    close(x2t_r_fd);
    close(t2x_w_fd);
    unlink(x2t_pipe);
    unlink(t2x_pipe);
    free(x2t_pipe);
    free(t2x_pipe);
}
