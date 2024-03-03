/* 
 * echoserveri.c - An iterative echo server 
 */ 
/* $begin echoserverimain */
#include "csapp.h"
#include <string.h>

void echo(int connfd);

typedef struct{ // represents a pool of connected descriptors
	int maxfd; // largest descriptor in read_set
	fd_set read_set; // set of all active descriptors
	fd_set ready_set; // subset of descriptors ready for reading
	int nready; // number of ready descriptors from select
	int maxi; // high water index into client array
	int clientfd[FD_SETSIZE]; // set of active descriptors
	rio_t clientrio[FD_SETSIZE]; // set of active read buffers
	int client_cnt;
}pool;

typedef struct Node{
	int id;
	int left_stock;
	int price;
	struct Node *left;
	struct Node *right;
}node;

node *head = NULL;
char str[MAXLINE];

node *newNode(int id, int left_stock, int price)
{
    node *tmp = (node *)malloc(sizeof(node));
    tmp->id = id;
    tmp->left_stock = left_stock;
    tmp->price = price;
    tmp->left = NULL;
    tmp->right = NULL;
    return (tmp);
}

void init_pool(int listenfd, pool *p){ 
   // initially, there are no connected descriptors
   int i;
   p->maxi = -1;
   for(i = 0 ; i < FD_SETSIZE ; i++)
      p->clientfd[i] = -1;
   // initially, listenfd is only member of select read set
   p->maxfd = listenfd;
   p->client_cnt = 0;
   FD_ZERO(&p->read_set);
   FD_SET(listenfd, &p->read_set);
}


void add_client(int connfd, pool *p) { 
	int i;
	p->nready--;
	for(i = 0 ; i < FD_SETSIZE ; i++) //Find an available slot
		if(p->clientfd[i] < 0){
			//Add connected descriptor to the pool
			p->client_cnt++;
			p->clientfd[i] = connfd;
			Rio_readinitb(&p->clientrio[i], connfd);

			//Add the descriptor to descriptor set
			FD_SET(connfd, &p->read_set);

			//update max descriptor and pool high water mark
			if(connfd > p->maxfd)
				p->maxfd = connfd;
			if(i > p->maxi)
				p->maxi = i;
			break;
		}
	if(i == FD_SETSIZE) //couldn't find an empty slot
		app_error("add_client error : Too manty clients");
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
	
	if (head) {
		fp = fopen("stock.txt", "w");
		saveStr(head, fp);
		fclose(fp);
	}
	exit(0);
}

void close_client(pool *p, int connfd, int i) {
	FILE *fp;
	
	Close(connfd);
	FD_CLR(connfd, &p->read_set);
	p->clientfd[i] = -1;
	p->client_cnt--;
	if (p->client_cnt == 0) {
		fp = fopen("stock.txt", "w");
		saveStr(head, fp);
		head = NULL;
		fclose(fp);
	}
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

void check_clients(pool *p) { 
	int i, connfd, n;
	char buf[MAXLINE];
	rio_t rio;

	for(i = 0 ; (i <= p->maxi) && (p->nready > 0);i++) {
		connfd = p->clientfd[i];
		rio = p->clientrio[i];
 
		if((connfd > 0) && (FD_ISSET(connfd, &p->ready_set))){ 
			p->nready--;
			if((n=Rio_readlineb(&rio, buf, MAXLINE)) > 0){ // read text line
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
						node->left_stock = node->left_stock - buynum;
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
						node->left_stock = node->left_stock + sellnum;
						strcpy(buf, "[sell] success\n\0");
						Rio_writen(connfd, buf, MAXLINE);
					}
				}
				else if(!strncmp(buf, "exit", 4)){
					close_client(p, connfd, i);
				}
			}
			else{
				close_client(p, connfd, i);
			}
		}
	}
}




int main(int argc, char **argv) 
{
    int listenfd, connfd;
    socklen_t clientlen;
    struct sockaddr_storage clientaddr;  /* Enough space for any address */  //line:netp:echoserveri:sockaddrstorage
    char client_hostname[MAXLINE], client_port[MAXLINE];
    static pool pool;
    FILE *fp;

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
    init_pool(listenfd, &pool);

    while (1) {
	// Wait for listening/connected descriptors to become ready
	pool.ready_set = pool.read_set;
	pool.nready = Select(pool.maxfd+1, &pool.ready_set, NULL, NULL, NULL);

	// if listening descriptor ready, add new client to pool
	if (FD_ISSET(listenfd, &pool.ready_set)) {
	   clientlen = sizeof(struct sockaddr_storage); 
	   connfd = Accept(listenfd, (SA *)&clientaddr, &clientlen);
	   Getnameinfo((SA *) &clientaddr, clientlen, client_hostname, MAXLINE, client_port, MAXLINE, 0);
           printf("Connected to (%s, %s)\n", client_hostname, client_port);
           add_client(connfd, &pool);
	}
	check_clients(&pool);
    }
    exit(0);
}
/* $end echoserverimain */
