#include "hashmap.h"
#include <cstdlib>
#include <ctime>

// Simple singleton for convenience wrappers
static HazardMap* gHazardMap = 0;

// ===== HazardMap implementation =====
HazardMap::HazardMap(int bucketsCount) : bucketCount(bucketsCount) {
    if (bucketCount < 16) bucketCount = 16;
    buckets = new HazardEntry*[bucketCount];
    for (int i = 0; i < bucketCount; i++) buckets[i] = 0;
}

HazardMap::~HazardMap() {
    clear();
    delete[] buckets;
    buckets = 0;
}

int HazardMap::hash(int row, int col) const {
    long long key = (static_cast<long long>(row) * 73856093LL) ^ (static_cast<long long>(col) * 19349663LL);
    if (key < 0) key = -key;
    return static_cast<int>(key % bucketCount);
}

HazardEntry* HazardMap::findEntry(int row, int col) const {
    int idx = hash(row, col);
    HazardEntry* cur = buckets[idx];
    while (cur != 0) {
        if (cur->row == row && cur->col == col) return cur;
        cur = cur->next;
    }
    return 0;
}

void HazardMap::clear() {
    for (int i = 0; i < bucketCount; i++) {
        HazardEntry* cur = buckets[i];
        while (cur != 0) {
            HazardEntry* nxt = cur->next;
            delete cur;
            cur = nxt;
        }
        buckets[i] = 0;
    }
}

void HazardMap::addHazard(int row, int col, const char* description) {
    int idx = hash(row, col);
    HazardEntry* cur = buckets[idx];
    while (cur != 0) {
        if (cur->row == row && cur->col == col) {
            // Update existing description
            std::strncpy(cur->desc, description, sizeof(cur->desc) - 1);
            cur->desc[sizeof(cur->desc) - 1] = '\0';
            return;
        }
        cur = cur->next;
    }
    HazardEntry* node = new HazardEntry();
    node->row = row;
    node->col = col;
    std::strncpy(node->desc, description, sizeof(node->desc) - 1);
    node->desc[sizeof(node->desc) - 1] = '\0';
    node->next = buckets[idx];
    buckets[idx] = node;
}

bool HazardMap::removeHazard(int row, int col) {
    int idx = hash(row, col);
    HazardEntry* cur = buckets[idx];
    HazardEntry* prev = 0;
    while (cur != 0) {
        if (cur->row == row && cur->col == col) {
            if (prev != 0) prev->next = cur->next;
            else buckets[idx] = cur->next;
            delete cur;
            return true;
        }
        prev = cur;
        cur = cur->next;
    }
    return false;
}

bool HazardMap::isHazard(int row, int col) const {
    return findEntry(row, col) != 0;
}

const char* HazardMap::getHazardDescription(int row, int col) const {
    HazardEntry* e = findEntry(row, col);
    if (e != 0) return e->desc;
    return "";
}

// ===== Global helpers =====
static void ensureMap() {
    if (gHazardMap == 0) {
        gHazardMap = new HazardMap();
    }
}

void addHazard(int row, int col, const char* description) {
    ensureMap();
    gHazardMap->addHazard(row, col, description);
}

bool removeHazard(int row, int col) {
    if (gHazardMap == 0) return false;
    return gHazardMap->removeHazard(row, col);
}

bool isHazard(int row, int col) {
    if (gHazardMap == 0) return false;
    return gHazardMap->isHazard(row, col);
}

const char* hazardDescription(int row, int col) {
    if (gHazardMap == 0) return "";
    return gHazardMap->getHazardDescription(row, col);
}

void clearHazards() {
    if (gHazardMap != 0) gHazardMap->clear();
}

void generateRandomHazards(int count, int rows, int cols, const char* descriptionPrefix) {
    ensureMap();
    if (rows <= 0 || cols <= 0 || count <= 0) return;
    std::srand(static_cast<unsigned int>(std::time(0)));
    char descBuf[64];
    for (int i = 0; i < count; i++) {
        int r = std::rand() % rows;
        int c = std::rand() % cols;
        // create description "prefix_idx"
        int idx = i % 100000;
        int len = 0;
        for (const char* p = descriptionPrefix; *p && len < 60; ++p) {
            descBuf[len++] = *p;
        }
        if (len < 63) descBuf[len++] = '_';
        // append idx digits
        char numbuf[12];
        int n = idx;
        int numlen = 0;
        if (n == 0) {
            numbuf[numlen++] = '0';
        } else {
            while (n > 0 && numlen < 10) {
                numbuf[numlen++] = char('0' + (n % 10));
                n /= 10;
            }
        }
        for (int k = numlen - 1; k >= 0 && len < 63; k--) {
            descBuf[len++] = numbuf[k];
        }
        descBuf[len] = '\0';
        gHazardMap->addHazard(r, c, descBuf);
    }
}
