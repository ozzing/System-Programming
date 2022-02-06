
#include "csapp.h"
#include <semaphore.h>
#define MAX 0xffff

typedef struct item {
    int ID;
    int left_stock;
    int price;
    int readcnt;
}item;

typedef struct bin {
    item it;
    int num;
    struct bin* left;
    struct bin* right;
    struct bin* next;
}bin;
bin* head;

typedef struct { /* Represents a pool of connected descr iptors */
    int maxfd; /* Largest descr iptor in read_set */
    fd_set read_set; /* Set of al l act ive descr iptors */
    fd_set ready_set; /* Subset of descr iptors ready for reading */
    int nready; /* Number of ready descr iptors from select */
    int maxi; /* High water index into cl ient array */
    int clientfd[FD_SETSIZE]; /* Set of act ive descr iptors */
    rio_t clientrio[FD_SETSIZE]; /* Set of act ive read buffers */
} pool;

void init_pool(int listenfd, pool* p)
{
    /* Ini t ial ly, there are no connected descr iptors */
    int i;
    p->maxi = -1;
    for (i = 0; i < FD_SETSIZE; i++)
        p->clientfd[i] = -1;

    /* Ini t ial ly, l istenfd is only member of select read set */
    p->maxfd = listenfd;
    FD_ZERO(&p->read_set);
    FD_SET(listenfd, &p->read_set);
}

void add_client(int connfd, pool* p)
{
    int i;
    p->nready--;
    for (i = 0; i < FD_SETSIZE; i++) /* Find an avai lable slot */
        if (p->clientfd[i] < 0) {
            /* Add connected descr iptor to the pool */
            p->clientfd[i] = connfd;
            Rio_readinitb(&p->clientrio[i], connfd);
            /* Add the descr iptor to descr iptor set */
            FD_SET(connfd, &p->read_set);
            /* Update max descr iptor and pool high water mark */
            if (connfd > p->maxfd)
                p->maxfd = connfd;
            if (i > p->maxi)
                p->maxi = i;
            break;

        }
    if (i == FD_SETSIZE) /* Couldn¡¯ t find an empty slot */
        app_error("add_client error : Too many clients");
}

bin* initNode(int ID, int left_stock, int price) {
    bin* node = calloc(1, sizeof(bin));
    node->it.ID = ID;
    node->it.left_stock = left_stock;
    node->it.price = price;
    node->it.readcnt = 0;
    node->num = 0;
    node->left = NULL;
    node->right = NULL;
    return node;
}


int insert(bin* temp, bin* node) {
    if (head == NULL) {
        head = node;
        head->next = head;
    }
    else {
        temp = head->next;
        if (temp->left == NULL) {
            temp->left = node;
            temp->num++;
            if (head->right != NULL) {
                while (temp->next != NULL) {
                    temp = temp->next;
                }
                temp->next = node;
            }
        }
        else {
            if (head->right == NULL) {
                head->right = node;
                head->next = head->left;
                head->next->next = node;
            }
            else {
                temp->right = node;
                temp->num++;
                while (temp->next != NULL) {
                    temp = temp->next;
                }
                temp->next = node;
                head->next = head->next->next;
            }

        }
    }
    return 0;
}

char* show(bin* walk) {
    walk->next = head->left;
    walk = head;
    char* buf = (char*)malloc(sizeof(char) * MAX);
    strcpy(buf, "");
    char* temp = (char*)malloc(sizeof(char) * MAX);
    while (walk) {
        sprintf(temp, "%d %d %d/", walk->it.ID, walk->it.left_stock, walk->it.price);
        strcat(buf, temp);
        walk = walk->next;
    }
    return buf;
}

int buy(char* b) {
    char buf[3];
    int ID, buy_stock;
    sscanf(b, "%s %d %d", buf, &ID, &buy_stock);
    bin* walk = head;
    walk->next = head->left;
    while (walk) {
        if (walk->it.ID == ID) { // found the item
            if (walk->it.left_stock >= buy_stock) {   // if still left
                walk->it.left_stock -= buy_stock;
                return 1;
            }
        }
        walk = walk->next;
    }
    return 0;
}

int sell(char* s) {
    char buf[4];
    int ID, sell_stock;
    sscanf(s, "%s %d %d", buf, &ID, &sell_stock);
    bin* walk = head;
    walk->next = head->left;
    while (walk) {
        if (walk->it.ID == ID) { // found the item
            walk->it.left_stock += sell_stock;
            return 1;
        }
        walk = walk->next;
    }
    return 0;
}

int main(int argc, char** argv)
{
    FILE* fin = fopen("stock.txt", "r");
    int ID, left_stock, price;
    while (1) {
        bin* temp = head;
        int i = fscanf(fin, "%d%d%d", &ID, &left_stock, &price);
        if (feof(fin)) break;
        i++;
        bin* node = initNode(ID, left_stock, price);
        insert(temp, node);
    }
    /*---------------------------------------------------------------------*/

    int listenfd, connfd;
    socklen_t clientlen;
    struct sockaddr_storage clientaddr;  /* Enough space for any address */  //line:netp:echoserveri:sockaddrstorage
    char client_hostname[MAXLINE], client_port[MAXLINE];
    int n;
    char cmd[MAXLINE];
    char* buf = (char*)malloc(sizeof(char) * MAXLINE);
    rio_t rio;
    static pool pool;

    if (argc != 2) {
        fprintf(stderr, "usage: %s <port>\n", argv[0]);
        exit(0);
    }

    listenfd = Open_listenfd(argv[1]);
    init_pool(listenfd, &pool);

    while (1) {
        pool.ready_set = pool.read_set;
        pool.nready = Select(pool.maxfd + 1, &pool.ready_set, NULL, NULL, NULL);

        if (FD_ISSET(listenfd, &pool.ready_set)) {
            clientlen = sizeof(struct sockaddr_storage);
            connfd = Accept(listenfd, (SA*)&clientaddr, &clientlen);
            add_client(connfd, &pool);
            Getnameinfo((SA*)&clientaddr, clientlen, client_hostname, MAXLINE,
                client_port, MAXLINE, 0);
            printf("Connected to (%s, %s)\n", client_hostname, client_port);
        }
        /*---------------------------------------------------------------------*/
        for (int i = 0; (i <= pool.maxi) && (pool.nready > 0); i++) {
            connfd = pool.clientfd[i];
            rio = pool.clientrio[i];

            if ((connfd > 0) && (FD_ISSET(connfd, &pool.ready_set))) {
                pool.nready--;
                if ((n = Rio_readlineb(&rio, cmd, MAXLINE)) != 0) {
                    cmd[n - 1] = '\0';
                    /*---------------------------------------------------------------------*/
                    if (strcmp(cmd, "show") == 0) {
                        bin* walk = head;
                        buf = show(walk);
                        Rio_writen(connfd, buf, strlen(buf));
                        Rio_writen(connfd, "\n", 1);
                    }
                    else if (strncmp(cmd, "buy", 3) == 0) {
                        if (1 == buy(cmd)) {
                            strcpy(buf, "[buy] \x1b[32msuccess\x1b[0m");
                            Rio_writen(connfd, buf, strlen(buf));
                            Rio_writen(connfd, "\n", 1);
                        }
                        else {
                            strcpy(buf, "not enough stock left");
                            Rio_writen(connfd, buf, strlen(buf));
                            Rio_writen(connfd, "\n", 1);
                        }
                    }
                    else if (strncmp(cmd, "sell", 4) == 0) {
                        if (1 == sell(cmd)) {
                            strcpy(buf, "[sell] \x1b[32msuccess\x1b[0m");
                            Rio_writen(connfd, buf, strlen(buf));
                            Rio_writen(connfd, "\n", 1);
                        }
                        else {
                            strcpy(buf, "not existing stock item");
                            Rio_writen(connfd, buf, strlen(buf));
                            Rio_writen(connfd, "\n", 1);
                        }
                    }
                    else if (strcmp(cmd, "exit") == 0) {
                        Close(connfd);
                        FD_CLR(connfd, &pool.read_set);
                        pool.clientfd[i] = -1;
                    }
                    /*---------------------------------------------------------------------*/
                    bin* prnt = head;
                    FILE* fout = fopen("stock.txt", "w");
                    prnt = head;
                    prnt->next = head->left;
                    while (prnt) {
                        fprintf(fout, "%d %d %d\n", prnt->it.ID, prnt->it.left_stock, prnt->it.price);
                        prnt = prnt->next;
                    }
                    fclose(fout);
                    // Rio_writen(connfd, buf, n);
                }
                else {
                    Close(connfd);
                    FD_CLR(connfd, &pool.read_set);
                    pool.clientfd[i] = -1;
                }
            }
        }

     /*---------------------------------------------------------------------*/
    }

    exit(0);
}