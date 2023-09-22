//
//  SensorConnectionView.swift
//  TheBlueApp
//
//  Created by Max Gulda on 2023-08-27.
//
import SwiftUI

struct SensorConnectionView: View {
    
    //@EnvironmentObject var bleController: BLEController
    @EnvironmentObject var bleController: BLEController
    var body: some View {
        ZStack {
            
            Color("ThemeColor")
                .edgesIgnoringSafeArea(.all)
            VStack {
                
                Spacer()
                Image(systemName: "figure.walk.circle.fill")
                    .foregroundColor(.white)
                    .font(.system(size: 120))
                Text("Connect to all the sensors before starting your measurement")
                    .fontWeight(.bold)
                    .font(.title2)
                    .foregroundColor(.white)
                    .multilineTextAlignment(.center)
                Spacer()
                
                SensorConnectView(
                    imageName: "doc.text.below.ecg",
                    dataName: "Finapress",
                    buttonText: "Connect",
                    buttonAction: {
                        bleController.connectToSensor(type: .finapress)
                    },
                    isConnected: bleController.sensors.first(where: { $0.type == .finapress })?.isConnected ?? false
                )

                SensorConnectView(
                    imageName: "figure.walk.arrival",
                    dataName: "Left leg",
                    buttonText: "Connect",
                    buttonAction: {
                        bleController.connectToSensor(type: .left_leg)
                    },
                    isConnected: bleController.sensors.first(where: { $0.type == .left_leg })?.isConnected ?? false
                )

                SensorConnectView(
                    imageName: "figure.walk.arrival",
                    dataName: "Right leg",
                    buttonText: "Connect",
                    buttonAction: {
                        bleController.connectToSensor(type: .right_leg)
                    },
                    isConnected: bleController.sensors.first(where: { $0.type == .right_leg })?.isConnected ?? false
                )

                SensorConnectView(
                    imageName: "bolt.heart.fill",
                    dataName: "Chest",
                    buttonText: "Connect",
                    buttonAction: {
                        bleController.connectToSensor(type: .chest)
                    },
                    isConnected: bleController.sensors.first(where: { $0.type == .chest })?.isConnected ?? false
                )

                
                Spacer()
                
                NavigationLink(destination: SensorsDataView()) {
                    MainButton(text: "Start Measurement")
                }.simultaneousGesture(TapGesture().onEnded{
                    bleController.sendStartCommand()
                })
                .navigationBarTitle("Configuration")
                
            }
        }
    }
}

struct SensorConnectionView_Previews: PreviewProvider {
    static var previews: some View {
        SensorConnectionView()
    }
}
