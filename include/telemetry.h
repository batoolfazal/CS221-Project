#ifndef TELEMETRY_H
#define TELEMETRY_H

#include <vector>

// Doubly linked list node for telemetry
struct TelemetryNode {
    int row;
    int col;
    TelemetryNode* prev;
    TelemetryNode* next;
};

// POD entry used for sorting/summary
struct TelemetryEntry {
    int row;
    int col;
    int metric; // optional (e.g., timestamp or cost); default 0
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

    // Copy up to maxCount entries into out array (start-to-end order).
    // Returns number of entries copied.
    int toArray(TelemetryEntry* out, int maxCount) const;

    // Export to std::vector for downstream consumers (e.g., GUI/report)
    std::vector<TelemetryEntry> toVector() const;
};

#endif // TELEMETRY_H

