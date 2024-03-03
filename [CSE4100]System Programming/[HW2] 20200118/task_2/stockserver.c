/* 
 * echoserveri.c - An iterative echo server 
 */ 
/* $begin echoserverimain */
#include "csapp.h"
#include <string.h>

#define NTHREADS 50
#define SBUFSIZE 100

typedef struct {
	int* buf;          /* Buffer array */
	int n;             /* Maximum number of slots */
	int front;         /* buf[(front+1)%n] is first item */
	int rear;          /* buf[rear%n] is last item */
	sem_t mutex;       /* Protects accesses to buf */
	sem_t slots;       /* Counts available slots */
	sem_t items;       /* Counts available items */
} sbuf_t;

typedef struct Node{
	int id;
	int left_stock;
	int price;
	struct Node *left;
	struct Node *right;
	sem_t mutex;
	sem_t w;
	int readnt;
}node;

node *head = NULL;
char str[MAXLINE];
sbuf_t sbuf;
int thread_cnt;
sem_t mutex;

void sbuf_init(sbuf_t *sp, int n){
	sp->buf = Calloc(n, sizeof(int));
	sp->n = n; // buffer holds max of n items
	sp->front = sp->rear = 0; // empty buffer if front == rear
	Sem_init(&sp->mutex, 0, 1); // binary semaphore for locking
	Sem_init(&sp->slots, 0, n); // initially, buf has n empty slots
	Sem_init(&sp->items, 0, 0); 
}

void sbuf_deinit(sbuf_t *sp){ 
	Free(sp->buf);
}

void sbuf_insert(sbuf_t *sp, int item){ // insert item onto the rear of shared buffer
	P(&sp->slots); // wait for available slot
	P(&sp->mutex); // lock buffer
	sp->buf[(++sp->rear)%(sp->n)] = item; // insert item
	V(&sp->mutex); // unlock buffer
	V(&sp->items); // announce available item
}

int sbuf_remove(sbuf_t *sp){ // remove and return the first item from buffer
	int item;
	P(&sp->items); // wait for available item
	P(&sp->mutex); // lock buffer
	item = sp->buf[(++sp->front)%(sp->n)]; // remove item
	V(&sp->mutex); // unlock buffer
	V(&sp->slots); // announce available slot
	return item;
}

node *newNode(int id, int left_stock, int price)
{
    node *tmp = (node *)malloc(sizeof(node));
    tmp->id = id;
    tmp->left_stock = left_stock;
    tmp->price = price;
    tmp->left = NULL;
    tmp->right = NULL;
    Sem_init(&(tmp->mutex), 0, 1);
    Sem_init(&(tmp->w), 0, 1);
    return (tmp);
}


void saveStr(node *root, FILE *fp) {
	if (root) {
		fprintf(fp, "%d %d %d\n", root->id, root->left_stock, root->price);
		saveStr(root->left, fp);
		saveStr(root->right, fp);
		free(root);
		}
}

void sig_handler(int sig)
{
	FILE *fp;
	
	P(&mutex);
	if (head) {
		fp = fopen("stock.txt", "w");
		saveStr(head, fp);
		fclose(fp);
	}
	V(&mutex);
	exit(0);
}

void close_client(int connfd) {
	FILE *fp;
	
	P(&mutex);
	Close(connfd);
		fp = fopen("stock.txt", "w");
		saveStr(head, fp);
		head = NULL;
		fclose(fp);
	V(&mutex);
}

node *insertNode(node *root, int id, int left_stock, int price)
{
    if (root == NULL)
	return newNode(id, left_stock, price);
    else {
	if (id > root->id)
		root->left = insertNode(root->left, id, left_stock, price);
	else
		root->right = insertNode(root->right, id, left_stock, price);
   }
   return root;
}

void makeStr(node *root)
{
	char tmp[11];
	
	if (root == NULL)
		return ;
	sprintf(tmp, "%d", root->id);
	strcat(str, tmp);
	strcat(str, " ");
	memset(tmp, '\0', strlen(tmp));

	sprintf(tmp, "%d", root->left_stock);
	strcat(str, tmp);
	strcat(str, " ");
	memset(tmp, '\0', strlen(tmp));

	sprintf(tmp, "%d", root->price);
	strcat(str, tmp);
	strcat(str, "\n");
	memset(tmp, '\0', strlen(tmp));
	
	makeStr(root->left);
	makeStr(root->right);
}

node *findId(node *root, int id) {

	while (root) {
	if (id == root->id)
		return root;
	else if (id > root->id)
		root = root->left;
	else
		root = root->right;
	}
	return NULL;
}

void execFunc(int connfd) { 
	int n;
	char buf[MAXLINE];
	rio_t rio;

	Rio_readinitb(&rio, connfd);
	while ((n=Rio_readlineb(&rio, buf, MAXLINE)) != 0){ // read text line
		printf("Server received %d bytes on fd %d\n", n, connfd);
			
		if(!strncmp(buf, "show", 4)){ 
			memset(str, '\0', MAXLINE); 
			makeStr(head); 
			Rio_writen(connfd, str, MAXLINE);
		} 
		else if(!strncmp(buf, "buy", 3)){ 
			int buyid, buynum;
			buf[strlen(buf) - 1] = '\0';
			memset(buf,' ', 3);
			sscanf(buf, "%d %d",&buyid, &buynum);
			node *node = findId(head, buyid); 
			if(node == NULL){ // no node
				strcpy(buf, "Invalid stock ID\n\0");
				Rio_writen(connfd, buf, MAXLINE);
			}
					
			else if(node->left_stock < buynum){ // there is not enough stock
				strcpy(buf, "Not enough left stock\n\0");
				Rio_writen(connfd, buf, MAXLINE);
			}
			else{ // there is enough stock
				P(&(node->mutex));
				node->readnt--;
				if ((node->readnt) == 0)
					V(&(node->w));
				V(&(node->mutex));
	
				P(&(node->w));
				node->left_stock = node->left_stock - buynum;
				V(&(node->w));
				strcpy(buf, "[buy] success\n\0");
				Rio_writen(connfd, buf, MAXLINE);
			} 
				
		}
		else if(!strncmp(buf, "sell", 4)){
			int sellid, sellnum;
			buf[strlen(buf) - 1] = '\0';
			memset(buf, ' ', 4);
			sscanf(buf, "%d %d",&sellid, &sellnum);
			node *node = findId(head, sellid); // find node from binary tree
			if(node == NULL){ // there is no node
				strcpy(buf, "Invalid stock ID\n\0");
				Rio_writen(connfd, buf, MAXLINE);
			}
			else{
				P(&(node->mutex));
				node->readnt--;
				if ((node->readnt) == 0)
					V(&(node->w));
				V(&(node->mutex));

				P(&(node->w));
				node->left_stock = node->left_stock + sellnum;
				V(&(node->w));
				strcpy(buf, "[sell] success\n\0");
				Rio_writen(connfd, buf, MAXLINE);
			}
		}
		else if(!strncmp(buf, "exit", 4))
			close_client(connfd);
	}
}

void *thread(void *vargp) {
	Pthread_detach(pthread_self());
	while (1) {
		int connfd = sbuf_remove(&sbuf);
		execFunc(connfd);
		Close(connfd);
	}
}

int main(int argc, char **argv) 
{
    int i, listenfd, connfd;
    socklen_t clientlen;
    struct sockaddr_storage clientaddr;  /* Enough space for any address */  //line:netp:echoserveri:sockaddrstorage
    char client_hostname[MAXLINE], client_port[MAXLINE];
    FILE *fp;
    pthread_t tid;
    thread_cnt = 0;

    Signal(SIGINT, sig_handler);
    	
    if (argc != 2) {
	fprintf(stderr, "usage: %s <port>\n", argv[0]);
	exit(0);
    }
    fp = fopen("stock.txt", "r");
    if (!fp) {
	printf("Error: file does not exist.\n");
	return (0);
    }
    else {
	int id, stock, price;
	while (fscanf(fp, "%d %d %d\n", &id, &stock, &price) != EOF) 
		head = insertNode(head, id, stock, price);
	fclose(fp);
    }

    listenfd = Open_listenfd(argv[1]);
    Sem_init(&mutex, 0, 1);
    sbuf_init(&sbuf, SBUFSIZE);

    for(i = 0; i < NTHREADS; i++)
	Pthread_create(&tid, NULL, thread, NULL);

    while (1) {
	   clientlen = sizeof(struct sockaddr_storage); 
	   connfd = Accept(listenfd, (SA *)&clientaddr, &clientlen);
	   Getnameinfo((SA *) &clientaddr, clientlen, client_hostname, MAXLINE, client_port, MAXLINE, 0);
           printf("Connected to (%s, %s)\n", client_hostname, client_port);
           sbuf_insert(&sbuf, connfd);
	   thread_cnt++;
	}
    exit(0);
}
