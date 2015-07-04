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

import bb.cascades 1.2
import "CustomComponents"

Page {
    ScrollView {
        Container {
            layout: DockLayout {}
            horizontalAlignment: HorizontalAlignment.Fill
            verticalAlignment: VerticalAlignment.Fill
            Container {
                horizontalAlignment: HorizontalAlignment.Center
                verticalAlignment: VerticalAlignment.Center
                
                CustomOption { optionName: "Notify On Condition Changed" }
                CustomOption { optionName: "Notify On Cycle Count Changed" }
                CustomOption { optionName: "Notify On Full Charge Capacity Changed" }
                CustomOption { optionName: "Notify On Level Changed" }
                CustomOption { optionName: "Notify On Charging State Changed" }
                CustomOption { optionName: "Notify On Present Changed" }
                CustomOption { optionName: "Notify On Rxid Changed" }
                CustomOption { optionName: "Notify On Temperature Changed" }
            }
        }
    }
}
