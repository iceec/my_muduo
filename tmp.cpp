#include <iostream>
#include <thread>
#include <unistd.h>
#include <sys/syscall.h>
using namespace std;
__thread int a;
int j;
void show(const char * p)
{
int t;
cout<<p<<": ";

cout<<&t<<endl;
cout<<&a<<endl;

cout<<::syscall(SYS_gettid)<<endl;   // 给的是真正的线程号   但是this_thread 不是 

while(1);


}

int main()
{
    cout << this_thread::get_id() << endl;
    cout<<&j<<endl;
    thread j1(show,"j1");
    thread j2(show,"j2");

    j1.join();
    j2.join();



    while (1)
        ;
}