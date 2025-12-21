#ifndef TELEMETRY_H
#define TELEMETRY_H

// Doubly linked list node for telemetry
struct TelemetryNode {
    int row;
    int col;
    TelemetryNode* prev;
    TelemetryNode* next;
};

// Telemetry log manages path history
class TelemetryLog {
private:
    TelemetryNode* head;
    TelemetryNode* tail;
    int count;
public:
    TelemetryLog();
    ~TelemetryLog();

    void clear();
    void logStep(int row, int col);      // append to tail
    void printLog() const;               // console dump
    int size() const;                    // number of entries
};

#endif // TELEMETRY_H

