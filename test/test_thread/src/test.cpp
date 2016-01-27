//#include <iostream>
//#include <list>

//#include "thread/thread.h"

//bcc_thread_local int glocvar;
//int gcnt;
//bacchus::Mutex gmutex;

//void testId(void*) {
//    std::cout << "id: " << bacchus::Thread::id() << std::endl;
//}

//void testLocal(void*) {
//    glocvar = 2;
//    std::cout << "local: " << glocvar << std::endl;
//}

//void testLock(void*) {
//    for (int i=0; i<10000; ++i) {
//        bacchus::LockGuard lock(gmutex);
//        ++gcnt;
//    }
//}

//int main(int /*argc*/, char*/*argv*/[]) {
//    std::cout << "Hello, kittie!" << std::endl;

//    {
//    std::cout << std::endl << "Test 1: num cores" << std::endl;
//    std::cout << bacchus::Thread::procnum() << std::endl;
//    }

//    {
//    std::cout << std::endl << "Test 2: test id" << std::endl;
//    std::cout << "main thread: " << bacchus::Thread::id() << std::endl;
//    bacchus::Thread t1(testId, NULL);
//    t1.join();
//    bacchus::Thread t2(testId, NULL);
//    t2.join();
//    bacchus::Thread t3(testId, NULL);
//    t3.join();
//    }

//    {
//    std::cout << std::endl << "Test 3: thread local" << std::endl;
//    glocvar = 1;
//    std::cout << "global value: " << glocvar << std::endl;
//    bacchus::Thread t(testLocal, NULL);
//    t.join();
//    if (glocvar==1)
//        std::cout << "local not changed: OK" << std::endl;
//    else
//        std::cout << "local changed: FALSE!" << std::endl;
//    }

//    {
//    std::cout << std::endl << "Test 4: mutex locking " << std::endl;
//    gcnt = 0;
//    std::list<bacchus::Thread*> tl;
//    for(int i=0; i<100; ++i)
//        tl.push_back(new bacchus::Thread(testLock, NULL));

//    std::list<bacchus::Thread*>::iterator it;
//    for (it = tl.begin(); it != tl.end(); ++it) {
//        bacchus::Thread* t = *it;
//        t->join();
//        delete t;
//    }
//    std::cout << "count: " << gcnt << std::endl;
//    }

//    return 0;
//}
