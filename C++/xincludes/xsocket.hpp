#ifndef XSOCKET_HPP
#define XSOCKET_HPP

#include <iostream>
#include <string>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

using namespace std;

class XTcpServer
{
public:
    void Run();
    virtual void DoWork(int sockfd, const struct sockaddr_in& client_addr);
    bool Listen(uint32_t port, int domain=AF_INET, int type=SOCK_STREAM, int backlog=9);

private:
    int m_sockfd;
};

bool XTcpServer::Listen(uint32_t port, int domain, int type, int backlog)
{
    int ret = -1;
    struct sockaddr_in server_addr;

    server_addr.sin_family = domain;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr.s_addr = INADDR_ANY;
    bzero(&(server_addr.sin_zero), 8);

    m_sockfd = socket(domain, type, 0);
    if( m_sockfd == -1 )
    {
        cerr << "socket() execute fail." << endl;
        return false;
    }

    ret = bind(m_sockfd, (const struct sockaddr*)&server_addr, sizeof(struct sockaddr));
    if( ret == -1 )
    {
        cerr << "bind() execute fail." << endl;
        return false;
    }

    ret = listen(m_sockfd, backlog);
    if( ret == -1 )
    {
        cerr << "listen() execute fail." << endl;
        return false;
    }

    return true;
}

void XTcpServer::Run()
{
    int new_fd = -1;
    socklen_t sin_size = 0;
    struct sockaddr_in client_addr;

    sin_size = sizeof(struct sockaddr_in);
    while(true)
    {
        bzero(&client_addr, sin_size);
        new_fd = accept(m_sockfd, (struct sockaddr*)&client_addr, &sin_size);
        if( new_fd == -1 )
        {
            cerr << "accept() execute fail." << endl;
            exit(EXIT_FAILURE);
        }

        DoWork(new_fd, client_addr);

        close(new_fd);
    }
}

void XTcpServer::DoWork(int sockfd, const struct sockaddr_in& client_addr)
{
    send(sockfd, "XTcpServer:I accept your request.", string("XTcpServer:I accept your request.").length(), 0);
}

#endif
