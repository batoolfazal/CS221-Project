// Mission orchestrator: DFS -> BFS -> A* with NFZ and hazards, telemetry, report
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include "CampusMap.h"
#include "NFZ.h"
#include "hashmap.h"
#include "telemetry.h"
#include "sort_utils.h"
#include "astar.h"
#include "HazardDetector.h"
#include <vector>
#include <string>
#include <algorithm>
#include <utility>
#include <limits>

// Global telemetry sink for A* tracePath (declared extern in astar.cpp)
TelemetryLog* gTelemetryPtr = 0;
HazardDetector* gHazardDetector = nullptr;

enum MissionStatus { INIT, VALIDATED, BFS_FAILED, PATH_PLANNED, RE_ROUTED, COMPLETED };

static bool explainCellStatus(CampusMap& map, int r, int c, std::string& reason) {
    if (!map.isInBounds(r, c)) { reason = "Out of bounds"; return false; }
    if (map.isObstacle(r, c)) { reason = "Blocked: obstacle/building"; return false; }
    if (map.isNoFlyZone(r, c)) { reason = "Blocked: campus no-fly zone"; return false; }
    if (map.isHazard(r, c)) { reason = "Blocked: static hazard"; return false; }
    if (isNFZ(r, c)) { reason = "Blocked: NFZ database"; return false; }
    if (gHazardDetector && gHazardDetector->isHazard(r, c)) { reason = "Blocked: dynamic hazard"; return false; }
    return true;
}

static std::pair<int,int> findNearestValidCell(int r, int c, CampusMap& map) {
    int rows = map.getRows(), cols = map.getCols();
    int maxDist = rows + cols;
    for (int dist = 0; dist <= maxDist; dist++) {
        for (int dr = -dist; dr <= dist; dr++) {
            int dcAbs = dist - std::abs(dr);
            int dcVals[2] = { dcAbs, -dcAbs };
            for (int k = 0; k < 2; k++) {
                int dc = dcVals[k];
                int nr = r + dr, nc = c + dc;
                std::string reason;
                if (explainCellStatus(map, nr, nc, reason)) {
                    return std::make_pair(nr, nc);
                }
                if (dcAbs == 0) break;
            }
        }
    }
    return std::make_pair(-1, -1);
}

static bool promptAndValidatePoint(const char* label, CampusMap& map, int& r, int& c, bool required) {
    while (true) {
        std::cout << "[" << label << " VALIDATION] Enter " << label << " row and col (0-" << (map.getRows() - 1)
                  << " for rows, 0-" << (map.getCols() - 1) << "; e.g., 10 12): ";
        std::cout.flush();
        if (!(std::cin >> r >> c)) {
            std::cin.clear();
            std::cin.ignore(1024, '\n');
            std::cout << "[" << label << " VALIDATION] Invalid input. Please enter two integers.\n";
            continue;
        }
        std::string reason;
        if (explainCellStatus(map, r, c, reason)) {
            std::cout << "[" << label << " VALIDATION] Accepted (" << r << "," << c << ")\n";
            return true;
        }
        std::cout << "[" << label << " VALIDATION] Invalid/blocked: " << reason << ".\n";
        auto cand = findNearestValidCell(r, c, map);
        if (cand.first == -1) {
            std::cout << "[" << label << " VALIDATION] No valid cell found nearby.\n";
            return !required;
        }
        std::cout << "[" << label << " VALIDATION] Nearest free cell: (" << cand.first << "," << cand.second << "). Use this? (y/n): ";
        char ans = 'n';
        std::cin >> ans;
        if (ans == 'y' || ans == 'Y') {
            r = cand.first;
            c = cand.second;
            std::cout << "[" << label << " VALIDATION] Using suggested cell.\n";
            return true;
        } else {
            std::cout << "[" << label << " VALIDATION] Re-enter coordinates.\n";
        }
    }
}

int main() {
    std::srand(static_cast<unsigned int>(std::time(0)));

    // 1. Load grid
    CampusMap campus(80, 80);
    if (!campus.loadFromFile("Mapping/campus_map.txt")) {
        campus.buildMap();
        campus.saveToFile("Mapping/campus_map.txt");
    }

    // 2. NFZ DB (sample entries)
    insertNFZ(10, 35);
    insertNFZ(29, 55);
    insertNFZ(40, 55);

    // 3. Dynamic hazards (hashmap) + HazardDetectorUpdated
    // (Use updated generator akin to GenerateHazards.py)
    HazardDetector detector(campus.getCols(), campus.getRows());
    gHazardDetector = &detector;
    hdGenerateRandomHazards(10);

    // Waypoints
    int startR = 74, startC = 40;
    int goalR = 15, goalC = 34;
    if (!promptAndValidatePoint("START", campus, startR, startC, true)) {
        std::cout << "Mission aborted: START invalid and no correction available.\n";
        return 0;
    }

    int waypointCount = 0;
    std::cout << "[WAYPOINT VALIDATION] Enter number of waypoints (0-3): ";
    std::cin >> waypointCount;
    if (std::cin.fail() || waypointCount < 0) { waypointCount = 0; std::cin.clear(); std::cin.ignore(1024, '\n'); }
    if (waypointCount > 3) waypointCount = 3;
    std::vector<std::pair<int,int> > waypoints;
    for (int i = 0; i < waypointCount; i++) {
        int wr = 0, wc = 0;
        if (!promptAndValidatePoint(("WAYPOINT " + std::to_string(i+1)).c_str(), campus, wr, wc, false)) {
            std::cout << "[WAYPOINT VALIDATION] Waypoint " << (i + 1) << " rejected. Aborting waypoint chain.\n";
            break;
        }
        waypoints.push_back(std::make_pair(wr, wc));
    }

    if (!promptAndValidatePoint("GOAL", campus, goalR, goalC, true)) {
        std::cout << "Mission aborted: GOAL invalid and no correction available.\n";
        return 0;
    }

    // Telemetry per leg
    struct LegData {
        std::string name;
        TelemetryLog log;
        std::vector<TelemetryEntry> path;
        std::vector<std::string> hazards;
    };
    std::vector<LegData> legs;
    MissionStatus status = VALIDATED;

    // 4. DFS pre-check
    std::cout << "[DFS] Running DFS pre-check from START to explore reachable free cells...\n";
    if (status == VALIDATED) runDfs(campus, startR, startC);
    std::cout << "[DFS] Traversal completed.\n";

    // 5. BFS pre-check
    std::cout << "[BFS] Running BFS connectivity check to first target...\n";
    int bfsTargetR = waypoints.empty() ? goalR : waypoints[0].first;
    int bfsTargetC = waypoints.empty() ? goalC : waypoints[0].second;
    bool bfsOk = bfsConnectivity(campus, startR, startC, bfsTargetR, bfsTargetC);
    std::cout << "[BFS] Path " << (bfsOk ? "exists" : "blocked") << " to first target.\n";
    if (!bfsOk) {
        status = BFS_FAILED;
        std::cout << "[STATUS] Mission aborted: BFS reported path blocked.\n";
    }

    // 6. A* pathfinding (per-leg telemetry)
    int curR = startR, curC = startC;
    std::vector<std::pair<int,int> > targets = waypoints;
    targets.push_back(std::make_pair(goalR, goalC));
    if (status != BFS_FAILED) {
        std::cout << "[A*] Running A* through waypoints...\n";
        bool allLegsOk = true;
        for (size_t idx = 0; idx < targets.size(); idx++) {
            int tr = targets[idx].first;
            int tc = targets[idx].second;
            std::string reason;
            if (!explainCellStatus(campus, tr, tc, reason)) {
                std::cout << "[A*] Target (" << tr << "," << tc << ") invalid: " << reason << ". Aborting mission.\n";
                allLegsOk = false;
                break;
            }
            bool bfsLeg = bfsConnectivity(campus, curR, curC, tr, tc);
            if (!bfsLeg) {
                std::cout << "[BFS] Gatekeeper: path blocked for segment " << (idx + 1) << ". Aborting mission.\n";
                allLegsOk = false;
                break;
            }
            std::cout << "[A*] Segment " << (idx + 1) << ": (" << curR << "," << curC << ") -> (" << tr << "," << tc << ")\n";
            legs.push_back({ "Leg " + std::to_string(idx + 1), TelemetryLog(), {}, {} });
            gTelemetryPtr = &legs.back().log;
            aStarSearch(campus, campus.getRows(), campus.getCols(), curR, curC, tr, tc);
            // Cache path and hazards
            const int MAX = 4096;
            TelemetryEntry buf[MAX];
            int n = legs.back().log.toArray(buf, MAX);
            legs.back().path.assign(buf, buf + n);
            legs.back().hazards.clear();
            for (int i = 0; i < n; i++) {
                int rr = buf[i].row, cc = buf[i].col;
                const char* desc = hazardDescription(rr, cc);
                if (desc && desc[0] != '\0') legs.back().hazards.push_back(std::string("(") + std::to_string(rr) + "," + std::to_string(cc) + ") HASH " + desc);
                if (gHazardDetector) {
                    Hazard* hz = gHazardDetector->getHazard(rr, cc);
                    if (hz && hz->isActive()) {
                        legs.back().hazards.push_back(std::string("(") + std::to_string(rr) + "," + std::to_string(cc) + ") "
                            + hz->type + " severity " + std::to_string(hz->severity) + "/5");
                    }
                }
            }
            curR = tr; curC = tc;
        }
        status = allLegsOk ? PATH_PLANNED : BFS_FAILED;
    }

    // 7. Re-route if new hazard appears
    if (status == PATH_PLANNED) {
        std::cout << "[HAZARD] Injecting new hazards and re-evaluating last leg...\n";
        if (!targets.empty()) {
            int tr = targets.back().first;
            int tc = targets.back().second;
            hdAddHazard((curR + tr) / 2, (curC + tc) / 2 + 1, "WEATHER", "Storm cell", 120, 4);
            bool bfsLeg = bfsConnectivity(campus, curR, curC, tr, tc);
            if (bfsLeg) {
                std::cout << "[A*] Re-routing with A* after hazard injection...\n";
                legs.push_back({ "Reroute", TelemetryLog(), {}, {} });
                gTelemetryPtr = &legs.back().log;
                aStarSearch(campus, campus.getRows(), campus.getCols(), curR, curC, tr, tc);
                const int MAX = 4096;
                TelemetryEntry buf[MAX];
                int n = legs.back().log.toArray(buf, MAX);
                legs.back().path.assign(buf, buf + n);
                legs.back().hazards.clear();
                for (int i = 0; i < n; i++) {
                    int rr = buf[i].row, cc = buf[i].col;
                    const char* desc = hazardDescription(rr, cc);
                    if (desc && desc[0] != '\0') legs.back().hazards.push_back(std::string("(") + std::to_string(rr) + "," + std::to_string(cc) + ") HASH " + desc);
                    if (gHazardDetector) {
                        Hazard* hz = gHazardDetector->getHazard(rr, cc);
                        if (hz && hz->isActive()) {
                            legs.back().hazards.push_back(std::string("(") + std::to_string(rr) + "," + std::to_string(cc) + ") "
                                + hz->type + " severity " + std::to_string(hz->severity) + "/5");
                        }
                    }
                }
                status = RE_ROUTED;
            } else {
                std::cout << "[BFS] Re-route blocked by BFS. Mission terminated.\n";
                status = BFS_FAILED;
            }
        }
    }

    // 9. Visualization of final accepted path only
    if (status == PATH_PLANNED || status == RE_ROUTED) {
        std::cout << "[VIS] Visualization (partial view around START, legend: S=start, G=goal, #=obstacle, N=NFZ, H=hazard, *=path, .=free)\n";
        std::vector<std::pair<int,int>> pathMarks;
        for (size_t i = 0; i < legs.size(); i++) {
            for (size_t k = 0; k < legs[i].path.size(); k++) {
                pathMarks.push_back(std::make_pair(legs[i].path[k].row, legs[i].path[k].col));
            }
        }
        int view = 10;
        int rs = std::max(0, startR - view);
        int re = std::min(campus.getRows() - 1, startR + view);
        int cs = std::max(0, startC - view);
        int ce = std::min(campus.getCols() - 1, startC + view);
        for (int r = rs; r <= re; r++) {
            for (int c = cs; c <= ce; c++) {
                char sym = '.';
                if (campus.isObstacle(r, c)) sym = '#';
                else if (campus.isNoFlyZone(r, c) || isNFZ(r, c)) sym = 'N';
                else if (campus.isHazard(r, c) || (gHazardDetector && gHazardDetector->isHazard(r, c))) sym = 'H';
                for (size_t m = 0; m < pathMarks.size(); m++) {
                    if (pathMarks[m].first == r && pathMarks[m].second == c) { sym = '*'; break; }
                }
                if (r == startR && c == startC) sym = 'S';
                if (r == goalR && c == goalC) sym = 'G';
                std::cout << sym;
            }
            std::cout << "\n";
        }
    } else {
        std::cout << "Mission failed before visualization; skipping grid view.\n";
    }

    // 10. Report generation with sorting per leg
    std::ofstream out("mission_summary.txt");
    if (out.is_open()) {
        out << "==============================\n";
        out << "MISSION SUMMARY\n";
        out << "==============================\n";
        out << "Map: Mapping/campus_map.txt (" << campus.getRows() << "x" << campus.getCols() << ")\n";
        out << "Start: (" << startR << "," << startC << ")\n";
        out << "Goal: (" << goalR << "," << goalC << ")\n";
        out << "Waypoints: [";
        for (size_t i = 0; i < waypoints.size(); i++) {
            out << "(" << waypoints[i].first << "," << waypoints[i].second << ")";
            if (i + 1 < waypoints.size()) out << ", ";
        }
        out << "]\n\n";
        for (size_t i = 0; i < legs.size(); i++) {
            out << legs[i].name << ":\n";
            if (!legs[i].hazards.empty()) {
                out << "Hazards encountered:\n";
                for (size_t h = 0; h < legs[i].hazards.size(); h++) {
                    out << "  " << legs[i].hazards[h] << "\n";
                }
            }
            out << "Path:\n";
            std::vector<TelemetryEntry> p = legs[i].path;
            if (!p.empty()) {
                if ((int)p.size() <= 16) insertionSort(p.data(), (int)p.size(), cmpByRowCol);
                else if ((int)p.size() <= 256) quickSort(p.data(), (int)p.size(), cmpByRowCol);
                else mergeSort(p.data(), (int)p.size(), cmpByRowCol);
                TelemetryEntry last = p[0];
                out << "(" << last.row << "," << last.col << ")\n";
                for (size_t k = 1; k < p.size(); k++) {
                    if (p[k].row == last.row && p[k].col == last.col) continue;
                    out << "(" << p[k].row << "," << p[k].col << ")\n";
                    last = p[k];
                }
            }
            out << "\n";
        }
        out.close();
        std::cout << "mission_summary.txt written.\n";
    } else {
        std::cout << "Failed to write mission_summary.txt\n";
    }

    return 0;
}
