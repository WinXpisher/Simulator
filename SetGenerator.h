#pragma once

#include <cstdlib>
#include <vector>
#include <string>
#include <ctime>
#include "SimulatorPrimitives.h"

// генератор вх≥дних набор≥в (завдань ≥ ресурс≥в)
class SetGenerator
{
private:
    std::vector<std::string> procArchs;
    std::vector<std::string> osTypes;

    int getRandomNumber(int min, int max) {
        return min + rand() % (max - min + 1);
    }

    double roundToDigits(double x, int digits)
    {
        double factor = std::pow(10.0, digits);
        return std::round(x * factor) / factor;
    }

    void fillTask(Task& task)
    {
        std::string arch = procArchs[getRandomNumber(0, procArchs.size() - 1)];
        std::string os = osTypes[getRandomNumber(0, osTypes.size() - 1)];

        task = Task{
            Task::generateId(),
            Task::WAITING,
            getRandomNumber(1, 15), // к≥льк≥сть задач (в д≥апазон≥ в≥д 1 до 15)
            {
                arch,      // арх≥тектура процесора
                os,        // операц≥йна система
                getRandomNumber(1, 10), // к≥льк≥сть процесор≥в (в≥д 1 до 10)
                getRandomNumber(1, 20), // швидкод≥€ процесор≥в (в≥д 1 до 20)
                getRandomNumber(1, 64), // об'Їм оперативноњ пам'€т≥ (в≥д 1 до 64 √б)
                getRandomNumber(10, 512) // доступний обс€г в≥нчестера (в≥д 10 до 512 √б)
            },
            // коеф≥ц≥Їнт зв'€зност≥ задач у завданн≥ (в≥д 0.0 до 1.0)
            roundToDigits(static_cast<double>(getRandomNumber(0, 10)) / 10, 1),
            getRandomNumber(1, 10), // пр≥оритет завданн€ (в≥д 1 до 10)
            static_cast<double>(getRandomNumber(1, 60)), // час виконанн€ одн≥Їњ задач≥ (в≥д 1 до 60 хв)
            static_cast<double>(getRandomNumber(5, 100))  // розм≥р задач≥ (в≥д 5 до 100 ћб)
        };
    }

    void fillResource(Resource& res)
    {
        std::string arch = procArchs[getRandomNumber(0, procArchs.size() - 1)];
        std::string os = osTypes[getRandomNumber(0, osTypes.size() - 1)];

        res = Resource{
            Resource::generateId(),
            {
                arch,      // арх≥тектура процесора
                os,        // операц≥йна система
                getRandomNumber(1, 50), // к≥льк≥сть процесор≥в (в≥д 1 до 50)
                getRandomNumber(5, 40), // швидкод≥€ процесор≥в (в≥д 5 до 40)
                getRandomNumber(8, 1024), // об'Їм оперативноњ пам'€т≥ (в≥д 8 до 1024 √б)
                getRandomNumber(256, 3000) // доступний обс€г в≥нчестера (в≥д 256 до 3000 √б)
            },
            static_cast<double>(getRandomNumber(5, 60)) // пропускна здатн≥сть каналу (в≥д 5 до 60 ћб/хв)
        };
    }
public:
    SetGenerator(
        const std::vector<std::string>& procArchs,
        const std::vector<std::string>& osTypes
    ) : procArchs(procArchs), osTypes(osTypes)
    {
        // ≥н≥ц≥ал≥зац≥€ генератора випадкових чисел
        std::srand(std::time(0));
    }

    void fillRandomTasks(vector<Task>& tasks, int count)
    {
        for (int i = 0; i < count; ++i)
        {
            Task task;
            fillTask(task);
            tasks.push_back(task);
        }
    }

    void fillRandomResources(vector<Resource>& resources, int count)
    {
        for (int i = 0; i < count; ++i)
        {
            Resource res;
            fillResource(res);
            resources.push_back(res);
        }
    }
};