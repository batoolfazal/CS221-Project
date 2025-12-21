#include "telemetry.h"
#include <iostream>
#include <vector>

TelemetryLog::TelemetryLog() : head(0), tail(0), count(0) {}

TelemetryLog::~TelemetryLog() {
    clear();
}

void TelemetryLog::clear() {
    TelemetryNode* cur = head;
    while (cur != 0) {
        TelemetryNode* nxt = cur->next;
        delete cur;
        cur = nxt;
    }
    head = 0;
    tail = 0;
    count = 0;
}

void TelemetryLog::logStep(int row, int col) {
    TelemetryNode* node = new TelemetryNode();
    node->row = row;
    node->col = col;
    node->prev = tail;
    node->next = 0;
    if (tail != 0) {
        tail->next = node;
    } else {
        head = node;
    }
    tail = node;
    count++;
}

void TelemetryLog::printLog() const {
    const TelemetryNode* cur = head;
    std::cout << "Telemetry Path (" << count << " steps): ";
    while (cur != 0) {
        std::cout << "(" << cur->row << "," << cur->col << ") ";
        cur = cur->next;
    }
    std::cout << std::endl;
}

int TelemetryLog::size() const {
    return count;
}

int TelemetryLog::toArray(TelemetryEntry* out, int maxCount) const {
    if (out == 0 || maxCount <= 0) return 0;
    int written = 0;
    const TelemetryNode* cur = head;
    while (cur != 0 && written < maxCount) {
        out[written].row = cur->row;
        out[written].col = cur->col;
        out[written].metric = 0;
        written++;
        cur = cur->next;
    }
    return written;
}

std::vector<TelemetryEntry> TelemetryLog::toVector() const {
    std::vector<TelemetryEntry> result;
    const TelemetryNode* cur = head;
    while (cur != 0) {
        TelemetryEntry e;
        e.row = cur->row;
        e.col = cur->col;
        e.metric = 0;
        result.push_back(e);
        cur = cur->next;
    }
    return result;
}

