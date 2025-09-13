#include "GetTaskStatusStr.h"

QString getTaskStatusStr(const Task& task)
{
    switch (task.status)
    {
    case Task::WAITING:
        return "WAITING";
    case Task::CANCELLED:
        return "CANCELLED";
    case Task::SENDING:
        return "SENDING";
    case Task::RUNNING:
        return "RUNNING";
    case Task::PERFORMED:
        return "PERFORMED";
    case Task::DIVIDED:
        return "DIVIDED";
    case Task::DIVIDED_RUNNING:
        return "DIVIDED_RUNNING";
    default:
        return "UNKNOWN";
    }
}
