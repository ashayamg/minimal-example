#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/inotify.h>
#include <iostream>
#include <unistd.h>


#define EVENT_SIZE (sizeof(struct inotify_event))
#define EVENT_BUF_LEN (1024* (EVENT_SIZE+16))

int main()
{

    int length,i=0;
    int fd,wd;
    char buffer[EVENT_BUF_LEN];

    fd=inotify_init();

    if(fd<0)
    {
        perror("inotify_init");
    }

    wd =inotify_add_watch(fd,"/tmp",IN_CREATE|IN_DELETE|IN_MODIFY);

    while(1)
    {

        length = read(fd,buffer,EVENT_BUF_LEN);
        i=0;
        if(length<0)
        {
            perror("Read length ==0");
        }


        while(i<length)
        {
            struct inotify_event * event = (struct inotify_event *)&buffer[i];

            if(event->len &&(event->mask & IN_MODIFY) && !(event->mask & IN_ISDIR) )
            {
                std::cout<<"Name: "<<event->name<<"|"<<event->wd<<"\n";
            }
            i += EVENT_SIZE + event->len;

        }
    
    }

}
