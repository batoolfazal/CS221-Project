#include "sort_utils.h"

// ===== Insertion Sort (good for small n) =====
void insertionSort(TelemetryEntry* arr, int n, TelemetryCmp cmp) {
    for (int i = 1; i < n; i++) {
        TelemetryEntry key = arr[i];
        int j = i - 1;
        while (j >= 0 && cmp(key, arr[j])) {
            arr[j + 1] = arr[j];
            j--;
        }
        arr[j + 1] = key;
    }
}

// ===== Merge Sort (stable) =====
static void merge(TelemetryEntry* arr, TelemetryEntry* tmp, int l, int m, int r, TelemetryCmp cmp) {
    int i = l, j = m + 1, k = l;
    while (i <= m && j <= r) {
        if (!cmp(arr[j], arr[i])) {
            tmp[k++] = arr[i++];
        } else {
            tmp[k++] = arr[j++];
        }
    }
    while (i <= m) tmp[k++] = arr[i++];
    while (j <= r) tmp[k++] = arr[j++];
    for (int t = l; t <= r; t++) arr[t] = tmp[t];
}

static void mergeSortRec(TelemetryEntry* arr, TelemetryEntry* tmp, int l, int r, TelemetryCmp cmp) {
    if (l >= r) return;
    int m = l + (r - l) / 2;
    mergeSortRec(arr, tmp, l, m, cmp);
    mergeSortRec(arr, tmp, m + 1, r, cmp);
    merge(arr, tmp, l, m, r, cmp);
}

void mergeSort(TelemetryEntry* arr, int n, TelemetryCmp cmp) {
    if (n <= 1) return;
    TelemetryEntry* tmp = new TelemetryEntry[n];
    mergeSortRec(arr, tmp, 0, n - 1, cmp);
    delete[] tmp;
}

// ===== Quick Sort (in-place) =====
static int partition(TelemetryEntry* arr, int low, int high, TelemetryCmp cmp) {
    TelemetryEntry pivot = arr[high];
    int i = low - 1;
    for (int j = low; j <= high - 1; j++) {
        if (cmp(arr[j], pivot)) {
            i++;
            TelemetryEntry tmp = arr[i];
            arr[i] = arr[j];
            arr[j] = tmp;
        }
    }
    TelemetryEntry tmp = arr[i + 1];
    arr[i + 1] = arr[high];
    arr[high] = tmp;
    return i + 1;
}

static void quickSortRec(TelemetryEntry* arr, int low, int high, TelemetryCmp cmp) {
    if (low < high) {
        int pi = partition(arr, low, high, cmp);
        quickSortRec(arr, low, pi - 1, cmp);
        quickSortRec(arr, pi + 1, high, cmp);
    }
}

void quickSort(TelemetryEntry* arr, int n, TelemetryCmp cmp) {
    if (n <= 1) return;
    quickSortRec(arr, 0, n - 1, cmp);
}

// ===== Comparators =====
bool cmpByMetricAsc(const TelemetryEntry& a, const TelemetryEntry& b) {
    return a.metric < b.metric;
}

bool cmpByRowCol(const TelemetryEntry& a, const TelemetryEntry& b) {
    if (a.row != b.row) return a.row < b.row;
    return a.col < b.col;
}

