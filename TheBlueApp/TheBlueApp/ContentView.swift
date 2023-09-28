//
//  ContentView.swift
//  TheBlueApp
//
//  Created by Max Gulda on 2023-08-26.
//
import CoreBluetooth
import SwiftUI

struct ContentView: View {
    
    @EnvironmentObject var bleController: BLEController
    
    var body: some View {
        NavigationView {
            ZStack {
                Color("ThemeColor")
                    .edgesIgnoringSafeArea(.all)
                VStack {
                    Spacer()
                    HStack(spacing: 0) {
                        Text("Data")
                            .font(.system(size: 52))
                            .fontWeight(.bold)
                            .foregroundColor(.blue)
                        Text("logger")
                            .font(.system(size: 50))
                            .foregroundColor(.white)
                    }
                    
                    Spacer()
                    Image(systemName: "figure.walk.circle.fill")
                        .foregroundColor(.white)
                        .font(.system(size: 120))
                    Spacer()
                    Spacer()
                    NavigationLink(destination: SensorConnectionView()) {
                        MainButton(text: "Get Started")
                    }
                    Spacer()
                }
            }
            
        }
    }
       
}

struct ContentView_Previews: PreviewProvider {
    static var previews: some View {
        ContentView()
    }
}
