#include "time_wheel.h"

uint32_t DcTimeWheel::PRECISION_MS = 10;

DcTimeWheel::DcTimeWheel(uint64_t max_timeout_ms, uint64_t now_ms)
    : max_timeout_ms_(max_timeout_ms)
    , precision_ms_(PRECISION_MS)
    , slot_num_(max_timeout_ms / PRECISION_MS + 10)
    , timer_slot_(new DcTimer*[slot_num_])
    , first_slot_time_ms_(now_ms / PRECISION_MS * PRECISION_MS)
    , cur_slot_index_(0)
    , cur_slot_time_ms_(now_ms / PRECISION_MS * PRECISION_MS) {

    for (uint32_t i = 0; i < slot_num_; i++) {
        timer_slot_[i] = new DcTimer();
    }
}

DcTimeWheel::~DcTimeWheel() {
    for (uint32_t i = 0; i < slot_num_; i++) {

        DcTimer* tmp = timer_slot_[i];

        if (!tmp) {
            continue;
        }

        while (tmp->prev_ != tmp && tmp->next_ != tmp) {
            tmp->next_->Relieve(); 
        }

        delete tmp;
        tmp = NULL;
    } 

    delete[] timer_slot_;
    timer_slot_ = NULL;
}

int DcTimeWheel::AddTimer(DcTimer* t, uint64_t timeout_ms) {
    if (timeout_ms > max_timeout_ms_) {
        return -1;
    }

    int slot_offset = timeout_ms % precision_ms_ == 0 ? timeout_ms / precision_ms_ : timeout_ms / precision_ms_ + 1;
    DcTimer* tmp = timer_slot_[(cur_slot_index_ + slot_offset) % slot_num_];
    tmp->InsertAfter(t);

    return 0;
}

void DcTimeWheel::DelTimer(DcTimer* t) {
    t->Relieve(); 
}

int DcTimeWheel::ResetTimer(DcTimer* t, uint64_t timeout_ms) {
    t->Relieve(); 
    return AddTimer(t, timeout_ms);
}

std::vector<DcTimer*> DcTimeWheel::PeriodicCheckExpired(uint64_t now_ms) {

    std::vector<DcTimer*> expireds;

    for (uint32_t i = 0; cur_slot_time_ms_ < now_ms && i < slot_num_; i++) {
        DcTimer* tmp = timer_slot_[cur_slot_index_];

        while (tmp->prev_ != tmp && tmp->next_ != tmp) {
            DcTimer* item = tmp->next_;
            tmp->next_->Relieve(); 

            expireds.push_back(item); 
        }

        cur_slot_time_ms_ += precision_ms_;
        cur_slot_index_++; 
    }

    return expireds;
}

