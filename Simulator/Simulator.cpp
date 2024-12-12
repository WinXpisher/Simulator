#include <iostream>
#include "SimulatorPrimitives.h"
#include "SimulationEnvironment.h"

using namespace std;

int main() {
    DataBase db;
    // ресурси
    db.availableResources.push_back({
        {"x86", "Linux", 4, 3200, 16, 500},
        100,
        10,
        0
        });
    db.availableResources.push_back({
        {"ARM", "Windows", 2, 2000, 8, 256},
        50.0,
        20.0,
        0
        });

    // задачі
    db.tasks.push_back({
        Task::WAITING,
        1,
        {"x86", "Linux", 4, 3000, 8, 100},
        0.4, 
        1,        
        5.0
        });
    db.tasks.push_back({
        Task::WAITING,
        1,
        {"ARM", "Windows", 1, 1500, 4, 100},
        0.2,
        2,
        10.0
        });
    db.tasks.push_back({
        Task::WAITING,
        1,
        {"x86", "Arch Linux", 2, 3200, 16, 500},
        0.5,
        3,
        15.0
        });

    TaskAnalizer taskAnalizer(&db);
    taskAnalizer.analizeAllTasks();
 
    auto& results = taskAnalizer.getAnalizeResult();

    for (auto& result : results) {
        cout << "Resources Available: " << result.resources.size() << endl;
        for (auto& res : result.resources) {
            cout << "Resource: " << res->resDesc.procArch << ", " << res->resDesc.os << endl;
        }
        cout << "Status: " << (result.taskPtr->status == Task::WAITING ? "WAITING" : "CANCELLED") << endl;
        cout << "-----------------------\n";
    }

    return 0;
}