#include "SimulatorPrimitives.h"

int Task::globalId = 1;
std::string Resource::globalId = "";

std::string Resource::generateId()
{
    if (globalId.empty()) 
    {
        globalId = "A";
        return globalId;
    }

    int i = globalId.size() - 1;
    // доки літера є останньою в алфавіті - Z,
    // замінюємо її на першу - A
    while (i >= 0 && globalId[i] == 'Z') 
    {
        globalId[i] = 'A';
        --i;
    }
    // якщо всі літери були Z, додаємо новий разряд
    if (i < 0)
        globalId.insert(globalId.begin(), 'A');
    // інакше, просто замінюємо останню (з кінця) літеру,
    // яка не була Z на наступну по алфавіту
    else
        globalId[i]++;

    return globalId;
}

std::string Task::generateId()
{
    return std::to_string(globalId++); 
}

int Task::getRemainingSubTasksCount(const Task& parentTask)
{
    int countToSubstract = 0;
    // підраховуємо суму задач всіх дочірніх завдань поточного завдання
    for (const Task& child : parentTask.simulationInfo.childTasks)
    {
        countToSubstract += child.count;
    }
    // повертаємо різницю задач батьківського завдання і підрахованої суми
    return parentTask.count - countToSubstract;
}

std::istream& operator>>(
    std::istream& is,
    ResourceDescriptor& resDesc
)
{
    std::string line;
    std::getline(is, line); resDesc.procArch = line.substr(10);
    std::getline(is, line); resDesc.os = line.substr(4);
    std::getline(is, line); resDesc.procCount = std::stoi(line.substr(11));
    std::getline(is, line); resDesc.procSpeed = std::stoi(line.substr(11));
    std::getline(is, line); resDesc.memSize = std::stoi(line.substr(9));
    std::getline(is, line); resDesc.discSize = std::stoi(line.substr(10));

    return is;
}

std::ostream& operator<<(
    std::ostream& os,
    const ResourceDescriptor& resDesc
)
{
    os << "procArch: " << resDesc.procArch << "\n"
        << "os: " << resDesc.os << "\n"
        << "procCount: " << resDesc.procCount << "\n"
        << "procSpeed: " << resDesc.procSpeed << "\n"
        << "memSize: " << resDesc.memSize << "\n"
        << "discSize: " << resDesc.discSize << "\n";
    return os;
}


std::istream& operator>>(
    std::istream& is,
    Resource& res
)
{
    std::string line;
    std::getline(is, line); res.id = line.substr(4);
    is >> res.resDesc;
    std::getline(is, line); res.bandwidth = std::stod(line.substr(11));

    return is;
}

std::ostream& operator<<(
    std::ostream& os,
    const Resource& res
)
{
    os << "id: " << res.id << "\n"
        << res.resDesc
        << "bandwidth: " << res.bandwidth << "\n";
    return os;
}

std::istream& operator>>(
    std::istream& is,
    Task& task
    )
{
    std::string line;
    std::getline(is, line); task.id = line.substr(4);
    std::getline(is, line); task.count = std::stoi(line.substr(7));
    is >> task.resDesc;
    std::getline(is, line); task.connectivity = std::stod(line.substr(14));
    std::getline(is, line); task.priority = std::stoi(line.substr(10));
    std::getline(is, line); task.performTime = std::stod(line.substr(13));
    std::getline(is, line); task.subTaskSize = std::stod(line.substr(13));

    return is;
}

std::ostream& operator<<(
    std::ostream& os,
    const Task& task
    )
{
    os << "id: " << task.id << "\n"
        << "count: " << task.count << "\n"
        << task.resDesc
        << "connectivity: " << task.connectivity << "\n"
        << "priority: " << task.priority << "\n"
        << "performTime: " << task.performTime << "\n"
        << "subTaskSize: " << task.subTaskSize << "\n";
    return os;
}