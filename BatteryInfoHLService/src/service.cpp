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

#include "service.hpp"

#include <bb/Application>
#include <bb/platform/Notification>
#include <bb/platform/NotificationDefaultApplicationSettings>
#include <bb/system/InvokeManager>

#include <QSettings>
#include <QTimer>

using namespace bb::device;
using namespace bb::platform;
using namespace bb::system;

Service::Service() :
        QObject(),
        m_notify(new Notification(this)),
        m_invokeManager(new InvokeManager(this)),
        m_batteryInfo(new BatteryInfo(this))
{
    m_invokeManager->connect(m_invokeManager, SIGNAL(invoked(const bb::system::InvokeRequest&)),
            this, SLOT(handleInvoke(const bb::system::InvokeRequest&)));

    NotificationDefaultApplicationSettings notificationSettings;
    notificationSettings.setPreview(NotificationPriorityPolicy::Allow);
    notificationSettings.apply();

    m_notify->setTitle("BatteryInfoHL Service");

    bb::system::InvokeRequest request;
    request.setTarget("com.CellNinja.BatteryInfoHL");
    request.setAction("bb.action.START");
    m_notify->setInvokeRequest(request);

    // Set the initial values
    m_lastBatteryInfo.chargingState = m_batteryInfo->chargingState();
    m_lastBatteryInfo.condition = m_batteryInfo->condition();
    m_lastBatteryInfo.cycleCount = m_batteryInfo->cycleCount();
    m_lastBatteryInfo.fullChargeCapacity = m_batteryInfo->fullChargeCapacity();
    m_lastBatteryInfo.level = m_batteryInfo->level();
    m_lastBatteryInfo.present = m_batteryInfo->isPresent();
//    m_lastBatteryInfo.rxid = m_batteryInfo->rxid(); // 10.3.0 and up only
    m_lastBatteryInfo.temperature = m_batteryInfo->temperature();

    // Connect to BatteryInfo signals
    connect(m_batteryInfo, SIGNAL(conditionChanged(bb::device::BatteryCondition::Type)), this, SLOT(onConditionChanged(bb::device::BatteryCondition::Type)));
    connect(m_batteryInfo, SIGNAL(cycleCountChanged(int)), this, SLOT(onCycleCountChanged(int)));
    connect(m_batteryInfo, SIGNAL(fullChargeCapacityChanged(int)), this, SLOT(onFullChargeCapacityChanged(int)));
    connect(m_batteryInfo, SIGNAL(levelChanged(int, bb::device::BatteryChargingState::Type)), this, SLOT(onLevelOrChargingStateChanged(int, bb::device::BatteryChargingState::Type)));
    connect(m_batteryInfo, SIGNAL(presentChanged(bool)), this, SLOT(onPresentChanged(bool)));
//    connect(m_batteryInfo, SIGNAL(rxidChanged(QString)), this, SLOT(onRxidChanged(QString))); // 10.3.0 and up only
    connect(m_batteryInfo, SIGNAL(temperatureChanged(float)), this, SLOT(onTemperatureChanged(float)));
}

void Service::handleInvoke(const bb::system::InvokeRequest & request)
{
    // Not used
    Q_UNUSED(request);
}

void Service::notifyHub(const QString& body) {
    m_notify->setBody(body);
    m_notify->notify();
}

void Service::onConditionChanged(BatteryCondition::Type condition) {
    if (m_lastBatteryInfo.condition == condition)
        return;

    QSettings settings;
    if (settings.value("NotifyOnConditionChanged", true).toBool()) {
        QString newCondition;
        switch(condition) {
            case BatteryCondition::Bad : {
                newCondition = "Bad";
                break;
            }
            case BatteryCondition::OK : {
                newCondition = "OK";
                break;
            }
            case BatteryCondition::Unknown : {
                newCondition = "Unknown";
                break;
            }
            default : {
                newCondition = "Unknown";
                break;
            }
        }

        notifyHub("Condition changed, new condition: " + newCondition);
    }

    m_lastBatteryInfo.condition = condition;
}

void Service::onCycleCountChanged(int cycleCount) {
    if (m_lastBatteryInfo.cycleCount == cycleCount)
        return;

    QSettings settings;
    if (settings.value("NotifyOnCycleCountChanged", true).toBool()) {
        notifyHub("Cycle count changed, new cycle count: " + QString::number(cycleCount));
    }

    m_lastBatteryInfo.cycleCount = cycleCount;
}

void Service::onFullChargeCapacityChanged(int fullChargeCapacity) {
    if (m_lastBatteryInfo.fullChargeCapacity == fullChargeCapacity)
        return;

    QSettings settings;
    if (settings.value("NotifyOnFullChargeCapacityChanged", true).toBool()) {
        notifyHub("Full charge capacity changed, new full charge capacity: " + QString::number(fullChargeCapacity));
    }

    m_lastBatteryInfo.fullChargeCapacity = fullChargeCapacity;
}

void Service::onLevelOrChargingStateChanged(int level, BatteryChargingState::Type newChargingState) {
    QSettings settings;

    if (m_lastBatteryInfo.level != level) {
        if (settings.value("NotifyOnLevelChanged", true).toBool()) {
            notifyHub("Level changed, new level: " + QString::number(level) + "%");
        }
    }

    if (m_lastBatteryInfo.chargingState != newChargingState) {
        if (settings.value("NotifyOnChargingStateChanged", true).toBool()) {
            QString newState;
            switch(newChargingState) {
                case BatteryChargingState::Charging : {
                    newState = "Charging";
                    break;
                }
                case BatteryChargingState::Discharging : {
                    newState = "Discharging";
                    break;
                }
                case BatteryChargingState::Full : {
                    newState = "Full";
                    break;
                }
                case BatteryChargingState::NotCharging : {
                    newState = "Not Charging";
                    break;
                }
                case BatteryChargingState::Unknown : {
                    newState = "Unknown";
                    break;
                }
                default : {
                    newState = "Unknown";
                    break;
                }
            }

            notifyHub("Charging state changed, new charging state: " + newState);
        }
    }

    m_lastBatteryInfo.level = level;
    m_lastBatteryInfo.chargingState = newChargingState;
}

void Service::onPresentChanged(bool present) {
    if (m_lastBatteryInfo.present == present)
        return;

    QSettings settings;
    if (settings.value("NotifyOnPresentChanged", true).toBool()) {
        notifyHub("Present changed, new present: " + (present ? QString("True") : QString("False")));
    }

    m_lastBatteryInfo.present = present;
}

void Service::onRxidChanged(QString rxid) {
    if (m_lastBatteryInfo.rxid == rxid)
        return;

    QSettings settings;
    if (settings.value("NotifyOnRxidChanged", true).toBool()) {
        notifyHub("Rxid changed, new rxid: " + rxid);
    }

    m_lastBatteryInfo.rxid = rxid;
}

void Service::onTemperatureChanged(float temperature) {
    if (m_lastBatteryInfo.temperature == temperature)
        return;

    QSettings settings;
    if (settings.value("NotifyOnTemperatureChanged", true).toBool()) {
        QByteArray degreeSign = "°";
        notifyHub("Temperature changed, new temperature: " + QString::number(temperature) + QString::fromUtf8(degreeSign) + "C");
    }

    m_lastBatteryInfo.temperature = temperature;
}
