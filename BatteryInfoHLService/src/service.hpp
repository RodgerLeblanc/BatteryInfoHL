/*
 * Copyright (c) 2013-2015 BlackBerry Limited.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef SERVICE_H_
#define SERVICE_H_

#include <QObject>

#include <bb/device/BatteryInfo>
#include <bb/device/BatteryChargingState>
#include <bb/device/BatteryCondition>

namespace bb {
    class Application;
    namespace platform {
        class Notification;
    }
    namespace system {
        class InvokeManager;
        class InvokeRequest;
    }
}

struct MyBatteryInfo
{
    bb::device::BatteryCondition::Type condition;
    int cycleCount;
    int fullChargeCapacity;
    int level;
    bb::device::BatteryChargingState::Type chargingState;
    bool present;
    QString rxid; // 10.3.0 and up only
    float temperature;
};

class Service: public QObject
{
    Q_OBJECT
public:
    Service();
    virtual ~Service() {}
private slots:
    void handleInvoke(const bb::system::InvokeRequest &);
    void onConditionChanged(bb::device::BatteryCondition::Type condition);
    void onCycleCountChanged(int cycleCount);
    void onFullChargeCapacityChanged(int fullChargeCapacity);
    void onLevelOrChargingStateChanged(int level, bb::device::BatteryChargingState::Type newChargingState);
    void onPresentChanged(bool present);
    void onRxidChanged(QString rxid);
    void onTemperatureChanged(float temperature);

private:
    void notifyHub(const QString& body);

    bb::platform::Notification * m_notify;
    bb::system::InvokeManager * m_invokeManager;

    bb::device::BatteryInfo* m_batteryInfo;

    MyBatteryInfo m_lastBatteryInfo;
};

#endif /* SERVICE_H_ */
