#include <stdio.h>
#include <string.h>
#include <curses.h>
struct node{
    char statement[40];
    int next;
};
struct node textbuffer[25];
int free_head;
int inuse_head;
int size=0;

void adjust();
void edit(char *filename){
    FILE *fp=NULL;
    fp = fopen(filename,"r");
    if(fp==NULL){
        fp=fopen(filename,"a");
        free_head=0;
        inuse_head=0;
    }
    else{
        while(fgets(textbuffer[size].statement, sizeof(textbuffer[size].statement),fp) != NULL){
            if(size != 0){
                textbuffer[size-1].next=size;
                textbuffer[size-1].statement[strlen(textbuffer[size-1].statement)-1]='\0';
            }
            size++;
        }
        inuse_head=0;
        adjust();
    }
}
void insert(int line, char *stat){
    if(size!=0){
        if(line > size){
            int tmp;
            strcpy(textbuffer[free_head].statement,stat);
            tmp=inuse_head;
            for(int i=1;i<size;i++){
                tmp=textbuffer[tmp].next;
            }
            textbuffer[tmp].next=free_head;
            size++;
            adjust();
        }
        else {
            int tmp=inuse_head;
            int tmp2=inuse_head;
            strcpy(textbuffer[free_head].statement,stat);
            for(int i=1;i<line;i++){
                tmp=textbuffer[tmp].next;
            }
            if(line+1 != 1){
                tmp2=textbuffer[tmp].next;
                textbuffer[tmp].next=free_head;
            }
            textbuffer[free_head].next=tmp2;
            if(line+1 == 1) inuse_head = free_head;
            size++;
            adjust();
        }
    }
    else{
        strcpy(textbuffer[size].statement,stat);
        inuse_head=size;
        size++;
        adjust();
    }
}
void delete(int line){
    int tmp = inuse_head;
    int tmp2 = inuse_head;
    for(int i=1;i<line-1;i++){
        tmp=textbuffer[tmp].next;
    }
    if(line != 1){
        tmp2=textbuffer[tmp].next;
        textbuffer[tmp].next=textbuffer[tmp2].next;
        strcpy(textbuffer[tmp2].statement,"");
        size--;
        adjust();
    }
    else{
        inuse_head = textbuffer[tmp].next;
        strcpy(textbuffer[tmp].statement,"");
        size--;
        adjust();
    }
}
void save(char *filename){
    FILE *fp =NULL;
    int tmp=inuse_head;
    fp=fopen(filename,"w");
    for(int i=0;i<size;i++){
        if(i==size-1) fprintf(fp,"%s",textbuffer[tmp].statement);
        else fprintf(fp,"%s\n",textbuffer[tmp].statement);
        tmp=textbuffer[tmp].next;
    }
}
void print(){
    int tmp=inuse_head;
    for(int i=0;i<size;++i){
        printw("%s\n",textbuffer[tmp].statement);
        tmp=textbuffer[tmp].next;
    }
}
void adjust(){
    for(int j=0;j<=size;j++){
        if(strlen(textbuffer[j].statement)==1){
            free_head=j;
            break;
        }
        if((j==size) && size !=25 ) free_head=size;
    }
}
 int main(int argc,char **argv){
    char filename[20],text[40],input;
    int cursor=0;
    strcpy(filename,argv[1]);
    edit(filename);
    cursor=size;
    initscr();//Open the window
    while(true) {
		if(input == 'I'|| input =='i'){
            if(size>0){
                clear();
                printw("I");
                refresh();
            }
            printw("\n");
            refresh();
            scanw("%s",text);
            insert(++cursor,text);
        }
        else if(input == 'D' || input =='d'){
                delete(cursor+1);

        }
        else if(input == 'S'|| input =='s'){
                save(filename);
            }
        else if(input == 'W'|| input =='w'){
            if(cursor!=0)//Top Limit
                move(--cursor,0);
        }
        else if(input == 'Z'|| input =='z'){
            if(cursor!=24)//Bottom Limit
                move(++cursor,0);
        }
        else if(input=='X' || input=='x'){
                endwin();//Close the window
                return 0;
        }

	    clear();//clear before displaying
        print();//print the buffer
        refresh();//display the buffer
        move(cursor,0);//Update the cursor
        input=getch();//Get the next instruction
    }
    endwin();//Close the window
}
