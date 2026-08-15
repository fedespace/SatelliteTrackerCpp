//
//  PassViewModel.swift
//  SatelliteTracker
//
//  Created by Federica Lombardo on 02/08/2026.
//

import SwiftUI


@Observable
class PassViewModel {
    var passes: [Pass] = []
    var aosDates: [Date?] {
        let formatter = DateFormatter()
        formatter.dateFormat = "yyyy-M-d H:m:s"
        formatter.timeZone = .current
        return passes.map { formatter.date(from: String($0.aos.split(separator: ".0")[0])) }
    }
    var losDates: [Date?] {
        let formatter = DateFormatter()
        formatter.dateFormat = "yyyy-M-d H:m:s"
        formatter.timeZone = .current
        return passes.map { formatter.date(from: String($0.los.split(separator: ".0")[0])) }
    }
    var inputType: InputOptions? = .name
    var searchItem: String? = ""
    
    
    func fetchPasses(satellite: String, startTime: Date, endTime: Date, gsLat: String, gsLon: String, gsAlt: String, gsMask: String, inputType: InputOptions, searchItem: String) async {
        let inputBinding = Binding<InputOptions?>(
                    get: { self.inputType },
                    set: { self.inputType = $0 }
                )
                
                let searchBinding = Binding<String?>(
                    get: { self.searchItem },
                    set: { self.searchItem = $0 }
                )
        let pService = PassService(inputType: inputBinding, searchItem: searchBinding)
        do {
            passes = try await pService.fetchPasses(satellite: satellite, startTime: startTime, endTime: endTime, gsLat: gsLat, gsLon: gsLon, gsAlt: gsAlt, gsMask: gsMask, inputType: inputType, searchItem: searchItem)
            print(passes)
        } catch {
            let errorMess = error.localizedDescription
            print("Error message: \(errorMess)")
        }
    }
}
