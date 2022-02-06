
#include "csapp.h"
#include <semaphore.h>
#define MAX 0xffff

typedef struct item {
    int ID;
    int left_stock;
    int price;
    int readcnt;
    sem_t mutex;
}item;

typedef struct bin {
    item it;
    int num;
    struct bin* left;
    struct bin* right;
    struct bin* next;
}bin;
bin* head;

typedef struct {
    int* buf;
    int n;
    int front;
    int rear;
    sem_t mutex;
    sem_t slots;
    sem_t items;
}sbuf_t;
sbuf_t sbuf;

void sbuf_init(sbuf_t *sp, int n){
    sp->buf = Calloc(n, sizeof(int));
    sp->n = n;
    sp->front = sp->rear = 0;
    Sem_init(&sp->mutex, 0, 1);
    Sem_init(&sp->slots, 0, n);
    Sem_init(&sp->items, 0, 0);
}

void sbuf_deinit(sbuf_t* sp) {
    Free(sp->buf);
}

void sbuf_insert(sbuf_t* sp, int item) {
    P(&sp->slots);
    P(&sp->mutex);
    sp->buf[(++sp->rear) % (sp->n)] = item;
    V(&sp->mutex);
    V(&sp->items);
}

int sbuf_remove(sbuf_t* sp) {
    int item;
    P(&sp->items);
    P(&sp->mutex);
    item = sp->buf[(++sp->front) % (sp->n)];
    V(&sp->mutex);
    V(&sp->slots);
    return item;
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

static sem_t mutex;
static void init_stock() {
    Sem_init(&mutex, 0, 1);
}

void stock(int connfd) {
    int n;
    char*buf=(char*)malloc(sizeof(char)*MAXLINE);
    static pthread_once_t once = PTHREAD_ONCE_INIT;
    char cmd[MAXLINE];
    rio_t rio;
    Pthread_once(&once, init_stock);
    Rio_readinitb(&rio, connfd);
    while ((n = Rio_readlineb(&rio, cmd, MAXLINE)) != 0) {
        P(&mutex);
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
        V(&mutex);
    }
}

void* thread(void* vargp) {
    Pthread_detach(pthread_self());
    while (1) {
        int connfd = sbuf_remove(&sbuf);
        stock(connfd);
        Close(connfd);
    }
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
    pthread_t tid;

    if (argc != 2) {
        fprintf(stderr, "usage: %s <port>\n", argv[0]);
        exit(0);
    }

    listenfd = Open_listenfd(argv[1]);
    sbuf_init(&sbuf, 100);
    for (int i = 0; i < 100; i++) {
        Pthread_create(&tid, NULL, thread, NULL);
    }
    while (1) {
            clientlen = sizeof(struct sockaddr_storage);
            connfd = Accept(listenfd, (SA*)&clientaddr, &clientlen);
            sbuf_insert(&sbuf, connfd);
            Getnameinfo((SA*)&clientaddr, clientlen, client_hostname, MAXLINE,
                client_port, MAXLINE, 0);
            printf("Connected to (%s, %s)\n", client_hostname, client_port);
    }

    exit(0);
}