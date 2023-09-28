//
//  CustomViews.swift
//  TheBlueApp
//
//  Created by Max Gulda on 2023-08-27.
//
import SwiftUI

struct MainButton: View {
    
    var text: String
    
    var body: some View {
        Text(text)
            .font(.title2)
            .padding()
            .foregroundColor(.black)
            .background(
                RoundedRectangle(cornerRadius: 25)
                    .stroke(Color.white, lineWidth: 2)
                    .background(Color.white.cornerRadius(25))
            )
    }
}

struct ConnectionButton: ButtonStyle {
    var isConnected: Bool

    func makeBody(configuration: Configuration) -> some View {
        configuration.label
            .padding(8)
            .background(isConnected ? Color.green : Color.red)
            .foregroundColor(.white)
            .clipShape(Capsule())
    }
}


struct SensorConnectView: View {
    var imageName: String
    var dataName: String
    var buttonText: String
    var buttonAction: () -> Void
    var isConnected: Bool

    var body: some View {
        HStack {
            Image(systemName: imageName)
                .foregroundColor(.white)
                .font(.title2)
                .frame(width: 32)
            Text(dataName)
                .textCase(.uppercase)
                .foregroundColor(.white)
                .font(.title3)
                .frame(width: 196, alignment: .leading)
            Button(buttonText) {
                buttonAction()
            }
            .buttonStyle(ConnectionButton(isConnected: isConnected))
        }
    }
}




