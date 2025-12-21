#ifndef SORT_UTILS_H
#define SORT_UTILS_H

#include "telemetry.h"

// Comparator signature: return true if a should come before b.
typedef bool (*TelemetryCmp)(const TelemetryEntry& a, const TelemetryEntry& b);

void insertionSort(TelemetryEntry* arr, int n, TelemetryCmp cmp);
void mergeSort(TelemetryEntry* arr, int n, TelemetryCmp cmp);
void quickSort(TelemetryEntry* arr, int n, TelemetryCmp cmp);

// Common comparators
bool cmpByMetricAsc(const TelemetryEntry& a, const TelemetryEntry& b);
bool cmpByRowCol(const TelemetryEntry& a, const TelemetryEntry& b);

#endif // SORT_UTILS_H

