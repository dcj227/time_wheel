#include <unistd.h>
#include <sys/time.h>
#include <iostream>

#include <time_wheel.h>

class TestItem : public DcTimer {
public:
    int i_;
};

int main() {
    struct timeval tv;
    gettimeofday(&tv, NULL);

    uint64_t now_ms = tv.tv_sec*1000 + tv.tv_usec/1000;
    //std::cout << now_ms << std::endl; 
    DcTimeWheel time_wheel(60 * 1000, now_ms);

    for (int i = 0; i < 10; i++) {
        {
        TestItem* tmp = new TestItem();
        tmp->i_ = i;
        
        time_wheel.AddTimer(tmp, i * 1000);
        }
{
        TestItem* tmp1 = new TestItem();
        tmp1->i_ = i+1;
        
        time_wheel.AddTimer(tmp1, i * 1000);
}
{
        TestItem* tmp1 = new TestItem();
        tmp1->i_ = i+2;
        
        time_wheel.AddTimer(tmp1, i * 1000);
}
    }

    for (;;) {
        struct timeval tv;
        gettimeofday(&tv, NULL);

        uint64_t now_ms1 = tv.tv_sec*1000 + tv.tv_usec/1000;
        //std::cout << now_ms << std::endl; 

        if (now_ms1 - now_ms > 20 * 1000) {
            break;
        }

        std::vector<DcTimer*> ret = time_wheel.PeriodicCheckExpired(now_ms1);
        for (uint32_t i = 0; i < ret.size(); i++) {

            std::cout << ((TestItem*)ret[i])->i_ << std::endl; 
            delete ((TestItem*)ret[i]);
        }

        usleep(100 * 1000);
    }


    return 0;
}
