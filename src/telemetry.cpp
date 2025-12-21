#include "telemetry.h"
#include <iostream>

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

