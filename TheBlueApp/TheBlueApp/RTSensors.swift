//
//  RTSensors.swift
//  TheBlueApp
//
//  Created by Max Gulda on 2023-08-27.
//

import Foundation
import CoreBluetooth

enum RTSensorType {
    case right_leg
    case left_leg
    case chest
    case finapress
}

struct RTSensor {
    let peripheral: CBPeripheral
    let name: String
    let type: RTSensorType
    var isConnected: Bool
}
