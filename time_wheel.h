#ifndef __DC_TIME_WHEEL_H__
#define __DC_TIME_WHEEL_H__

#include <stdint.h>
#include <cstdlib>
#include <vector>

class DcTimer {
public:
    DcTimer() : prev_(this)
              , next_(this) {
    }

    virtual ~DcTimer() {}

    // 把t添加到当前节点的后面
    void InsertAfter(DcTimer* t) {
        t->prev_ = this;
        t->next_ = next_;

        t->next_->prev_ = t;
        next_ = t;
    }

    void Relieve() {
        this->prev_->next_ = this->next_;
        this->next_->prev_ = this->prev_;

        this->prev_ = this;
        this->next_ = this; 
    }
    
    DcTimer* DoExpired();

    DcTimer* prev_;
    DcTimer* next_;
};

class DcTimeWheel {
public:
    DcTimeWheel(uint64_t max_timeout_ms, uint64_t now_ms);
    virtual ~DcTimeWheel();

    int AddTimer(DcTimer* t, uint64_t timeout_ms);
    void DelTimer(DcTimer* t);
    int ResetTimer(DcTimer* t, uint64_t timeout_ms);

    // 创建时间轮后，必须在最大超时时间之内调用PeriodicCheckExpired函数。
    std::vector<DcTimer*> PeriodicCheckExpired(uint64_t now_ms); 

private:
    uint64_t max_timeout_ms_;
    uint32_t precision_ms_;      // interval time between slots
    uint32_t slot_num_;

    DcTimer** timer_slot_;

    uint64_t first_slot_time_ms_;   // 第一个插槽的时间
    uint32_t cur_slot_index_;       // 当前检查超时的插槽
    uint64_t cur_slot_time_ms_;     // 当前插槽表示的时间

    static uint32_t PRECISION_MS;
};

#endif  //  __DC_TIME_WHEEL_H__
