//
//  SensorDataView.swift
//  TheBlueApp
//
//  Created by Max Gulda on 2023-08-27.
//

import SwiftUI

struct SensorsDataView: View {
    
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
                Spacer()
                SensorDataView(imageName: "figure.walk.departure", dataValue: "")
                
                SensorDataView(imageName: "figure.walk.arrival", dataValue: "")
                
                SensorDataView(imageName: "heart.fill", dataValue: "")
                
                SensorDataView(imageName: "doc.text.below.ecg", dataValue: "")
                Spacer()
                Button(action: {
                    // button action
                }) {
                    MainButton(text: "End Session")
                }.simultaneousGesture(TapGesture().onEnded{
                    bleController.sendStopCommand()
                })
            }
        }
        .navigationTitle("Active Session")
    }
}

struct SensorsDataView_Previews: PreviewProvider {
    static var previews: some View {
        SensorsDataView()
    }
}
