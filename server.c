#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/times.h>
#include <sys/select.h>
#include <unistd.h>

#define SERVER_BUSY "Nao ha mais conexoes disponiveis\n"

#define MAX_USERS 2
#define MSG_LEN 4096

int socket_listening ;
fd_set select_set ;

int  *list_of_sockets ;

int  max_users = MAX_USERS ;
int  list_len = 0 ;
int flag = 0;

char msg[MSG_LEN+1] ;

void show_help(char *name) {
	printf("Servidor VoidPong\n") ;
	printf("[uso] %s <port> [<max_users>]\n", name) ;
}

char insert_socket_into_list(int socket) {
    int i ;

    if ( list_len == max_users ) {
        return 1 ;
    }

    for ( i = 0; i < max_users; i++ ) {
        if ( list_of_sockets[i] == -1 ) {
            list_of_sockets[i] = socket ;
            list_len++ ;
            break ;
        }
    }
    return 0 ;
}

void remove_socket_from_list(int _sock) {
    int i ;

    for ( i = 0; i < max_users; i++ ) {
        if ( list_of_sockets[i] == _sock ) {
            close(list_of_sockets[i]) ;
            list_of_sockets[i] = -1 ;
            list_len-- ;
            break ;
        }
    }
}

char get_message_from_socket(int _sock) {
    int t ;

    memset(msg,0x0,MSG_LEN+1) ;
    t = recv(_sock, msg, MSG_LEN, 0 ) ;

    if ( t == 0 ) {
        remove_socket_from_list(_sock) ;
        return 1 ;
    }
    return 0 ;
}

void send_message_to_all(int _sock) {
    int i ;

    for ( i = 0; i < max_users; i++ ) {
        if ( (list_of_sockets[i] != -1)    &&
             (list_of_sockets[i] != _sock) &&
             (list_of_sockets[i] != socket_listening) ) {

            send(list_of_sockets[i], msg, strlen(msg), 0) ;
        }
    }
}

int main(int argc, char **argv) {
    int port;
    int t;

    struct sockaddr_in server ;
    struct timeval select_time ;

    if ( argc == 1 ) {
        show_help(argv[0]);
        return -1;
    }

    if ( argc > 2 ) {
        max_users = atoi(argv[2]);
    }

    port = atoi(argv[1]);

    socket_listening = socket(AF_INET, SOCK_STREAM, 0) ;

    if ( socket_listening < 0 ){
        perror("socket");
        return -1 ;
    }

    server.sin_family = AF_INET ;
    server.sin_port = htons(port) ;
    server.sin_addr.s_addr = INADDR_ANY ;

    t = sizeof(struct sockaddr_in) ;
    if ( bind( socket_listening, (struct sockaddr *) &server, t ) < 0 ) {
        perror("bind") ;
        return -1 ;
    }

    if ( listen(socket_listening, 5) < 0 ) {
        perror("listen") ;
        return -1 ;
    }

    list_of_sockets = (int *) malloc( max_users * sizeof(int) ) ;
    if ( list_of_sockets == NULL ) {
        perror("malloc") ;
        return -1 ;
    }

    for ( t = 0; t < max_users; t++ )
        list_of_sockets[t] = -1 ;

    while(1){
    	FD_ZERO(&select_set) ;
        FD_SET(socket_listening, &select_set) ;
        for ( t = 0; list_len > 0 && t < max_users; t++ ) {
            if ( list_of_sockets[t] != -1 ) {
                FD_SET(list_of_sockets[t], &select_set) ;
            }
        }

        printf("[+] Ouvindo na porta %d [%d/%d] ...\n", port, list_len, max_users) ;
        if(list_len==2){        	
	    	msg[0] = 'c';
        	send_message_to_all(list_of_sockets[0]);
        	send_message_to_all(list_of_sockets[1]);
        }
        select_time.tv_sec = 2 ;
        select_time.tv_usec = 0 ;

        if ( (t=select(FD_SETSIZE, &select_set, NULL, NULL, &select_time)) < 0 ) {
            perror("select") ;
            return -1 ;
        }

        if(t>0){
        	if ( FD_ISSET(socket_listening, &select_set) ) {
                int n ;

                if ( (n=accept(socket_listening, NULL, NULL)) < 0 ) {
                    perror("accept") ;
                } else if ( insert_socket_into_list(n) == 1 ) { 
                    send(n,SERVER_BUSY,strlen(SERVER_BUSY),0) ;
                    close(n) ;
                }
                continue ;
            } else {
            	if(list_len==2 && !flag){
	            	msg[0] = 'j';
		        	msg[1] = 1+'0';
		        	send_message_to_all(list_of_sockets[0]);
		        	msg[1] = 2+'0';
		        	send_message_to_all(list_of_sockets[1]);
		        	flag = 1;
		        }
                int i ;
                for ( i = 0; i < max_users; i++ ) {
                    if ( FD_ISSET(list_of_sockets[i], &select_set) ) {
                        if ( get_message_from_socket(list_of_sockets[i]) == 0 ) {
                            send_message_to_all(list_of_sockets[i]) ;
                        }
                    }
                }
            }
        }
    } 

    return 0 ;
}