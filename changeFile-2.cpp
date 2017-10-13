#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/inotify.h>
#include <iostream>
#include <unistd.h>
#include <iostream>
#include <fstream>
#include <cstring>


#define EVENT_SIZE (sizeof(struct inotify_event))
#define EVENT_BUF_LEN (1024* (EVENT_SIZE+16))

int gb_prevFilePos = 0;

void openAndReadFile()
{
    char buffer[1024];
    memset(buffer,0,1024);
    std::ifstream ifile;
    int posFile=0;

    ifile.open("/tmp/test");
    ifile.seekg(gb_prevFilePos,ifile.beg);
    

    ifile.getline(buffer,1024);
    posFile=ifile.tellg();
    std::cout<<posFile<<"|"<<buffer<<"\n";
    while(ifile)
    {
        //posFile=ifile.tellg();
        ifile.getline(buffer,1024);
        if(posFile==ifile.tellg() || ifile.tellg()==-1)      
           continue;
        posFile=ifile.tellg(); 
        std::cout<<posFile<<"|"<<buffer<<"\n";                
    }
    gb_prevFilePos = posFile;
    ifile.close();
}



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

    wd =inotify_add_watch(fd,"/tmp",IN_CREATE|IN_MODIFY);

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

                if(strcmp("test",event->name)==0)
                {
                    openAndReadFile();
                }

            }
            i += EVENT_SIZE + event->len;

        }
    
    }

}
