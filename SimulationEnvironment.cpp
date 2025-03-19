#include <chrono>
#include <thread>
#include "SimulationEnvironment.h"
#include "ResourceManager.h"

using DM = DistributionMethod;
SimulationEnvironment::SimulationEnvironment(DataBase* dataBase, Logger* logger):
    logger(logger), dataBase(dataBase), taskAnalizer(dataBase)
{
    
}

SimulationEnvironment::~SimulationEnvironment()
{
    for (DM* dmPtr : distributionMethods)
    {
        delete dmPtr;
    }
}

void SimulationEnvironment::prepareForSimulation(
    int channelCount,
    int timeUnitToSkip,
    int waitMilliSec
)
{
    this->channelCount = channelCount;
    simContext.timeUnitToSkip = timeUnitToSkip;
    simContext.waitMilliSec = waitMilliSec;

    initTasksSimulationInfo();
    initSimContext();
}

void SimulationEnvironment::initSimContext()
{
    subTaskWaitingTimeSum = 0;
    resourceStagnationSum = 0;
    simulationClock = 0;

    taskAnalizer.analizeAllTasks();

    // отримуємо результати аналізу задач, які не мають статусу CANCELLED
    simContext.anResults =
        taskAnalizer.getAnalizeResultClear();

    simContext.assignedResource = nullptr;     
    simContext.hasTask = true;                 
    simContext.subTasksRemain = 0;
    simContext.areSubTasksConnected = false;
}

void SimulationEnvironment::initTasksSimulationInfo()
{
    for (Task& task : dataBase->tasks)
    {
        task.simulationInfo.parentTaskPtr = nullptr;
        task.simulationInfo.timePerformed = 0;
        task.simulationInfo.waitingTime = 0;
    }
}

void SimulationEnvironment::runSimulation(const DM* dm)
{
    bool isItFirstIteration = true;
    while (true)
    {
        {
            std::lock_guard<std::mutex> lock(dataBaseMutex);
            
            // якщо це перша ітерація
            if (isItFirstIteration)
            {
                isItFirstIteration = false;
                logger->logSimulationData(
                    SimulationData
                    {
                        simulationClock,
                        getSubTaskWaitingTimeAv(),
                        getResourceStagnationAv()
                    }
                );
                // виводимо завдання, які було скасовано
                for (const Task& task : dataBase->tasks)
                {
                    if (task.status == Task::TaskStatus::CANCELLED)
                        logger->logTaskCancelled(task);
                }
            }

            simContext.actionTaken = false;
            // якщо ще є задачі для відправки
            if (simContext.subTasksRemain > 0)
            {
                // використовуємо призначений ресурс, якщо він є, інакше шукаємо вільний
                Resource* targetResource = simContext.assignedResource;
                if (!targetResource)
                {
                    targetResource = ResourceManager::findAnyFreeResource(
                        *simContext.anResult.task,
                        simContext.anResult.resources,
                        simContext.areSubTasksConnected
                    );
                }

                // якщо ресурс знайдено, відправляємо завдання (або його частину)
                if (targetResource)
                {
                    int sent = trySendTaskToResource(
                        *simContext.anResult.task,
                        *targetResource,
                        simContext.areSubTasksConnected
                    );
                    if (sent > 0)
                    {
                        simContext.subTasksRemain -= sent;
                        simContext.actionTaken = true;
                    }
                    // якщо це не призначений ресурс, скидаємо його для 
                    // наступного пошуку
                    if (targetResource != simContext.assignedResource)
                    {
                        targetResource = nullptr;
                    }
                }
            }
            // якщо ж задач для відправки немає, завантажуємо нове завдання
            else if (simContext.hasTask)
            {
                Resource* resource = nullptr;
                simContext.hasTask = dm->nextTask(
                    simContext.anResults,
                    simContext.anResult,
                    resource
                );
                // якщо завдання було отримано
                if (simContext.hasTask)
                {
                    simContext.subTasksRemain = simContext.anResult.task->count;
                    simContext.areSubTasksConnected = 
                        taskAnalizer.areSubTasksConnected(*simContext.anResult.task);
                    // зберігаємо ресурс, якщо він наданий
                    simContext.assignedResource = resource;
                    simContext.actionTaken = true;
                }
            }
            if (haveAllTasksPerformed())
            {
                break;
            }
            // перевіряємо, чи всі завдання виконано
            if (simContext.subTasksRemain <= 0 && !simContext.hasTask)
            {
                if (haveAllTasksPerformed())
                {
                    break;
                }
            }
        }
        // якщо жодної дії не виконано, чекаємо
        if (!simContext.actionTaken)
        {
            {
                std::lock_guard<std::mutex> lock(dataBaseMutex);
                simulationClock += simContext.timeUnitToSkip;
                modelWaiting(simContext.timeUnitToSkip);
            }
            wait(simContext.waitMilliSec);
        }
    }
}

bool SimulationEnvironment::haveAllTasksPerformed()
{
    // Моделюємо нульове очікування, щоб задачі з нульовим часом надсилання
    // та (або) нульовим часом виконання обробилися одразу. Причому порядок
    // виклику відрізніється від того, який в методі modelWaiting.
    waitForSendingPool(0);
    waitForResources(0, dataBase->availableResources);

    for (const Task& task : dataBase->tasks)
    {
        // якщо хоч одне завдання не має статус виконано або скасовано,
        // значить ще є завдання, які будуть виконуватися
        if (task.status != Task::TaskStatus::PERFORMED &&
            task.status != Task::TaskStatus::CANCELLED)
        {
            return false;
        }
    }
    return true;
}


int SimulationEnvironment::trySendTaskToResource(
    Task& task,
    Resource& resource,
    bool areSubTasksConnected
)
{
    // кількість задач, які було надіслано
    int sentSubTasksCount = 0;
    // Отримуємо копію завдання, але якщо завдання має статус DIVIDED,
    // то враховуються його дочірні елементи, а саме від загальної суми задач
    // поточного завдання віднімається сума задач дочірніх завдань.
    // Інакше копія ніяк не змінюється.
    Task copyWithRemainingCount = getTaskCopyWithRemainingCount(task);
    bool canTaskBeSent = ResourceManager::canTaskBeSentToResource(
        copyWithRemainingCount, resource, areSubTasksConnected, &sentSubTasksCount
    );

    if (canTaskBeSent)
    {
        Task* taskToBeSent; // завдання, яке буде відправлятися
        // Якщо надсилатися буде не все завдання, а якась частина
        // або завдання вже є розділеним, то беремо його частину,
        // створюємо на її основі нове завдання, яке стане дочірнім
        if (sentSubTasksCount < task.count || task.status == Task::TaskStatus::DIVIDED)
        {
            // створюємо копію головної задачі, яка стане дочірньою
            Task childTask = task;
            childTask.simulationInfo.childTasks = std::list<Task>();
            // присвоюємо айді в форматі [parentId].[childId], наприклад 1.1
            childTask.id += "." + std::to_string(
                task.simulationInfo.childTasks.size() + 1
            );

            childTask.simulationInfo.parentTaskPtr = &task;
            childTask.count = sentSubTasksCount;

            task.simulationInfo.childTasks.push_back(childTask);
            taskToBeSent = &task.simulationInfo.childTasks.back();

            // встановлюємо батьківському завданню статус DIVIDED або DIVIDED_RUNNING
            setDividedTaskStatus(task);
        }
        else
        {
            // інакше будемо відправляти все завдання повністю
            taskToBeSent = &task;
        }

        // хоча завдання ще не виконується, додаємо його до ресурсу,
        // щоб відмітити, що ресурс вже зайнятий, хоча статус ще буде SENDING
        taskToBeSent->status = Task::TaskStatus::SENDING;
        resource.performingTasks.push_back(taskToBeSent);

        logger->logTaskSendingPool(*taskToBeSent);

        // відправляємо задачу в пул задач на очікування
        SendingTask st {
            taskToBeSent,
            getNetworkDelay(*taskToBeSent, resource, sentSubTasksCount)
        };
  
        sendingPool.push_back(st);

        return sentSubTasksCount;
    }
    return 0;
}

Task SimulationEnvironment::getTaskCopyWithRemainingCount(const Task& task)
{
    if (task.status != Task::TaskStatus::DIVIDED)
        return task;
 
    Task copy = task;
    copy.count = Task::getRemainingSubTasksCount(task);
    return copy;
}

void SimulationEnvironment::setDividedTaskStatus(Task& parentTask)
{
    // скільки залишилося нерозподілених на ресурси задач, враховуючи
    // задачі дочірніх завдань
    int remainingCount = Task::getRemainingSubTasksCount(parentTask);
    // якщо задач не залишилося
    if (!remainingCount)
        parentTask.status = Task::TaskStatus::DIVIDED_RUNNING;
    // інакше помічаємо батьківське завдання як просто розділене
    else
        parentTask.status = Task::TaskStatus::DIVIDED;
}

void SimulationEnvironment::wait(int time)
{
    // моделюємо очікування
    std::this_thread::sleep_for(std::chrono::milliseconds(time));
}

void SimulationEnvironment::modelWaiting(double time)
{
    resourceStagnationSum +=
        ResourceManager::calcResourceStagnation(dataBase->availableResources);
    modelWaitingForSubTasks(time);
    calcSubTaskWaitingTimeCurrent();

    logger->logSimulationData(
        SimulationData
        {
            simulationClock,
            getSubTaskWaitingTimeAv(),
            getResourceStagnationAv()
        }
    );

    // спочатку моделюємо очікування на ресурсах, які вже виконуються,
    // щоб завдання не могли надсіслатися і одразу виконатися на якусь частину
    waitForResources(time, dataBase->availableResources);
    waitForSendingPool(time);
}

void SimulationEnvironment::modelWaitingForSubTasks(double time)
{
    for (Task& task : dataBase->tasks)
    {
        if (task.status == Task::TaskStatus::WAITING)
        {
            // додаємо добуток часу виконання однієї задачі на кількість
            // задач в одному завданні
            task.simulationInfo.waitingTime += time *
                task.count;
        }
        else if (task.status == Task::TaskStatus::DIVIDED)
        {
            // додаємо добуток часу виконання однієї задачі на кількість
            // задач, які ще залишилось виконати
            task.simulationInfo.waitingTime += time *
                Task::getRemainingSubTasksCount(task);
        }
    }
}

void SimulationEnvironment::calcSubTaskWaitingTimeCurrent()
{
    subTaskWaitingTimeSum = 0;
    for (const Task& task : dataBase->tasks)
    {
        subTaskWaitingTimeSum += task.simulationInfo.waitingTime;
    }
}

int SimulationEnvironment::getSubTasksCount() const
{
    int sum = 0;
    for (const Task& task : dataBase->tasks)
        sum += task.count;
    return sum;
}

void SimulationEnvironment::waitForSendingPool(double time)
{
    if (sendingPool.empty())
        return;

    // обробляємо одночасно кількість завдань, що не перевищує channelCount
    int tasksToProcess = std::min(static_cast<int>(sendingPool.size()), channelCount);

    // проходимо по задачах, які можемо обробити одночасно
    for (int i = 0; i < tasksToProcess; i++)
    {
        sendingPool[i].timeToSend -= time;

        // якщо завдання було відправлено
        if (sendingPool[i].timeToSend <= 0)
        {
            // встановлюємо статус RUNNING
            sendingPool[i].task->status = Task::TaskStatus::RUNNING;

            // логуємо, що задача відправлена на ресурс
            const Resource* resToLog =
                ResourceManager::findResourceTaskIsPerfOn(
                    sendingPool[i].task,
                    dataBase->availableResources
                );
            if (resToLog)
                logger->logTaskSentToRes(*sendingPool[i].task, *resToLog);
        }
    }

    // видаляємо всі завершені задачі з початку пулу до tasksToProcess
    sendingPool.erase(
        std::remove_if(sendingPool.begin(), sendingPool.begin() + tasksToProcess,
            [](const SendingTask& task) { return task.timeToSend <= 0; }),
        sendingPool.begin() + tasksToProcess);
}


void SimulationEnvironment::waitForResources(
    double time,
    vector<Resource>& resources
)
{
    // проходимо по кожному ресурсу
    for (Resource& res : resources)
    {
        // проходимо по кожному завданню, що виконується на поточному ресурсі.
        // Використовуємо ітератор, щоб видалять завдання, у яких час, що
        // залишився до кінця їх виконання, менше або дорівнює нулю
        for (
            auto iter = res.performingTasks.begin();
            iter != res.performingTasks.end();
            )
        {
            // якщо задача вже виконується
            if ((*iter)->status == Task::TaskStatus::RUNNING)
                // додаємо час, який мав пройти
                (*iter)->simulationInfo.timePerformed += time;

            // якщо завдання вже було виконано
            if ((*iter)->performTime <= (*iter)->simulationInfo.timePerformed)
            {
                // фактичне значення часу виконання могло перевищити загальний
                // час виконання, тому гарантуємо, що вони дорівнюють
                (*iter)->simulationInfo.timePerformed = (*iter)->performTime;
                // помічаємо завдання як виконане
                (*iter)->status = Task::TaskStatus::PERFORMED;
                logger->logTaskPerformed(**iter);
                finishDividedTaskIfNeed(**iter);
                // видаляємо завдання з списку тих, які виконуються
                iter = res.performingTasks.erase(iter);
            }
            else
                ++iter;
        }
    }
}

void SimulationEnvironment::finishDividedTaskIfNeed(Task& childTask)
{
    Task* parentPtr = childTask.simulationInfo.parentTaskPtr;
    // якщо parentPtr=nullptr, значить завдання не є дочірнім
    if (!parentPtr)
        return;

    // якщо завдання не виконується (або виконується не повністю)
    // в розділеному режимі, воно не може бути виконано
    if (parentPtr->status != Task::TaskStatus::DIVIDED_RUNNING)
        return;

    for (Task& child : parentPtr->simulationInfo.childTasks)
    {
        // якщо хоча б одне дочірнє завдання не є виконаним,
        // батьківське завдання не може вважатися виконаним
        if (child.status != Task::TaskStatus::PERFORMED)
            return;
    }
    // якщо всі перевірки пройшли, бітьківське завдання було виконано
    parentPtr->status = Task::TaskStatus::PERFORMED;
    logger->logTaskPerformed(*parentPtr);
}


double SimulationEnvironment::getNetworkDelay(
    const Task& task,
    const Resource& res,
    int subTasksCount
)
{
    // ділимо розмір задачі на пропускну здатність каналу size/(size/time)
    // і отримуємо час, за який відправиться одна задача
    double delayPerSubTask = task.subTaskSize / res.bandwidth;
    return delayPerSubTask * subTasksCount;
}