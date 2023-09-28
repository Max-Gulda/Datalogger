//
//  TheBlueAppApp.swift
//  TheBlueApp
//
//  Created by Max Gulda on 2023-08-26.
//

import SwiftUI

@main
struct TheBlueAppApp: App {
    // Create your BLEController object here
    let bleController = BLEController()

    var body: some Scene {
        WindowGroup {
            // Inject the BLEController as an environment object
            ContentView()
                .environmentObject(bleController)
        }
    }
}
