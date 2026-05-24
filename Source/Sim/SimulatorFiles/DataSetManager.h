#pragma once

#include <filesystem>
#include <vector>
#include "SimulatorPrimitives.h"
#include "FileManager.h"
#include <algorithm>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonParseError>
#include <QJsonDocument>
#include <QMap>
#include <QFile>

class DataSetManager
{
    /*
     * Class responding for recording sets of data (tasks and resources)
     * to files and reading that data from files
    */
private:
    FileManager fm;

    enum class TaskFields
    {
        Id,
        Count,
        ResDesc,
        Connectivity,
        Priority,
        PerformTime,
        SubTaskSize,
        Unknown
    };

    enum class ResFields
    {
        Id,
        ResDesc,
        Bandwidth,
        Unknown
    };

    enum class ResDescFields
    {
        ProcArch,
        Os,
        ProcCount,
        ProcSpeed,
        MemSize,
        DiscSize,
        Unknown
    };

    QMap<TaskFields, std::string> taskFieldsMap;
    QMap<ResFields, std::string> resFieldsMap;
    QMap<ResDescFields, std::string> resDescFieldsMap;

    bool convertTaskToJson(const Task& task, QJsonObject& j)
    {
        j[QString::fromStdString(taskFieldsMap[TaskFields::Id])]           = QString::fromStdString(task.id);
        j[QString::fromStdString(taskFieldsMap[TaskFields::Count])]        = task.count;
        j[QString::fromStdString(taskFieldsMap[TaskFields::Connectivity])] = task.connectivity;
        j[QString::fromStdString(taskFieldsMap[TaskFields::Priority])]     = task.priority;
        j[QString::fromStdString(taskFieldsMap[TaskFields::PerformTime])]  = task.performTime;
        j[QString::fromStdString(taskFieldsMap[TaskFields::SubTaskSize])]  = task.subTaskSize;

        QJsonObject resDescObj;
        if (convertResDescToJson(task.resDesc, resDescObj))
        {
            j[QString::fromStdString(taskFieldsMap[TaskFields::ResDesc])] = resDescObj;
        }
        else
        {
            return false;
        }

        return true;
    }

    bool convertResToJson(const Resource& res, QJsonObject& j)
    {
        j[QString::fromStdString(resFieldsMap[ResFields::Id])]        = QString::fromStdString(res.id);
        j[QString::fromStdString(resFieldsMap[ResFields::Bandwidth])] = res.bandwidth;

        QJsonObject resDescObj;
        if (convertResDescToJson(res.resDesc, resDescObj))
        {
            j[QString::fromStdString(resFieldsMap[ResFields::ResDesc])] = resDescObj;
        }
        else
        {
            return false;
        }

        return true;
    }

    bool convertResDescToJson(const ResourceDescriptor& resDesc, QJsonObject& j)
    {
        j[QString::fromStdString(resDescFieldsMap[ResDescFields::ProcArch])]  = QString::fromStdString(resDesc.procArch);
        j[QString::fromStdString(resDescFieldsMap[ResDescFields::Os])]        = QString::fromStdString(resDesc.os);
        j[QString::fromStdString(resDescFieldsMap[ResDescFields::ProcCount])] = resDesc.procCount;
        j[QString::fromStdString(resDescFieldsMap[ResDescFields::ProcSpeed])] = resDesc.procSpeed;
        j[QString::fromStdString(resDescFieldsMap[ResDescFields::MemSize])]   = resDesc.memSize;
        j[QString::fromStdString(resDescFieldsMap[ResDescFields::DiscSize])]  = resDesc.discSize;

        return true;
    }

    bool convertJsonToTask(const QJsonObject& j, Task& task)
    {
        for (auto it = j.begin(); it != j.end(); ++it)
        {
            QString key = it.key();
            QJsonValue val = it.value();

            TaskFields field = TaskFields::Unknown;

            for (auto mapIt = taskFieldsMap.begin(); mapIt != taskFieldsMap.end(); ++mapIt)
            {
                if (QString::fromStdString(mapIt.value()) == key)
                {
                    field = mapIt.key();
                    break;
                }
            }

            if (field == TaskFields::Unknown)
            {
                qWarning() << "Field" << key << "is not in Task.";
                return false;
            }

            switch (field)
            {
            case TaskFields::Id:
                if (!val.isString()) return false;
                task.id = val.toString().toStdString();
                break;
            case TaskFields::Count:
                if (!val.isDouble()) return false;
                task.count = val.toInt();
                break;
            case TaskFields::Connectivity:
                if (!val.isDouble()) return false;
                task.connectivity = val.toDouble();
                break;
            case TaskFields::Priority:
                if (!val.isDouble()) return false;
                task.priority = val.toInt();
                break;
            case TaskFields::PerformTime:
                if (!val.isDouble()) return false;
                task.performTime = val.toDouble();
                break;
            case TaskFields::SubTaskSize:
                if (!val.isDouble()) return false;
                task.subTaskSize = val.toDouble();
                break;
            case TaskFields::ResDesc:
                if (!val.isObject()) return false;
                if (!convertJsonToResDesc(val.toObject(), task.resDesc))
                {
                    return false;
                }
                break;
            default:
                return false;
            }
        }
        return true;
    }

    bool convertJsonToRes(const QJsonObject& j, Resource& res)
    {
        for (auto it = j.begin(); it != j.end(); ++it)
        {
            QString key = it.key();
            QJsonValue val = it.value();

            ResFields field = ResFields::Unknown;
            for (auto mapIt = resFieldsMap.begin(); mapIt != resFieldsMap.end(); ++mapIt)
            {
                if (QString::fromStdString(mapIt.value()) == key)
                {
                    field = mapIt.key();
                    break;
                }
            }

            if (field == ResFields::Unknown)
            {
                qWarning() << "Field" << key << "is not in Resource.";
                return false;
            }

            switch (field)
            {
            case ResFields::Id:
                if (!val.isString()) return false;
                res.id = val.toString().toStdString();
                break;
            case ResFields::Bandwidth:
                if (!val.isDouble()) return false;
                res.bandwidth = val.toDouble();
                break;
            case ResFields::ResDesc:
                if (!val.isObject()) return false;
                if (!convertJsonToResDesc(val.toObject(), res.resDesc))
                {
                    return false;
                }
                break;
            default:
                return false;
            }
        }
        return true;
    }

    bool convertJsonToResDesc(const QJsonObject& j, ResourceDescriptor& resDesc)
    {
        for (auto it = j.begin(); it != j.end(); ++it)
        {
            QString key = it.key();
            QJsonValue val = it.value();

            ResDescFields field = ResDescFields::Unknown;
            for (auto mapIt = resDescFieldsMap.begin(); mapIt != resDescFieldsMap.end(); ++mapIt)
            {
                if (QString::fromStdString(mapIt.value()) == key)
                {
                    field = mapIt.key();
                    break;
                }
            }

            if (field == ResDescFields::Unknown)
            {
                qWarning() << "Field" << key << "is not in ResourceDescriptor.";
                return false;
            }

            switch (field)
            {
            case ResDescFields::ProcArch:
                if (!val.isString()) return false;
                resDesc.procArch = val.toString().toStdString();
                break;
            case ResDescFields::Os:
                if (!val.isString()) return false;
                resDesc.os = val.toString().toStdString();
                break;
            case ResDescFields::ProcCount:
                if (!val.isDouble()) return false;
                resDesc.procCount = val.toInt();
                break;
            case ResDescFields::ProcSpeed:
                if (!val.isDouble()) return false;
                resDesc.procSpeed = val.toInt();
                break;
            case ResDescFields::MemSize:
                if (!val.isDouble()) return false;
                resDesc.memSize = val.toInt();
                break;
            case ResDescFields::DiscSize:
                if (!val.isDouble()) return false;
                resDesc.discSize = val.toInt();
                break;
            default:
                return false;
            }
        }
        return true;
    }


public:
    DataSetManager()
    {
        //Task
        taskFieldsMap[TaskFields::Id]           = "id";
        taskFieldsMap[TaskFields::Count]        = "count";
        taskFieldsMap[TaskFields::ResDesc]      = "resDesc";
        taskFieldsMap[TaskFields::Connectivity] = "connectivity";
        taskFieldsMap[TaskFields::Priority]     = "priority";
        taskFieldsMap[TaskFields::PerformTime]  = "performTime";
        taskFieldsMap[TaskFields::SubTaskSize]  = "subTaskSize";
        //Res
        resFieldsMap[ResFields::Id]        = "id";
        resFieldsMap[ResFields::ResDesc]   = "resDesc";
        resFieldsMap[ResFields::Bandwidth] = "bandwidth";
        //ResDesc
        resDescFieldsMap[ResDescFields::ProcArch]  = "procArch";
        resDescFieldsMap[ResDescFields::Os]        = "os";
        resDescFieldsMap[ResDescFields::ProcCount] = "procCount";
        resDescFieldsMap[ResDescFields::MemSize]   = "memSize";
        resDescFieldsMap[ResDescFields::DiscSize]  = "discSize";
        resDescFieldsMap[ResDescFields::ProcSpeed] = "procSpeed";
    }

    // --- TASKS ---
    int writeTasks(const std::vector<Task>& tasks)
    {
        fm.selectDir("");
        fm.selectDir("Simulation data");
        fm.selectDir("Task set");

        const std::string& prefix = "tasks_set";
        const std::string& extension = ".json";
        
        int taskSetNumber = fm.getNextFileNumber(prefix, extension);
        const std::string fileName = prefix +
            std::to_string(taskSetNumber) +
            extension;

        QJsonArray tasksJson;
        QJsonDocument doc;

        for (const Task& task : tasks)
        {
            QJsonObject j;
            convertTaskToJson(task, j);
            tasksJson.append(j);
        }
        doc.setArray(tasksJson);
        fm.writeString(fileName, doc.toJson().toStdString());
        
        return taskSetNumber;
    }

    void loadTasks(std::vector<Task>& tasks, int taskSetNum)
    {
        qDebug() << "Enter loadTasks";
        fm.selectDir("");
        fm.selectDir("Simulation data");
        fm.selectDir("Task set");

        const std::string prefix = "tasks_set";
        const std::string extension = ".json";
        const std::string fileName = prefix + std::to_string(taskSetNum) + extension;

        std::filesystem::path p = fm.getPath();
        p.append(fileName);
        QFile file(p.string().c_str());
        if (!file.open(QIODevice::ReadOnly))
        {
            qDebug() << "Not open";
            return;
        }
        QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
        file.close();

        if (!doc.isArray())
        {
            qDebug() << "Not array";
            return;
        }
        for (auto taskJson : doc.array())
        {
            Task t;
            if (!taskJson.isObject())
            {
                continue;
            }
            if (convertJsonToTask(taskJson.toObject(), t))
            {
                tasks.push_back(t);
                qDebug() << "Task pushed: " << tasks.size();
            }
        }
    }

    // --- RESOURCES ---
    int writeResources(const std::vector<Resource>& resources)
    {
        fm.selectDir("");
        fm.selectDir("Simulation data");
        fm.selectDir("Resource set");

        const std::string prefix = "resources_set";
        const std::string extension = ".json";

        int resSetNumber = fm.getNextFileNumber(prefix, extension);
        const std::string fileName = prefix +
                                     std::to_string(resSetNumber) +
                                     extension;

        QJsonArray resourcesJson;
        QJsonDocument doc;

        for (const Resource& res : resources)
        {
            QJsonObject j;
            convertResToJson(res, j);
            resourcesJson.append(j);
        }

        doc.setArray(resourcesJson);
        fm.writeString(fileName, doc.toJson().toStdString());

        return resSetNumber;
    }

    void loadResources(std::vector<Resource>& resources, int resourceSetNum)
    {
        fm.selectDir("");
        fm.selectDir("Simulation data");
        fm.selectDir("Resource set");

        const std::string prefix = "resources_set";
        const std::string extension = ".json";
        const std::string fileName = prefix + std::to_string(resourceSetNum) + extension;

        std::filesystem::path p = fm.getPath();
        p.append(fileName);
        QFile file(p.string().c_str());
        if (!file.open(QIODevice::ReadOnly))
        {
            return;
        }
        QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
        file.close();

        if (!doc.isArray())
        {
            return;
        }

        for (auto resJson : doc.array())
        {
            if (!resJson.isObject())
            {
                continue;
            }

            Resource res;
            if (convertJsonToRes(resJson.toObject(), res))
            {
                resources.push_back(res);
            }
        }
    }
};
