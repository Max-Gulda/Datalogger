//
//  BLEController.swift
//  TheBlueApp
//
//  Created by Max Gulda on 2023-08-27.
//

import Foundation
import CoreBluetooth
import Combine

class BLEController: NSObject, ObservableObject, CBCentralManagerDelegate, CBPeripheralDelegate {
    
    var myCentral: CBCentralManager!
    
    @Published var isSwitchedOn = false
    // Finapress
    let finapressServiceUUID = CBUUID(string: "6E54EED4-9FC2-4EC4-BE2B-D6FBE2B3D9C7")
    let finapressCharacteristicUUID_tx = CBUUID(string: "A2D0DB30-5C84-4377-BE6D-07F9ACD958E9")
    let finapressCharacteristicUUID_start_stop = CBUUID(string: "6BCBB238-6652-4894-8332-B2D0124733A5")
    let finapressServiceDataUUID = CBUUID(string: "DAAB73E7-4E08-4DF9-88B5-560655A8024E")
    let finapressCharacteristicUUID_ADC_A0 = CBUUID(string: "CE9D265D-FBFA-4771-BA57-986D9A55C0A3")
    let finapressCharacteristicUUID_ADC_A1 = CBUUID(string: "F639D3CC-21C5-44D7-A115-BBB1731A3D86")
    let finapressCharacteristicUUID_ADC_A2 = CBUUID(string: "E189BE3E-355D-4FC1-A0D5-8651F600900C")
    let finapressCharacteristicUUID_ADC_A3 = CBUUID(string: "B723FA0C-F044-4871-843D-79A7C3201CCF")
    let finapressCharacteristicUUID_ADC_A4 = CBUUID(string: "1F7E8039-C922-4609-AF61-E602780396C9")
    let finapressCharacteristicUUID_ADC_A5 = CBUUID(string: "7BC207C6-3192-4890-A4A0-FE627630D663")
    let finapressCharacteristicUUID_ADC_A6 = CBUUID(string: "1088C846-5D02-431A-9C41-1E05DBC3CE2D")
    let finapressCharacteristicUUID_ADC_A7 = CBUUID(string: "44809D0C-28B0-45E7-9D17-026F8254C55A")
    
    // Left Leg
    let leftLegServiceUUID = CBUUID(string: "64530F27-3632-4070-82DA-3E6BA60B27AD")
    let leftLegCharacteristicUUID_tx = CBUUID(string: "9CC29B03-C3E1-421D-949B-5763479C3D33")
    let leftLegCharacteristicUUID_start_stop = CBUUID(string: "4C8C4047-7CC9-4BDF-A008-4ED0B35EF3AA")
    let leftLegServiceDataUUID = CBUUID(string: "334F7C35-0F96-4443-A364-B7DDD07E9AE1")
    let leftLegCharacteristicUUID_ACC_X = CBUUID(string: "07980EF5-7E56-4F40-91B0-BD312F86C62F")
    let leftLegCharacteristicUUID_ACC_Y = CBUUID(string: "808FF366-ABCD-42FE-82BC-3E0E2CA6636E")
    let leftLegCharacteristicUUID_ACC_Z = CBUUID(string: "F6DFC378-A314-448C-902C-97016FB314C9")
    let leftLegCharacteristicUUID_GYRO_X = CBUUID(string: "5D78290C-50BF-4EEC-BF6D-50F0C42BF432")
    let leftLegCharacteristicUUID_GYRO_Y = CBUUID(string: "55CB1DCE-706C-4992-951E-A91B24433E4F")
    let leftLegCharacteristicUUID_GYRO_Z = CBUUID(string: "F68CE15C-95E0-474D-90F7-D5D9137CD9B9")
    let leftLegCharacteristicUUID_EMG = CBUUID(string: "5DEB7A06-CA45-4EF2-9309-DE897B03D549")
    
    // Chest
    let chestServiceUUID = CBUUID(string: "26931357-A022-4C29-AA56-BCBBB3C72C94")
    let chestCharacteristicUUID_tx = CBUUID(string: "3DF2D329-A9D4-4D0E-9600-154072BE7544")
    let chestCharacteristicUUID_start_stop = CBUUID(string: "1FF7D19B-5B6F-4A30-81BB-C76A61D71D98")
    let chestServiceDataUUID = CBUUID(string: "8A82BEA0-7A31-49A7-8771-2BF0E56B1955")
    let chestCharacteristicUUID_ACC_X = CBUUID(string: "2C15828E-71D6-4792-AA29-1F96E2189E96")
    let chestCharacteristicUUID_ACC_Y = CBUUID(string: "42403077-321E-49DE-9538-0D1F7C8CE15D")
    let chestCharacteristicUUID_ACC_Z = CBUUID(string: "D2F534FF-C04F-4466-A22F-64622A47EB23")
    let chestCharacteristicUUID_GYRO_X = CBUUID(string: "4292DC20-A07D-4993-A71F-4A9CBDC11EDB")
    let chestCharacteristicUUID_GYRO_Y = CBUUID(string: "0F845E80-CB2D-49A0-8DEA-52A5B5C289B8")
    let chestCharacteristicUUID_GYRO_Z = CBUUID(string: "EF25FE39-AEA3-4079-86D6-1A5EB341EEE1")
    let chestCharacteristicUUID_EMG = CBUUID(string: "3B427CE5-34B3-459D-8F0F-FE5A3506A47A")
    
    // Right Leg
    let rightLegServiceUUID = CBUUID(string: "99FD4277-7073-4DDD-8486-A3EB735A832C")
    let rightLegCharacteristicUUID_tx = CBUUID(string: "32489387-B321-4329-B4F3-413F7F0E5F08")
    let rightLegCharacteristicUUID_start_stop = CBUUID(string: "43AD0C7C-AAB2-42A8-B880-81457B8722BC")
    let rightLegServiceDataUUID = CBUUID(string: "F86BA921-6AEB-4B67-BB58-668049C9E588")
    let rightLegCharacteristicUUID_ACC_X = CBUUID(string: "DD64FED8-BE2A-4052-9E84-3054FFC21058")
    let rightLegCharacteristicUUID_ACC_Y = CBUUID(string: "062B4B58-7583-469B-B579-391B70252575")
    let rightLegCharacteristicUUID_ACC_Z = CBUUID(string: "6E387753-E305-461A-9F0A-9955252AA6A9")
    let rightLegCharacteristicUUID_GYRO_X = CBUUID(string: "D29E3D76-6220-4218-BF1C-DCC17BC967DB")
    let rightLegCharacteristicUUID_GYRO_Y = CBUUID(string: "3600361E-FFEF-46B9-8EA7-4BA1DB5D3F2C")
    let rightLegCharacteristicUUID_GYRO_Z = CBUUID(string: "1920F52D-E85D-4A72-B917-B59AFA425621")
    let rightLegCharacteristicUUID_EMG = CBUUID(string: "B86949BA-1DE3-4044-8C26-6DD5B0B9031A")
    
    var sensorType: RTSensorType!
    
    override init() {
        super.init()
        myCentral = CBCentralManager(delegate: self, queue: nil)
    }
    
    func centralManagerDidUpdateState(_ central: CBCentralManager) {
        if central.state == .poweredOn {
            isSwitchedOn = true
        }
        else {
            isSwitchedOn = false
        }
    }
    
    func connectToSensor(type: RTSensorType) {
        sensorType = type
        var serviceUUIDs: [CBUUID] = []
        
        switch sensorType {
        case .right_leg:
            serviceUUIDs = [rightLegServiceUUID]
        case .left_leg:
            serviceUUIDs = [leftLegServiceUUID]
        case .chest:
            serviceUUIDs = [chestServiceUUID]
        case .finapress:
            serviceUUIDs = [finapressServiceUUID]
        case .none: // Add the colon here
            serviceUUIDs = []
        }
        print(serviceUUIDs)
        myCentral.scanForPeripherals(withServices: serviceUUIDs, options: nil)
    }
    
    @Published var sensors: [RTSensor] = []
    
    func centralManager(_ central: CBCentralManager, didDiscover peripheral: CBPeripheral, advertisementData: [String: Any], rssi RSSI: NSNumber) {
            
        if let serviceUUIDs = advertisementData[CBAdvertisementDataServiceUUIDsKey] as? [CBUUID] {
            for serviceUUID in serviceUUIDs {
                print("Service UUID: \(serviceUUID.uuidString)")
            }
        }
        
        // Set the delegate for the peripheral
        peripheral.delegate = self
        
        // Add the peripheral to your sensors list
        sensors.append(RTSensor(peripheral: peripheral, name: peripheral.name ?? "NoName", type: sensorType, isConnected: false))
        //sensors.last!.peripheral = self
        
        // Stop scanning and connect to the peripheral
        central.stopScan()
        central.connect(sensors.last!.peripheral, options: nil)
    }

    
    func centralManager(_ central: CBCentralManager, didConnect peripheral: CBPeripheral) {
        print("I just connected to something!")
        if let connectedSensorIndex = sensors.firstIndex(where: { $0.peripheral == peripheral }) {
            // Set the connection status based on the connected sensor type
            sensors[connectedSensorIndex].isConnected = true
        }
        // Now that the peripheral is connected, you can discover the services and characteristics you need
        peripheral.discoverServices([rightLegServiceUUID, rightLegServiceDataUUID, leftLegServiceUUID,leftLegServiceDataUUID, chestServiceUUID, chestServiceDataUUID, finapressServiceUUID, finapressServiceDataUUID])
    }
    
    
    func peripheral(_ peripheral: CBPeripheral, didDiscoverServices error: Error?) {
        print("I just discovered a service!")
        guard error == nil else {
            // Handle the error
            return
        }
        
        // Loop through the discovered services
        for service in peripheral.services ?? [] {
            if service.uuid == rightLegServiceUUID {
                // This is the right leg sensor service
                // Now discover and interact with its characteristics
                peripheral.discoverCharacteristics([rightLegCharacteristicUUID_tx,rightLegCharacteristicUUID_start_stop], for: service)
            } else if service.uuid == leftLegServiceUUID {
                // This is the left leg sensor service
                // Now discover and interact with its characteristics
                peripheral.discoverCharacteristics([leftLegCharacteristicUUID_tx, leftLegCharacteristicUUID_start_stop], for: service)
            } else if service.uuid == chestServiceUUID {
                // This is the chest sensor service
                // Now discover and interact with its characteristics
                peripheral.discoverCharacteristics([chestCharacteristicUUID_tx, chestCharacteristicUUID_start_stop], for: service)
            } else if service.uuid == finapressServiceUUID {
                // This is the finapress sensor service
                // Now discover and interact with its characteristics
                peripheral.discoverCharacteristics([finapressCharacteristicUUID_tx, finapressCharacteristicUUID_start_stop], for: service)
            } else if service.uuid == rightLegServiceDataUUID {
                peripheral.discoverCharacteristics([rightLegCharacteristicUUID_ACC_X, rightLegCharacteristicUUID_ACC_Y,rightLegCharacteristicUUID_ACC_Z,rightLegCharacteristicUUID_GYRO_X,rightLegCharacteristicUUID_GYRO_Y,rightLegCharacteristicUUID_GYRO_Z,rightLegCharacteristicUUID_EMG], for: service)
            } else if service.uuid == leftLegServiceDataUUID {
                peripheral.discoverCharacteristics([leftLegCharacteristicUUID_ACC_X, leftLegCharacteristicUUID_ACC_Y,leftLegCharacteristicUUID_ACC_Z,leftLegCharacteristicUUID_GYRO_X,leftLegCharacteristicUUID_GYRO_Y,leftLegCharacteristicUUID_GYRO_Z,leftLegCharacteristicUUID_EMG], for: service)
            } else if service.uuid == chestServiceDataUUID {
                peripheral.discoverCharacteristics([chestCharacteristicUUID_ACC_X, chestCharacteristicUUID_ACC_Y,chestCharacteristicUUID_ACC_Z,chestCharacteristicUUID_GYRO_X,chestCharacteristicUUID_GYRO_Y,chestCharacteristicUUID_GYRO_Z,chestCharacteristicUUID_EMG,chestCharacteristicUUID_ACC_X, chestCharacteristicUUID_ACC_Y,chestCharacteristicUUID_ACC_Z,chestCharacteristicUUID_GYRO_X], for: service)
            } else if service.uuid == finapressServiceDataUUID {
                peripheral.discoverCharacteristics([finapressCharacteristicUUID_ADC_A0, finapressCharacteristicUUID_ADC_A1,finapressCharacteristicUUID_ADC_A2,finapressCharacteristicUUID_ADC_A3,finapressCharacteristicUUID_ADC_A4,finapressCharacteristicUUID_ADC_A5,finapressCharacteristicUUID_ADC_A6,finapressCharacteristicUUID_ADC_A7], for: service)
            }
        }
    }
        
        func peripheral(_ peripheral: CBPeripheral, didDiscoverCharacteristicsFor service: CBService, error: Error?) {
                if let error = error {
                    // Handle the error
                    print("Error discovering characteristics for service \(service.uuid): \(error.localizedDescription)")
                    return
                }
                
                // Loop through the discovered characteristics and print their UUIDs
                if let characteristics = service.characteristics {
                    for characteristic in characteristics {
                        print("Characteristic UUID: \(characteristic.uuid.uuidString)")
                        
                        // Set up notifications for characteristics with the .notify property
                        if characteristic.properties.contains(.notify) {
                            print("\(characteristic.uuid): prop contains notify")
                            peripheral.setNotifyValue(true, for: characteristic)
                        }
                        
                        // You can print other properties or information about the characteristic as needed
                    }
                }
            }
        
        
        
        func centralManager(_ central: CBCentralManager, didDisconnectPeripheral peripheral: CBPeripheral, error: Error?) {
            if let disconnectedSensorIndex = sensors.firstIndex(where: { $0.peripheral == peripheral }) {
                // Set the connection status based on the disconnected sensor type
                sensors[disconnectedSensorIndex].isConnected = false
            }
            // Handle disconnection error if needed
        }
        
        
    @Published var lastDataUpdateTime: Date? = nil
    
    @Published var lastDataUpdateTime_left: Date? = nil
    @Published var lastDataUpdateTime_right: Date? = nil
    @Published var lastDataUpdateTime_chest: Date? = nil
    @Published var lastDataUpdateTime_fina: Date? = nil
    
    
    @Published var leftLeg_Start_Stop = 0
    @Published var leftLeg_ACC_X = 0.0
    @Published var leftLeg_ACC_Y = 0.0
    @Published var leftLeg_ACC_Z = 0.0
    @Published var leftLeg_Gyro_X = 0.0
    @Published var leftLeg_Gyro_Y = 0.0
    @Published var leftLeg_Gyro_Z = 0.0
    @Published var leftLeg_EMG = 0.0
    
    @Published var rightLeg_Start_Stop = 0
    @Published var rightLeg_ACC_X = 0.0
    @Published var rightLeg_ACC_Y = 0.0
    @Published var rightLeg_ACC_Z = 0.0
    @Published var rightLeg_Gyro_X = 0.0
    @Published var rightLeg_Gyro_Y = 0.0
    @Published var rightLeg_Gyro_Z = 0.0
    @Published var rightLeg_EMG = 0.0
    
    @Published var chest_Start_Stop = 0
    @Published var chest_ACC_X = 0.0
    @Published var chest_ACC_Y = 0.0
    @Published var chest_ACC_Z = 0.0
    @Published var chest_Gyro_X = 0.0
    @Published var chest_Gyro_Y = 0.0
    @Published var chest_Gyro_Z = 0.0
    @Published var chest_EMG = 0.0
    
    @Published var finapress_Start_Stop = 0
    @Published var finapress_ADC_A0 = 0.0
    @Published var finapress_ADC_A1 = 0.0
    @Published var finapress_ADC_A2 = 0.0
    @Published var finapress_ADC_A3 = 0.0
    @Published var finapress_ADC_A4 = 0.0
    @Published var finapress_ADC_A5 = 0.0
    @Published var finapress_ADC_A6 = 0.0
    @Published var finapress_ADC_A7 = 0.0
    @Published var finapress_ADC_A8 = 0.0
    
        func peripheral(_ peripheral: CBPeripheral, didUpdateValueFor characteristic: CBCharacteristic, error: Error?) {
            if let error = error {
                print("Error receiving data for characteristic \(characteristic.uuid.uuidString): \(error.localizedDescription)")
                return
            }
            
            if let receivedData = characteristic.value {
                switch characteristic.uuid.uuidString {
                    // Left Leg Characteristics
                case leftLegCharacteristicUUID_start_stop.uuidString:
                    guard let characteristicData = characteristic.value else { return }
                    let byteArray = [UInt8](characteristicData)
                    var value = 0

                    for element in byteArray {
                        value <<= 8 // Shift left by 8 bits (equivalent to *= 256)
                        value |= Int(element)
                    }

                    if value > Int16.max {
                        // If the most significant bit (sign bit) is set, the value is negative
                        value -= Int(1 << 16)

                    }
                    leftLeg_Start_Stop = value;
                    print("Left leg Start Stop: \(value)")
                    lastDataUpdateTime = Date()
                    lastDataUpdateTime_left = lastDataUpdateTime
                case leftLegCharacteristicUUID_ACC_X.uuidString:
                    guard let characteristicData = characteristic.value else { return }
                    let byteArray = [UInt8](characteristicData)
                    var value = 0

                    for element in byteArray {
                        value <<= 8 // Shift left by 8 bits (equivalent to *= 256)
                        value |= Int(element)
                    }

                    if value > Int16.max {
                        // If the most significant bit (sign bit) is set, the value is negative
                        value -= Int(1 << 16)

                    }
                    
                    let valueDouble = Double(value) / 4096.0
                    leftLeg_ACC_X = valueDouble;
                    print("Left leg ACC X: \(valueDouble)")
                    lastDataUpdateTime = Date()
                case leftLegCharacteristicUUID_ACC_Y.uuidString:
                    guard let characteristicData = characteristic.value else { return }
                    let byteArray = [UInt8](characteristicData)
                    var value = 0

                    for element in byteArray {
                        value <<= 8 // Shift left by 8 bits (equivalent to *= 256)
                        value |= Int(element)
                    }

                    if value > Int16.max {
                        // If the most significant bit (sign bit) is set, the value is negative
                        value -= Int(1 << 16)

                    }

                    let valueDouble = Double(value) / 4096.0
                    leftLeg_ACC_Y = valueDouble;
                                        
                    print("Left leg ACC Y: \(valueDouble)")
                    lastDataUpdateTime = Date()
                    lastDataUpdateTime_left = lastDataUpdateTime
                case leftLegCharacteristicUUID_ACC_Z.uuidString:
                    guard let characteristicData = characteristic.value else { return }
                    let byteArray = [UInt8](characteristicData)
                    var value = 0

                    for element in byteArray {
                        value <<= 8 // Shift left by 8 bits (equivalent to *= 256)
                        value |= Int(element)
                    }

                    if value > Int16.max {
                        // If the most significant bit (sign bit) is set, the value is negative
                        value -= Int(1 << 16)

                    }

                    let valueDouble = Double(value) / 4096.0
                    leftLeg_ACC_Z = valueDouble;
                                        
                    print("Left leg ACC Z: \(valueDouble)")
                    lastDataUpdateTime = Date()
                    lastDataUpdateTime_left = lastDataUpdateTime
                case leftLegCharacteristicUUID_GYRO_X.uuidString:
                    guard let characteristicData = characteristic.value else { return }
                    let byteArray = [UInt8](characteristicData)
                    var value = 0

                    for element in byteArray {
                        value <<= 8 // Shift left by 8 bits (equivalent to *= 256)
                        value |= Int(element)
                    }

                    if value > Int16.max {
                        // If the most significant bit (sign bit) is set, the value is negative
                        value -= Int(1 << 16)

                    }

                    let valueDouble = Double(value) / 4096.0
                    leftLeg_Gyro_X = valueDouble;
                                        
                    print("Left leg Gyro X: \(valueDouble)")
                    lastDataUpdateTime = Date()
                    lastDataUpdateTime_left = lastDataUpdateTime
                case leftLegCharacteristicUUID_GYRO_Y.uuidString:
                    guard let characteristicData = characteristic.value else { return }
                    let byteArray = [UInt8](characteristicData)
                    var value = 0

                    for element in byteArray {
                        value <<= 8 // Shift left by 8 bits (equivalent to *= 256)
                        value |= Int(element)
                    }

                    if value > Int16.max {
                        // If the most significant bit (sign bit) is set, the value is negative
                        value -= Int(1 << 16)

                    }

                    let valueDouble = Double(value) / 4096.0
                    leftLeg_Gyro_Y = valueDouble;
                                        
                    print("Left leg Gyro Y: \(valueDouble)")
                    lastDataUpdateTime = Date()
                    lastDataUpdateTime_left = lastDataUpdateTime
                case leftLegCharacteristicUUID_GYRO_Z.uuidString:
                    guard let characteristicData = characteristic.value else { return }
                    let byteArray = [UInt8](characteristicData)
                    var value = 0

                    for element in byteArray {
                        value <<= 8 // Shift left by 8 bits (equivalent to *= 256)
                        value |= Int(element)
                    }

                    if value > Int16.max {
                        // If the most significant bit (sign bit) is set, the value is negative
                        value -= Int(1 << 16)

                    }

                    let valueDouble = Double(value) / 4096.0
                    leftLeg_Gyro_Z = valueDouble;
                                        
                    print("Left leg Gyro Z: \(valueDouble)")
                    lastDataUpdateTime = Date()
                    lastDataUpdateTime_left = lastDataUpdateTime
                case leftLegCharacteristicUUID_EMG.uuidString:
                    guard let characteristicData = characteristic.value else { return }
                    let byteArray = [UInt8](characteristicData)
                    var value = 0
                    
                    for element in byteArray {
                        value <<= 8 // Shift left by 8 bits (equivalent to *= 256)
                        value |= Int(element)
                    }

                    if value > Int16.max {
                        // If the most significant bit (sign bit) is set, the value is negative
                        value -= Int(1 << 16)

                    }

                    let valueDouble = Double(value) / 4096.0
                    leftLeg_EMG = valueDouble * 3.3
                    //valueDouble *= 3.3
                    print("Left leg EMG: \(valueDouble)")
                    lastDataUpdateTime = Date()
                    lastDataUpdateTime_left = lastDataUpdateTime
                    
                    // Right Leg Characteristics
                case rightLegCharacteristicUUID_start_stop.uuidString:
                    guard let characteristicData = characteristic.value else { return }
                    let byteArray = [UInt8](characteristicData)
                    var value = 0
                    for element in byteArray {
                        value <<= 8 // Shift left by 8 bits (equivalent to *= 256)
                        value |= Int(element)
                    }

                    if value > Int16.max {
                        // If the most significant bit (sign bit) is set, the value is negative
                        value -= Int(1 << 16)

                    }
                    rightLeg_Start_Stop = value
                    print("Right leg Start Stop: \(value)")
                    lastDataUpdateTime = Date()
                    lastDataUpdateTime_right = lastDataUpdateTime
                case rightLegCharacteristicUUID_ACC_X.uuidString:
                    guard let characteristicData = characteristic.value else { return }
                    let byteArray = [UInt8](characteristicData)
                    var value = 0

                    for element in byteArray {
                        value <<= 8 // Shift left by 8 bits (equivalent to *= 256)
                        value |= Int(element)
                    }

                    if value > Int16.max {
                        // If the most significant bit (sign bit) is set, the value is negative
                        value -= Int(1 << 16)

                    }

                    let valueDouble = Double(value) / 4096.0
                    rightLeg_ACC_X = valueDouble
                    print("Right leg ACC X: \(valueDouble)")
                    lastDataUpdateTime = Date()
                    lastDataUpdateTime_right = lastDataUpdateTime
                case rightLegCharacteristicUUID_ACC_Y.uuidString:
                    guard let characteristicData = characteristic.value else { return }
                    let byteArray = [UInt8](characteristicData)
                    var value = 0
                    for element in byteArray {
                        value <<= 8 // Shift left by 8 bits (equivalent to *= 256)
                        value |= Int(element)
                    }

                    if value > Int16.max {
                        // If the most significant bit (sign bit) is set, the value is negative
                        value -= Int(1 << 16)

                    }

                    let valueDouble = Double(value) / 4096.0
                    rightLeg_ACC_Y = valueDouble
                    print("Right leg ACC Y: \(valueDouble)")
                    lastDataUpdateTime = Date()
                    lastDataUpdateTime_right = lastDataUpdateTime
                case rightLegCharacteristicUUID_ACC_Z.uuidString:
                    guard let characteristicData = characteristic.value else { return }
                    let byteArray = [UInt8](characteristicData)
                    var value = 0
                    for element in byteArray {
                        value <<= 8 // Shift left by 8 bits (equivalent to *= 256)
                        value |= Int(element)
                    }

                    if value > Int16.max {
                        // If the most significant bit (sign bit) is set, the value is negative
                        value -= Int(1 << 16)

                    }

                    let valueDouble = Double(value) / 4096.0
                    rightLeg_ACC_Z = valueDouble
                    print("Right leg ACC Z: \(valueDouble)")
                    lastDataUpdateTime = Date()
                    lastDataUpdateTime_right = lastDataUpdateTime
                case rightLegCharacteristicUUID_GYRO_X.uuidString:
                    guard let characteristicData = characteristic.value else { return }
                    let byteArray = [UInt8](characteristicData)
                    var value = 0
                    for element in byteArray {
                        value <<= 8 // Shift left by 8 bits (equivalent to *= 256)
                        value |= Int(element)
                    }

                    if value > Int16.max {
                        // If the most significant bit (sign bit) is set, the value is negative
                        value -= Int(1 << 16)

                    }

                    let valueDouble = Double(value) / 4096.0
                    rightLeg_Gyro_X = valueDouble
                    print("Right leg Gyro X: \(valueDouble)")
                    lastDataUpdateTime = Date()
                    lastDataUpdateTime_right = lastDataUpdateTime

                case rightLegCharacteristicUUID_GYRO_Y.uuidString:
                    guard let characteristicData = characteristic.value else { return }
                    let byteArray = [UInt8](characteristicData)
                    var value = 0
                    for element in byteArray {
                        value <<= 8 // Shift left by 8 bits (equivalent to *= 256)
                        value |= Int(element)
                    }

                    if value > Int16.max {
                        // If the most significant bit (sign bit) is set, the value is negative
                        value -= Int(1 << 16)

                    }

                    let valueDouble = Double(value) / 4096.0
                    rightLeg_Gyro_Y = valueDouble
                    print("Right leg Gyro Y: \(valueDouble)")
                    lastDataUpdateTime = Date()
                    lastDataUpdateTime_right = lastDataUpdateTime
                case rightLegCharacteristicUUID_GYRO_Z.uuidString:
                    guard let characteristicData = characteristic.value else { return }
                    let byteArray = [UInt8](characteristicData)
                    var value = 0
                    for element in byteArray {
                        value <<= 8 // Shift left by 8 bits (equivalent to *= 256)
                        value |= Int(element)
                    }

                    if value > Int16.max {
                        // If the most significant bit (sign bit) is set, the value is negative
                        value -= Int(1 << 16)

                    }

                    let valueDouble = Double(value) / 4096.0
                    rightLeg_Gyro_Z = valueDouble
                    print("Right leg Gyro Z: \(valueDouble)")
                    lastDataUpdateTime = Date()
                    lastDataUpdateTime_right = lastDataUpdateTime
                case rightLegCharacteristicUUID_EMG.uuidString:
                    guard let characteristicData = characteristic.value else { return }
                    let byteArray = [UInt8](characteristicData)
                    var value = 0
                    for element in byteArray {
                        value <<= 8 // Shift left by 8 bits (equivalent to *= 256)
                        value |= Int(element)
                    }

                    if value > Int16.max {
                        // If the most significant bit (sign bit) is set, the value is negative
                        value -= Int(1 << 16)

                    }

                    let valueDouble = Double(value) / 4096.0
                    rightLeg_EMG = valueDouble * 3.3
                    print("Right leg EMG: \(valueDouble)")
                    lastDataUpdateTime = Date()
                    lastDataUpdateTime_right = lastDataUpdateTime
                case chestCharacteristicUUID_start_stop.uuidString:
                    guard let characteristicData = characteristic.value else { return }
                    let byteArray = [UInt8](characteristicData)
                    var value = 0
                    for element in byteArray {
                        value <<= 8 // Shift left by 8 bits (equivalent to *= 256)
                        value |= Int(element)
                    }

                    if value > Int16.max {
                        // If the most significant bit (sign bit) is set, the value is negative
                        value -= Int(1 << 16)

                    }
                    chest_Start_Stop = value
                    print("Chest START STOP: \(value)")
                    lastDataUpdateTime = Date()
                    lastDataUpdateTime_chest = lastDataUpdateTime
                case chestCharacteristicUUID_ACC_X.uuidString:
                    guard let characteristicData = characteristic.value else { return }
                    let byteArray = [UInt8](characteristicData)
                    var value = 0
                    for element in byteArray {
                        value <<= 8 // Shift left by 8 bits (equivalent to *= 256)
                        value |= Int(element)
                    }

                    if value > Int16.max {
                        // If the most significant bit (sign bit) is set, the value is negative
                        value -= Int(1 << 16)

                    }

                    let valueDouble = Double(value) / 4096.0
                    chest_ACC_X = valueDouble
                    print("Chest Acc X: \(valueDouble)")
                    lastDataUpdateTime = Date()
                    lastDataUpdateTime_chest = lastDataUpdateTime
                case chestCharacteristicUUID_ACC_Y.uuidString:
                    guard let characteristicData = characteristic.value else { return }
                    let byteArray = [UInt8](characteristicData)
                    var value = 0
                    for element in byteArray {
                        value <<= 8 // Shift left by 8 bits (equivalent to *= 256)
                        value |= Int(element)
                    }

                    if value > Int16.max {
                        // If the most significant bit (sign bit) is set, the value is negative
                        value -= Int(1 << 16)

                    }

                    let valueDouble = Double(value) / 4096.0
                    chest_ACC_Y = valueDouble
                    print("Chest ACC Y: \(valueDouble)")
                    lastDataUpdateTime = Date()
                    lastDataUpdateTime_chest = lastDataUpdateTime
                case chestCharacteristicUUID_ACC_Z.uuidString:
                    guard let characteristicData = characteristic.value else { return }
                    let byteArray = [UInt8](characteristicData)
                    var value = 0
                    for element in byteArray {
                        value <<= 8 // Shift left by 8 bits (equivalent to *= 256)
                        value |= Int(element)
                    }

                    if value > Int16.max {
                        // If the most significant bit (sign bit) is set, the value is negative
                        value -= Int(1 << 16)

                    }

                    let valueDouble = Double(value) / 4096.0
                    chest_ACC_Z = valueDouble
                    print("Chest ACC Z: \(valueDouble)")
                    lastDataUpdateTime = Date()
                    lastDataUpdateTime_chest = lastDataUpdateTime
                case chestCharacteristicUUID_GYRO_X.uuidString:
                    guard let characteristicData = characteristic.value else { return }
                    let byteArray = [UInt8](characteristicData)
                    var value = 0
                    for element in byteArray {
                        value <<= 8 // Shift left by 8 bits (equivalent to *= 256)
                        value |= Int(element)
                    }

                    if value > Int16.max {
                        // If the most significant bit (sign bit) is set, the value is negative
                        value -= Int(1 << 16)

                    }

                    let valueDouble = Double(value) / 4096.0
                    chest_Gyro_X = valueDouble
                    print("Chest GYRO X: \(valueDouble)")
                    lastDataUpdateTime = Date()
                    lastDataUpdateTime_chest = lastDataUpdateTime
                case chestCharacteristicUUID_GYRO_Y.uuidString:
                    guard let characteristicData = characteristic.value else { return }
                    let byteArray = [UInt8](characteristicData)
                    var value = 0
                    for element in byteArray {
                        value <<= 8 // Shift left by 8 bits (equivalent to *= 256)
                        value |= Int(element)
                    }

                    if value > Int16.max {
                        // If the most significant bit (sign bit) is set, the value is negative
                        value -= Int(1 << 16)

                    }

                    let valueDouble = Double(value) / 4096.0
                    chest_Gyro_Y = valueDouble
                    print("Chest GYRO Y: \(valueDouble)")
                    lastDataUpdateTime = Date()
                    lastDataUpdateTime_chest = lastDataUpdateTime
                case chestCharacteristicUUID_GYRO_Z.uuidString:
                    guard let characteristicData = characteristic.value else { return }
                    let byteArray = [UInt8](characteristicData)
                    var value = 0
                    for element in byteArray {
                        value <<= 8 // Shift left by 8 bits (equivalent to *= 256)
                        value |= Int(element)
                    }

                    if value > Int16.max {
                        // If the most significant bit (sign bit) is set, the value is negative
                        value -= Int(1 << 16)

                    }

                    let valueDouble = Double(value) / 4096.0
                    chest_Gyro_Z = valueDouble
                    print("Chest GYRO Z: \(valueDouble)")
                    lastDataUpdateTime = Date()
                    lastDataUpdateTime_chest = lastDataUpdateTime
                case chestCharacteristicUUID_EMG.uuidString:
                    guard let characteristicData = characteristic.value else { return }
                    let byteArray = [UInt8](characteristicData)
                    var value = 0
                    for element in byteArray {
                        value <<= 8 // Shift left by 8 bits (equivalent to *= 256)
                        value |= Int(element)
                    }

                    if value > Int16.max {
                        // If the most significant bit (sign bit) is set, the value is negative
                        value -= Int(1 << 16)

                    }

                    let valueDouble = Double(value) / 4096.0
                    chest_EMG = valueDouble * 3.3
                    print("Chest EMG: \(valueDouble)")
                    lastDataUpdateTime = Date()
                    lastDataUpdateTime_chest = lastDataUpdateTime
                    // Finapress Characteristics
                case finapressCharacteristicUUID_start_stop.uuidString:
                    guard let characteristicData = characteristic.value else { return }
                    let byteArray = [UInt8](characteristicData)
                    var value = 0
                    for element in byteArray {
                        value <<= 8 // Shift left by 8 bits (equivalent to *= 256)
                        value |= Int(element)
                    }

                    if value > Int16.max {
                        // If the most significant bit (sign bit) is set, the value is negative
                        value -= Int(1 << 16)

                    }
                    finapress_Start_Stop = value
                    print("FINAPRESS START STOP: \(value)")
                    lastDataUpdateTime = Date()
                    lastDataUpdateTime_fina = lastDataUpdateTime
                case finapressCharacteristicUUID_ADC_A0.uuidString:
                    guard let characteristicData = characteristic.value else { return }
                    let byteArray = [UInt8](characteristicData)
                    var value = 0
                    for element in byteArray {
                        value <<= 8 // Shift left by 8 bits (equivalent to *= 256)
                        value |= Int(element)
                    }

                    if value > Int16.max {
                        // If the most significant bit (sign bit) is set, the value is negative
                        value -= Int(1 << 16)

                    }

                    let valueDouble = Double(value) / 4096.0
                    finapress_ADC_A0 = valueDouble
                    print("FINAPRESS ADC A0: \(valueDouble)")
                    lastDataUpdateTime = Date()
                    lastDataUpdateTime_fina = lastDataUpdateTime
                case finapressCharacteristicUUID_ADC_A1.uuidString:
                    guard let characteristicData = characteristic.value else { return }
                    let byteArray = [UInt8](characteristicData)
                    var value = 0
                    for element in byteArray {
                        value <<= 8 // Shift left by 8 bits (equivalent to *= 256)
                        value |= Int(element)
                    }

                    if value > Int16.max {
                        // If the most significant bit (sign bit) is set, the value is negative
                        value -= Int(1 << 16)

                    }
                    
                    let valueDouble = Double(value) / 4096.0
                    finapress_ADC_A1 = valueDouble
                    print("FINAPRESS ADC A1: \(valueDouble)")
                    lastDataUpdateTime = Date()
                    lastDataUpdateTime_fina = lastDataUpdateTime
                case finapressCharacteristicUUID_ADC_A2.uuidString:
                    guard let characteristicData = characteristic.value else { return }
                    let byteArray = [UInt8](characteristicData)
                    var value = 0
                    for element in byteArray {
                        value <<= 8 // Shift left by 8 bits (equivalent to *= 256)
                        value |= Int(element)
                    }

                    if value > Int16.max {
                        // If the most significant bit (sign bit) is set, the value is negative
                        value -= Int(1 << 16)

                    }

                    let valueDouble = Double(value) / 4096.0
                    finapress_ADC_A2 = valueDouble
                    print("FINAPRESS ADC A2: \(valueDouble)")
                    lastDataUpdateTime = Date()
                    lastDataUpdateTime_fina = lastDataUpdateTime
                case finapressCharacteristicUUID_ADC_A3.uuidString:
                    guard let characteristicData = characteristic.value else { return }
                    let byteArray = [UInt8](characteristicData)
                    var value = 0
                    for element in byteArray {
                        value <<= 8 // Shift left by 8 bits (equivalent to *= 256)
                        value |= Int(element)
                    }

                    if value > Int16.max {
                        // If the most significant bit (sign bit) is set, the value is negative
                        value -= Int(1 << 16)

                    }

                    let valueDouble = Double(value) / 4096.0
                    finapress_ADC_A3 = valueDouble
                    print("FINAPRESS ADC A3: \(valueDouble)")
                    lastDataUpdateTime = Date()
                    lastDataUpdateTime_fina = lastDataUpdateTime
                case finapressCharacteristicUUID_ADC_A4.uuidString:
                    guard let characteristicData = characteristic.value else { return }
                    let byteArray = [UInt8](characteristicData)
                    var value = 0
                    for element in byteArray {
                        value <<= 8 // Shift left by 8 bits (equivalent to *= 256)
                        value |= Int(element)
                    }

                    if value > Int16.max {
                        // If the most significant bit (sign bit) is set, the value is negative
                        value -= Int(1 << 16)

                    }

                    let valueDouble = Double(value) / 4096.0
                    finapress_ADC_A4 = valueDouble
                    print("FINAPRESS ADC A4: \(valueDouble)")
                    lastDataUpdateTime = Date()
                    lastDataUpdateTime_fina = lastDataUpdateTime
                case finapressCharacteristicUUID_ADC_A5.uuidString:
                    guard let characteristicData = characteristic.value else { return }
                    let byteArray = [UInt8](characteristicData)
                    var value = 0
                    for element in byteArray {
                        value <<= 8 // Shift left by 8 bits (equivalent to *= 256)
                        value |= Int(element)
                    }

                    if value > Int16.max {
                        // If the most significant bit (sign bit) is set, the value is negative
                        value -= Int(1 << 16)

                    }

                    let valueDouble = Double(value) / 4096.0
                    finapress_ADC_A5 = valueDouble
                    print("FINAPRESS ADC A5: \(valueDouble)")
                    lastDataUpdateTime = Date()
                    lastDataUpdateTime_fina = lastDataUpdateTime
                case finapressCharacteristicUUID_ADC_A6.uuidString:
                    guard let characteristicData = characteristic.value else { return }
                    let byteArray = [UInt8](characteristicData)
                    var value = 0
                    for element in byteArray {
                        value <<= 8 // Shift left by 8 bits (equivalent to *= 256)
                        value |= Int(element)
                    }

                    if value > Int16.max {
                        // If the most significant bit (sign bit) is set, the value is negative
                        value -= Int(1 << 16)

                    }

                    let valueDouble = Double(value) / 4096.0
                    finapress_ADC_A6 = valueDouble
                    print("FINAPRESS ADC A6: \(valueDouble)")
                    lastDataUpdateTime = Date()
                    lastDataUpdateTime_fina = lastDataUpdateTime
                case finapressCharacteristicUUID_ADC_A7.uuidString:
                    guard let characteristicData = characteristic.value else { return }
                    let byteArray = [UInt8](characteristicData)
                    var value = 0
                    for element in byteArray {
                        value <<= 8 // Shift left by 8 bits (equivalent to *= 256)
                        value |= Int(element)
                    }

                    if value > Int16.max {
                        // If the most significant bit (sign bit) is set, the value is negative
                        value -= Int(1 << 16)

                    }

                    let valueDouble = Double(value) / 4096.0
                    finapress_ADC_A7 = valueDouble
                    print("FINAPRESS ADC A7: \(valueDouble)")
                    lastDataUpdateTime = Date()
                    lastDataUpdateTime_fina = lastDataUpdateTime
                    
                    
                default:
                    // Handle other characteristics or ignore them if not needed
                    break
                }
            }
        }
        
        func sendNumericValueToPeripheral(withSensorType sensorType: RTSensorType, value: UInt8) {
            // Find the peripheral that matches the sensorType
            if let sensor = sensors.first(where: { $0.type == sensorType }) {
                // Extract the CBPeripheral from the sensor
                let peripheral = sensor.peripheral
                
                // Now, you can send the numeric value to the peripheral using the modified method
                sendNumericValueToCharacteristic(peripheral, characteristicUUID: finapressCharacteristicUUID_tx, value: value)
            }
        }


        func sendNumericValueToCharacteristic(_ peripheral: CBPeripheral, characteristicUUID: CBUUID, value: UInt8) {
            // Convert the numeric value to Data
            let dataToSend = Data([value])
            
            // Get the characteristic
            if let characteristic = peripheral.services?.first?.characteristics?.first(where: { $0.uuid == characteristicUUID }) {
                // Write the data to the characteristic
                peripheral.writeValue(dataToSend, for: characteristic, type: .withoutResponse) // or .withoutResponse if no response is needed
            }
        }


        func sendStringToCharacteristic(_ peripheral: CBPeripheral, characteristicUUID: CBUUID, message: String) {
            // Convert the string to Data
            if let dataToSend = message.data(using: .utf8) {
                // Get the characteristic
                if let characteristic = peripheral.services?.first?.characteristics?.first(where: { $0.uuid == characteristicUUID }) {
                    // Write the data to the characteristic
                    peripheral.writeValue(dataToSend, for: characteristic, type: .withoutResponse) // or .withoutResponse if no response is needed
                }
            }
        }

        func sendStringToPeripheral(withSensorType sensorType: RTSensorType, message: String) {
            // Find the peripheral that matches the sensorType
            if let sensor = sensors.first(where: { $0.type == sensorType }) {
                // Extract the CBPeripheral from the sensor
                let peripheral = sensor.peripheral
                
                // Now, you can send data to the peripheral using your existing method
                sendStringToCharacteristic(peripheral, characteristicUUID: finapressCharacteristicUUID_tx, message: message)
            }
        }
        func sendStopCommand() {
            sendNumericValueToPeripheral(withSensorType: .finapress, value: 0)
            print("Stop command sent (0)")
        }
        //func sendStartCommand() {
        //    sendNumericValueToPeripheral(withSensorType: .finapress, value: 1)
        //    print("Start command sent (1)")
        //}
        
        //func sendStartCommand() {
        //    // Check if all sensors have sent data within a certain time threshold
        //    let currentTime = Date()
        //    let threshold: TimeInterval = 5.0 // Set an appropriate time threshold (in seconds)

        //    while !(isSensorReady(lastDataUpdateTime_chest, currentTime, threshold) &&
        //            isSensorReady(lastDataUpdateTime_fina, currentTime, threshold)) {
        //        // Delay for a short duration before checking again
        //        usleep(100_000)  // Sleep for 0.1 second (adjust the duration as needed)
        //    }

        //    // All sensors are ready, send the start command
        //    sendNumericValueToPeripheral(withSensorType: .finapress, value: 1)
        //    print("Start command sent (1)")
        //}

        //// Function to check if a sensor is ready based on last update time
        //func isSensorReady(_ lastUpdateTime: Date?, _ currentTime: Date, _ threshold: TimeInterval) -> Bool {
        //    guard let lastUpdateTime = lastUpdateTime else {
        //        return false // If lastUpdateTime is nil, the sensor is not ready
        //    }
        //
        //    return currentTime.timeIntervalSince(lastUpdateTime) <= threshold
        //}

        func sendStartCommand(){
            sendNumericValueToPeripheral(withSensorType: .finapress, value: 1)
            print("Test start")
        }
        //func sendStopCommand(){
        //    sendStringToPeripheral(withSensorType: .finapress, message: "stop")
        //}
        //
    }

