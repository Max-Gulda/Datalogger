import SwiftUI

struct SensorDataView: View {
    let title: String
    let dataValue: String
    
    var body: some View {
        HStack {
            VStack(alignment: .leading) {
                Text(title)
                    .font(.headline)
                Text(dataValue)
                    .font(.subheadline)
            }
            Spacer()
        }
        .padding(10)
        .background(Color.white)
        .cornerRadius(10)
        .padding(.horizontal, 20)
        .shadow(radius: 5)
    }
}

struct SensorsDataView: View {
    @EnvironmentObject var bleController: BLEController
    @State private var isStopping = false
    @State private var previousSensorData: [(String, String)] = [] // Store previous sensor data
    let dataStopDuration: TimeInterval = 2
    
    var body: some View {
        ZStack {
            Color("ThemeColor")
                .edgesIgnoringSafeArea(.all)
            
            ScrollView {
                VStack(spacing: 20) { // Add spacing between SensorDataViews
                    Spacer()
                    Image(systemName: "figure.walk.circle.fill")
                        .foregroundColor(.white)
                        .font(.system(size: 120))
                    Spacer()
                    
                    let sensorData = [  ("Left Leg ACC X" , "\(bleController.leftLeg_ACC_X) g"),
                                        ("Left Leg ACC Y" , "\(bleController.leftLeg_ACC_Y) g"),
                                        ("Left Leg ACC Z" , "\(bleController.leftLeg_ACC_Z) g"),
                                        ("Left Leg Gyro X", "\(bleController.leftLeg_Gyro_X * 20000) deg/s"),
                                        ("Left Leg Gyro Y", "\(bleController.leftLeg_Gyro_Y * 20000) deg/s"),
                                        ("Left Leg Gyro Z", "\(bleController.leftLeg_Gyro_Z * 20000) deg/s"),
                                        ("Left Leg EMG"   , "\(bleController.leftLeg_EMG * 3.3) V"),
                                        
                                        ("Right Leg ACC X" , "\(bleController.rightLeg_ACC_X) g"),
                                        ("Right Leg ACC Y" , "\(bleController.rightLeg_ACC_Y) g"),
                                        ("Right Leg ACC Z" , "\(bleController.rightLeg_ACC_Z) g"),
                                        ("Right Leg Gyro X", "\(bleController.rightLeg_Gyro_X * 20000) deg/s"),
                                        ("Right Leg Gyro Y", "\(bleController.rightLeg_Gyro_Y * 20000) deg/s"),
                                        ("Right Leg Gyro Z", "\(bleController.rightLeg_Gyro_Z * 20000) deg/s"),
                                        ("Right Leg EMG"   , "\(bleController.rightLeg_EMG)"),
                                        
                                        ("Chest ACC X" , "\(bleController.chest_ACC_X) g"),
                                        ("Chest ACC Y" , "\(bleController.chest_ACC_Y) g"),
                                        ("Chest ACC Z" , "\(bleController.chest_ACC_Z) g"),
                                        ("Chest Gyro X", "\(bleController.chest_Gyro_X * 20000)deg/s"),
                                        ("Chest Gyro Y", "\(bleController.chest_Gyro_Y * 20000)deg/s"),
                                        ("Chest Gyro Z", "\(bleController.chest_Gyro_Z * 20000)deg/s"),
                                        ("Chest ECG"   , "\(bleController.chest_EMG * 3.3) V"),
                                        
                                        ("Finapress ADC A0", "\(bleController.finapress_ADC_A0)"),
                                        ("Finapress ADC A1", "\(bleController.finapress_ADC_A1)"),
                                        ("Finapress ADC A2", "\(bleController.finapress_ADC_A2)"),
                                        ("Finapress ADC A3", "\(bleController.finapress_ADC_A3)"),
                                        ("Finapress ADC A4", "\(bleController.finapress_ADC_A4)"),
                                        ("Finapress ADC A5", "\(bleController.finapress_ADC_A5)"),
                                        ("Finapress ADC A6", "\(bleController.finapress_ADC_A6)"),
                                        ("Finapress ADC A7", "\(bleController.finapress_ADC_A7)")
                                    ]
                    
                    ForEach(sensorData, id: \.0) { data in
                        SensorDataView(title: data.0, dataValue: data.1)
                    }
                    
                    Spacer()
                    Button(action: {
                        // Start the loop to repeatedly check and send the stop command
                        isStopping = true
                        var initialDataUpdateTime: Date? // Declare it here
                        
                        DispatchQueue.global().async {
                            while isStopping {
                                // Safely unwrap bleController.lastDataUpdateTime
                                if let lastUpdateTime = bleController.lastDataUpdateTime {
                                    initialDataUpdateTime = lastUpdateTime
                                }
                                
                                // Send the stop command
                                DispatchQueue.main.async {
                                    bleController.sendStopCommand()
                                }
                                
                                // Wait for a duration (e.g., 1 second)
                                usleep(1_000_00)  // Sleep for 0.1 second
                                
                                // Check if data has not changed for the specified duration
                                let currentTime = Date()
                                
                                // Safely unwrap initialDataUpdateTime
                                if let initialTime = initialDataUpdateTime,
                                   currentTime.timeIntervalSince(initialTime) >= dataStopDuration {
                                    print("Im stopping now")
                                    isStopping = false // Data has stopped changing, exit the loop
                                    break
                                }
                            }
                        }
                    }) {
                        Text("End Session")
                            .font(.headline)
                            .foregroundColor(.white)
                            .padding()
                            .background(Color.red)
                            .cornerRadius(10)
                    }
                    .disabled(isStopping)
                    
                    if isStopping {
                        ProgressView("Stopping...") // Show the loading indicator while stopping
                            .progressViewStyle(CircularProgressViewStyle(tint: .white))
                            .foregroundColor(.white)
                    }
                    Spacer()
                }
                .background(Color("ThemeColor")) // Set the background color for the VStack
            }
        }
        .navigationTitle("Active Session")
    }
}

struct SensorsDataView_Previews: PreviewProvider {
    static var previews: some View {
        SensorsDataView()
            .environmentObject(BLEController()) // You might need to provide a BLEController instance for the preview
    }
}
